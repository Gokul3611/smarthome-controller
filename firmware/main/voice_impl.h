/**
 * Voice Assistant Implementation
 * Complete implementation for Alexa (Espalexa) and Google Assistant (SinricPro)
 */

#ifndef VOICE_IMPL_H
#define VOICE_IMPL_H

#include "voice.h"
#include "config.h"
#include <Espalexa.h>

// SinricPro includes (will be enabled when library is installed)
// #include <SinricPro.h>
// #include <SinricProSwitch.h>

// External references
extern Espalexa alexaManager;
extern Device devices[4];
extern String sinricAppKey;
extern String sinricAppSecret;
extern String sinricDeviceIds[4];

// SinricPro device ID mapping (populated when devices are registered)
// String deviceIdToSinricId[4];

// ================================================================
// ALEXA INTEGRATION IMPLEMENTATION
// ================================================================

void alexaCallback(uint8_t brightness, int deviceId) {
    if (deviceId < 0 || deviceId >= 4) return;
    
    bool state = (brightness > 0);
    int brightnessPercent;
    
    if (devices[deviceId].type == TYPE_SWITCH) {
        brightnessPercent = 100;
    } else {
        brightnessPercent = map(brightness, 0, 255, 0, 100);
    }
    
    setDeviceState(deviceId, state, brightnessPercent, true);
    
    logMessage(LOG_INFO, "Alexa command: Device %d -> %s at %d%%", 
               deviceId, state ? "ON" : "OFF", brightnessPercent);
}

void registerAlexaDevice(int deviceId, const char* name) {
    if (deviceId < 0 || deviceId >= 4) return;
    
    // Device is already registered in setup()
    // This function is here for API completeness
    logMessage(LOG_DEBUG, "Alexa device registered: %s (ID: %d)", name, deviceId);
}

// ================================================================
// GOOGLE ASSISTANT / SINRICPRO INTEGRATION
// ================================================================

// Note: SinricPro library must be installed separately
// Uncomment the following code when SinricPro library is available

/*
bool onPowerState(const String &deviceId, bool &state) {
    int devIdx = getDeviceIdFromSinricId(deviceId);
    if (devIdx < 0) return false;
    
    setDeviceState(devIdx, state, devices[devIdx].brightness, true);
    
    logMessage(LOG_INFO, "Google Assistant: Device %d -> %s", 
               devIdx, state ? "ON" : "OFF");
    
    return true;
}

bool onBrightness(const String &deviceId, int &brightness) {
    int devIdx = getDeviceIdFromSinricId(deviceId);
    if (devIdx < 0) return false;
    
    setDeviceState(devIdx, true, brightness, true);
    
    logMessage(LOG_INFO, "Google Assistant: Device %d brightness -> %d%%", 
               devIdx, brightness);
    
    return true;
}

bool onPowerLevel(const String &deviceId, int &powerLevel) {
    int devIdx = getDeviceIdFromSinricId(deviceId);
    if (devIdx < 0) return false;
    
    // Power level is already 0-100, use directly
    setDeviceState(devIdx, true, powerLevel, true);
    
    logMessage(LOG_INFO, "Google Assistant: Device %d power level -> %d%%", 
               devIdx, powerLevel);
    
    return true;
}
*/

bool initSinricPro(const char* appKey, const char* appSecret, const char* deviceIds[4]) {
    // SinricPro initialization
    // Uncomment when SinricPro library is installed
    
    /*
    SinricPro.onConnected([](){
        logMessage(LOG_INFO, "SinricPro connected!");
    });
    
    SinricPro.onDisconnected([](){
        logMessage(LOG_WARN, "SinricPro disconnected!");
    });
    
    for (int i = 0; i < 4; i++) {
        if (strlen(deviceIds[i]) > 0) {
            deviceIdToSinricId[i] = String(deviceIds[i]);
            
            SinricProSwitch& mySwitch = SinricPro[deviceIds[i]];
            mySwitch.onPowerState(onPowerState);
            
            if (devices[i].type != TYPE_SWITCH) {
                mySwitch.onBrightness(onBrightness);
                mySwitch.onPowerLevel(onPowerLevel);
            }
            
            logMessage(LOG_INFO, "SinricPro device registered: %s", deviceIds[i]);
        }
    }
    
    SinricPro.begin(appKey, appSecret);
    return true;
    */
    
    logMessage(LOG_WARN, "SinricPro not available - Google Assistant disabled");
    logMessage(LOG_INFO, "To enable: Install SinricPro library and uncomment code in voice_impl.h");
    return false;
}

void updateSinricProState(int deviceId, bool state, int brightness) {
    if (deviceId < 0 || deviceId >= 4) return;
    
    // Update SinricPro with local state changes
    // Uncomment when SinricPro library is installed
    
    /*
    if (deviceIdToSinricId[deviceId].length() > 0) {
        SinricProSwitch& mySwitch = SinricPro[deviceIdToSinricId[deviceId]];
        mySwitch.sendPowerStateEvent(state);
        
        if (devices[deviceId].type != TYPE_SWITCH) {
            mySwitch.sendBrightnessEvent(brightness);
        }
    }
    */
}

// ================================================================
// GEMINI AI INTEGRATION (Future)
// ================================================================

void initGeminiAI() {
    // Reserved for future Gemini AI integration
    logMessage(LOG_INFO, "Gemini AI integration: Coming soon");
}

bool processGeminiCommand(const char* command) {
    // Process natural language commands via Gemini AI
    logMessage(LOG_DEBUG, "Gemini command: %s", command);
    
    // TODO: Implement Gemini API integration
    // - Send command to Gemini API
    // - Parse response for device control actions
    // - Execute actions on devices
    
    return false;
}

// ================================================================
// UTILITY FUNCTIONS
// ================================================================

uint8_t brightness100To255(int brightness) {
    if (brightness <= 0) return 0;
    if (brightness >= 100) return 255;
    return (uint8_t)map(brightness, 0, 100, 0, 255);
}

int brightness255To100(uint8_t brightness) {
    if (brightness == 0) return 0;
    if (brightness >= 255) return 100;
    return map(brightness, 0, 255, 0, 100);
}

int getDeviceIdFromSinricId(const String &sinricId) {
    /*
    for (int i = 0; i < 4; i++) {
        if (deviceIdToSinricId[i] == sinricId) {
            return i;
        }
    }
    */
    return -1;
}

bool isVoiceAssistantsConnected() {
    bool alexaReady = true;  // Alexa is always ready via mDNS
    bool sinricReady = false;
    
    // Check SinricPro connection
    // Uncomment when SinricPro library is installed
    // sinricReady = SinricPro.isConnected();
    
    return alexaReady || sinricReady;
}

// ================================================================
// INITIALIZATION
// ================================================================

void initVoiceAssistants() {
    logMessage(LOG_INFO, "Initializing voice assistants...");
    
    // Alexa is initialized in main setup() via Espalexa
    logMessage(LOG_INFO, "Amazon Alexa: Ready (via Espalexa)");
    
    // Initialize SinricPro if credentials are available
    if (sinricAppKey.length() > 0 && sinricAppSecret.length() > 0) {
        if (initSinricPro(sinricAppKey.c_str(), sinricAppSecret.c_str(), 
                          (const char**)sinricDeviceIds)) {
            logMessage(LOG_INFO, "Google Assistant: Ready (via SinricPro)");
        }
    } else {
        logMessage(LOG_INFO, "Google Assistant: Disabled (no credentials)");
        logMessage(LOG_INFO, "Set credentials in Google Apps Script to enable");
    }
    
    // Initialize Gemini AI (future)
    // initGeminiAI();
}

void handleVoiceAssistants() {
    // Handle SinricPro events
    // Uncomment when SinricPro library is installed
    // SinricPro.handle();
}

#endif // VOICE_IMPL_H
