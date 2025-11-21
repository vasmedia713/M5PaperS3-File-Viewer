# M5Paper S3 File Viewer

A complete file viewing application for the M5Stack M5Paper S3 e-ink device with wireless file transfer capabilities.

## Features

- **File Browser**: Navigate markdown documents and images stored on the device
- **Markdown Reader**: Paginated text rendering with basic formatting support
- **Image Viewer**: Full-screen photo viewing with gesture navigation
- **WiFi Upload**: Transfer files wirelessly from iPhone or any device via web interface
- **E-ink Optimization**: Smart refresh management to prevent ghosting

## Hardware Requirements

- M5Stack M5Paper S3 (ESP32-S3, 960×540 e-ink display)
- USB-C cable for programming
- iPhone or computer for file uploads

## Software Requirements

### PlatformIO (Recommended)

- PlatformIO IDE or PlatformIO Core
- Visual Studio Code (optional, but recommended)

### Arduino IDE

- Arduino IDE 2.0 or later
- ESP32 board support (version 2.0.11 or later)

## Installation

### Method 1: PlatformIO (Recommended)

1. **Install PlatformIO**
   - Install Visual Studio Code
   - Install PlatformIO IDE extension from VS Code marketplace

2. **Open Project**
   ```bash
   cd M5_Paper_Project
   code .
   ```

3. **Build and Upload**
   - Connect M5Paper S3 via USB-C
   - Press the power button until the LED blinks RED (download mode)
   - In VS Code, click "Upload" in PlatformIO toolbar
   - Or use terminal:
     ```bash
     pio run --target upload
     ```

4. **Monitor Serial Output** (optional)
   ```bash
   pio device monitor
   ```

### Method 2: Arduino IDE

1. **Install Arduino IDE**
   - Download from https://www.arduino.cc/en/software

2. **Install ESP32 Board Support**
   - Open Arduino IDE
   - Go to File → Preferences
   - Add to "Additional Board Manager URLs":
     ```
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
     ```
   - Go to Tools → Board → Boards Manager
   - Search "esp32" and install "esp32" by Espressif Systems (version 2.0.11+)

3. **Install Required Libraries**
   - Go to Sketch → Include Library → Manage Libraries
   - Install the following:
     - M5Unified (version 0.1.16+)
     - M5GFX (version 0.1.16+)
     - ArduinoJson (version 7.0.4+)
     - JPEGDEC by bitbank2

4. **Configure Board Settings**
   - Tools → Board → ESP32 Arduino → ESP32S3 Dev Module
   - Tools → PSRAM → "OPI PSRAM"
   - Tools → Partition Scheme → "Default 16MB with spiffs (3MB APP/9.9MB FATFS)"
   - Tools → Upload Speed → 921600
   - Tools → CPU Frequency → 240MHz

5. **Open and Upload**
   - Open `src/main.cpp` in Arduino IDE
   - Connect M5Paper S3 via USB-C
   - Long press power button until LED blinks RED
   - Click Upload button

## First Time Setup

### Entering Download Mode

The M5Paper S3 needs to be in download mode for programming:

1. Connect USB-C cable to computer
2. **Long press** the power button (on the side)
3. Wait until the back LED blinks **RED**
4. Release the button
5. Device is now in download mode
6. Upload your code immediately

### Initial Filesystem

The filesystem is automatically formatted on first boot. The following structure is created:

```
/files/          # User files (markdown and images)
/config/         # Configuration files
```

## Usage Guide

### File Browser

- **Main screen** shows two sections:
  - DOCUMENTS (.md files)
  - IMAGES (.jpg, .png files)
- **Tap** a file to open it
- **Touch top-right corner** to toggle WiFi upload mode

### Markdown Reader

- **Tap left side** or **swipe right**: Previous page
- **Tap right side** or **swipe left**: Next page
- **Tap top-left corner**: Return to file browser
- Supports basic markdown:
  - Headers (# ## ###)
  - Bullet lists (- *)
  - Plain text with word wrapping

### Image Viewer

- **Swipe left**: Next image
- **Swipe right**: Previous image
- **Tap screen**: Show/hide overlay
- **Tap BACK** (in overlay): Return to file browser
- Auto-scales images to fit 960×540 display
- Supports JPEG and PNG formats

### WiFi File Upload

1. **Enable WiFi Mode**
   - In file browser, tap top-right corner
   - Wait for WiFi server to start

2. **Connect from iPhone/Computer**
   - Open WiFi settings
   - Connect to network: `PaperS3-Files`
   - Password: `papers3app`

3. **Upload Files**
   - Open Safari (iPhone) or any browser
   - Navigate to: `http://192.168.4.1`
   - Upload markdown or image files
   - Files are saved to `/files/` directory

4. **Exit WiFi Mode**
   - Tap top-right corner again
   - Returns to file browser with updated file list

## File Transfer from iPhone

### Step-by-Step Instructions

1. **Prepare Files on iPhone**
   - Save markdown files (.md) to Files app
   - Save photos to Photos app or Files app

2. **Enable WiFi on M5Paper S3**
   - Power on M5Paper S3
   - Tap top-right corner of screen
   - Note the WiFi credentials displayed

3. **Connect iPhone to M5Paper S3**
   - Open Settings → WiFi
   - Find and tap "PaperS3-Files"
   - Enter password: "papers3app"
   - Wait for connection

4. **Open Upload Page**
   - Open Safari
   - Go to: `http://192.168.4.1`
   - You'll see the upload interface

5. **Upload Markdown**
   - Tap "Choose .md File" under Markdown Document
   - Browse to your markdown file
   - Tap "Upload Markdown"
   - Wait for confirmation

6. **Upload Images**
   - Tap "Choose Image" under Image section
   - Select photo from Photos or Files
   - Tap "Upload Image"
   - Wait for confirmation

7. **Disconnect and View**
   - Tap top-right corner on M5Paper S3 to exit WiFi mode
   - Your files will appear in the file browser

### Supported File Types

- **Markdown**: .md
- **Images**: .jpg, .jpeg, .png

### File Size Limits

- Maximum file size: ~2MB (limited by available RAM)
- Recommended image resolution: 1920×1080 or lower
- Large images are automatically scaled to fit display

## Display Optimization

The M5Paper S3 uses e-ink technology, which requires special handling:

- **Partial Refresh**: Used for most page turns (fast, minimal ghosting)
- **Full Refresh**: Automatically triggered every 10 page turns
- **Refresh Counter**: Tracked internally to prevent screen degradation

### Preventing Ghosting

The application automatically manages display refresh to minimize ghosting:
- Quick page turns use partial refresh
- Every 10th page uses full refresh to clear residual pixels
- Images always use full refresh for best quality

## Troubleshooting

### Upload Fails - Cannot Enter Download Mode

**Solution:**
1. Disconnect USB cable
2. Hold power button for 6 seconds (full power off)
3. Connect USB cable
4. Long press power button until RED LED blinks
5. Try upload immediately

### WiFi Connection Fails

**Symptoms:** Cannot find "PaperS3-Files" network

**Solutions:**
- Ensure WiFi mode is activated (check screen displays WiFi info)
- Wait 10 seconds after enabling WiFi
- Try forgetting other WiFi networks on iPhone temporarily
- Restart M5Paper S3 and try again

### File Upload Returns Error

**Symptoms:** Upload button completes but shows error

**Solutions:**
- Check file size (must be < 2MB)
- Verify file extension (.md, .jpg, .jpeg, .png only)
- Ensure filesystem isn't full
- Try smaller file first
- Check serial monitor for detailed error messages

### Filesystem Full

**Symptoms:** Uploads fail, cannot save files

**Solutions:**
- Connect to serial monitor (115200 baud)
- Delete old files through file browser (feature not implemented yet)
- Or reflash firmware (erases all files):
  ```bash
  pio run --target erase
  pio run --target upload
  ```

### Images Not Displaying

**Symptoms:** Image viewer shows "Error loading image"

**Solutions:**
- Verify image format (JPEG or PNG only)
- Try converting image to JPEG with lower quality
- Reduce image resolution to 1920×1080 or less
- Some PNG formats may not be supported - try JPEG

### Markdown Not Rendering Correctly

**Symptoms:** Text overlaps or formatting issues

**Solutions:**
- Use simple markdown (# headers, - lists, plain text)
- Avoid complex markdown features (tables, code blocks)
- Use shorter line lengths (< 80 characters)
- Check file encoding (must be UTF-8)

### Display Shows Ghosting

**Symptoms:** Previous page content visible

**Solutions:**
- This is normal for e-ink displays
- Full refresh occurs automatically every 10 pages
- Manually trigger full refresh by navigating 10+ pages
- Ghosting will clear after next full refresh

### Touch Not Responsive

**Symptoms:** Screen doesn't respond to taps/swipes

**Solutions:**
- Ensure you're tapping on active areas (see usage guide)
- Try firmer taps (touch sensor needs good contact)
- Screen may need calibration - restart device
- Clean screen with soft cloth

### Serial Monitor Shows Errors

**Common Errors:**

1. **"LittleFS Mount Failed"**
   - Filesystem needs initialization
   - Reflash firmware with erase option
   - Check board settings (PSRAM, partition scheme)

2. **"Failed to open file"**
   - File doesn't exist
   - Check filename in serial output
   - Verify files uploaded correctly

3. **"Memory error"**
   - File too large
   - Try smaller image
   - Reduce image dimensions

## Performance Notes

### Memory Usage

- Available heap: ~300KB after initialization
- Image decode buffer: ~2MB (uses PSRAM)
- Recommended file sizes:
  - Markdown: < 100KB
  - Images: < 1MB

### Display Refresh Times

- Partial refresh: ~300ms
- Full refresh: ~1000ms
- Image load and decode: 1-3 seconds

### WiFi Performance

- Upload speed: ~100KB/s
- Connection time: 5-10 seconds
- Concurrent clients: Up to 4

## Development Notes

### Adding Custom Markdown Features

Edit `src/markdown_reader.cpp`, function `applyMarkdownFormatting()`:

```cpp
void MarkdownReader::applyMarkdownFormatting(const String& line, int& y) {
    // Add custom formatting here
    if (line.startsWith("**")) {
        // Bold text handling
    }
}
```

### Customizing Upload Page

Edit the HTML in `src/wifi_server.cpp`, constant `UPLOAD_HTML`.

### Changing WiFi Credentials

Edit `src/config.h`:

```cpp
#define WIFI_SSID "YourSSID"
#define WIFI_PASSWORD "YourPassword"
```

### Adjusting Display Settings

Edit `src/config.h`:

```cpp
#define REFRESH_INTERVAL 10  // Full refresh every N pages
#define MD_LINE_HEIGHT 24    // Markdown line spacing
#define MD_MARGIN 20         // Page margins
```

## Project Structure

```
M5_Paper_Project/
├── platformio.ini              # PlatformIO configuration
├── README.md                   # This file
├── src/
│   ├── main.cpp               # Main application logic
│   ├── config.h               # Configuration constants
│   ├── utils.h                # Utility functions
│   ├── file_browser.h/cpp     # File browser module
│   ├── markdown_reader.h/cpp  # Markdown reader module
│   ├── image_viewer.h/cpp     # Image viewer module
│   └── wifi_server.h/cpp      # WiFi upload server
└── data/                      # Optional data files
```

## Libraries Used

- **M5Unified**: Hardware abstraction for M5Stack devices
- **M5GFX**: Graphics library for M5 displays
- **ArduinoJson**: JSON parsing (for future config features)
- **JPEGDEC**: JPEG image decoding
- **LittleFS**: Internal filesystem
- **WiFi**: ESP32 WiFi stack
- **WebServer**: HTTP server for file uploads

## License

This project is open source and available for modification and distribution.

## Credits

Created for the M5Stack M5Paper S3 e-ink device.

## Support

For issues and questions:
- Check the troubleshooting section above
- Review serial monitor output (115200 baud)
- Check M5Stack forums and documentation
- Review ESP32 Arduino core documentation

## Version History

**v1.0.0** - Initial release
- File browser with markdown and image support
- Markdown reader with pagination
- Image viewer with gesture navigation
- WiFi file upload from any device
- E-ink optimized display refresh
