/**
 * API Implementation for Local REST and WebSocket Endpoints
 * Complete implementation of all API functions declared in api.h
 */

#ifndef API_IMPL_H
#define API_IMPL_H

#include "api.h"
#include "config.h"
#include <ArduinoJson.h>

// External references to global objects and data
extern WebServer localServer;
extern WebSocketsServer webSocket;
extern Device devices[4];
extern Schedule schedules[SCHEDULE_MAX_COUNT];
extern Scene scenes[SCENE_MAX_COUNT];
extern String systemName;
extern bool cloudConnected;

// ================================================================
// UTILITY FUNCTIONS IMPLEMENTATION
// ================================================================

void sendJsonResponse(int code, const JsonDocument& doc) {
    String response;
    serializeJson(doc, response);
    localServer.send(code, "application/json", response);
}

void sendErrorResponse(int code, const char* message) {
    StaticJsonDocument<128> doc;
    doc["success"] = false;
    doc["error"] = message;
    sendJsonResponse(code, doc);
}

bool isValidDeviceId(int id) {
    return (id >= 0 && id < 4);
}

bool parseJsonBody(JsonDocument& doc) {
    if (!localServer.hasArg("plain")) {
        return false;
    }
    
    String body = localServer.arg("plain");
    DeserializationError error = deserializeJson(doc, body);
    
    if (error) {
        return false;
    }
    
    return true;
}

// ================================================================
// REST API ENDPOINT IMPLEMENTATIONS
// ================================================================

void handleGetStatus() {
    StaticJsonDocument<1024> doc;
    JsonArray devicesArray = doc.createNestedArray("devices");
    
    for (int i = 0; i < 4; i++) {
        JsonObject device = devicesArray.createNestedObject();
        device["id"] = i;
        device["name"] = devices[i].name;
        device["type"] = (int)devices[i].type;
        device["state"] = devices[i].state;
        device["brightness"] = devices[i].brightness;
        device["runtime"] = devices[i].totalRuntime;
        device["locked"] = devices[i].childLock;
    }
    
    sendJsonResponse(200, doc);
}

void handlePostControl() {
    StaticJsonDocument<256> doc;
    
    if (!parseJsonBody(doc)) {
        sendErrorResponse(400, "Invalid JSON");
        return;
    }
    
    if (!doc.containsKey("id")) {
        sendErrorResponse(400, "Missing 'id' parameter");
        return;
    }
    
    int deviceId = doc["id"].as<int>();
    
    if (!isValidDeviceId(deviceId)) {
        sendErrorResponse(400, "Invalid device ID");
        return;
    }
    
    if (devices[deviceId].childLock) {
        sendErrorResponse(403, "Device is locked");
        return;
    }
    
    bool state = doc.containsKey("state") ? doc["state"].as<bool>() : devices[deviceId].state;
    int brightness = doc.containsKey("brightness") ? doc["brightness"].as<int>() : devices[deviceId].brightness;
    bool fade = doc.containsKey("fade") ? doc["fade"].as<bool>() : true;
    
    // Validate brightness range
    if (brightness < 0 || brightness > 100) {
        sendErrorResponse(400, "Brightness must be 0-100");
        return;
    }
    
    // Apply control
    setDeviceState(deviceId, state, brightness, fade);
    
    StaticJsonDocument<64> response;
    response["success"] = true;
    sendJsonResponse(200, response);
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
    
    sendJsonResponse(200, doc);
}

void handlePostConfig() {
    StaticJsonDocument<512> doc;
    
    if (!parseJsonBody(doc)) {
        sendErrorResponse(400, "Invalid JSON");
        return;
    }
    
    if (!doc.containsKey("device_id")) {
        sendErrorResponse(400, "Missing 'device_id' parameter");
        return;
    }
    
    int deviceId = doc["device_id"].as<int>();
    
    if (!isValidDeviceId(deviceId)) {
        sendErrorResponse(400, "Invalid device ID");
        return;
    }
    
    bool configChanged = false;
    
    // Update device name
    if (doc.containsKey("name")) {
        String newName = doc["name"].as<String>();
        if (newName.length() > 0 && newName != devices[deviceId].name) {
            devices[deviceId].name = newName;
            configChanged = true;
        }
    }
    
    // Update device type
    if (doc.containsKey("type")) {
        int typeInt = doc["type"].as<int>();
        if (typeInt >= 0 && typeInt <= 2) {
            devices[deviceId].type = (DeviceType)typeInt;
            configChanged = true;
        }
    }
    
    // Update power-on behavior
    if (doc.containsKey("power_on_state")) {
        int ponInt = doc["power_on_state"].as<int>();
        if (ponInt >= 0 && ponInt <= 3) {
            devices[deviceId].powerOnBehavior = (PowerOnState)ponInt;
            configChanged = true;
        }
    }
    
    // Update default brightness
    if (doc.containsKey("default_brightness")) {
        int defBrightness = doc["default_brightness"].as<int>();
        if (defBrightness >= 0 && defBrightness <= 100) {
            devices[deviceId].defaultBrightness = defBrightness;
            configChanged = true;
        }
    }
    
    // Update auto-off
    if (doc.containsKey("auto_off")) {
        devices[deviceId].autoOffEnabled = doc["auto_off"].as<bool>();
        configChanged = true;
    }
    
    if (configChanged) {
        saveDeviceConfig();
    }
    
    StaticJsonDocument<64> response;
    response["success"] = true;
    sendJsonResponse(200, response);
}

void handleGetSchedules() {
    DynamicJsonDocument doc(2048);
    JsonArray schedulesArray = doc.createNestedArray("schedules");
    
    for (int i = 0; i < SCHEDULE_MAX_COUNT; i++) {
        if (schedules[i].deviceId < 0) continue;  // Skip empty slots
        
        JsonObject schedule = schedulesArray.createNestedObject();
        schedule["id"] = i;
        schedule["device_id"] = schedules[i].deviceId;
        schedule["start_mins"] = schedules[i].startMins;
        schedule["end_mins"] = schedules[i].endMins;
        schedule["start_brightness"] = schedules[i].startBrightness;
        schedule["end_brightness"] = schedules[i].endBrightness;
        schedule["active"] = schedules[i].active;
        schedule["days_of_week"] = schedules[i].daysOfWeek;
    }
    
    sendJsonResponse(200, doc);
}

void handlePostSchedule() {
    DynamicJsonDocument doc(512);
    
    if (!parseJsonBody(doc)) {
        sendErrorResponse(400, "Invalid JSON");
        return;
    }
    
    int scheduleId = doc.containsKey("id") ? doc["id"].as<int>() : -1;
    
    // Find or create schedule
    if (scheduleId < 0) {
        // Find empty slot
        for (int i = 0; i < SCHEDULE_MAX_COUNT; i++) {
            if (schedules[i].deviceId < 0) {
                scheduleId = i;
                break;
            }
        }
        
        if (scheduleId < 0) {
            sendErrorResponse(400, "No available schedule slots");
            return;
        }
    } else if (scheduleId >= SCHEDULE_MAX_COUNT) {
        sendErrorResponse(400, "Invalid schedule ID");
        return;
    }
    
    // Update schedule
    if (doc.containsKey("device_id")) {
        int devId = doc["device_id"].as<int>();
        if (!isValidDeviceId(devId)) {
            sendErrorResponse(400, "Invalid device ID");
            return;
        }
        schedules[scheduleId].deviceId = devId;
    }
    
    if (doc.containsKey("start_mins")) {
        schedules[scheduleId].startMins = doc["start_mins"].as<int>();
    }
    
    if (doc.containsKey("end_mins")) {
        schedules[scheduleId].endMins = doc["end_mins"].as<int>();
    }
    
    if (doc.containsKey("start_brightness")) {
        schedules[scheduleId].startBrightness = doc["start_brightness"].as<int>();
    }
    
    if (doc.containsKey("end_brightness")) {
        schedules[scheduleId].endBrightness = doc["end_brightness"].as<int>();
    }
    
    if (doc.containsKey("active")) {
        schedules[scheduleId].active = doc["active"].as<bool>();
    }
    
    if (doc.containsKey("days_of_week")) {
        schedules[scheduleId].daysOfWeek = doc["days_of_week"].as<uint8_t>();
    }
    
    saveSchedules();
    
    StaticJsonDocument<128> response;
    response["success"] = true;
    response["id"] = scheduleId;
    sendJsonResponse(200, response);
}

void handleDeleteSchedule() {
    String uri = localServer.uri();
    int scheduleId = uri.substring(uri.lastIndexOf('/') + 1).toInt();
    
    if (scheduleId < 0 || scheduleId >= SCHEDULE_MAX_COUNT) {
        sendErrorResponse(400, "Invalid schedule ID");
        return;
    }
    
    // Clear schedule
    schedules[scheduleId].deviceId = -1;
    schedules[scheduleId].active = false;
    saveSchedules();
    
    StaticJsonDocument<64> response;
    response["success"] = true;
    sendJsonResponse(200, response);
}

void handleGetScenes() {
    DynamicJsonDocument doc(2048);
    JsonArray scenesArray = doc.createNestedArray("scenes");
    
    for (int i = 0; i < SCENE_MAX_COUNT; i++) {
        if (!scenes[i].active || scenes[i].name.length() == 0) continue;
        
        JsonObject scene = scenesArray.createNestedObject();
        scene["id"] = i;
        scene["name"] = scenes[i].name;
        
        JsonArray devicesArray = scene.createNestedArray("devices");
        for (int j = 0; j < 4; j++) {
            if (scenes[i].devices[j].deviceId >= 0) {
                JsonObject device = devicesArray.createNestedObject();
                device["id"] = scenes[i].devices[j].deviceId;
                device["state"] = scenes[i].devices[j].state;
                device["brightness"] = scenes[i].devices[j].brightness;
            }
        }
    }
    
    sendJsonResponse(200, doc);
}

void handlePostScene() {
    DynamicJsonDocument doc(1024);
    
    if (!parseJsonBody(doc)) {
        sendErrorResponse(400, "Invalid JSON");
        return;
    }
    
    int sceneId = doc.containsKey("id") ? doc["id"].as<int>() : -1;
    
    // Find or create scene
    if (sceneId < 0) {
        for (int i = 0; i < SCENE_MAX_COUNT; i++) {
            if (!scenes[i].active) {
                sceneId = i;
                break;
            }
        }
        
        if (sceneId < 0) {
            sendErrorResponse(400, "No available scene slots");
            return;
        }
    } else if (sceneId >= SCENE_MAX_COUNT) {
        sendErrorResponse(400, "Invalid scene ID");
        return;
    }
    
    // Update scene
    if (doc.containsKey("name")) {
        scenes[sceneId].name = doc["name"].as<String>();
    }
    
    scenes[sceneId].active = true;
    
    if (doc.containsKey("devices")) {
        JsonArray devicesArray = doc["devices"].as<JsonArray>();
        int idx = 0;
        for (JsonObject device : devicesArray) {
            if (idx >= 4) break;
            
            scenes[sceneId].devices[idx].deviceId = device["id"].as<int>();
            scenes[sceneId].devices[idx].state = device["state"].as<bool>();
            scenes[sceneId].devices[idx].brightness = device["brightness"].as<int>();
            idx++;
        }
    }
    
    saveScenes();
    
    StaticJsonDocument<128> response;
    response["success"] = true;
    response["id"] = sceneId;
    sendJsonResponse(200, response);
}

void handleActivateScene() {
    String uri = localServer.uri();
    // Extract scene ID from URI like "/scenes/0/activate"
    int firstSlash = uri.indexOf('/');
    int secondSlash = uri.indexOf('/', firstSlash + 1);
    int thirdSlash = uri.indexOf('/', secondSlash + 1);
    
    String idStr = uri.substring(secondSlash + 1, thirdSlash);
    int sceneId = idStr.toInt();
    
    if (sceneId < 0 || sceneId >= SCENE_MAX_COUNT) {
        sendErrorResponse(400, "Invalid scene ID");
        return;
    }
    
    if (!scenes[sceneId].active) {
        sendErrorResponse(404, "Scene not found");
        return;
    }
    
    activateScene(sceneId);
    
    StaticJsonDocument<64> response;
    response["success"] = true;
    sendJsonResponse(200, response);
}

void handleDeleteScene() {
    String uri = localServer.uri();
    int sceneId = uri.substring(uri.lastIndexOf('/') + 1).toInt();
    
    if (sceneId < 0 || sceneId >= SCENE_MAX_COUNT) {
        sendErrorResponse(400, "Invalid scene ID");
        return;
    }
    
    scenes[sceneId].active = false;
    scenes[sceneId].name = "";
    saveScenes();
    
    StaticJsonDocument<64> response;
    response["success"] = true;
    sendJsonResponse(200, response);
}

void handleRestart() {
    StaticJsonDocument<64> doc;
    doc["success"] = true;
    doc["message"] = "Restarting device...";
    sendJsonResponse(200, doc);
    
    delay(500);
    ESP.restart();
}

void handleFactoryReset() {
    StaticJsonDocument<256> doc;
    
    if (!parseJsonBody(doc)) {
        sendErrorResponse(400, "Invalid JSON");
        return;
    }
    
    if (!doc.containsKey("confirm") || doc["confirm"].as<bool>() != true) {
        sendErrorResponse(400, "Confirmation required");
        return;
    }
    
    StaticJsonDocument<64> response;
    response["success"] = true;
    response["message"] = "Factory reset initiated...";
    sendJsonResponse(200, response);
    
    delay(500);
    
    // Clear WiFi settings
    WiFiManager wm;
    wm.resetSettings();
    
    // Clear all preferences
    preferences.begin(PREF_NAMESPACE, false);
    preferences.clear();
    preferences.end();
    
    ESP.restart();
}

// ================================================================
// WEBSOCKET IMPLEMENTATION
// ================================================================

void handleWebSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    switch(type) {
        case WStype_DISCONNECTED:
            logMessage(LOG_DEBUG, "WebSocket client #%u disconnected", num);
            break;
            
        case WStype_CONNECTED:
            {
                IPAddress ip = webSocket.remoteIP(num);
                logMessage(LOG_INFO, "WebSocket client #%u connected from %s", num, ip.toString().c_str());
                
                // Send current status to new client
                broadcastSystemStatus();
            }
            break;
            
        case WStype_TEXT:
            logMessage(LOG_DEBUG, "WebSocket message from #%u: %s", num, (char*)payload);
            // Could implement commands via WebSocket here
            break;
            
        case WStype_BIN:
        case WStype_ERROR:
        case WStype_FRAGMENT_TEXT_START:
        case WStype_FRAGMENT_BIN_START:
        case WStype_FRAGMENT:
        case WStype_FRAGMENT_FIN:
        case WStype_PING:
        case WStype_PONG:
            // Handle other types if needed
            break;
    }
}

void initWebSocket() {
    webSocket.begin();
    webSocket.onEvent(handleWebSocketEvent);
    logMessage(LOG_INFO, "WebSocket server initialized on port 81");
}

void broadcastSystemStatus() {
    DynamicJsonDocument doc(1024);
    doc["type"] = "system_status";
    doc["uptime"] = getUptimeSeconds();
    doc["rssi"] = WiFi.RSSI();
    doc["cloud_connected"] = cloudConnected;
    
    JsonArray devicesArray = doc.createNestedArray("devices");
    for (int i = 0; i < 4; i++) {
        JsonObject device = devicesArray.createNestedObject();
        device["id"] = i;
        device["state"] = devices[i].state;
        device["brightness"] = devices[i].brightness;
        device["name"] = devices[i].name;
    }
    
    String message;
    serializeJson(doc, message);
    webSocket.broadcastTXT(message);
}

// ================================================================
// API INITIALIZATION
// ================================================================

void initLocalAPI() {
    // REST endpoints
    localServer.on("/status", HTTP_GET, handleGetStatus);
    localServer.on("/control", HTTP_POST, handlePostControl);
    localServer.on("/info", HTTP_GET, handleGetInfo);
    localServer.on("/config", HTTP_POST, handlePostConfig);
    localServer.on("/schedules", HTTP_GET, handleGetSchedules);
    localServer.on("/schedules", HTTP_POST, handlePostSchedule);
    localServer.on("/scenes", HTTP_GET, handleGetScenes);
    localServer.on("/scenes", HTTP_POST, handlePostScene);
    localServer.on("/restart", HTTP_POST, handleRestart);
    localServer.on("/factory-reset", HTTP_POST, handleFactoryReset);
    
    // Pattern matching for DELETE and activate endpoints
    localServer.onNotFound([]() {
        String uri = localServer.uri();
        
        if (uri.startsWith("/schedules/") && localServer.method() == HTTP_DELETE) {
            handleDeleteSchedule();
        } else if (uri.startsWith("/scenes/") && uri.endsWith("/activate") && localServer.method() == HTTP_POST) {
            handleActivateScene();
        } else if (uri.startsWith("/scenes/") && localServer.method() == HTTP_DELETE) {
            handleDeleteScene();
        } else {
            sendErrorResponse(404, "Endpoint not found");
        }
    });
    
    localServer.begin();
    logMessage(LOG_INFO, "Local API server started on port 8080");
    
    // Initialize WebSocket
    initWebSocket();
}

void handleLocalAPIRequests() {
    localServer.handleClient();
    webSocket.loop();
}

#endif // API_IMPL_H
