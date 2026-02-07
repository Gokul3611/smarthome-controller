/**
 * Configuration Constants for Smart Home Controller
 * Version: 3.0
 */

#ifndef CONFIG_H
#define CONFIG_H

// ================================================================
// FIRMWARE VERSION
// ================================================================
#define FIRMWARE_VERSION 3.0
#define FIRMWARE_BUILD "PROD-001"

// ================================================================
// HARDWARE CONFIGURATION
// ================================================================
// Zero-Cross Detection Pin
#define ZCD_PIN 13

// TRIAC Control Pins (4 channels)
const int TRIAC_PINS[4] = {16, 17, 18, 19};

// Physical Switch Pins (4 channels)
const int SWITCH_PINS[4] = {32, 33, 25, 26};

// Optional Temperature Sensor (set to -1 if not available)
#define TEMP_SENSOR_PIN -1

// LED indicator pin (for OTA updates)
#ifndef LED_BUILTIN
#define LED_BUILTIN 2  // ESP32 built-in LED on GPIO 2
#endif

// ================================================================
// TIMING CONFIGURATION
// ================================================================
// Phase angle control timing (microseconds)
#define TIMER_INTERVAL_US 100
#define PHASE_STEPS 100

// Debounce delay for physical switches (milliseconds)
#define SWITCH_DEBOUNCE_MS 50

// Safety auto-off timer (milliseconds, 0 = disabled)
#define AUTO_OFF_MS 3600000  // 1 hour

// Factory reset long-press duration (milliseconds)
#define FACTORY_RESET_DURATION_MS 10000  // 10 seconds

// ================================================================
// NETWORK CONFIGURATION
// ================================================================
// Cloud polling interval (milliseconds)
#define CLOUD_POLL_INTERVAL_MS 2500  // 2.5 seconds

// WiFi reconnection settings
#define WIFI_RECONNECT_INTERVAL_MS 5000
#define WIFI_RECONNECT_MAX_ATTEMPTS 5
#define WIFI_RECONNECT_BACKOFF_MULTIPLIER 2

// HTTP request timeout (milliseconds)
#define HTTP_TIMEOUT_MS 15000

// WebSocket ping interval (milliseconds)
#define WEBSOCKET_PING_INTERVAL_MS 30000

// ================================================================
// OTA UPDATE CONFIGURATION
// ================================================================
#define OTA_CHECK_INTERVAL_MS 3600000  // Check every hour
#define OTA_PARTITION_LABEL "app0"

// ================================================================
// WATCHDOG CONFIGURATION
// ================================================================
#define WDT_TIMEOUT_CORE0 15  // seconds
#define WDT_TIMEOUT_CORE1 5   // seconds (time-critical)

// ================================================================
// NTP CONFIGURATION
// ================================================================
const char* NTP_SERVER = "pool.ntp.org";
const long GMT_OFFSET_SEC = 19800;  // IST: UTC+5:30
const int DAYLIGHT_OFFSET_SEC = 0;

// ================================================================
// LOGGING CONFIGURATION
// ================================================================
enum LogLevel {
    LOG_NONE = 0,
    LOG_ERROR = 1,
    LOG_WARN = 2,
    LOG_INFO = 3,
    LOG_DEBUG = 4,
    LOG_VERBOSE = 5
};

#define LOG_LEVEL LOG_INFO  // Change to LOG_DEBUG for development

// ================================================================
// MEMORY CONFIGURATION
// ================================================================
#define JSON_BUFFER_SIZE 2048
#define SCHEDULE_MAX_COUNT 10
#define SCENE_MAX_COUNT 10

// ================================================================
// DEVICE TYPES
// ================================================================
enum DeviceType {
    TYPE_SWITCH = 0,
    TYPE_FAN = 1,
    TYPE_DIMMER = 2
};

// ================================================================
// DEVICE STATES
// ================================================================
enum PowerOnState {
    POWER_ON_OFF = 0,      // Always off
    POWER_ON_ON = 1,       // Always on
    POWER_ON_LAST = 2,     // Remember last state
    POWER_ON_DEFAULT = 3   // Use default brightness
};

// ================================================================
// TRANSITION SETTINGS
// ================================================================
#define FADE_DURATION_MS 1000  // Default fade duration
#define FADE_STEPS 20          // Number of steps in fade transition

// ================================================================
// PREFERENCES KEYS
// ================================================================
#define PREF_NAMESPACE "smarthome"
#define PREF_SYSTEM_NAME "sys_name"
#define PREF_DEVICE_PREFIX "dev_"
#define PREF_SCHEDULE_PREFIX "sched_"
#define PREF_SCENE_PREFIX "scene_"

// ================================================================
// SINRIC PRO CONFIGURATION (Google Assistant)
// ================================================================
// Users should set these in Google Apps Script or via web interface
#define SINRIC_APP_KEY ""     // Will be loaded from cloud
#define SINRIC_APP_SECRET ""  // Will be loaded from cloud
#define SINRIC_WEBSOCKET_URL "ws.sinric.pro"

// ================================================================
// SIGNAL STRENGTH THRESHOLDS
// ================================================================
#define RSSI_EXCELLENT -50
#define RSSI_GOOD -60
#define RSSI_FAIR -70
#define RSSI_POOR -80

// ================================================================
// ERROR CODES
// ================================================================
enum ErrorCode {
    ERR_NONE = 0,
    ERR_WIFI_CONNECT = 1,
    ERR_CLOUD_CONNECT = 2,
    ERR_ZERO_CROSS_LOST = 3,
    ERR_OVERTEMP = 4,
    ERR_BROWNOUT = 5,
    ERR_OTA_FAILED = 6,
    ERR_MEMORY_LOW = 7
};

#endif // CONFIG_H
