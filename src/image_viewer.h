#ifndef IMAGE_VIEWER_H
#define IMAGE_VIEWER_H

#include <M5Unified.h>
#include <vector>
#include <LittleFS.h>
#include <JPEGDEC.h>
#include "config.h"

class ImageViewer {
public:
    ImageViewer();
    void begin(const String& filename);
    void draw();
    void update();
    bool shouldExit();
    void nextImage();
    void prevImage();

private:
    void loadImage();
    void drawImage();
    void drawOverlay();
    void handleTouch();
    void getAllImages();
    static int jpegDrawCallback(JPEGDRAW *pDraw);

    String currentFilename;
    std::vector<String> allImages;
    int currentImageIndex;
    bool exitRequested;
    bool showOverlay;
    unsigned long overlayTimer;

    int startTouchX;
    int startTouchY;
    bool touching;

    JPEGDEC jpeg;
    static ImageViewer* instance; // For static callback
};

#endif // IMAGE_VIEWER_H
