# Smart Home Controller - Flutter App Guide

## Overview

Professional Flutter mobile application for Smart Home Controller with full dashboard functionality matching the web interface.

## Features

### ✅ **Dashboard**
- **System Statistics**: Real-time display of total devices, active devices, schedules, and scenes
- **Connection Status**: Live connection indicator
- **Device Control**: Full control of all connected devices
- **Real-time Updates**: Auto-refresh every 3 seconds

### ✅ **Device Management**
- View all connected devices with online/offline status
- Toggle device power on/off
- Adjust brightness (lights) / speed (fans) with slider
- Edit device name and type
- Device status badges with real-time updates
- Professional glassmorphism UI

### ✅ **Schedules**
- Create automated schedules for devices
- Set start and end times
- Configure days of the week
- View all schedules with device information
- Delete schedules

### ✅ **Scenes**
- Create custom scenes for quick device control
- Activate scenes with one tap
- View all configured scenes
- Delete scenes

### ✅ **Settings**
- WiFi reset for devices
- Professional confirmation dialogs
- Safety warnings

### ✅ **Authentication**
- Professional login screen with validation
- Signup with full form validation
- Forgot password with professional email handling
- Error handling and user feedback
- Secure password fields with show/hide toggle

### ✅ **UI/UX**
- Modern glassmorphism design
- Smooth animations and transitions
- Responsive layout for all screen sizes
- Professional color scheme matching web dashboard
- Consistent navigation with back buttons
- Loading states and error handling

## Getting Started

### Prerequisites

- Flutter SDK 3.10.0 or higher
- Dart SDK
- Android Studio or VS Code with Flutter extensions
- Android device or emulator for testing

### Installation

1. **Clone the repository**
   ```bash
   cd frontend/flutter\ project/demo
   ```

2. **Install dependencies**
   ```bash
   flutter pub get
   ```

3. **Configure API endpoint**
   
   Edit `lib/config/api_config.dart`:
   ```dart
   class ApiConfig {
     static const String baseUrl = 'YOUR_API_ENDPOINT_HERE';
     static const String apiKey = 'YOUR_API_KEY_HERE';
     static const Duration timeout = Duration(seconds: 30);
   }
   ```

4. **Run the app**
   ```bash
   flutter run
   ```

### Build for Production

#### Android APK
```bash
flutter build apk --release
```

The APK will be generated at: `build/app/outputs/flutter-apk/app-release.apk`

#### Android App Bundle (for Play Store)
```bash
flutter build appbundle --release
```

#### iOS
```bash
flutter build ios --release
```

## Project Structure

```
lib/
├── config/
│   ├── api_config.dart         # API configuration
│   └── app_theme.dart          # Theme and styling constants
├── models/
│   ├── device.dart             # Device data model
│   ├── schedule.dart           # Schedule and Scene models
│   └── user.dart               # User data model
├── pages/
│   ├── dashboard_page.dart     # Main dashboard with tabs
│   ├── login_screen.dart       # Login page
│   ├── signup_screen.dart      # Sign up page
│   ├── forgot_password_screen.dart  # Password recovery
│   ├── home_page.dart          # Navigation hub
│   ├── devices_page.dart       # Device list
│   ├── connection_page.dart    # Network information
│   ├── profile_page.dart       # User profile
│   ├── fan_page.dart           # Fan control
│   ├── light_page.dart         # Light control
│   └── faq_page.dart           # FAQ page
├── services/
│   └── api_service.dart        # API integration
└── main.dart                   # App entry point
```

## API Integration

The app communicates with the Google Apps Script backend through the `ApiService` class.

### Available API Methods

- `login(email, password)` - User authentication
- `signup(name, email, password)` - User registration
- `getDevices(userId)` - Fetch all devices
- `updateDevice(uid, name, type)` - Update device properties
- `updateDeviceState(uid, channel, state, value)` - Control device
- `resetWifi(uid)` - Reset device WiFi
- `getSchedules(userId)` - Fetch schedules
- `saveSchedule(schedule)` - Create/update schedule
- `deleteSchedule(scheduleId)` - Delete schedule
- `getScenes(userId)` - Fetch scenes
- `saveScene(scene)` - Create/update scene
- `activateScene(sceneId)` - Activate a scene
- `deleteScene(sceneId)` - Delete scene

## Features Implementation

### Dashboard Page

The dashboard is the heart of the app with three tabs:

**1. Devices Tab**
- Grid/list view of all devices
- Real-time status updates
- Quick controls (power, brightness/speed)
- Edit device properties

**2. Schedules Tab**
- List of all schedules
- Create new schedules with time picker
- Device selection dropdown
- Delete functionality

**3. Scenes Tab**
- List of all scenes
- One-tap scene activation
- Create new scenes
- Delete functionality

### State Management

The app uses StatefulWidget with setState for state management:
- Device states refresh every 3 seconds
- Tab-based navigation with TabController
- Form validation with GlobalKey<FormState>
- Loading states for async operations

### Error Handling

- Network error detection with try-catch
- User-friendly error messages
- Validation on all forms
- Connection status indicator
- Loading spinners for async operations

### Professional UI Elements

- **Glassmorphism**: Frosted glass effect with backdrop blur
- **Gradients**: Beautiful color gradients for buttons and icons
- **Shadows**: Proper elevation and shadows
- **Animations**: Smooth transitions and hover effects
- **Typography**: Consistent font sizing and weights
- **Colors**: Professional dark theme with accent colors

## Customization

### Theme Colors

Edit `lib/config/app_theme.dart` to customize:

```dart
class AppTheme {
  // Primary colors
  static const Color primaryIndigo = Color(0xFF6366F1);
  static const Color primaryPurple = Color(0xFF8B5CF6);
  
  // Background colors
  static const Color backgroundDark = Color(0xFF0F172A);
  static const Color backgroundMedium = Color(0xFF1E293B);
  
  // Status colors
  static const Color successGreen = Color(0xFF10B981);
  static const Color errorRed = Color(0xFFEF4444);
  static const Color warningOrange = Color(0xFFF59E0B);
  
  // Text colors
  static const Color textPrimary = Color(0xFFF8FAFC);
  static const Color textSecondary = Color(0xFF94A3B8);
}
```

### API Timeout

Edit `lib/config/api_config.dart`:

```dart
static const Duration timeout = Duration(seconds: 30);
```

### Auto-Refresh Interval

Edit `lib/pages/dashboard_page.dart`:

```dart
Timer.periodic(const Duration(seconds: 3), (timer) {
  // Change to desired interval
});
```

## Testing

### Run Tests
```bash
flutter test
```

### Run Integration Tests
```bash
flutter drive --target=test_driver/app.dart
```

## Deployment

### Android

1. **Generate Keystore**
   ```bash
   keytool -genkey -v -keystore ~/upload-keystore.jks -keyalg RSA -keysize 2048 -validity 10000 -alias upload
   ```

2. **Configure Signing**
   
   Create `android/key.properties`:
   ```properties
   storePassword=<password>
   keyPassword=<password>
   keyAlias=upload
   storeFile=<path-to-keystore>
   ```

3. **Build Release**
   ```bash
   flutter build appbundle --release
   ```

### iOS

1. **Configure Xcode**
   - Open `ios/Runner.xcworkspace`
   - Configure signing and capabilities
   - Set app icons and launch screen

2. **Build Release**
   ```bash
   flutter build ios --release
   ```

## Troubleshooting

### Common Issues

**1. Network Connection Error**
- Check API endpoint in `api_config.dart`
- Verify API key is correct
- Check device internet connection
- Check API is accessible

**2. Build Errors**
- Run `flutter clean`
- Run `flutter pub get`
- Delete build folders
- Update Flutter SDK: `flutter upgrade`

**3. Device Not Connecting**
- Check Bluetooth/WiFi permissions in `AndroidManifest.xml`
- Grant necessary permissions on device
- Check device compatibility

**4. UI Not Updating**
- Check setState() calls
- Verify API responses
- Check timer is running
- Debug with print statements

### Debug Mode

Run with verbose logging:
```bash
flutter run -v
```

Check logs:
```bash
flutter logs
```

## Performance Optimization

- Use `const` constructors where possible
- Implement efficient state management
- Optimize image assets
- Minimize widget rebuilds
- Use `ListView.builder` for long lists
- Implement pagination for large datasets

## Security Best Practices

- ✅ Input validation on all forms
- ✅ Secure password handling (no plaintext storage)
- ✅ API key protection (not hardcoded in production)
- ✅ HTTPS communication
- ✅ Error message sanitization
- ✅ Timeout on API calls

## Contributing

1. Follow Flutter best practices
2. Use proper naming conventions
3. Add comments for complex logic
4. Test on multiple devices
5. Update documentation

## License

MIT License - See LICENSE file for details

## Support

For issues and questions:
- GitHub Issues: [Repository Issues](https://github.com/Gokul3611/smarthome-controller/issues)
- Documentation: [Main README](../../../README.md)

## Version History

### v1.0.0 (Current)
- ✅ Full dashboard implementation
- ✅ Device management
- ✅ Schedules and scenes
- ✅ Professional authentication
- ✅ Real-time updates
- ✅ Settings and WiFi reset
- ✅ Production-ready UI

## Acknowledgments

- Flutter team for the excellent framework
- Material Design guidelines
- Community packages: http, connectivity_plus, network_info_plus, device_info_plus
