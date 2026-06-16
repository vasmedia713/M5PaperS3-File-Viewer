# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

M5Paper S3 File Viewer - An e-ink document and image viewer for the M5Stack M5Paper S3 device with wireless file upload capabilities.

**Platform:** ESP32-S3 with Arduino framework
**Display:** 960×540 e-ink touchscreen
**Build System:** PlatformIO

## Essential Build Commands

### Development
```bash
# Build without uploading
pio run

# Build and upload (device must be in download mode)
pio run --target upload

# Monitor serial output (115200 baud)
pio device monitor

# Clean build
pio run --target clean
```

### Device Setup
Before uploading, M5Paper S3 must enter download mode:
1. Long press power button (on side)
2. Wait for back LED to blink RED
3. Release button
4. Upload immediately (within ~30 seconds)

### Erase Flash (removes all user files)
```bash
pio run --target erase
pio run --target upload
```

## Architecture

### State Machine Design
The application uses a state-based architecture defined in `src/main.cpp`:
- `STATE_FILE_BROWSER`: Home screen showing files
- `STATE_MARKDOWN_READER`: Paginated markdown viewer
- `STATE_IMAGE_VIEWER`: Full-screen image viewer
- `STATE_WIFI_SERVER`: WiFi AP mode for file uploads

State transitions are managed in the main loop based on user touch input.

### Modular Components

**File Browser** (`file_browser.h/cpp`):
- Scans `/files/` directory for `.md` and image files
- Renders two-column list (Documents | Images)
- Touch coordinate mapping for file selection
- Triggers state transitions to readers

**Markdown Reader** (`markdown_reader.h/cpp`):
- Word-wrap algorithm for line breaking
- Page builder creates pages based on line count
- Supports headers (#, ##, ###), lists (-, *), paragraphs
- E-ink optimization: partial refresh for page turns, full refresh every 10 pages
- Touch/swipe gesture navigation

**Image Viewer** (`image_viewer.h/cpp`):
- JPEGDEC library for JPEG decoding
- M5GFX built-in for PNG decoding
- Uses PSRAM for decode buffer (~2MB)
- Aspect-ratio preserving scaling to 960×540
- Gallery navigation with overlay UI

**WiFi Server** (`wifi_server.h/cpp`):
- ESP32 WiFi AP mode (SSID: "PaperS3-Files")
- HTTP server on port 80
- Embedded HTML upload interface (responsive design)
- Handles chunked file uploads
- Saves directly to LittleFS `/files/` directory

### Critical Configuration

**PSRAM Requirement:**
The M5Paper S3 uses OPI PSRAM which is REQUIRED for image decoding. This is configured in `platformio.ini`:
```ini
board_build.arduino.memory_type = qio_opi
build_flags = -DBOARD_HAS_PSRAM
```

**Partition Scheme:**
Uses 16MB flash with default partition (3MB APP / 9.9MB FATFS):
```ini
board_build.partitions = default_16MB.csv
```

**Display Refresh Management:**
E-ink displays require careful refresh handling to prevent ghosting:
- Partial refresh: fast (~300ms) but accumulates ghosting
- Full refresh: slow (~1s) but clears ghosting completely
- `REFRESH_INTERVAL` in `config.h` controls full refresh frequency (default: every 10 page turns)

## File Structure

```
src/
├── main.cpp              # State machine, setup(), loop()
├── config.h              # All configuration constants
├── utils.h               # Inline utility functions (drawCenteredText, etc.)
├── file_browser.{h,cpp}  # File listing and selection
├── markdown_reader.{h,cpp} # MD parsing and pagination
├── image_viewer.{h,cpp}  # Image decoding and display
└── wifi_server.{h,cpp}   # WiFi AP and HTTP upload server
```

## Key Configuration Constants

Located in `src/config.h`:

**WiFi:**
- `WIFI_SSID`: "PaperS3-Files"
- `WIFI_PASSWORD`: "papers3app"

**Display:**
- `SCREEN_WIDTH`: 960
- `SCREEN_HEIGHT`: 540
- `REFRESH_INTERVAL`: 10 (full refresh every N pages)

**Markdown:**
- `MD_LINE_HEIGHT`: 24 pixels
- `MD_MARGIN`: 20 pixels
- `MD_MAX_LINES_PER_PAGE`: 20

**Touch:**
- `SWIPE_THRESHOLD`: 50 pixels minimum for swipe
- `TAP_THRESHOLD`: 10 pixels maximum for tap

## Development Guidelines

### Touch Coordinate System
- Origin (0,0) is top-left
- Screen dimensions: 960×540
- Touch areas are checked in order (top-right WiFi toggle → file selection)
- Swipe detection uses `startX/startY` and `endX/endY` with threshold comparison

### E-ink Display Best Practices
- Use `M5.Display.clearDisplay()` sparingly (slow full refresh)
- Prefer `M5.Display.fillRect()` for partial updates
- Call `M5.Display.display()` only when ready to render
- Track refresh count to trigger periodic full refreshes
- Avoid rapid updates (wait >300ms between refreshes)

### Memory Management
- Available heap: ~300KB after initialization
- PSRAM: 8MB available for image buffers
- File reads: use 4KB chunks to avoid heap fragmentation
- Always close File handles after use

### Adding New File Types
1. Add extension check in `FileBrowser::scanFiles()`
2. Create new viewer class (follow existing viewer patterns)
3. Add new `AppState` to enum in `config.h`
4. Add state transition logic in `main.cpp` loop()
5. Initialize viewer in `setup()`

### Modifying WiFi Upload Interface
The HTML is embedded in `wifi_server.cpp` as the `UPLOAD_HTML` constant. To modify:
1. Edit the HTML string (uses modern responsive CSS)
2. Keep file size reasonable (embedded in firmware)
3. Maintain separate upload buttons for markdown vs images
4. Test on iPhone Safari (primary target platform)

## Testing Workflow

### Serial Monitor Output
Connect at 115200 baud to see:
- Filesystem initialization status
- File discovery logs
- Touch coordinate debugging
- State transition messages
- Error messages (file access, memory, etc.)

### Test Files
Use `SAMPLE.md` provided in root directory as test content.

### Common Issues

**Upload fails "Connecting..."**
- Device not in download mode (need RED LED)
- Wrong USB cable (must support data, not just charging)
- Try different USB port

**Image not displaying**
- File too large (>2MB may fail)
- PSRAM not configured (check platformio.ini)
- Unsupported format (only JPEG/PNG)

**Touch not responding**
- Check serial monitor for touch coordinates
- Verify touch area calculations in component handlers
- E-ink may need firmer taps than LCD

**Ghosting on screen**
- Normal for e-ink displays
- Increase `REFRESH_INTERVAL` in config.h for more frequent full refreshes
- Or manually trigger full refresh by calling `M5.Display.clearDisplay()` + `M5.Display.display()`

## Library Dependencies

Managed automatically by PlatformIO:
- **M5Unified** (≥0.1.16): Hardware abstraction, initialization
- **M5GFX** (≥0.1.16): Display graphics primitives
- **ArduinoJson** (≥7.0.4): JSON parsing (for future config features)
- **JPEGDEC**: JPEG image decoding

Built-in ESP32 libraries used:
- WiFi, WebServer, LittleFS, FS

## File System

Uses LittleFS (ESP32 internal flash):
- `/files/` - User-uploaded markdown and images
- `/config/` - Reserved for configuration (currently unused)
- Total available: ~12MB for user files

Files are automatically scanned on:
- Initial boot
- Exiting WiFi mode
- File browser initialization

## Reference Documentation

- **README.md**: User guide with features and troubleshooting
- **BUILD_INSTRUCTIONS.md**: Complete build and upload procedures
- **IPHONE_UPLOAD_GUIDE.md**: Step-by-step file transfer guide
- **PROJECT_SUMMARY.md**: Architecture overview and metrics

For M5Stack hardware specifics, allowed to fetch from docs.m5stack.com (see .claude/settings.local.json).
