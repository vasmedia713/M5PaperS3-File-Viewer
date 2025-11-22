#ifndef CONFIG_H
#define CONFIG_H

#include <M5GFX.h>

// Define TFT colors if not already defined
#ifndef TFT_BLACK
#define TFT_BLACK 0x0000
#endif
#ifndef TFT_WHITE
#define TFT_WHITE 0xFFFF
#endif

// WiFi Configuration
#define WIFI_SSID "PaperS3-Files"
#define WIFI_PASSWORD "papers3app"
#define WIFI_CHANNEL 6
#define MAX_WIFI_CLIENTS 4

// File System Paths
#define FILES_DIR "/files"
#define CONFIG_DIR "/config"
#define MANIFEST_FILE "/config/manifest.json"

// Display Settings (Portrait Mode)
#define SCREEN_WIDTH 540
#define SCREEN_HEIGHT 960
#define REFRESH_INTERVAL 10  // Full refresh every N page turns

// UI Colors (use TFT_ constants for better compatibility)
#define COLOR_BG TFT_WHITE      // White
#define COLOR_FG TFT_BLACK      // Black
#define COLOR_GRAY 0x7BEF       // Medium gray

// Touch Settings
#define SWIPE_THRESHOLD 50   // Minimum distance for swipe detection
#define TAP_THRESHOLD 50     // Maximum movement for tap detection (increased for e-ink touchscreen)

// Markdown Settings
#define MD_LINE_HEIGHT 24
#define MD_MARGIN 20
#define MD_MAX_LINES_PER_PAGE 20

// Image Settings
#define IMG_DECODE_BUFFER_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT * 2)

// Buttons (M5Paper S3 specific)
// The M5Paper S3 has touch screen, we'll use virtual buttons
// Physical power button on side

// App States
enum AppState {
    STATE_FILE_BROWSER,
    STATE_MARKDOWN_READER,
    STATE_IMAGE_VIEWER,
    STATE_WIFI_SERVER
};

#endif // CONFIG_H
