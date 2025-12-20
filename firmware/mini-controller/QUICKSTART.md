# ⚡ Quick Start - 5 Minute Setup

Get your mini controller running in just 5 minutes!

---

## What You Need

- ✅ ESP32 board (any model)
- ✅ USB cable
- ✅ Arduino IDE installed
- ✅ WiFi network (2.4GHz)

**That's it!** Most ESP32 boards have a built-in LED, so no external components needed for your first test.

---

## Step 1: Install Arduino IDE (Skip if installed)

1. Download: https://www.arduino.cc/en/software
2. Install and open

---

## Step 2: Add ESP32 Support

1. In Arduino IDE, go to `File` → `Preferences`
2. Add this URL to "Additional Board Manager URLs":
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
3. Click OK
4. Go to `Tools` → `Board` → `Boards Manager`
5. Search "ESP32"
6. Install "ESP32 by Espressif Systems"
7. Wait for installation to complete

---

## Step 3: Install Libraries

1. Go to `Tools` → `Manage Libraries`
2. Search and install these **three** libraries:
   - **ArduinoJson** by Benoit Blanchon
   - **WiFiManager** by tzapu
   - **WebSockets** by Markus Sattler
3. Click "Install" for each

---

## Step 4: Upload Firmware

1. Open `mini-controller.ino` in Arduino IDE
2. Select your board:
   - `Tools` → `Board` → `ESP32 Dev Module` (or your specific board)
3. Select port:
   - `Tools` → `Port` → Select your COM port
4. Click Upload button (→)
5. Wait for "Done uploading"

---

## Step 5: Setup WiFi

1. Open Serial Monitor (🔍 icon or `Ctrl+Shift+M`)
2. Set baud rate to **115200**
3. Press ESP32 reset button
4. Look for WiFi network on your phone: **"Mini_Controller"**
5. Connect to it
6. Portal opens automatically
7. Select your home WiFi
8. Enter password
9. Click Save

**Note:** If portal doesn't open, go to http://192.168.4.1 in browser

---

## Step 6: Verify Connection

In Serial Monitor, you should see:

```
========================================
Mini Smart Home Controller v1.0
Build: MINI-001
========================================
[INFO] WiFi connected: 192.168.1.XXX
[INFO] mDNS started: mini_controller.local
[INFO] Local API server started on port 8080
[INFO] WebSocket server started on port 81
========================================
[INFO] System ready - LED connected to GPIO 2
========================================
```

**✅ Success!** Your mini controller is online!

---

## Step 7: Test It!

### Test Built-in LED

1. Open Serial Monitor
2. Type this command in top box:
   ```
   LED ON
   ```
3. Watch built-in LED turn on!

### Test with Dashboard

1. Open cloud dashboard (URL from main project)
2. Wait 10 seconds
3. Look for "Test LED" device
4. Toggle switch
5. Watch LED respond!

### Test with App

1. Open mobile app
2. Login
3. Find "Test LED"
4. Toggle switch
5. LED responds!

---

## 🎉 You're Done!

Your mini controller is working! What you achieved:

- ✅ ESP32 connected to WiFi
- ✅ Communicating with cloud
- ✅ Controllable from dashboard
- ✅ Controllable from app
- ✅ Real-time updates working

---

## Next Steps

### Want More Visibility?

Add an external LED:
1. Get 1 LED + 1 resistor (220Ω)
2. See [WIRING.md](./WIRING.md) for simple connection
3. LED will be much brighter and easier to see!

### Want to Learn More?

- [README.md](./README.md) - Complete documentation
- [TESTING.md](./TESTING.md) - Full test procedures
- [WIRING.md](./WIRING.md) - Detailed wiring guide

### Ready for Production?

Once you're comfortable with the mini controller:
1. Study the code to understand how it works
2. Move to full firmware: `../main/main.ino`
3. Build the AC control hardware
4. Deploy the production system

---

## 🐛 Troubleshooting

### LED not blinking?
- Check GPIO 2 is correct for your board
- Some boards use GPIO 25 or other pins
- Change `LED_PIN` in code

### Can't connect to WiFi?
- Make sure it's 2.4GHz network (not 5GHz)
- Move ESP32 closer to router
- Check password is correct

### Not appearing in dashboard?
- Wait 30 seconds
- Check Serial Monitor for errors
- Verify internet connection
- Refresh dashboard page

### Serial Monitor shows errors?
- Re-upload firmware
- Check libraries are installed
- Try different USB cable
- Press reset button

---

## 💡 Pro Tips

**Tip 1:** Keep Serial Monitor open while testing to see what's happening

**Tip 2:** Note the IP address shown in Serial Monitor - you can use it for direct API calls

**Tip 3:** If you change WiFi, long-press reset button for 10 seconds to reset settings

**Tip 4:** The built-in LED might be faint - try it in a dark room or add external LED

---

## 🆘 Need Help?

- Check [README.md](./README.md) for detailed docs
- Check [TESTING.md](./TESTING.md) for validation steps
- Open an issue on GitHub
- Include Serial Monitor output

---

**Enjoy your mini controller! 🚀**

From here, you can explore the code, add features, or move to the full production system when ready.
