# Smart Home Controller - Features

This document lists the features implemented in the Smart Home Controller system.

## Mobile App (Flutter)

### Authentication & User Management
- User registration and login with email/password
- Password hashing
- Session management with tokens
- Profile management
- Forgot password flow

### Device Control
- Real-time device listing
- Device power toggle (on/off)
- Fan speed control (0-100%)
- Light brightness control (0-100%)
- Device renaming
- Device type selection (Light/Fan)
- Dynamic UI (fan controls appear only for fans)
- Device status indicators (online/offline)
- Multiple device support (up to 4 channels per ESP32)

### Network & Connection
- WiFi configuration interface
- WiFi password reset (from app)
- Device discovery on local network
- Connection status monitoring
- Signal strength (RSSI) display
- IP address display

### Automation
- Schedule creation (alarm-style interface)
- Time-based automation
- Day-of-week selection
- Multiple schedules per device
- Enable/disable schedules
- Schedule editing and deletion

### Scenes
- Scene creation (device combinations)
- Multi-device scene support
- Quick scene activation
- Scene naming and management
- Scene editing and deletion

### System Information
- Device statistics (uptime, memory, version)
- System information page
- FAQ section

### UI/UX
- Dark theme with glassmorphism effects
- Smooth animations
- Responsive layout
- Material Design 3

---

## Web Dashboard

### Device Management
- Real-time device grid view
- Device power control
- Fan speed sliders
- Device renaming (click on device name)
- Device type selection
- Online/offline status indicators
- Auto-refresh every 3 seconds

### Scheduling
- Visual schedule list
- Add/edit/delete schedules
- Time picker interface
- Day selection (checkboxes)
- Device selection dropdown
- Schedule enable/disable

### Scenes
- Scene management interface
- Create custom scenes
- Quick activation
- Scene editing

### Deployment
- Hosted on Google Apps Script
- HTTPS by default
- No server maintenance required

---

## Backend (Google Apps Script)

### API Endpoints
- Device polling (ESP32 checks for commands)
- Device registration
- State updates (from devices)
- Device list retrieval
- Device configuration updates
- Schedule management (CRUD)
- Scene management (CRUD)
- User authentication
- WiFi reset commands

### Database (Google Sheets)
- Devices sheet (device registry)
- DeviceStates sheet (real-time states)
- Schedules sheet (automation schedules)
- Scenes sheet (scene configurations)
- Users sheet (user accounts)
- Commands sheet (pending command queue)

### Features
- RESTful API design
- JSON request/response format
- API key authentication
- Auto-initialization of database
- Command queuing system
- 2.5s polling interval
- Device online/offline detection

---

## ESP32 Firmware

### Hardware Control
- 4-channel TRIAC phase-angle control
- Zero-cross detection
- Microsecond timing precision
- Multi-core architecture (FreeRTOS)
- Physical button inputs (4 switches)

### Network Features
- WiFi connectivity with auto-reconnect
- Captive portal for initial setup
- REST API server (ports 80, 8080)
- WebSocket server (port 81)
- Google Apps Script cloud polling

### Voice Assistants
- Amazon Alexa integration (via Espalexa, local network)
- Google Assistant via SinricPro — **stubbed, not yet functional**

### Advanced Features
- OTA firmware updates with rollback protection
- Scene execution
- Schedule execution
- Smooth fade transitions
- Watchdog timers
- Safety features (auto-shutoff, zero-cross monitoring)
- Persistent storage (Preferences)

### Local API Endpoints
- GET /status — Device states
- POST /control — Device control
- GET /info — System information
- GET/POST /schedules — Schedule management
- GET/POST /scenes — Scene management
- WebSocket — Real-time updates

---

## Security

### Authentication
- Password hashing (SHA-256)
- Token-based authentication
- API key validation
- HTTPS by default (Apps Script)

### Best Practices
- Credentials stored in Google Apps Script Properties (not in code)
- Input validation on API endpoints
- Error handling on network calls
