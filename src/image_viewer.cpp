#include "image_viewer.h"
#include "utils.h"

ImageViewer* ImageViewer::instance = nullptr;

ImageViewer::ImageViewer()
    : currentImageIndex(0), exitRequested(false), showOverlay(true),
      overlayTimer(0), startTouchX(0), startTouchY(0), touching(false) {
    instance = this;
}

void ImageViewer::begin(const String& filename) {
    currentFilename = filename;
    currentImageIndex = 0;
    exitRequested = false;
    showOverlay = true;
    overlayTimer = millis();

    getAllImages();

    // Find current image index
    for (size_t i = 0; i < allImages.size(); i++) {
        if (allImages[i] == filename) {
            currentImageIndex = i;
            break;
        }
    }

    loadImage();
}

void ImageViewer::getAllImages() {
    allImages.clear();

    File root = LittleFS.open("/files");
    if (!root || !root.isDirectory()) {
        return;
    }

    File file = root.openNextFile();
    while (file) {
        if (!file.isDirectory()) {
            String filename = String(file.name());
            String lowerName = filename;
            lowerName.toLowerCase();

            if (lowerName.endsWith(".jpg") || lowerName.endsWith(".jpeg") ||
                lowerName.endsWith(".png")) {
                allImages.push_back(filename);
            }
        }
        file = root.openNextFile();
    }

    Serial.printf("Found %d images\n", allImages.size());
}

void ImageViewer::loadImage() {
    M5.Display.fillScreen(COLOR_BG);
    M5.Display.setTextColor(COLOR_FG);
    drawCenteredText("Loading...", SCREEN_HEIGHT / 2);
    M5.Display.display();

    drawImage();
}

void ImageViewer::drawImage() {
    M5.Display.fillScreen(COLOR_BG);

    File file = LittleFS.open(currentFilename, "r");
    if (!file) {
        Serial.printf("Failed to open image: %s\n", currentFilename.c_str());
        drawCenteredText("Error loading image", SCREEN_HEIGHT / 2);
        M5.Display.display();
        return;
    }

    // Check if JPEG
    String lowerName = currentFilename;
    lowerName.toLowerCase();

    if (lowerName.endsWith(".jpg") || lowerName.endsWith(".jpeg")) {
        // Allocate buffer for JPEG file
        size_t fileSize = file.size();
        uint8_t* buffer = (uint8_t*)malloc(fileSize);

        if (buffer == nullptr) {
            Serial.println("Failed to allocate memory for JPEG");
            file.close();
            drawCenteredText("Memory error", SCREEN_HEIGHT / 2);
            M5.Display.display();
            return;
        }

        file.read(buffer, fileSize);
        file.close();

        // Decode JPEG
        if (jpeg.openRAM(buffer, fileSize, jpegDrawCallback)) {
            int imgWidth = jpeg.getWidth();
            int imgHeight = jpeg.getHeight();

            Serial.printf("Image size: %dx%d\n", imgWidth, imgHeight);

            // Calculate scaling to fit screen
            float scaleX = (float)SCREEN_WIDTH / imgWidth;
            float scaleY = (float)SCREEN_HEIGHT / imgHeight;
            float scale = min(scaleX, scaleY);

            int scaledWidth = imgWidth * scale;
            int scaledHeight = imgHeight * scale;

            // Center the image
            int offsetX = (SCREEN_WIDTH - scaledWidth) / 2;
            int offsetY = (SCREEN_HEIGHT - scaledHeight) / 2;

            // Decode with scaling
            jpeg.setPixelType(RGB565_BIG_ENDIAN);
            jpeg.decode(offsetX, offsetY, 0);
            jpeg.close();
        } else {
            Serial.println("Failed to decode JPEG");
            drawCenteredText("Invalid JPEG", SCREEN_HEIGHT / 2);
        }

        free(buffer);
    } else if (lowerName.endsWith(".png")) {
        // For PNG, load into memory buffer first (workaround for M5GFX + LittleFS incompatibility)
        size_t fileSize = file.size();
        uint8_t* buffer = (uint8_t*)malloc(fileSize);

        if (buffer == nullptr) {
            Serial.println("Failed to allocate memory for PNG");
            file.close();
            drawCenteredText("Memory error", SCREEN_HEIGHT / 2);
            M5.Display.display();
            return;
        }

        // Read entire PNG file into buffer
        size_t bytesRead = file.read(buffer, fileSize);
        file.close();

        if (bytesRead != fileSize) {
            Serial.printf("Failed to read PNG file: read %d of %d bytes\n", bytesRead, fileSize);
            free(buffer);
            drawCenteredText("Error reading PNG", SCREEN_HEIGHT / 2);
            M5.Display.display();
            return;
        }

        Serial.printf("Loaded PNG into memory: %d bytes\n", fileSize);

        // Decode PNG from memory buffer
        // M5.Display.drawPng() handles scaling and centering automatically
        M5.Display.drawPng(buffer, fileSize, 0, 0);

        free(buffer);
    } else {
        file.close();
        drawCenteredText("Unsupported format", SCREEN_HEIGHT / 2);
    }

    if (showOverlay) {
        drawOverlay();
    }

    M5.Display.display();
}

int ImageViewer::jpegDrawCallback(JPEGDRAW *pDraw) {
    if (instance == nullptr) return 0;

    // Draw the pixels to the display
    M5.Display.pushImage(pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight, (uint16_t*)pDraw->pPixels);

    return 1; // Continue decoding
}

void ImageViewer::draw() {
    drawImage();
}

void ImageViewer::drawOverlay() {
    // Draw semi-transparent overlay at bottom
    int overlayY = SCREEN_HEIGHT - 60;

    M5.Display.fillRect(0, overlayY, SCREEN_WIDTH, 60, COLOR_GRAY);

    M5.Display.setTextColor(COLOR_BG);
    M5.Display.setTextSize(1);

    // Image info
    String imageInfo = String(currentImageIndex + 1) + " / " + String(allImages.size());
    drawCenteredText(imageInfo.c_str(), overlayY + 10);

    // Filename
    String displayName = currentFilename;
    if (displayName.startsWith("/files/")) {
        displayName = displayName.substring(7);
    }
    drawCenteredText(displayName.c_str(), overlayY + 30);

    // Navigation hints
    M5.Display.setCursor(20, overlayY + 10);
    M5.Display.print("<< BACK");

    M5.Display.setTextColor(COLOR_FG);
}

void ImageViewer::update() {
    handleTouch();

    // Auto-hide overlay after 3 seconds
    if (showOverlay && (millis() - overlayTimer > 3000)) {
        showOverlay = false;
        draw();
    }
}

void ImageViewer::handleTouch() {
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

        // Check for back button
        if (showOverlay && startTouchX < 100 && startTouchY > SCREEN_HEIGHT - 60) {
            exitRequested = true;
            touching = false;
            return;
        }

        // Check for swipe
        if (abs(deltaX) > SWIPE_THRESHOLD && abs(deltaX) > abs(deltaY)) {
            overlayTimer = millis(); // Reset overlay timer

            if (deltaX < 0) {
                // Swipe left = next image
                nextImage();
            } else {
                // Swipe right = previous image
                prevImage();
            }
        }

        touching = false;
    }
}

void ImageViewer::nextImage() {
    if (allImages.size() == 0) return;

    currentImageIndex = (currentImageIndex + 1) % allImages.size();
    currentFilename = allImages[currentImageIndex];
    showOverlay = true;
    overlayTimer = millis();

    Serial.printf("Next image: %s\n", currentFilename.c_str());
    loadImage();
}

void ImageViewer::prevImage() {
    if (allImages.size() == 0) return;

    currentImageIndex--;
    if (currentImageIndex < 0) {
        currentImageIndex = allImages.size() - 1;
    }
    currentFilename = allImages[currentImageIndex];
    showOverlay = true;
    overlayTimer = millis();

    Serial.printf("Previous image: %s\n", currentFilename.c_str());
    loadImage();
}

bool ImageViewer::shouldExit() {
    return exitRequested;
}
