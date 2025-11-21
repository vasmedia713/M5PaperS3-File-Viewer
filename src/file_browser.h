#ifndef FILE_BROWSER_H
#define FILE_BROWSER_H

#include <M5Unified.h>
#include <vector>
#include <LittleFS.h>
#include "config.h"

class FileBrowser {
public:
    FileBrowser();
    void begin();
    void draw();
    void update();
    String getSelectedFile();
    bool hasSelection();
    void clearSelection();

private:
    void loadFiles();
    void drawFileList();
    void drawHeader();
    void handleTouch();
    void handleButtons();
    int getTouchIndex(int touchY);

    std::vector<String> mdFiles;
    std::vector<String> imageFiles;
    int selectedIndex;
    int scrollOffset;
    bool fileSelected;
    String selectedFile;

    static const int ITEM_HEIGHT = 40;
    static const int ITEMS_PER_PAGE = 11;
    static const int HEADER_HEIGHT = 60;
};

#endif // FILE_BROWSER_H
