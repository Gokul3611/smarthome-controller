/**
 * Automation Implementation
 * Schedules and Scenes automation logic
 */

#ifndef AUTOMATION_IMPL_H
#define AUTOMATION_IMPL_H

#include "automation.h"

// External references
extern Device devices[4];
extern Schedule schedules[SCHEDULE_MAX_COUNT];
extern Scene scenes[SCENE_MAX_COUNT];
extern void setDeviceState(int deviceId, bool state, int brightness, bool fade);
extern void logMessage(LogLevel level, const char* format, ...);

// ================================================================
// UTILITY FUNCTIONS
// ================================================================

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
// SCENES
// ================================================================

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

// ================================================================
// SCHEDULES
// ================================================================

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

#endif // AUTOMATION_IMPL_H
