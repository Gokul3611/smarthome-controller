# Features

Implemented features across all system components.

## Mobile App (Flutter)

### Authentication
- User registration and login with email/password
- Password hashing (SHA-256)
- Session management with tokens
- Profile management
- Forgot password flow

### Device Control
- Real-time device listing with status indicators
- Power toggle (on/off)
- Fan speed control (0-100%)
- Light brightness control (0-100%)
- Device renaming and type selection (Light/Fan)
- Dynamic UI (fan controls appear only for fan-type devices)
- Up to 4 channels per ESP32

### Automation
- Schedule creation with alarm-style interface
- Day-of-week selection
- Multiple schedules per device
- Enable/disable and delete schedules

### Scenes
- Create named device-combination presets
- Quick scene activation
- Scene editing and deletion

### UI
- Dark theme with glassmorphism effects
- Smooth animations and responsive layout
- Material Design 3

## Web Dashboard

### Device Management
- Real-time device grid view with auto-refresh (3 s)
- Power control and fan speed sliders
- Inline device renaming
- Device type selection
- Online/offline status indicators

### Scheduling and Scenes
- Visual schedule list with add/edit/delete
- Time picker and day selection
- Scene management with quick activation

### Settings
- WiFi reset for devices
- Device configuration
- Tabbed navigation (Devices, Schedules, Scenes)
- Responsive design
- Hosted on Google Apps Script (HTTPS, no server maintenance)

## Backend (Google Apps Script)

### API
- RESTful JSON endpoints for device polling, registration, state updates
- Device list retrieval and configuration updates
- Schedule and scene CRUD
- User authentication
- WiFi reset commands
- API key authentication
- Command queuing system

### Database (Google Sheets)
- Devices, DeviceStates, Schedules, Scenes, Users, Commands sheets
- Auto-initialization of database structure
- Device online/offline detection
- Automatic device registration on first poll

## ESP32 Firmware

### Hardware Control
- 4-channel TRIAC phase-angle control
- Zero-cross detection with microsecond timing
- Dual-core FreeRTOS architecture
- Physical button inputs (4 switches) with debouncing
- Smooth fade transitions between brightness levels

### Networking
- WiFi with auto-reconnect and captive portal setup
- REST API server (ports 80, 8080)
- WebSocket server (port 81) for real-time state sync
- Google Apps Script cloud polling (2.5 s interval)
- OTA firmware updates with rollback protection

### Voice Control
- Amazon Alexa integration (local network, via Espalexa)
- Device discovery and on/off/brightness commands

### Safety
- Watchdog timers (15 s Core 0, 5 s Core 1)
- Zero-cross monitoring with auto-shutoff
- Configurable auto-off timer per device (default 1 hour)
- ESP32 brownout protection
- Heap monitoring

### Persistence
- EEPROM-based state storage across reboots
- NTP time synchronization for schedule execution

## Not Yet Implemented

- Google Assistant integration via SinricPro (scaffolded, not wired up)
- Energy monitoring / power consumption tracking
- Multi-user device sharing
- iOS app distribution (Flutter code exists, not tested on iOS)
