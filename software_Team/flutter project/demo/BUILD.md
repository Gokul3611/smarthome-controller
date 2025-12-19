# Flutter Build Procedures

**Document Number:** SOFT-BUILD-001  
**Revision:** 1.0  
**Date:** 2025-12-16  
**Classification:** Technical Procedure

## 1.0 Build APK in 3 Steps

```bash
# 1. Install dependencies
flutter pub get

# 2. Build APK
flutter build apk --release

# 3. Find your APK
# Location: build/app/outputs/flutter-apk/app-release.apk
```

## 2.0 Install on Phone

```bash
# Transfer APK to phone and tap to install
# OR use ADB:
adb install build/app/outputs/flutter-apk/app-release.apk
```

Done!
