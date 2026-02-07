# Changelog

## v3.1 - Repository Reorganization (December 2024)

- Renamed `software_Team/` to `frontend/` and `hardware/PCB_Manufacturing/` to `hardware/pcb/`
- Removed redundant documentation files from root directory
- Updated all documentation references to reflect new folder structure

---

## v3.0 - Firmware Overhaul (December 2024)

### Architecture
- Multi-core FreeRTOS design: Core 1 for time-critical TRIAC control, Core 0 for network
- Thread-safe inter-core communication via FreeRTOS queues and mutexes
- Modular code organization (separate header/impl files)

### New Features
- Amazon Alexa voice control (via Espalexa, local network)
- Google Assistant support stubbed via SinricPro (not yet functional)
- Local REST API on port 8080 with full device/schedule/scene CRUD
- WebSocket server on port 81 for real-time state updates
- Scene and schedule automation with persistent storage
- Smooth fade transitions for dimmers and fans
- OTA firmware updates with rollback protection
- Enhanced cloud protocol for Google Apps Script

### Safety & Reliability
- Dual watchdog timers (Core 0: 15s, Core 1: 5s)
- Zero-cross signal monitoring with safety shutdown
- Auto-shutoff per device (configurable, default 1 hour)
- Heap monitoring and brownout protection

### Code Quality
- Comprehensive input validation on all API endpoints
- Structured logging with configurable levels (NONE through VERBOSE)
- Named constants for all magic numbers (see `config.h`)
- ISR-safe variable access with `volatile` and `portMUX_TYPE`

### Known Limitations
- SinricPro library not included by default — requires manual installation
- Requires ESP32 Arduino Core 2.0.x or later
- Minimum 4MB flash for OTA
- WiFi 2.4GHz only

### Breaking Changes from v2.1
- New local API on port 8080 (port 80 still redirects)
- Alexa devices now use custom names from configuration
- Uses new NVS preference keys (migration is automatic)

---

## v2.1 - Initial Release

- Basic 4-channel TRIAC control
- Amazon Alexa integration
- Google Apps Script cloud polling
- WiFi management with captive portal
