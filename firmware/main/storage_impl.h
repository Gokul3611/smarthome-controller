/**
 * Storage and Persistence Implementation
 * Manages configuration persistence using ESP32 NVS (Preferences)
 */

#ifndef STORAGE_IMPL_H
#define STORAGE_IMPL_H

#include "storage.h"

// External references
extern Device devices[4];
extern Schedule schedules[SCHEDULE_MAX_COUNT];
extern Scene scenes[SCENE_MAX_COUNT];
extern String systemName;
extern void logMessage(LogLevel level, const char* format, ...);
extern int calculateFireTick(int percent);

// ================================================================
// DEVICE CONFIGURATION
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

// ================================================================
// SCHEDULES
// ================================================================

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

// ================================================================
// SCENES
// ================================================================

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

#endif // STORAGE_IMPL_H
