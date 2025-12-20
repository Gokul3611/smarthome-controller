/**
 * Configuration file for Mini LED Controller
 * Edit these settings before uploading to your ESP32
 */

#ifndef MINI_CONFIG_H
#define MINI_CONFIG_H

// ================================================================
// DEVICE CONFIGURATION
// ================================================================
// Device name (shown in WiFi setup and dashboard)
#define DEVICE_NAME "Mini_LED_Controller"

// Firmware version
#define FIRMWARE_VERSION "1.0"

// ================================================================
// HARDWARE CONFIGURATION
// ================================================================
// Built-in LED pin
// Common values:
// - GPIO 2: Most ESP32 DevKit boards
// - GPIO 5: Some ESP32 boards
// - GPIO 16: NodeMCU-32S
// - GPIO 22: DOIT ESP32 DevKit
#define LED_PIN 2

// ================================================================
// PWM CONFIGURATION
// ================================================================
// PWM channel (0-15 available on ESP32)
#define PWM_CHANNEL 0

// PWM frequency in Hz (1-40000)
// 5000 Hz is good for LEDs
#define PWM_FREQUENCY 5000

// PWM resolution in bits (1-16)
// 8-bit = 256 levels (0-255)
// 10-bit = 1024 levels (0-1023)
#define PWM_RESOLUTION 8

// ================================================================
// NETWORK CONFIGURATION
// ================================================================
// Local API server port
#define LOCAL_SERVER_PORT 8080

// WebSocket server port
#define WEBSOCKET_PORT 81

// Cloud polling interval in milliseconds
// 5000 = 5 seconds
#define CLOUD_POLL_INTERVAL_MS 5000

// HTTP request timeout in milliseconds
#define HTTP_TIMEOUT_MS 10000

// Minimum URL length for validation
#define MIN_URL_LENGTH 10

// WiFi configuration portal timeout in seconds
// 180 = 3 minutes
#define WIFI_CONFIG_TIMEOUT 180

// ================================================================
// CLOUD INTEGRATION
// ================================================================
// Google Apps Script URL for cloud integration
// Leave empty ("") to disable cloud features
// Example: "https://script.google.com/macros/s/YOUR_SCRIPT_ID/exec"
// IMPORTANT: Replace with your own script URL before deployment
#define GOOGLE_SCRIPT_URL ""

// ================================================================
// MDNS CONFIGURATION
// ================================================================
// mDNS hostname (access device via http://hostname.local)
#define MDNS_HOSTNAME "mini-led"

// ================================================================
// DEFAULT LED STATE
// ================================================================
// Default LED state on power-up
#define DEFAULT_LED_STATE false  // false = OFF, true = ON

// Default brightness (0-100%)
#define DEFAULT_BRIGHTNESS 100

// Default LED name
#define DEFAULT_LED_NAME "Built-in LED"

// ================================================================
// DEBUG CONFIGURATION
// ================================================================
// Enable verbose serial output
// true = detailed logs, false = minimal logs
#define DEBUG_MODE false

// Serial baud rate
#define SERIAL_BAUD 115200

#endif // MINI_CONFIG_H
