# Mobile Application Quick Start Guide

**Document Number:** APP-QUICK-001  
**Revision:** 1.0  
**Date:** 2025-12-16  
**Classification:** User Guide

This is a quick reference guide for building the Android APK.

## 1.0 Quick Build Commands

### 1.1 Recommended: Step-by-Step Build

```bash
# 1. Navigate to Flutter project
cd "software_Team/flutter project/demo"

# 2. Get dependencies
flutter pub get

# 3. Build APK (release or debug)
flutter build apk --release

# 4. Copy to app folder
cp build/app/outputs/flutter-apk/app-release.apk \
   ../../../app/release/smart-home-controller-v1.0.0-release.apk

# 5. Return to root
cd ../../..
```

### 1.2 Alternative: Multi-line Command

```bash
# Build release APK
cd "software_Team/flutter project/demo" && \
  flutter pub get && \
  flutter build apk --release && \
  cp build/app/outputs/flutter-apk/app-release.apk \
     ../../../app/release/smart-home-controller-v1.0.0-release.apk && \
  cd ../../..

# Build debug APK
cd "software_Team/flutter project/demo" && \
  flutter pub get && \
  flutter build apk --debug && \
  cp build/app/outputs/flutter-apk/app-debug.apk \
     ../../../app/debug/smart-home-controller-v1.0.0-debug.apk && \
  cd ../../..
```

### 1.3 Build Script

Save this as `build-apk.sh` in the repository root:

```bash
#!/bin/bash
# Build Smart Home Controller APK

BUILD_TYPE="${1:-release}"  # Default to release build
VERSION="1.0.0"

echo "Building $BUILD_TYPE APK..."

cd "software_Team/flutter project/demo" || exit 1
flutter pub get || exit 1
flutter build apk --"$BUILD_TYPE" || exit 1

cp "build/app/outputs/flutter-apk/app-$BUILD_TYPE.apk" \
   "../../../app/$BUILD_TYPE/smart-home-controller-v$VERSION-$BUILD_TYPE.apk" || exit 1

cd ../../..
echo " APK built successfully: app/$BUILD_TYPE/smart-home-controller-v$VERSION-$BUILD_TYPE.apk"
```

Usage:
```bash
chmod +x build-apk.sh
./build-apk.sh release  # or ./build-apk.sh debug
```

## 2.0 Output Location

After building:
- **Release APK**: `app/release/smart-home-controller-v1.0.0-release.apk`
- **Debug APK**: `app/debug/smart-home-controller-v1.0.0-debug.apk`

## 3.0 Quick Install

```bash
# Install via ADB
adb install app/release/smart-home-controller-v1.0.0-release.apk
```

## 4.0 Verify Build

```bash
# Check APK exists
ls -lh app/release/*.apk

# Get APK info
aapt dump badging app/release/smart-home-controller-v1.0.0-release.apk | grep package
```

## 5.0 Prerequisites

Make sure you have:
- [ ] Flutter SDK installed (`flutter --version`)
- [ ] Android SDK installed (`flutter doctor`)
- [ ] All licenses accepted (`flutter doctor --android-licenses`)

## 6.0 Full Documentation

For complete instructions, see [app/README.md](./README.md)
