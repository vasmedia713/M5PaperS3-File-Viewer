# M5PaperS3 File Viewer - Quick Start

## 5-Minute Setup

### What You Need
- M5Stack M5Paper S3 device
- USB-C data cable
- Computer with PlatformIO (VS Code extension)

---

## Upload in 3 Steps

### Step 1: Enter Download Mode
1. **Long press** the power button on the right side of device
2. **Wait for RED LED** to blink on back
3. **Release button** and immediately proceed to step 2

⚠️ **You have ~30 seconds before download mode exits**

### Step 2: Connect & Upload
1. **Plug USB-C cable** into device and computer
2. **Open VS Code**
3. Press **Ctrl+Alt+U** to build and upload

The terminal will show progress and say `Upload complete!` when done.

### Step 3: Verify Success
1. **Splash screen appears** on device (good sign!)
2. **File browser loads** showing available files
3. **Touch "WiFi" button** to enable wireless file upload

---

## Upload Files to Device

1. **Touch "WiFi"** on device → "WiFi Server Active" appears
2. **Connect computer** to "PaperS3-Files" WiFi network
3. **Password:** `papers3app`
4. **Open browser:** Go to IP address shown on device
5. **Upload files** (markdown, text, images, comics)
6. **Touch "WiFi"** again to exit and see files in browser

---

## If Upload Fails

| Problem | Fix |
|---------|-----|
| "Upload Failed" | Device exited download mode → repeat Step 1 |
| "Port not found" | Try different USB port or USB cable |
| Device won't respond | Hold power button 10+ seconds to force restart |
| Can't find WiFi network | Make sure you touched "WiFi" button and see "WiFi Server Active" message |

---

## Supported Formats

✓ `.md` - Markdown documents  
✓ `.txt` - Plain text  
✓ `.jpg`, `.png`, `.webp` - Images  
✓ `.cbz` - Comic book archives  

---

## Keyboard Shortcuts

| What | How |
|------|-----|
| Build & Upload | **Ctrl+Alt+U** |
| Monitor Logs | **Ctrl+Alt+M** |
| Clean Build | **Ctrl+Alt+C** |

---

## Detailed Guide

For troubleshooting, file size limits, and full setup instructions, see **[UPLOAD_TUTORIAL.md](UPLOAD_TUTORIAL.md)**
