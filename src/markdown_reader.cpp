#include "markdown_reader.h"
#include "utils.h"

MarkdownReader::MarkdownReader()
    : currentPage(0), refreshCounter(0), exitRequested(false),
      startTouchX(0), startTouchY(0), touching(false) {
}

void MarkdownReader::begin(const String& fname) {
    filename = fname;
    currentPage = 0;
    refreshCounter = 0;
    exitRequested = false;

    loadContent();
    parseIntoPages();
}

void MarkdownReader::loadContent() {
    content = "";

    File file = LittleFS.open(filename, "r");
    if (!file) {
        Serial.printf("Failed to open file: %s\n", filename.c_str());
        content = "Error: Could not open file.";
        return;
    }

    while (file.available()) {
        content += (char)file.read();
    }
    file.close();

    Serial.printf("Loaded %d bytes from %s\n", content.length(), filename.c_str());
}

void MarkdownReader::parseIntoPages() {
    pages.clear();

    int contentWidth = SCREEN_WIDTH - (MD_MARGIN * 2);
    int contentHeight = SCREEN_HEIGHT - 80; // Leave space for header/footer
    int maxLinesPerPage = contentHeight / MD_LINE_HEIGHT;

    std::vector<String> currentPageLines;
    String line = "";

    M5.Display.setTextSize(1);

    for (size_t i = 0; i < content.length(); i++) {
        char c = content[i];

        if (c == '\n') {
            // Process the line
            std::vector<String> wrappedLines = wordWrap(line, contentWidth);

            for (const String& wrappedLine : wrappedLines) {
                currentPageLines.push_back(wrappedLine);

                if ((int)currentPageLines.size() >= maxLinesPerPage) {
                    pages.push_back(currentPageLines);
                    currentPageLines.clear();
                }
            }

            line = "";
        } else {
            line += c;
        }
    }

    // Process remaining line
    if (line.length() > 0) {
        std::vector<String> wrappedLines = wordWrap(line, contentWidth);
        for (const String& wrappedLine : wrappedLines) {
            currentPageLines.push_back(wrappedLine);

            if ((int)currentPageLines.size() >= maxLinesPerPage) {
                pages.push_back(currentPageLines);
                currentPageLines.clear();
            }
        }
    }

    // Add remaining lines as last page
    if (currentPageLines.size() > 0) {
        pages.push_back(currentPageLines);
    }

    Serial.printf("Parsed into %d pages\n", pages.size());
}

void MarkdownReader::draw() {
    // Use partial refresh for most updates
    bool useFullRefresh = (refreshCounter % REFRESH_INTERVAL == 0);

    if (useFullRefresh) {
        M5.Display.fillScreen(COLOR_BG);
    } else {
        M5.Display.fillScreen(COLOR_BG);
    }

    M5.Display.setTextSize(1);
    drawPage();
    drawNavigation();
    M5.Display.display();

    refreshCounter++;
}

void MarkdownReader::drawPage() {
    if (pages.size() == 0) {
        drawCenteredText("No content to display", SCREEN_HEIGHT / 2);
        return;
    }

    if (currentPage >= (int)pages.size()) {
        currentPage = pages.size() - 1;
    }

    // Header
    M5.Display.fillRect(0, 0, SCREEN_WIDTH, 40, COLOR_FG);
    M5.Display.setTextColor(COLOR_BG);
    M5.Display.setTextSize(1);
    M5.Display.setCursor(MD_MARGIN, 15);

    String displayName = filename;
    if (displayName.startsWith("/files/")) {
        displayName = displayName.substring(7);
    }
    M5.Display.print(displayName);

    // Content
    M5.Display.setTextColor(COLOR_FG);
    int y = 50;

    for (const String& line : pages[currentPage]) {
        applyMarkdownFormatting(line, y);
        y += MD_LINE_HEIGHT;
    }
}

void MarkdownReader::applyMarkdownFormatting(const String& line, int& y) {
    String processedLine = line;

    // Simple markdown parsing
    if (processedLine.startsWith("# ")) {
        M5.Display.setTextSize(2);
        M5.Display.setCursor(MD_MARGIN, y);
        M5.Display.print(processedLine.substring(2));
        M5.Display.setTextSize(1);
        y += 10; // Extra spacing after header
    } else if (processedLine.startsWith("## ")) {
        M5.Display.setTextSize(2);
        M5.Display.setCursor(MD_MARGIN, y);
        M5.Display.print(processedLine.substring(3));
        M5.Display.setTextSize(1);
        y += 5;
    } else if (processedLine.startsWith("### ")) {
        M5.Display.setTextSize(1);
        M5.Display.setCursor(MD_MARGIN, y);
        M5.Display.setTextColor(COLOR_GRAY);
        M5.Display.print(processedLine.substring(4));
        M5.Display.setTextColor(COLOR_FG);
    } else if (processedLine.startsWith("- ") || processedLine.startsWith("* ")) {
        M5.Display.setCursor(MD_MARGIN + 10, y);
        M5.Display.print("\x07 "); // Bullet
        M5.Display.print(processedLine.substring(2));
    } else {
        M5.Display.setCursor(MD_MARGIN, y);
        M5.Display.print(processedLine);
    }
}

void MarkdownReader::drawNavigation() {
    // Footer with page info
    int footerY = SCREEN_HEIGHT - 30;
    M5.Display.fillRect(0, footerY, SCREEN_WIDTH, 30, COLOR_GRAY);

    M5.Display.setTextColor(COLOR_BG);
    M5.Display.setTextSize(1);

    String pageInfo = "Page " + String(currentPage + 1) + " of " + String(pages.size());
    drawCenteredText(pageInfo.c_str(), footerY + 10);

    // Draw navigation hints
    M5.Display.setCursor(20, footerY + 10);
    M5.Display.print("<< PREV");

    M5.Display.setCursor(SCREEN_WIDTH - 100, footerY + 10);
    M5.Display.print("NEXT >>");

    M5.Display.setTextColor(COLOR_FG);
}

void MarkdownReader::update() {
    handleTouch();
}

void MarkdownReader::handleTouch() {
    auto touch = M5.Touch.getDetail();

    if (touch.wasPressed()) {
        startTouchX = touch.x;
        startTouchY = touch.y;
        touching = true;
    }

    if (touch.wasReleased() && touching) {
        int deltaX = touch.x - startTouchX;
        int deltaY = touch.y - startTouchY;

        // Check for back button (top left)
        if (startTouchX < 100 && startTouchY < 40) {
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

void MarkdownReader::nextPage() {
    if (currentPage < (int)pages.size() - 1) {
        currentPage++;
        draw();
    }
}

void MarkdownReader::prevPage() {
    if (currentPage > 0) {
        currentPage--;
        draw();
    }
}

bool MarkdownReader::shouldExit() {
    return exitRequested;
}
