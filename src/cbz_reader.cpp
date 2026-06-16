#include "cbz_reader.h"
#include "utils.h"
#include "mz.h"
#include "mz_zip.h"

CBZReader::CBZReader()
    : currentPageIndex(0), exitRequested(false), showOverlay(true),
      overlayTimer(0), imageBuffer(nullptr), imageBufferSize(0),
      startTouchX(0), startTouchY(0), touching(false) {
}

void CBZReader::begin(const String& fname) {
    filename = fname;
    currentPageIndex = 0;
    exitRequested = false;
    showOverlay = true;
    overlayTimer = millis();
    imageBuffer = nullptr;
    imageBufferSize = 0;

    extractAndListPages();
    sortPagesNaturally();

    if (pageFilenames.size() > 0) {
        loadPage();
    } else {
        Serial.println("No images found in CBZ file");
    }
}

void CBZReader::extractAndListPages() {
    pageFilenames.clear();

    // Ensure filename starts with /files/
    String filepath = filename;
    if (!filepath.startsWith("/files/") && !filepath.startsWith("/")) {
        filepath = "/files/" + filepath;
    }

    Serial.printf("Opening CBZ file: %s\n", filepath.c_str());

    void* handle = nullptr;
    mz_zip_reader_create(&handle);

    int err = mz_zip_reader_open_file(handle, filepath.c_str());
    if (err != MZ_OK) {
        Serial.printf("Failed to open ZIP file: %d\n", err);
        mz_zip_reader_delete(&handle);
        return;
    }

    // List all files in the archive
    err = mz_zip_reader_goto_first_entry(handle);
    while (err == MZ_OK) {
        mz_zip_file* file_info = nullptr;
        err = mz_zip_reader_file_stat(handle, &file_info);

        if (err == MZ_OK && file_info != nullptr) {
            String entryName = String(file_info->filename);
            entryName.toLowerCase();

            // Filter for supported image formats
            if (entryName.endsWith(".jpg") || entryName.endsWith(".jpeg") ||
                entryName.endsWith(".png") || entryName.endsWith(".webp")) {
                // Only add if it's not a directory
                if (!file_info->external_fa & 0x10) {
                    pageFilenames.push_back(file_info->filename);
                    Serial.printf("Added page: %s\n", file_info->filename);
                }
            }
        }

        err = mz_zip_reader_goto_next_entry(handle);
    }

    mz_zip_reader_close(handle);
    mz_zip_reader_delete(&handle);

    Serial.printf("Found %d images in CBZ\n", pageFilenames.size());
}

void CBZReader::sortPagesNaturally() {
    if (pageFilenames.size() <= 1) return;

    // Simple bubble sort using natural comparison
    for (size_t i = 0; i < pageFilenames.size(); i++) {
        for (size_t j = i + 1; j < pageFilenames.size(); j++) {
            if (naturalCompare(pageFilenames[i], pageFilenames[j]) > 0) {
                // Swap
                String temp = pageFilenames[i];
                pageFilenames[i] = pageFilenames[j];
                pageFilenames[j] = temp;
            }
        }
    }

    // Log sorted order
    Serial.println("Sorted page order:");
    for (size_t i = 0; i < pageFilenames.size(); i++) {
        Serial.printf("  [%d] %s\n", i, pageFilenames[i].c_str());
    }
}

int CBZReader::naturalCompare(const String& a, const String& b) {
    int aPos = 0, bPos = 0;
    int aLen = a.length(), bLen = b.length();

    while (aPos < aLen && bPos < bLen) {
        // Extract numeric parts
        if (isdigit(a[aPos]) && isdigit(b[bPos])) {
            int aNum = extractNumericPart(a, aPos);
            int bNum = extractNumericPart(b, bPos);

            if (aNum != bNum) {
                return aNum - bNum;
            }
        } else {
            char aChar = tolower(a[aPos]);
            char bChar = tolower(b[bPos]);

            if (aChar != bChar) {
                return aChar - bChar;
            }

            aPos++;
            bPos++;
        }
    }

    return aLen - bLen;
}

int CBZReader::extractNumericPart(const String& str, int& pos) {
    int num = 0;
    while (pos < (int)str.length() && isdigit(str[pos])) {
        num = num * 10 + (str[pos] - '0');
        pos++;
    }
    return num;
}

void CBZReader::loadPage() {
    M5.Display.fillScreen(COLOR_BG);
    M5.Display.setTextColor(COLOR_FG);
    drawCenteredText("Loading...", SCREEN_HEIGHT / 2);
    M5.Display.display();

    drawPage();
}

void CBZReader::drawPage() {
    if (pageFilenames.size() == 0) {
        M5.Display.fillScreen(COLOR_BG);
        drawCenteredText("No images in CBZ", SCREEN_HEIGHT / 2 - 20);
        M5.Display.display();
        return;
    }

    if (currentPageIndex >= (int)pageFilenames.size()) {
        currentPageIndex = pageFilenames.size() - 1;
    }

    String filepath = filename;
    if (!filepath.startsWith("/files/") && !filepath.startsWith("/")) {
        filepath = "/files/" + filepath;
    }

    Serial.printf("Loading page %d: %s from %s\n", currentPageIndex,
                  pageFilenames[currentPageIndex].c_str(), filepath.c_str());

    // Open ZIP and extract the image
    void* handle = nullptr;
    mz_zip_reader_create(&handle);

    int err = mz_zip_reader_open_file(handle, filepath.c_str());
    if (err != MZ_OK) {
        Serial.printf("Failed to open CBZ file: %d\n", err);
        M5.Display.fillScreen(COLOR_BG);
        drawCenteredText("Error opening CBZ", SCREEN_HEIGHT / 2);
        M5.Display.display();
        mz_zip_reader_delete(&handle);
        return;
    }

    // Locate and open the image file
    err = mz_zip_reader_locate_entry(handle, pageFilenames[currentPageIndex].c_str(), 0);
    if (err != MZ_OK) {
        Serial.printf("Failed to locate file in archive: %d\n", err);
        M5.Display.fillScreen(COLOR_BG);
        drawCenteredText("Error reading image", SCREEN_HEIGHT / 2);
        M5.Display.display();
        mz_zip_reader_close(handle);
        mz_zip_reader_delete(&handle);
        return;
    }

    mz_zip_file* file_info = nullptr;
    err = mz_zip_reader_file_stat(handle, &file_info);
    if (err != MZ_OK || file_info == nullptr) {
        Serial.println("Failed to get file info");
        M5.Display.fillScreen(COLOR_BG);
        drawCenteredText("Error reading image", SCREEN_HEIGHT / 2);
        M5.Display.display();
        mz_zip_reader_close(handle);
        mz_zip_reader_delete(&handle);
        return;
    }

    // Extract image to buffer
    uint32_t fileSize = file_info->uncompressed_size;
    Serial.printf("Image size: %d bytes\n", fileSize);

    // Free previous buffer if it exists
    if (imageBuffer != nullptr) {
        free(imageBuffer);
        imageBuffer = nullptr;
    }

    imageBuffer = (uint8_t*)malloc(fileSize);
    if (imageBuffer == nullptr) {
        Serial.println("Failed to allocate buffer for image");
        M5.Display.fillScreen(COLOR_BG);
        drawCenteredText("Memory error", SCREEN_HEIGHT / 2);
        M5.Display.display();
        mz_zip_reader_close(handle);
        mz_zip_reader_delete(&handle);
        return;
    }

    err = mz_zip_reader_read_open(handle);
    if (err != MZ_OK) {
        Serial.printf("Failed to open read stream: %d\n", err);
        free(imageBuffer);
        imageBuffer = nullptr;
        mz_zip_reader_close(handle);
        mz_zip_reader_delete(&handle);
        return;
    }

    int32_t bytesRead = 0;
    uint8_t* bufferPos = imageBuffer;
    while ((bytesRead = mz_zip_reader_read(handle, bufferPos, fileSize - (bufferPos - imageBuffer))) > 0) {
        bufferPos += bytesRead;
    }

    mz_zip_reader_read_close(handle);
    mz_zip_reader_close(handle);
    mz_zip_reader_delete(&handle);

    imageBufferSize = bufferPos - imageBuffer;
    Serial.printf("Read %d bytes from archive\n", imageBufferSize);

    // Now decode and display the image
    M5.Display.fillScreen(COLOR_BG);

    String imageName = pageFilenames[currentPageIndex];
    if (imageName.length() > 30) {
        imageName = imageName.substring(imageName.length() - 30);
    }

    imageName.toLowerCase();

    if (imageName.endsWith(".jpg") || imageName.endsWith(".jpeg")) {
        // JPEG decoding with aspect ratio preservation
        LGFX_Sprite sprite(&M5.Display);
        sprite.setColorDepth(16);

        // Create temporary sprite to decode dimensions
        sprite.createSprite(1, 1);
        sprite.drawJpg(imageBuffer, imageBufferSize, 0, 0);
        int imgWidth = sprite.width();
        int imgHeight = sprite.height();
        sprite.deleteSprite();

        Serial.printf("JPEG size: %dx%d\n", imgWidth, imgHeight);

        // Calculate scaling to fit screen (preserve aspect ratio)
        float scaleX = (float)SCREEN_WIDTH / imgWidth;
        float scaleY = (float)SCREEN_HEIGHT / imgHeight;
        float scale = min(scaleX, scaleY);

        int scaledWidth = imgWidth * scale;
        int scaledHeight = imgHeight * scale;
        int offsetX = (SCREEN_WIDTH - scaledWidth) / 2;
        int offsetY = (SCREEN_HEIGHT - scaledHeight) / 2;

        Serial.printf("Scaled to: %dx%d at offset (%d,%d)\n", scaledWidth, scaledHeight, offsetX, offsetY);

        // Create sprite with scaled dimensions
        if (sprite.createSprite(scaledWidth, scaledHeight)) {
            sprite.drawJpg(imageBuffer, imageBufferSize, 0, 0);
            sprite.pushSprite(offsetX, offsetY);
            sprite.deleteSprite();
        }
    } else if (imageName.endsWith(".png") || imageName.endsWith(".webp")) {
        // PNG/WebP decoding with aspect ratio preservation
        LGFX_Sprite tempSprite(&M5.Display);
        tempSprite.setColorDepth(16);

        // Create temporary sprite to decode dimensions
        tempSprite.createSprite(1, 1);
        tempSprite.drawPng(imageBuffer, imageBufferSize, 0, 0);
        int imgWidth = tempSprite.width();
        int imgHeight = tempSprite.height();
        tempSprite.deleteSprite();

        Serial.printf("PNG/WebP size: %dx%d\n", imgWidth, imgHeight);

        // Calculate scaling to fit screen (preserve aspect ratio)
        float scaleX = (float)SCREEN_WIDTH / imgWidth;
        float scaleY = (float)SCREEN_HEIGHT / imgHeight;
        float scale = min(scaleX, scaleY);

        int scaledWidth = imgWidth * scale;
        int scaledHeight = imgHeight * scale;
        int offsetX = (SCREEN_WIDTH - scaledWidth) / 2;
        int offsetY = (SCREEN_HEIGHT - scaledHeight) / 2;

        Serial.printf("Scaled to: %dx%d at offset (%d,%d)\n", scaledWidth, scaledHeight, offsetX, offsetY);

        // Create sprite with scaled dimensions
        LGFX_Sprite sprite(&M5.Display);
        sprite.setColorDepth(16);
        if (sprite.createSprite(scaledWidth, scaledHeight)) {
            sprite.drawPng(imageBuffer, imageBufferSize, 0, 0);
            sprite.pushSprite(offsetX, offsetY);
            sprite.deleteSprite();
        }
    }

    if (showOverlay) {
        drawOverlay();
    }

    M5.Display.display();
}

void CBZReader::drawOverlay() {
    // Draw overlay at bottom with page info
    int overlayY = SCREEN_HEIGHT - 50;

    M5.Display.fillRect(0, overlayY, SCREEN_WIDTH, 50, COLOR_GRAY);

    M5.Display.setTextColor(COLOR_BG);
    M5.Display.setTextSize(4);

    // Page info
    String pageInfo = String(currentPageIndex + 1) + " / " + String(pageFilenames.size());
    drawCenteredText(pageInfo.c_str(), overlayY + 12);

    // Navigation hints
    M5.Display.setCursor(20, overlayY + 12);
    M5.Display.print("<< PREV");

    M5.Display.setCursor(SCREEN_WIDTH - 120, overlayY + 12);
    M5.Display.print("NEXT >>");

    M5.Display.setTextColor(COLOR_FG);
}

void CBZReader::draw() {
    drawPage();
}

void CBZReader::update() {
    handleTouch();

    // Auto-hide overlay after 3 seconds
    if (showOverlay && (millis() - overlayTimer > 3000)) {
        showOverlay = false;
        draw();
    }
}

void CBZReader::handleTouch() {
    auto touch = M5.Touch.getDetail();

    if (touch.wasPressed()) {
        startTouchX = touch.x;
        startTouchY = touch.y;
        touching = true;

        // Show overlay on tap
        if (!showOverlay) {
            showOverlay = true;
            overlayTimer = millis();
            draw();
        }
    }

    if (touch.wasReleased() && touching) {
        int deltaX = touch.x - startTouchX;
        int deltaY = touch.y - startTouchY;

        // Check for back button (top left corner)
        if (startTouchX < 100 && startTouchY < 50) {
            exitRequested = true;
            touching = false;
            return;
        }

        // Check for swipe
        if (abs(deltaX) > SWIPE_THRESHOLD && abs(deltaX) > abs(deltaY)) {
            if (deltaX < 0) {
                // Swipe left = next page
                nextPage();
            } else {
                // Swipe right = previous page
                prevPage();
            }
        }
        // Check for tap on navigation areas
        else if (abs(deltaX) < TAP_THRESHOLD && abs(deltaY) < TAP_THRESHOLD) {
            // Tap on left side = previous page
            if (startTouchX < SCREEN_WIDTH / 3) {
                prevPage();
            }
            // Tap on right side = next page
            else if (startTouchX > (SCREEN_WIDTH * 2) / 3) {
                nextPage();
            }
        }

        touching = false;
    }
}

void CBZReader::nextPage() {
    if (currentPageIndex < (int)pageFilenames.size() - 1) {
        currentPageIndex++;
        loadPage();
    }
}

void CBZReader::prevPage() {
    if (currentPageIndex > 0) {
        currentPageIndex--;
        loadPage();
    }
}

bool CBZReader::shouldExit() {
    return exitRequested;
}
