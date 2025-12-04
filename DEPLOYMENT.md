# Complete System Deployment Guide

This guide walks you through deploying the entire Smart Home Controller system from scratch.

## 📋 Prerequisites

- Google Account (for Apps Script and Sheets)
- Flutter SDK installed (for mobile app)
- Arduino IDE or PlatformIO (for ESP32)
- ESP32 development board
- Basic knowledge of JavaScript, Dart, and C++

---

## 🚀 Quick Start (30 Minutes)

### Step 1: Backend Setup (10 minutes)

1. **Create Google Sheets Database**
   ```
   1. Go to https://sheets.google.com
   2. Create new spreadsheet: "Smart Home Controller DB"
   3. Copy Spreadsheet ID from URL
   ```

2. **Deploy Apps Script Backend**
   ```
   1. Go to https://script.google.com
   2. New Project → "Smart Home Controller"
   3. Copy all files from backend/google-apps-script/
   4. Set Script Properties:
      - SHEET_ID: [your spreadsheet ID]
      - API_KEY: smarthome-2024-[random string]
   5. Deploy → New deployment → Web app
   6. Copy Web App URL
   ```

### Step 2: Web Dashboard (2 minutes)

The dashboard is automatically deployed with the backend!

Access it at: `[Your Web App URL]?page=dashboard`

### Step 3: Configure Flutter App (5 minutes)

1. **Update API Configuration**
   ```dart
   // Edit: software_Team/flutter project/demo/lib/config/api_config.dart
   
   static const String baseUrl = 'YOUR_WEB_APP_URL';
   static const String apiKey = 'YOUR_API_KEY';
   ```

2. **Build APK**
   ```bash
   cd "software_Team/flutter project/demo"
   flutter pub get
   flutter build apk --release
   ```

3. **Copy to app folder**
   ```bash
   cp build/app/outputs/flutter-apk/app-release.apk \
      ../../../app/release/smart-home-controller-v1.0.0-release.apk
   ```

### Step 4: Configure ESP32 (5 minutes)

1. **Update Firmware Configuration**
   ```cpp
   // Edit: firmware/main/config.h or main.ino
   
   String GOOGLE_SCRIPT_URL = "YOUR_WEB_APP_URL";
   String API_KEY = "YOUR_API_KEY";
   ```

2. **Flash Firmware**
   ```
   1. Open firmware/main/main.ino in Arduino IDE
   2. Select Board: ESP32 Dev Module
   3. Connect ESP32 via USB
   4. Click Upload
   ```

### Step 5: First Run (5 minutes)

1. **ESP32 Setup**
   ```
   1. Power on ESP32
   2. Connect to WiFi: "Smart_Home_Hub"
   3. Open browser to 192.168.4.1
   4. Configure your WiFi
   5. Device restarts and connects
   ```

2. **Test Dashboard**
   ```
   1. Open dashboard URL
   2. Device should appear automatically
   3. Try toggling power
   ```

3. **Test Mobile App**
   ```
   1. Install APK on Android device
   2. Sign up / Log in
   3. Devices should appear
   4. Test control functions
   ```

**🎉 Congratulations! Your system is live!**

---

## 📝 Detailed Deployment Steps

### Backend Deployment (Detailed)

#### 1. Google Sheets Setup

Create a new Google Sheets with this structure:

**Sheet 1: Devices**
```
Headers: UID | Name | Type | IP | Version | Uptime | RSSI | Heap | Last Seen | User ID | Created
```

**Sheet 2: DeviceStates**
```
Headers: UID | Channel | State | Value | Type | Timestamp
```

**Sheet 3: Schedules**
```
Headers: ID | User ID | Device UID | Channel | Name | Start Time | End Time | Days | Action | Value | Enabled | Created
```

**Sheet 4: Scenes**
```
Headers: ID | User ID | Name | Devices | Created
```

**Sheet 5: Users**
```
Headers: ID | Name | Email | Password | Created
```

**Sheet 6: Commands**
```
Headers: UID | Command | Timestamp | Executed
```

*Note: The backend will auto-create these sheets if they don't exist*

#### 2. Apps Script Deployment

**File Structure:**
```
Code.gs          - Main API logic
Database.gs      - Database helpers
Dashboard.html   - Web UI
appsscript.json  - Manifest
```

**Deployment Settings:**
- Execute as: Me (your account)
- Who has access: Anyone (or Anyone with Google account)
- Version: New

**Get URLs:**
```
Web App URL: https://script.google.com/macros/s/[ID]/exec
Dev URL: https://script.google.com/macros/s/[ID]/dev
```

Use the Web App URL for production!

#### 3. Custom Domain (Optional)

For a memorable URL like `home.yourdomain.com`:

**Option A: URL Shortener**
```
1. Use bit.ly or similar
2. Create short link: https://bit.ly/myhome
3. Points to your Apps Script URL
```

**Option B: Domain Forward**
```
1. In domain settings (GoDaddy, Namecheap, etc.)
2. Add forward rule
3. home.yourdomain.com → Apps Script URL
```

**Option C: Custom Domain (Advanced)**
```
1. Use Cloudflare Workers or similar
2. Create proxy to Apps Script
3. Set up DNS: home.yourdomain.com
```

---

### Flutter App Deployment (Detailed)

#### 1. Development Setup

**Install Flutter:**
```bash
# macOS
brew install flutter

# Linux
snap install flutter --classic

# Windows
# Download from https://flutter.dev
```

**Verify Installation:**
```bash
flutter doctor
flutter doctor --android-licenses
```

#### 2. Project Configuration

**Update Dependencies:**
```yaml
# pubspec.yaml already configured with:
dependencies:
  http: ^1.6.0
  connectivity_plus: ^6.0.3
  network_info_plus: ^5.0.3
  device_info_plus: ^10.1.0
```

**Configure Backend:**
```dart
// lib/config/api_config.dart
static const String baseUrl = 'https://script.google.com/macros/s/YOUR_ID/exec';
static const String apiKey = 'smarthome-2024-YOUR_KEY';
```

#### 3. Build Process

**Debug Build (for testing):**
```bash
flutter build apk --debug
```

**Release Build (for distribution):**
```bash
flutter build apk --release
```

**Split APKs (smaller size):**
```bash
flutter build apk --split-per-abi
# Creates: arm64-v8a, armeabi-v7a, x86_64
```

**App Bundle (for Play Store):**
```bash
flutter build appbundle --release
```

#### 4. Signing (Production)

**Create Keystore:**
```bash
keytool -genkey -v -keystore ~/smarthome-release.jks \
  -keyalg RSA -keysize 2048 -validity 10000 \
  -alias smarthome
```

**Create key.properties:**
```properties
storePassword=yourPassword
keyPassword=yourPassword
keyAlias=smarthome
storeFile=/path/to/smarthome-release.jks
```

**Configure build.gradle:**
```gradle
// android/app/build.gradle
android {
    signingConfigs {
        release {
            keyAlias keystoreProperties['keyAlias']
            keyPassword keystoreProperties['keyPassword']
            storeFile keystoreProperties['storeFile']
            storePassword keystoreProperties['storePassword']
        }
    }
    buildTypes {
        release {
            signingConfig signingConfigs.release
        }
    }
}
```

#### 5. Distribution

**GitHub Releases:**
```bash
1. Go to GitHub repo
2. Create new release
3. Upload APK
4. Add release notes
```

**Direct Distribution:**
```
Place APK in app/release/ folder
Update dashboard download link
Share link with users
```

---

### ESP32 Firmware Deployment (Detailed)

#### 1. Hardware Setup

**Required Components:**
- ESP32-WROOM-32 board
- 4x TRIAC modules (BT136)
- Zero-cross detection circuit
- 5V power supply
- Physical buttons (optional)

**Connections:**
```
ESP32 GPIO → TRIAC Control:
- GPIO 16 → TRIAC Channel 1
- GPIO 17 → TRIAC Channel 2
- GPIO 18 → TRIAC Channel 3
- GPIO 19 → TRIAC Channel 4

ESP32 GPIO → Zero-Cross:
- GPIO 13 → Zero-cross signal

ESP32 GPIO → Buttons:
- GPIO 32 → Button 1
- GPIO 33 → Button 2
- GPIO 25 → Button 3
- GPIO 26 → Button 4
```

#### 2. Arduino IDE Setup

**Install Board Support:**
```
1. File → Preferences
2. Additional Boards Manager URLs:
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
3. Tools → Board → Boards Manager
4. Search "ESP32" → Install
```

**Install Libraries:**
```
Library Manager (Sketch → Include Library → Manage Libraries):
- WiFiManager (by tzapu)
- ArduinoJson (by Benoit Blanchon)
- Espalexa (by Christian Schwinne)
- WebSockets (by Markus Sattler)
- SinricPro
```

#### 3. Configuration

**Edit firmware/main/main.ino:**
```cpp
// Google Apps Script Configuration
String GOOGLE_SCRIPT_URL = "https://script.google.com/macros/s/YOUR_ID/exec";
String API_KEY = "smarthome-2024-YOUR_KEY";

// Device Configuration
const char* DEVICE_NAME = "Smart Home Hub";
const char* AP_NAME = "Smart_Home_Hub";

// Alexa Configuration (optional)
String ALEXA_DEVICE_1 = "Living Room Light";
String ALEXA_DEVICE_2 = "Bedroom Fan";
String ALEXA_DEVICE_3 = "Kitchen Light";
String ALEXA_DEVICE_4 = "Porch Light";

// SinricPro Configuration (optional - for Google Assistant)
#define SINRIC_APP_KEY "your-app-key"
#define SINRIC_APP_SECRET "your-app-secret"
```

#### 4. Upload Firmware

**Board Settings:**
```
Board: "ESP32 Dev Module"
Upload Speed: 921600
CPU Frequency: 240MHz
Flash Frequency: 80MHz
Flash Mode: QIO
Flash Size: 4MB
Partition Scheme: Default
Core Debug Level: None
Port: (select your USB port)
```

**Upload:**
```
1. Connect ESP32 via USB
2. Press and hold BOOT button (if needed)
3. Click Upload in Arduino IDE
4. Wait for "Hard resetting via RTS pin..."
5. Done!
```

#### 5. OTA Updates

**Enable OTA in firmware:**
```cpp
#include <ArduinoOTA.h>

void setupOTA() {
  ArduinoOTA.setHostname("smarthome-esp32");
  ArduinoOTA.setPassword("admin");
  ArduinoOTA.begin();
}

void loop() {
  ArduinoOTA.handle();
  // ... rest of code
}
```

**Upload via OTA:**
```
1. Arduino IDE → Tools → Port
2. Select "smarthome-esp32 at 192.168.1.x"
3. Click Upload
4. No USB cable needed!
```

---

## 🔍 Verification & Testing

### Test Checklist

**Backend:**
- [ ] Sheets database created
- [ ] Apps Script deployed
- [ ] Test API endpoint: `[URL]?action=get_devices&api_key=[KEY]`
- [ ] Dashboard loads correctly
- [ ] Can create test device manually

**Mobile App:**
- [ ] APK builds successfully
- [ ] App installs on device
- [ ] Can signup/login
- [ ] API configuration correct

**ESP32:**
- [ ] Firmware uploads successfully
- [ ] Serial monitor shows WiFi connection
- [ ] Device appears in backend
- [ ] Can control via dashboard

**Integration:**
- [ ] ESP32 polls backend
- [ ] Dashboard shows device
- [ ] App shows device
- [ ] Control works from all interfaces
- [ ] WiFi reset works

---

## 🐛 Troubleshooting

### Common Issues

**1. "Apps Script execution failed"**
```
Solution:
- Check Script Properties are set
- Verify SHEET_ID is correct
- Re-deploy with new version
```

**2. "No devices showing in app"**
```
Solution:
- Check API URL in app config
- Verify API key matches
- Check ESP32 serial output
- Ensure ESP32 is online
```

**3. "ESP32 won't connect to WiFi"**
```
Solution:
- Check WiFi credentials
- Ensure 2.4GHz network
- Try WiFi reset (hold button 10s)
- Check serial monitor for errors
```

**4. "Flutter build fails"**
```
Solution:
- Run: flutter clean
- Run: flutter pub get
- Check Flutter version: flutter doctor
- Update dependencies
```

**5. "Dashboard not loading"**
```
Solution:
- Check deployment URL
- Clear browser cache
- Check browser console for errors
- Verify Apps Script permissions
```

---

## 📊 Monitoring & Maintenance

### System Health Checks

**Daily:**
- Check device online status
- Monitor API quotas

**Weekly:**
- Review error logs
- Check database size
- Test backup/restore

**Monthly:**
- Update firmware if needed
- Review user feedback
- Plan new features

### Backup Strategy

**Google Sheets:**
```
1. File → Download → Excel
2. Save with date: SmartHome_backup_2024-12-04.xlsx
3. Store securely
```

**Apps Script:**
```
1. File → Make a copy
2. Name: SmartHome Backend v1.0
3. Keep version history
```

**Firmware:**
```
1. Git commit and push regularly
2. Tag releases: git tag v1.0.0
3. Keep binary backups
```

---

## 🎯 Next Steps

After successful deployment:

1. **Customize** - Add your branding, colors, logos
2. **Extend** - Implement additional features
3. **Scale** - Add more devices
4. **Share** - Distribute app to users
5. **Monitor** - Track usage and performance

---

## 📞 Support

- **Documentation**: See README.md and FEATURES.md
- **Issues**: GitHub Issues
- **Updates**: Watch repository for releases

---

**Estimated Total Deployment Time**: 30-60 minutes
**Difficulty Level**: Intermediate
**Cost**: Free (using Google free tier)

Good luck with your deployment! 🚀
