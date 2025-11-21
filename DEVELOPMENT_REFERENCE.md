# M5PaperS3 Development Reference

**Quick reference for editing and adding features to this project**

---

## 🚨 CRITICAL: Display Fix Requirements

### Must-Have Configuration

**platformio.ini - Library Dependencies:**
```ini
lib_deps =
    m5stack/M5Unified@^0.1.16
    https://github.com/m5stack/M5GFX.git#develop  # ⚠️ MUST USE DEVELOP BRANCH!
```

**Why:** M5GFX stable release (v0.1.16) has a bug with M5PaperS3 display initialization. The develop branch contains the fix.

**Reference:** https://github.com/m5stack/M5GFX/issues/119

---

## 📐 Display Specifications

| Property | Value |
|----------|-------|
| Resolution | 960×540 pixels |
| Size | 4.7 inches |
| Type | E-ink (EPD) |
| Colors | 16-level grayscale |
| Controller | Direct EPD (not IT8951) |
| Rotation | 1 = Landscape mode |

**Important:** M5PaperS3 ≠ M5Paper (original). They use different display controllers!

---

## 🎨 Display Operations

### Initialization Pattern
```cpp
auto cfg = M5.config();
M5.begin(cfg);

// Verify display detected
Serial.printf("Display: %dx%d\n", M5.Display.width(), M5.Display.height());
// Should show: 960x540 (not 0x0!)

M5.Display.setRotation(1);  // Landscape
```

### Drawing to E-ink

```cpp
// ❌ WRONG - Don't use this
M5.Display.clearDisplay();

// ✅ CORRECT - Use this instead
M5.Display.fillScreen(TFT_WHITE);

// Draw your content
M5.Display.setTextColor(TFT_BLACK);
M5.Display.setTextSize(2);
M5.Display.setCursor(100, 100);
M5.Display.println("Hello");

// ⚠️ REQUIRED - Push to screen (e-ink needs explicit refresh)
M5.Display.display();
```

### Color Constants
```cpp
TFT_WHITE  // 0xFFFF - White background
TFT_BLACK  // 0x0000 - Black text
COLOR_GRAY // 0x7BEF - Medium gray (defined in config.h)
```

---

## 🔧 Common Modifications

### Adding New Features

#### 1. New File Type Support
**Files to modify:**
- `src/file_browser.cpp` - Add extension check in `scanFiles()`
- `src/config.h` - Add new `AppState` enum value
- `src/main.cpp` - Add state transition logic in `loop()`

**Example - Adding PDF support:**
```cpp
// In file_browser.cpp
if (lowerName.endsWith(".pdf")) {
    pdfFiles.push_back(filename);
}

// In config.h
enum AppState {
    STATE_FILE_BROWSER,
    STATE_MARKDOWN_READER,
    STATE_IMAGE_VIEWER,
    STATE_PDF_VIEWER,     // New state
    STATE_WIFI_SERVER
};

// In main.cpp
case STATE_PDF_VIEWER:
    pdfViewer.update();
    if (pdfViewer.shouldExit()) {
        currentState = STATE_FILE_BROWSER;
    }
    break;
```

#### 2. Changing Font Sizes
**Location:** Each viewer component has its own font size settings

```cpp
// File browser header
M5.Display.setTextSize(2);  // src/file_browser.cpp:57

// Markdown reader
#define MD_LINE_HEIGHT 24   // src/config.h:40
M5.Display.setTextSize(1);  // src/markdown_reader.cpp

// Increase for better readability:
M5.Display.setTextSize(3);  // Larger text
```

#### 3. Adjusting Touch Areas
**Location:** `src/main.cpp` and individual viewer files

```cpp
// WiFi button touch area (top-right corner)
if (touch.x > SCREEN_WIDTH - 100 && touch.y < 40) {  // main.cpp:127
    toggleWiFi();
}

// Adjust touch sensitivity
#define SWIPE_THRESHOLD 50   // config.h:36 - Min distance for swipe
#define TAP_THRESHOLD 10     // config.h:37 - Max movement for tap
```

#### 4. Changing WiFi Credentials
**Location:** `src/config.h`

```cpp
#define WIFI_SSID "PaperS3-Files"     // Line 15
#define WIFI_PASSWORD "papers3app"    // Line 16
#define WIFI_CHANNEL 6                // Line 17
```

**After changing:** Clean build and re-upload
```powershell
pio run --target clean
pio run --target upload
```

---

## 🐛 Debugging Tips

### Serial Monitor
```powershell
C:\Users\user\.platformio\penv\Scripts\platformio.exe device monitor
```

**Key debug output to check:**
```
Display width: 960    # Should NOT be 0
Display height: 540   # Should NOT be 0
```

### Common Issues

#### Display shows 0x0 dimensions
**Cause:** M5GFX wrong version
**Fix:** Verify `platformio.ini` uses develop branch
**Reference:** https://github.com/m5stack/M5GFX/issues/119

#### Screen fades but doesn't update
**Cause:** Missing `M5.Display.display()` call
**Fix:** Add `M5.Display.display()` after drawing

#### Touch not responding
**Cause:** Wrong touch coordinates or thresholds
**Fix:** Check serial monitor for touch coordinates, adjust areas

#### Strange speaker sounds
**Cause:** Speaker initialization enabled
**Fix:** Ensure `cfg.internal_spk = false;` in setup()

---

## 📁 File Structure

```
src/
├── main.cpp              # State machine, setup(), loop()
├── config.h              # ALL configuration constants
├── utils.h               # Inline utility functions
├── file_browser.*        # File listing and selection
├── markdown_reader.*     # MD parsing and pagination
├── image_viewer.*        # Image decoding and display
└── wifi_server.*         # WiFi AP and HTTP upload server
```

**Modification Guidelines:**
- **Constants:** Always define in `config.h`, never hardcode
- **Utilities:** Add to `utils.h` if used in multiple files
- **State transitions:** Handle in `main.cpp` loop()
- **Display operations:** Always end with `M5.Display.display()`

---

## 🔗 Essential URLs

### Primary References (Check These First)
1. **M5GFX Issue #119** - Display fix solution
   https://github.com/m5stack/M5GFX/issues/119

2. **M5PaperS3 Documentation** - Official setup guide
   https://github.com/m5stack/M5GFX/blob/master/docs/M5PaperS3.md

3. **M5Stack Official Docs** - Hardware specs
   https://docs.m5stack.com/en/core/papers3

4. **Factory Demo Source** - Reference implementation
   https://github.com/m5stack/M5PaperS3-UserDemo

### Library Documentation
- **M5Unified:** https://github.com/m5stack/M5Unified
- **M5GFX:** https://github.com/m5stack/M5GFX
- **JPEGDEC:** https://github.com/bitbank2/JPEGDEC

### Community
- **Forum Discussion:** https://community.m5stack.com/topic/7108/paper-s3-questions
- **Arduino Examples:** https://docs.m5stack.com/en/arduino/m5papers3/program

---

## 🚀 Build & Deploy

### Standard Workflow
```powershell
cd C:\Users\user\Projects\M5_Paper_Project

# Build only
C:\Users\user\.platformio\penv\Scripts\platformio.exe run

# Upload (device in download mode first!)
C:\Users\user\.platformio\penv\Scripts\platformio.exe run --target upload

# Monitor
C:\Users\user\.platformio\penv\Scripts\platformio.exe device monitor
```

### Download Mode
1. Long press power button (side)
2. Wait for RED LED blink
3. Release button
4. Upload within ~30 seconds

### Git Workflow
```powershell
git add .
git commit -m "Description of changes"
git push
```

---

## ✅ Pre-Commit Checklist

Before committing changes:
- [ ] Code compiles without errors
- [ ] Tested on actual M5PaperS3 hardware
- [ ] Display refresh working (called `M5.Display.display()`)
- [ ] Serial output checked for errors
- [ ] No hardcoded values (use config.h constants)
- [ ] Updated relevant documentation
- [ ] No sensitive credentials added

---

## 📝 Notes for Future Enhancements

### Potential Features
- [ ] PDF viewer support
- [ ] TXT file viewer
- [ ] Battery level indicator
- [ ] Sleep mode for power saving
- [ ] Adjustable font sizes in UI
- [ ] Bookmarks for documents
- [ ] File management (delete, rename)
- [ ] Dark mode / inverted colors
- [ ] Custom fonts

### Performance Improvements
- [ ] Optimize e-ink refresh rates
- [ ] Reduce memory usage in image viewer
- [ ] Cache frequently accessed files
- [ ] Lazy loading for large documents

### Known Limitations
- Text size currently small for e-ink
- No error recovery for corrupt files
- Limited to 16-level grayscale
- No nested folder support

---

**Last Updated:** 2025-01-21
**Working Configuration:** M5Unified 0.1.16 + M5GFX develop branch
**Device:** M5PaperS3 (ESP32-S3)
