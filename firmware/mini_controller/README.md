# Mini LED Controller for ESP32

A simplified firmware for controlling the ESP32's built-in LED remotely through a web dashboard and REST API.

## Features

✅ **WiFi Connectivity** - Automatic connection with captive portal  
✅ **PWM Brightness Control** - 0-100% brightness control  
✅ **REST API** - Full REST API for LED control  
✅ **WebSocket** - Real-time updates to connected clients  
✅ **Web Dashboard** - Beautiful built-in web interface  
✅ **Cloud Integration** - Sync with Google Apps Script backend  
✅ **mDNS Support** - Access via `mini-led.local`  

## Hardware Requirements

- **ESP32 DevKit** or any compatible ESP32 board
- Built-in LED (typically connected to GPIO 2)

## Pin Configuration

| Component | GPIO Pin | Notes |
|-----------|----------|-------|
| Built-in LED | 2 | Default on most ESP32 boards |

> **Note:** Some ESP32 boards use different pins for the built-in LED. Check your board's documentation if GPIO 2 doesn't work. Common alternatives are GPIO 5, GPIO 16, or GPIO 22.

## Installation

### Using Arduino IDE

1. **Install ESP32 Board Support:**
   - Open Arduino IDE
   - Go to `File` → `Preferences`
   - Add this URL to "Additional Board Manager URLs":
     ```
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
     ```
   - Go to `Tools` → `Board` → `Board Manager`
   - Search for "ESP32" and install "esp32 by Espressif Systems"

2. **Install Required Libraries:**
   - Go to `Sketch` → `Include Library` → `Manage Libraries`
   - Install the following libraries:
     - **WiFiManager** by tzapu (v2.0.16+)
     - **ArduinoJson** by Benoit Blanchon (v6.x)
     - **WebSockets** by Markus Sattler (v2.4.0+)

3. **Configure the Firmware:**
   - Open `firmware/mini_controller/mini_controller.ino`
   - Update the Google Apps Script URL (optional):
     ```cpp
     String GOOGLE_SCRIPT_URL = "your_google_script_url_here";
     ```
   - If your board uses a different LED pin, update:
     ```cpp
     #define LED_PIN 2  // Change to your board's LED pin
     ```

4. **Select Board and Upload:**
   - Go to `Tools` → `Board` → `ESP32 Arduino` → `ESP32 Dev Module`
   - Select the correct COM port under `Tools` → `Port`
   - Click `Upload` button

### Using PlatformIO

Create a `platformio.ini` file:

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200

lib_deps = 
    tzapu/WiFiManager@^2.0.16
    bblanchon/ArduinoJson@^6.21.3
    links2004/WebSockets@^2.4.0

upload_speed = 921600
monitor_filters = esp32_exception_decoder
```

Then run:
```bash
pio run --target upload
pio device monitor
```

## First Time Setup

1. **Power on the ESP32**
   - The device will create a WiFi access point named `Mini_LED_Controller`

2. **Connect to the WiFi network**
   - From your phone or laptop, connect to `Mini_LED_Controller`
   - A captive portal should open automatically
   - If not, open your browser and go to `192.168.4.1`

3. **Configure WiFi**
   - Select your home WiFi network
   - Enter the password
   - Click "Save"

4. **Access the Web Interface**
   - Once connected, the serial monitor will show the device's IP address
   - Open a web browser and navigate to the IP address (e.g., `http://192.168.1.100:8080`)
   - Or use mDNS: `http://mini-led.local:8080`

## Usage

### Web Dashboard

Access the built-in web dashboard by opening your browser to:
- `http://<ESP32-IP-Address>:8080`
- Or via mDNS: `http://mini-led.local:8080`

The dashboard provides:
- **LED Status Indicator** - Visual feedback of LED state
- **Toggle Button** - Turn LED on/off
- **Brightness Slider** - Adjust brightness from 0-100%
- **System Status** - IP address, WiFi signal strength
- **Real-time Updates** - Automatic sync via WebSocket

### REST API

The mini controller provides a REST API on port 8080:

#### Get Device Status
```bash
GET http://<IP>:8080/status
```

Response:
```json
{
  "devices": [
    {
      "id": 0,
      "name": "Built-in LED",
      "type": "LED",
      "state": true,
      "brightness": 75,
      "runtime": 3600
    }
  ]
}
```

#### Control LED
```bash
POST http://<IP>:8080/control
Content-Type: application/json

{
  "state": true,
  "brightness": 75
}
```

Response:
```json
{
  "success": true,
  "state": true,
  "brightness": 75
}
```

#### Get System Information
```bash
GET http://<IP>:8080/info
```

Response:
```json
{
  "name": "Mini_LED_Controller",
  "device_type": "mini_controller",
  "firmware": "1.0",
  "mac": "AA:BB:CC:DD:EE:FF",
  "ip": "192.168.1.100",
  "uptime": 3600,
  "rssi": -45,
  "heap": 280000,
  "cloud_connected": true
}
```

#### Update Configuration
```bash
POST http://<IP>:8080/config
Content-Type: application/json

{
  "name": "Office LED"
}
```

### WebSocket API

Connect to `ws://<IP>:81` for real-time updates.

**Receive LED state updates:**
```json
{
  "type": "device_update",
  "id": 0,
  "state": true,
  "brightness": 75,
  "name": "Built-in LED"
}
```

**Send control commands:**
```json
{
  "action": "control",
  "state": true,
  "brightness": 50
}
```

**Request status:**
```json
{
  "action": "get_status"
}
```

### Cloud Integration

The mini controller can sync with the main Smart Home Controller's Google Apps Script backend.

**Configure the URL:**
```cpp
String GOOGLE_SCRIPT_URL = "https://script.google.com/macros/s/YOUR_SCRIPT_ID/exec";
```

**Cloud Features:**
- Automatic state synchronization every 5 seconds
- Remote control from the main dashboard
- Runtime statistics tracking
- Device name management

## Factory Reset

If you need to reconfigure WiFi:

1. Hold the `BOOT` button on the ESP32 for 10 seconds during startup
2. Or upload the firmware again with WiFiManager credentials cleared:
   ```cpp
   // Add at the start of setup()
   WiFiManager wm;
   wm.resetSettings();
   ```

## Troubleshooting

### LED Not Working

1. **Check the LED Pin:**
   - Different ESP32 boards use different pins
   - Try changing `LED_PIN` to 5, 16, or 22
   - Check your board's pinout diagram

2. **Test with Basic Blink:**
   ```cpp
   void setup() {
     pinMode(LED_PIN, OUTPUT);
   }
   void loop() {
     digitalWrite(LED_PIN, HIGH);
     delay(1000);
     digitalWrite(LED_PIN, LOW);
     delay(1000);
   }
   ```

### Can't Connect to WiFi

1. **Check Serial Monitor:**
   - Open Serial Monitor at 115200 baud
   - Look for error messages

2. **Ensure 2.4GHz WiFi:**
   - ESP32 only supports 2.4GHz networks
   - 5GHz networks won't work

3. **Factory Reset:**
   - Re-upload firmware with `wm.resetSettings()` added

### Web Interface Not Loading

1. **Check IP Address:**
   - Look in Serial Monitor for the assigned IP
   - Try accessing via mDNS: `http://mini-led.local:8080`

2. **Firewall Issues:**
   - Ensure your firewall allows connections on ports 8080 and 81
   - Try from a device on the same network

### WebSocket Not Connecting

1. **Check Browser Console:**
   - Open Developer Tools (F12)
   - Look for WebSocket errors

2. **Port Availability:**
   - Ensure port 81 is not blocked
   - Check if another service is using port 81

## Serial Monitor Output

Expected output on successful startup:

```
========================================
Mini LED Controller v1.0
========================================
[INFO] Device MAC: AA:BB:CC:DD:EE:FF
[INFO] WiFi connected: 192.168.1.100
[INFO] Signal strength: -45 dBm
[INFO] mDNS started: mini-led.local
[INFO] Redirect server started on port 80
[INFO] REST API server started on port 8080
[INFO] WebSocket server started on port 81
========================================
[INFO] System initialization complete
[INFO] Free heap: 280000 bytes
========================================

[INFO] Access web interface at: http://192.168.1.100:8080
```

## Integration with Main Dashboard

The mini controller uses the same API format as the main controller and can be integrated with the existing Google Apps Script dashboard.

**Backend Integration Steps:**

1. Add mini controller to your device registry
2. Use the `device_type: "mini_controller"` field to identify it
3. The LED will appear as Device 1 (`d1`) in cloud sync
4. Control it through the main dashboard alongside other devices

## Technical Specifications

| Specification | Value |
|---------------|-------|
| Firmware Version | 1.0 |
| Platform | ESP32 |
| PWM Frequency | 5 kHz |
| PWM Resolution | 8-bit (0-255) |
| Brightness Levels | 101 (0-100%) |
| WiFi Standard | 802.11 b/g/n (2.4GHz) |
| REST API Port | 8080 |
| WebSocket Port | 81 |
| Cloud Sync Interval | 5 seconds |
| Memory Usage | ~50KB program, ~20KB RAM |

## Development

### Modify LED Pin

If your board uses a different LED pin:

```cpp
#define LED_PIN 5  // Change to your board's LED pin
```

### Adjust PWM Settings

For different LED characteristics:

```cpp
#define PWM_FREQUENCY 5000  // Adjust frequency (Hz)
#define PWM_RESOLUTION 8    // 8-bit = 0-255, 10-bit = 0-1023
```

### Change Device Name

```cpp
#define DEVICE_NAME "My_LED_Controller"
```

### Debug Mode

Enable verbose logging:

```cpp
// Add at the start of setup()
Serial.setDebugOutput(true);
```

## License

MIT License - Part of the Smart Home Controller Project

## Support

- **Main Project**: https://github.com/Gokul3611/smarthome-controller
- **Issues**: https://github.com/Gokul3611/smarthome-controller/issues
- **Documentation**: See main project README.md

## Related Documentation

- [Main Controller README](../../README.md)
- [Hardware Guide](../../hardware/README.md)
- [Backend Guide](../../backend/google-apps-script/README.md)
- [Deployment Guide](../../DEPLOYMENT.md)

---

**Built with ❤️ as part of the Smart Home Controller Project**
