# Smart Home Controller - IoT Load Management System
**Version 3.1 | Production Release**

## Abstract

This document describes a complete Internet-of-Things (IoT) automation system for residential AC load control. The system comprises ESP32-based hardware, cloud infrastructure, web-based dashboard, and cross-platform mobile application. The design emphasizes safety, reliability, and standards compliance while providing multiple control interfaces including WiFi, physical switches, and voice integration.

**Keywords:** IoT, smart home, ESP32, load control, home automation, embedded systems

---

## 1. System Overview

### 1.1 Scope

The Smart Home Controller is a distributed control system designed for residential automation applications. The system provides wireless control and monitoring of four independent AC electrical loads with real-time status feedback and cloud synchronization.

### 1.2 Architecture

The system employs a layered architecture:

1. **Hardware Layer:** ESP32-based controller with TRIAC load switching
2. **Firmware Layer:** FreeRTOS-based embedded software
3. **Communication Layer:** WiFi (IEEE 802.11 b/g/n), HTTP/HTTPS, WebSocket
4. **Backend Layer:** Google Apps Script cloud services
5. **Presentation Layer:** Web dashboard and mobile applications

### 1.3 Key Specifications

| Parameter | Specification |
|-----------|--------------|
| Control Channels | 4 independent AC outputs |
| Input Voltage | 110-240V AC, 50/60Hz |
| Channel Capacity | 4A per channel (resistive) |
| Wireless Protocol | WiFi 2.4GHz (802.11 b/g/n) |
| Control Methods | WiFi, physical switches, voice |
| Operating Temperature | -20°C to +70°C |

---

## 2. System Components

### 2.1 Hardware Platform

**Microcontroller:** Espressif ESP32-WROOM-32
- Dual-core Xtensa LX6, 240MHz
- 520KB SRAM, 4MB Flash
- Integrated WiFi and Bluetooth

**Power Management:**
- Isolated AC-DC converter (HLK-PM01)
- 5V DC output, 600mA capacity

**Load Switching:**
- TRIAC-based AC switching (BTA16-600B)
- Zero-cross detection for EMI reduction
- Optocoupler isolation (5kV rating)

**Documentation:** See `hardware/` directory for complete specifications.

### 2.2 Firmware Architecture

**Operating System:** FreeRTOS v10.x
- Task-based scheduling
- Dual-core utilization (control on Core 1, networking on Core 0)
- Watchdog timer protection

**Key Features:**
- Zero-cross synchronized phase control
- Non-volatile configuration storage (NVS)
- Over-the-air (OTA) firmware updates
- Modular API architecture

**Source Code:** `firmware/main/` directory

### 2.3 Backend Services

**Platform:** Google Apps Script
- RESTful API endpoints
- Google Sheets data persistence
- Authentication and access control

**Functions:**
- Device state management
- Configuration synchronization
- Schedule execution
- User interface serving

**Implementation:** `backend/google-apps-script/` directory

### 2.4 User Interfaces

**Web Dashboard:**
- Real-time control via WebSocket
- Responsive design (desktop/mobile)
- System statistics and monitoring

**Mobile Application:**
- Flutter framework (cross-platform)
- Local network and cloud control modes
- Native iOS and Android support

**Documentation:** `app/` and `backend/` directories

---

## 3. Features and Capabilities

### 3.1 Control Features

- Individual device on/off control
- Brightness/speed adjustment (0-100%, phase control)
- Scene activation (multi-device presets)
- Time-based scheduling
- Physical switch override

### 3.2 System Features

- Real-time status synchronization
- Cloud backup and restore
- OTA firmware updates
- Voice assistant integration (Alexa, Google Home)
- Child lock functionality
- Auto-shutoff timers

### 3.3 Monitoring and Diagnostics

- Device runtime tracking
- WiFi signal strength (RSSI)
- System uptime and statistics
- Error logging and reporting

**Complete Feature List:** See `FEATURES.md`

---

## 4. Documentation Index

### 4.1 Getting Started

| Document | Description |
|----------|-------------|
| `DEPLOYMENT.md` | System deployment and configuration |
| `INSTALL.md` | Installation procedures |
| `HOW_TO_VIEW_UI.md` | User interface access guide |

### 4.2 Technical Documentation

| Document | Description |
|----------|-------------|
| `hardware/README.md` | Hardware design specification |
| `hardware/PCB_Manufacturing/` | Manufacturing documentation |
| `SYSTEM_INTEGRATION.md` | Architecture and integration |
| `OTA_GUIDE.md` | Firmware update procedures |

### 4.3 Production Documentation

| Document | Description |
|----------|-------------|
| `PRODUCTION_GUIDE.md` | Production deployment procedures |
| `PRODUCTION_READY_SUMMARY.md` | Production readiness assessment |
| `HANDOVER.md` | Knowledge transfer documentation |
| `hardware/bom/BOM.md` | Bill of materials |

### 4.4 Quality Documentation

| Document | Description |
|----------|-------------|
| `docs/SECURITY_BEST_PRACTICES.md` | Security guidelines |
| `docs/PRODUCTION_DEPLOYMENT_CHECKLIST.md` | Deployment checklist |
| `hardware/PCB_Manufacturing/TESTING_VALIDATION.md` | Test procedures |

---

## 5. Quick Start

### 5.1 Prerequisites

- ESP32 development board or custom hardware
- Google account (for backend services)
- Arduino IDE or PlatformIO
- WiFi network (2.4GHz)

### 5.2 Hardware Setup

1. Assemble circuit per `hardware/CONNECTION_GUIDE.md`
2. Verify AC wiring and isolation
3. Connect ESP32 to programming interface

### 5.3 Firmware Installation

```bash
# Configure WiFi credentials
# Edit firmware/main/config.h

# Compile and upload
cd firmware/main
arduino --upload main.ino

# Monitor serial output
screen /dev/ttyUSB0 115200
```

### 5.4 Backend Deployment

1. Open Google Apps Script editor
2. Import files from `backend/google-apps-script/`
3. Deploy as web application
4. Configure firmware with deployment URL

**Detailed Instructions:** See `DEPLOYMENT.md`

---

## 6. System Architecture

### 6.1 Data Flow

```
Physical Switch → ESP32 GPIO → Firmware Processing
     ↓                              ↓
User Input (Web/App) → Backend API → Cloud Database
     ↓                              ↓
     └──────── Status Sync ─────────┘
                  ↓
            TRIAC Control → AC Load
```

### 6.2 Communication Protocols

- **Local Control:** WebSocket (Port 81), HTTP REST API (Port 8080)
- **Cloud Sync:** HTTPS polling (2.5s interval)
- **Voice Control:** SSDP/UPnP discovery for Alexa/Google Home

### 6.3 Safety Architecture

- Dual-isolation design (optical + physical)
- Zero-cross switching (EMI reduction)
- Watchdog timer protection
- Fail-safe shutdown on errors

**Detailed Architecture:** See `SYSTEM_INTEGRATION.md`

---

## 7. Standards and Compliance

### 7.1 Hardware Standards

- **IPC-2221:** PCB design standard
- **IEC 60950-1:** IT equipment safety
- **RoHS:** Lead-free compliance

### 7.2 Software Standards

- **FreeRTOS:** Real-time operating system
- **RESTful API:** HTTP-based service architecture
- **JSON:** Data interchange format

### 7.3 Wireless Standards

- **IEEE 802.11 b/g/n:** WiFi communication
- **SSDP/UPnP:** Device discovery protocol

---

## 8. Development Team and Contributions

### 8.1 Project Structure

This system was developed as a comprehensive embedded systems project, demonstrating:

- Hardware circuit design and PCB layout
- Embedded firmware development (C/C++)
- Cloud backend implementation
- Web and mobile application development
- System integration and testing

### 8.2 Repository Organization

```
smarthome-controller/
├── firmware/              # ESP32 embedded software
├── hardware/              # Circuit design and PCB files
├── backend/               # Cloud services (Google Apps Script)
├── app/                   # Mobile application (Flutter)
├── docs/                  # Additional documentation
└── software_Team/         # Development resources
```

---

## 9. Licensing

This project is released under the MIT License. See LICENSE file for details.

Hardware designs are available under CERN Open Hardware License v2.0.

---

## 10. Support and Contact

### 10.1 Documentation

Primary documentation is maintained in this repository. For specific topics:

- Hardware questions: See `hardware/` directory
- Firmware issues: See `firmware/` directory  
- Backend integration: See `backend/` directory
- Mobile app: See `app/` directory

### 10.2 Issue Tracking

Technical issues should be reported via GitHub Issues with appropriate labels:
- `hardware`: Circuit or PCB issues
- `firmware`: Embedded software bugs
- `backend`: Cloud service problems
- `app`: Mobile application issues

---

## 11. Version Information

- **System Version:** 3.1
- **Hardware Revision:** 3.1 (compact design)
- **Firmware Version:** 3.0
- **Backend API Version:** 3.0
- **Release Date:** December 2024

**Status:** Production Ready

---

## References

1. ESP32 Technical Reference Manual, Espressif Systems
2. IEC 60950-1:2005, Safety of IT Equipment
3. IPC-2221B, Generic Standard on Printed Board Design
4. FreeRTOS Kernel Guide, Amazon Web Services

---

**Document Control:**
- **Document Number:** SYS-README-001
- **Revision:** 3.1
- **Last Updated:** December 2024
- **Classification:** Public Technical Documentation

**End of Document**
