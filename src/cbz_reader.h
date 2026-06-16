#ifndef CBZ_READER_H
#define CBZ_READER_H

#include <M5Unified.h>
#include <vector>
#include <LittleFS.h>
#include "config.h"

class CBZReader {
public:
    CBZReader();
    void begin(const String& filename);
    void draw();
    void update();
    bool shouldExit();
    void nextPage();
    void prevPage();

private:
    void extractAndListPages();
    void sortPagesNaturally();
    void loadPage();
    void drawPage();
    void drawOverlay();
    void handleTouch();
    int naturalCompare(const String& a, const String& b);
    int extractNumericPart(const String& str, int& pos);

    String filename;
    std::vector<String> pageFilenames;
    int currentPageIndex;
    bool exitRequested;
    bool showOverlay;
    unsigned long overlayTimer;

    uint8_t* imageBuffer;
    int imageBufferSize;

    int startTouchX;
    int startTouchY;
    bool touching;
};

#endif // CBZ_READER_H
