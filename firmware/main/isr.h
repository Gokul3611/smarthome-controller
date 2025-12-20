/**
 * Interrupt Service Routine (ISR) Module
 * Hardware timer and zero-cross detection for TRIAC control
 * Time-critical code running on ESP32 Core 1
 */

#ifndef ISR_H
#define ISR_H

#include "config.h"
#include <Arduino.h>

// ================================================================
// ISR HANDLERS
// ================================================================

/**
 * Hardware timer ISR for phase angle control
 * Controls TRIAC firing at precise phase angles
 * IRAM_ATTR: Ensures code is in IRAM for fast execution
 */
void IRAM_ATTR onTimerFire();

/**
 * Zero-cross detection ISR
 * Resets TRIACs at AC zero crossing point
 * IRAM_ATTR: Ensures code is in IRAM for fast execution
 */
void IRAM_ATTR onZeroCross();

/**
 * Check zero-cross signal health
 * Call from Core 0 task to monitor signal integrity
 * Implements safety shutdown if signal lost
 */
void checkZeroCrossHealth();

#endif // ISR_H
