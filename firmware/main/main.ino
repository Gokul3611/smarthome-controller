/**
 * 🏆 MASTER FIRMWARE: 4-Channel Universal Node (Production v2.1)
 * ---------------------------------------------------------------
 * FIXED: Library Include Order & File Conflict Resolution
 */

#include <WiFi.h>
#include <WebServer.h>       // <--- Moved UP to force library loading
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>     // Ensure Version 6.x is installed
#include <Espalexa.h>
#include <Preferences.h>
#include <WiFiManager.h>     // Must be after WebServer.h
#include <ESPmDNS.h>
#include <esp_task_wdt.h>
#include <time.h> 

// ================================================================
// 📝 USER CONFIGURATION
// ================================================================
// 👇👇 PASTE YOUR GOOGLE SCRIPT URL HERE 👇👇
String GOOGLE_SCRIPT_URL = "https://script.google.com/macros/s/AKfycbxJP31g9LhRulRHTbTd6KidEiBXlxCKfKcXkiUGe961IfNZDgHuoWAIif91PrPUQnHrIQ/exec"; 

String systemName = "Smart_Home_Setup"; 
#define AUTO_OFF_MS 3600000         // 1 Hour Safety Timer
#define WDT_TIMEOUT 15              // 15 Seconds Watchdog
const float FIRMWARE_VERSION = 1.0; 

// NTP Settings
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 19800; 
const int   daylightOffset_sec = 0;

// ================================================================
// 🔌 HARDWARE PIN MAP
// ================================================================
#define ZCD_PIN 13
const int TRIAC_PINS[4] = {16, 17, 18, 19};
const int SWITCH_PINS[4] = {32, 33, 25, 26};

// ================================================================
// ⚙ SYSTEM DATA
// ================================================================
enum DeviceType { TYPE_SWITCH = 0, TYPE_FAN = 1, TYPE_DIMMER = 2 };

struct Device {
    String name;          
    volatile DeviceType type; 
    int brightness;       
    volatile bool state;  
    volatile int fireTick;
    bool childLock;       
    bool autoOffEnabled;  
    unsigned long lastOnTime; 
};

struct Schedule {
    int16_t startMins; int16_t endMins; int8_t deviceId;
    int8_t startVal; int8_t endVal; bool active;
};

Device ports[4]; 
Schedule schedules[5];
bool lastSwitchState[4]; 

// ================================================================
// 🤖 OBJECTS
// ================================================================
Espalexa alexa;
Preferences preferences; 
WebServer redirectServer(80); 
hw_timer_t *timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

// ================================================================
// ⚡ CORE 1: POWER CONTROL
// ================================================================
void IRAM_ATTR onTimerFire() {
    portENTER_CRITICAL_ISR(&timerMux);
    static int tickCounter = 0; 
    tickCounter++;
    for (int i = 0; i < 4; i++) {
        if (ports[i].state && ports[i].type != TYPE_SWITCH) {
            if (tickCounter >= ports[i].fireTick) digitalWrite(TRIAC_PINS[i], HIGH); 
        }
    }
    if (tickCounter > 100) tickCounter = 0; 
    portEXIT_CRITICAL_ISR(&timerMux);
}

void IRAM_ATTR onZeroCross() {
    portENTER_CRITICAL_ISR(&timerMux);
    for (int i = 0; i < 4; i++) {
        if (ports[i].state) {
            if (ports[i].type == TYPE_SWITCH) digitalWrite(TRIAC_PINS[i], HIGH); 
            else digitalWrite(TRIAC_PINS[i], LOW);  
        } else {
            digitalWrite(TRIAC_PINS[i], LOW);
        }
    }
    portEXIT_CRITICAL_ISR(&timerMux);
}

// ================================================================
// 🧠 HELPERS
// ================================================================
int calculateFireTick(int percent) {
    if (percent >= 100) return 0;
    if (percent <= 0) return 100;
    return 100 - percent; 
}

void saveConfig() {
    preferences.begin("home-node", false);
    preferences.putString("sys_name", systemName);
    for(int i=0; i<4; i++) {
        preferences.putInt(("type" + String(i)).c_str(), (int)ports[i].type);
        preferences.putString(("name" + String(i)).c_str(), ports[i].name);
        preferences.putBool(("lock" + String(i)).c_str(), ports[i].childLock);
    }
    preferences.end();
}

void loadConfig() {
    preferences.begin("home-node", true);
    systemName = preferences.getString("sys_name", "Smart_Home_Setup");
    for(int i=0; i<4; i++) {
        ports[i].type = (DeviceType)preferences.getInt(("type" + String(i)).c_str(), 0);
        ports[i].name = preferences.getString(("name" + String(i)).c_str(), "Device " + String(i+1));
        ports[i].childLock = preferences.getBool(("lock" + String(i)).c_str(), false);
        ports[i].state = false;
        ports[i].brightness = 100;
        ports[i].fireTick = calculateFireTick(100);
        ports[i].autoOffEnabled = true; 
    }
    preferences.end();
}

void performOTA(String binUrl) {
    Serial.println("[OTA] Starting...");
    timerStop(timer); 
    detachInterrupt(digitalPinToInterrupt(ZCD_PIN));
    for(int i=0; i<4; i++) digitalWrite(TRIAC_PINS[i], LOW);

    WiFiClientSecure client;
    client.setInsecure(); 
    client.setTimeout(15000); 
    t_httpUpdate_return ret = httpUpdate.update(client, binUrl);

    switch (ret) {
      case HTTP_UPDATE_FAILED:
        timerStart(timer); 
        attachInterrupt(digitalPinToInterrupt(ZCD_PIN), onZeroCross, RISING);
        break;
      case HTTP_UPDATE_OK: ESP.restart(); break;
    }
}

void voiceCallback(uint8_t brightness, int deviceId) {
    if (brightness > 0) {
        ports[deviceId].state = true;
        ports[deviceId].lastOnTime = millis();
        if(ports[deviceId].type == TYPE_SWITCH) ports[deviceId].brightness = 100;
        else ports[deviceId].brightness = map(brightness, 0, 255, 0, 100);
    } else {
        ports[deviceId].state = false;
        ports[deviceId].brightness = 0;
    }
    ports[deviceId].fireTick = calculateFireTick(ports[deviceId].brightness);
}

void checkFactoryReset(bool currentState) {
    static unsigned long pressStartTime = 0;
    static bool isPressed = false;
    if (currentState == LOW) {
        if (!isPressed) { isPressed = true; pressStartTime = millis(); }
        if (millis() - pressStartTime > 10000) { 
            WiFiManager wm; wm.resetSettings(); delay(100); ESP.restart();      
        }
    } else isPressed = false;
}

// ================================================================
// 🌐 CORE 0 TASK
// ================================================================
void taskConnectivity(void * parameter) {
    HTTPClient http;
    unsigned long lastSyncTime = 0;
    
    esp_task_wdt_config_t wdt_config = { .timeout_ms = WDT_TIMEOUT * 1000, .idle_core_mask = (1 << portNUM_PROCESSORS) - 1, .trigger_panic = true };
    esp_task_wdt_init(&wdt_config);
    esp_task_wdt_add(NULL); 

    while(true) {
        esp_task_wdt_reset(); 
        redirectServer.handleClient();

        for(int i=0; i<4; i++) {
            bool currentRead = digitalRead(SWITCH_PINS[i]);
            if (i == 0) checkFactoryReset(currentRead);
            if (currentRead != lastSwitchState[i]) {
                delay(50); 
                if (digitalRead(SWITCH_PINS[i]) == currentRead) {
                    lastSwitchState[i] = currentRead;
                    if (!ports[i].childLock) {
                        ports[i].state = !ports[i].state;
                        if(ports[i].state) ports[i].lastOnTime = millis();
                        ports[i].fireTick = calculateFireTick(ports[i].brightness);
                    }
                }
            }
        }

        unsigned long now = millis();
        for(int i=0; i<4; i++) {
            if(ports[i].state && ports[i].autoOffEnabled && AUTO_OFF_MS > 0) {
                if (now - ports[i].lastOnTime > AUTO_OFF_MS) {
                    ports[i].state = false; ports[i].fireTick = calculateFireTick(0);
                }
            }
        }

        if (millis() - lastSyncTime > 2500) { 
            if(WiFi.status() == WL_CONNECTED) {
                StaticJsonDocument<1024> doc;
                doc["action"] = "poll"; doc["ver"] = FIRMWARE_VERSION; doc["uid"] = WiFi.macAddress();
                for(int i=0; i<4; i++) {
                    String key = "d" + String(i+1);
                    doc[key]["s"] = ports[i].state ? 1 : 0;
                    doc[key]["v"] = ports[i].brightness;
                }
                String jsonPayload; serializeJson(doc, jsonPayload);

                http.begin(GOOGLE_SCRIPT_URL);
                http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
                int httpCode = http.POST(jsonPayload);
                
                if(httpCode == 200) {
                    String response = http.getString();
                    StaticJsonDocument<1024> respDoc;
                    deserializeJson(respDoc, response);
                    
                    if (respDoc.containsKey("sys_name")) {
                        String newName = respDoc["sys_name"];
                        if (newName != systemName && newName.length() > 0) {
                            systemName = newName; saveConfig(); ESP.restart();
                        }
                    }
                    if (respDoc.containsKey("ota_update") && respDoc["ota_update"] == true) {
                          performOTA(respDoc["bin_url"]); 
                    }
                    for(int i=0; i<4; i++) {
                         String key = "d" + String(i+1);
                         if(respDoc.containsKey(key)) {
                             JsonObject d = respDoc[key];
                             String typeStr = d["type"];
                             DeviceType newType = ports[i].type;
                             if(typeStr == "FAN") newType = TYPE_FAN;
                             else if(typeStr == "SWITCH") newType = TYPE_SWITCH;
                             else if(typeStr == "DIMMER") newType = TYPE_DIMMER;
                             
                             if(newType != ports[i].type) { ports[i].type = newType; saveConfig(); }
                             if(d.containsKey("lock")) {
                                 bool remoteLock = d["lock"];
                                 if(remoteLock != ports[i].childLock) { ports[i].childLock = remoteLock; saveConfig(); }
                             }
                             if(d["update"] == true) { 
                                 ports[i].state = (d["state"] == 1);
                                 ports[i].brightness = d["val"];
                                 if(ports[i].state) ports[i].lastOnTime = millis();
                                 ports[i].fireTick = calculateFireTick(ports[i].brightness);
                             }
                          }
                    }
                }
                http.end();
            }
            lastSyncTime = millis();
        }
        vTaskDelay(10 / portTICK_PERIOD_MS); 
    }
}

// ================================================================
// 🚀 SETUP
// ================================================================
const char* custom_css = R"===(<meta name='viewport' content='width=device-width, initial-scale=1.0'><style>body{font-family:sans-serif;padding:20px;}button{width:100%;padding:15px;margin-top:10px;border-radius:10px;}</style>)===";

void setup() {
    Serial.begin(115200);
    
    pinMode(ZCD_PIN, INPUT_PULLUP);
    for(int i=0; i<4; i++) {
        pinMode(TRIAC_PINS[i], OUTPUT); digitalWrite(TRIAC_PINS[i], LOW);
        pinMode(SWITCH_PINS[i], INPUT_PULLUP); lastSwitchState[i] = digitalRead(SWITCH_PINS[i]);
    }
    loadConfig(); 

    WiFiManager wm;
    wm.setCustomHeadElement(custom_css);
    wm.setTitle("Setup " + systemName);
    std::vector<const char *> menu = {"wifi"}; wm.setMenu(menu); 
    
    if(!wm.autoConnect(systemName.c_str())) Serial.println("Offline Mode");
    else Serial.println("WiFi Connected");

    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

    if (WiFi.status() == WL_CONNECTED) {
        String mdnsName = systemName; mdnsName.replace(" ", ""); 
        if (MDNS.begin(mdnsName.c_str())) Serial.println("mDNS Active");
        
        redirectServer.on("/", []() {
            redirectServer.sendHeader("Location", GOOGLE_SCRIPT_URL, true);
            redirectServer.send(302, "text/plain", "Redirecting...");
        });
        redirectServer.begin();
    }

    alexa.addDevice("Port 1", [](uint8_t b){ voiceCallback(b, 0); });
    alexa.addDevice("Port 2", [](uint8_t b){ voiceCallback(b, 1); });
    alexa.addDevice("Port 3", [](uint8_t b){ voiceCallback(b, 2); });
    alexa.addDevice("Port 4", [](uint8_t b){ voiceCallback(b, 3); });
    alexa.begin();

    timer = timerBegin(1000000); 
    timerAttachInterrupt(timer, &onTimerFire);
    timerAlarm(timer, 100, true, 0); 
    attachInterrupt(digitalPinToInterrupt(ZCD_PIN), onZeroCross, RISING);

    xTaskCreatePinnedToCore(taskConnectivity, "Connectivity", 10000, NULL, 1, NULL, 0);
    Serial.println("System Ready.");
}

void loop() {
    alexa.loop(); 
}