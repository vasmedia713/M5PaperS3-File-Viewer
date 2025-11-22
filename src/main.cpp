#include <M5Unified.h>
#include <LittleFS.h>
#include "config.h"
#include "utils.h"
#include "file_browser.h"
#include "markdown_reader.h"
#include "image_viewer.h"
#include "wifi_server.h"

// Global objects
FileBrowser fileBrowser;
MarkdownReader markdownReader;
ImageViewer imageViewer;
WiFiFileServer wifiServer;

// Current app state
AppState currentState = STATE_FILE_BROWSER;
AppState previousState = STATE_FILE_BROWSER;

// WiFi mode toggle
bool wifiEnabled = false;
unsigned long lastButtonCheck = 0;
const unsigned long BUTTON_CHECK_INTERVAL = 100;

// Forward declaration
void toggleWiFi();

void setup() {
    // Initialize Serial FIRST for debugging
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n\n=== M5Paper S3 File Viewer ===");
    Serial.println("Starting initialization...");

    // Initialize M5Unified properly for M5PaperS3
    Serial.println("Initializing M5...");
    auto cfg = M5.config();
    M5.begin(cfg);
    Serial.println("M5.begin() completed");

    // Print display info
    Serial.printf("Display width: %d\n", M5.Display.width());
    Serial.printf("Display height: %d\n", M5.Display.height());

    // Set rotation to portrait mode
    M5.Display.setRotation(0);  // 0 = Portrait (540x960)
    Serial.printf("After rotation: %dx%d\n", M5.Display.width(), M5.Display.height());

    // Clear screen and draw splash
    Serial.println("Drawing splash screen...");
    M5.Display.fillScreen(TFT_WHITE);
    M5.Display.setTextColor(TFT_BLACK);
    M5.Display.setTextSize(4);
    drawCenteredText("M5Paper S3", SCREEN_HEIGHT / 2 - 40);
    M5.Display.setTextSize(3);
    drawCenteredText("File Viewer", SCREEN_HEIGHT / 2 + 20);

    Serial.println("Pushing to display...");
    M5.Display.display();
    Serial.println("Splash screen should now be visible");

    delay(2000);

    // Initialize filesystem
    Serial.println("Initializing filesystem...");
    if (!initFileSystem()) {
        Serial.println("ERROR: Filesystem mount failed!");
        M5.Display.fillScreen(COLOR_BG);
        M5.Display.setTextColor(COLOR_FG);
        drawCenteredText("ERROR: Failed to mount filesystem", SCREEN_HEIGHT / 2);
        M5.Display.display();
        while (1) {
            delay(1000);
        }
    }
    Serial.println("Filesystem mounted successfully");

    // List filesystem contents (debug)
    Serial.println("\nFilesystem contents:");
    File root = LittleFS.open("/");
    File file = root.openNextFile();
    while (file) {
        Serial.printf("  %s (%d bytes)\n", file.name(), file.size());
        file = root.openNextFile();
    }

    // Initialize file browser
    Serial.println("Initializing file browser...");
    fileBrowser.begin();

    // Show initial screen
    Serial.println("Drawing file browser...");
    currentState = STATE_FILE_BROWSER;
    fileBrowser.draw();

    Serial.println("\n=== Initialization complete! ===");
    Serial.println("\nControls:");
    Serial.println("  - Touch to select files");
    Serial.println("  - Swipe left/right to navigate");
    Serial.println("  - Long press power button for menu");
}

void loop() {
    M5.update();

    // Check for WiFi toggle (using touch in top-right corner)
    if (millis() - lastButtonCheck > BUTTON_CHECK_INTERVAL) {
        lastButtonCheck = millis();

        auto touch = M5.Touch.getDetail();
        if (touch.wasPressed()) {
            // Top-right corner = WiFi toggle (adjusted for larger font and header)
            if (touch.x > SCREEN_WIDTH - 150 && touch.y < 80) {
                toggleWiFi();
            }
        }
    }

    // Update WiFi server if running
    if (wifiEnabled) {
        wifiServer.update();
    }

    // State machine
    switch (currentState) {
        case STATE_FILE_BROWSER:
            fileBrowser.update();

            if (fileBrowser.hasSelection()) {
                String selectedFile = fileBrowser.getSelectedFile();
                fileBrowser.clearSelection();

                Serial.println("\n=== FILE SELECTED ===");
                Serial.printf("Selected file: %s\n", selectedFile.c_str());
                Serial.printf("File length: %d\n", selectedFile.length());

                String lowerName = selectedFile;
                lowerName.toLowerCase();

                if (lowerName.endsWith(".md")) {
                    Serial.println("Opening markdown file...");
                    currentState = STATE_MARKDOWN_READER;
                    markdownReader.begin(selectedFile);
                    markdownReader.draw();
                    Serial.println("Markdown reader initialized");
                } else if (lowerName.endsWith(".jpg") || lowerName.endsWith(".jpeg") ||
                           lowerName.endsWith(".png")) {
                    Serial.println("Opening image file...");
                    currentState = STATE_IMAGE_VIEWER;
                    imageViewer.begin(selectedFile);
                    imageViewer.draw();
                    Serial.println("Image viewer initialized");
                } else {
                    Serial.printf("Unknown file type: %s\n", selectedFile.c_str());
                }
            }
            break;

        case STATE_MARKDOWN_READER:
            markdownReader.update();

            if (markdownReader.shouldExit()) {
                currentState = STATE_FILE_BROWSER;
                fileBrowser.begin(); // Refresh file list
                fileBrowser.draw();
            }
            break;

        case STATE_IMAGE_VIEWER:
            imageViewer.update();

            if (imageViewer.shouldExit()) {
                currentState = STATE_FILE_BROWSER;
                fileBrowser.begin(); // Refresh file list
                fileBrowser.draw();
            }
            break;

        case STATE_WIFI_SERVER:
            // WiFi server runs in background
            break;
    }

    delay(50); // Small delay to prevent excessive CPU usage
}

void toggleWiFi() {
    wifiEnabled = !wifiEnabled;

    M5.Display.fillScreen(COLOR_BG);
    M5.Display.setTextColor(COLOR_FG);
    M5.Display.setTextSize(4);

    if (wifiEnabled) {
        Serial.println("Starting WiFi server...");
        drawCenteredText("Starting WiFi...", SCREEN_HEIGHT / 2 - 40);
        M5.Display.display();

        wifiServer.begin();

        delay(1000);

        M5.Display.fillScreen(COLOR_BG);
        M5.Display.setTextSize(3);
        drawCenteredText("WiFi Server Active", 50);

        M5.Display.setTextSize(2);
        drawCenteredText("Connect to WiFi:", 120);

        M5.Display.setTextSize(3);
        drawCenteredText(WIFI_SSID, 160);

        M5.Display.setTextSize(2);
        drawCenteredText("Password:", 220);

        M5.Display.setTextSize(3);
        drawCenteredText(WIFI_PASSWORD, 260);

        M5.Display.setTextSize(2);
        String ipMsg = "Open: http://" + wifiServer.getIPAddress();
        drawCenteredText(ipMsg.c_str(), 330);

        M5.Display.setTextSize(2);
        drawCenteredText("Touch top-right to exit", 400);

        M5.Display.display();

        Serial.println("WiFi server started");
        Serial.printf("SSID: %s\n", WIFI_SSID);
        Serial.printf("Password: %s\n", WIFI_PASSWORD);
        Serial.printf("IP: %s\n", wifiServer.getIPAddress().c_str());

    } else {
        Serial.println("Stopping WiFi server...");
        drawCenteredText("Stopping WiFi...", SCREEN_HEIGHT / 2);
        M5.Display.display();

        wifiServer.stop();

        delay(1000);

        // Return to file browser
        currentState = STATE_FILE_BROWSER;
        fileBrowser.begin();
        fileBrowser.draw();

        Serial.println("WiFi server stopped");
    }
}
