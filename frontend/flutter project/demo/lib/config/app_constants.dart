/// Application-wide constants
/// 
/// This file contains all magic numbers, durations, limits, and other
/// constant values used throughout the application to improve maintainability.
class AppConstants {
  // Prevent instantiation
  AppConstants._();

  // ==================== Device Constants ====================
  
  /// Maximum length for device names
  static const int maxDeviceNameLength = 30;
  
  /// Minimum brightness/speed value
  static const int minDeviceValue = 0;
  
  /// Maximum brightness/speed value
  static const int maxDeviceValue = 100;
  
  /// Maximum number of devices per user
  static const int maxDevices = 50;
  
  /// Device online timeout in milliseconds
  static const int deviceOnlineTimeout = 10000; // 10 seconds

  // ==================== Schedule Constants ====================
  
  /// Maximum length for schedule names
  static const int maxScheduleNameLength = 50;
  
  /// Maximum number of schedules per user
  static const int maxSchedules = 100;
  
  /// Days of week representation
  static const List<String> daysOfWeek = [
    'Sun',
    'Mon',
    'Tue',
    'Wed',
    'Thu',
    'Fri',
    'Sat',
  ];

  // ==================== Scene Constants ====================
  
  /// Maximum length for scene names
  static const int maxSceneNameLength = 50;
  
  /// Maximum length for scene descriptions
  static const int maxSceneDescriptionLength = 200;
  
  /// Maximum number of scenes per user
  static const int maxScenes = 50;
  
  /// Maximum devices per scene
  static const int maxDevicesPerScene = 20;

  // ==================== User Constants ====================
  
  /// Minimum name length
  static const int minNameLength = 2;
  
  /// Maximum name length
  static const int maxNameLength = 50;
  
  /// Minimum password length
  static const int minPasswordLength = 6;
  
  /// Maximum password length
  static const int maxPasswordLength = 50;

  // ==================== UI Constants ====================
  
  /// Auto-refresh interval for dashboard (milliseconds)
  static const int autoRefreshInterval = 3000; // 3 seconds
  
  /// Default animation duration (milliseconds)
  static const int defaultAnimationDuration = 300;
  
  /// Snackbar display duration (milliseconds)
  static const int snackbarDuration = 4000; // 4 seconds
  
  /// Debounce duration for sliders (milliseconds)
  static const int sliderDebounceDuration = 500;
  
  /// Toast notification duration (milliseconds)
  static const int toastDuration = 3000; // 3 seconds

  // ==================== Network Constants ====================
  
  /// Default API timeout (seconds)
  static const int defaultApiTimeout = 30;
  
  /// Retry attempts for failed requests
  static const int maxRetryAttempts = 3;
  
  /// Delay between retries (milliseconds)
  static const int retryDelay = 1000; // 1 second

  // ==================== Error Messages ====================
  
  static const String errorNetworkConnection = 
      'Network error. Please check your connection.';
  
  static const String errorServerUnreachable = 
      'Unable to reach server. Please try again later.';
  
  static const String errorInvalidCredentials = 
      'Invalid email or password.';
  
  static const String errorUnknown = 
      'An unexpected error occurred. Please try again.';
  
  static const String errorDeviceNotFound = 
      'Device not found or offline.';
  
  static const String errorPermissionDenied = 
      'Permission denied. Please check your account.';

  // ==================== Success Messages ====================
  
  static const String successLogin = 'Welcome back!';
  static const String successSignup = 'Account created successfully!';
  static const String successDeviceUpdated = 'Device updated successfully';
  static const String successScheduleCreated = 'Schedule created successfully';
  static const String successSceneCreated = 'Scene created successfully';
  static const String successSceneActivated = 'Scene activated successfully';
  static const String successWifiReset = 'WiFi reset command sent';

  // ==================== Validation Messages ====================
  
  static const String validationEmailRequired = 'Email is required';
  static const String validationEmailInvalid = 'Enter a valid email';
  static const String validationPasswordRequired = 'Password is required';
  static const String validationPasswordTooShort = 
      'Password must be at least $minPasswordLength characters';
  static const String validationPasswordMismatch = 'Passwords do not match';
  static const String validationNameRequired = 'Name is required';
  static const String validationNameTooShort = 
      'Name must be at least $minNameLength characters';
  static const String validationDeviceNameRequired = 
      'Device name is required';
  static const String validationDeviceNameTooLong = 
      'Device name is too long (max $maxDeviceNameLength characters)';
  static const String validationScheduleNameRequired = 
      'Schedule name is required';
  static const String validationSceneNameRequired = 
      'Scene name is required';

  // ==================== Regex Patterns ====================
  
  /// Email validation pattern
  static final RegExp emailPattern = RegExp(
    r'^[\w-\.]+@([\w-]+\.)+[\w-]{2,4}$',
  );

  // ==================== Default Values ====================
  
  /// Default user ID for guest users
  static const String defaultUserId = 'default';
  
  /// Default device type
  static const String defaultDeviceType = 'light';
  
  /// Default device version
  static const String defaultDeviceVersion = '1.0';
  
  /// Default schedule days (everyday)
  static const String defaultScheduleDays = '1111111';

  // ==================== Asset Paths ====================
  
  // Add asset paths here when assets are added
  // static const String logoPath = 'assets/images/logo.png';

  // ==================== Route Names ====================
  
  static const String routeLogin = '/';
  static const String routeDashboard = '/dashboard';
  static const String routeHome = '/home';
  static const String routeDevices = '/devices';
  static const String routeConnection = '/connection';
  static const String routeProfile = '/profile';
  static const String routeFaq = '/faq';
}
