# 🔬 Mini Smart Home Controller - Testing Version

> A simplified ESP32 firmware for testing connectivity between the hardware, cloud dashboard, and mobile app using an LED instead of AC control.

## ⚡ Quick Links

- **[⚡ 5-Minute Quick Start](./QUICKSTART.md)** ← Start here if you're in a hurry!
- **[📐 Wiring Guide](./WIRING.md)** - Detailed connection instructions
- **[🧪 Testing Guide](./TESTING.md)** - Complete validation procedures

## 🎯 Purpose

This mini controller is designed to:
- **Test** the complete system integration without dealing with high-voltage AC
- **Verify** bidirectional communication between ESP32 ↔ Cloud ↔ Dashboard ↔ App
- **Validate** the API endpoints and WebSocket connections
- **Learn** how the system works before deploying the full production firmware

## ✨ Features

- ✅ Simple LED on/off control (replaces AC TRIAC control)
- ✅ PWM brightness control (0-100%)
- ✅ WiFi connectivity with captive portal setup
- ✅ Google Apps Script cloud integration
- ✅ Local REST API (port 8080)
- ✅ WebSocket real-time updates (port 81)
- ✅ Compatible with existing dashboard and Flutter app
- ✅ Runtime statistics tracking
- ✅ Signal strength monitoring

## 🔧 Hardware Requirements

### Required Components
- **ESP32 Development Board** (any variant with WiFi)
- **1x LED** (any color, 3mm or 5mm)
- **1x 220Ω Resistor** (for current limiting)
- **Breadboard** (optional, for easy connections)
- **Jumper wires**

### Wiring Diagram

```
ESP32                    LED
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
GPIO 2 ──────┤220Ω├──────►|──── GND
              Resistor    LED
```

**Simple Wiring Steps:**
1. Connect the **positive leg** (longer leg) of the LED to GPIO 2 through the 220Ω resistor
2. Connect the **negative leg** (shorter leg) of the LED to GND
3. That's it! 

**Note:** Most ESP32 boards have a built-in LED on GPIO 2, so you can test without any external LED first!

## 📦 Software Requirements

### Arduino IDE Setup

1. **Install Arduino IDE**
   - Download from: https://www.arduino.cc/en/software
   - Install version 2.0 or later

2. **Install ESP32 Board Support**
   - Open Arduino IDE
   - Go to `File` → `Preferences`
   - Add to "Additional Board Manager URLs":
     ```
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
     ```
   - Go to `Tools` → `Board` → `Boards Manager`
   - Search for "ESP32" and install "ESP32 by Espressif Systems"

3. **Install Required Libraries**
   - Go to `Tools` → `Manage Libraries`
   - Install the following libraries:
     - **ArduinoJson** by Benoit Blanchon (v6.x)
     - **WiFiManager** by tzapu
     - **WebSockets** by Markus Sattler

## 🚀 Quick Start Guide

### Step 1: Upload Firmware

1. **Open the sketch**
   ```
   File → Open → Navigate to firmware/mini-controller/mini-controller.ino
   ```

2. **Configure Board Settings**
   - Board: "ESP32 Dev Module" (or your specific board)
   - Upload Speed: 115200
   - Flash Frequency: 80MHz
   - Port: Select your ESP32's COM port

3. **Upload**
   - Click the Upload button (→) 
   - Wait for compilation and upload to complete

### Step 2: Configure WiFi

1. **Enter Setup Mode**
   - After upload, the ESP32 will create a WiFi access point
   - Look for network named: **"Mini_Controller"**

2. **Connect and Configure**
   - Connect your phone/laptop to "Mini_Controller" WiFi
   - A captive portal will open automatically (if not, go to http://192.168.4.1)
   - Select your home WiFi network
   - Enter your WiFi password
   - Click "Save"

3. **Verify Connection**
   - Open Serial Monitor (115200 baud)
   - You should see:
     ```
     [INFO] WiFi connected: 192.168.x.x
     [INFO] mDNS started: mini_controller.local
     [INFO] System ready - LED connected to GPIO 2
     ```

### Step 3: Test with Dashboard

1. **Open the Cloud Dashboard**
   - Go to: https://script.google.com/macros/s/[YOUR_SCRIPT_ID]/exec
   - (Or use the URL from the main project README)

2. **Find Your Device**
   - Your mini controller should appear in the devices list
   - Device name: "Test LED"
   - You should see the device status (Online/Offline)

3. **Control the LED**
   - Toggle the switch to turn LED ON/OFF
   - Adjust brightness slider to control LED intensity
   - Watch the LED respond in real-time!

### Step 4: Test with Flutter App

1. **Open the Mobile App**
   - Launch the Smart Home Controller app
   - Login with your credentials

2. **Control from App**
   - Navigate to Devices page
   - Find "Test LED" device
   - Toggle switch or adjust brightness
   - Changes should reflect on both app and dashboard instantly

## 🔌 API Endpoints

The mini controller exposes the same API as the full system:

### Local API (Direct Connection)

**Base URL:** `http://[ESP32_IP]:8080`

#### Get Device Status
```http
GET /status
```

**Response:**
```json
{
  "devices": [
    {
      "id": 0,
      "name": "Test LED",
      "type": 0,
      "state": true,
      "brightness": 75,
      "runtime": 1234,
      "locked": false
    }
  ]
}
```

#### Control Device
```http
POST /control
Content-Type: application/json

{
  "id": 0,
  "state": true,
  "brightness": 80
}
```

**Response:**
```json
{
  "success": true
}
```

#### Get System Info
```http
GET /info
```

**Response:**
```json
{
  "name": "Mini_Controller",
  "firmware": "1.0",
  "build": "MINI-001",
  "mac": "AA:BB:CC:DD:EE:FF",
  "ip": "192.168.1.100",
  "uptime": 3600,
  "rssi": -45,
  "signal": "Excellent",
  "heap": 250000,
  "cloud_connected": true
}
```

### WebSocket (Real-time Updates)

**URL:** `ws://[ESP32_IP]:81`

**Events:**
```json
{
  "type": "device_update",
  "id": 0,
  "state": true,
  "brightness": 75,
  "name": "Test LED"
}
```

## 🧪 Testing Scenarios

### Test 1: Dashboard Control
1. Open dashboard in browser
2. Toggle LED switch ON
3. Verify LED turns on
4. Adjust brightness slider
5. Verify LED brightness changes

**Expected Result:** LED responds to all dashboard commands in real-time

### Test 2: App Control
1. Open mobile app
2. Toggle LED switch OFF
3. Verify LED turns off
4. Check dashboard updates automatically

**Expected Result:** App controls LED, dashboard shows updated state

### Test 3: Bidirectional Sync
1. Open both dashboard and app
2. Control LED from dashboard
3. Verify app updates automatically
4. Control LED from app
5. Verify dashboard updates automatically

**Expected Result:** Both interfaces stay in sync

### Test 4: Direct API Control
1. Find ESP32 IP from Serial Monitor
2. Use curl or Postman to send command:
   ```bash
   curl -X POST http://192.168.1.100:8080/control \
     -H "Content-Type: application/json" \
     -d '{"id":0,"state":true,"brightness":50}'
   ```
3. Verify LED turns on at 50% brightness

**Expected Result:** LED responds to direct API calls

## 🔍 Troubleshooting

### LED Not Responding

**Problem:** LED doesn't turn on/off
- Check wiring connections
- Verify GPIO 2 is correct for your board
- Check Serial Monitor for errors
- Test with built-in LED first

### WiFi Connection Failed

**Problem:** Can't connect to WiFi
- Check WiFi credentials
- Verify 2.4GHz network (ESP32 doesn't support 5GHz)
- Move closer to router
- Try factory reset (hold boot button during power-up)

### Dashboard Not Showing Device

**Problem:** Device not appearing in dashboard
- Check Serial Monitor for "Cloud sync failed" errors
- Verify GOOGLE_SCRIPT_URL in code matches your deployment
- Check internet connectivity
- Wait 5-10 seconds for device to register

### WebSocket Not Connecting

**Problem:** Real-time updates not working
- Check firewall settings
- Verify port 81 is accessible
- Check browser console for WebSocket errors
- Try using IP address instead of hostname

## 📊 Serial Monitor Output

Normal operation looks like this:

```
========================================
Mini Smart Home Controller v1.0
Build: MINI-001
========================================
[INFO] WiFi connected: 192.168.1.100
[INFO] Signal strength: Excellent (-42 dBm)
[INFO] mDNS started: mini_controller.local
[INFO] Redirect server started on port 80
[INFO] Local API server started on port 8080
[INFO] WebSocket server started on port 81
========================================
[INFO] System ready - LED connected to GPIO 2
[INFO] Free heap: 267532 bytes
========================================
[INFO] WebSocket client #0 connected
[INFO] LED ON - Brightness: 75%
[INFO] LED OFF
```

## 🎓 Understanding the Code

### Key Differences from Production Firmware

| Feature | Production | Mini Controller |
|---------|-----------|-----------------|
| Output Control | TRIAC phase control | Simple PWM LED |
| Channels | 4 channels | 1 channel (LED) |
| Complexity | Multi-core FreeRTOS | Single-threaded |
| Voice Control | Alexa + Google | None (not needed) |
| Safety Features | Zero-cross detection | None (safe LED) |
| Schedules/Scenes | Full support | Not implemented |

### Architecture

```
┌─────────────────┐
│   Flutter App   │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│ Cloud Backend   │ ◄─── Google Apps Script
│  (REST API)     │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│   Dashboard     │
└────────┬────────┘
         │
    ┌────┴────┐
    │         │
    ▼         ▼
[REST]    [WebSocket]
    │         │
    └────┬────┘
         ▼
┌─────────────────┐
│  ESP32 + LED    │ ◄─── Mini Controller
└─────────────────┘
```

## 🔄 Next Steps

After successfully testing the mini controller:

1. **Understand the API Flow**
   - Study how device states are synced
   - Learn the JSON message format
   - Observe timing and polling intervals

2. **Customize for Your Needs**
   - Change LED pin if needed
   - Modify device name
   - Adjust polling interval

3. **Progress to Full System**
   - Move to production firmware in `/firmware/main/`
   - Understand TRIAC control
   - Learn about safety features

4. **Build Additional Features**
   - Add more LEDs for multi-channel testing
   - Implement schedules
   - Add scenes support

## 📚 Additional Resources

- **Main Project README:** [../README.md](../../README.md)
- **Full Firmware:** [../main/main.ino](../main/main.ino)
- **Backend Documentation:** [../../backend/google-apps-script/README.md](../../backend/google-apps-script/README.md)
- **App Documentation:** [../../frontend/flutter project/demo/README.md](../../frontend/flutter%20project/demo/README.md)

## 🐛 Known Limitations

- Only one device (LED) supported
- No schedules or scenes
- No voice control
- No OTA updates
- Basic error handling

These are intentional simplifications for testing purposes.

## 💡 Tips

- **Start Simple:** Test with built-in LED before adding external components
- **Monitor Serial:** Keep Serial Monitor open to see what's happening
- **Check IP:** Note the ESP32 IP address for direct API testing
- **Use mDNS:** Access via `mini_controller.local` instead of IP (on same network)
- **Test Incrementally:** Verify WiFi → Cloud → Dashboard → App in order

## 📝 License

Same as main project - MIT License

---

**Happy Testing! 🚀**

If you encounter any issues or have questions, check the main project documentation or open an issue on GitHub.
