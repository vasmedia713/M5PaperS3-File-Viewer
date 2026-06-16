# Build and Flash Instructions

Complete guide for compiling and uploading the M5Paper S3 File Viewer firmware.

## Prerequisites

### Hardware

- M5Stack M5Paper S3 device
- USB-C cable (data capable, not charge-only)
- Computer (Windows, macOS, or Linux)

### Software

Choose ONE method:

**Method A: PlatformIO (Recommended)**
- [Visual Studio Code](https://code.visualstudio.com/)
- PlatformIO IDE extension
- Python 3.7+ (usually auto-installed with PlatformIO)

**Method B: Arduino IDE**
- [Arduino IDE 2.0+](https://www.arduino.cc/en/software)
- ESP32 board support package

---

## Method A: PlatformIO (Recommended)

### Step 1: Install Visual Studio Code

1. Download from https://code.visualstudio.com/
2. Install for your operating system
3. Launch VS Code

### Step 2: Install PlatformIO Extension

1. Open VS Code
2. Click Extensions icon (left sidebar) or press `Ctrl+Shift+X`
3. Search for "PlatformIO IDE"
4. Click Install
5. Wait for installation (may take 5-10 minutes)
6. Restart VS Code when prompted

### Step 3: Open Project

1. Open VS Code
2. File → Open Folder
3. Navigate to `M5_Paper_Project`
4. Click Select Folder

You should see the project structure in the left sidebar.

### Step 4: Install Dependencies

Dependencies are automatically installed when you first build. To manually install:

1. Open PlatformIO toolbar (bottom of VS Code)
2. Click "Build" (checkmark icon)
3. Wait for libraries to download

Libraries that will be installed:
- M5Unified (v0.1.16+)
- M5GFX (v0.1.16+)
- ArduinoJson (v7.0.4+)
- JPEGDEC

### Step 5: Connect M5Paper S3

1. **Connect USB-C cable** from computer to M5Paper S3
2. **Enter download mode:**
   - Long press power button (side of device)
   - Hold for 3-5 seconds
   - Wait until **back LED blinks RED**
   - Release button
   - Device is now in download mode

**Important:** You have about 30 seconds to start upload after entering download mode. If it times out, repeat this step.

### Step 6: Build and Upload

**Option A: Using PlatformIO Toolbar**

1. Click PlatformIO icon in left sidebar (alien head)
2. Under "PROJECT TASKS" → env:m5stack-papers3
3. Click "Upload" (right arrow icon)
4. Wait for compilation and upload (2-5 minutes first time)

**Option B: Using VS Code Terminal**

1. Open terminal: Terminal → New Terminal
2. Run command:
   ```bash
   pio run --target upload
   ```
3. Wait for completion

### Step 7: Monitor Serial Output (Optional)

To see debug output:

1. Click PlatformIO icon
2. Under "PROJECT TASKS" → env:m5stack-papers3
3. Click "Monitor"

Or use terminal:
```bash
pio device monitor
```

Press `Ctrl+C` to exit monitor.

### Troubleshooting PlatformIO

**"No such file or directory: platformio"**
- Solution: Restart VS Code
- Or add PlatformIO to PATH (check PlatformIO documentation)

**"Failed to connect to ESP32"**
- Solution: Re-enter download mode (long press power button until RED LED)
- Check USB cable (use different cable/port)
- Install CH340/CP2102 drivers if needed

**"Library dependencies failed"**
- Solution: Click PlatformIO icon → Clean
- Then try upload again
- Check internet connection

**Upload stuck at "Connecting..."**
- Solution: Hold power button, wait for RED LED, immediately release
- Start upload within 10 seconds
- Try different USB port

---

## Method B: Arduino IDE

### Step 1: Install Arduino IDE

1. Download from https://www.arduino.cc/en/software
2. Choose version 2.0 or later
3. Install for your operating system
4. Launch Arduino IDE

### Step 2: Add ESP32 Board Support

1. Open Arduino IDE
2. Go to **File → Preferences**
3. In "Additional Board Manager URLs", add:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
4. Click OK
5. Go to **Tools → Board → Boards Manager**
6. Search for "esp32"
7. Install **"esp32" by Espressif Systems** (version 2.0.11 or later)
8. Wait for installation (may take 10+ minutes)

### Step 3: Install Required Libraries

1. Go to **Sketch → Include Library → Manage Libraries**
2. Install each of the following:

**M5Unified:**
- Search: "M5Unified"
- Install: "M5Unified" by M5Stack (v0.1.16+)
- Click "Install All" when prompted for dependencies

**M5GFX:**
- Search: "M5GFX"
- Install: "M5GFX" by M5Stack (v0.1.16+)

**ArduinoJson:**
- Search: "ArduinoJson"
- Install: "ArduinoJson" by Benoit Blanchon (v7.0.4+)

**JPEGDEC:**
- Search: "JPEGDEC"
- Install: "JPEGDEC" by bitbank2

### Step 4: Configure Board Settings

1. Go to **Tools → Board → ESP32 Arduino**
2. Select **"ESP32S3 Dev Module"**

3. Configure the following settings:

| Setting | Value |
|---------|-------|
| USB CDC On Boot | Enabled |
| CPU Frequency | 240MHz (WiFi) |
| Core Debug Level | None (or Info for debugging) |
| Flash Frequency | 80MHz |
| Flash Mode | QIO |
| Flash Size | 16MB (128Mb) |
| Partition Scheme | Default 16MB with spiffs (3MB APP/9.9MB FATFS) |
| PSRAM | **OPI PSRAM** ⚠️ Important! |
| Upload Speed | 921600 |
| USB Mode | Hardware CDC and JTAG |

**⚠️ Critical:** PSRAM **must** be set to "OPI PSRAM" or the device won't work.

### Step 5: Prepare Source Code

Arduino IDE expects a single .ino file. You have two options:

**Option A: Create Sketch from main.cpp**

1. Create new folder: `M5PaperFileViewer`
2. Create file: `M5PaperFileViewer.ino`
3. Copy ALL .cpp file contents into .ino:
   - Start with `main.cpp`
   - Append `file_browser.cpp`
   - Append `markdown_reader.cpp`
   - Append `image_viewer.cpp`
   - Append `wifi_server.cpp`
4. Copy all .h files to same folder
5. Open `M5PaperFileViewer.ino` in Arduino IDE

**Option B: Rename and Combine**

1. Copy `M5_Paper_Project` folder
2. Rename to `M5PaperFileViewer`
3. In `src` folder:
   - Rename `main.cpp` to `M5PaperFileViewer.ino`
   - Move all files from `src` to root folder
4. Open `M5PaperFileViewer.ino`

### Step 6: Connect M5Paper S3

1. Connect USB-C cable
2. **Enter download mode:**
   - Long press power button
   - Wait for RED LED blink
   - Release

3. **Select Port:**
   - Tools → Port
   - Select the COM port (Windows) or /dev/cu.* (Mac) or /dev/ttyUSB* (Linux)
   - Usually shows as "USB JTAG/serial debug unit"

### Step 7: Compile and Upload

1. Click **Verify** (checkmark) to compile
   - First compile takes 5-10 minutes
   - Should show "Compilation complete"

2. Click **Upload** (right arrow)
   - Must be in download mode (RED LED)
   - Upload takes 2-5 minutes
   - Progress bar shows upload status

3. Wait for "Done uploading"

### Step 8: Monitor Serial (Optional)

1. Tools → Serial Monitor
2. Set baud rate: **115200**
3. You'll see debug output from device

### Troubleshooting Arduino IDE

**"Sketch too big"**
- Solution: Check Partition Scheme is set to "Default 16MB with spiffs"
- Ensure Flash Size is 16MB

**"Compilation error: M5Unified.h: No such file"**
- Solution: Install M5Unified library (Step 3)
- Restart Arduino IDE after installing

**"Brownout detector triggered"**
- Solution: Use better USB cable
- Try different USB port (prefer USB 3.0)
- Use powered USB hub

**"Timed out waiting for packet header"**
- Solution: Re-enter download mode
- Press and hold power button until RED LED
- Upload immediately after seeing RED LED

**"Could not open port"**
- Solution: Close Serial Monitor before uploading
- Check port selection
- Install CH340 or CP2102 drivers

**PSRAM errors**
- Solution: MUST set Tools → PSRAM → "OPI PSRAM"
- This is the most common mistake

---

## Verifying Successful Upload

After upload completes:

1. **Device should auto-restart**
   - Screen shows "M5Paper S3 File Viewer"
   - Then "Initializing..."
   - Finally shows "File Browser"

2. **Serial Monitor shows:**
   ```
   M5Paper S3 File Viewer
   ======================
   LittleFS mounted successfully
   Found 0 markdown files and 0 images
   Initialization complete!
   ```

3. **Screen displays:**
   - "File Browser" header
   - "No files found"
   - "Connect to WiFi to upload"

**Success!** Your device is ready to use.

---

## First-Time Setup After Flash

### Create Test Files Directory

The filesystem is automatically initialized on first boot.

### Upload Test Files

1. Tap top-right corner on device
2. Connect phone/computer to "PaperS3-Files" (password: papers3app)
3. Open browser to http://192.168.4.1
4. Upload the included `SAMPLE.md` file
5. Upload a test image (any .jpg or .png)

### Test Functionality

1. Exit WiFi mode (tap top-right)
2. Tap on `SAMPLE.md` to open
3. Test navigation (swipe left/right)
4. Return to browser (tap top-left)
5. Tap on image to open image viewer

---

## Updating Firmware

To update to a new version:

1. Flash new firmware (same process as initial upload)
2. **⚠️ This will erase all uploaded files**
3. To preserve files: manually backup via serial/SD card (advanced)

---

## Advanced: Partial Upload (Modified Files Only)

If you only modified certain files:

### PlatformIO
```bash
pio run --target upload
```
(Automatically only compiles changed files)

### Arduino IDE
1. Make changes
2. Click Upload
3. Arduino automatically detects changes

---

## Build Flags Reference

From `platformio.ini`:

```ini
-DBOARD_HAS_PSRAM          # Enable PSRAM support
-DARDUINO_M5STACK_PAPERS3  # M5Paper S3 specific
-DCORE_DEBUG_LEVEL=3       # Debug verbosity (0-5)
```

To change debug level:
- 0 = None
- 1 = Error
- 2 = Warn
- 3 = Info
- 4 = Debug
- 5 = Verbose

---

## Filesystem Erase (Clean Flash)

To completely erase flash and start fresh:

### PlatformIO
```bash
pio run --target erase
pio run --target upload
```

### Arduino IDE
1. Tools → Erase Flash → "All Flash Contents"
2. Upload sketch

**⚠️ Warning:** This erases ALL data including uploaded files.

---

## Common Upload Issues

### Issue: Upload fails at 0%

**Symptoms:**
```
Connecting........._____....
```

**Solutions:**
1. Device not in download mode
2. Long press power button until RED LED
3. Start upload within 10 seconds

### Issue: Upload fails at random percentage

**Symptoms:**
```
Writing at 0x00050000... (50%)
A fatal error occurred: MD5 of file does not match
```

**Solutions:**
1. Bad USB cable - try different cable
2. USB hub - try direct connection
3. Unstable power - try different USB port
4. Re-enter download mode and retry

### Issue: Serial port not detected

**Windows:**
- Install CH340 driver: http://www.wch.cn/downloads/CH341SER_EXE.html
- Or CP2102 driver: https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers

**macOS:**
- Usually works without drivers
- If not, install CH340 driver for macOS

**Linux:**
- Add user to dialout group:
  ```bash
  sudo usermod -a -G dialout $USER
  ```
- Logout and login
- May need CH340 kernel module

---

## Compilation Time Estimates

First compilation (downloads libraries + compiles):
- **PlatformIO**: 5-10 minutes
- **Arduino IDE**: 3-7 minutes

Subsequent compilations:
- **PlatformIO**: 30-60 seconds
- **Arduino IDE**: 20-40 seconds

Upload time:
- **Both methods**: 30-90 seconds

---

## Storage Space

Compiled binary size: ~1.5MB

Remaining flash (for files): ~12MB
- Enough for 100+ markdown documents
- Or 10-20 high-resolution photos
- Or mix of both

---

## Development Workflow

For active development:

1. **Make code changes**
2. **Quick test:**
   ```bash
   pio run         # Compile only
   ```
3. **If successful:**
   ```bash
   pio run --target upload
   ```
4. **Monitor output:**
   ```bash
   pio device monitor
   ```

Keyboard shortcuts (PlatformIO):
- Build: `Ctrl+Alt+B`
- Upload: `Ctrl+Alt+U`
- Serial Monitor: `Ctrl+Alt+S`

---

## Success Checklist

Before considering build complete:

- [ ] No compilation errors
- [ ] Upload shows 100% complete
- [ ] Device restarts automatically
- [ ] Screen shows "File Browser"
- [ ] Serial monitor shows initialization messages
- [ ] WiFi mode can be enabled (tap top-right)
- [ ] Can upload test file
- [ ] Test file appears in browser
- [ ] Can open and read test file

---

## Next Steps

After successful flash:

1. Read **IPHONE_UPLOAD_GUIDE.md** for file transfer instructions
2. Upload sample content
3. Customize WiFi credentials in `src/config.h` if desired
4. Experiment with your own markdown files

---

## Getting Help

If you encounter issues:

1. Check this guide's troubleshooting sections
2. Review serial monitor output (115200 baud)
3. Search error messages
4. Check M5Stack forums
5. Verify board settings match exactly

Common mistakes:
- ❌ PSRAM not set to "OPI PSRAM"
- ❌ Wrong partition scheme
- ❌ Not in download mode
- ❌ Bad USB cable
- ❌ Wrong board selected

---

**You're now ready to build and flash the M5Paper S3 File Viewer!** 🎉
