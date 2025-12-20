# ⚡ Quick Deployment Checklist

Use this checklist for rapid deployment. See [DEPLOY_STEPS.md](./DEPLOY_STEPS.md) for detailed instructions.

---

## 🎯 Pre-Deployment

- [ ] Google Account ready
- [ ] GitHub account with repo access
- [ ] Arduino IDE installed (for ESP32)
- [ ] Android device available (for testing)

---

## 📝 Backend Deployment

- [ ] Create Google Sheets: "Smart Home Controller DB"
- [ ] Copy Spreadsheet ID: `_______________________`
- [ ] Create Apps Script project
- [ ] Copy Code.gs, Database.gs, Dashboard.html
- [ ] Set Script Properties:
  - [ ] SHEET_ID: `_______________________`
  - [ ] API_KEY: `_______________________`
- [ ] Deploy as Web App (Anyone access)
- [ ] Copy Web App URL: `_______________________`
- [ ] Test: `URL?action=health&api_key=KEY`

---

## 📱 Mobile App Deployment

- [ ] Update `api_config.dart`:
  - [ ] baseUrl: `_______________________`
  - [ ] apiKey: `_______________________`
- [ ] Commit and push to trigger build
- [ ] Download APK from GitHub Actions
- [ ] Install on Android device
- [ ] Test login and device control

**OR Create Release:**
```bash
git tag v1.0.0
git push origin v1.0.0
```

---

## 💻 Web Dashboard

- [ ] Access: `WEB_APP_URL?page=dashboard`
- [ ] Bookmark URL
- [ ] Verify dashboard loads
- [ ] Test device visibility

---

## 🔌 ESP32 Firmware

- [ ] Install ESP32 board support in Arduino IDE
- [ ] Install libraries:
  - [ ] WiFiManager
  - [ ] ArduinoJson
  - [ ] Espalexa
  - [ ] WebSockets
  - [ ] SinricPro (optional)
- [ ] Edit `firmware/main/main.ino`:
  - [ ] GOOGLE_SCRIPT_URL: `_______________________`
  - [ ] API_KEY: `_______________________`
- [ ] Select Board: "ESP32 Dev Module"
- [ ] Upload firmware
- [ ] Configure WiFi (connect to "Smart_Home_Hub")
- [ ] Verify connection in Serial Monitor

---

## ✅ Testing

- [ ] Backend health check passes
- [ ] ESP32 appears in dashboard
- [ ] ESP32 appears in mobile app
- [ ] Dashboard control works
- [ ] Mobile app control works
- [ ] Physical buttons work (if connected)
- [ ] Device synchronization works

---

## 🚀 Deployment URLs

Save these for reference:

```
Spreadsheet ID:  _______________________________________
Web App URL:     _______________________________________
API Key:         _______________________________________
Dashboard URL:   _______________________________________
Latest APK:      https://github.com/Gokul3611/smarthome-controller/releases/latest
```

---

## 🔄 Continuous Deployment

### Automatic Builds
Every push to `main` triggers:
- Flutter APK build
- Artifacts uploaded to GitHub Actions
- Available for 90 days

### Release Creation
Tag a version to create official release:
```bash
git tag v1.0.0
git push origin v1.0.0
```
- APK automatically attached
- Release notes auto-generated
- Public download link created

---

## ⏱️ Time Estimates

| Task | Time |
|------|------|
| Backend Setup | 15 min |
| Mobile App Build | 20 min |
| Dashboard Access | 2 min |
| ESP32 Firmware | 15 min |
| Testing | 10 min |
| **Total** | **~60 min** |

---

## 🆘 Quick Troubleshooting

| Problem | Quick Fix |
|---------|-----------|
| Backend error | Re-deploy Apps Script |
| ESP32 won't connect | Use 2.4GHz WiFi, factory reset |
| App can't connect | Check api_config.dart values |
| Build fails | Run `flutter clean && flutter pub get` |

---

## 📱 Distribution

**Share APK:**
- Get from: https://github.com/Gokul3611/smarthome-controller/releases
- Or from: GitHub Actions artifacts
- Direct install or Google Play Store

**Share Dashboard:**
- Share: `YOUR_WEB_APP_URL?page=dashboard`

---

✅ **Deployment Complete!** See [DEPLOY_STEPS.md](./DEPLOY_STEPS.md) for detailed help.
