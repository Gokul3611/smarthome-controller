# System Deployment Procedures
**Smart Home Controller v3.1**

## Document Control
- **Document Number:** SYS-DEPLOY-001
- **Revision:** 3.1  
- **Classification:** Deployment Guide
- **Date:** December 2024

## 1. Scope

This document provides procedures for deploying the Smart Home Controller system from initial hardware assembly through production operation.

## 2. Prerequisites

### 2.1 Hardware Requirements
- ESP32-WROOM-32 development board or custom PCB
- AC power source (110-240V, 50/60Hz)
- USB-to-serial adapter (for programming)
- Test loads (resistive, <4A per channel)

### 2.2 Software Requirements
- Arduino IDE 1.8.x or later, OR PlatformIO
- ESP32 board support package
- USB drivers (CH340/CP2102/FTDI)
- Google account (for backend services)

### 2.3 Network Requirements
- WiFi access point (2.4GHz, WPA2)
- Internet connectivity (for cloud features)
- Static IP reservation (recommended for production)

## 3. Hardware Assembly

### 3.1 Circuit Verification
1. Verify circuit assembly per `hardware/CONNECTION_GUIDE.md`
2. Inspect solder joints for quality (IPC-A-610 criteria)
3. Measure isolation resistance (AC to DC): >10MΩ required

### 3.2 Power-On Test
1. Apply 5V DC to ESP32 VIN (initial test, AC disconnected)
2. Verify 3.3V regulator output
3. Connect USB-to-serial adapter
4. Verify serial communication (115200 baud)

## 4. Firmware Installation

### 4.1 Configuration
Edit `firmware/main/config.h`:

```cpp
// WiFi credentials
const char* ssid = "YOUR_NETWORK_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Backend URL (configure after backend deployment)
const char* cloudURL = "YOUR_GOOGLE_APPS_SCRIPT_URL";
```

### 4.2 Compilation and Upload

**Using Arduino IDE:**
```
1. Open firmware/main/main.ino
2. Select board: "ESP32 Dev Module"
3. Select port: /dev/ttyUSB0 (or appropriate port)
4. Click Upload
```

**Using PlatformIO:**
```bash
cd firmware/main
pio run --target upload
pio device monitor --baud 115200
```

### 4.3 Verification
Monitor serial output for:
- WiFi connection confirmation
- IP address assignment
- WebSocket server initialization
- Cloud polling activation

## 5. Backend Deployment

### 5.1 Google Apps Script Setup
1. Access https://script.google.com
2. Create new project: "SmartHomeController"
3. Import `backend/google-apps-script/Code.gs`
4. Import `backend/google-apps-script/Dashboard.html`

### 5.2 Deployment Configuration
1. Deploy → New deployment
2. Type: Web app
3. Execute as: Me
4. Access: Anyone with link
5. Deploy and copy URL

### 5.3 Firmware Update
Update `config.h` with deployment URL and re-upload firmware.

## 6. System Validation

### 6.1 Functional Tests
| Test | Procedure | Pass Criteria |
|------|-----------|---------------|
| Power supply | Measure 5V rail | 4.95-5.05V |
| WiFi connectivity | Check serial output | Connected, IP assigned |
| Cloud polling | Monitor serial | HTTP 200 responses |
| WebSocket | Connect client | Connection accepted |
| TRIAC control | Command channel ON | Load energizes |
| Physical switches | Press button | State toggles |

### 6.2 Safety Verification
⚠️ **WARNING:** AC mains voltage tests require qualified personnel.

1. Isolation test: AC to DC >10MΩ @ 500V DC
2. Ground continuity: <1Ω to safety earth
3. No exposed conductors
4. Proper fuse ratings verified

## 7. Dashboard Access

### 7.1 Web Interface
- **URL:** Google Apps Script deployment URL
- **Authentication:** Google account or link-based access
- **Features:** Device control, system statistics, configuration

### 7.2 Mobile Application
- **Android:** Install APK from `app/release/`
- **iOS:** Install via TestFlight or Xcode
- **Configuration:** Enter backend URL in settings

## 8. Production Deployment

### 8.1 Enclosure Installation
1. Mount PCB in appropriate enclosure (IP20 minimum)
2. Provide ventilation for heat dissipation
3. Label AC terminals (Live, Neutral, Ground)
4. Secure all connections with proper torque

### 8.2 AC Wiring
Refer to local electrical codes. Typical procedure:
1. De-energize AC circuit (verify with multimeter)
2. Connect AC input (Live, Neutral, Ground)
3. Connect loads to TRIAC outputs
4. Verify wiring before energizing
5. Apply AC power via variac (gradual voltage increase)

### 8.3 Final Commissioning
1. Verify all channels operational
2. Test emergency shutdown (physical switches)
3. Configure schedules and scenes
4. Document installation (location, date, settings)

## 9. Troubleshooting

| Symptom | Probable Cause | Resolution |
|---------|----------------|------------|
| No WiFi connection | Incorrect credentials | Verify SSID/password in config.h |
| No cloud polling | Wrong URL | Check deployment URL |
| TRIAC not switching | Hardware fault | Test optocoupler, TRIAC |
| Firmware upload fails | Driver issue | Install CH340/CP2102 drivers |
| Dashboard not loading | Deployment issue | Re-deploy Apps Script |

## 10. Maintenance

### 10.1 Firmware Updates
Use OTA (Over-the-Air) update procedure per `OTA_GUIDE.md`.

### 10.2 Periodic Checks
- Monthly: Verify operation of all channels
- Quarterly: Check for firmware updates
- Annually: Inspect for dust accumulation, clean if necessary

## References

1. ESP32 Datasheet, Espressif Systems
2. Local electrical codes (NEC, IEC 60364, etc.)
3. IPC-A-610: Acceptability of Electronic Assemblies

---

**Document Control:** SYS-DEPLOY-001 Rev 3.1  
**Approved by:** [Signature]  
**Date:** December 2024
