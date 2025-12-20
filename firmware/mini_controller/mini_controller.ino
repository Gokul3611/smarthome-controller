/**
 * ⚡ Mini Controller: ESP32 Built-in LED Remote Control v1.0
 * ==========================================================
 * Simple firmware for controlling ESP32's built-in LED remotely
 * 
 * Features:
 * - WiFi connectivity with captive portal
 * - REST API for LED control (on/off/brightness)
 * - WebSocket real-time updates
 * - Google Apps Script cloud integration
 * - Web dashboard compatible
 * - PWM brightness control (0-100%)
 * 
 * Hardware:
 * - ESP32 DevKit or compatible
 * - Built-in LED (typically GPIO 2)
 * 
 * Copyright (c) 2024 - Smart Home Controller Project
 */

#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiManager.h>
#include <ESPmDNS.h>
#include <WebSocketsServer.h>

// ================================================================
// CONFIGURATION
// ================================================================
#define FIRMWARE_VERSION "1.0"
#define DEVICE_NAME "Mini_LED_Controller"

// Built-in LED Pin (GPIO 2 for most ESP32 boards)
#define LED_PIN 2

// PWM Configuration
#define PWM_CHANNEL 0
#define PWM_FREQUENCY 5000
#define PWM_RESOLUTION 8  // 8-bit (0-255)

// Network Configuration
#define LOCAL_SERVER_PORT 8080
#define WEBSOCKET_PORT 81
#define CLOUD_POLL_INTERVAL_MS 5000

// Google Apps Script URL (configure this)
String GOOGLE_SCRIPT_URL = "https://script.google.com/macros/s/AKfycbxJP31g9LhRulRHTbTd6KidEiBXlxCKfKcXkiUGe961IfNZDgHuoWAIif91PrPUQnHrIQ/exec";

// ================================================================
// GLOBAL OBJECTS
// ================================================================
WebServer server(LOCAL_SERVER_PORT);
WebServer redirectServer(80);
WebSocketsServer webSocket = WebSocketsServer(WEBSOCKET_PORT);
HTTPClient http;

// ================================================================
// DEVICE STATE
// ================================================================
struct LEDState {
    bool state;           // On/Off
    int brightness;       // 0-100%
    String name;          // Device name
    unsigned long lastOnTime;
    unsigned long totalRuntime;  // seconds
} led;

// System state
unsigned long bootTime = 0;
bool cloudConnected = false;
String systemUID;

// ================================================================
// UTILITY FUNCTIONS
// ================================================================

void logMessage(const char* level, const char* message) {
    Serial.print("[");
    Serial.print(level);
    Serial.print("] ");
    Serial.println(message);
}

unsigned long getUptimeSeconds() {
    return (millis() - bootTime) / 1000;
}

// Convert brightness percentage (0-100) to PWM value (0-255)
int brightnessToLEDC(int percent) {
    if (percent <= 0) return 0;
    if (percent >= 100) return 255;
    return map(percent, 0, 100, 0, 255);
}

// ================================================================
// LED CONTROL
// ================================================================

void setLEDState(bool state, int brightness) {
    led.state = state;
    led.brightness = constrain(brightness, 0, 100);
    
    if (led.state) {
        int pwmValue = brightnessToLEDC(led.brightness);
        ledcWrite(PWM_CHANNEL, pwmValue);
        led.lastOnTime = millis();
        Serial.printf("[INFO] LED ON at %d%% (PWM: %d)\n", led.brightness, pwmValue);
    } else {
        ledcWrite(PWM_CHANNEL, 0);
        Serial.println("[INFO] LED OFF");
    }
    
    // Broadcast state change to WebSocket clients
    broadcastLEDState();
}

void updateRuntimeStatistics() {
    static unsigned long lastUpdate = 0;
    unsigned long now = millis();
    
    if (now - lastUpdate >= 1000) {  // Update every second
        if (led.state) {
            led.totalRuntime++;
        }
        lastUpdate = now;
    }
}

// ================================================================
// WEBSOCKET HANDLERS
// ================================================================

void broadcastLEDState() {
    StaticJsonDocument<256> doc;
    doc["type"] = "device_update";
    doc["id"] = 0;
    doc["state"] = led.state;
    doc["brightness"] = led.brightness;
    doc["name"] = led.name;
    
    String message;
    serializeJson(doc, message);
    webSocket.broadcastTXT(message);
}

void handleWebSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[WS] Client #%u disconnected\n", num);
            break;
            
        case WStype_CONNECTED: {
            IPAddress ip = webSocket.remoteIP(num);
            Serial.printf("[WS] Client #%u connected from %d.%d.%d.%d\n", 
                         num, ip[0], ip[1], ip[2], ip[3]);
            // Send current state to new client
            broadcastLEDState();
            break;
        }
        
        case WStype_TEXT: {
            Serial.printf("[WS] Received: %s\n", payload);
            
            // Parse JSON command
            StaticJsonDocument<256> doc;
            DeserializationError error = deserializeJson(doc, payload, length);
            
            if (!error) {
                if (doc.containsKey("action")) {
                    String action = doc["action"].as<String>();
                    
                    if (action == "control") {
                        bool state = doc["state"].as<bool>();
                        int brightness = doc.containsKey("brightness") ? 
                                       doc["brightness"].as<int>() : 100;
                        setLEDState(state, brightness);
                    } else if (action == "get_status") {
                        broadcastLEDState();
                    }
                }
            }
            break;
        }
        
        default:
            break;
    }
}

// ================================================================
// REST API HANDLERS
// ================================================================

void sendJsonResponse(int code, const JsonDocument& doc) {
    String response;
    serializeJson(doc, response);
    server.send(code, "application/json", response);
}

void sendErrorResponse(int code, const char* message) {
    StaticJsonDocument<128> doc;
    doc["error"] = message;
    doc["success"] = false;
    sendJsonResponse(code, doc);
}

// GET /status - Get LED state
void handleGetStatus() {
    StaticJsonDocument<512> doc;
    
    JsonArray devices = doc.createNestedArray("devices");
    JsonObject device = devices.createNestedObject();
    device["id"] = 0;
    device["name"] = led.name;
    device["type"] = "LED";
    device["state"] = led.state;
    device["brightness"] = led.brightness;
    device["runtime"] = led.totalRuntime;
    
    sendJsonResponse(200, doc);
    logMessage("INFO", "GET /status");
}

// POST /control - Control LED
void handlePostControl() {
    if (!server.hasArg("plain")) {
        sendErrorResponse(400, "Missing request body");
        return;
    }
    
    String body = server.arg("plain");
    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, body);
    
    if (error) {
        sendErrorResponse(400, "Invalid JSON");
        return;
    }
    
    if (!doc.containsKey("state")) {
        sendErrorResponse(400, "Missing 'state' field");
        return;
    }
    
    bool state = doc["state"].as<bool>();
    int brightness = doc.containsKey("brightness") ? doc["brightness"].as<int>() : 100;
    
    setLEDState(state, brightness);
    
    StaticJsonDocument<128> response;
    response["success"] = true;
    response["state"] = led.state;
    response["brightness"] = led.brightness;
    sendJsonResponse(200, response);
    
    Serial.printf("[INFO] POST /control - State: %s, Brightness: %d%%\n", 
                  state ? "ON" : "OFF", brightness);
}

// GET /info - System information
void handleGetInfo() {
    StaticJsonDocument<512> doc;
    
    doc["name"] = DEVICE_NAME;
    doc["device_type"] = "mini_controller";
    doc["firmware"] = FIRMWARE_VERSION;
    doc["mac"] = systemUID;
    doc["ip"] = WiFi.localIP().toString();
    doc["uptime"] = getUptimeSeconds();
    doc["rssi"] = WiFi.RSSI();
    doc["heap"] = ESP.getFreeHeap();
    doc["cloud_connected"] = cloudConnected;
    
    sendJsonResponse(200, doc);
    logMessage("INFO", "GET /info");
}

// POST /config - Update device configuration
void handlePostConfig() {
    if (!server.hasArg("plain")) {
        sendErrorResponse(400, "Missing request body");
        return;
    }
    
    String body = server.arg("plain");
    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, body);
    
    if (error) {
        sendErrorResponse(400, "Invalid JSON");
        return;
    }
    
    if (doc.containsKey("name")) {
        led.name = doc["name"].as<String>();
    }
    
    StaticJsonDocument<128> response;
    response["success"] = true;
    response["name"] = led.name;
    sendJsonResponse(200, response);
    
    logMessage("INFO", "POST /config");
}

// GET / - Root endpoint
void handleRoot() {
    String html = R"(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Mini LED Controller</title>
    <style>
        * { margin: 0; padding: 0; box-sizing: border-box; }
        body {
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            display: flex;
            align-items: center;
            justify-content: center;
            padding: 20px;
        }
        .container {
            background: white;
            border-radius: 20px;
            padding: 40px;
            box-shadow: 0 20px 60px rgba(0,0,0,0.3);
            max-width: 400px;
            width: 100%;
        }
        h1 {
            color: #2d3748;
            margin-bottom: 10px;
            font-size: 28px;
        }
        .subtitle {
            color: #718096;
            margin-bottom: 30px;
            font-size: 14px;
        }
        .led-control {
            text-align: center;
            margin-bottom: 30px;
        }
        .led-indicator {
            width: 80px;
            height: 80px;
            border-radius: 50%;
            margin: 20px auto;
            transition: all 0.3s;
            box-shadow: 0 0 20px rgba(0,0,0,0.1);
        }
        .led-indicator.on {
            background: #48bb78;
            box-shadow: 0 0 40px rgba(72, 187, 120, 0.6);
        }
        .led-indicator.off {
            background: #cbd5e0;
        }
        .toggle-btn {
            background: #667eea;
            color: white;
            border: none;
            padding: 15px 40px;
            border-radius: 10px;
            font-size: 16px;
            font-weight: 600;
            cursor: pointer;
            transition: all 0.3s;
            width: 100%;
        }
        .toggle-btn:hover {
            background: #5a67d8;
            transform: translateY(-2px);
        }
        .brightness-control {
            margin: 20px 0;
        }
        .brightness-label {
            color: #4a5568;
            font-weight: 600;
            margin-bottom: 10px;
            display: block;
        }
        .slider {
            width: 100%;
            height: 8px;
            border-radius: 5px;
            background: #e2e8f0;
            outline: none;
            -webkit-appearance: none;
        }
        .slider::-webkit-slider-thumb {
            -webkit-appearance: none;
            appearance: none;
            width: 24px;
            height: 24px;
            border-radius: 50%;
            background: #667eea;
            cursor: pointer;
        }
        .slider::-moz-range-thumb {
            width: 24px;
            height: 24px;
            border-radius: 50%;
            background: #667eea;
            cursor: pointer;
            border: none;
        }
        .brightness-value {
            text-align: center;
            color: #667eea;
            font-size: 24px;
            font-weight: 700;
            margin-top: 10px;
        }
        .status {
            background: #f7fafc;
            padding: 15px;
            border-radius: 10px;
            font-size: 14px;
            color: #4a5568;
        }
        .status-row {
            display: flex;
            justify-content: space-between;
            margin: 5px 0;
        }
        .status-label {
            font-weight: 600;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>💡 LED Controller</h1>
        <p class="subtitle">ESP32 Built-in LED</p>
        
        <div class="led-control">
            <div class="led-indicator" id="ledIndicator"></div>
            <button class="toggle-btn" id="toggleBtn" onclick="toggleLED()">Turn On</button>
        </div>
        
        <div class="brightness-control">
            <label class="brightness-label">Brightness</label>
            <input type="range" min="0" max="100" value="100" class="slider" id="brightness" oninput="updateBrightness()">
            <div class="brightness-value"><span id="brightnessValue">100</span>%</div>
        </div>
        
        <div class="status">
            <div class="status-row">
                <span class="status-label">Status:</span>
                <span id="status">OFF</span>
            </div>
            <div class="status-row">
                <span class="status-label">IP Address:</span>
                <span id="ipAddress">Loading...</span>
            </div>
            <div class="status-row">
                <span class="status-label">Signal:</span>
                <span id="signal">Loading...</span>
            </div>
        </div>
    </div>
    
    <script>
        let ledState = false;
        let brightness = 100;
        
        // WebSocket connection
        const ws = new WebSocket('ws://' + window.location.hostname + ':81');
        
        ws.onopen = function() {
            console.log('WebSocket connected');
            updateStatus();
        };
        
        ws.onmessage = function(event) {
            const data = JSON.parse(event.data);
            if (data.type === 'device_update') {
                ledState = data.state;
                brightness = data.brightness;
                updateUI();
            }
        };
        
        function updateUI() {
            const indicator = document.getElementById('ledIndicator');
            const btn = document.getElementById('toggleBtn');
            const statusText = document.getElementById('status');
            
            if (ledState) {
                indicator.className = 'led-indicator on';
                btn.textContent = 'Turn Off';
                statusText.textContent = 'ON (' + brightness + '%)';
            } else {
                indicator.className = 'led-indicator off';
                btn.textContent = 'Turn On';
                statusText.textContent = 'OFF';
            }
            
            document.getElementById('brightness').value = brightness;
            document.getElementById('brightnessValue').textContent = brightness;
        }
        
        function toggleLED() {
            ledState = !ledState;
            controlLED(ledState, brightness);
        }
        
        function updateBrightness() {
            brightness = parseInt(document.getElementById('brightness').value);
            document.getElementById('brightnessValue').textContent = brightness;
            if (ledState) {
                controlLED(true, brightness);
            }
        }
        
        function controlLED(state, bright) {
            fetch('/control', {
                method: 'POST',
                headers: {'Content-Type': 'application/json'},
                body: JSON.stringify({state: state, brightness: bright})
            })
            .then(response => response.json())
            .then(data => {
                if (data.success) {
                    updateUI();
                }
            })
            .catch(error => console.error('Error:', error));
        }
        
        function updateStatus() {
            fetch('/info')
            .then(response => response.json())
            .then(data => {
                document.getElementById('ipAddress').textContent = data.ip;
                const rssi = data.rssi;
                let signalQuality = 'Excellent';
                if (rssi < -80) signalQuality = 'Weak';
                else if (rssi < -70) signalQuality = 'Fair';
                else if (rssi < -60) signalQuality = 'Good';
                document.getElementById('signal').textContent = signalQuality + ' (' + rssi + ' dBm)';
            });
            
            fetch('/status')
            .then(response => response.json())
            .then(data => {
                if (data.devices && data.devices.length > 0) {
                    ledState = data.devices[0].state;
                    brightness = data.devices[0].brightness;
                    updateUI();
                }
            });
        }
        
        // Initial status update
        updateStatus();
        
        // Periodic status update
        setInterval(updateStatus, 5000);
    </script>
</body>
</html>
)";
    server.send(200, "text/html", html);
}

// ================================================================
// CLOUD SYNC
// ================================================================

bool syncWithCloud() {
    if (GOOGLE_SCRIPT_URL.length() < 10) {
        return false;  // URL not configured
    }
    
    StaticJsonDocument<512> doc;
    
    // Build request
    doc["action"] = "poll";
    doc["uid"] = systemUID;
    doc["ver"] = FIRMWARE_VERSION;
    doc["device_type"] = "mini_controller";
    doc["uptime"] = getUptimeSeconds();
    doc["rssi"] = WiFi.RSSI();
    doc["heap"] = ESP.getFreeHeap();
    
    // Add LED state
    JsonObject device = doc.createNestedObject("d1");
    device["s"] = led.state ? 1 : 0;
    device["v"] = led.brightness;
    device["t"] = "LED";
    device["runtime"] = led.totalRuntime;
    
    String jsonPayload;
    serializeJson(doc, jsonPayload);
    
    http.begin(GOOGLE_SCRIPT_URL);
    http.addHeader("Content-Type", "application/json");
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    http.setTimeout(10000);
    
    int httpCode = http.POST(jsonPayload);
    
    if (httpCode == 200) {
        String response = http.getString();
        StaticJsonDocument<512> respDoc;
        DeserializationError error = deserializeJson(respDoc, response);
        
        if (!error) {
            // Process cloud response
            if (respDoc.containsKey("d1") && respDoc["d1"].containsKey("update") && 
                respDoc["d1"]["update"] == true) {
                bool newState = (respDoc["d1"]["state"].as<int>() == 1);
                int newBrightness = respDoc["d1"]["val"].as<int>();
                setLEDState(newState, newBrightness);
            }
            
            if (respDoc.containsKey("sys_name")) {
                String newName = respDoc["sys_name"].as<String>();
                if (newName.length() > 0) {
                    led.name = newName;
                }
            }
        }
        
        http.end();
        return true;
    } else {
        http.end();
        return false;
    }
}

// ================================================================
// SETUP
// ================================================================

void setup() {
    Serial.begin(115200);
    delay(500);
    
    bootTime = millis();
    
    Serial.println("\n========================================");
    Serial.printf("Mini LED Controller v%s\n", FIRMWARE_VERSION);
    Serial.println("========================================");
    
    // Initialize LED with PWM
    ledcSetup(PWM_CHANNEL, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcAttachPin(LED_PIN, PWM_CHANNEL);
    ledcWrite(PWM_CHANNEL, 0);  // Start with LED off
    
    // Initialize LED state
    led.state = false;
    led.brightness = 100;
    led.name = "Built-in LED";
    led.lastOnTime = millis();
    led.totalRuntime = 0;
    
    // Get system UID
    systemUID = WiFi.macAddress();
    Serial.printf("[INFO] Device MAC: %s\n", systemUID.c_str());
    
    // Initialize WiFi with captive portal
    WiFiManager wm;
    wm.setConfigPortalTimeout(180);  // 3 minutes timeout
    
    if (!wm.autoConnect(DEVICE_NAME)) {
        Serial.println("[WARN] WiFi connection failed - Restarting...");
        delay(3000);
        ESP.restart();
    } else {
        Serial.printf("[INFO] WiFi connected: %s\n", WiFi.localIP().toString().c_str());
        Serial.printf("[INFO] Signal strength: %d dBm\n", WiFi.RSSI());
    }
    
    // Initialize mDNS
    if (MDNS.begin("mini-led")) {
        MDNS.addService("http", "tcp", LOCAL_SERVER_PORT);
        MDNS.addService("ws", "tcp", WEBSOCKET_PORT);
        Serial.println("[INFO] mDNS started: mini-led.local");
    }
    
    // Setup redirect server (port 80)
    redirectServer.on("/", []() {
        redirectServer.sendHeader("Location", "http://" + WiFi.localIP().toString() + ":" + String(LOCAL_SERVER_PORT), true);
        redirectServer.send(302, "text/plain", "Redirecting...");
    });
    redirectServer.begin();
    Serial.println("[INFO] Redirect server started on port 80");
    
    // Setup REST API server
    server.on("/", HTTP_GET, handleRoot);
    server.on("/status", HTTP_GET, handleGetStatus);
    server.on("/control", HTTP_POST, handlePostControl);
    server.on("/info", HTTP_GET, handleGetInfo);
    server.on("/config", HTTP_POST, handlePostConfig);
    
    // Enable CORS
    server.enableCORS(true);
    
    server.begin();
    Serial.printf("[INFO] REST API server started on port %d\n", LOCAL_SERVER_PORT);
    
    // Setup WebSocket server
    webSocket.begin();
    webSocket.onEvent(handleWebSocketEvent);
    Serial.printf("[INFO] WebSocket server started on port %d\n", WEBSOCKET_PORT);
    
    Serial.println("========================================");
    Serial.println("[INFO] System initialization complete");
    Serial.printf("[INFO] Free heap: %d bytes\n", ESP.getFreeHeap());
    Serial.println("========================================");
    Serial.printf("\n[INFO] Access web interface at: http://%s:%d\n\n", 
                  WiFi.localIP().toString().c_str(), LOCAL_SERVER_PORT);
}

// ================================================================
// MAIN LOOP
// ================================================================

void loop() {
    static unsigned long lastCloudSync = 0;
    unsigned long now = millis();
    
    // Handle web servers
    redirectServer.handleClient();
    server.handleClient();
    webSocket.loop();
    
    // Update runtime statistics
    updateRuntimeStatistics();
    
    // Cloud sync
    if (now - lastCloudSync >= CLOUD_POLL_INTERVAL_MS) {
        if (WiFi.status() == WL_CONNECTED) {
            cloudConnected = syncWithCloud();
        } else {
            cloudConnected = false;
        }
        lastCloudSync = now;
    }
    
    // Small delay to prevent watchdog triggers
    delay(10);
}
