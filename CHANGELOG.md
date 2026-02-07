# Changelog

## v3.1 - Repository Reorganization (December 2024)

- Renamed `software_Team/` to `frontend/` and `hardware/PCB_Manufacturing/` to `hardware/pcb/`
- Removed redundant documentation files from root directory
- Updated all documentation references to reflect new folder structure

## v3.0 - Firmware Rewrite (December 2024)

### Architecture
- Dual-core FreeRTOS design: time-critical TRIAC control on Core 1, networking on Core 0
- Thread-safe inter-core communication via FreeRTOS queues and mutexes

### New Features
- Amazon Alexa voice control via Espalexa (local network)
- SinricPro integration scaffolded for Google Assistant (requires library installation)
- Local REST API on port 8080 (status, control, info, schedules, scenes)
- WebSocket server on port 81 for real-time state sync
- Scene and schedule automation with persistent storage
- Smooth fade transitions for dimmers and fans
- OTA firmware updates with rollback protection
- Enhanced Google Apps Script cloud polling protocol

### Safety and Reliability
- Watchdog timers (15 s Core 0, 5 s Core 1)
- Zero-cross monitoring with auto-shutoff
- Configurable per-device auto-off timer
- Heap monitoring and brownout protection

### Code Quality
- Modular header files (config.h, api.h, voice.h, etc.)
- Structured logging with configurable levels
- Input validation on all API endpoints
- Comprehensive error handling

## v2.1 - Initial Release

- Basic 4-channel TRIAC phase-angle control
- Alexa integration
- Google Apps Script cloud polling
- WiFi management with captive portal
