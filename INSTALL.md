# Installation & Setup Guide

Complete guide for installing and configuring the Smart Home Controller firmware.

## Table of Contents

1. [Prerequisites](#prerequisites)
2. [Hardware Setup](#hardware-setup)
3. [Software Installation](#software-installation)
4. [Firmware Upload](#firmware-upload)
5. [Initial Configuration](#initial-configuration)
6. [Google Apps Script Setup](#google-apps-script-setup)
7. [Voice Assistant Setup](#voice-assistant-setup)
8. [Flutter App Integration](#flutter-app-integration)
9. [Troubleshooting](#troubleshooting)

## Prerequisites

### Hardware Requirements

- ESP32 development board (WROOM-32 or compatible)
- 4-channel TRIAC controller board
- Zero-cross detection circuit
- 5V power supply (2A minimum)
- USB cable for programming

### Software Requirements

- Arduino IDE 2.x or PlatformIO
- USB-to-UART drivers for ESP32
- Google Apps Script account (free)
- WiFi network (2.4GHz)

## Hardware Setup

### Pin Connections

Connect your ESP32 to the TRIAC controller as follows:

| ESP32 Pin | Function | Connection |
|-----------|----------|------------|
| GPIO 13 | Zero-Cross Input | Connect to ZC detector output |
| GPIO 16 | TRIAC 1 Control | Connect to TRIAC 1 optocoupler |
| GPIO 17 | TRIAC 2 Control | Connect to TRIAC 2 optocoupler |
| GPIO 18 | TRIAC 3 Control | Connect to TRIAC 3 optocoupler |
| GPIO 19 | TRIAC 4 Control | Connect to TRIAC 4 optocoupler |
| GPIO 32 | Switch 1 | Physical button with pull-up |
| GPIO 33 | Switch 2 | Physical button with pull-up |
| GPIO 25 | Switch 3 | Physical button with pull-up |
| GPIO 26 | Switch 4 | Physical button with pull-up |
| 5V | Power | 5V power supply |
| GND | Ground | Common ground |

### Safety Warnings

⚠️ **HIGH VOLTAGE WARNING** ⚠️

- TRIAC circuits handle mains voltage (110-240V AC)
- Always disconnect power before working on circuits
- Use proper isolation and safety measures
- Have a qualified electrician review your setup
- Test with low voltage loads first

## Software Installation

### Option 1: Arduino IDE (Recommended for beginners)

1. **Install Arduino IDE**
   - Download from [arduino.cc](https://www.arduino.cc/en/software)
   - Install version 2.0 or later

2. **Add ESP32 Board Support**
   - Open Arduino IDE
   - Go to File → Preferences
   - Add to "Additional Board Manager URLs":
     ```
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
     ```
   - Go to Tools → Board → Boards Manager
   - Search for "esp32"
   - Install "esp32 by Espressif Systems" (version 2.0.x or later)

3. **Install Required Libraries**
   - Go to Tools → Manage Libraries
   - Install the following libraries:
     - **WiFiManager** by tzapu (v2.0.16 or later)
     - **ArduinoJson** by Benoit Blanchon (v6.x - NOT v7)
     - **Espalexa** by Christian Schwinne (v2.7.0 or later)
     - **WebSockets** by Markus Sattler (v2.4.0 or later)
     - **SinricPro** (optional, for Google Assistant)

### Option 2: PlatformIO (Recommended for advanced users)

1. **Install VS Code**
   - Download from [code.visualstudio.com](https://code.visualstudio.com/)

2. **Install PlatformIO Extension**
   - Open VS Code
   - Go to Extensions (Ctrl+Shift+X)
   - Search for "PlatformIO IDE"
   - Click Install

3. **Create Project**
   ```bash
   cd smarthome-controller/firmware
   pio init -b esp32dev
   ```

4. **Configure platformio.ini**
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
   ```

## Firmware Upload

### Using Arduino IDE

1. **Open the Firmware**
   - Navigate to `smarthome-controller/firmware/main/`
   - Open `main.ino` in Arduino IDE

2. **Configure Board Settings**
   - Tools → Board → ESP32 Arduino → ESP32 Dev Module
   - Tools → Flash Size → 4MB (32Mb)
   - Tools → Partition Scheme → Default 4MB with spiffs
   - Tools → Upload Speed → 921600
   - Tools → CPU Frequency → 240MHz

3. **Configure Google Apps Script URL** (Before Upload)
   - Open `main.ino`
   - Find the line:
     ```cpp
     String GOOGLE_SCRIPT_URL = "your_url_here";
     ```
   - Replace with your Google Apps Script URL (see section below)

4. **Upload**
   - Connect ESP32 via USB
   - Select the correct COM port (Tools → Port)
   - Click Upload button (→)
   - Wait for upload to complete

### Using PlatformIO

```bash
# Build firmware
pio run

# Upload firmware
pio run --target upload

# Monitor serial output
pio device monitor
```

## Initial Configuration

### First Boot Setup

1. **Power On**
   - Power the ESP32
   - Wait 10-15 seconds for boot

2. **Connect to WiFi Portal**
   - On your phone/computer, connect to WiFi network: `Smart_Home_Hub`
   - Browser should automatically open to configuration page
   - If not, navigate to: `http://192.168.4.1`

3. **Configure WiFi**
   - Click "Scan networks"
   - Select your WiFi network from the list
   - Enter WiFi password
   - Click "Save & Connect"
   - Device will reboot and connect to your network

4. **Find Device IP**
   - Check your router's DHCP client list
   - Or use mDNS: `http://smart-home-hub.local` (Mac/Linux)
   - Or check Serial Monitor at 115200 baud for IP address

### Factory Reset

If you need to reset the device:

1. Hold Switch 1 (GPIO 32) for 10 seconds
2. Device will erase all settings and reboot
3. Follow "First Boot Setup" again

## Google Apps Script Setup

### Create Apps Script

1. **Open Google Apps Script**
   - Go to [script.google.com](https://script.google.com)
   - Click "New Project"

2. **Add Script Code**

```javascript
function doPost(e) {
  try {
    var data = JSON.parse(e.postData.contents);
    
    // Log incoming data
    Logger.log('Received: ' + JSON.stringify(data));
    
    // Store device state in Properties Service
    var props = PropertiesService.getScriptProperties();
    props.setProperty(data.uid, JSON.stringify(data));
    
    // Prepare response
    var response = {
      sys_name: "Living Room Hub",
      ota_update: false,
      bin_url: "",
      d1: {
        type: "FAN",
        name: "Ceiling Fan",
        lock: false,
        update: false,
        state: 0,
        val: 100
      },
      d2: {
        type: "SWITCH",
        name: "Light",
        lock: false,
        update: false,
        state: 0,
        val: 100
      },
      d3: {
        type: "DIMMER",
        name: "Lamp",
        lock: false,
        update: false,
        state: 0,
        val: 100
      },
      d4: {
        type: "SWITCH",
        name: "Outlet",
        lock: false,
        update: false,
        state: 0,
        val: 100
      }
    };
    
    return ContentService.createTextOutput(JSON.stringify(response))
      .setMimeType(ContentService.MimeType.JSON);
      
  } catch (error) {
    Logger.log('Error: ' + error.toString());
    return ContentService.createTextOutput(JSON.stringify({error: error.toString()}))
      .setMimeType(ContentService.MimeType.JSON);
  }
}

function doGet(e) {
  return HtmlService.createHtmlOutput('<h1>Smart Home Controller API</h1>');
}
```

3. **Deploy**
   - Click Deploy → New deployment
   - Type: Web app
   - Execute as: Me
   - Who has access: Anyone
   - Click Deploy
   - Copy the Web app URL

4. **Update Firmware**
   - Paste the URL into `GOOGLE_SCRIPT_URL` in `main.ino`
   - Re-upload firmware OR
   - Update via OTA (if already running)

## Voice Assistant Setup

### Amazon Alexa

1. **Ensure Same Network**
   - ESP32 and Alexa device must be on same WiFi network

2. **Discover Devices**
   - Open Alexa app
   - Go to Devices → Add Device
   - Select "Other"
   - Click "Discover Devices"
   - Wait for discovery (30-45 seconds)

3. **Control Devices**
   - "Alexa, turn on Device 1"
   - "Alexa, set Ceiling Fan to 50%"
   - "Alexa, turn off all devices"

### Google Assistant (via SinricPro)

1. **Create SinricPro Account**
   - Go to [sinric.pro](https://sinric.pro)
   - Sign up for free account

2. **Add Devices**
   - Dashboard → Devices → Add Device
   - Select device type (Switch, Dimmer, Fan, etc.)
   - Create 4 devices for your 4 channels
   - Copy Device IDs

3. **Get Credentials**
   - Go to Credentials
   - Copy App Key and App Secret

4. **Configure in Google Apps Script**
   - Update your Apps Script to return:
     ```javascript
     {
       sinric_key: "your_app_key",
       sinric_secret: "your_app_secret"
     }
     ```

5. **Link Google Assistant**
   - Open Google Home app
   - Add device → Works with Google
   - Search for "Smart Home"
   - Link your SinricPro account
   - Discover devices

6. **Control via Google**
   - "Hey Google, turn on the fan"
   - "Hey Google, set living room light to 30%"

## Flutter App Integration

### Local Control API

The device exposes a REST API on port 8080:

```bash
# Get device status
curl http://192.168.1.100:8080/status

# Control device
curl -X POST http://192.168.1.100:8080/control \
  -H "Content-Type: application/json" \
  -d '{"id": 0, "state": true, "brightness": 75}'

# Get system info
curl http://192.168.1.100:8080/info
```

### WebSocket Real-Time Updates

Connect to `ws://192.168.1.100:81` for real-time device state updates.

Example (JavaScript):
```javascript
const ws = new WebSocket('ws://192.168.1.100:81');

ws.onmessage = (event) => {
  const data = JSON.parse(event.data);
  console.log('Device update:', data);
};
```

## Troubleshooting

### Device Won't Boot

- Check power supply (5V, 2A minimum)
- Check USB cable and connections
- Try factory reset
- Re-upload firmware with "Erase All Flash" option

### WiFi Won't Connect

- Ensure 2.4GHz network (5GHz not supported)
- Check signal strength (need >-80 dBm)
- Try factory reset and reconfigure
- Check router MAC filtering / DHCP settings

### Voice Control Not Working

**Alexa:**
- Ensure same WiFi network
- Restart Alexa device
- Delete and rediscover devices
- Check mDNS is working

**Google Assistant:**
- Verify SinricPro credentials
- Check cloud connection status
- Unlink and relink account
- Check SinricPro dashboard for device status

### OTA Update Failed

- Ensure stable internet connection
- Check firmware binary URL is accessible
- Verify sufficient flash space (need >1MB free)
- Try upload via USB instead

### TRIAC Control Not Working

- Verify zero-cross signal with oscilloscope
- Check optocoupler connections
- Test with multimeter (AC voltage present)
- Ensure proper TRIAC ratings for load
- Check for loose connections

### Serial Monitor Shows Errors

Common error messages:

- `Zero-cross signal lost`: Check ZC detector circuit
- `WiFi disconnected`: Check network stability
- `Cloud sync failed`: Check Google Apps Script URL
- `Memory low`: Reduce LOG_LEVEL or restart device

## Advanced Configuration

### Custom Configuration

Edit `config.h` to customize:

- Pin assignments
- Timing parameters
- Safety timeouts
- Log levels
- Buffer sizes

### Compile-Time Options

Add to Arduino IDE or `platformio.ini`:

```cpp
// Enable verbose debugging
-DLOG_LEVEL=LOG_DEBUG

// Increase stack sizes
-DCONFIG_ARDUINO_LOOP_STACK_SIZE=16384

// Enable PSRAM (if available)
-DBOARD_HAS_PSRAM
```

## Support

- GitHub Issues: [github.com/Gokul3611/smarthome-controller/issues](https://github.com/Gokul3611/smarthome-controller/issues)
- Documentation: See README.md
- Community: Check Wiki for user guides

---

**Last Updated:** December 2024  
**Firmware Version:** 3.0
