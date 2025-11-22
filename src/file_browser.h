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
    String truncateFilename(String filename);

    std::vector<String> mdFiles;
    std::vector<String> imageFiles;
    int selectedIndex;
    int scrollOffset;
    bool fileSelected;
    String selectedFile;

    static const int ITEM_HEIGHT = 90;  // Increased for better touch spacing and e-ink readability
    static const int ITEMS_PER_PAGE = 4;  // Reduced to fit larger items on screen
    static const int HEADER_HEIGHT = 80;  // Height for header with larger font
    static const int MAX_FILENAME_CHARS = 35;  // Maximum characters to display before truncation
};

#endif // FILE_BROWSER_H
