import 'dart:convert';
import 'package:http/http.dart' as http;
import '../config/api_config.dart';
import '../models/device.dart';
import '../models/schedule.dart';
import '../models/user.dart';

/// Service class for handling all API communications with the backend
/// 
/// This class follows the Singleton pattern to ensure only one instance
/// exists throughout the application lifecycle.
class ApiService {
  // Singleton pattern
  static final ApiService _instance = ApiService._internal();
  factory ApiService() => _instance;
  ApiService._internal();

  /// Common headers for all API requests
  Map<String, String> get _headers => {
        'Content-Type': 'application/json',
      };

  /// Handles HTTP errors and converts them to user-friendly messages
  String _handleHttpError(int statusCode) {
    switch (statusCode) {
      case 400:
        return 'Bad request. Please check your input.';
      case 401:
        return 'Authentication failed. Please login again.';
      case 403:
        return 'Access denied.';
      case 404:
        return 'Resource not found.';
      case 500:
        return 'Server error. Please try again later.';
      case 503:
        return 'Service unavailable. Please try again later.';
      default:
        return 'Network error occurred. Please check your connection.';
    }
  }

  /// Logs debug information in development mode
  void _logDebug(String message) {
    // Only log in debug mode
    assert(() {
      print('[ApiService] $message');
      return true;
    }());
  }

  /// Checks if the response body is valid JSON
  bool _isValidJsonResponse(String? body) {
    if (body == null || body.isEmpty) {
      return false;
    }
    final trimmed = body.trim();
    if (trimmed.isEmpty) {
      return false;
    }
    return trimmed.startsWith('{') || trimmed.startsWith('[');
  }

  // ==================== Authentication APIs ====================

  /// Authenticates user with email and password
  /// 
  /// Returns a Map containing:
  /// - success: bool
  /// - user: Map with user details (if successful)
  /// - error: String (if failed)
  Future<Map<String, dynamic>> login(String email, String password) async {
    try {
      _logDebug('Login attempt for: $email');

      final response = await http.post(
        Uri.parse(ApiConfig.baseUrl),
        headers: _headers,
        body: jsonEncode({
          'action': 'login',
          'api_key': ApiConfig.apiKey,
          'email': email,
          'password': password,
        }),
      ).timeout(ApiConfig.timeout);

      _logDebug('Login response status: ${response.statusCode}');
      _logDebug('Login response body: ${response.body.substring(0, response.body.length > 100 ? 100 : response.body.length)}');

      // Check if response status is OK
      if (response.statusCode != 200) {
        return {'success': false, 'error': _handleHttpError(response.statusCode)};
      }

      // Check if response is JSON
      if (!_isValidJsonResponse(response.body)) {
        _logDebug('Response is not JSON: ${response.body.substring(0, response.body.length > 50 ? 50 : response.body.length)}');
        return {'success': false, 'error': 'Server error. Invalid response format.'};
      }

      // Parse JSON response with additional safety
      try {
        final data = jsonDecode(response.body);
        _logDebug('Login response: ${data['success']}');
        return data;
      } on FormatException catch (e) {
        _logDebug('JSON decode failed even after validation: $e');
        return {'success': false, 'error': 'Server returned invalid data. Please try again.'};
      }
    } on http.ClientException catch (e) {
      _logDebug('Client error during login: $e');
      return {'success': false, 'error': 'Network error. Please check your connection.'};
    } catch (e) {
      _logDebug('Error during login: $e');
      return {'success': false, 'error': 'An error occurred. Please try again.'};
    }
  }

  /// Registers a new user account
  /// 
  /// Returns a Map containing:
  /// - success: bool
  /// - user: Map with user details (if successful)
  /// - error: String (if failed)
  Future<Map<String, dynamic>> signup(
      String name, String email, String password) async {
    try {
      _logDebug('Signup attempt for: $email');

      final response = await http.post(
        Uri.parse(ApiConfig.baseUrl),
        headers: _headers,
        body: jsonEncode({
          'action': 'signup',
          'api_key': ApiConfig.apiKey,
          'name': name,
          'email': email,
          'password': password,
        }),
      ).timeout(ApiConfig.timeout);

      _logDebug('Signup response status: ${response.statusCode}');

      // Check if response status is OK
      if (response.statusCode != 200) {
        return {'success': false, 'error': _handleHttpError(response.statusCode)};
      }

      // Check if response is JSON
      if (!_isValidJsonResponse(response.body)) {
        _logDebug('Signup response is not JSON');
        return {'success': false, 'error': 'Server error. Invalid response format.'};
      }

      // Parse JSON response with additional safety
      try {
        final data = jsonDecode(response.body);
        _logDebug('Signup response: ${data['success']}');
        return data;
      } on FormatException catch (e) {
        _logDebug('JSON decode failed even after validation: $e');
        return {'success': false, 'error': 'Server returned invalid data. Please try again.'};
      }
    } on http.ClientException catch (e) {
      _logDebug('Client error during signup: $e');
      return {'success': false, 'error': 'Network error. Please check your connection.'};
    } catch (e) {
      _logDebug('Error during signup: $e');
      return {'success': false, 'error': 'An error occurred. Please try again.'};
    }
  }

  // ==================== Device APIs ====================

  /// Fetches all devices for a specific user
  /// 
  /// Returns a List of Device objects
  /// Returns empty list if request fails
  Future<List<Device>> getDevices(String userId) async {
    try {
      _logDebug('Fetching devices for user: $userId');

      final url = Uri.parse(
          '${ApiConfig.baseUrl}?action=get_devices&api_key=${ApiConfig.apiKey}&user_id=$userId');

      final response = await http.get(url).timeout(ApiConfig.timeout);

      if (response.statusCode != 200) {
        _logDebug('Error fetching devices: ${response.statusCode}');
        return [];
      }

      // Check if response is JSON
      if (!_isValidJsonResponse(response.body)) {
        _logDebug('Devices response is not JSON');
        return [];
      }

      final data = jsonDecode(response.body);

      if (data['success'] == true && data['devices'] != null) {
        final devices = (data['devices'] as List)
            .map((device) => Device.fromJson(device))
            .toList();
        _logDebug('Fetched ${devices.length} devices');
        return devices;
      }

      return [];
    } on FormatException catch (e) {
      _logDebug('JSON parse error getting devices: $e');
      return [];
    } catch (e) {
      _logDebug('Error getting devices: $e');
      return [];
    }
  }

  /// Updates device properties (name, type)
  /// 
  /// Returns true if successful, false otherwise
  Future<bool> updateDevice(String uid, String name, String type) async {
    try {
      _logDebug('Updating device: $uid');

      final response = await http.post(
        Uri.parse(ApiConfig.baseUrl),
        headers: _headers,
        body: jsonEncode({
          'action': 'update_device',
          'api_key': ApiConfig.apiKey,
          'uid': uid,
          'name': name,
          'type': type,
        }),
      ).timeout(ApiConfig.timeout);

      if (response.statusCode != 200) {
        return false;
      }

      // Check if response is JSON
      if (!_isValidJsonResponse(response.body)) {
        return false;
      }

      final data = jsonDecode(response.body);
      _logDebug('Update device response: ${data['success']}');

      return data['success'] == true;
    } on FormatException catch (e) {
      _logDebug('JSON parse error updating device: $e');
      return false;
    } catch (e) {
      _logDebug('Error updating device: $e');
      return false;
    }
  }

  /// Updates device state (power, brightness/speed)
  /// 
  /// Returns true if successful, false otherwise
  Future<bool> updateDeviceState(
      String uid, int channel, bool state, int value) async {
    try {
      _logDebug('Updating device state: $uid, channel: $channel, state: $state, value: $value');

      final response = await http.post(
        Uri.parse(ApiConfig.baseUrl),
        headers: _headers,
        body: jsonEncode({
          'action': 'update_state',
          'api_key': ApiConfig.apiKey,
          'uid': uid,
          'states': [
            {
              'channel': channel,
              'state': state,
              'value': value,
            }
          ],
        }),
      ).timeout(ApiConfig.timeout);

      if (response.statusCode != 200) {
        return false;
      }

      // Check if response is JSON
      if (!_isValidJsonResponse(response.body)) {
        return false;
      }

      final data = jsonDecode(response.body);
      return data['success'] == true;
    } on FormatException catch (e) {
      _logDebug('JSON parse error updating device state: $e');
      return false;
    } catch (e) {
      _logDebug('Error updating device state: $e');
      return false;
    }
  }

  /// Resets WiFi configuration for a device
  /// 
  /// Returns true if successful, false otherwise
  Future<bool> resetWifi(String uid) async {
    try {
      _logDebug('Resetting WiFi for device: $uid');

      final response = await http.post(
        Uri.parse(ApiConfig.baseUrl),
        headers: _headers,
        body: jsonEncode({
          'action': 'reset_wifi',
          'api_key': ApiConfig.apiKey,
          'uid': uid,
        }),
      ).timeout(ApiConfig.timeout);

      if (response.statusCode != 200) {
        return false;
      }

      // Check if response is JSON
      if (!_isValidJsonResponse(response.body)) {
        return false;
      }

      final data = jsonDecode(response.body);
      _logDebug('Reset WiFi response: ${data['success']}');

      return data['success'] == true;
    } on FormatException catch (e) {
      _logDebug('JSON parse error resetting WiFi: $e');
      return false;
    } catch (e) {
      _logDebug('Error resetting WiFi: $e');
      return false;
    }
  }

  // ==================== Schedule APIs ====================

  /// Fetches all schedules for a specific user
  /// 
  /// Returns a List of Schedule objects
  /// Returns empty list if request fails
  Future<List<Schedule>> getSchedules(String userId) async {
    try {
      _logDebug('Fetching schedules for user: $userId');

      final url = Uri.parse(
          '${ApiConfig.baseUrl}?action=get_schedules&api_key=${ApiConfig.apiKey}&user_id=$userId');

      final response = await http.get(url).timeout(ApiConfig.timeout);

      if (response.statusCode != 200) {
        _logDebug('Error fetching schedules: ${response.statusCode}');
        return [];
      }

      // Check if response is JSON
      if (!_isValidJsonResponse(response.body)) {
        _logDebug('Schedules response is not JSON');
        return [];
      }

      final data = jsonDecode(response.body);

      if (data['success'] == true && data['schedules'] != null) {
        final schedules = (data['schedules'] as List)
            .map((schedule) => Schedule.fromJson(schedule))
            .toList();
        _logDebug('Fetched ${schedules.length} schedules');
        return schedules;
      }

      return [];
    } on FormatException catch (e) {
      _logDebug('JSON parse error getting schedules: $e');
      return [];
    } catch (e) {
      _logDebug('Error getting schedules: $e');
      return [];
    }
  }

  /// Creates or updates a schedule
  /// 
  /// Returns true if successful, false otherwise
  Future<bool> saveSchedule(Schedule schedule) async {
    try {
      _logDebug('Saving schedule: ${schedule.name}');

      final response = await http.post(
        Uri.parse(ApiConfig.baseUrl),
        headers: _headers,
        body: jsonEncode({
          'action': 'save_schedule',
          'api_key': ApiConfig.apiKey,
          'schedule': schedule.toJson(),
        }),
      ).timeout(ApiConfig.timeout);

      if (response.statusCode != 200) {
        return false;
      }

      // Check if response is JSON
      if (!_isValidJsonResponse(response.body)) {
        return false;
      }

      final data = jsonDecode(response.body);
      _logDebug('Save schedule response: ${data['success']}');

      return data['success'] == true;
    } on FormatException catch (e) {
      _logDebug('JSON parse error saving schedule: $e');
      return false;
    } catch (e) {
      _logDebug('Error saving schedule: $e');
      return false;
    }
  }

  /// Deletes a schedule
  /// 
  /// Returns true if successful, false otherwise
  Future<bool> deleteSchedule(String scheduleId) async {
    try {
      _logDebug('Deleting schedule: $scheduleId');

      final response = await http.post(
        Uri.parse(ApiConfig.baseUrl),
        headers: _headers,
        body: jsonEncode({
          'action': 'delete_schedule',
          'api_key': ApiConfig.apiKey,
          'schedule_id': scheduleId,
        }),
      ).timeout(ApiConfig.timeout);

      if (response.statusCode != 200) {
        return false;
      }

      // Check if response is JSON
      if (!_isValidJsonResponse(response.body)) {
        return false;
      }

      final data = jsonDecode(response.body);
      _logDebug('Delete schedule response: ${data['success']}');

      return data['success'] == true;
    } on FormatException catch (e) {
      _logDebug('JSON parse error deleting schedule: $e');
      return false;
    } catch (e) {
      _logDebug('Error deleting schedule: $e');
      return false;
    }
  }

  // ==================== Scene APIs ====================

  /// Fetches all scenes for a specific user
  /// 
  /// Returns a List of Scene objects
  /// Returns empty list if request fails
  Future<List<Scene>> getScenes(String userId) async {
    try {
      _logDebug('Fetching scenes for user: $userId');

      final url = Uri.parse(
          '${ApiConfig.baseUrl}?action=get_scenes&api_key=${ApiConfig.apiKey}&user_id=$userId');

      final response = await http.get(url).timeout(ApiConfig.timeout);

      if (response.statusCode != 200) {
        _logDebug('Error fetching scenes: ${response.statusCode}');
        return [];
      }

      // Check if response is JSON
      if (!_isValidJsonResponse(response.body)) {
        _logDebug('Scenes response is not JSON');
        return [];
      }

      final data = jsonDecode(response.body);

      if (data['success'] == true && data['scenes'] != null) {
        final scenes = (data['scenes'] as List)
            .map((scene) => Scene.fromJson(scene))
            .toList();
        _logDebug('Fetched ${scenes.length} scenes');
        return scenes;
      }

      return [];
    } on FormatException catch (e) {
      _logDebug('JSON parse error getting scenes: $e');
      return [];
    } catch (e) {
      _logDebug('Error getting scenes: $e');
      return [];
    }
  }

  /// Creates or updates a scene
  /// 
  /// Returns true if successful, false otherwise
  Future<bool> saveScene(Scene scene) async {
    try {
      _logDebug('Saving scene: ${scene.name}');

      final response = await http.post(
        Uri.parse(ApiConfig.baseUrl),
        headers: _headers,
        body: jsonEncode({
          'action': 'save_scene',
          'api_key': ApiConfig.apiKey,
          'scene': scene.toJson(),
        }),
      ).timeout(ApiConfig.timeout);

      if (response.statusCode != 200) {
        return false;
      }

      // Check if response is JSON
      if (!_isValidJsonResponse(response.body)) {
        return false;
      }

      final data = jsonDecode(response.body);
      _logDebug('Save scene response: ${data['success']}');

      return data['success'] == true;
    } on FormatException catch (e) {
      _logDebug('JSON parse error saving scene: $e');
      return false;
    } catch (e) {
      _logDebug('Error saving scene: $e');
      return false;
    }
  }

  /// Activates a scene (applies device states)
  /// 
  /// Returns true if successful, false otherwise
  Future<bool> activateScene(String sceneId) async {
    try {
      _logDebug('Activating scene: $sceneId');

      final response = await http.post(
        Uri.parse(ApiConfig.baseUrl),
        headers: _headers,
        body: jsonEncode({
          'action': 'activate_scene',
          'api_key': ApiConfig.apiKey,
          'scene_id': sceneId,
        }),
      ).timeout(ApiConfig.timeout);

      if (response.statusCode != 200) {
        return false;
      }

      // Check if response is JSON
      if (!_isValidJsonResponse(response.body)) {
        return false;
      }

      final data = jsonDecode(response.body);
      _logDebug('Activate scene response: ${data['success']}');

      return data['success'] == true;
    } on FormatException catch (e) {
      _logDebug('JSON parse error activating scene: $e');
      return false;
    } catch (e) {
      _logDebug('Error activating scene: $e');
      return false;
    }
  }

  /// Deletes a scene
  /// 
  /// Returns true if successful, false otherwise
  Future<bool> deleteScene(String sceneId) async {
    try {
      _logDebug('Deleting scene: $sceneId');

      final response = await http.post(
        Uri.parse(ApiConfig.baseUrl),
        headers: _headers,
        body: jsonEncode({
          'action': 'delete_scene',
          'api_key': ApiConfig.apiKey,
          'scene_id': sceneId,
        }),
      ).timeout(ApiConfig.timeout);

      if (response.statusCode != 200) {
        return false;
      }

      // Check if response is JSON
      if (!_isValidJsonResponse(response.body)) {
        return false;
      }

      final data = jsonDecode(response.body);
      _logDebug('Delete scene response: ${data['success']}');

      return data['success'] == true;
    } on FormatException catch (e) {
      _logDebug('JSON parse error deleting scene: $e');
      return false;
    } catch (e) {
      _logDebug('Error deleting scene: $e');
      return false;
    }
  }
}
