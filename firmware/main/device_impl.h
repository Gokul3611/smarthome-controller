/**
 * Device Management Implementation
 * Thread-safe device state management for ESP32 multi-core operation
 */

#ifndef DEVICE_IMPL_H
#define DEVICE_IMPL_H

#include "device.h"

// External references
extern Device devices[4];
extern FadeState fadeStates[4];
extern portMUX_TYPE timerMux;
extern void broadcastDeviceState(int deviceId);
extern void logMessage(LogLevel level, const char* format, ...);

// ================================================================
// UTILITY FUNCTIONS
// ================================================================

int calculateFireTick(int percent) {
    if (percent >= 100) return 0;
    if (percent <= 0) return 100;
    return 100 - percent;
}

// ================================================================
// DEVICE STATE MANAGEMENT
// ================================================================

void setDeviceState(int deviceId, bool state, int brightness, bool fade) {
    if (!isValidDeviceId(deviceId)) return;
    
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
        // Immediate change - use critical section to protect ISR-accessed variables
        portENTER_CRITICAL(&timerMux);
        devices[deviceId].state = state;
        devices[deviceId].brightness = state ? brightness : 0;
        devices[deviceId].fireTick = calculateFireTick(devices[deviceId].brightness);
        portEXIT_CRITICAL(&timerMux);
        
        if (state) {
            devices[deviceId].lastOnTime = millis();
        }
    }
    
    // Broadcast to WebSocket clients
    broadcastDeviceState(deviceId);
}

bool getDeviceState(int deviceId, bool &state, int &brightness) {
    if (!isValidDeviceId(deviceId)) return false;
    
    // Atomic read of volatile variables
    portENTER_CRITICAL(&timerMux);
    state = devices[deviceId].state;
    brightness = devices[deviceId].brightness;
    portEXIT_CRITICAL(&timerMux);
    
    return true;
}

void processFadeTransitions() {
    for (int i = 0; i < 4; i++) {
        if (!fadeStates[i].active) continue;
        
        unsigned long now = millis();
        if (now - fadeStates[i].lastStepTime >= fadeStates[i].stepInterval) {
            fadeStates[i].currentStep++;
            fadeStates[i].lastStepTime = now;
            
            // Use critical section to protect ISR-accessed variables
            portENTER_CRITICAL(&timerMux);
            if (fadeStates[i].currentStep >= fadeStates[i].totalSteps) {
                // Fade complete
                devices[i].brightness = fadeStates[i].targetBrightness;
                devices[i].state = (devices[i].brightness > 0);
            } else {
                // Calculate intermediate brightness
                int startBrightness = devices[i].brightness;
                int range = fadeStates[i].targetBrightness - startBrightness;
                float progress = (float)fadeStates[i].currentStep / fadeStates[i].totalSteps;
                devices[i].brightness = startBrightness + (int)(range * progress);
            }
            
            devices[i].fireTick = calculateFireTick(devices[i].brightness);
            portEXIT_CRITICAL(&timerMux);
            
            // Update fade state after critical section (not accessed by ISR)
            if (fadeStates[i].currentStep >= fadeStates[i].totalSteps) {
                fadeStates[i].active = false;
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
    for (int i = 0; i < 4; i++) {
        if (devices[i].autoOffEnabled && devices[i].state) {
            if (millis() - devices[i].lastOnTime > AUTO_OFF_MS) {
                logMessage(LOG_INFO, "Auto-off triggered for device %d", i);
                setDeviceState(i, false, 0, true);
            }
        }
    }
}

#endif // DEVICE_IMPL_H
