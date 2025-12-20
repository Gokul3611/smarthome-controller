/**
 * ISR Implementation
 * Time-critical interrupt handlers for TRIAC control
 */

#ifndef ISR_IMPL_H
#define ISR_IMPL_H

#include "isr.h"

// External references
extern Device devices[4];
extern const int TRIAC_PINS[4];
extern portMUX_TYPE timerMux;
extern volatile bool zeroCrossDetected;
extern volatile unsigned long lastZeroCrossTime;
extern int currentError;
extern void logMessage(LogLevel level, const char* format, ...);

// ================================================================
// ISR HANDLERS
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
            
            // Safety: Turn off all TRIACs - hardware first, then state
            // Order matters: Set pins LOW before clearing state to ensure
            // ISR doesn't re-trigger TRIACs during the transition
            for (int i = 0; i < 4; i++) {
                digitalWrite(TRIAC_PINS[i], LOW);
            }
            
            // Update state with critical section
            portENTER_CRITICAL(&timerMux);
            for (int i = 0; i < 4; i++) {
                devices[i].state = false;
            }
            portEXIT_CRITICAL(&timerMux);
        }
    }
}

#endif // ISR_IMPL_H
