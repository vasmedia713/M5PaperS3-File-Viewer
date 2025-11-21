# M5PaperS3 Display Troubleshooting Session Summary

## Problem
M5PaperS3 device was not displaying anything on the e-ink screen. After uploading code, the device would:
- Make strange speaker sounds
- Screen would fade slightly
- No visible content would appear
- Display dimensions reported as 0x0

## Root Cause
The issue was caused by **M5GFX library version incompatibility**:
- M5GFX version 0.1.16 (stable release) has a bug with M5PaperS3 support
- The library was failing to initialize the e-ink display controller
- M5Unified could not detect the display hardware (reported 0x0 dimensions)

## Solution
Switch to M5GFX **develop branch** which contains the M5PaperS3 fixes:

### Key Changes in platformio.ini

```ini
[env:m5stack-papers3]
platform = espressif32
board = esp32-s3-devkitc-1
framework = arduino

; M5Paper S3 has 16MB flash and OPI PSRAM
board_build.flash_size = 16MB
board_build.arduino.memory_type = qio_opi
board_build.partitions = default_16MB.csv
board_upload.flash_size = 16MB
board_build.f_cpu = 240000000L
board_build.f_flash = 80000000L

; Build flags for M5Paper S3
build_flags =
    -DBOARD_HAS_PSRAM
    -DARDUINO_M5STACK_PAPERS3
    -DARDUINO_USB_CDC_ON_BOOT=1
    -DCORE_DEBUG_LEVEL=3
    -DCONFIG_SPIRAM_MODE_OCT=1
    -DCONFIG_SPIRAM_CACHE_WORKAROUND=1
    -DM5PAPER
    -DM5UNIFIED_PC_BUILD=0

; Upload settings
upload_speed = 921600
monitor_speed = 115200

; Library dependencies - CRITICAL: Use M5GFX develop branch!
lib_deps =
    m5stack/M5Unified@^0.1.16
    https://github.com/m5stack/M5GFX.git#develop
    bblanchon/ArduinoJson@^7.0.4
    https://github.com/bitbank2/JPEGDEC.git

; Filesystem settings
board_build.filesystem = littlefs
```

### Key Changes in main.cpp

```cpp
void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n\n=== M5Paper S3 File Viewer ===");
    Serial.println("Starting initialization...");

    // Simple M5Unified initialization - let it auto-detect
    Serial.println("Initializing M5...");
    auto cfg = M5.config();
    M5.begin(cfg);
    Serial.println("M5.begin() completed");

    // Verify display was detected
    Serial.printf("Display width: %d\n", M5.Display.width());
    Serial.printf("Display height: %d\n", M5.Display.height());

    // Should now show: 960x540 instead of 0x0
    M5.Display.setRotation(1);

    // Draw to e-ink display
    M5.Display.fillScreen(TFT_WHITE);
    M5.Display.setTextColor(TFT_BLACK);
    M5.Display.setTextSize(3);
    M5.Display.setCursor(100, 200);
    M5.Display.println("M5Paper S3");

    // Push to screen (required for e-ink)
    M5.Display.display();
}
```

### Other Important Fixes

1. **Disabled Speaker** - Added `cfg.internal_spk = false` to prevent strange sounds
2. **Replaced clearDisplay() calls** - Changed all `M5.Display.clearDisplay()` to `M5.Display.fillScreen(COLOR_BG)` throughout all files
3. **Proper color constants** - Use `TFT_WHITE` and `TFT_BLACK` instead of custom hex values

## Current Status

✅ **WORKING** - Display now initializes correctly and shows content!

### What's Currently Visible:
- Black header bar with "File Browser" text
- Center text: "No files found" / "Connect to WiFi to upload"
- Touch interface is functional

### Files Modified:
- `platformio.ini` - Updated M5GFX to develop branch
- `src/main.cpp` - Simplified initialization
- `src/file_browser.cpp` - Changed clearDisplay() to fillScreen()
- `src/markdown_reader.cpp` - Changed clearDisplay() to fillScreen()
- `src/image_viewer.cpp` - Changed clearDisplay() to fillScreen()
- `src/config.h` - Added TFT color definitions

## Device Information

**Model:** M5PaperS3 (printed on device)
**Display:** 960×540 @ 4.7" e-ink display with 16-level grayscale
**Controller:** Direct EPD control (not IT8951 like original M5Paper)
**MCU:** ESP32-S3

## Important Notes

### Upload Procedure
The M5PaperS3 requires entering download mode before upload:
1. **Long press** power button (side of device)
2. Wait for back LED to blink **RED**
3. Release button
4. Upload within ~30 seconds

### Build Commands
```powershell
# Full path to platformio (if 'pio' not in PATH)
C:\Users\user\.platformio\penv\Scripts\platformio.exe run --target clean
C:\Users\user\.platformio\penv\Scripts\platformio.exe run --target upload
C:\Users\user\.platformio\penv\Scripts\platformio.exe device monitor
```

### Serial Monitor
- **Port:** COM4
- **Baud:** 115200
- To see full boot sequence, press reset button while monitor is running

## Next Steps / TODO

### Option 1: Test WiFi File Upload
1. Touch top-right corner to enable WiFi
2. Connect to WiFi SSID: `PaperS3-Files` / Password: `papers3app`
3. Open browser to displayed IP address
4. Upload test `.md` or `.jpg` files
5. Exit WiFi mode (touch top-right again)
6. Test file browsing and viewing

### Option 2: Improve E-ink Display Readability
Current text is very small (designed for LCD). Need to:
- Increase font sizes in file browser
- Adjust spacing for e-ink readability
- Optimize refresh rates (currently uses partial refresh)

### Known Issues to Address
1. **Small text** - Font sizes too small for comfortable e-ink reading
2. **Refresh rate** - May need to optimize full vs partial refresh strategy
3. **Touch calibration** - May need adjustment for accurate tap detection

## Reference Links

- **M5PaperS3 Factory Demo:** https://github.com/m5stack/M5PaperS3-UserDemo
- **M5GFX Issue #119 (M5PaperS3 fix):** https://github.com/m5stack/M5GFX/issues/119
- **M5GFX M5PaperS3 Docs:** https://github.com/m5stack/M5GFX/blob/master/docs/M5PaperS3.md
- **M5Stack Docs:** https://docs.m5stack.com/en/core/papers3

## Key Learnings

1. **M5PaperS3 ≠ M5Paper** - Different display controllers (EPD direct vs IT8951)
2. **Stable releases may lag** - Develop branch had critical fixes months before stable
3. **E-ink requires display()** - Unlike LCD, must call `M5.Display.display()` to refresh
4. **Auto-detection works** - M5Unified correctly detects M5PaperS3 with proper M5GFX version
5. **clearDisplay() is problematic** - Use `fillScreen()` instead for e-ink displays

## Summary for Next AI Session

**TL;DR:** M5PaperS3 display wasn't working because M5GFX 0.1.16 has a bug. Switched to develop branch (`https://github.com/m5stack/M5GFX.git#develop` in platformio.ini) and display now works perfectly. Device model is "M5PaperS3", display is 960x540 e-ink, currently showing file browser with small text that needs sizing improvement.
