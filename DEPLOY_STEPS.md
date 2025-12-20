# 🚀 Step-by-Step Deployment Guide

Complete deployment instructions for the Smart Home Controller system. Follow these steps in order to deploy the entire system from scratch.

---

## 📋 Prerequisites Checklist

Before starting deployment, ensure you have:

- [ ] Google Account (for backend)
- [ ] GitHub account with this repository
- [ ] Android device for testing (optional)
- [ ] ESP32 development board (for hardware deployment)
- [ ] Arduino IDE or PlatformIO installed (for firmware)
- [ ] Basic command line knowledge

**Estimated Time:** 45-60 minutes for complete deployment

---

## 🔧 Part 1: Backend Deployment (15 minutes)

### Step 1.1: Create Google Sheets Database

1. Open your browser and go to: https://sheets.google.com
2. Click **"Blank spreadsheet"** (green plus button)
3. Name it: `Smart Home Controller DB`
4. Copy the Spreadsheet ID from the URL:
   ```
   https://docs.google.com/spreadsheets/d/SPREADSHEET_ID_HERE/edit
   ```
5. Save this ID - you'll need it later

### Step 1.2: Create Sheet Structure

Create the following sheets in your spreadsheet:

1. **Sheet 1: Devices**
   - Rename "Sheet1" to "Devices"
   - Add headers in row 1:
     ```
     UID | Name | Type | IP | Version | Uptime | RSSI | Heap | Last Seen | User ID | Created
     ```

2. **Sheet 2: DeviceStates**
   - Click "+" to add new sheet, name it "DeviceStates"
   - Add headers:
     ```
     UID | Channel | State | Value | Type | Timestamp
     ```

3. **Sheet 3: Users**
   - Click "+" to add new sheet, name it "Users"
   - Add headers:
     ```
     ID | Name | Email | Password | Created
     ```

4. **Sheet 4: Commands**
   - Click "+" to add new sheet, name it "Commands"
   - Add headers:
     ```
     UID | Command | Timestamp | Executed
     ```

### Step 1.3: Deploy Google Apps Script Backend

1. Open a new tab and go to: https://script.google.com
2. Click **"New Project"**
3. Name it: `Smart Home Controller Backend`
4. Delete the default `function myFunction()` code

5. **Add Code.gs file:**
   - Copy the entire content from: `backend/google-apps-script/Code.gs`
   - Paste it into the editor

6. **Add Database.gs file:**
   - Click the "+" next to "Files"
   - Select "Script"
   - Name it: `Database`
   - Copy content from: `backend/google-apps-script/Database.gs`
   - Paste and save

7. **Add Dashboard.html file:**
   - Click the "+" next to "Files"
   - Select "HTML"
   - Name it: `Dashboard`
   - Copy content from: `backend/google-apps-script/Dashboard.html`
   - Paste and save

### Step 1.4: Configure Script Properties

1. In Apps Script editor, click on **"Project Settings"** (gear icon on left)
2. Scroll to **"Script Properties"**
3. Click **"Add script property"**
4. Add the following properties:
   
   **Property 1:**
   - Property: `SHEET_ID`
   - Value: `[Your Spreadsheet ID from Step 1.1]`
   
   **Property 2:**
   - Property: `API_KEY`
   - Value: `smarthome-2024-[YourRandomString]`
   - (Make this at least 20 characters, include numbers and letters)

5. Click **"Save script properties"**

### Step 1.5: Deploy as Web App

1. Click **"Deploy"** button (top right)
2. Select **"New deployment"**
3. Click the gear icon next to "Select type"
4. Choose **"Web app"**
5. Configure deployment:
   - Description: `Production v1.0`
   - Execute as: **Me (your email)**
   - Who has access: **Anyone**
6. Click **"Deploy"**
7. Click **"Authorize access"**
8. Select your Google account
9. Click **"Advanced"** → **"Go to Smart Home Controller Backend (unsafe)"**
10. Click **"Allow"**
11. **IMPORTANT:** Copy the **Web app URL** - Save it somewhere safe!
    ```
    Example: https://script.google.com/macros/s/ABC123.../exec
    ```

### Step 1.6: Test Backend

1. Open a new tab
2. Paste your Web App URL and add: `?action=health&api_key=YOUR_API_KEY`
3. You should see: `{"status":"ok","timestamp":...}`
4. ✅ Backend is deployed and working!

---

## 📱 Part 2: Mobile App Deployment (20 minutes)

### Step 2.1: Update Flutter App Configuration

1. Open the file: `frontend/flutter project/demo/lib/config/api_config.dart`
2. Update the following values:
   ```dart
   static const String baseUrl = 'YOUR_WEB_APP_URL_HERE';
   static const String apiKey = 'YOUR_API_KEY_HERE';
   ```
3. Save the file

### Step 2.2: Build APK Using GitHub Actions (Automated)

**Option A: Automatic Build on Push (Recommended)**

1. Commit and push your changes:
   ```bash
   git add .
   git commit -m "Configure API for deployment"
   git push origin main
   ```

2. Go to your GitHub repository
3. Click **"Actions"** tab
4. Watch the **"Build and Deploy"** workflow run
5. Wait 5-10 minutes for build to complete
6. Once complete, click on the workflow run
7. Download the **"release-apk"** artifact
8. Extract the APK file

**Option B: Manual Trigger**

1. Go to GitHub repository → **"Actions"** tab
2. Click **"Build and Deploy"** workflow
3. Click **"Run workflow"** button
4. Select branch: `main`
5. Click **"Run workflow"**
6. Wait for completion and download APK

**Option C: Create a Release Tag (For Official Releases)**

1. Create and push a version tag:
   ```bash
   git tag v1.0.0
   git push origin v1.0.0
   ```
2. GitHub Actions will automatically:
   - Build the APK
   - Create a GitHub Release
   - Attach the APK to the release
3. Go to: **Releases** section on GitHub to download

### Step 2.3: Install APK on Android Device

1. Download the APK to your Android device
2. Open **Settings** → **Security**
3. Enable **"Install from unknown sources"** or **"Install unknown apps"**
4. Open the downloaded APK file
5. Tap **"Install"**
6. Open the app and create an account
7. ✅ Mobile app is deployed!

---

## 💻 Part 3: Web Dashboard Access (2 minutes)

### Step 3.1: Access Dashboard

Your web dashboard is automatically deployed with the backend!

1. Open your browser
2. Go to: `YOUR_WEB_APP_URL?page=dashboard`
3. The dashboard will load
4. ✅ Web dashboard is accessible!

### Step 3.2: Create Bookmark (Optional)

1. Bookmark the dashboard URL for easy access
2. Name it: "Smart Home Dashboard"

---

## 🔌 Part 4: ESP32 Firmware Deployment (15 minutes)

### Step 4.1: Install Arduino IDE (If not installed)

1. Download from: https://www.arduino.cc/en/software
2. Install Arduino IDE
3. Open Arduino IDE

### Step 4.2: Setup ESP32 Board Support

1. Go to: **File** → **Preferences**
2. In **"Additional Board Manager URLs"**, add:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
3. Click **OK**
4. Go to: **Tools** → **Board** → **Boards Manager**
5. Search for: `ESP32`
6. Install: **"esp32 by Espressif Systems"**
7. Wait for installation to complete

### Step 4.3: Install Required Libraries

Go to: **Sketch** → **Include Library** → **Manage Libraries**

Install the following libraries (search and click "Install"):

1. **WiFiManager** by tzapu (version 2.0.16+)
2. **ArduinoJson** by Benoit Blanchon (version 6.x)
3. **Espalexa** by Christian Schwinne (version 2.7.0+)
4. **WebSockets** by Markus Sattler (version 2.4.0+)
5. **SinricPro** (version 2.10.0+) - Optional for Google Assistant

### Step 4.4: Configure Firmware

1. Open file: `firmware/main/main.ino` in Arduino IDE
2. Find these lines near the top (around line 20-30):
   ```cpp
   String GOOGLE_SCRIPT_URL = "your_script_url_here";
   String API_KEY = "your_api_key_here";
   ```
3. Replace with your values:
   ```cpp
   String GOOGLE_SCRIPT_URL = "YOUR_WEB_APP_URL";
   String API_KEY = "YOUR_API_KEY";
   ```
4. Save the file

### Step 4.5: Upload Firmware to ESP32

1. Connect ESP32 to your computer via USB
2. In Arduino IDE, go to **Tools** and configure:
   - Board: **"ESP32 Dev Module"**
   - Upload Speed: **921600**
   - CPU Frequency: **240MHz**
   - Flash Size: **4MB**
   - Port: **Select your ESP32 port** (e.g., COM3, /dev/ttyUSB0)
3. Click the **Upload** button (→)
4. Wait for "Connecting........"
5. Some boards require holding the BOOT button during upload
6. Wait for "Hard resetting via RTS pin..."
7. ✅ Firmware uploaded successfully!

### Step 4.6: Configure WiFi on ESP32

1. Open **Serial Monitor** (Tools → Serial Monitor)
2. Set baud rate to: **115200**
3. Press the RESET button on ESP32
4. You should see boot messages
5. Look for: `AP Mode: Smart_Home_Hub`
6. On your phone/computer, connect to WiFi: **"Smart_Home_Hub"**
7. Browser should open automatically to `192.168.4.1`
   - If not, open browser and go to: `192.168.4.1`
8. Select your WiFi network from the list
9. Enter your WiFi password
10. Click **"Save"**
11. ESP32 will restart and connect to your WiFi
12. ✅ ESP32 is online!

---

## ✅ Part 5: Verification & Testing (10 minutes)

### Step 5.1: Verify Backend Connection

1. Check Serial Monitor on ESP32
2. Look for messages like:
   ```
   [INFO] Connected to WiFi
   [INFO] IP Address: 192.168.1.XXX
   [INFO] Polling backend...
   [INFO] Backend response received
   ```

### Step 5.2: Verify Dashboard

1. Open your dashboard URL
2. Refresh the page
3. You should see your ESP32 device appear
4. Device shows: Name, Status, IP address
5. Try toggling a switch - should see response in Serial Monitor

### Step 5.3: Verify Mobile App

1. Open the Smart Home Controller app
2. Sign up or log in
3. You should see your device(s) listed
4. Try controlling a device
5. Check Serial Monitor for confirmation

### Step 5.4: Test End-to-End

**Test 1: Control from Dashboard**
- Toggle a switch in dashboard
- Verify ESP32 receives command (Serial Monitor)
- Verify app updates automatically

**Test 2: Control from App**
- Toggle a switch in mobile app
- Verify ESP32 receives command
- Verify dashboard updates

**Test 3: Physical Button (if connected)**
- Press physical button on ESP32
- Verify dashboard updates
- Verify app updates

---

## 🎉 Deployment Complete!

Your Smart Home Controller system is now fully deployed!

### What You've Deployed:

✅ **Backend:** Google Apps Script with database  
✅ **Dashboard:** Web-based control interface  
✅ **Mobile App:** Android APK ready for distribution  
✅ **Firmware:** ESP32 connected and operational  
✅ **Automation:** GitHub Actions for continuous deployment

---

## 📊 Next Steps

### For Regular Updates:

1. **Update Mobile App:**
   - Change version in `pubspec.yaml`
   - Push to main branch or create a tag
   - GitHub Actions builds automatically
   - Download from Actions or Releases

2. **Update Firmware:**
   - Modify code in `firmware/main/main.ino`
   - Upload via Arduino IDE or OTA

3. **Update Backend:**
   - Edit files in `backend/google-apps-script/`
   - Go to Apps Script editor
   - Deploy → Manage deployments → Edit
   - Create new version

### For Distribution:

1. **Share Mobile App:**
   - Download APK from GitHub Releases
   - Share direct download link
   - Or publish to Google Play Store

2. **Share Dashboard:**
   - Share your Web App URL with `?page=dashboard`
   - Users can bookmark it

3. **Manufacture Hardware:**
   - Use PCB files in `hardware/pcb/`
   - Follow assembly guide in `hardware/README.md`
   - Flash firmware on each unit

---

## 🆘 Troubleshooting

### "Backend not responding"
- Check SHEET_ID in Script Properties
- Verify API_KEY matches everywhere
- Re-deploy Apps Script

### "ESP32 won't connect"
- Ensure 2.4GHz WiFi (not 5GHz)
- Check WiFi credentials
- Try factory reset (hold button 10s)

### "App can't connect"
- Verify baseUrl in api_config.dart
- Check apiKey matches backend
- Rebuild and reinstall APK

### "GitHub Actions build fails"
- Check Flutter version compatibility
- Verify pubspec.yaml syntax
- Review Actions logs for details

---

## 📞 Support

- **Issues:** https://github.com/Gokul3611/smarthome-controller/issues
- **Documentation:** See README.md and other guides
- **Updates:** Watch repository for new releases

---

**Total Deployment Time:** ~45-60 minutes  
**Difficulty:** Intermediate  
**Cost:** $0 (using free tiers)

🎊 **Congratulations on deploying your Smart Home Controller!**
