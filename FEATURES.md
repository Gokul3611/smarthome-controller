# System Feature Specification
**Smart Home Controller v3.1**

## Document Control
- **Document Number:** SYS-FEAT-001
- **Revision:** 3.1
- **Date:** December 2024

## 1. Feature Categories

### 1.1 Core Control Features
| Feature ID | Description | Interface |
|------------|-------------|-----------|
| FC-001 | Individual device on/off control | All interfaces |
| FC-002 | Brightness/speed adjustment (0-100%) | Web, app |
| FC-003 | Scene activation (multi-device presets) | All interfaces |
| FC-004 | Schedule management (time-based automation) | Web, app |
| FC-005 | Physical switch override | Hardware inputs |

### 1.2 Communication Features
| Feature ID | Description | Protocol |
|------------|-------------|----------|
| CM-001 | WiFi connectivity | IEEE 802.11 b/g/n |
| CM-002 | Local WebSocket control | Port 81 |
| CM-003 | Local REST API | Port 8080, HTTP |
| CM-004 | Cloud synchronization | HTTPS, 2.5s polling |
| CM-005 | Voice assistant integration | SSDP/UPnP |

### 1.3 System Features
| Feature ID | Description | Implementation |
|------------|-------------|----------------|
| SF-001 | Real-time state synchronization | WebSocket push |
| SF-002 | Configuration persistence | NVS (non-volatile storage) |
| SF-003 | OTA firmware updates | HTTPS download |
| SF-004 | Watchdog timer protection | FreeRTOS |
| SF-005 | Error logging and diagnostics | Serial + cloud |

### 1.4 Safety Features
| Feature ID | Description | Standard |
|------------|-------------|----------|
| SAF-001 | Dual isolation (optical + physical) | IEC 60950-1 |
| SAF-002 | Zero-cross switching | EMI reduction |
| SAF-003 | Thermal monitoring | ESP32 internal sensor |
| SAF-004 | Overcurrent protection | Tri-level fusing |
| SAF-005 | Auto-shutdown on error | Firmware fail-safe |

## 2. User Interface Features

### 2.1 Web Dashboard
- Real-time device control
- System statistics display
- Schedule and scene management
- Configuration interface
- Responsive design (desktop/mobile)

### 2.2 Mobile Application
- Cross-platform (Android/iOS)
- Local network discovery
- Cloud-based remote access
- Push notifications
- Dark mode support

### 2.3 Voice Control
- Amazon Alexa integration
- Google Home integration
- Device discovery via SSDP
- Natural language commands

## 3. Advanced Features

### 3.1 Automation
- Time-based schedules (daily, weekly)
- Scene presets (multi-device coordination)
- Conditional logic (if-then rules)
- Sunrise/sunset adjustments

### 3.2 Monitoring
- Device runtime tracking
- Power consumption estimation
- WiFi signal strength (RSSI)
- System uptime and statistics

### 3.3 Security
- Child lock per device
- Access logging
- HTTPS encryption (cloud)
- WPA2 WiFi security

## 4. Hardware Features

### 4.1 Control Channels
- 4 independent AC outputs
- 4A continuous per channel (resistive)
- Phase control dimming (0-100%)
- Zero-cross synchronized switching

### 4.2 Input Interfaces
- 4 physical switch inputs
- Pull-up configuration
- Software debouncing (50ms)
- Toggle mode operation

### 4.3 Indicators
- Power LED (system status)
- Activity LED (network traffic)
- Per-channel status LEDs (optional)

## 5. Software Architecture

### 5.1 Firmware Components
- **Core 0:** Network stack, HTTP server, WebSocket
- **Core 1:** TRIAC control, zero-cross detection
- **FreeRTOS:** Task scheduling, inter-task communication
- **NVS:** Configuration persistence

### 5.2 Backend Services
- RESTful API (Google Apps Script)
- Data persistence (Google Sheets)
- User authentication
- Web app delivery

### 5.3 Client Applications
- Flutter mobile app (Android/iOS)
- Web dashboard (HTML/CSS/JavaScript)
- Voice assistant skills (Alexa/Google)

## 6. Performance Specifications

| Parameter | Specification |
|-----------|--------------|
| Control latency (local) | <100ms |
| Control latency (cloud) | <5s (polling interval) |
| WebSocket update rate | Real-time (<100ms) |
| Zero-cross accuracy | ±50µs |
| Phase control resolution | 100 steps (10ms half-cycle) |

## 7. Compatibility

### 7.1 Load Types
- Resistive: Full capacity (4A)
- Inductive: Derated 50% (2A)
- Capacitive: Derated 50% (2A)
- LED: Compatible with dimmable types

### 7.2 Network Compatibility
- WiFi standards: 802.11 b/g/n
- Frequency: 2.4GHz only (5GHz not supported)
- Security: WPA2-PSK required

## References

1. ESP32 Technical Reference Manual
2. MOC3041 Optocoupler Datasheet
3. IEC 60950-1 Safety Standard

---

**Document Control:** SYS-FEAT-001 Rev 3.1
