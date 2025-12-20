/**
 * Device Management Module
 * Handles device state, control, and fade transitions
 * Professional modular architecture for ESP32 smart home controller
 */

#ifndef DEVICE_H
#define DEVICE_H

#include "config.h"
#include <Arduino.h>

// ================================================================
// EXTERNAL DEPENDENCIES
// ================================================================
extern portMUX_TYPE timerMux;

// ================================================================
// DEVICE STATE MANAGEMENT
// ================================================================

/**
 * Set device state with optional fade transition
 * Thread-safe for multi-core operation
 * 
 * @param deviceId Device index (0-3)
 * @param state Target state (true=ON, false=OFF)
 * @param brightness Target brightness (0-100)
 * @param fade Enable smooth fade transition
 */
void setDeviceState(int deviceId, bool state, int brightness, bool fade = false);

/**
 * Get device state atomically
 * Thread-safe read of device state
 * 
 * @param deviceId Device index (0-3)
 * @param state Output: current state
 * @param brightness Output: current brightness
 * @return true if successful, false if invalid deviceId
 */
bool getDeviceState(int deviceId, bool &state, int &brightness);

/**
 * Process fade transitions for all devices
 * Call regularly from main loop (Core 0)
 */
void processFadeTransitions();

/**
 * Update runtime statistics for all active devices
 * Call periodically to track usage time
 */
void updateRuntimeStatistics();

/**
 * Check and enforce auto-off timers
 * Call periodically from main loop
 */
void checkAutoOff();

// ================================================================
// UTILITY FUNCTIONS
// ================================================================

/**
 * Calculate TRIAC fire tick from brightness percentage
 * 
 * @param percent Brightness (0-100)
 * @return Fire tick delay (0-100, where 0=full brightness)
 */
int calculateFireTick(int percent);

/**
 * Validate device ID
 * 
 * @param deviceId Device index to validate
 * @return true if valid (0-3), false otherwise
 */
inline bool isValidDeviceId(int deviceId) {
    return (deviceId >= 0 && deviceId < 4);
}

#endif // DEVICE_H
