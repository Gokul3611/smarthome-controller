#ifndef DEVICE_CONTROL_H
#define DEVICE_CONTROL_H

#include <Arduino.h>

enum DeviceType {
  DEVICE_LIGHT,
  DEVICE_FAN
};

struct DeviceState {
  DeviceType type;
  bool power;
  uint8_t speed; // 0–5
};

void device_control_init();
void device_control_set(uint8_t id, bool power, uint8_t speed = 0);
DeviceState device_control_get(uint8_t id);

#endif // DEVICE_CONTROL_H
