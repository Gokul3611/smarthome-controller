/**
 * ⚡ MINI SMART HOME CONTROLLER - Testing Version
 * ==============================================================
 * Simplified test firmware for ESP32 with LED control
 * 
 * Purpose:
 * - Test connectivity between ESP32, Dashboard, and Flutter App
 * - Use LED instead of AC TRIAC for safe testing
 * - Verify bidirectional communication
 * 
 * Features:
 * - Simple LED on/off control (replaces AC device)
 * - WiFi connectivity with WiFiManager
 * - Google Apps Script cloud integration
 * - Local REST API (port 8080)
 * - WebSocket real-time updates (port 81)
 * - Compatible with existing dashboard and app
 * 
 * Hardware Required:
 * - ESP32 Development Board
 * - 1x LED + 220Ω Resistor (connected to GPIO 2)
 * 
 * Copyright (c) 2024 - Test Version
 */

#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <WiFiManager.h>
#include <ESPmDNS.h>
#include <WebSocketsServer.h>

// ================================================================
// CONFIGURATION
// ================================================================
#define FIRMWARE_VERSION 1.0
#define FIRMWARE_BUILD "MINI-001"

// Hardware Configuration
#define LED_PIN 2  // Built-in LED or connect external LED here

// Cloud Configuration
// IMPORTANT: Replace with YOUR Google Apps Script deployment URL
// Get your URL from: backend/google-apps-script/README.md
// This is a placeholder - update it before use!
String GOOGLE_SCRIPT_URL = "YOUR_GOOGLE_SCRIPT_URL_HERE";

// Alternative: Use the demo/test endpoint (if available)
// String GOOGLE_SCRIPT_URL = "https://script.google.com/macros/s/YOUR_DEPLOYMENT_ID/exec";

String systemName = "Mini_Controller";

// Network Configuration
#define CLOUD_POLL_INTERVAL_MS 2500  // 2.5 seconds
#define HTTP_TIMEOUT_MS 15000

// ================================================================
// GLOBAL OBJECTS
// ================================================================
WebServer redirectServer(80);
WebServer localServer(8080);
WebSocketsServer webSocket = WebSocketsServer(81);

// ================================================================
// DEVICE STATE
// ================================================================
struct Device {
    String name;
    bool state;
    int brightness;
    unsigned long totalRuntime;  // seconds
};

Device device;
unsigned long bootTime = 0;
bool cloudConnected = false;

// ================================================================
// UTILITY FUNCTIONS
// ================================================================
void logMessage(const char* format, ...) {
    Serial.print("[INFO] ");
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    Serial.println(buffer);
}

unsigned long getUptimeSeconds() {
    return (millis() - bootTime) / 1000;
}

const char* getSignalStrength(int rssi) {
    if (rssi >= -50) return "Excellent";
    if (rssi >= -60) return "Good";
    if (rssi >= -70) return "Fair";
    if (rssi >= -80) return "Poor";
    return "Weak";
}

// ================================================================
// DEVICE CONTROL
// ================================================================
void setDeviceState(bool state, int brightness) {
    device.state = state;
    device.brightness = brightness;
    
    // Control LED based on state and brightness
    if (state) {
        // Map brightness (0-100) to PWM (0-255)
        int pwmValue = map(brightness, 0, 100, 0, 255);
        analogWrite(LED_PIN, pwmValue);
        logMessage("LED ON - Brightness: %d%%", brightness);
    } else {
        analogWrite(LED_PIN, 0);
        logMessage("LED OFF");
    }
    
    // Broadcast state change to WebSocket clients
    broadcastDeviceState();
}

void broadcastDeviceState() {
    StaticJsonDocument<256> doc;
    doc["type"] = "device_update";
    doc["id"] = 0;
    doc["state"] = device.state;
    doc["brightness"] = device.brightness;
    doc["name"] = device.name;
    
    String message;
    serializeJson(doc, message);
    webSocket.broadcastTXT(message);
}

// ================================================================
// REST API ENDPOINTS
// ================================================================
void handleGetStatus() {
    StaticJsonDocument<512> doc;
    JsonArray devicesArray = doc.createNestedArray("devices");
    
    JsonObject deviceObj = devicesArray.createNestedObject();
    deviceObj["id"] = 0;
    deviceObj["name"] = device.name;
    deviceObj["type"] = 0;  // TYPE_SWITCH
    deviceObj["state"] = device.state;
    deviceObj["brightness"] = device.brightness;
    deviceObj["runtime"] = device.totalRuntime;
    deviceObj["locked"] = false;
    
    String response;
    serializeJson(doc, response);
    localServer.send(200, "application/json", response);
}

void handlePostControl() {
    if (!localServer.hasArg("plain")) {
        localServer.send(400, "application/json", "{\"success\":false,\"error\":\"No data\"}");
        return;
    }
    
    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, localServer.arg("plain"));
    
    if (error) {
        localServer.send(400, "application/json", "{\"success\":false,\"error\":\"Invalid JSON\"}");
        return;
    }
    
    bool state = doc.containsKey("state") ? doc["state"].as<bool>() : device.state;
    int brightness = doc.containsKey("brightness") ? doc["brightness"].as<int>() : device.brightness;
    
    // Validate brightness range
    if (brightness < 0 || brightness > 100) {
        localServer.send(400, "application/json", "{\"success\":false,\"error\":\"Brightness must be 0-100\"}");
        return;
    }
    
    setDeviceState(state, brightness);
    
    localServer.send(200, "application/json", "{\"success\":true}");
}

void handleGetInfo() {
    StaticJsonDocument<512> doc;
    
    doc["name"] = systemName;
    doc["firmware"] = String(FIRMWARE_VERSION, 1);
    doc["build"] = FIRMWARE_BUILD;
    doc["mac"] = WiFi.macAddress();
    doc["ip"] = WiFi.localIP().toString();
    doc["uptime"] = getUptimeSeconds();
    doc["rssi"] = WiFi.RSSI();
    doc["signal"] = getSignalStrength(WiFi.RSSI());
    doc["heap"] = ESP.getFreeHeap();
    doc["cloud_connected"] = cloudConnected;
    
    String response;
    serializeJson(doc, response);
    localServer.send(200, "application/json", response);
}

void handleCORS() {
    localServer.sendHeader("Access-Control-Allow-Origin", "*");
    localServer.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    localServer.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    localServer.send(200);
}

void initLocalAPI() {
    // Enable CORS for all endpoints
    localServer.on("/status", HTTP_OPTIONS, handleCORS);
    localServer.on("/control", HTTP_OPTIONS, handleCORS);
    localServer.on("/info", HTTP_OPTIONS, handleCORS);
    
    // API endpoints
    localServer.on("/status", HTTP_GET, []() {
        localServer.sendHeader("Access-Control-Allow-Origin", "*");
        handleGetStatus();
    });
    
    localServer.on("/control", HTTP_POST, []() {
        localServer.sendHeader("Access-Control-Allow-Origin", "*");
        handlePostControl();
    });
    
    localServer.on("/info", HTTP_GET, []() {
        localServer.sendHeader("Access-Control-Allow-Origin", "*");
        handleGetInfo();
    });
    
    localServer.begin();
    logMessage("Local API server started on port 8080");
    
    // WebSocket events
    webSocket.begin();
    webSocket.onEvent([](uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
        switch(type) {
            case WStype_DISCONNECTED:
                logMessage("WebSocket client #%u disconnected", num);
                break;
            case WStype_CONNECTED:
                logMessage("WebSocket client #%u connected", num);
                broadcastDeviceState();
                break;
            case WStype_TEXT:
                // Handle incoming WebSocket messages if needed
                break;
        }
    });
    logMessage("WebSocket server started on port 81");
}

// ================================================================
// CLOUD COMMUNICATION
// ================================================================
bool syncWithCloud(HTTPClient &http) {
    StaticJsonDocument<512> doc;
    
    // Build request
    doc["action"] = "poll";
    doc["uid"] = WiFi.macAddress();
    doc["ver"] = FIRMWARE_VERSION;
    doc["uptime"] = getUptimeSeconds();
    doc["rssi"] = WiFi.RSSI();
    doc["heap"] = ESP.getFreeHeap();
    
    // Add device state
    JsonObject d1 = doc.createNestedObject("d1");
    d1["s"] = device.state ? 1 : 0;
    d1["v"] = device.brightness;
    d1["t"] = 0;  // TYPE_SWITCH
    d1["runtime"] = device.totalRuntime;
    
    String jsonPayload;
    serializeJson(doc, jsonPayload);
    
    http.begin(GOOGLE_SCRIPT_URL);
    http.addHeader("Content-Type", "application/json");
    http.setTimeout(HTTP_TIMEOUT_MS);
    
    int httpCode = http.POST(jsonPayload);
    
    if(httpCode == 200) {
        String response = http.getString();
        StaticJsonDocument<512> respDoc;
        DeserializationError error = deserializeJson(respDoc, response);
        
        if (error) {
            logMessage("JSON parse error: %s", error.c_str());
            http.end();
            return false;
        }
        
        // Process cloud response
        processCloudResponse(respDoc);
        http.end();
        return true;
    } else {
        logMessage("Cloud sync failed: HTTP %d", httpCode);
        http.end();
        return false;
    }
}

void processCloudResponse(const JsonDocument &respDoc) {
    // System name update
    if (respDoc.containsKey("sys_name")) {
        String newName = respDoc["sys_name"].as<String>();
        if (newName != systemName && newName.length() > 0) {
            systemName = newName;
            logMessage("System name updated: %s", systemName.c_str());
        }
    }
    
    // Device update
    if(respDoc.containsKey("d1")) {
        JsonObject d = respDoc["d1"].as<JsonObject>();
        
        // Name update
        if (d.containsKey("name")) {
            String newName = d["name"].as<String>();
            if (newName != device.name && newName.length() > 0) {
                device.name = newName;
            }
        }
        
        // State update
        if(d.containsKey("update") && d["update"] == true) {
            bool newState = (d["state"].as<int>() == 1);
            int newBrightness = d["val"].as<int>();
            setDeviceState(newState, newBrightness);
        }
    }
}

// ================================================================
// SETUP
// ================================================================
void setup() {
    Serial.begin(115200);
    delay(100);
    
    bootTime = millis();
    
    logMessage("========================================");
    logMessage("Mini Smart Home Controller v%.1f", FIRMWARE_VERSION);
    logMessage("Build: %s", FIRMWARE_BUILD);
    logMessage("========================================");
    
    // Initialize LED pin
    pinMode(LED_PIN, OUTPUT);
    analogWrite(LED_PIN, 0);
    
    // Initialize device state
    device.name = "Test LED";
    device.state = false;
    device.brightness = 100;
    device.totalRuntime = 0;
    
    // Initialize WiFi
    WiFiManager wm;
    wm.setTitle("Setup " + systemName);
    wm.setConfigPortalTimeout(180);  // 3 minutes timeout
    
    if(!wm.autoConnect(systemName.c_str())) {
        logMessage("WiFi connection failed - Restarting...");
        delay(3000);
        ESP.restart();
    }
    
    logMessage("WiFi connected: %s", WiFi.localIP().toString().c_str());
    logMessage("Signal strength: %s (%d dBm)", 
               getSignalStrength(WiFi.RSSI()), WiFi.RSSI());
    
    // Initialize mDNS
    String mdnsName = systemName;
    mdnsName.replace(" ", "");
    mdnsName.toLowerCase();
    
    if (MDNS.begin(mdnsName.c_str())) {
        MDNS.addService("http", "tcp", 80);
        MDNS.addService("http", "tcp", 8080);
        MDNS.addService("ws", "tcp", 81);
        logMessage("mDNS started: %s.local", mdnsName.c_str());
    }
    
    // Setup redirect server (port 80)
    redirectServer.on("/", []() {
        redirectServer.sendHeader("Location", GOOGLE_SCRIPT_URL, true);
        redirectServer.send(302, "text/plain", "Redirecting to cloud dashboard...");
    });
    redirectServer.begin();
    logMessage("Redirect server started on port 80");
    
    // Setup local API server
    initLocalAPI();
    
    logMessage("========================================");
    logMessage("System ready - LED connected to GPIO %d", LED_PIN);
    logMessage("Free heap: %d bytes", ESP.getFreeHeap());
    logMessage("========================================");
}

// ================================================================
// MAIN LOOP
// ================================================================
void loop() {
    static unsigned long lastSyncTime = 0;
    static unsigned long lastRuntimeUpdate = 0;
    
    // Handle web servers
    redirectServer.handleClient();
    localServer.handleClient();
    webSocket.loop();
    
    // Update runtime statistics
    unsigned long now = millis();
    if (now - lastRuntimeUpdate >= 1000) {
        if (device.state) {
            device.totalRuntime++;
        }
        lastRuntimeUpdate = now;
    }
    
    // Cloud sync
    if (now - lastSyncTime > CLOUD_POLL_INTERVAL_MS) {
        if(WiFi.status() == WL_CONNECTED) {
            HTTPClient http;
            cloudConnected = syncWithCloud(http);
        } else {
            cloudConnected = false;
            logMessage("WiFi disconnected, attempting reconnect...");
            WiFi.reconnect();
        }
        lastSyncTime = now;
    }
    
    delay(10);
}
