# M5PaperS3 File Viewer - Deployment Guide

**Last Updated:** 2025-01-21
**Version:** 1.1.0
**GitHub:** https://github.com/vasmedia713/M5PaperS3-File-Viewer

---

## 🎯 Current Status

✅ **Portrait Mode** - Display configured for 540×960 (portrait orientation)
✅ **Touch Detection** - Enhanced with active release polling and 50px threshold
✅ **File Selection** - Working with improved touch sensitivity
✅ **File Path Handling** - Automatic `/files/` prefix handling
✅ **UI Spacing** - 90px item height for better touch accuracy
✅ **Filename Truncation** - 35 character limit with ellipsis
✅ **Debug Logging** - Comprehensive serial output for troubleshooting

---

## 📋 Recent Changes (Session 2025-01-21)

### Display & Orientation
- **Portrait Mode**: Switched from landscape (960×540) to portrait (540×960)
- **Rotation**: Changed from rotation 1 to rotation 0 in `main.cpp`
- **Screen Constants**: Updated `SCREEN_WIDTH=540` and `SCREEN_HEIGHT=960` in `config.h`

### Touch & UI Improvements
- **Item Height**: Increased from 70px to 90px for better touch targets
- **TAP_THRESHOLD**: Increased from 10px to 50px for e-ink touchscreen tolerance
- **Touch Detection**: Completely rewrote to use active polling with release detection
- **Filename Truncation**: Added 35-character limit with ellipsis for long filenames
- **Max Characters**: `MAX_FILENAME_CHARS = 35` in `file_browser.h`

### Bug Fixes
- **File Path Handling**: Auto-prepend `/files/` if missing in markdown/image viewers
- **Touch Release**: Active polling loop waits for actual finger release
- **Debug Output**: Added comprehensive logging for touch events and file selection

### Files Modified
```
src/config.h               - Screen dimensions, TAP_THRESHOLD
src/file_browser.h         - ITEM_HEIGHT, MAX_FILENAME_CHARS
src/file_browser.cpp       - Touch handling, truncation, debug logging
src/main.cpp               - Display rotation, debug output
src/markdown_reader.cpp    - File path handling, error messages
src/image_viewer.cpp       - File path handling, error messages
```

---

## 🔧 Build & Upload Instructions

### Prerequisites
- **Hardware**: M5Stack M5PaperS3 device
- **Software**: PlatformIO (installed in VS Code)
- **Cable**: USB-C data cable (not charge-only)
- **Port**: COM4 (or check Device Manager)

### Quick Upload (3 Steps)

#### 1. Enter Download Mode
```
- Long press power button (on side of device)
- Wait for back LED to blink RED
- Release button
- Upload within ~30 seconds
```

#### 2. Build & Upload
```powershell
cd C:\Users\user\projects\M5_Paper_Project
C:\Users\user\.platformio\penv\Scripts\platformio.exe run --target upload
```

#### 3. Monitor Serial Output
```powershell
C:\Users\user\.platformio\penv\Scripts\platformio.exe device monitor
```
(Baud: 115200, Ctrl+C to exit)

### Build Only (No Upload)
```powershell
C:\Users\user\.platformio\penv\Scripts\platformio.exe run
```

### Clean Build
```powershell
C:\Users\user\.platformio\penv\Scripts\platformio.exe run --target clean
C:\Users\user\.platformio\penv\Scripts\platformio.exe run --target upload
```

---

## 🎨 Configuration Reference

### Display Settings (config.h:25-28)
```cpp
// Display Settings (Portrait Mode)
#define SCREEN_WIDTH 540
#define SCREEN_HEIGHT 960
#define REFRESH_INTERVAL 10  // Full refresh every N page turns
```

### Touch Settings (config.h:35-37)
```cpp
// Touch Settings
#define SWIPE_THRESHOLD 50   // Minimum distance for swipe detection
#define TAP_THRESHOLD 50     // Maximum movement for tap detection
```

### File Browser Settings (file_browser.h:34-37)
```cpp
static const int ITEM_HEIGHT = 90;              // Touch target height
static const int ITEMS_PER_PAGE = 4;            // Items visible on screen
static const int HEADER_HEIGHT = 80;            // Header bar height
static const int MAX_FILENAME_CHARS = 35;       // Truncation limit
```

### WiFi Settings (config.h:14-18)
```cpp
#define WIFI_SSID "PaperS3-Files"
#define WIFI_PASSWORD "papers3app"
#define WIFI_CHANNEL 6
#define MAX_WIFI_CLIENTS 4
```

---

## 🐛 Troubleshooting

### Touch Not Working

**Symptoms:**
- Tapping files shows "Touch released at different position (drag detected)"
- Files don't open when tapped

**Serial Debug Output:**
```
Touch at Y: 514
Touch index: 3 (total files: 4)
Waiting for touch release...
Touch released. Movement: 0 pixels (threshold: 50)
✓✓✓ SELECTED MARKDOWN [3]: /files/SAMPLE.md
```

**Solutions:**
1. Check serial monitor for "Waiting for touch release..." (confirms new firmware)
2. If missing, re-upload firmware (may not have uploaded correctly)
3. Try tapping more firmly and releasing cleanly
4. If threshold too strict, increase `TAP_THRESHOLD` in `config.h`

### Files Not Opening

**Symptoms:**
- File selected (✓✓✓ in serial) but screen just flashes
- Error message shown on screen

**Serial Debug Output:**
```
=== FILE SELECTED ===
Selected file: /files/SAMPLE.md
Opening markdown file...
Attempting to open: /files/SAMPLE.md
Loaded 156 bytes from /files/SAMPLE.md
```

**Solutions:**
1. Check if file exists: Use WiFi mode to verify files are uploaded
2. Check file path in serial output
3. Ensure files are in `/files/` directory on LittleFS
4. If "Failed to open file", the file may be corrupted or missing

### Display Shows Wrong Orientation

**Symptoms:**
- Screen rotated 90 degrees
- UI appears sideways

**Solution:**
Check `main.cpp:46`:
```cpp
M5.Display.setRotation(0);  // 0 = Portrait, 1 = Landscape
```

Match with `config.h:26-27`:
```cpp
#define SCREEN_WIDTH 540    // Portrait
#define SCREEN_HEIGHT 960
```

### Upload Fails "Port Busy"

**Symptoms:**
```
Could not open COM4, the port is busy
```

**Solution:**
1. Close serial monitor (Ctrl+C)
2. Close any other serial programs (Arduino IDE, PuTTY, etc.)
3. Retry upload

### Upload Fails "Access Denied"

**Symptoms:**
```
PermissionError(13, 'Access is denied.')
```

**Solution:**
1. Device not in download mode (need RED LED)
2. Try different USB port
3. Try different USB cable
4. Check Device Manager for COM port

---

## 📊 Serial Monitor Guide

### What to Look For

#### Startup Sequence
```
=== M5Paper S3 File Viewer ===
Starting initialization...
Initializing M5...
Display width: 540    ← Should be 540, not 0
Display height: 960   ← Should be 960, not 0
After rotation: 540x960

=== SCANNING FILES ===
Found file: /files/SAMPLE.md
  -> Added to mdFiles
Total: 1 markdown files and 0 images
```

#### Touch Events
```
Touch at Y: 210
Touch index: 0 (total files: 4)
Waiting for touch release...
Touch released. Movement: 2 pixels (threshold: 50)
✓✓✓ SELECTED MARKDOWN [0]: /files/SAMPLE.md
```

#### File Opening
```
=== FILE SELECTED ===
Selected file: /files/SAMPLE.md
File length: 17
Opening markdown file...
Attempting to open: /files/SAMPLE.md
Loaded 156 bytes from /files/SAMPLE.md
Parsed into 1 pages
Markdown reader initialized
```

### Debug Indicators

| Message | Meaning |
|---------|---------|
| `✓✓✓ SELECTED` | File successfully selected |
| `Rejected: moved X pixels` | Touch moved too much (drag, not tap) |
| `Touch outside file area` | Tapped empty space |
| `Waiting for touch release...` | New firmware active polling |
| `Touch released. Movement: X` | Shows exact movement delta |

---

## ✅ Testing Checklist

### After Upload

- [ ] Device boots and shows file browser
- [ ] Display is in portrait orientation (tall, not wide)
- [ ] File browser shows "DOCUMENTS" and/or "IMAGES" sections
- [ ] Filenames are visible and truncated if long
- [ ] Tapping file shows selection (gray highlight)
- [ ] Serial monitor shows "✓✓✓ SELECTED" message
- [ ] Markdown files open and display content
- [ ] Image files open and display correctly
- [ ] Touch WiFi button (top-right) to enter WiFi mode
- [ ] Can upload files via web browser at 192.168.4.1
- [ ] Exit WiFi mode returns to updated file list
- [ ] All files open correctly after upload

### File Browser
- [ ] Items are well-spaced (90px height)
- [ ] No accidental touches on wrong files
- [ ] Filenames don't run off screen edge
- [ ] Long filenames show "..." truncation
- [ ] Section headers clearly visible

### Touch Response
- [ ] Quick taps register as selections
- [ ] Files open after tap (not just flash)
- [ ] No false "drag detected" on simple taps
- [ ] Swipe gestures work in viewers

### File Viewers
- [ ] Markdown displays with proper formatting
- [ ] Images display scaled to fit screen
- [ ] Back button returns to file browser
- [ ] Navigation gestures work (swipe left/right)

---

## 🔗 Key Reference Files

### User Documentation
- **README.md** - Main project overview and features
- **BUILD_INSTRUCTIONS.md** - Detailed build setup
- **IPHONE_UPLOAD_GUIDE.md** - WiFi file transfer guide
- **PROJECT_SUMMARY.md** - Architecture overview

### Developer Documentation
- **CLAUDE.md** - AI assistant context and guidelines
- **DEVELOPMENT_REFERENCE.md** - Quick development reference
- **TROUBLESHOOTING_SESSION.md** - Display fix history
- **DEPLOYMENT_GUIDE.md** - This file

### Important Links
- **GitHub Repository**: https://github.com/vasmedia713/M5PaperS3-File-Viewer
- **M5GFX Issue #119**: https://github.com/m5stack/M5GFX/issues/119 (Critical display fix)
- **M5PaperS3 Docs**: https://docs.m5stack.com/en/core/papers3

---

## 🚀 Quick Start Summary

### For First-Time Deploy
1. Install PlatformIO in VS Code
2. Clone repository
3. Open project folder
4. Put M5PaperS3 in download mode (RED LED)
5. Run: `pio run --target upload`
6. Device boots → shows file browser

### For Updates
1. Pull latest from GitHub
2. Enter download mode (RED LED)
3. Run: `pio run --target upload`
4. Verify via serial monitor

### For Testing
1. Upload some .md or .jpg files via WiFi
2. Tap files to test selection
3. Check serial monitor for debug output
4. Verify all features work

---

## 📝 Version History

### v1.1.0 (2025-01-21)
- Switch to portrait mode (540×960)
- Improve touch detection with active polling
- Add filename truncation (35 chars)
- Increase item spacing to 90px
- Fix file path handling
- Add comprehensive debug logging

### v1.0.0 (2025-01-15)
- Initial release
- Landscape mode (960×540)
- Basic file browser
- Markdown and image viewers
- WiFi file upload server

---

## 🆘 Support

### Getting Help
1. Check serial monitor output (115200 baud)
2. Review TROUBLESHOOTING_SESSION.md
3. Check GitHub Issues
4. Verify M5GFX develop branch is being used

### Common Issues Database
See **TROUBLESHOOTING_SESSION.md** for:
- Display not working (0×0 dimensions)
- Touch not responding
- Files not opening
- WiFi connection issues
- Build/upload errors

---

## 📦 Critical Dependencies

### Must-Have Configuration (platformio.ini)
```ini
lib_deps =
    m5stack/M5Unified@^0.1.16
    https://github.com/m5stack/M5GFX.git#develop  # CRITICAL: Must use develop branch!
    bblanchon/ArduinoJson@^7.0.4
    https://github.com/bitbank2/JPEGDEC.git

board_build.arduino.memory_type = qio_opi  # REQUIRED: OPI PSRAM
build_flags = -DBOARD_HAS_PSRAM            # REQUIRED: Enable PSRAM
```

**⚠️ WARNING**: Using M5GFX stable release (v0.1.16) will cause display initialization to fail (0×0 dimensions). Always use `develop` branch!

---

## 🎯 Next Steps / Future Enhancements

### Potential Improvements
- [ ] Add PDF viewer support
- [ ] Implement file deletion via long-press
- [ ] Add page numbers to markdown reader
- [ ] Battery percentage indicator
- [ ] Sleep mode for power saving
- [ ] Adjustable font sizes in UI
- [ ] File sorting (alphabetical, date)
- [ ] Folder/category support
- [ ] Search functionality

### Performance Optimizations
- [ ] Optimize e-ink refresh rates
- [ ] Cache frequently accessed files
- [ ] Lazy loading for large documents
- [ ] Reduce memory usage in image viewer

---

**Deployment Complete!** 🎉

For questions or issues, refer to the troubleshooting section above or check the serial monitor output for detailed debug information.
