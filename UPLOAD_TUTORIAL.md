# M5PaperS3 File Viewer - Upload & Update Tutorial

This guide walks you through building and uploading the M5PaperS3 File Viewer application to your device.

---

## Prerequisites

### Hardware
- **M5Stack M5Paper S3** device
- **USB-C cable** (data + power capable - not just charging cable)
- **Computer** with Windows/Mac/Linux

### Software
- **PlatformIO** installed (VS Code extension or CLI)
- **Python 3.6+** (required by PlatformIO)
- **Git** (optional, for version control)

---

## Step 1: Install PlatformIO

### Option A: VS Code Extension (Recommended)
1. Open **Visual Studio Code**
2. Go to **Extensions** (Ctrl+Shift+X)
3. Search for **"PlatformIO IDE"** by PlatformIO
4. Click **Install**
5. Wait for installation to complete (may take 2-3 minutes)
6. Restart VS Code

### Option B: Command Line
```bash
pip install platformio
```

---

## Step 2: Prepare Your Device

### Enter Download Mode
Before you can upload, the M5Paper S3 must be in **download mode** to accept new firmware.

**Steps:**
1. **Locate the power button** on the right side of the device
2. **Long press** the power button (hold for 3+ seconds)
3. **Watch the back LED light** on the device
4. **Wait for RED LED flash** - this indicates download mode is active
5. **Release the button** once you see the red LED
6. **Act quickly** - you have ~30 seconds before the device exits download mode

**Visual Indicator:**
- Red LED blinking = Download mode ✓ Ready to upload
- No LED/other colors = Not in download mode ✗ Try again

---

## Step 3: Connect Device via USB

1. **Plug in the USB-C cable** to the device's USB-C port (bottom of device)
2. **Plug the other end** into your computer's USB port
3. **Verify connection** - Windows will show a USB device connection notification
4. **Keep device in download mode** - don't close the connection

---

## Step 4: Build & Upload

### Using VS Code & PlatformIO

**Step 1: Open the Project**
1. Open VS Code
2. Click **File → Open Folder**
3. Navigate to the M5PaperS3-File-Viewer folder
4. Click **Select Folder**

**Step 2: Start Upload**
1. Click the **PlatformIO icon** (alien head) in the left sidebar
2. Expand **"PlatformIO: Quick Access"**
3. Under "General", click **"Upload"**
4. Or use keyboard shortcut: **Ctrl+Alt+U** (Windows) or **Cmd+Shift+U** (Mac)

**Step 3: Wait for Build & Upload**
The terminal will show progress:
```
Compiling .pio/build/m5stack-papers3/...
Building firmware...
[SUCCESS] Built project
Uploading .pio/build/m5stack-papers3/firmware.bin...
[===          ] 33%
[======       ] 66%
[==============] 100%
Upload complete!
```

### Using Command Line

```bash
# Navigate to project folder
cd C:\Users\josev\Projects\M5PaperS3-File-Viewer

# Build only (no upload)
pio run

# Build and upload
pio run --target upload

# Monitor serial output after upload
pio device monitor --baud 115200
```

---

## Step 5: Verify Upload Success

### Serial Monitor
1. Click **PlatformIO icon** in VS Code sidebar
2. Click **"Monitor"** under "General"
3. Set baud rate to **115200** in the dropdown
4. Watch for boot messages:

```
=== M5Paper S3 File Viewer ===
Starting initialization...
Initializing M5...
M5.begin() completed
Display width: 540
Display height: 960
...
=== Initialization complete! ===
```

### Visual Confirmation on Device
1. **Splash screen appears** with "OmniBrowser" logo (good sign!)
2. **File browser loads** showing your files
3. **WiFi button** visible in top-right corner
4. **Device is responsive** to touches

If you see all of these, **upload was successful!** ✓

---

## Step 6: Upload Files to Device

Now that the app is running, you can upload files wirelessly.

### Using the WiFi Upload Interface

**Step 1: Activate WiFi Mode**
1. Touch the **"WiFi"** button in the top-right corner of the device
2. Wait for WiFi to initialize (1-2 seconds)
3. You'll see:
   - WiFi Server Active
   - SSID: "PaperS3-Files"
   - Password: "papers3app"
   - IP address: "http://192.168.x.x"

**Step 2: Connect Your Computer**
1. On your computer, go to **WiFi settings**
2. Look for network **"PaperS3-Files"**
3. Enter password: **"papers3app"**
4. Connect to the network

**Step 3: Open Upload Page**
1. Open a **web browser** (Chrome, Safari, Firefox, Edge)
2. Go to the IP address shown on device (e.g., `http://192.168.4.1`)
3. You'll see the upload interface with file type options:
   - 📝 Markdown Document (.md)
   - 📄 Text File (.txt)
   - 🖼️ Image (.jpg, .jpeg, .png, .webp)
   - 📚 Comic Book (.cbz)

**Step 4: Upload Files**
1. Click **"Choose File"** in the section for your file type
2. Select your file from your computer
3. Click **"Upload [Type]"**
4. Wait for success message: "✓ File uploaded successfully!"
5. Repeat for additional files

**Step 5: Exit WiFi Mode**
1. Return to device screen
2. Touch the **"WiFi"** button again to turn off WiFi server
3. File browser will refresh automatically
4. Your uploaded files now appear in the file list

---

## Troubleshooting

### "Upload Failed" or Timeout Error

**Problem:** Upload stops or times out

**Solution:**
1. **Exit download mode** - the device only stays in download mode for ~30 seconds
2. **Re-enter download mode:**
   - Long press power button until RED LED blinks
   - Immediately try upload again
3. **Check USB cable** - use a data cable, not just a charging cable
4. **Try different USB port** - sometimes specific ports have better firmware upload support

---

### "Memory Error" on Device

**Problem:** Device shows "Memory error" when trying to open files

**Solution:**
1. Files are too large or too many files in `/files/` directory
2. Erase the device and restart:
   ```bash
   pio run --target erase
   ```
3. Re-upload firmware:
   ```bash
   pio run --target upload
   ```

---

### Device Won't Respond After Upload

**Problem:** Device appears frozen or unresponsive

**Solution:**
1. **Force restart:** Hold power button for 10+ seconds until device completely powers off
2. **Wait:** Let device sit for 5 seconds
3. **Power on:** Press power button briefly to turn on
4. **Watch splash screen:** Should appear in 2-3 seconds

---

### Can't Connect to WiFi Upload Server

**Problem:** Can't find "PaperS3-Files" WiFi network

**Solution:**
1. **Verify WiFi is enabled:**
   - Touch "WiFi" button on device screen
   - Watch for "WiFi Server Active" message
   - Check WiFi IP address is displayed

2. **Wait for startup:**
   - WiFi takes 2-3 seconds to initialize
   - Watch for the full message screen

3. **Check WiFi list:**
   - Refresh your computer's WiFi network list
   - Look for "PaperS3-Files" (case-sensitive)

4. **Check password:**
   - Default: `papers3app` (all lowercase)
   - Make sure caps lock is off

---

### Image Doesn't Display Correctly

**Problem:** Image appears stretched, cropped, or won't load

**Solution:**
1. **File format:** Verify file is `.jpg`, `.jpeg`, `.png`, or `.webp`
2. **File size:** Large images (>2MB) may fail to load
3. **File integrity:** Try uploading a different image to test
4. **Check serial monitor:**
   - Connect via serial monitor
   - Error messages will show what went wrong
   - Look for: "Failed to open image", "Memory error", etc.

---

### Serial Monitor Shows Gibberish

**Problem:** Serial output is unreadable/garbled text

**Solution:**
1. **Check baud rate** - must be exactly **115200**
2. **Disconnect/reconnect** USB cable
3. **Restart device** - turn off and on
4. **Change USB port** - try different port on computer

---

## File Size Limits

| File Type | Max Size | Notes |
|-----------|----------|-------|
| `.md` (Markdown) | ~300KB | Limited by heap memory |
| `.txt` (Text) | ~300KB | Limited by heap memory |
| `.jpg` (JPEG) | ~2MB | Uses PSRAM buffer |
| `.png` (PNG) | ~2MB | Uses PSRAM buffer |
| `.webp` (WebP) | ~2MB | Uses PSRAM buffer |
| `.cbz` (Comic) | Limited by SD card | Each page extracted on-demand |

---

## Complete Workflow Example

Here's a typical session:

```
1. Device in download mode (RED LED blinking)
   ↓
2. Run: pio run --target upload
   ↓
3. Wait for "Upload complete!" message
   ↓
4. Watch splash screen appear on device
   ↓
5. File browser loads (empty)
   ↓
6. Touch "WiFi" button
   ↓
7. Device shows IP: http://192.168.4.1
   ↓
8. Connect computer to "PaperS3-Files" WiFi
   ↓
9. Open browser, go to http://192.168.4.1
   ↓
10. Upload markdown files, text files, images, CBZ comics
   ↓
11. Touch "WiFi" button again to exit WiFi mode
   ↓
12. Files appear in file browser
   ↓
13. Select file to read/view
```

---

## Quick Reference: Commands

### PlatformIO Commands

```bash
# Build only (no upload)
pio run

# Build and upload
pio run --target upload

# Upload (shortcut)
pio run -t upload

# Monitor serial output
pio device monitor --baud 115200

# Erase flash memory completely
pio run --target erase

# Clean build directory
pio run --target clean

# Full clean build and upload
pio run --target clean && pio run --target upload
```

### Device Controls

| Action | Control |
|--------|---------|
| Power On/Off | Press power button (right side) |
| Enter Download Mode | Long press power button until RED LED blinks |
| Exit WiFi Mode | Touch "WiFi" button on screen |
| Navigate Files | Tap/swipe on file names |
| Page Navigation | Swipe left (next), Swipe right (previous) |
| Exit Reader | Tap top-left corner |
| Gallery Navigation (Images) | Swipe left/right |

---

## Support & Debugging

### Check Device Logs
Connect to serial monitor to see detailed logs:

```bash
pio device monitor --baud 115200
```

Look for these messages:
- ✓ `File loaded successfully` - file opened
- ✓ `Parsed into X pages` - pagination successful
- ✗ `Failed to open file` - file access error
- ✗ `Memory error` - insufficient heap/PSRAM

### Hardware Info
- **Display:** 960×540 resolution (portrait)
- **Processor:** ESP32-S3 (dual-core 240 MHz)
- **RAM:** 8MB PSRAM + ~300KB heap
- **Storage:** 16MB flash (12MB user files)
- **Touch:** Capacitive touchscreen

---

## Tips for Success

1. **Always use a data USB cable** - charging-only cables won't work
2. **Stay in download mode** - don't let 30 seconds elapse
3. **Use short filenames** - long filenames (>30 chars) get truncated
4. **Test with small files first** - before uploading large CBZ archives
5. **Monitor serial output** - it shows helpful debugging info
6. **WiFi password is case-sensitive** - `papers3app` (all lowercase)
7. **Keep files organized** - use meaningful names for easy browsing

---

## Next Steps

After uploading:
- Read the [README.md](README.md) for feature overview
- Check [CLAUDE.md](CLAUDE.md) for architecture details
- See [iPhone_Upload_Guide.md](IPHONE_UPLOAD_GUIDE.md) for phone-based uploads

Good luck! 📖✨
