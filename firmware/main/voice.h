/**
 * Voice Assistant Integration
 * Supports Amazon Alexa (via Espalexa) and Google Assistant (via SinricPro)
 */

#ifndef VOICE_H
#define VOICE_H

#include <Espalexa.h>

// SinricPro includes - Comment out if library not installed
// #include <SinricPro.h>
// #include <SinricProSwitch.h>

#include "config.h"

// ================================================================
// GLOBAL OBJECTS
// ================================================================
extern Espalexa alexaManager;

// ================================================================
// INITIALIZATION
// ================================================================

/**
 * Initialize voice assistant integrations
 * Sets up both Alexa and Google Assistant support
 */
void initVoiceAssistants();

/**
 * Update voice assistant loop
 * Must be called regularly from main loop or connectivity task
 */
void handleVoiceAssistants();

// ================================================================
// ALEXA INTEGRATION (via Espalexa)
// ================================================================

/**
 * Alexa callback for device control
 * @param brightness - 0-255 brightness value
 * @param deviceId - Device index (0-3)
 */
void alexaCallback(uint8_t brightness, int deviceId);

/**
 * Register device with Alexa
 * @param deviceId - Device index
 * @param name - Device name for voice control
 */
void registerAlexaDevice(int deviceId, const char* name);

// ================================================================
// GOOGLE ASSISTANT INTEGRATION (via SinricPro)
// ================================================================

/**
 * Initialize SinricPro for Google Assistant
 * @param appKey - SinricPro app key
 * @param appSecret - SinricPro app secret
 * @param deviceIds - Array of SinricPro device IDs
 */
bool initSinricPro(const char* appKey, const char* appSecret, const char* deviceIds[4]);

/**
 * SinricPro power state callback
 */
bool onPowerState(const String &deviceId, bool &state);

/**
 * SinricPro brightness callback (for dimmers and fans)
 */
bool onBrightness(const String &deviceId, int &brightness);

/**
 * SinricPro power level callback (for fans)
 */
bool onPowerLevel(const String &deviceId, int &powerLevel);

/**
 * Update device state in SinricPro
 * Call this when device state changes locally
 */
void updateSinricProState(int deviceId, bool state, int brightness);

// ================================================================
// GEMINI AI INTEGRATION (Future)
// ================================================================

/**
 * Initialize Gemini AI integration
 * Reserved for future smart automation features
 */
void initGeminiAI();

/**
 * Process natural language command via Gemini
 * @param command - Natural language command
 * @return - Success/failure
 */
bool processGeminiCommand(const char* command);

// ================================================================
// UTILITY FUNCTIONS
// ================================================================

/**
 * Convert brightness from 0-100 scale to 0-255 scale (Alexa)
 */
uint8_t brightness100To255(int brightness);

/**
 * Convert brightness from 0-255 scale to 0-100 scale
 */
int brightness255To100(uint8_t brightness);

/**
 * Get device ID from SinricPro device ID string
 */
int getDeviceIdFromSinricId(const String &sinricId);

/**
 * Check if voice assistants are connected
 */
bool isVoiceAssistantsConnected();

#endif // VOICE_H
