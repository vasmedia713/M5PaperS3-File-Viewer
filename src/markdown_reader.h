#ifndef MARKDOWN_READER_H
#define MARKDOWN_READER_H

#include <M5Unified.h>
#include <vector>
#include <LittleFS.h>
#include "config.h"

class MarkdownReader {
public:
    MarkdownReader();
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
    void applyMarkdownFormatting(const String& line, int& y);

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

#endif // MARKDOWN_READER_H
