# 🚀 Deployment Quick Start

**Choose your deployment scenario:**

---

## 🎯 Scenario 1: First Time Deployment
**"I want to deploy everything from scratch"**

📖 **Follow:** [DEPLOY_STEPS.md](./DEPLOY_STEPS.md)  
⏱️ **Time:** 60 minutes  
✅ **Gets you:** Complete working system

**Quick Steps:**
1. Setup Google Sheets + Apps Script backend
2. Get Web App URL and API Key
3. Push code to GitHub (triggers APK build)
4. Flash ESP32 with firmware
5. Configure WiFi
6. Test everything

---

## 🔄 Scenario 2: Update Mobile App
**"I made changes to the Flutter app"**

### Automatic (Recommended)
```bash
# Commit your changes
git add .
git commit -m "Update app features"
git push origin main

# Wait 10 minutes, then download from:
# GitHub → Actions → Latest workflow → Artifacts
```

### Create Release
```bash
# For official releases
git tag v1.0.1
git push origin v1.0.1

# Download from:
# GitHub → Releases → Latest
```

---

## 🔧 Scenario 3: Update ESP32 Firmware
**"I modified the ESP32 code"**

### Via Arduino IDE
```
1. Open firmware/main/main.ino
2. Tools → Board → ESP32 Dev Module
3. Tools → Port → Select your ESP32
4. Click Upload (→)
```

### Via OTA (Over The Air)
```
1. Build firmware binary
2. Upload to web server
3. Update backend with binary URL
4. ESP32 updates automatically
```

📖 **Details:** See [OTA_GUIDE.md](./OTA_GUIDE.md)

---

## ☁️ Scenario 4: Update Backend
**"I changed Google Apps Script code"**

```
1. Go to: script.google.com
2. Open your project
3. Edit Code.gs, Database.gs, or Dashboard.html
4. Save changes
5. Deploy → Manage deployments → Edit
6. Version: New version
7. Deploy
```

⚠️ **Note:** URL stays the same, no config changes needed

---

## 🌐 Scenario 5: Share with Users
**"I want to distribute to end users"**

### Option A: GitHub Releases
```
👥 Share this link:
https://github.com/Gokul3611/smarthome-controller/releases/latest

Users click "Assets" → Download APK
```

### Option B: Direct APK
```
1. Get APK from GitHub Actions or Releases
2. Upload to Google Drive / Dropbox
3. Share link with users
4. Users enable "Install from unknown sources"
5. Users install APK
```

### Option C: Google Play Store
```
1. Build: flutter build appbundle
2. Create Play Developer account ($25 one-time)
3. Upload to Play Console
4. Submit for review
5. Users install from Play Store
```

---

## 🔑 Scenario 6: Change API Keys
**"I need to update credentials"**

### Update Backend
```
1. Go to: script.google.com
2. Project Settings → Script Properties
3. Edit API_KEY value
4. Save
```

### Update Mobile App
```
1. Edit: frontend/flutter project/demo/lib/config/api_config.dart
2. Change apiKey value
3. Commit and push (triggers rebuild)
4. Download new APK
5. Reinstall app
```

### Update ESP32
```
1. Edit: firmware/main/main.ino
2. Change API_KEY value
3. Upload firmware
```

---

## 🐛 Scenario 7: Something's Not Working
**"I need to troubleshoot"**

### Quick Checks
```
✓ Backend health:    URL?action=health&api_key=YOUR_KEY
✓ ESP32 logs:        Arduino IDE → Serial Monitor (115200)
✓ App logs:          Android Studio logcat
✓ Dashboard:         Browser console (F12)
```

### Common Fixes
| Problem | Fix |
|---------|-----|
| "Backend error" | Re-deploy Apps Script |
| "ESP32 offline" | Check WiFi (2.4GHz), factory reset |
| "App can't connect" | Verify api_config.dart |
| "Build fails" | `flutter clean && flutter pub get` |

📖 **Full troubleshooting:** [DEPLOY_STEPS.md](./DEPLOY_STEPS.md) Section 6

---

## 📦 Scenario 8: Backup Everything
**"I want to backup my deployment"**

### Backend
```
1. Google Sheets:
   File → Download → Excel (.xlsx)

2. Apps Script:
   File → Make a copy
   Name: "SmartHome Backup YYYY-MM-DD"
```

### Code
```bash
# Already backed up in Git!
git log  # View history
git tag  # View releases
```

### Credentials
```
Save in password manager:
- Spreadsheet ID
- Web App URL  
- API Key
- Device MAC addresses
```

---

## 🏭 Scenario 9: Production Deployment
**"I'm deploying for a company/clients"**

📖 **Follow:** [PRODUCTION_GUIDE.md](./PRODUCTION_GUIDE.md)

**Additional steps:**
- Use production Google account
- Implement rate limiting
- Enable logging and monitoring
- Setup automated backups
- Configure custom domain
- Implement user authentication
- Setup error tracking
- Create support documentation

---

## 🧪 Scenario 10: Test Environment
**"I want a staging environment"**

```
1. Create separate Google Sheets: "SmartHome_Test_DB"
2. Deploy new Apps Script: "SmartHome_Test_Backend"
3. Use different API key: "test-api-key-2024"
4. Create test branch: git checkout -b test
5. Update configs with test credentials
6. Deploy to test devices with test firmware
```

---

## 📊 Need More Detail?

| Document | Purpose |
|----------|---------|
| [DEPLOY_STEPS.md](./DEPLOY_STEPS.md) | Complete step-by-step guide |
| [QUICK_DEPLOY_CHECKLIST.md](./QUICK_DEPLOY_CHECKLIST.md) | Fast reference checklist |
| [DEPLOYMENT_OVERVIEW.md](./DEPLOYMENT_OVERVIEW.md) | Visual architecture guide |
| [DEPLOYMENT.md](./DEPLOYMENT.md) | Original technical guide |
| [PRODUCTION_GUIDE.md](./PRODUCTION_GUIDE.md) | Enterprise deployment |

---

## ⚡ Ultra-Quick Reference

```bash
# First deployment
See: DEPLOY_STEPS.md

# Build APK
git push origin main
# Download from Actions after 10 min

# Create release
git tag v1.0.0 && git push origin v1.0.0

# Update ESP32
Arduino IDE → Upload

# Update backend
script.google.com → Edit → Deploy

# Share app
/releases/latest

# Test backend
URL?action=health&api_key=KEY
```

---

**🎯 Start here:** [DEPLOY_STEPS.md](./DEPLOY_STEPS.md)
