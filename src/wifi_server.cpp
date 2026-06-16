#include "wifi_server.h"

WiFiFileServer* WiFiFileServer::instance = nullptr;

// HTML content for upload page
const char UPLOAD_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
    <title>M5Paper S3 File Upload</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }
        body {
            font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, "Helvetica Neue", Arial, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            padding: 20px;
            display: flex;
            justify-content: center;
            align-items: center;
        }
        .container {
            background: white;
            border-radius: 20px;
            box-shadow: 0 20px 60px rgba(0,0,0,0.3);
            padding: 40px;
            max-width: 500px;
            width: 100%;
        }
        h1 {
            color: #333;
            margin-bottom: 10px;
            font-size: 28px;
            text-align: center;
        }
        .subtitle {
            color: #666;
            text-align: center;
            margin-bottom: 30px;
            font-size: 14px;
        }
        .upload-section {
            margin-bottom: 30px;
            padding: 30px;
            border: 2px dashed #ddd;
            border-radius: 15px;
            background: #f9f9f9;
            text-align: center;
            transition: all 0.3s;
        }
        .upload-section:hover {
            border-color: #667eea;
            background: #f0f4ff;
        }
        .upload-section h2 {
            color: #667eea;
            margin-bottom: 15px;
            font-size: 20px;
        }
        input[type="file"] {
            display: none;
        }
        .file-label {
            display: inline-block;
            padding: 12px 30px;
            background: #667eea;
            color: white;
            border-radius: 25px;
            cursor: pointer;
            transition: all 0.3s;
            font-weight: 500;
            margin-bottom: 15px;
        }
        .file-label:hover {
            background: #5568d3;
            transform: translateY(-2px);
            box-shadow: 0 5px 15px rgba(102, 126, 234, 0.4);
        }
        .file-label:active {
            transform: translateY(0);
        }
        button {
            width: 100%;
            padding: 15px;
            background: #764ba2;
            color: white;
            border: none;
            border-radius: 25px;
            font-size: 16px;
            font-weight: 600;
            cursor: pointer;
            transition: all 0.3s;
        }
        button:hover {
            background: #653a8e;
            transform: translateY(-2px);
            box-shadow: 0 5px 15px rgba(118, 75, 162, 0.4);
        }
        button:active {
            transform: translateY(0);
        }
        button:disabled {
            background: #ccc;
            cursor: not-allowed;
            transform: none;
        }
        .file-name {
            color: #333;
            margin: 10px 0;
            font-size: 14px;
            word-break: break-all;
        }
        .status {
            margin-top: 20px;
            padding: 15px;
            border-radius: 10px;
            text-align: center;
            font-weight: 500;
            display: none;
        }
        .status.success {
            background: #d4edda;
            color: #155724;
            border: 1px solid #c3e6cb;
        }
        .status.error {
            background: #f8d7da;
            color: #721c24;
            border: 1px solid #f5c6cb;
        }
        .status.info {
            background: #d1ecf1;
            color: #0c5460;
            border: 1px solid #bee5eb;
        }
        .device-info {
            background: #f0f4ff;
            padding: 15px;
            border-radius: 10px;
            margin-top: 20px;
            font-size: 12px;
            color: #666;
            text-align: center;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>📄 M5Paper S3</h1>
        <p class="subtitle">Upload files wirelessly to your device</p>

        <div class="upload-section">
            <h2>📝 Markdown Document</h2>
            <label for="mdFile" class="file-label">Choose .md File</label>
            <input type="file" id="mdFile" accept=".md">
            <div class="file-name" id="mdFileName">No file selected</div>
            <button onclick="uploadFile('md')">Upload Markdown</button>
        </div>

        <div class="upload-section">
            <h2>📄 Text File</h2>
            <label for="txtFile" class="file-label">Choose .txt File</label>
            <input type="file" id="txtFile" accept=".txt">
            <div class="file-name" id="txtFileName">No file selected</div>
            <button onclick="uploadFile('txt')">Upload Text</button>
        </div>

        <div class="upload-section">
            <h2>🖼️ Image</h2>
            <label for="imgFile" class="file-label">Choose Image</label>
            <input type="file" id="imgFile" accept=".jpg,.jpeg,.png,.webp">
            <div class="file-name" id="imgFileName">No file selected</div>
            <button onclick="uploadFile('img')">Upload Image</button>
        </div>

        <div class="upload-section">
            <h2>📚 Comic Book (CBZ)</h2>
            <label for="cbzFile" class="file-label">Choose .cbz File</label>
            <input type="file" id="cbzFile" accept=".cbz">
            <div class="file-name" id="cbzFileName">No file selected</div>
            <button onclick="uploadFile('cbz')">Upload CBZ</button>
        </div>

        <div class="status" id="status"></div>

        <div class="device-info">
            Connected to: <strong>PaperS3-Files</strong><br>
            Supported formats: .md, .txt, .jpg, .jpeg, .png, .webp, .cbz
        </div>
    </div>

    <script>
        document.getElementById('mdFile').addEventListener('change', function(e) {
            const fileName = e.target.files[0] ? e.target.files[0].name : 'No file selected';
            document.getElementById('mdFileName').textContent = fileName;
        });

        document.getElementById('txtFile').addEventListener('change', function(e) {
            const fileName = e.target.files[0] ? e.target.files[0].name : 'No file selected';
            document.getElementById('txtFileName').textContent = fileName;
        });

        document.getElementById('imgFile').addEventListener('change', function(e) {
            const fileName = e.target.files[0] ? e.target.files[0].name : 'No file selected';
            document.getElementById('imgFileName').textContent = fileName;
        });

        document.getElementById('cbzFile').addEventListener('change', function(e) {
            const fileName = e.target.files[0] ? e.target.files[0].name : 'No file selected';
            document.getElementById('cbzFileName').textContent = fileName;
        });

        function showStatus(message, type) {
            const status = document.getElementById('status');
            status.textContent = message;
            status.className = 'status ' + type;
            status.style.display = 'block';

            if (type === 'success') {
                setTimeout(() => {
                    status.style.display = 'none';
                }, 3000);
            }
        }

        async function uploadFile(type) {
            let fileInput;
            if (type === 'md') {
                fileInput = document.getElementById('mdFile');
            } else if (type === 'txt') {
                fileInput = document.getElementById('txtFile');
            } else if (type === 'cbz') {
                fileInput = document.getElementById('cbzFile');
            } else {
                fileInput = document.getElementById('imgFile');
            }
            const file = fileInput.files[0];

            if (!file) {
                showStatus('Please select a file first', 'error');
                return;
            }

            const formData = new FormData();
            formData.append('file', file);

            showStatus('Uploading...', 'info');

            try {
                const response = await fetch('/upload', {
                    method: 'POST',
                    body: formData
                });

                const result = await response.json();

                if (response.ok && result.status === 'ok') {
                    showStatus('✓ File uploaded successfully!', 'success');
                    fileInput.value = '';
                    if (type === 'md') {
                        document.getElementById('mdFileName').textContent = 'No file selected';
                    } else if (type === 'txt') {
                        document.getElementById('txtFileName').textContent = 'No file selected';
                    } else if (type === 'cbz') {
                        document.getElementById('cbzFileName').textContent = 'No file selected';
                    } else {
                        document.getElementById('imgFileName').textContent = 'No file selected';
                    }
                } else {
                    showStatus('Upload failed: ' + (result.message || 'Unknown error'), 'error');
                }
            } catch (error) {
                showStatus('Upload failed: ' + error.message, 'error');
            }
        }
    </script>
</body>
</html>
)rawliteral";

WiFiFileServer::WiFiFileServer() : server(80), running(false) {
    instance = this;
}

void WiFiFileServer::begin() {
    setupAccessPoint();
    setupWebServer();
    running = true;
}

void WiFiFileServer::setupAccessPoint() {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(WIFI_SSID, WIFI_PASSWORD, WIFI_CHANNEL, 0, MAX_WIFI_CLIENTS);

    delay(100);

    IPAddress ip = WiFi.softAPIP();
    ipAddress = ip.toString();

    Serial.println("Access Point started");
    Serial.printf("SSID: %s\n", WIFI_SSID);
    Serial.printf("Password: %s\n", WIFI_PASSWORD);
    Serial.printf("IP Address: %s\n", ipAddress.c_str());
}

void WiFiFileServer::setupWebServer() {
    server.on("/", HTTP_GET, handleRoot);
    server.on("/upload", HTTP_POST, handleUpload, handleFileUpload);
    server.onNotFound(handleNotFound);

    server.begin();
    Serial.println("Web server started");
}

void WiFiFileServer::handleRoot() {
    if (instance) {
        instance->server.send_P(200, "text/html", UPLOAD_HTML);
    }
}

void WiFiFileServer::handleUpload() {
    if (instance) {
        instance->server.send(200, "application/json", "{\"status\":\"ok\"}");
    }
}

void WiFiFileServer::handleFileUpload() {
    if (!instance) return;

    HTTPUpload& upload = instance->server.upload();

    if (upload.status == UPLOAD_FILE_START) {
        String filename = upload.filename;
        if (!filename.startsWith("/")) {
            filename = "/files/" + filename;
        }

        Serial.printf("Upload started: %s\n", filename.c_str());

        File file = LittleFS.open(filename, "w");
        if (!file) {
            Serial.println("Failed to open file for writing");
            return;
        }
        file.close();

        upload.filename = filename; // Store the full path

    } else if (upload.status == UPLOAD_FILE_WRITE) {
        File file = LittleFS.open(upload.filename, "a");
        if (file) {
            file.write(upload.buf, upload.currentSize);
            file.close();
        }

    } else if (upload.status == UPLOAD_FILE_END) {
        Serial.printf("Upload completed: %s (%d bytes)\n", upload.filename.c_str(), upload.totalSize);
    }
}

void WiFiFileServer::handleNotFound() {
    if (instance) {
        instance->server.send(404, "text/plain", "Not found");
    }
}

void WiFiFileServer::update() {
    if (running) {
        server.handleClient();
    }
}

void WiFiFileServer::stop() {
    server.stop();
    WiFi.softAPdisconnect(true);
    WiFi.mode(WIFI_OFF);
    running = false;

    Serial.println("WiFi server stopped");
}

bool WiFiFileServer::isRunning() {
    return running;
}

String WiFiFileServer::getIPAddress() {
    return ipAddress;
}
