#include "device_control.h"

// --- CONFIGURATION: Define your ESP32 GPIO Pins here ---
// Example: Relay 1 (Pin 23), Relay 2 (Pin 22), Relay 3 (Pin 21), Fan (Pin 19)
const int DEVICE_PINS[4] = { 23, 22, 21, 19 }; 

static DeviceState devices[4];

void device_control_init() {
  for (int i = 0; i < 4; i++) {
    // 1. Initialize the struct
    devices[i].type = (i == 3) ? DEVICE_FAN : DEVICE_LIGHT; // Assume last one is Fan
    devices[i].power = false;
    devices[i].speed = 0;

    // 2. Initialize the Hardware
    pinMode(DEVICE_PINS[i], OUTPUT);
    digitalWrite(DEVICE_PINS[i], LOW); // Start turned off
  }
}

void device_control_set(uint8_t id, bool power, uint8_t speed) {
  if (id >= 4) return;

  // Update State
  devices[id].power = power;

  // Update Hardware
  if (power) {
    digitalWrite(DEVICE_PINS[id], HIGH); // Turn Relay ON
  } else {
    digitalWrite(DEVICE_PINS[id], LOW);  // Turn Relay OFF
  }

  // Handle Fan Speed (PWM) logic here if needed later
  if (devices[id].type == DEVICE_FAN) {
    devices[id].speed = power ? speed : 0;
    // Note: You will need 'ledcWrite' here if you want real speed control
  }
}

DeviceState device_control_get(uint8_t id) {
  if (id >= 4) {
    DeviceState dummy = { DEVICE_LIGHT, false, 0 };
    return dummy;
  }
  return devices[id];
}