#ifndef WIFI_SERVER_H
#define WIFI_SERVER_H

#include <M5Unified.h>
#include <WiFi.h>
#include <WebServer.h>
#include <LittleFS.h>
#include "config.h"

class WiFiFileServer {
public:
    WiFiFileServer();
    void begin();
    void update();
    void stop();
    bool isRunning();
    String getIPAddress();

private:
    void setupAccessPoint();
    void setupWebServer();

    // Handler functions
    static void handleRoot();
    static void handleUpload();
    static void handleFileUpload();
    static void handleNotFound();

    WebServer server;
    bool running;
    String ipAddress;

    static WiFiFileServer* instance;
};

#endif // WIFI_SERVER_H
