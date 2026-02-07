"# Smart Home Controller

![License](https://img.shields.io/badge/license-MIT-orange)
![Platform](https://img.shields.io/badge/platform-ESP32-green)

Full-stack IoT home automation system: ESP32 firmware with 4-channel TRIAC phase-angle control, Google Apps Script cloud backend, web dashboard, and Flutter mobile app. Controls AC loads (lights, fans) via physical switches, web interface, mobile app, and Amazon Alexa voice commands.

Built as an internship project demonstrating hardware design, embedded firmware, cloud services, and mobile development.

## Project Status

**Maturity**: Portfolio project / advanced prototype
**Hardware tested**: ESP32-WROOM-32 with BT136 TRIAC modules
**Known limitations**:
- Google Assistant integration (SinricPro) is scaffolded but not fully wired up
- Flutter app requires backend URL configuration before use
- No automated test suite

**System components**:
- Hardware (PCB schematics, BOM)
- Firmware (ESP32 C++, FreeRTOS dual-core)
- Backend (Google Apps Script + Google Sheets)
- Web dashboard (HTML/CSS/JS served from Apps Script)
- Mobile app (Flutter/Dart, Android)

## Quick Links

- [Deployment Guide](./DEPLOYMENT.md) — Set up the full system
- [Feature List](./FEATURES.md) — What is implemented
- [Hardware Guide](./hardware/README.md) — Circuit diagrams and assembly
- [Backend Guide](./backend/google-apps-script/README.md) — Cloud API and database
- [Mobile App](./app/README.md) — Download and build instructions
- [Changelog](./CHANGELOG.md) — Version history

## Repository Structure

```
smarthome-controller/
├── firmware/                   # ESP32 firmware
│   ├── main/                   #   Main controller (4-channel TRIAC)
│   └── mini_controller/        #   Simplified variant
├── backend/                    # Cloud services
│   └── google-apps-script/     #   Apps Script backend + web dashboard
├── frontend/                   # Mobile application
│   └── flutter project/demo/   #   Flutter Android/iOS app
├── hardware/                   # Hardware design files
│   ├── pcb/                    #   KiCad PCB layout & schematic
│   ├── schematics/             #   Circuit diagrams
│   └── bom/                    #   Bill of materials
├── app/                        # APK build artifacts (via GitHub Actions)
├── README.md                   # This file
├── FEATURES.md                 # Implemented feature list
├── DEPLOYMENT.md               # Setup and deployment guide
└── CHANGELOG.md                # Version history
```

## Architecture

### Dual-Core Design (FreeRTOS)

```
Core 1 (PRO_CPU) — time-critical:
├── Zero-cross detection ISR (rising edge, GPIO 13)
├── Hardware timer ISR for phase-angle control (100 us intervals)
├── TRIAC firing (<50 us accuracy)
└── No blocking calls or network operations

Core 0 (APP_CPU) — connectivity:
├── WiFi management with auto-reconnect
├── Google Apps Script polling (2.5 s interval)
├── Amazon Alexa voice handling (local, via Espalexa)
├── Physical switch debouncing
├── HTTP server (port 80 redirect, port 8080 REST API)
├── WebSocket server (port 81, real-time state sync)
├── OTA firmware updates
├── Schedule and scene execution
├── Fade transitions
└── Watchdog monitoring (15 s timeout)
```

### Data Flow

```
Physical Switch --> Debounce --> Device State --> TRIAC Control
                                    |
                          Cloud Sync / WebSocket Broadcast
                                    |
Voice Command --> Alexa --> Device State --> TRIAC Control
```

## Hardware Requirements

### Components

- ESP32-WROOM-32 (4 MB flash, dual-core)
- 4x BT136 TRIACs with MOC3021 optocouplers
- Zero-cross detection circuit
- 5V power supply (2A minimum)
- 4x physical push buttons (optional)

### Pin Configuration

| Function | GPIO | Notes |
|----------|------|-------|
| Zero-Cross Detection | 13 | Rising edge interrupt |
| TRIAC Channel 1 | 16 | |
| TRIAC Channel 2 | 17 | |
| TRIAC Channel 3 | 18 | |
| TRIAC Channel 4 | 19 | |
| Switch 1 | 32 | Internal pull-up |
| Switch 2 | 33 | Internal pull-up |
| Switch 3 | 25 | Internal pull-up |
| Switch 4 | 26 | Internal pull-up |

## Installation

### Arduino IDE

1. Add ESP32 board support URL:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```

2. Install libraries via Library Manager:
   - WiFiManager by tzapu (v2.0.16+)
   - ArduinoJson by Benoit Blanchon (v6.x)
   - Espalexa by Christian Schwinne (v2.7.0+)
   - WebSockets by Markus Sattler (v2.4.0+)

3. Open `firmware/main/main.ino`

4. Set your Google Apps Script URL and API key (see [DEPLOYMENT.md](./DEPLOYMENT.md)):
   ```cpp
   String GOOGLE_SCRIPT_URL = "your_script_url_here";
   String API_KEY = "your_api_key_here";
   ```

5. Select board: ESP32 Dev Module, upload speed 921600

6. Upload firmware

### Initial WiFi Setup

1. Power on the ESP32
2. Connect to WiFi network `Smart_Home_Hub`
3. Open `192.168.4.1` in a browser
4. Select your WiFi network and enter password
5. Device reboots and connects

### Factory Reset

Hold Switch 1 for 10 seconds to clear WiFi credentials and settings.

## Local API

### REST API (Port 8080)

**GET /status** — Current device states
```json
{
  "devices": [
    {"id": 0, "name": "Living Room Light", "type": 2, "state": true, "brightness": 75, "runtime": 3600}
  ]
}
```

**POST /control** — Control a device
```json
{"id": 0, "state": true, "brightness": 75}
```

**GET /info** — System information
```json
{
  "name": "Living Room Hub", "firmware": "3.0", "mac": "AA:BB:CC:DD:EE:FF",
  "ip": "192.168.1.100", "uptime": 3600, "rssi": -45, "heap": 180000
}
```

### WebSocket (Port 81)

Real-time device state updates:
```json
{"type": "device_update", "id": 0, "state": true, "brightness": 75}
```

## Voice Control

### Amazon Alexa (local network)

1. Open Alexa app > Devices > Add Device > Other > Discover Devices
2. Devices appear with their configured names
3. Supports on/off and brightness/speed commands

### Google Assistant (via SinricPro)

**Note**: SinricPro integration is scaffolded in the firmware but not fully wired up.
To complete it, create a SinricPro account, add devices, and configure the app key/secret.

## Safety Features

- Zero-cross detection with auto-shutoff if AC signal is lost
- Watchdog timers: 15 s (Core 0), 5 s (Core 1)
- Configurable auto-off per device (default 1 hour)
- ESP32 built-in brownout protection
- OTA rollback on failed updates
- Heap monitoring to prevent memory exhaustion

## Troubleshooting

| Problem | Solution |
|---------|----------|
| Device not responding | Check 5V power supply; verify zero-cross signal |
| WiFi won't connect | Ensure 2.4 GHz network; check signal > -80 dBm; factory reset |
| Alexa can't find devices | Must be on same network; rediscover in Alexa app |
| OTA update failed | Check internet; verify binary URL; check flash space |

## License

MIT License — see [LICENSE](./LICENSE) file.

## Support

- [GitHub Issues](https://github.com/Gokul3611/smarthome-controller/issues)" 
