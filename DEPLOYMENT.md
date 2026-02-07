# Deployment Guide

Step-by-step instructions for deploying the complete Smart Home Controller system.

## Prerequisites

- Google Account (for Apps Script backend and Sheets database)
- Arduino IDE 2.x or PlatformIO (for ESP32 firmware)
- Flutter SDK (for building the mobile app, or use GitHub Actions)
- ESP32-WROOM-32 development board with TRIAC hardware
- 2.4 GHz WiFi network

## 1. Backend Setup

### Create Google Sheets Database

1. Go to https://sheets.google.com
2. Create a new blank spreadsheet named `Smart Home Controller DB`
3. Copy the Spreadsheet ID from the URL:
   ```
   https://docs.google.com/spreadsheets/d/<SPREADSHEET_ID>/edit
   ```

The backend auto-creates required sheets (Devices, DeviceStates, Schedules, Scenes, Users, Commands) on first use.

### Deploy Apps Script

1. Go to https://script.google.com and create a new project
2. Copy the contents of these files into the project:
   - `backend/google-apps-script/Code.gs`
   - `backend/google-apps-script/Database.gs`
   - `backend/google-apps-script/Dashboard.html`
3. Go to Project Settings > Script Properties and add:
   - `SHEET_ID` = your Spreadsheet ID from above
   - `API_KEY` = a random string, e.g. `smarthome-2024-<random>`
4. Click Deploy > New deployment > Web app
   - Execute as: Me
   - Who has access: Anyone
5. Copy the Web App URL

### Verify Backend

Open in a browser:
```
<WEB_APP_URL>?action=health&api_key=<YOUR_API_KEY>
```

Expected response: `{"status":"ok"}`

The web dashboard is available at:
```
<WEB_APP_URL>?page=dashboard
```

## 2. Mobile App

### Option A: GitHub Actions (recommended)

Push code to the `main` branch. The workflow at `.github/workflows/build-and-deploy.yml` builds release and debug APKs automatically. Download from the Actions tab > Artifacts.

To create a tagged release:
```bash
git tag v1.0.0
git push origin v1.0.0
```

### Option B: Build locally

1. Update `frontend/flutter project/demo/lib/config/api_config.dart`:
   ```dart
   static const String baseUrl = '<YOUR_WEB_APP_URL>';
   static const String apiKey = '<YOUR_API_KEY>';
   ```

2. Build:
   ```bash
   cd "frontend/flutter project/demo"
   flutter pub get
   flutter build apk --release
   ```

3. The APK is at `build/app/outputs/flutter-apk/app-release.apk`

## 3. ESP32 Firmware

1. Open `firmware/main/main.ino` in Arduino IDE

2. Set your credentials near the top of the file:
   ```cpp
   String GOOGLE_SCRIPT_URL = "<YOUR_WEB_APP_URL>";
   String API_KEY = "<YOUR_API_KEY>";
   ```

3. Install required libraries (Sketch > Include Library > Manage Libraries):
   - WiFiManager (tzapu)
   - ArduinoJson (Benoit Blanchon)
   - Espalexa (Christian Schwinne)
   - WebSockets (Markus Sattler)

4. Board settings:
   - Board: ESP32 Dev Module
   - Upload Speed: 921600
   - Flash Size: 4MB
   - Partition Scheme: Default

5. Connect ESP32 via USB and upload

6. After upload, open Serial Monitor at 115200 baud to verify boot

## 4. First Run

1. Power on the ESP32
2. Connect to the `Smart_Home_Hub` WiFi network from a phone or laptop
3. Open `192.168.4.1` in a browser and configure your home WiFi
4. The device reboots and connects to your network
5. Open the web dashboard — the device should appear within a few seconds
6. Install the APK on an Android device, sign up, and verify device control

## Verification Checklist

- [ ] Backend health endpoint returns `{"status":"ok"}`
- [ ] Web dashboard loads and shows connected device(s)
- [ ] ESP32 Serial Monitor shows WiFi connection and backend polling
- [ ] Mobile app can sign up, log in, and list devices
- [ ] Toggling a device from dashboard/app changes state on ESP32
- [ ] Physical button toggle is reflected in dashboard/app

## Troubleshooting

| Problem | Fix |
|---------|-----|
| Apps Script execution failed | Verify SHEET_ID and API_KEY in Script Properties; redeploy |
| No devices in app | Check `api_config.dart` URL and key; check ESP32 serial output |
| ESP32 won't connect to WiFi | Ensure 2.4 GHz network; hold Switch 1 for 10 s to factory reset |
| Flutter build fails | Run `flutter clean && flutter pub get`; check `flutter doctor` |
| Dashboard won't load | Clear browser cache; check deployment URL; verify Apps Script permissions |
