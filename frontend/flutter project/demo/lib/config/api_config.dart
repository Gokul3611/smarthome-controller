class ApiConfig {
  // Google Apps Script Backend URL
  // Replace with your deployed Apps Script web app URL
  static const String baseUrl = 'https://script.google.com/macros/s/YOUR_DEPLOYMENT_ID/exec';
  
  // API Key (must match the one in Apps Script properties)
  static const String apiKey = 'smarthome-2024';
  
  // WebSocket URL for ESP32 (if using direct connection)
  static const String wsUrl = 'ws://192.168.1.100:81';
  
  // Polling interval for device updates (milliseconds)
  static const int pollInterval = 3000;
  
  // Request timeout
  static const Duration timeout = Duration(seconds: 30);
  
  // App version
  static const String appVersion = '1.0.0';
  
  // GitHub releases URL for APK download
  static const String apkDownloadUrl = 'https://github.com/Gokul3611/smarthome-controller/releases/latest';
}
