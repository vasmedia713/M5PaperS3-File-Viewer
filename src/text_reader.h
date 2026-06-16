#ifndef TEXT_READER_H
#define TEXT_READER_H

#include <M5Unified.h>
#include <vector>
#include <LittleFS.h>
#include "config.h"

class TextReader {
public:
    TextReader();
    void begin(const String& filename);
    void draw();
    void update();
    bool shouldExit();
    void nextPage();
    void prevPage();

private:
    void loadContent();
    void parseIntoPages();
    void drawPage();
    void drawNavigation();
    void handleTouch();

    String filename;
    String content;
    std::vector<std::vector<String>> pages;
    int currentPage;
    int refreshCounter;
    bool exitRequested;

    int startTouchX;
    int startTouchY;
    bool touching;
};

#endif // TEXT_READER_H
