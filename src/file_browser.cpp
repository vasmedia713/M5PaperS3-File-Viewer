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
    textFiles.clear();
    imageFiles.clear();
    cbzFiles.clear();

    Serial.println("\n=== SCANNING FILES ===");
    File root = LittleFS.open("/files");
    if (!root || !root.isDirectory()) {
        Serial.println("Failed to open /files directory");
        return;
    }

    File file = root.openNextFile();
    while (file) {
        if (!file.isDirectory()) {
            String filename = String(file.name());
            Serial.printf("Found file: %s\n", filename.c_str());

            String lowerName = filename;
            lowerName.toLowerCase();

            if (lowerName.endsWith(".md")) {
                mdFiles.push_back(filename);
                Serial.printf("  -> Added to mdFiles\n");
            } else if (lowerName.endsWith(".txt")) {
                textFiles.push_back(filename);
                Serial.printf("  -> Added to textFiles\n");
            } else if (lowerName.endsWith(".jpg") || lowerName.endsWith(".jpeg") ||
                       lowerName.endsWith(".png") || lowerName.endsWith(".webp")) {
                imageFiles.push_back(filename);
                Serial.printf("  -> Added to imageFiles\n");
            } else if (lowerName.endsWith(".cbz")) {
                cbzFiles.push_back(filename);
                Serial.printf("  -> Added to cbzFiles\n");
            }
        }
        file = root.openNextFile();
    }

    Serial.printf("\nTotal: %d markdown files, %d text files, %d images, and %d CBZ files\n", mdFiles.size(), textFiles.size(), imageFiles.size(), cbzFiles.size());

    // Print all files
    if (mdFiles.size() > 0) {
        Serial.println("\nMarkdown files:");
        for (size_t i = 0; i < mdFiles.size(); i++) {
            Serial.printf("  [%d] %s\n", i, mdFiles[i].c_str());
        }
    }
    if (textFiles.size() > 0) {
        Serial.println("\nText files:");
        for (size_t i = 0; i < textFiles.size(); i++) {
            Serial.printf("  [%d] %s\n", i, textFiles[i].c_str());
        }
    }
    if (imageFiles.size() > 0) {
        Serial.println("\nImage files:");
        for (size_t i = 0; i < imageFiles.size(); i++) {
            Serial.printf("  [%d] %s\n", i, imageFiles[i].c_str());
        }
    }
    if (cbzFiles.size() > 0) {
        Serial.println("\nCBZ files:");
        for (size_t i = 0; i < cbzFiles.size(); i++) {
            Serial.printf("  [%d] %s\n", i, cbzFiles[i].c_str());
        }
    }
    Serial.println("===================\n");
}

void FileBrowser::draw() {
    M5.Display.fillScreen(COLOR_BG);
    M5.Display.setTextSize(4);
    drawHeader();
    drawFileList();
    M5.Display.display();
}

void FileBrowser::drawHeader() {
    M5.Display.fillRect(0, 0, SCREEN_WIDTH, HEADER_HEIGHT, COLOR_FG);
    M5.Display.setTextColor(COLOR_BG);
    M5.Display.setTextSize(4);
    M5.Display.setCursor(20, 20);
    M5.Display.print("OmniBrowser v.0");

    // Draw WiFi button in top-right corner
    M5.Display.setTextSize(4);
    // Calculate position to fit "WiFi" text (font size 4 = ~24 pixels per char, 4 chars = ~96px)
    int wifiWidth = M5.Display.textWidth("WiFi");
    M5.Display.setCursor(SCREEN_WIDTH - wifiWidth - 10, 20);  // 10px margin from right
    M5.Display.print("WiFi");

    M5.Display.setTextSize(4);
    M5.Display.setTextColor(COLOR_FG);
}

String FileBrowser::truncateFilename(String filename) {
    // Remove /files/ prefix if present
    if (filename.startsWith("/files/")) {
        filename = filename.substring(7);
    }

    // Truncate if too long and add ellipsis
    if (filename.length() > MAX_FILENAME_CHARS) {
        filename = filename.substring(0, MAX_FILENAME_CHARS - 3) + "...";
    }

    return filename;
}

void FileBrowser::drawFileList() {
    int y = HEADER_HEIGHT + 20;  // Start with more padding from header
    int totalFiles = mdFiles.size() + textFiles.size() + imageFiles.size() + cbzFiles.size();

    if (totalFiles == 0) {
        M5.Display.setTextColor(COLOR_FG);
        M5.Display.setTextSize(3);
        drawCenteredText("No files found", SCREEN_HEIGHT / 2 - 80);
        M5.Display.setTextSize(2);
        drawCenteredText("Touch WiFi to upload", SCREEN_HEIGHT / 2);
        return;
    }

    // Draw section: Documents
    if (mdFiles.size() > 0) {
        M5.Display.setTextColor(COLOR_GRAY);
        M5.Display.setTextSize(2);
        M5.Display.setCursor(20, y);
        M5.Display.print("DOCUMENTS");
        y += 45;  // Space after section header

        for (size_t i = 0; i < mdFiles.size() && y + ITEM_HEIGHT < SCREEN_HEIGHT; i++) {
            bool isSelected = (selectedIndex == (int)i);

            // Draw selection background
            if (isSelected) {
                M5.Display.fillRect(10, y, SCREEN_WIDTH - 20, ITEM_HEIGHT - 10, COLOR_GRAY);
                M5.Display.setTextColor(COLOR_BG);
            } else {
                M5.Display.setTextColor(COLOR_FG);
            }

            // Draw filename with truncation
            M5.Display.setTextSize(3);
            M5.Display.setCursor(30, y + 25);  // Vertically centered in item
            String displayName = truncateFilename(mdFiles[i]);
            M5.Display.print(displayName);

            y += ITEM_HEIGHT;
        }
    }

    // Draw section: Text Files
    if (textFiles.size() > 0 && y + ITEM_HEIGHT < SCREEN_HEIGHT) {
        y += 10;  // Small gap between sections
        M5.Display.setTextColor(COLOR_GRAY);
        M5.Display.setTextSize(2);
        M5.Display.setCursor(20, y);
        M5.Display.print("TEXT");
        y += 45;  // Space after section header

        for (size_t i = 0; i < textFiles.size() && y + ITEM_HEIGHT < SCREEN_HEIGHT; i++) {
            bool isSelected = (selectedIndex == (int)(mdFiles.size() + i));

            // Draw selection background
            if (isSelected) {
                M5.Display.fillRect(10, y, SCREEN_WIDTH - 20, ITEM_HEIGHT - 10, COLOR_GRAY);
                M5.Display.setTextColor(COLOR_BG);
            } else {
                M5.Display.setTextColor(COLOR_FG);
            }

            // Draw filename with truncation
            M5.Display.setTextSize(3);
            M5.Display.setCursor(30, y + 25);  // Vertically centered in item
            String displayName = truncateFilename(textFiles[i]);
            M5.Display.print(displayName);

            y += ITEM_HEIGHT;
        }
    }

    // Draw section: Images
    if (imageFiles.size() > 0 && y + ITEM_HEIGHT < SCREEN_HEIGHT) {
        y += 10;  // Small gap between sections
        M5.Display.setTextColor(COLOR_GRAY);
        M5.Display.setTextSize(2);
        M5.Display.setCursor(20, y);
        M5.Display.print("IMAGES");
        y += 45;  // Space after section header

        for (size_t i = 0; i < imageFiles.size() && y + ITEM_HEIGHT < SCREEN_HEIGHT; i++) {
            bool isSelected = (selectedIndex == (int)(mdFiles.size() + textFiles.size() + i));

            // Draw selection background
            if (isSelected) {
                M5.Display.fillRect(10, y, SCREEN_WIDTH - 20, ITEM_HEIGHT - 10, COLOR_GRAY);
                M5.Display.setTextColor(COLOR_BG);
            } else {
                M5.Display.setTextColor(COLOR_FG);
            }

            // Draw filename with truncation
            M5.Display.setTextSize(3);
            M5.Display.setCursor(30, y + 25);  // Vertically centered in item
            String displayName = truncateFilename(imageFiles[i]);
            M5.Display.print(displayName);

            y += ITEM_HEIGHT;
        }
    }

    // Draw section: Comics (CBZ)
    if (cbzFiles.size() > 0 && y + ITEM_HEIGHT < SCREEN_HEIGHT) {
        y += 10;  // Small gap between sections
        M5.Display.setTextColor(COLOR_GRAY);
        M5.Display.setTextSize(2);
        M5.Display.setCursor(20, y);
        M5.Display.print("COMICS");
        y += 45;  // Space after section header

        for (size_t i = 0; i < cbzFiles.size() && y + ITEM_HEIGHT < SCREEN_HEIGHT; i++) {
            bool isSelected = (selectedIndex == (int)(mdFiles.size() + textFiles.size() + imageFiles.size() + i));

            // Draw selection background
            if (isSelected) {
                M5.Display.fillRect(10, y, SCREEN_WIDTH - 20, ITEM_HEIGHT - 10, COLOR_GRAY);
                M5.Display.setTextColor(COLOR_BG);
            } else {
                M5.Display.setTextColor(COLOR_FG);
            }

            // Draw filename with truncation
            M5.Display.setTextSize(3);
            M5.Display.setCursor(30, y + 25);  // Vertically centered in item
            String displayName = truncateFilename(cbzFiles[i]);
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
        Serial.printf("Touch at Y: %d\n", touchY);

        int index = getTouchIndex(touchY);
        Serial.printf("Touch index: %d (total files: %d)\n", index, mdFiles.size() + textFiles.size() + imageFiles.size() + cbzFiles.size());

        if (index >= 0 && index < (int)(mdFiles.size() + textFiles.size() + imageFiles.size() + cbzFiles.size())) {
            selectedIndex = index;
            draw();

            // Wait for touch release - actively poll until released
            Serial.println("Waiting for touch release...");
            unsigned long startWait = millis();
            int releaseY = touchY;

            while (millis() - startWait < 1000) {  // Max 1 second wait
                M5.update();
                auto currentTouch = M5.Touch.getDetail();

                if (currentTouch.isPressed()) {
                    releaseY = currentTouch.y;  // Track current position
                } else {
                    // Touch released
                    break;
                }
                delay(10);
            }

            int deltaY = abs(releaseY - touchY);
            Serial.printf("Touch released. Movement: %d pixels (threshold: %d)\n", deltaY, TAP_THRESHOLD);

            // If movement is small, it's a tap
            if (deltaY < TAP_THRESHOLD) {
                // Select the file
                if (selectedIndex < (int)mdFiles.size()) {
                    selectedFile = mdFiles[selectedIndex];
                    Serial.printf("✓✓✓ SELECTED MARKDOWN [%d]: %s\n", selectedIndex, selectedFile.c_str());
                } else if (selectedIndex < (int)(mdFiles.size() + textFiles.size())) {
                    selectedFile = textFiles[selectedIndex - mdFiles.size()];
                    Serial.printf("✓✓✓ SELECTED TEXT [%d]: %s\n", selectedIndex - mdFiles.size(), selectedFile.c_str());
                } else if (selectedIndex < (int)(mdFiles.size() + textFiles.size() + imageFiles.size())) {
                    selectedFile = imageFiles[selectedIndex - mdFiles.size() - textFiles.size()];
                    Serial.printf("✓✓✓ SELECTED IMAGE [%d]: %s\n", selectedIndex - mdFiles.size() - textFiles.size(), selectedFile.c_str());
                } else {
                    selectedFile = cbzFiles[selectedIndex - mdFiles.size() - textFiles.size() - imageFiles.size()];
                    Serial.printf("✓✓✓ SELECTED CBZ [%d]: %s\n", selectedIndex - mdFiles.size() - textFiles.size() - imageFiles.size(), selectedFile.c_str());
                }
                fileSelected = true;
            } else {
                Serial.printf("Rejected: moved %d pixels\n", deltaY);
            }
        } else {
            Serial.println("Touch outside file area");
        }
    }
}

void FileBrowser::handleButtons() {
    // M5Paper S3 uses touch primarily, but we can add navigation with touches
    // Physical buttons would be handled here if available
}

int FileBrowser::getTouchIndex(int touchY) {
    int y = HEADER_HEIGHT + 20;  // Start position matching drawFileList

    // Skip "DOCUMENTS" header
    y += 45;

    // Check documents section
    for (size_t i = 0; i < mdFiles.size(); i++) {
        // Check if touch is within this item's bounds
        if (touchY >= y && touchY <= y + ITEM_HEIGHT - 10) {
            return i;
        }
        y += ITEM_HEIGHT;
    }

    // Check if we have text files section
    if (textFiles.size() > 0) {
        y += 10;  // Gap between sections
        y += 45;  // Skip "TEXT" header

        // Check text files section
        for (size_t i = 0; i < textFiles.size(); i++) {
            // Check if touch is within this item's bounds
            if (touchY >= y && touchY <= y + ITEM_HEIGHT - 10) {
                return mdFiles.size() + i;
            }
            y += ITEM_HEIGHT;
        }
    }

    // Check if we have images section
    if (imageFiles.size() > 0) {
        y += 10;  // Gap between sections
        y += 45;  // Skip "IMAGES" header

        // Check images section
        for (size_t i = 0; i < imageFiles.size(); i++) {
            // Check if touch is within this item's bounds
            if (touchY >= y && touchY <= y + ITEM_HEIGHT - 10) {
                return mdFiles.size() + textFiles.size() + i;
            }
            y += ITEM_HEIGHT;
        }
    }

    // Check if we have CBZ section
    if (cbzFiles.size() > 0) {
        y += 10;  // Gap between sections
        y += 45;  // Skip "COMICS" header

        // Check CBZ section
        for (size_t i = 0; i < cbzFiles.size(); i++) {
            // Check if touch is within this item's bounds
            if (touchY >= y && touchY <= y + ITEM_HEIGHT - 10) {
                return mdFiles.size() + textFiles.size() + imageFiles.size() + i;
            }
            y += ITEM_HEIGHT;
        }
    }

    return -1;  // No item touched
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
