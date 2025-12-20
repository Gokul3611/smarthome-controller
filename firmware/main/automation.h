/**
 * Automation Module
 * Schedules and Scenes management for smart home automation
 */

#ifndef AUTOMATION_H
#define AUTOMATION_H

#include "config.h"
#include <time.h>

// ================================================================
// UTILITY FUNCTIONS
// ================================================================

/**
 * Convert time to minutes since midnight
 * 
 * @param hour Hour (0-23)
 * @param minute Minute (0-59)
 * @return Minutes since midnight
 */
int timeToMinutes(int hour, int minute);

/**
 * Get current time in minutes since midnight
 * 
 * @return Minutes since midnight, or -1 if time not available
 */
int getCurrentMinutes();

// ================================================================
// SCENES
// ================================================================

/**
 * Activate a scene
 * Applies the device states defined in the scene
 * 
 * @param sceneId Scene index (0-9)
 */
void activateScene(int sceneId);

// ================================================================
// SCHEDULES
// ================================================================

/**
 * Process schedules
 * Check and execute active schedules based on current time
 * Call periodically (e.g., every minute)
 */
void processSchedules();

#endif // AUTOMATION_H
