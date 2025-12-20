/**
 * Storage and Persistence Module
 * Handles device configuration, schedules, and scenes storage
 * Uses ESP32 Preferences (NVS) for non-volatile storage
 */

#ifndef STORAGE_H
#define STORAGE_H

#include "config.h"
#include <Preferences.h>

// ================================================================
// EXTERNAL DEPENDENCIES
// ================================================================
extern Preferences preferences;

// ================================================================
// DEVICE CONFIGURATION
// ================================================================

/**
 * Save device configuration to flash
 * Saves device types, names, settings, and last state
 */
void saveDeviceConfig();

/**
 * Load device configuration from flash
 * Restores device types, names, and applies power-on behavior
 */
void loadDeviceConfig();

// ================================================================
// SCHEDULES
// ================================================================

/**
 * Save schedules to flash
 * Persists all schedule configurations
 */
void saveSchedules();

/**
 * Load schedules from flash
 * Restores schedule configurations
 */
void loadSchedules();

// ================================================================
// SCENES
// ================================================================

/**
 * Save scenes to flash
 * Persists all scene configurations
 */
void saveScenes();

/**
 * Load scenes from flash
 * Restores scene configurations
 */
void loadScenes();

#endif // STORAGE_H
