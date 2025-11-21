#ifndef UTILS_H
#define UTILS_H

#include <M5Unified.h>
#include <vector>
#include <LittleFS.h>

// Utility functions

// Check if file has specific extension
inline bool hasExtension(const String& filename, const String& ext) {
    return filename.endsWith(ext) || filename.endsWith(ext.substring(0, ext.length()));
}

// Get file extension
inline String getExtension(const String& filename) {
    int lastDot = filename.lastIndexOf('.');
    if (lastDot == -1) return "";
    return filename.substring(lastDot);
}

// List files in directory with filter
inline std::vector<String> listFiles(const char* dirname, const char* extension = nullptr) {
    std::vector<String> files;

    File root = LittleFS.open(dirname);
    if (!root || !root.isDirectory()) {
        return files;
    }

    File file = root.openNextFile();
    while (file) {
        if (!file.isDirectory()) {
            String filename = String(file.name());
            if (extension == nullptr || filename.endsWith(extension)) {
                files.push_back(filename);
            }
        }
        file = root.openNextFile();
    }

    return files;
}

// Draw centered text
inline void drawCenteredText(const char* text, int y, uint16_t color = 0x0000) {
    M5.Display.setTextColor(color);
    int16_t x = (M5.Display.width() - M5.Display.textWidth(text)) / 2;
    M5.Display.setCursor(x, y);
    M5.Display.print(text);
}

// Word wrap text
inline std::vector<String> wordWrap(const String& text, int maxWidth) {
    std::vector<String> lines;
    String line = "";
    String word = "";

    for (size_t i = 0; i < text.length(); i++) {
        char c = text[i];

        if (c == ' ' || c == '\n') {
            String testLine = line + (line.length() > 0 ? " " : "") + word;
            if (M5.Display.textWidth(testLine.c_str()) > maxWidth && line.length() > 0) {
                lines.push_back(line);
                line = word;
            } else {
                line = testLine;
            }
            word = "";

            if (c == '\n') {
                lines.push_back(line);
                line = "";
            }
        } else {
            word += c;
        }
    }

    // Add remaining word and line
    if (word.length() > 0) {
        String testLine = line + (line.length() > 0 ? " " : "") + word;
        if (M5.Display.textWidth(testLine.c_str()) > maxWidth && line.length() > 0) {
            lines.push_back(line);
            lines.push_back(word);
        } else {
            lines.push_back(testLine);
        }
    } else if (line.length() > 0) {
        lines.push_back(line);
    }

    return lines;
}

// Initialize filesystem
inline bool initFileSystem() {
    if (!LittleFS.begin(true)) {
        Serial.println("LittleFS Mount Failed");
        return false;
    }

    // Create directories if they don't exist
    if (!LittleFS.exists("/files")) {
        LittleFS.mkdir("/files");
    }
    if (!LittleFS.exists("/config")) {
        LittleFS.mkdir("/config");
    }

    Serial.println("LittleFS mounted successfully");
    return true;
}

#endif // UTILS_H
