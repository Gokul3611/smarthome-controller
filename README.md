"# Smart Home Controller - Production Firmware v3.0

![Version](https://img.shields.io/badge/version-3.0-blue)
![Platform](https://img.shields.io/badge/platform-ESP32-green)
![License](https://img.shields.io/badge/license-MIT-orange)

## Overview

**Complete IoT smart home solution** with ESP32 hardware, cloud backend, web dashboard, and mobile app.

Production-ready firmware for ESP32-based 4-channel TRIAC smart home controller with cloud integration, voice assistant support, and local control capabilities.

### 🚀 Quick Start

**New to the project?** See [DEPLOYMENT.md](./DEPLOYMENT.md) for a 30-minute setup guide.

**Want to see all features?** Check [FEATURES.md](./FEATURES.md) for the complete list of 100+ features.

### Key Features

- ⚡ **Multi-core Architecture**: Time-critical TRIAC control on Core 1, network operations on Core 0
- 🎙️ **Voice Assistants**: Amazon Alexa + Google Assistant (via SinricPro) + future Gemini AI
- ☁️ **Cloud Integration**: Google Apps Script backend for remote control and configuration
- 🌐 **Web Dashboard**: Full-featured control panel accessible from anywhere
- 📱 **Flutter App Integration**: Cross-platform mobile app with real-time control
- 🔄 **OTA Updates**: Over-the-air firmware updates with rollback protection
- 🎬 **Scenes & Schedules**: Automation and preset device combinations
- 🛡️ **Safety Features**: Watchdog timers, zero-cross monitoring, auto-shutoff
- 💾 **Persistent Storage**: Configuration and state retention across reboots
- 🎨 **Premium UI**: Glassmorphism design with minimal aesthetics

## Repository Structure

```
smarthome-controller/
├── firmware/                      # ESP32 firmware code
│   └── main/                     # Main firmware application
├── backend/                       # Backend services
│   └── google-apps-script/       # ☁️ Google Apps Script backend
│       ├── Code.gs              # Main API logic
│       ├── Database.gs          # Database helpers
│       ├── Dashboard.html       # Web dashboard UI
│       └── README.md            # Backend deployment guide
├── software_Team/                 # Software development files
│   └── flutter project/          # Flutter mobile app source code
│       └── demo/                 # Main Flutter application
│           ├── lib/
│           │   ├── config/      # API configuration
│           │   ├── models/      # Data models
│           │   ├── services/    # API service layer
│           │   └── pages/       # UI pages
│           └── pubspec.yaml
├── app/                          # 📱 Android APK builds folder
│   ├── release/                  # Release APK builds
│   ├── debug/                    # Debug APK builds
│   └── README.md                 # App build & installation guide
├── README.md                     # This file (overview)
├── FEATURES.md                   # 🎯 Complete feature list
├── DEPLOYMENT.md                 # 🚀 Deployment guide
├── INSTALL.md                    # Installation instructions
└── CHANGELOG.md                  # Version history
```

### 📱 Mobile App

The Android mobile app APK files are located in the [`app/`](./app/) directory. See the [App README](./app/README.md) for:
- Download instructions
- Build instructions from source
- Installation guide
- Troubleshooting

### ☁️ Backend & Web Dashboard

The system uses **Google Apps Script** as the backend with **Google Sheets** as the database. See the [Backend README](./backend/google-apps-script/README.md) for:
- Backend deployment steps
- API documentation
- Database schema
- Web dashboard access

**Features:**
- RESTful API for devices and apps
- Real-time device control
- User authentication
- Scheduling & scenes management
- Web dashboard with premium UI
- Global access (works from anywhere)

### 📚 Documentation

- **[FEATURES.md](./FEATURES.md)** - Complete list of all 100+ features
- **[DEPLOYMENT.md](./DEPLOYMENT.md)** - Step-by-step deployment guide (30 min setup)
- **[Backend README](./backend/google-apps-script/README.md)** - Backend API and deployment
- **[App README](./app/README.md)** - Mobile app build and distribution

## Hardware Requirements

### ESP32 Module
- ESP32-WROOM-32 or compatible
- Minimum 4MB flash
- Dual-core operation

### TRIAC Controller Hardware
- 4x TRIAC channels (BT136 or similar)
- Zero-cross detection circuit
- MOC3021/3041 optocouplers
- 5V power supply

### Pin Configuration

| Function | GPIO Pin | Notes |
|----------|----------|-------|
| Zero-Cross Detection | 13 | Rising edge interrupt |
| TRIAC 1 | 16 | Channel 1 control |
| TRIAC 2 | 17 | Channel 2 control |
| TRIAC 3 | 18 | Channel 3 control |
| TRIAC 4 | 19 | Channel 4 control |
| Switch 1 | 32 | Physical button input |
| Switch 2 | 33 | Physical button input |
| Switch 3 | 25 | Physical button input |
| Switch 4 | 26 | Physical button input |

## Software Architecture

### Core Separation

```
CORE 1 (PRO_CPU) - TIME-CRITICAL:
├── Zero-cross detection ISR (RISING edge @ GPIO13)
├── Hardware timer ISR for phase-angle control (100µs intervals)
├── TRIAC firing with <50µs accuracy
├── Alexa library polling (minimal)
└── NO blocking calls or network operations

CORE 0 (APP_CPU) - CONNECTIVITY:
├── WiFi management with auto-reconnect
├── Google Apps Script polling (2.5s interval)
├── Voice assistant handling (Alexa + SinricPro)
├── Physical switch debouncing
├── Web servers (port 80, 8080, WebSocket 81)
├── OTA update handling
├── Schedule execution
├── Scene activation
├── Fade transitions
└── Watchdog monitoring (15s timeout)
```

### Data Flow

```
Physical Switch → Debounce → Device State Update → TRIAC Control
                                      ↓
                            Cloud Sync / WebSocket Broadcast
                                      ↓
Voice Command → Alexa/Google → Device State Update → TRIAC Control
                                      ↓
                            Cloud Sync / WebSocket Broadcast
```

## Installation

### Arduino IDE Setup

1. Install ESP32 board support:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```

2. Install required libraries:
   - WiFiManager by tzapu (v2.0.16+)
   - ArduinoJson by Benoit Blanchon (v6.x)
   - Espalexa by Christian Schwinne (v2.7.0+)
   - WebSockets by Markus Sattler (v2.4.0+)
   - SinricPro (v2.10.0+) [for Google Assistant]

3. Open `firmware/main/main.ino`

4. Configure Google Apps Script URL:
   ```cpp
   String GOOGLE_SCRIPT_URL = "your_script_url_here";
   ```

5. Select board: `ESP32 Dev Module`

6. Upload firmware

### PlatformIO Setup

Create `platformio.ini`:

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200

lib_deps = 
    tzapu/WiFiManager@^2.0.16
    bblanchon/ArduinoJson@^6.21.3
    vintlabs/Fauxmo ESP@^3.4
    links2004/WebSockets@^2.4.0
    sinricpro/SinricPro@^2.10.0

build_flags = 
    -DCORE_DEBUG_LEVEL=3
    -DBOARD_HAS_PSRAM
```

## Configuration

### Initial Setup

1. Power on the device
2. Connect to WiFi network: `Smart_Home_Hub`
3. Open browser to `192.168.4.1`
4. Select your WiFi network and enter password
5. Device will reboot and connect

### Factory Reset

Hold Switch 1 for 10 seconds to reset all settings and WiFi credentials.

### Google Apps Script Backend

Deploy the Google Apps Script to handle:
- Device state polling
- Remote configuration
- OTA update distribution
- Usage statistics
- Flutter app API proxy

Example request format:
```json
{
  "action": "poll",
  "uid": "AA:BB:CC:DD:EE:FF",
  "ver": 3.0,
  "uptime": 3600,
  "rssi": -45,
  "heap": 180000,
  "d1": {"s": 1, "v": 75, "t": 1, "runtime": 1800},
  "d2": {"s": 0, "v": 100, "t": 0, "runtime": 0},
  "d3": {"s": 1, "v": 50, "t": 2, "runtime": 900},
  "d4": {"s": 0, "v": 100, "t": 0, "runtime": 0}
}
```

Example response format:
```json
{
  "sys_name": "Living Room Hub",
  "ota_update": false,
  "bin_url": "",
  "d1": {
    "type": "FAN",
    "name": "Ceiling Fan",
    "lock": false,
    "update": true,
    "state": 1,
    "val": 80
  },
  "schedules": [],
  "scenes": []
}
```

## API Documentation

### Local REST API (Port 8080)

All endpoints return JSON responses.

#### GET /status
Get current device states
```json
{
  "devices": [
    {
      "id": 0,
      "name": "Living Room Light",
      "type": 2,
      "state": true,
      "brightness": 75,
      "runtime": 3600
    }
  ]
}
```

#### POST /control
Control a device
```json
{
  "id": 0,
  "state": true,
  "brightness": 75
}
```

#### GET /info
System information
```json
{
  "name": "Living Room Hub",
  "firmware": "3.0",
  "mac": "AA:BB:CC:DD:EE:FF",
  "ip": "192.168.1.100",
  "uptime": 3600,
  "rssi": -45,
  "heap": 180000,
  "cloud_connected": true
}
```

### WebSocket (Port 81)

Real-time device state updates:
```json
{
  "type": "device_update",
  "id": 0,
  "state": true,
  "brightness": 75,
  "name": "Living Room Light"
}
```

## Voice Assistant Setup

### Amazon Alexa

1. Open Alexa app
2. Go to Devices → Add Device
3. Select "Other" → "Discover Devices"
4. Devices will appear as configured names

### Google Assistant (via SinricPro)

1. Create account at sinric.pro
2. Add devices in SinricPro dashboard
3. Configure App Key and Secret in Google Apps Script
4. Link "Smart Home" service in Google Home app
5. Discover devices

## Advanced Features

### Scenes

Create preset device combinations:
```cpp
Scene scene;
scene.name = "Movie Time";
scene.devices[0] = {0, false, 0};     // Light off
scene.devices[1] = {1, true, 20};     // Dimmer at 20%
```

### Schedules

Time-based automation:
```cpp
Schedule schedule;
schedule.deviceId = 0;
schedule.startMins = 18 * 60;         // 6:00 PM
schedule.endMins = 23 * 60;           // 11:00 PM
schedule.startBrightness = 0;
schedule.endBrightness = 75;
schedule.daysOfWeek = 0x7F;           // All days
```

### Smooth Dimming

Automatic fade transitions:
```cpp
setDeviceState(deviceId, true, 75, true);  // Fade to 75%
```

## Safety Features

- **Zero-Cross Detection**: Monitors AC waveform; shuts down if signal lost
- **Watchdog Timers**: 15s Core 0, 5s Core 1 (time-critical)
- **Auto-Shutoff**: Configurable timeout per device (default 1 hour)
- **Brownout Protection**: ESP32 built-in brownout detector
- **OTA Rollback**: Failed updates automatically rollback
- **Memory Monitoring**: Tracks heap usage to prevent crashes

## Troubleshooting

### Device Not Responding
1. Check power supply (stable 5V required)
2. Verify zero-cross signal with oscilloscope
3. Check Serial monitor for error messages

### WiFi Connection Issues
1. Ensure 2.4GHz WiFi network
2. Check signal strength (need >-80 dBm)
3. Factory reset and reconfigure

### Voice Control Not Working
- **Alexa**: Ensure device is on same network, rediscover devices
- **Google**: Verify SinricPro credentials, check cloud connection

### OTA Update Failed
1. Check internet connectivity
2. Verify firmware binary URL is accessible
3. Ensure sufficient flash space
4. Review Serial logs for specific error

## Development

### Building from Source

```bash
git clone https://github.com/Gokul3611/smarthome-controller.git
cd smarthome-controller/firmware/main
# Open in Arduino IDE or PlatformIO
```

### Debugging

Enable verbose logging:
```cpp
#define LOG_LEVEL LOG_DEBUG  // in config.h
```

Monitor Serial output at 115200 baud.

### Contributing

1. Fork the repository
2. Create feature branch
3. Make changes with proper documentation
4. Test thoroughly on hardware
5. Submit pull request

## License

MIT License - See LICENSE file for details

## Support

- **Issues**: https://github.com/Gokul3611/smarthome-controller/issues
- **Wiki**: https://github.com/Gokul3611/smarthome-controller/wiki
- **Email**: support@smarthome-controller.com

## Changelog

### v3.0 (Current)
- Multi-core FreeRTOS architecture
- Google Assistant integration (SinricPro)
- WebSocket real-time updates
- Scene and schedule support
- Smooth fade transitions
- Enhanced safety features
- OTA rollback protection
- Improved cloud communication
- Production-ready code quality

### v2.1
- Basic TRIAC control
- Alexa integration
- Google Apps Script polling
- WiFi management

---

**Built with ❤️ for reliable smart home automation**
" 
