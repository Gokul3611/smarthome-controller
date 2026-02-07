# Deployment Guide

This guide walks you through deploying the entire Smart Home Controller system.

## Prerequisites

- Google Account (for Apps Script and Sheets)
- Flutter SDK installed (for mobile app)
- Arduino IDE or PlatformIO (for ESP32)
- ESP32 development board
- Basic knowledge of JavaScript, Dart, and C++

---

## Step 1: Backend Setup

1. **Create Google Sheets Database**
   1. Go to https://sheets.google.com
   2. Create new spreadsheet: "Smart Home Controller DB"
   3. Copy Spreadsheet ID from URL

2. **Deploy Apps Script Backend**
   1. Go to https://script.google.com
   2. New Project → "Smart Home Controller"
   3. Copy all files from `backend/google-apps-script/`
   4. Set Script Properties:
      - `SHEET_ID`: your spreadsheet ID
      - `API_KEY`: `smarthome-2024-<random string>`
   5. Deploy → New deployment → Web app
   6. Execute as: Me; Who has access: Anyone
   7. Copy Web App URL

The web dashboard is automatically deployed with the backend. Access it at: `[Your Web App URL]?page=dashboard`

The backend will auto-create the required sheets (Devices, DeviceStates, Schedules, Scenes, Users, Commands) on first use.

---

## Step 2: Configure and Build Flutter App

1. **Update API Configuration**
   ```dart
   // Edit: frontend/flutter project/demo/lib/config/api_config.dart
   static const String baseUrl = 'YOUR_WEB_APP_URL';
   static const String apiKey = 'YOUR_API_KEY';
   ```

2. **Build APK**
   ```bash
   cd "frontend/flutter project/demo"
   flutter pub get
   flutter build apk --release
   ```

3. The APK is at `build/app/outputs/flutter-apk/app-release.apk`. Install it on your Android device.

---

## Step 3: Configure and Flash ESP32

1. **Update Firmware Configuration**
   ```cpp
   // Edit: firmware/main/main.ino
   String GOOGLE_SCRIPT_URL = "YOUR_WEB_APP_URL";
   String API_KEY = "YOUR_API_KEY";
   ```

2. **Flash Firmware**
   1. Open `firmware/main/main.ino` in Arduino IDE
   2. Select Board: ESP32 Dev Module
   3. Connect ESP32 via USB
   4. Click Upload

See [INSTALL.md](./INSTALL.md) for detailed Arduino IDE/PlatformIO setup and library installation.

---

## Step 4: First Run

1. Power on ESP32
2. Connect to WiFi network: `Smart_Home_Hub`
3. Open browser to `192.168.4.1`
4. Configure your WiFi network
5. Device restarts and connects
6. Open dashboard — device should appear automatically
7. Install and open the mobile app, sign up, and test control

---

## Verification Checklist

**Backend:**
- [ ] Google Sheets database created
- [ ] Apps Script deployed
- [ ] Test API: `[URL]?action=get_devices&api_key=[KEY]`
- [ ] Dashboard loads and displays correctly

**Mobile App:**
- [ ] APK builds successfully
- [ ] App installs and runs
- [ ] Can sign up / log in
- [ ] Devices appear in app

**ESP32:**
- [ ] Firmware uploads successfully
- [ ] Serial monitor (115200 baud) shows WiFi connection
- [ ] Device appears in backend and dashboard
- [ ] Control works from dashboard and app

---

## Troubleshooting

**"Apps Script execution failed"**
- Check Script Properties are set correctly
- Verify SHEET_ID matches your spreadsheet
- Re-deploy with a new version

**"No devices showing"**
- Check API URL in app config
- Verify API key matches between firmware, app, and backend
- Check ESP32 serial output for connection errors

**"ESP32 won't connect to WiFi"**
- Ensure 2.4GHz network (5GHz not supported)
- Try factory reset (hold Switch 1 for 10 seconds)
- Check serial monitor for error messages

**"Flutter build fails"**
- Run `flutter clean && flutter pub get`
- Check Flutter SDK version with `flutter doctor`

---

**Estimated deployment time**: 30–60 minutes depending on familiarity with the tools.
**Cost**: Free (using Google free tier).
