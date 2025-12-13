# How to View the Smart Home Controller UI

This guide explains how to see and interact with the Smart Home Controller user interfaces after cloning the repository.

---

## 📱 Option 1: View Web Dashboard UI

The web dashboard has a modern glassmorphism design with gradient accents.

### Method A: Quick Preview (Easiest - No Setup Required)

Simply open the dashboard HTML file directly in your browser:

```bash
# On Windows
start backend/google-apps-script/Dashboard.html

# On Mac
open backend/google-apps-script/Dashboard.html

# On Linux
xdg-open backend/google-apps-script/Dashboard.html
```

**OR** navigate to `backend/google-apps-script/Dashboard.html` and double-click it.

**Note:** This shows the UI design but won't have live device data (backend not connected).

### Method B: Full Deployment (With Backend)

For full functionality with real device control:

1. **Deploy to Google Apps Script:**
   ```bash
   cd backend/google-apps-script
   # Follow deployment instructions in README.md
   ```

2. **Access via deployed URL:**
   - Get your Google Apps Script web app URL
   - Open in browser
   - Full functionality with device control

**Result:** Modern glassmorphism UI with:
- System statistics cards (Total Devices, Active Devices, Schedules, Scenes)
- Device control cards with toggle switches
- Brightness/speed sliders
- Gradient accents (#6366F1 to #8B5CF6)
- Backdrop blur effects

---

## 📱 Option 2: View Flutter Mobile App UI

The Flutter app has a native mobile UI matching the web dashboard design.

### Prerequisites

Install Flutter SDK:
```bash
# Download from https://flutter.dev/docs/get-started/install
# Or use package manager:

# Mac (using Homebrew)
brew install flutter

# Ubuntu/Debian
sudo snap install flutter --classic
```

### Running the App

1. **Navigate to the Flutter project:**
   ```bash
   cd "software_Team/flutter project/demo"
   ```

2. **Install dependencies:**
   ```bash
   flutter pub get
   ```

3. **Run on connected device or emulator:**
   ```bash
   # List available devices
   flutter devices

   # Run on connected device
   flutter run

   # OR build APK to install on Android
   flutter build apk --release
   # APK will be in build/app/outputs/flutter-apk/
   ```

4. **Run on Web (for quick preview):**
   ```bash
   flutter run -d chrome
   ```

**Result:** Native mobile app with:
- Home page with navigation cards
- Devices page with glassmorphism cards
- Fan and Light control pages
- Connection management
- Matching color scheme with web dashboard

---

## 🖼️ Option 3: View Screenshots (No Running Required)

If you just want to see what the UI looks like without running anything:

### View in GitHub

1. **Open README.md** - Has embedded screenshot at the top
2. **Open UI_PREVIEW.md** - Complete gallery with 3 screenshots
3. **Visit screenshot URLs directly:**
   - [Full Dashboard View](https://github.com/user-attachments/assets/d744726f-4be5-4d46-ad67-1705a11d3b0f)
   - [Active Devices](https://github.com/user-attachments/assets/411dd368-23e9-42dc-804b-4337b6b28414)
   - [Empty State](https://github.com/user-attachments/assets/283358ca-8a84-419f-ab56-5fe5a26b01dd)

### View Locally

Open these documentation files in your browser or markdown viewer:
```bash
# View the UI preview documentation
cat UI_PREVIEW.md

# Or open in browser (if markdown viewer installed)
open UI_PREVIEW.md  # Mac
start UI_PREVIEW.md  # Windows
xdg-open UI_PREVIEW.md  # Linux
```

---

## 🎨 What You'll See

### Web Dashboard Features

**Header Section:**
- 🏠 Smart Home Controller logo with gradient
- Professional IoT Management System subtitle
- Connection status indicator
- Action buttons: Download App, Schedules, Scenes, Settings

**System Statistics:**
- 📱 Total Devices (with green gradient icon)
- ⚡ Active Devices (with blue gradient icon)
- ⏰ Schedules (with purple gradient icon)
- 🎬 Scenes (with orange gradient icon)

**Device Cards:**
Each device card shows:
- Device name (e.g., "Living Room Fan")
- Online status indicator (pulsing green dot)
- Device type badge (Fan/Light)
- Device UID (MAC address)
- Power toggle switch with gradient
- Brightness/speed slider (0-100%)

**Design Elements:**
- Glassmorphism with backdrop blur
- Gradient backgrounds (#0F172A to #1E293B)
- Indigo to purple accent gradients
- Smooth animations and transitions
- Responsive layout (mobile, tablet, desktop)

### Flutter App Features

**Pages:**
- **Home:** Welcome screen with navigation cards
- **Devices:** List of all devices with control access
- **Fan Control:** Speed adjustment with real-time slider
- **Light Control:** Brightness adjustment
- **Connection:** WiFi and network settings
- **Profile:** User settings and preferences

**Design Elements:**
- Same color palette as web dashboard
- Glassmorphism containers with backdrop blur
- Gradient buttons and accents
- Native iOS/Android feel
- Smooth page transitions

---

## 🚀 Quick Start (Fastest Way to See UI)

**For Web Dashboard (30 seconds):**
```bash
# Just open the HTML file
open backend/google-apps-script/Dashboard.html
```

**For Flutter App (5 minutes):**
```bash
cd "software_Team/flutter project/demo"
flutter pub get
flutter run -d chrome  # Opens in browser
```

**For Screenshots (10 seconds):**
```bash
# Open the UI preview documentation
open UI_PREVIEW.md  # or view on GitHub
```

---

## ❓ Troubleshooting

### "I don't see the modern UI when I open Dashboard.html"

**Possible causes:**
1. **Browser caching:** Hard refresh with `Ctrl+Shift+R` (Windows/Linux) or `Cmd+Shift+R` (Mac)
2. **Wrong file:** Make sure you're opening `backend/google-apps-script/Dashboard.html`, not an old version
3. **Browser compatibility:** Use a modern browser (Chrome 90+, Firefox 88+, Safari 14+)

### "Flutter commands not found"

Install Flutter SDK:
```bash
# Visit https://flutter.dev/docs/get-started/install
# Follow installation instructions for your OS
```

### "I want to see the UI with real device data"

You need to:
1. Deploy the backend to Google Apps Script
2. Set up ESP32 firmware on hardware
3. Configure the connection between components
4. See [DEPLOYMENT.md](./DEPLOYMENT.md) for full setup

### "The UI looks different from the screenshots"

The screenshots show the UI with:
- Multiple devices connected
- Some devices active (powered on)
- System stats populated with real data

Without backend connection, you'll see:
- Empty state or placeholder data
- All zeros in statistics
- No real device cards

This is expected - the UI is there, just waiting for data.

---

## 📚 Next Steps

After viewing the UI:

1. **Like what you see?** Deploy the backend for full functionality
   - See [DEPLOYMENT.md](./DEPLOYMENT.md)

2. **Want to customize?** Check the design system
   - See [UI_PREVIEW.md](./UI_PREVIEW.md) for colors and styles

3. **Ready for production?** Follow the production guide
   - See [PRODUCTION_GUIDE.md](./PRODUCTION_GUIDE.md)

4. **Need help?** Check the FAQ
   - See [INSTALL.md](./INSTALL.md)

---

## 🎯 Summary

| Method | Time | Requirements | Shows |
|--------|------|--------------|-------|
| Open HTML directly | 30 sec | Browser only | UI design |
| Flutter web | 5 min | Flutter SDK | Mobile UI |
| View screenshots | 10 sec | None | All UIs |
| Full deployment | 30 min | Google account + ESP32 | Everything |

**Recommended for first-time viewing:** Open `Dashboard.html` directly in your browser.

---

**Last Updated:** December 13, 2025  
**For Issues:** Check [GitHub Issues](https://github.com/Gokul3611/smarthome-controller/issues)
