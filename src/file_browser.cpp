#include "file_browser.h"
#include "utils.h"

FileBrowser::FileBrowser()
    : selectedIndex(0), scrollOffset(0), fileSelected(false) {
}

void FileBrowser::begin() {
    loadFiles();
    selectedIndex = 0;
    scrollOffset = 0;
    fileSelected = false;
    selectedFile = "";
}

void FileBrowser::loadFiles() {
    mdFiles.clear();
    imageFiles.clear();

    File root = LittleFS.open("/files");
    if (!root || !root.isDirectory()) {
        Serial.println("Failed to open /files directory");
        return;
    }

    File file = root.openNextFile();
    while (file) {
        if (!file.isDirectory()) {
            String filename = String(file.name());
            String lowerName = filename;
            lowerName.toLowerCase();

            if (lowerName.endsWith(".md")) {
                mdFiles.push_back(filename);
            } else if (lowerName.endsWith(".jpg") || lowerName.endsWith(".jpeg") ||
                       lowerName.endsWith(".png")) {
                imageFiles.push_back(filename);
            }
        }
        file = root.openNextFile();
    }

    Serial.printf("Found %d markdown files and %d images\n", mdFiles.size(), imageFiles.size());
}

void FileBrowser::draw() {
    M5.Display.fillScreen(COLOR_BG);
    M5.Display.setTextSize(1);
    drawHeader();
    drawFileList();
    M5.Display.display();
}

void FileBrowser::drawHeader() {
    M5.Display.fillRect(0, 0, SCREEN_WIDTH, HEADER_HEIGHT, COLOR_FG);
    M5.Display.setTextColor(COLOR_BG);
    M5.Display.setTextSize(2);
    M5.Display.setCursor(20, 20);
    M5.Display.print("File Browser");

    // Draw WiFi button in top-right corner
    M5.Display.setTextSize(2);
    M5.Display.setCursor(SCREEN_WIDTH - 90, 20);
    M5.Display.print("WiFi");

    M5.Display.setTextSize(1);
    M5.Display.setTextColor(COLOR_FG);
}

void FileBrowser::drawFileList() {
    int y = HEADER_HEIGHT + 10;
    int totalFiles = mdFiles.size() + imageFiles.size();

    if (totalFiles == 0) {
        M5.Display.setTextColor(COLOR_FG);
        M5.Display.setTextSize(3);
        drawCenteredText("No files found", SCREEN_HEIGHT / 2 - 30);
        M5.Display.setTextSize(2);
        drawCenteredText("Touch WiFi button", SCREEN_HEIGHT / 2 + 20);
        drawCenteredText("to upload files", SCREEN_HEIGHT / 2 + 50);
        M5.Display.setTextSize(1);
        return;
    }

    // Draw section: Documents
    if (mdFiles.size() > 0) {
        M5.Display.setTextColor(COLOR_GRAY);
        M5.Display.setCursor(20, y);
        M5.Display.print("DOCUMENTS");
        y += 25;

        for (size_t i = 0; i < mdFiles.size() && y < SCREEN_HEIGHT - 20; i++) {
            bool isSelected = (selectedIndex == (int)i);

            if (isSelected) {
                M5.Display.fillRect(10, y - 5, SCREEN_WIDTH - 20, ITEM_HEIGHT, COLOR_GRAY);
                M5.Display.setTextColor(COLOR_BG);
            } else {
                M5.Display.setTextColor(COLOR_FG);
            }

            M5.Display.setCursor(30, y + 10);
            String displayName = mdFiles[i];
            if (displayName.startsWith("/files/")) {
                displayName = displayName.substring(7);
            }
            M5.Display.print(displayName);

            y += ITEM_HEIGHT;
        }
    }

    // Draw section: Images
    if (imageFiles.size() > 0 && y < SCREEN_HEIGHT - 60) {
        y += 10;
        M5.Display.setTextColor(COLOR_GRAY);
        M5.Display.setCursor(20, y);
        M5.Display.print("IMAGES");
        y += 25;

        for (size_t i = 0; i < imageFiles.size() && y < SCREEN_HEIGHT - 20; i++) {
            bool isSelected = (selectedIndex == (int)(mdFiles.size() + i));

            if (isSelected) {
                M5.Display.fillRect(10, y - 5, SCREEN_WIDTH - 20, ITEM_HEIGHT, COLOR_GRAY);
                M5.Display.setTextColor(COLOR_BG);
            } else {
                M5.Display.setTextColor(COLOR_FG);
            }

            M5.Display.setCursor(30, y + 10);
            String displayName = imageFiles[i];
            if (displayName.startsWith("/files/")) {
                displayName = displayName.substring(7);
            }
            M5.Display.print(displayName);

            y += ITEM_HEIGHT;
        }
    }
}

void FileBrowser::update() {
    handleTouch();
    handleButtons();
}

void FileBrowser::handleTouch() {
    auto touch = M5.Touch.getDetail();

    if (touch.wasPressed()) {
        int touchY = touch.y;
        int index = getTouchIndex(touchY);

        if (index >= 0 && index < (int)(mdFiles.size() + imageFiles.size())) {
            selectedIndex = index;
            draw();

            // Wait for release
            delay(200);

            // Check if still on same item (tap vs drag)
            auto touchRelease = M5.Touch.getDetail();
            if (!touchRelease.isPressed() && abs(touchRelease.y - touchY) < TAP_THRESHOLD) {
                // Select the file
                if (selectedIndex < (int)mdFiles.size()) {
                    selectedFile = mdFiles[selectedIndex];
                } else {
                    selectedFile = imageFiles[selectedIndex - mdFiles.size()];
                }
                fileSelected = true;
                Serial.printf("Selected file: %s\n", selectedFile.c_str());
            }
        }
    }
}

void FileBrowser::handleButtons() {
    // M5Paper S3 uses touch primarily, but we can add navigation with touches
    // Physical buttons would be handled here if available
}

int FileBrowser::getTouchIndex(int touchY) {
    int y = HEADER_HEIGHT + 35; // Start after "DOCUMENTS" header
    int totalFiles = mdFiles.size() + imageFiles.size();

    for (int i = 0; i < totalFiles; i++) {
        if (i == (int)mdFiles.size()) {
            y += 35; // Skip "IMAGES" header
        }

        if (touchY >= y - 5 && touchY <= y + ITEM_HEIGHT - 5) {
            return i;
        }

        y += ITEM_HEIGHT;
    }

    return -1;
}

String FileBrowser::getSelectedFile() {
    return selectedFile;
}

bool FileBrowser::hasSelection() {
    return fileSelected;
}

void FileBrowser::clearSelection() {
    fileSelected = false;
    selectedFile = "";
}
