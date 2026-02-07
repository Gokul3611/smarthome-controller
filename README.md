"# Smart Home Controller

![Version](https://img.shields.io/badge/version-3.0-blue)
![Platform](https://img.shields.io/badge/platform-ESP32-green)
![License](https://img.shields.io/badge/license-MIT-orange)

Full-stack IoT home automation system with ESP32 firmware, Google Apps Script backend, web dashboard, and Flutter mobile app. Controls 4 AC load channels via TRIAC phase-angle dimming with cloud synchronization, scheduling, and Amazon Alexa voice control.

## Project Status

**Maturity**: Portfolio project / advanced prototype
**Tested on**: ESP32-WROOM-32 with custom TRIAC controller board
**Known limitations**:
- Google Assistant integration (SinricPro) is stubbed but not yet functional
- No automated test suite for firmware or backend
- Flutter app requires manual API URL configuration

**What works**:
- 4-channel TRIAC phase-angle control with zero-cross detection
- Amazon Alexa voice control (via Espalexa, local network)
- Google Apps Script cloud backend with polling
- Web dashboard with real-time device control
- Flutter mobile app with device control, schedules, and scenes
- OTA firmware updates with rollback protection
- Scene and schedule automation
- WebSocket real-time state sync
- Persistent storage across reboots
- GitHub Actions CI for automated APK builds

**Not yet implemented**:
- Google Assistant control (SinricPro code is stubbed)
- Automated tests

## UI Preview

![Smart Home Controller Dashboard](https://github.com/user-attachments/assets/d744726f-4be5-4d46-ad67-1705a11d3b0f)

*Web dashboard with real-time device control and system statistics*

## Quick Start

1. **Deploy backend** — Copy `backend/google-apps-script/` files to a Google Apps Script project and deploy as a web app. See [Deployment Guide](./DEPLOYMENT.md).
2. **Flash firmware** — Open `firmware/main/main.ino` in Arduino IDE, set your Apps Script URL, and upload to ESP32. See [Installation Guide](./INSTALL.md).
3. **Build mobile app** — Run `flutter build apk --release` in `frontend/flutter project/demo/`. Or download from [Releases](https://github.com/Gokul3611/smarthome-controller/releases/latest).

## Documentation

- [Deployment Guide](./DEPLOYMENT.md) — Full system setup
- [Installation Guide](./INSTALL.md) — Firmware and hardware setup
- [Features List](./FEATURES.md) — Complete feature documentation
- [Hardware Guide](./hardware/README.md) — Circuit diagrams and assembly
- [OTA Updates](./OTA_GUIDE.md) — Firmware update procedures
- [Mobile App](./app/README.md) — APK download and build instructions
- [Backend & Dashboard](./backend/google-apps-script/README.md) — API and web dashboard
- [Bill of Materials](./hardware/bom/BOM.md) — Component list and cost
- [Security Practices](./docs/SECURITY_BEST_PRACTICES.md) — Security considerations

## System Architecture

### Components

- **Multi-core architecture**: Time-critical TRIAC control on Core 1, network operations on Core 0
- **Amazon Alexa**: Voice control via Espalexa (local network discovery)
- **Cloud backend**: Google Apps Script + Google Sheets for remote control and configuration
- **Web dashboard**: HTML5 control panel served from Apps Script
- **Flutter mobile app**: Cross-platform app with device control, schedules, and scenes
- **OTA updates**: Over-the-air firmware updates with rollback protection
- **Scenes & schedules**: Automation and preset device combinations
- **Safety features**: Watchdog timers, zero-cross monitoring, auto-shutoff
- **Persistent storage**: Configuration and state retention across reboots

## Repository Structure

```
smarthome-controller/
├── firmware/                      # ESP32 firmware code
│   ├── main/                     # Main 4-channel controller firmware
│   └── mini_controller/          # Simplified single-channel variant
├── backend/                       # Cloud backend
│   └── google-apps-script/       # Google Apps Script API + web dashboard
├── frontend/                      # Mobile app
│   └── flutter project/demo/     # Flutter cross-platform app
├── hardware/                      # Hardware design files
│   ├── pcb/                      # KiCad PCB design
│   ├── schematics/               # Circuit diagrams
│   └── bom/                      # Bill of materials
├── app/                          # Android APK builds (via CI)
├── docs/                         # Additional documentation
├── DEPLOYMENT.md                 # Full system deployment guide
├── INSTALL.md                    # Firmware installation and setup
├── FEATURES.md                   # Feature list
├── OTA_GUIDE.md                  # OTA update procedures
└── CHANGELOG.md                  # Version history
```

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
└── NO blocking calls or network operations

CORE 0 (APP_CPU) - CONNECTIVITY:
├── WiFi management with auto-reconnect
├── Google Apps Script cloud polling (2.5s interval)
├── Alexa voice assistant handling
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

Voice Command → Alexa → Device State Update → TRIAC Control
                                      ↓
                            Cloud Sync / WebSocket Broadcast
```

## Local REST API (Port 8080)

| Endpoint | Method | Description |
|----------|--------|-------------|
| `/status` | GET | Current device states |
| `/control` | POST | Control a device (`{"id": 0, "state": true, "brightness": 75}`) |
| `/info` | GET | System information (firmware, IP, uptime, RSSI, heap) |
| `/config` | POST | Update device configuration |
| `/schedules` | GET/POST | List or create schedules |
| `/schedules/{id}` | DELETE | Delete a schedule |
| `/scenes` | GET/POST | List or create scenes |
| `/scenes/{id}/activate` | POST | Activate a scene |
| `/scenes/{id}` | DELETE | Delete a scene |
| `/restart` | POST | Restart device |
| `/factory-reset` | POST | Factory reset (requires `{"confirm": true}`) |

WebSocket on port 81 provides real-time device state updates.

## License

MIT License — See [LICENSE](./LICENSE) file for details.

## Support

- **Issues**: https://github.com/Gokul3611/smarthome-controller/issues" 
