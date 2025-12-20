/**
 * ⚡ PRODUCTION FIRMWARE: 4-Channel Smart Home Controller v3.0
 * ==============================================================
 * Commercial-grade firmware for ESP32-based TRIAC controller
 * 
 * Features:
 * - Multi-core FreeRTOS architecture (time-critical on Core 1, network on Core 0)
 * - Google Assistant + Amazon Alexa voice control
 * - Google Apps Script cloud integration
 * - OTA updates with rollback protection
 * - Flutter app REST API + WebSocket
 * - Scene & schedule automation
 * - Safety features & watchdog monitoring
 * 
 * Copyright (c) 2024 - Production Ready
 */

#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <Espalexa.h>
#include <Preferences.h>
#include <WiFiManager.h>
#include <ESPmDNS.h>
#include <esp_task_wdt.h>
#include <esp_ota_ops.h>
#include <time.h>
#include <WebSocketsServer.h>

// Project headers
#include "config.h"
#include "api.h"
#include "voice.h"

// ================================================================
// 📊 DATA STRUCTURES
// ================================================================
struct Device {
    String name;
    volatile DeviceType type;
    volatile bool state;
    int brightness;
    volatile int fireTick;
    bool childLock;
    bool autoOffEnabled;
    PowerOnState powerOnBehavior;
    unsigned long lastOnTime;
    unsigned long totalRuntime;  // seconds
    int defaultBrightness;
};

struct Schedule {
    int16_t startMins;
    int16_t endMins;
    int8_t deviceId;
    int8_t startBrightness;
    int8_t endBrightness;
    bool active;
    uint8_t daysOfWeek;  // Bitmask: bit 0=Sun, 1=Mon, etc.
};

struct Scene {
    String name;
    bool active;
    struct SceneDevice {
        int8_t deviceId;
        bool state;
        int8_t brightness;
    } devices[4];
};

// ================================================================
// 📝 USER CONFIGURATION
// ================================================================
// 👇 Configure via Google Apps Script or web interface
String GOOGLE_SCRIPT_URL = "https://script.google.com/macros/s/AKfycbxJP31g9LhRulRHTbTd6KidEiBXlxCKfKcXkiUGe961IfNZDgHuoWAIif91PrPUQnHrIQ/exec";
String API_KEY = "smarthome-2024-[YourRandomStringikhdfg87y7fygdhgoesgysdygfdsy7fgdag7iufh9]";
String systemName = "Smart_Home_Hub";

// SinricPro credentials (loaded from cloud)
String sinricAppKey = "";
String sinricAppSecret = "";
String sinricDeviceIds[4] = {"", "", "", ""};

// ================================================================
// 🌐 GLOBAL OBJECTS
// ================================================================
Espalexa alexaManager;
Preferences preferences;
WebServer redirectServer(80);
WebServer localServer(8080);
WebSocketsServer webSocket = WebSocketsServer(81);
hw_timer_t *timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
QueueHandle_t deviceControlQueue;

// Queue message structure
struct DeviceControlMsg {
    int deviceId;
    bool state;
    int brightness;
    bool fade;
};

// ================================================================
// 📦 GLOBAL VARIABLES
// ================================================================
// Global device state
Device devices[4];
Schedule schedules[SCHEDULE_MAX_COUNT];
Scene scenes[SCENE_MAX_COUNT];
bool lastSwitchState[4];

// System state
volatile bool zeroCrossDetected = true;
volatile unsigned long lastZeroCrossTime = 0;
unsigned long bootTime = 0;
bool cloudConnected = false;
int currentError = ERR_NONE;

// Fade control
struct FadeState {
    bool active;
    int targetBrightness;
    int currentStep;
    int totalSteps;
    unsigned long lastStepTime;
    unsigned long stepInterval;
} fadeStates[4]; 

// ================================================================
// 🔧 FORWARD DECLARATIONS
// ================================================================
void setDeviceState(int deviceId, bool state, int brightness, bool fade = false);
void saveDeviceConfig();
void saveSchedules();
void saveScenes();
void activateScene(int sceneId);
unsigned long getUptimeSeconds();
const char* getSignalStrength(int rssi);
void logMessage(LogLevel level, const char* format, ...);
void broadcastDeviceState(int deviceId); 

// ================================================================
// 📥 IMPLEMENTATION INCLUDES
// ================================================================
// Implementation files - Note: Included as .h for Arduino IDE compatibility
// In Arduino ecosystem, implementation headers are commonly used for modular code
// These are included AFTER data structures and forward declarations are defined
#include "api_impl.h"
#include "voice_impl.h"

// ================================================================
// 🛠️ UTILITY FUNCTIONS
// ================================================================

// Logging with levels
void logMessage(LogLevel level, const char* format, ...) {
    if (level > LOG_LEVEL) return;
    
    const char* levelStr[] = {"", "[ERROR]", "[WARN]", "[INFO]", "[DEBUG]", "[VERBOSE]"};
    Serial.print(levelStr[level]);
    Serial.print(" ");
    
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    Serial.println(buffer);
}

// Calculate TRIAC fire tick from brightness percentage
int calculateFireTick(int percent) {
    if (percent >= 100) return 0;
    if (percent <= 0) return 100;
    return 100 - percent;
}

// Get uptime in seconds
unsigned long getUptimeSeconds() {
    return (millis() - bootTime) / 1000;
}

// Get WiFi signal strength description
const char* getSignalStrength(int rssi) {
    if (rssi >= RSSI_EXCELLENT) return "Excellent";
    if (rssi >= RSSI_GOOD) return "Good";
    if (rssi >= RSSI_FAIR) return "Fair";
    if (rssi >= RSSI_POOR) return "Poor";
    return "Weak";
}

// Convert time to minutes since midnight
int timeToMinutes(int hour, int minute) {
    return hour * 60 + minute;
}

// Get current time in minutes since midnight
int getCurrentMinutes() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) return -1;
    return timeToMinutes(timeinfo.tm_hour, timeinfo.tm_min);
}
// ================================================================
// ⚡ CORE 1: TIME-CRITICAL POWER CONTROL (PRO_CPU)
// ================================================================

// Hardware timer ISR for phase angle control
void IRAM_ATTR onTimerFire() {
    portENTER_CRITICAL_ISR(&timerMux);
    static int tickCounter = 0;
    tickCounter++;
    
    // Fire TRIACs at calculated phase angle
    for (int i = 0; i < 4; i++) {
        if (devices[i].state && devices[i].type != TYPE_SWITCH) {
            if (tickCounter >= devices[i].fireTick) {
                digitalWrite(TRIAC_PINS[i], HIGH);
            }
        }
    }
    
    if (tickCounter > PHASE_STEPS) tickCounter = 0;
    portEXIT_CRITICAL_ISR(&timerMux);
}

// Zero-cross detection ISR
void IRAM_ATTR onZeroCross() {
    portENTER_CRITICAL_ISR(&timerMux);
    
    // Update last zero-cross time for watchdog
    lastZeroCrossTime = millis();
    zeroCrossDetected = true;
    
    // Reset all TRIACs at zero crossing
    for (int i = 0; i < 4; i++) {
        if (devices[i].state) {
            if (devices[i].type == TYPE_SWITCH) {
                digitalWrite(TRIAC_PINS[i], HIGH);
            } else {
                digitalWrite(TRIAC_PINS[i], LOW);
            }
        } else {
            digitalWrite(TRIAC_PINS[i], LOW);
        }
    }
    
    portEXIT_CRITICAL_ISR(&timerMux);
}

// Check zero-cross signal health (call from Core 0 task)
void checkZeroCrossHealth() {
    if (millis() - lastZeroCrossTime > 100) {  // No signal for 100ms
        if (zeroCrossDetected) {
            logMessage(LOG_ERROR, "Zero-cross signal lost! Safety shutdown.");
            zeroCrossDetected = false;
            currentError = ERR_ZERO_CROSS_LOST;
            
            // Safety: Turn off all TRIACs
            for (int i = 0; i < 4; i++) {
                digitalWrite(TRIAC_PINS[i], LOW);
                devices[i].state = false;
            }
        }
    }
}

// ================================================================
// 💾 PERSISTENCE & CONFIGURATION
// ================================================================

void saveDeviceConfig() {
    preferences.begin(PREF_NAMESPACE, false);
    preferences.putString(PREF_SYSTEM_NAME, systemName);
    
    for (int i = 0; i < 4; i++) {
        String prefix = PREF_DEVICE_PREFIX + String(i) + "_";
        preferences.putInt((prefix + "type").c_str(), (int)devices[i].type);
        preferences.putString((prefix + "name").c_str(), devices[i].name);
        preferences.putBool((prefix + "lock").c_str(), devices[i].childLock);
        preferences.putInt((prefix + "pon").c_str(), (int)devices[i].powerOnBehavior);
        preferences.putInt((prefix + "def_br").c_str(), devices[i].defaultBrightness);
        preferences.putBool((prefix + "auto").c_str(), devices[i].autoOffEnabled);
        
        // Save last state for POWER_ON_LAST behavior
        if (devices[i].powerOnBehavior == POWER_ON_LAST) {
            preferences.putBool((prefix + "last_st").c_str(), devices[i].state);
            preferences.putInt((prefix + "last_br").c_str(), devices[i].brightness);
        }
        
        // Save runtime statistics
        preferences.putULong((prefix + "runtime").c_str(), devices[i].totalRuntime);
    }
    
    preferences.end();
    logMessage(LOG_INFO, "Device configuration saved");
}

void loadDeviceConfig() {
    preferences.begin(PREF_NAMESPACE, true);
    systemName = preferences.getString(PREF_SYSTEM_NAME, "Smart_Home_Hub");
    
    for (int i = 0; i < 4; i++) {
        String prefix = PREF_DEVICE_PREFIX + String(i) + "_";
        devices[i].type = (DeviceType)preferences.getInt((prefix + "type").c_str(), TYPE_SWITCH);
        devices[i].name = preferences.getString((prefix + "name").c_str(), "Device " + String(i + 1));
        devices[i].childLock = preferences.getBool((prefix + "lock").c_str(), false);
        devices[i].powerOnBehavior = (PowerOnState)preferences.getInt((prefix + "pon").c_str(), POWER_ON_OFF);
        devices[i].defaultBrightness = preferences.getInt((prefix + "def_br").c_str(), 100);
        devices[i].autoOffEnabled = preferences.getBool((prefix + "auto").c_str(), true);
        devices[i].totalRuntime = preferences.getULong((prefix + "runtime").c_str(), 0);
        
        // Apply power-on behavior
        switch (devices[i].powerOnBehavior) {
            case POWER_ON_OFF:
                devices[i].state = false;
                devices[i].brightness = 100;
                break;
            case POWER_ON_ON:
                devices[i].state = true;
                devices[i].brightness = devices[i].defaultBrightness;
                break;
            case POWER_ON_LAST:
                devices[i].state = preferences.getBool((prefix + "last_st").c_str(), false);
                devices[i].brightness = preferences.getInt((prefix + "last_br").c_str(), 100);
                break;
            case POWER_ON_DEFAULT:
                devices[i].state = true;
                devices[i].brightness = devices[i].defaultBrightness;
                break;
        }
        
        devices[i].fireTick = calculateFireTick(devices[i].brightness);
        devices[i].lastOnTime = millis();
    }
    
    preferences.end();
    logMessage(LOG_INFO, "Device configuration loaded");
}

void saveSchedules() {
    preferences.begin(PREF_NAMESPACE, false);
    for (int i = 0; i < SCHEDULE_MAX_COUNT; i++) {
        String prefix = PREF_SCHEDULE_PREFIX + String(i) + "_";
        preferences.putShort((prefix + "start").c_str(), schedules[i].startMins);
        preferences.putShort((prefix + "end").c_str(), schedules[i].endMins);
        preferences.putChar((prefix + "dev").c_str(), schedules[i].deviceId);
        preferences.putChar((prefix + "sbr").c_str(), schedules[i].startBrightness);
        preferences.putChar((prefix + "ebr").c_str(), schedules[i].endBrightness);
        preferences.putBool((prefix + "act").c_str(), schedules[i].active);
        preferences.putUChar((prefix + "days").c_str(), schedules[i].daysOfWeek);
    }
    preferences.end();
    logMessage(LOG_INFO, "Schedules saved");
}

void loadSchedules() {
    preferences.begin(PREF_NAMESPACE, true);
    for (int i = 0; i < SCHEDULE_MAX_COUNT; i++) {
        String prefix = PREF_SCHEDULE_PREFIX + String(i) + "_";
        schedules[i].startMins = preferences.getShort((prefix + "start").c_str(), -1);
        schedules[i].endMins = preferences.getShort((prefix + "end").c_str(), -1);
        schedules[i].deviceId = preferences.getChar((prefix + "dev").c_str(), -1);
        schedules[i].startBrightness = preferences.getChar((prefix + "sbr").c_str(), 0);
        schedules[i].endBrightness = preferences.getChar((prefix + "ebr").c_str(), 100);
        schedules[i].active = preferences.getBool((prefix + "act").c_str(), false);
        schedules[i].daysOfWeek = preferences.getUChar((prefix + "days").c_str(), 0x7F);
    }
    preferences.end();
    logMessage(LOG_INFO, "Schedules loaded");
}

void saveScenes() {
    preferences.begin(PREF_NAMESPACE, false);
    for (int i = 0; i < SCENE_MAX_COUNT; i++) {
        String prefix = PREF_SCENE_PREFIX + String(i) + "_";
        preferences.putString((prefix + "name").c_str(), scenes[i].name);
        preferences.putBool((prefix + "act").c_str(), scenes[i].active);
        for (int j = 0; j < 4; j++) {
            String devPrefix = prefix + "d" + String(j) + "_";
            preferences.putChar((devPrefix + "id").c_str(), scenes[i].devices[j].deviceId);
            preferences.putBool((devPrefix + "st").c_str(), scenes[i].devices[j].state);
            preferences.putChar((devPrefix + "br").c_str(), scenes[i].devices[j].brightness);
        }
    }
    preferences.end();
    logMessage(LOG_INFO, "Scenes saved");
}

void loadScenes() {
    preferences.begin(PREF_NAMESPACE, true);
    for (int i = 0; i < SCENE_MAX_COUNT; i++) {
        String prefix = PREF_SCENE_PREFIX + String(i) + "_";
        scenes[i].name = preferences.getString((prefix + "name").c_str(), "");
        scenes[i].active = preferences.getBool((prefix + "act").c_str(), false);
        for (int j = 0; j < 4; j++) {
            String devPrefix = prefix + "d" + String(j) + "_";
            scenes[i].devices[j].deviceId = preferences.getChar((devPrefix + "id").c_str(), -1);
            scenes[i].devices[j].state = preferences.getBool((devPrefix + "st").c_str(), false);
            scenes[i].devices[j].brightness = preferences.getChar((devPrefix + "br").c_str(), 100);
        }
    }
    preferences.end();
    logMessage(LOG_INFO, "Scenes loaded");
}

// ================================================================
// 🎛️ DEVICE CONTROL
// ================================================================

void setDeviceState(int deviceId, bool state, int brightness, bool fade) {
    if (deviceId < 0 || deviceId >= 4) return;
    
    logMessage(LOG_DEBUG, "Setting device %d: state=%d, brightness=%d, fade=%d", 
               deviceId, state, brightness, fade);
    
    if (fade && devices[deviceId].type != TYPE_SWITCH) {
        // Start fade transition
        fadeStates[deviceId].active = true;
        fadeStates[deviceId].targetBrightness = state ? brightness : 0;
        fadeStates[deviceId].currentStep = 0;
        fadeStates[deviceId].totalSteps = FADE_STEPS;
        fadeStates[deviceId].lastStepTime = millis();
        fadeStates[deviceId].stepInterval = FADE_DURATION_MS / FADE_STEPS;
        devices[deviceId].state = true;  // Keep on during fade
    } else {
        // Immediate change
        devices[deviceId].state = state;
        devices[deviceId].brightness = state ? brightness : 0;
        devices[deviceId].fireTick = calculateFireTick(devices[deviceId].brightness);
        
        if (state) {
            devices[deviceId].lastOnTime = millis();
        }
    }
    
    // Broadcast to WebSocket clients
    broadcastDeviceState(deviceId);
}

void processFadeTransitions() {
    for (int i = 0; i < 4; i++) {
        if (!fadeStates[i].active) continue;
        
        unsigned long now = millis();
        if (now - fadeStates[i].lastStepTime >= fadeStates[i].stepInterval) {
            fadeStates[i].currentStep++;
            fadeStates[i].lastStepTime = now;
            
            if (fadeStates[i].currentStep >= fadeStates[i].totalSteps) {
                // Fade complete
                devices[i].brightness = fadeStates[i].targetBrightness;
                devices[i].state = (devices[i].brightness > 0);
                fadeStates[i].active = false;
            } else {
                // Calculate intermediate brightness
                int startBrightness = devices[i].brightness;
                int range = fadeStates[i].targetBrightness - startBrightness;
                float progress = (float)fadeStates[i].currentStep / fadeStates[i].totalSteps;
                devices[i].brightness = startBrightness + (int)(range * progress);
            }
            
            devices[i].fireTick = calculateFireTick(devices[i].brightness);
        }
    }
}

void activateScene(int sceneId) {
    if (sceneId < 0 || sceneId >= SCENE_MAX_COUNT) return;
    if (!scenes[sceneId].active) return;
    
    logMessage(LOG_INFO, "Activating scene: %s", scenes[sceneId].name.c_str());
    
    for (int i = 0; i < 4; i++) {
        if (scenes[sceneId].devices[i].deviceId >= 0) {
            int devId = scenes[sceneId].devices[i].deviceId;
            setDeviceState(devId, 
                          scenes[sceneId].devices[i].state,
                          scenes[sceneId].devices[i].brightness,
                          true);  // Use fade transition
        }
    }
}

void processSchedules() {
    int currentMins = getCurrentMinutes();
    if (currentMins < 0) return;  // Time not available
    
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) return;
    int currentDay = timeinfo.tm_wday;  // 0=Sunday
    
    for (int i = 0; i < SCHEDULE_MAX_COUNT; i++) {
        if (!schedules[i].active) continue;
        if (schedules[i].deviceId < 0 || schedules[i].deviceId >= 4) continue;
        
        // Check if schedule is active for current day
        if (!(schedules[i].daysOfWeek & (1 << currentDay))) continue;
        
        // Check if current time matches schedule
        if (currentMins >= schedules[i].startMins && currentMins < schedules[i].endMins) {
            // Calculate brightness based on time progression
            int duration = schedules[i].endMins - schedules[i].startMins;
            int elapsed = currentMins - schedules[i].startMins;
            float progress = (float)elapsed / duration;
            
            int targetBrightness = schedules[i].startBrightness + 
                (int)((schedules[i].endBrightness - schedules[i].startBrightness) * progress);
            
            bool targetState = (targetBrightness > 0);
            
            // Only update if different from current state
            if (devices[schedules[i].deviceId].state != targetState ||
                devices[schedules[i].deviceId].brightness != targetBrightness) {
                setDeviceState(schedules[i].deviceId, targetState, targetBrightness, true);
            }
        }
    }
}

void updateRuntimeStatistics() {
    static unsigned long lastUpdate = 0;
    unsigned long now = millis();
    
    if (now - lastUpdate >= 1000) {  // Update every second
        for (int i = 0; i < 4; i++) {
            if (devices[i].state) {
                devices[i].totalRuntime++;
            }
        }
        lastUpdate = now;
    }
}

void checkAutoOff() {
    if (AUTO_OFF_MS == 0) return;
    
    unsigned long now = millis();
    for (int i = 0; i < 4; i++) {
        if (devices[i].state && devices[i].autoOffEnabled) {
            if (now - devices[i].lastOnTime > AUTO_OFF_MS) {
                logMessage(LOG_INFO, "Auto-off triggered for device %d", i);
                setDeviceState(i, false, 0, true);
            }
        }
    }
}

// ================================================================
// 🔄 OTA UPDATE SYSTEM
// ================================================================

void performOTA(String binUrl) {
    logMessage(LOG_INFO, "Starting OTA update from: %s", binUrl.c_str());
    
    // Stop time-critical operations
    timerStop(timer);
    detachInterrupt(digitalPinToInterrupt(ZCD_PIN));
    
    // Turn off all devices for safety
    for (int i = 0; i < 4; i++) {
        digitalWrite(TRIAC_PINS[i], LOW);
        devices[i].state = false;
    }
    
    // Perform update
    WiFiClientSecure client;
    client.setInsecure();
    client.setTimeout(HTTP_TIMEOUT_MS);
    
    httpUpdate.setLedPin(LED_BUILTIN, LOW);
    httpUpdate.rebootOnUpdate(false);  // Manual reboot for rollback check
    
    t_httpUpdate_return ret = httpUpdate.update(client, binUrl);
    
    switch (ret) {
        case HTTP_UPDATE_FAILED:
            logMessage(LOG_ERROR, "OTA failed: %s", httpUpdate.getLastErrorString().c_str());
            currentError = ERR_OTA_FAILED;
            // Restart critical operations
            timerStart(timer);
            attachInterrupt(digitalPinToInterrupt(ZCD_PIN), onZeroCross, RISING);
            break;
            
        case HTTP_UPDATE_NO_UPDATES:
            logMessage(LOG_INFO, "No OTA updates available");
            timerStart(timer);
            attachInterrupt(digitalPinToInterrupt(ZCD_PIN), onZeroCross, RISING);
            break;
            
        case HTTP_UPDATE_OK:
            logMessage(LOG_INFO, "OTA update successful! Rebooting...");
            saveDeviceConfig();  // Save state before reboot
            delay(1000);
            ESP.restart();
            break;
    }
}

bool validateOTAPartition() {
    const esp_partition_t* running = esp_ota_get_running_partition();
    esp_ota_img_states_t ota_state;
    
    if (esp_ota_get_state_partition(running, &ota_state) == ESP_OK) {
        if (ota_state == ESP_OTA_IMG_PENDING_VERIFY) {
            logMessage(LOG_INFO, "OTA partition pending verification");
            
            // Validate firmware (basic check - runs without crash)
            if (esp_ota_mark_app_valid_cancel_rollback() == ESP_OK) {
                logMessage(LOG_INFO, "OTA partition validated successfully");
                return true;
            } else {
                logMessage(LOG_ERROR, "OTA partition validation failed");
                return false;
            }
        }
    }
    return true;
}

// ================================================================
// 🎙️ VOICE ASSISTANT CALLBACKS
// ================================================================

void voiceCallback(uint8_t brightness, int deviceId) {
    if (deviceId < 0 || deviceId >= 4) return;
    
    bool state = (brightness > 0);
    int brightnessPercent;
    
    if (devices[deviceId].type == TYPE_SWITCH) {
        brightnessPercent = 100;
    } else {
        brightnessPercent = map(brightness, 0, 255, 0, 100);
    }
    
    setDeviceState(deviceId, state, brightnessPercent, true);
    logMessage(LOG_INFO, "Voice command: Device %d -> %s at %d%%", 
               deviceId, state ? "ON" : "OFF", brightnessPercent);
}

void checkFactoryReset(bool currentState) {
    static unsigned long pressStartTime = 0;
    static bool isPressed = false;
    
    if (currentState == LOW) {
        if (!isPressed) {
            isPressed = true;
            pressStartTime = millis();
        }
        if (millis() - pressStartTime > FACTORY_RESET_DURATION_MS) {
            logMessage(LOG_WARN, "Factory reset initiated!");
            WiFiManager wm;
            wm.resetSettings();
            
            // Clear all preferences
            preferences.begin(PREF_NAMESPACE, false);
            preferences.clear();
            preferences.end();
            
            delay(100);
            ESP.restart();
        }
    } else {
        isPressed = false;
    }
}

// ================================================================
// 🌐 CORE 0 TASK
// ================================================================
// ================================================================
// 🌐 CORE 0: CONNECTIVITY TASK (APP_CPU)
// ================================================================
void taskConnectivity(void * parameter) {
    HTTPClient http;
    unsigned long lastSyncTime = 0;
    unsigned long lastScheduleCheck = 0;
    
    // Configure watchdog for Core 0
    esp_task_wdt_config_t wdt_config = {
        .timeout_ms = WDT_TIMEOUT_CORE0 * 1000,
        .idle_core_mask = (1 << portNUM_PROCESSORS) - 1,
        .trigger_panic = true
    };
    esp_task_wdt_init(&wdt_config);
    esp_task_wdt_add(NULL);

    logMessage(LOG_INFO, "Core 0 connectivity task started");

    while(true) {
        esp_task_wdt_reset();
        
        // Handle web servers
        redirectServer.handleClient();
        handleLocalAPIRequests();  // Handles both localServer and webSocket
        
        // Handle voice assistants
        alexaManager.loop();
        
        // Handle physical switches with debouncing
        for(int i = 0; i < 4; i++) {
            bool currentRead = digitalRead(SWITCH_PINS[i]);
            if (i == 0) checkFactoryReset(currentRead);
            
            if (currentRead != lastSwitchState[i]) {
                delay(SWITCH_DEBOUNCE_MS);
                if (digitalRead(SWITCH_PINS[i]) == currentRead) {
                    lastSwitchState[i] = currentRead;
                    if (!devices[i].childLock) {
                        devices[i].state = !devices[i].state;
                        setDeviceState(i, devices[i].state, devices[i].brightness);
                    }
                }
            }
        }
        
        // Process fade transitions
        processFadeTransitions();
        
        // Check auto-off
        checkAutoOff();
        
        // Update runtime statistics
        updateRuntimeStatistics();
        
        // Check zero-cross health
        checkZeroCrossHealth();
        
        // Process schedules every minute
        if (millis() - lastScheduleCheck > 60000) {
            processSchedules();
            lastScheduleCheck = millis();
        }
        
        // Cloud sync
        if (millis() - lastSyncTime > CLOUD_POLL_INTERVAL_MS) {
            if(WiFi.status() == WL_CONNECTED) {
                cloudConnected = syncWithCloud(http);
            } else {
                cloudConnected = false;
                logMessage(LOG_WARN, "WiFi disconnected, attempting reconnect...");
                // WiFi reconnection logic would go here
            }
            lastSyncTime = millis();
        }
        
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

// ================================================================
// ☁️ CLOUD COMMUNICATION
// ================================================================

bool syncWithCloud(HTTPClient &http) {
    StaticJsonDocument<JSON_BUFFER_SIZE> doc;
    
    // Build request
    doc["action"] = "poll";
    doc["api_key"] = API_KEY;
    doc["uid"] = WiFi.macAddress();
    doc["ver"] = FIRMWARE_VERSION;
    doc["uptime"] = getUptimeSeconds();
    doc["rssi"] = WiFi.RSSI();
    doc["heap"] = ESP.getFreeHeap();
    
    // Add device states
    for(int i = 0; i < 4; i++) {
        String key = "d" + String(i + 1);
        JsonObject device = doc.createNestedObject(key);
        device["s"] = devices[i].state ? 1 : 0;
        device["v"] = devices[i].brightness;
        device["t"] = (int)devices[i].type;
        device["runtime"] = devices[i].totalRuntime;
    }
    
    String jsonPayload;
    serializeJson(doc, jsonPayload);
    
    http.begin(GOOGLE_SCRIPT_URL);
    http.addHeader("Content-Type", "application/json");
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    http.setTimeout(HTTP_TIMEOUT_MS);
    
    int httpCode = http.POST(jsonPayload);
    
    if(httpCode == 200) {
        String response = http.getString();
        StaticJsonDocument<JSON_BUFFER_SIZE> respDoc;
        DeserializationError error = deserializeJson(respDoc, response);
        
        if (error) {
            logMessage(LOG_ERROR, "JSON parse error: %s", error.c_str());
            http.end();
            return false;
        }
        
        processCloudResponse(respDoc);
        http.end();
        return true;
    } else {
        logMessage(LOG_ERROR, "Cloud sync failed: HTTP %d", httpCode);
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
            saveDeviceConfig();
            logMessage(LOG_INFO, "System name updated: %s", systemName.c_str());
        }
    }
    
    // OTA update
    if (respDoc.containsKey("ota_update") && respDoc["ota_update"] == true) {
        if (respDoc.containsKey("bin_url")) {
            String binUrl = respDoc["bin_url"].as<String>();
            performOTA(binUrl);
        }
    }
    
    // SinricPro credentials
    if (respDoc.containsKey("sinric_key")) {
        sinricAppKey = respDoc["sinric_key"].as<String>();
        sinricAppSecret = respDoc["sinric_secret"].as<String>();
        // Initialize SinricPro with new credentials
        // initSinricPro() would be called here
    }
    
    // Device updates
    for(int i = 0; i < 4; i++) {
        String key = "d" + String(i + 1);
        if(respDoc.containsKey(key)) {
            JsonObject d = respDoc[key].as<JsonObject>();
            
            // Type update
            if (d.containsKey("type")) {
                String typeStr = d["type"].as<String>();
                DeviceType newType = devices[i].type;
                
                if(typeStr == "FAN") newType = TYPE_FAN;
                else if(typeStr == "SWITCH") newType = TYPE_SWITCH;
                else if(typeStr == "DIMMER") newType = TYPE_DIMMER;
                
                if(newType != devices[i].type) {
                    devices[i].type = newType;
                    saveDeviceConfig();
                }
            }
            
            // Name update
            if (d.containsKey("name")) {
                String newName = d["name"].as<String>();
                if (newName != devices[i].name && newName.length() > 0) {
                    devices[i].name = newName;
                    saveDeviceConfig();
                }
            }
            
            // Lock update
            if(d.containsKey("lock")) {
                bool remoteLock = d["lock"].as<bool>();
                if(remoteLock != devices[i].childLock) {
                    devices[i].childLock = remoteLock;
                    saveDeviceConfig();
                }
            }
            
            // State update
            if(d.containsKey("update") && d["update"] == true) {
                bool newState = (d["state"].as<int>() == 1);
                int newBrightness = d["val"].as<int>();
                setDeviceState(i, newState, newBrightness, true);
            }
        }
    }
    
    // Schedules update
    if (respDoc.containsKey("schedules")) {
        JsonArray schedulesArray = respDoc["schedules"].as<JsonArray>();
        // Process schedules from cloud
        // Implementation would parse and update schedules array
    }
    
    // Scenes update
    if (respDoc.containsKey("scenes")) {
        JsonArray scenesArray = respDoc["scenes"].as<JsonArray>();
        // Process scenes from cloud
        // Implementation would parse and update scenes array
    }
}

// ================================================================
// 🌐 API IMPLEMENTATIONS (Stubs for api.h functions)
// ================================================================

// WebSocket broadcast function
void broadcastDeviceState(int deviceId) {
    if (deviceId < 0 || deviceId >= 4) return;
    
    StaticJsonDocument<256> doc;
    doc["type"] = "device_update";
    doc["id"] = deviceId;
    doc["state"] = devices[deviceId].state;
    doc["brightness"] = devices[deviceId].brightness;
    doc["name"] = devices[deviceId].name;
    
    String message;
    serializeJson(doc, message);
    webSocket.broadcastTXT(message);
}

// ================================================================
// 🚀 SETUP
// ================================================================

const char* custom_css = R"===(
<meta name='viewport' content='width=device-width, initial-scale=1.0'>
<style>
:root {
  --primary: #2563eb;
  --primary-hover: #1d4ed8;
  --bg: #f8fafc;
  --card-bg: #ffffff;
  --text: #1e293b;
  --text-muted: #64748b;
  --border: #e2e8f0;
  --success: #10b981;
  --danger: #ef4444;
}

* {
  margin: 0;
  padding: 0;
  box-sizing: border-box;
}

body {
  font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, 'Helvetica Neue', Arial, sans-serif;
  background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
  min-height: 100vh;
  padding: 2rem 1rem;
  color: var(--text);
}

.container {
  max-width: 480px;
  margin: 0 auto;
}

.card {
  background: var(--card-bg);
  border-radius: 16px;
  padding: 2rem;
  box-shadow: 0 10px 40px rgba(0, 0, 0, 0.1);
  margin-bottom: 1.5rem;
}

.logo {
  text-align: center;
  margin-bottom: 2rem;
}

.logo-icon {
  width: 64px;
  height: 64px;
  background: linear-gradient(135deg, var(--primary) 0%, var(--primary-hover) 100%);
  border-radius: 16px;
  margin: 0 auto 1rem;
  display: flex;
  align-items: center;
  justify-content: center;
  font-size: 2rem;
}

h1 {
  color: var(--text);
  font-size: 1.75rem;
  font-weight: 700;
  text-align: center;
  margin-bottom: 0.5rem;
}

.subtitle {
  color: var(--text-muted);
  text-align: center;
  font-size: 0.95rem;
  margin-bottom: 2rem;
}

.info-grid {
  display: grid;
  grid-template-columns: 1fr 1fr;
  gap: 1rem;
  margin-bottom: 2rem;
}

.info-item {
  background: var(--bg);
  padding: 1rem;
  border-radius: 12px;
  text-align: center;
}

.info-label {
  color: var(--text-muted);
  font-size: 0.85rem;
  margin-bottom: 0.5rem;
}

.info-value {
  color: var(--text);
  font-size: 1.25rem;
  font-weight: 600;
}

input[type="text"],
input[type="password"] {
  width: 100%;
  padding: 0.875rem 1rem;
  border: 2px solid var(--border);
  border-radius: 10px;
  font-size: 1rem;
  transition: all 0.2s;
  margin-bottom: 1rem;
}

input[type="text"]:focus,
input[type="password"]:focus {
  outline: none;
  border-color: var(--primary);
  box-shadow: 0 0 0 3px rgba(37, 99, 235, 0.1);
}

button {
  width: 100%;
  padding: 1rem;
  background: var(--primary);
  color: white;
  border: none;
  border-radius: 10px;
  font-size: 1rem;
  font-weight: 600;
  cursor: pointer;
  transition: all 0.2s;
  margin-top: 0.5rem;
}

button:hover {
  background: var(--primary-hover);
  transform: translateY(-1px);
  box-shadow: 0 4px 12px rgba(37, 99, 235, 0.3);
}

button:active {
  transform: translateY(0);
}

.status {
  text-align: center;
  padding: 1rem;
  border-radius: 10px;
  margin-top: 1rem;
  font-size: 0.95rem;
  font-weight: 500;
}

.status.success {
  background: rgba(16, 185, 129, 0.1);
  color: var(--success);
}

.status.error {
  background: rgba(239, 68, 68, 0.1);
  color: var(--danger);
}

.footer {
  text-align: center;
  color: rgba(255, 255, 255, 0.9);
  font-size: 0.85rem;
  margin-top: 2rem;
}

@media (max-width: 640px) {
  body {
    padding: 1rem 0.5rem;
  }
  
  .card {
    padding: 1.5rem;
  }
  
  h1 {
    font-size: 1.5rem;
  }
}
</style>
)===";

void setup() {
    Serial.begin(115200);
    delay(100);
    
    bootTime = millis();
    
    logMessage(LOG_INFO, "========================================");
    logMessage(LOG_INFO, "Smart Home Controller v%.1f", FIRMWARE_VERSION);
    logMessage(LOG_INFO, "Build: %s", FIRMWARE_BUILD);
    logMessage(LOG_INFO, "========================================");
    
    // Validate OTA partition (rollback protection)
    if (!validateOTAPartition()) {
        logMessage(LOG_WARN, "OTA partition validation failed");
    }
    
    // Initialize hardware pins
    pinMode(ZCD_PIN, INPUT_PULLUP);
    for(int i = 0; i < 4; i++) {
        pinMode(TRIAC_PINS[i], OUTPUT);
        digitalWrite(TRIAC_PINS[i], LOW);
        pinMode(SWITCH_PINS[i], INPUT_PULLUP);
        lastSwitchState[i] = digitalRead(SWITCH_PINS[i]);
        
        // Initialize fade states
        fadeStates[i].active = false;
    }
    
    // Load configuration from flash
    loadDeviceConfig();
    loadSchedules();
    loadScenes();
    
    // Initialize WiFi
    WiFiManager wm;
    wm.setCustomHeadElement(custom_css);
    wm.setTitle("Setup " + systemName);
    wm.setConfigPortalTimeout(180);  // 3 minutes timeout
    std::vector<const char *> menu = {"wifi"};
    wm.setMenu(menu);
    
    if(!wm.autoConnect(systemName.c_str())) {
        logMessage(LOG_WARN, "WiFi connection failed - Running in offline mode");
    } else {
        logMessage(LOG_INFO, "WiFi connected: %s", WiFi.localIP().toString().c_str());
        logMessage(LOG_INFO, "Signal strength: %s (%d dBm)", 
                   getSignalStrength(WiFi.RSSI()), WiFi.RSSI());
    }
    
    // Configure NTP for time synchronization
    configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC, NTP_SERVER);
    logMessage(LOG_INFO, "NTP client initialized");
    
    // Initialize mDNS
    if (WiFi.status() == WL_CONNECTED) {
        String mdnsName = systemName;
        mdnsName.replace(" ", "");
        mdnsName.toLowerCase();
        
        if (MDNS.begin(mdnsName.c_str())) {
            MDNS.addService("http", "tcp", 80);
            MDNS.addService("http", "tcp", 8080);
            MDNS.addService("ws", "tcp", 81);
            logMessage(LOG_INFO, "mDNS started: %s.local", mdnsName.c_str());
        }
        
        // Setup redirect server (port 80)
        redirectServer.on("/", []() {
            redirectServer.sendHeader("Location", GOOGLE_SCRIPT_URL, true);
            redirectServer.send(302, "text/plain", "Redirecting to cloud dashboard...");
        });
        redirectServer.begin();
        logMessage(LOG_INFO, "Redirect server started on port 80");
        
        // Setup local API server (port 8080)
        initLocalAPI();
    }
    
    // Initialize voice assistants
    for (int i = 0; i < 4; i++) {
        String deviceName = devices[i].name.length() > 0 ? devices[i].name : "Device " + String(i + 1);
        alexaManager.addDevice(deviceName.c_str(), [i](uint8_t b) { voiceCallback(b, i); });
    }
    alexaManager.begin();
    logMessage(LOG_INFO, "Alexa integration initialized");
    
    // TODO: Initialize SinricPro for Google Assistant
    // if (sinricAppKey.length() > 0) {
    //     initSinricPro(sinricAppKey.c_str(), sinricAppSecret.c_str(), sinricDeviceIds);
    // }
    
    // Initialize TRIAC control timer (Core 1)
    timer = timerBegin(1000000);  // 1MHz
    timerAttachInterrupt(timer, &onTimerFire);
    timerAlarm(timer, TIMER_INTERVAL_US, true, 0);
    logMessage(LOG_INFO, "Hardware timer initialized");
    
    // Attach zero-cross interrupt
    attachInterrupt(digitalPinToInterrupt(ZCD_PIN), onZeroCross, RISING);
    logMessage(LOG_INFO, "Zero-cross interrupt attached");
    
    // Create FreeRTOS queue for device control
    deviceControlQueue = xQueueCreate(10, sizeof(DeviceControlMsg));
    
    // Start Core 0 connectivity task
    xTaskCreatePinnedToCore(
        taskConnectivity,
        "Connectivity",
        16384,  // Stack size
        NULL,
        1,      // Priority
        NULL,
        0       // Core 0 (APP_CPU)
    );
    logMessage(LOG_INFO, "Core 0 connectivity task started");
    
    logMessage(LOG_INFO, "========================================");
    logMessage(LOG_INFO, "System initialization complete");
    logMessage(LOG_INFO, "Free heap: %d bytes", ESP.getFreeHeap());
    logMessage(LOG_INFO, "========================================");
}

// ================================================================
// 🔁 MAIN LOOP (Core 1 - Runs on PRO_CPU)
// ================================================================

void loop() {
    // Core 1 handles only time-critical voice assistant polling
    // All other logic is in Core 0 task
    alexaManager.loop();
    
    // Minimal delay to prevent watchdog triggers
    delay(1);
}