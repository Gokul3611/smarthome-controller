# Quick Start Guide - Building APK

This is a quick reference guide for building the Android APK.

## 🚀 Quick Build Commands

### One-Line Build (from repository root)

```bash
# Build release APK
cd "software_Team/flutter project/demo" && flutter pub get && flutter build apk --release && cp build/app/outputs/flutter-apk/app-release.apk ../../../app/release/smart-home-controller-v1.0.0-release.apk && cd ../../..

# Build debug APK
cd "software_Team/flutter project/demo" && flutter pub get && flutter build apk --debug && cp build/app/outputs/flutter-apk/app-debug.apk ../../../app/debug/smart-home-controller-v1.0.0-debug.apk && cd ../../..
```

### Step-by-Step Build

```bash
# 1. Navigate to Flutter project
cd "software_Team/flutter project/demo"

# 2. Get dependencies
flutter pub get

# 3. Build APK
flutter build apk --release

# 4. Copy to app folder
cp build/app/outputs/flutter-apk/app-release.apk \
   ../../../app/release/smart-home-controller-v1.0.0-release.apk

# 5. Return to root
cd ../../..
```

## 📦 Output Location

After building:
- **Release APK**: `app/release/smart-home-controller-v1.0.0-release.apk`
- **Debug APK**: `app/debug/smart-home-controller-v1.0.0-debug.apk`

## 📲 Quick Install

```bash
# Install via ADB
adb install app/release/smart-home-controller-v1.0.0-release.apk
```

## 🔍 Verify Build

```bash
# Check APK exists
ls -lh app/release/*.apk

# Get APK info
aapt dump badging app/release/smart-home-controller-v1.0.0-release.apk | grep package
```

## ⚠️ Prerequisites

Make sure you have:
- [ ] Flutter SDK installed (`flutter --version`)
- [ ] Android SDK installed (`flutter doctor`)
- [ ] All licenses accepted (`flutter doctor --android-licenses`)

## 📚 Full Documentation

For complete instructions, see [app/README.md](./README.md)
