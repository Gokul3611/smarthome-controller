import 'dart:convert';
import 'package:http/http.dart' as http;
import '../config/api_config.dart';
import '../models/device.dart';
import '../models/schedule.dart';
import '../models/user.dart';

class ApiService {
  static final ApiService _instance = ApiService._internal();
  factory ApiService() => _instance;
  ApiService._internal();

  // User login
  Future<Map<String, dynamic>> login(String email, String password) async {
    try {
      final response = await http.post(
        Uri.parse(ApiConfig.baseUrl),
        headers: {'Content-Type': 'application/json'},
        body: jsonEncode({
          'action': 'login',
          'api_key': ApiConfig.apiKey,
          'email': email,
          'password': password,
        }),
      ).timeout(ApiConfig.timeout);

      final data = jsonDecode(response.body);
      return data;
    } catch (e) {
      return {'success': false, 'error': e.toString()};
    }
  }

  // User signup
  Future<Map<String, dynamic>> signup(
      String name, String email, String password) async {
    try {
      final response = await http.post(
        Uri.parse(ApiConfig.baseUrl),
        headers: {'Content-Type': 'application/json'},
        body: jsonEncode({
          'action': 'signup',
          'api_key': ApiConfig.apiKey,
          'name': name,
          'email': email,
          'password': password,
        }),
      ).timeout(ApiConfig.timeout);

      final data = jsonDecode(response.body);
      return data;
    } catch (e) {
      return {'success': false, 'error': e.toString()};
    }
  }

  // Get all devices
  Future<List<Device>> getDevices(String userId) async {
    try {
      final url = Uri.parse(
          '${ApiConfig.baseUrl}?action=get_devices&api_key=${ApiConfig.apiKey}&user_id=$userId');

      final response = await http.get(url).timeout(ApiConfig.timeout);

      final data = jsonDecode(response.body);

      if (data['success'] == true && data['devices'] != null) {
        return (data['devices'] as List)
            .map((device) => Device.fromJson(device))
            .toList();
      }

      return [];
    } catch (e) {
      print('Error getting devices: $e');
      return [];
    }
  }

  // Update device name and type
  Future<bool> updateDevice(
      String uid, String name, String type) async {
    try {
      final response = await http.post(
        Uri.parse(ApiConfig.baseUrl),
        headers: {'Content-Type': 'application/json'},
        body: jsonEncode({
          'action': 'update_device',
          'api_key': ApiConfig.apiKey,
          'uid': uid,
          'name': name,
          'type': type,
        }),
      ).timeout(ApiConfig.timeout);

      final data = jsonDecode(response.body);
      return data['success'] == true;
    } catch (e) {
      print('Error updating device: $e');
      return false;
    }
  }

  // Update device state
  Future<bool> updateDeviceState(
      String uid, int channel, bool state, int value) async {
    try {
      final response = await http.post(
        Uri.parse(ApiConfig.baseUrl),
        headers: {'Content-Type': 'application/json'},
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

      final data = jsonDecode(response.body);
      return data['success'] == true;
    } catch (e) {
      print('Error updating device state: $e');
      return false;
    }
  }

  // Reset WiFi
  Future<bool> resetWifi(String uid) async {
    try {
      final response = await http.post(
        Uri.parse(ApiConfig.baseUrl),
        headers: {'Content-Type': 'application/json'},
        body: jsonEncode({
          'action': 'reset_wifi',
          'api_key': ApiConfig.apiKey,
          'uid': uid,
        }),
      ).timeout(ApiConfig.timeout);

      final data = jsonDecode(response.body);
      return data['success'] == true;
    } catch (e) {
      print('Error resetting WiFi: $e');
      return false;
    }
  }

  // Get schedules
  Future<List<Schedule>> getSchedules(String userId) async {
    try {
      final url = Uri.parse(
          '${ApiConfig.baseUrl}?action=get_schedules&api_key=${ApiConfig.apiKey}&user_id=$userId');

      final response = await http.get(url).timeout(ApiConfig.timeout);

      final data = jsonDecode(response.body);

      if (data['success'] == true && data['schedules'] != null) {
        return (data['schedules'] as List)
            .map((schedule) => Schedule.fromJson(schedule))
            .toList();
      }

      return [];
    } catch (e) {
      print('Error getting schedules: $e');
      return [];
    }
  }

  // Save schedule
  Future<bool> saveSchedule(Schedule schedule) async {
    try {
      final response = await http.post(
        Uri.parse(ApiConfig.baseUrl),
        headers: {'Content-Type': 'application/json'},
        body: jsonEncode({
          'action': 'save_schedule',
          'api_key': ApiConfig.apiKey,
          'schedule': schedule.toJson(),
        }),
      ).timeout(ApiConfig.timeout);

      final data = jsonDecode(response.body);
      return data['success'] == true;
    } catch (e) {
      print('Error saving schedule: $e');
      return false;
    }
  }

  // Delete schedule
  Future<bool> deleteSchedule(String scheduleId) async {
    try {
      final response = await http.post(
        Uri.parse(ApiConfig.baseUrl),
        headers: {'Content-Type': 'application/json'},
        body: jsonEncode({
          'action': 'delete_schedule',
          'api_key': ApiConfig.apiKey,
          'schedule_id': scheduleId,
        }),
      ).timeout(ApiConfig.timeout);

      final data = jsonDecode(response.body);
      return data['success'] == true;
    } catch (e) {
      print('Error deleting schedule: $e');
      return false;
    }
  }

  // Get scenes
  Future<List<Scene>> getScenes(String userId) async {
    try {
      final url = Uri.parse(
          '${ApiConfig.baseUrl}?action=get_scenes&api_key=${ApiConfig.apiKey}&user_id=$userId');

      final response = await http.get(url).timeout(ApiConfig.timeout);

      final data = jsonDecode(response.body);

      if (data['success'] == true && data['scenes'] != null) {
        return (data['scenes'] as List)
            .map((scene) => Scene.fromJson(scene))
            .toList();
      }

      return [];
    } catch (e) {
      print('Error getting scenes: $e');
      return [];
    }
  }

  // Save scene
  Future<bool> saveScene(Scene scene) async {
    try {
      final response = await http.post(
        Uri.parse(ApiConfig.baseUrl),
        headers: {'Content-Type': 'application/json'},
        body: jsonEncode({
          'action': 'save_scene',
          'api_key': ApiConfig.apiKey,
          'scene': scene.toJson(),
        }),
      ).timeout(ApiConfig.timeout);

      final data = jsonDecode(response.body);
      return data['success'] == true;
    } catch (e) {
      print('Error saving scene: $e');
      return false;
    }
  }

  // Delete scene
  Future<bool> deleteScene(String sceneId) async {
    try {
      final response = await http.post(
        Uri.parse(ApiConfig.baseUrl),
        headers: {'Content-Type': 'application/json'},
        body: jsonEncode({
          'action': 'delete_scene',
          'api_key': ApiConfig.apiKey,
          'scene_id': sceneId,
        }),
      ).timeout(ApiConfig.timeout);

      final data = jsonDecode(response.body);
      return data['success'] == true;
    } catch (e) {
      print('Error deleting scene: $e');
      return false;
    }
  }
}
