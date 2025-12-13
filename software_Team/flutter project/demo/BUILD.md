# Quick Build Guide

## Build APK in 3 Steps

```bash
# 1. Install dependencies
flutter pub get

# 2. Build APK
flutter build apk --release

# 3. Find your APK
# Location: build/app/outputs/flutter-apk/app-release.apk
```

## Install on Phone

```bash
# Transfer APK to phone and tap to install
# OR use ADB:
adb install build/app/outputs/flutter-apk/app-release.apk
```

Done!
