# iPhone File Upload Guide for M5Paper S3

Complete step-by-step instructions for transferring files from your iPhone to the M5Paper S3.

## Quick Start (5 Steps)

1. **Enable WiFi on M5Paper S3**: Tap top-right corner
2. **Connect iPhone to "PaperS3-Files"**: Password is `papers3app`
3. **Open Safari**: Go to `http://192.168.4.1`
4. **Upload your files**: Use the upload buttons
5. **Exit WiFi mode**: Tap top-right corner on M5Paper again

---

## Detailed Instructions

### Prerequisites

- M5Paper S3 powered on and running the File Viewer app
- iPhone with WiFi enabled
- Files ready to upload (.md or images)

### Part 1: Prepare Files on iPhone

#### For Markdown Files

**Option A: Create in Notes**
1. Open Notes app
2. Create or open a note
3. Tap share icon → Save to Files
4. Choose location and save as `.md` extension
5. Note: You may need to add ".md" manually

**Option B: Use Files App**
1. Open Files app
2. Navigate to your markdown file
3. Ensure it has `.md` extension
4. Remember the location

**Option C: Email to Yourself**
1. Email the .md file to yourself
2. Open Mail on iPhone
3. Long press attachment → Save to Files
4. Choose location

#### For Photos/Images

**Option A: From Photos App**
1. Open Photos
2. Select photo you want to upload
3. Keep Photos app open for later

**Option B: From Files App**
1. Save images to Files app
2. Supported formats: .jpg, .jpeg, .png
3. Remember the location

### Part 2: Enable WiFi on M5Paper S3

1. **Power on the device**
   - Press power button once
   - Wait for File Browser to appear

2. **Enter WiFi mode**
   - **Tap the top-right corner** of the screen
   - You'll see "Starting WiFi..." message

3. **Note the credentials**
   - Screen will display:
     ```
     WiFi Server Active

     Connect to WiFi:
     PaperS3-Files

     Password:
     papers3app

     Open: http://192.168.4.1
     ```
   - Keep this screen visible

### Part 3: Connect iPhone to M5Paper S3

1. **Open iPhone Settings**
   - Tap Settings icon
   - Tap Wi-Fi

2. **Find the network**
   - Look for "PaperS3-Files" in the list
   - It may take 5-10 seconds to appear
   - If not visible, wait and tap refresh

3. **Connect to network**
   - Tap "PaperS3-Files"
   - Enter password: `papers3app`
   - Tap Join

4. **Confirm connection**
   - You'll see a checkmark next to "PaperS3-Files"
   - You may see "No Internet Connection" - this is normal
   - Tap "Use Without Internet" if prompted

### Part 4: Open Upload Page

1. **Open Safari**
   - Close Settings
   - Open Safari browser

2. **Navigate to upload page**
   - Tap address bar
   - Type exactly: `192.168.4.1`
   - Tap Go

3. **Upload page should load**
   - You'll see "M5Paper S3 File Upload" page
   - Beautiful purple gradient design
   - Two upload sections: Markdown and Image

**Troubleshooting:**
- If page doesn't load, check WiFi connection
- Ensure you're connected to "PaperS3-Files"
- Try typing `http://192.168.4.1` instead
- Wait 10 seconds and refresh

### Part 5: Upload Markdown Files

1. **Tap "Choose .md File"** button
   - Browser will show file picker
   - Navigate to your markdown file

2. **Select your file**
   - Tap the .md file you want to upload
   - You may need to navigate through folders
   - File name will appear below the button

3. **Upload the file**
   - Tap "Upload Markdown" button
   - You'll see "Uploading..." message
   - Wait for confirmation

4. **Success confirmation**
   - Green checkmark appears: "✓ File uploaded successfully!"
   - File is now saved on M5Paper S3
   - You can upload more files

### Part 6: Upload Images

1. **Tap "Choose Image"** button
   - You'll see "Photo Library" option
   - Or browse Files app

2. **Select from Photos**
   - Tap "Photo Library"
   - Choose an album
   - Tap the photo you want

   **OR Select from Files**
   - Tap "Browse"
   - Navigate to your image
   - Supported: .jpg, .jpeg, .png

3. **Confirm selection**
   - Image name appears below button
   - Preview may show

4. **Upload the image**
   - Tap "Upload Image" button
   - Uploading message appears
   - Wait for completion (may take 5-10 seconds for large images)

5. **Success confirmation**
   - Green checkmark: "✓ File uploaded successfully!"
   - Image is saved on M5Paper S3

### Part 7: Upload Multiple Files

**To upload multiple files:**

1. After first upload completes
2. Tap "Choose .md File" or "Choose Image" again
3. Select next file
4. Tap upload button
5. Repeat for all files

**Tips:**
- Upload files one at a time
- Wait for confirmation before next upload
- Smaller files upload faster
- Keep upload page open

### Part 8: Exit WiFi Mode

1. **Return to M5Paper S3**
   - Look at the device screen
   - Should still show "WiFi Server Active"

2. **Disable WiFi**
   - **Tap top-right corner** of screen
   - You'll see "Stopping WiFi..." message

3. **File Browser returns**
   - After 2 seconds, File Browser appears
   - Your uploaded files are now visible
   - Listed under DOCUMENTS or IMAGES

4. **Reconnect iPhone to normal WiFi**
   - Open iPhone Settings → WiFi
   - Connect to your home/office WiFi

### Part 9: View Your Files

1. **In File Browser**
   - Scroll to find your uploaded file
   - DOCUMENTS section = .md files
   - IMAGES section = image files

2. **Open a markdown file**
   - Tap the file name
   - Markdown reader opens
   - Swipe to navigate pages

3. **Open an image**
   - Tap the image name
   - Full-screen image viewer opens
   - Swipe left/right for navigation

---

## Common Issues and Solutions

### "Cannot connect to PaperS3-Files"

**Causes:**
- WiFi mode not enabled on M5Paper S3
- Incorrect password
- Too far from device

**Solutions:**
1. Verify WiFi mode is ON (check M5Paper S3 screen)
2. Re-enter password exactly: `papers3app` (all lowercase)
3. Move iPhone closer to M5Paper S3 (within 3 meters)
4. Restart WiFi on iPhone (toggle off/on)
5. Restart M5Paper S3

### "Cannot open 192.168.4.1"

**Causes:**
- Not connected to PaperS3-Files network
- Typo in address
- Safari cached old page

**Solutions:**
1. Check WiFi settings - ensure connected to "PaperS3-Files"
2. Type exactly: `192.168.4.1` (no spaces, no www)
3. Clear Safari cache: Settings → Safari → Clear History
4. Try private browsing mode
5. Force quit Safari and reopen

### "Upload failed" error message

**Causes:**
- File too large (>2MB)
- Unsupported file type
- Filesystem full on M5Paper S3

**Solutions:**
1. Check file size (must be < 2MB)
2. Verify file extension (.md, .jpg, .jpeg, .png only)
3. Try a smaller file first
4. Compress image before uploading
5. Free up space on M5Paper S3 (delete old files)

### Files not appearing in File Browser

**Causes:**
- Still in WiFi mode
- Upload didn't complete
- File saved to wrong location

**Solutions:**
1. Exit WiFi mode (tap top-right corner)
2. Wait for File Browser to refresh
3. Scroll down to check both sections
4. Re-upload the file
5. Check serial monitor for errors

### Image uploads very slowly

**Causes:**
- Large file size
- High resolution photo
- WiFi interference

**Solutions:**
1. Reduce image size/quality before uploading
2. Use "Small" or "Medium" size when exporting from Photos
3. Move closer to M5Paper S3
4. Avoid 2.4GHz interference (microwave, Bluetooth)
5. Wait patiently - large images can take 20-30 seconds

---

## File Preparation Tips

### Markdown Files

**Best practices:**
- Keep files under 50KB for best performance
- Use simple markdown syntax
- Headers: `#`, `##`, `###`
- Lists: `-` or `*` bullets
- Short paragraphs (3-5 lines)
- Avoid tables, code blocks, complex formatting

**Creating markdown on iPhone:**

1. **Using Notes app:**
   ```
   Create note → Format as plain text → Export as .md
   ```

2. **Using third-party apps:**
   - iA Writer (recommended)
   - Bear
   - Drafts
   - Notion (export as markdown)

3. **Online editors:**
   - Open markdown editor in Safari
   - Create content
   - Download as .md
   - Save to Files app

### Images

**Best practices:**
- Resolution: 1920×1080 or lower
- Format: JPEG (better compatibility than PNG)
- File size: < 500KB ideal, < 1MB maximum
- Aspect ratio: Any (will auto-scale)

**Optimizing photos on iPhone:**

1. **Use "Small" export size:**
   - Select photo in Photos app
   - Tap share → Save to Files
   - Choose "Small" or "Medium" in options

2. **Edit before uploading:**
   - Open photo in Photos
   - Tap Edit
   - Crop to remove unnecessary parts
   - Save

3. **Use compression apps:**
   - "Compress Photos" app
   - "Photo & Picture Resizer" app
   - Set quality to 70-80%

---

## Advanced Tips

### Batch Upload Workflow

For uploading many files efficiently:

1. **Prepare all files in one folder on iPhone**
   - Create "M5Paper Uploads" folder in Files app
   - Put all .md and images there

2. **Upload during single WiFi session**
   - Enable WiFi on M5Paper S3
   - Connect iPhone
   - Open upload page
   - Upload all files one by one
   - Don't disconnect WiFi between uploads

3. **Organize on device later**
   - Use file browser to find files
   - Current version doesn't support folders
   - Use descriptive filenames

### Offline File Preparation

1. **Create content while offline**
   - Write markdown in Notes or iA Writer
   - Take/edit photos

2. **Upload when near M5Paper S3**
   - Enable WiFi mode
   - Batch upload all prepared files

### Remote Upload (Same Room)

You can upload from multiple devices:

1. M5Paper S3 supports up to 4 simultaneous clients
2. Anyone connected to PaperS3-Files can upload
3. Great for sharing files in meetings/classrooms

### Creating Reading Lists

Use descriptive filenames for organization:

```
01-morning-news.md
02-article-technology.md
03-recipe-dinner.md
photo-001-vacation.jpg
photo-002-family.jpg
```

Files are sorted alphabetically in File Browser.

---

## iPhone Screenshots Guide

### Where to Find Each Step

**Enable WiFi Mode:**
```
[M5Paper S3 Screen]
┌─────────────────────────────┐
│  File Browser         [TAP] │ ← Tap here
│                             │
│  DOCUMENTS                  │
│    example.md               │
│                             │
│  IMAGES                     │
│    photo.jpg                │
└─────────────────────────────┘
```

**WiFi Info Screen:**
```
[M5Paper S3 Screen]
┌─────────────────────────────┐
│    WiFi Server Active       │
│                             │
│   Connect to WiFi:          │
│   PaperS3-Files             │
│                             │
│   Password:                 │
│   papers3app                │
│                             │
│   Open: http://192.168.4.1  │
│                             │
│   Touch top-right to exit   │
└─────────────────────────────┘
```

**iPhone WiFi Settings:**
```
[iPhone Settings > WiFi]
┌─────────────────────────────┐
│ < WiFi                      │
│                             │
│ Wi-Fi          [ON] ————    │
│                             │
│ ✓ PaperS3-Files        🔒  │ ← Connected
│   Your Home WiFi       🔒  │
│   Coffee Shop          🔒  │
└─────────────────────────────┘
```

**Upload Page in Safari:**
```
[Safari Browser]
┌─────────────────────────────┐
│ 🔒 192.168.4.1              │
│                             │
│   📄 M5Paper S3             │
│   Upload files wirelessly   │
│                             │
│ ┌─────────────────────────┐ │
│ │  📝 Markdown Document   │ │
│ │  [Choose .md File]      │ │
│ │  example.md             │ │
│ │  [Upload Markdown]      │ │
│ └─────────────────────────┘ │
│                             │
│ ┌─────────────────────────┐ │
│ │  🖼️ Image               │ │
│ │  [Choose Image]         │ │
│ │  photo.jpg              │ │
│ │  [Upload Image]         │ │
│ └─────────────────────────┘ │
└─────────────────────────────┘
```

---

## FAQ

**Q: Do I need internet connection?**
A: No. M5Paper S3 creates its own WiFi network. No internet required.

**Q: Can I upload from Android?**
A: Yes! Same process works with any device (Android, Windows, Mac).

**Q: How many files can I store?**
A: Limited by 16MB flash storage. Approximately 100-200 markdown files or 10-20 high-quality images.

**Q: Can I delete files?**
A: Not in current version. You'll need to reflash firmware to clear all files.

**Q: What happens if upload fails mid-transfer?**
A: Partial file may be saved. Re-upload to overwrite.

**Q: Can I edit files on the device?**
A: No editing capability. Upload corrected version to overwrite.

**Q: Does it work with iPad?**
A: Yes, same process as iPhone.

**Q: Can I upload Word documents?**
A: No, only markdown (.md). Export Word to markdown first.

**Q: Maximum file name length?**
A: Recommended under 32 characters.

**Q: Special characters in filenames?**
A: Avoid: ` / \ : * ? " < > | `
Safe: letters, numbers, dash, underscore

---

## Quick Reference Card

**WiFi Credentials:**
```
SSID: PaperS3-Files
Password: papers3app
URL: http://192.168.4.1
```

**Supported Files:**
```
Documents: .md
Images: .jpg, .jpeg, .png
Max size: 2MB
```

**Controls:**
```
Enable WiFi: Tap top-right corner (File Browser)
Exit WiFi: Tap top-right corner (WiFi screen)
Upload: Use Safari at 192.168.4.1
```

**Troubleshooting Checklist:**
```
□ WiFi mode enabled on M5Paper S3
□ iPhone connected to "PaperS3-Files"
□ Correct password: papers3app
□ Safari opened to 192.168.4.1
□ File size < 2MB
□ Correct file extension
□ Wait for upload confirmation
```

---

## Need More Help?

1. **Check main README.md** for detailed troubleshooting
2. **Monitor serial output** (if you have USB connected)
3. **Try test files first** (small .md and .jpg)
4. **Restart both devices** when in doubt

Happy uploading! 📄➡️📱➡️🖥️
