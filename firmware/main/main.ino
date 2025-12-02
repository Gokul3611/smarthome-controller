#include <Arduino.h>
#include "wifi_manager.h"
#include "device_control.h"
#include "webserver.h"

// We need to know if we are in AP mode or Station mode
extern bool runningAP; // Access the variable from wifi_manager.cpp

void setup() {
  Serial.begin(115200);
  delay(100);

  // 1. Initialize Hardware (Relays/LEDs)
  device_control_init();

  // 2. Initialize Wi-Fi
  // If no saved Wifi, this starts AP mode and the Provisioning WebServer
  wifi_manager_init(); 
  
  // 3. Only start the Control WebServer if we are NOT in provisioning mode
  // We need to modify wifi_manager.h to expose the 'runningAP' state or check WiFi status
  if (WiFi.status() == WL_CONNECTED) {
     webserver_init();
     Serial.println("System Ready: Control API Running");
  } else {
     Serial.println("System in Provisioning Mode (AP)");
  }
}

void loop() {
  wifi_manager_loop(); // Handles DNS/AP Webserver if needed
  
  if (WiFi.status() == WL_CONNECTED) {
    webserver_loop(); // Handles Control API if connected
  }
}