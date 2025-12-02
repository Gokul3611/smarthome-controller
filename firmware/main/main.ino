#include <Arduino.h>
#include "wifi_manager.h"

void setup() {
  Serial.begin(115200);
  delay(100);
  wifi_manager_init();
}

void loop() {
  wifi_manager_loop();
  // other firmware tasks will be added later
}
