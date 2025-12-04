# Smart Home Controller - Android APK

This folder contains the Android APK builds for the Smart Home Controller mobile application.

## 📱 About the App

The Smart Home Controller app is a Flutter-based mobile application that allows you to:
- Control your ESP32-based 4-channel TRIAC smart home controller
- Monitor device states in real-time
- Configure WiFi settings for your controller
- Manage scenes and schedules
- View system information and statistics

## 📦 APK Files

APK files for different versions and build variants will be stored in this directory:

```
app/
├── README.md (this file)
├── release/
│   └── smart-home-controller-v{version}-release.apk
└── debug/
    └── smart-home-controller-v{version}-debug.apk
```

### Current Release

**Latest Version:** 1.0.0

- **Release APK:** Not yet built
- **Debug APK:** Not yet built

## 🛠️ Building the APK

To build the APK from source:

### Prerequisites

1. **Install Flutter SDK**
   ```bash
   # Download from https://flutter.dev/docs/get-started/install
   # Or use a package manager:
   # macOS: brew install flutter
   # Linux: snap install flutter --classic
   ```

2. **Verify Flutter installation**
   ```bash
   flutter doctor
   ```

3. **Install Android SDK and accept licenses**
   ```bash
   flutter doctor --android-licenses
   ```

### Build Steps

1. **Navigate to the Flutter project directory**
   ```bash
   cd "software_Team/flutter project/demo"
   ```

2. **Get dependencies**
   ```bash
   flutter pub get
   ```

3. **Build Release APK**
   ```bash
   flutter build apk --release
   ```
   
   The APK will be generated at:
   ```
   software_Team/flutter project/demo/build/app/outputs/flutter-apk/app-release.apk
   ```

4. **Build Debug APK (for testing)**
   ```bash
   flutter build apk --debug
   ```

5. **Copy APK to this folder**
   ```bash
   # For release build
   cp "software_Team/flutter project/demo/build/app/outputs/flutter-apk/app-release.apk" \
      app/release/smart-home-controller-v1.0.0-release.apk
   
   # For debug build
   cp "software_Team/flutter project/demo/build/app/outputs/flutter-apk/app-debug.apk" \
      app/debug/smart-home-controller-v1.0.0-debug.apk
   ```

### Build Variants

- **Release APK**: Optimized for production use, smaller size, no debug symbols
  ```bash
  flutter build apk --release
  ```

- **Debug APK**: Includes debug symbols, useful for development
  ```bash
  flutter build apk --debug
  ```

- **Profile APK**: For performance profiling
  ```bash
  flutter build apk --profile
  ```

- **Split APKs by ABI** (reduces size for specific architectures):
  ```bash
  flutter build apk --split-per-abi
  ```
  This generates separate APKs for:
  - `app-armeabi-v7a-release.apk` (32-bit ARM)
  - `app-arm64-v8a-release.apk` (64-bit ARM)
  - `app-x86_64-release.apk` (64-bit Intel)

## 📲 Installing the APK

### On Android Device

1. **Enable Installation from Unknown Sources**
   - Go to Settings → Security
   - Enable "Unknown Sources" or "Install Unknown Apps"

2. **Transfer and Install**
   - Transfer the APK to your device via USB, email, or cloud storage
   - Tap the APK file to install
   - Follow the on-screen prompts

### Using ADB (Android Debug Bridge)

```bash
# Install via ADB
adb install app/release/smart-home-controller-v1.0.0-release.apk

# Install and replace existing version
adb install -r app/release/smart-home-controller-v1.0.0-release.apk

# Uninstall
adb uninstall com.example.demo
```

## 🔐 App Signing

For production releases, APKs should be signed with a release keystore:

1. **Create a keystore** (one-time setup):
   ```bash
   keytool -genkey -v -keystore ~/smarthome-release-key.jks \
           -keyalg RSA -keysize 2048 -validity 10000 \
           -alias smarthome
   ```

2. **Configure signing in Android Studio or create `key.properties`**:
   ```properties
   storePassword=<password>
   keyPassword=<password>
   keyAlias=smarthome
   storeFile=<path-to-keystore>
   ```

3. **Build signed release APK**:
   ```bash
   flutter build apk --release
   ```

## 📝 Version History

### Version 1.0.0 (Initial Release)
- Initial release of Smart Home Controller app
- WiFi configuration interface
- Device control functionality
- Real-time status monitoring
- HTTP and WebSocket communication

## 🐛 Troubleshooting

### Build Issues

**Problem:** `flutter: command not found`
- **Solution:** Install Flutter SDK and add it to your PATH

**Problem:** Android SDK not found
- **Solution:** Run `flutter doctor` and follow instructions to install Android SDK

**Problem:** License errors
- **Solution:** Run `flutter doctor --android-licenses` and accept all licenses

### Installation Issues

**Problem:** "App not installed"
- **Solution:** Uninstall any existing version first, or use `adb install -r`

**Problem:** "Parse error"
- **Solution:** Ensure APK is not corrupted and is compatible with your Android version

## 🔗 Related Documentation

- [Flutter Project](../software_Team/flutter%20project/demo/README.md)
- [Firmware Documentation](../firmware/README.md)
- [Installation Guide](../INSTALL.md)
- [Main README](../README.md)

## 📧 Support

For issues or questions:
- Open an issue on [GitHub](https://github.com/Gokul3611/smarthome-controller/issues)
- Check the [project wiki](https://github.com/Gokul3611/smarthome-controller/wiki)

---

**Note:** APK files are typically not committed to Git repositories due to their size. Consider using GitHub Releases or a separate distribution channel for sharing built APKs.
