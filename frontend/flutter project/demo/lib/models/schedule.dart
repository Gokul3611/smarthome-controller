class Schedule {
  final String id;
  final String userId;
  final String deviceUid;
  final int channel;
  final String name;
  final String startTime; // HH:MM format
  final String endTime; // HH:MM format
  final String days; // Bitmask string: "1111100" (Mon-Fri)
  final String action; // 'on', 'off', 'set'
  final int value; // 0-100
  final bool enabled;

  Schedule({
    required this.id,
    required this.userId,
    required this.deviceUid,
    this.channel = 0,
    required this.name,
    required this.startTime,
    required this.endTime,
    required this.days,
    this.action = 'on',
    this.value = 100,
    this.enabled = true,
  });

  factory Schedule.fromJson(Map<String, dynamic> json) {
    return Schedule(
      id: json['id'] ?? '',
      userId: json['user_id'] ?? '',
      deviceUid: json['device_uid'] ?? '',
      channel: json['channel'] ?? 0,
      name: json['name'] ?? '',
      startTime: json['start_time'] ?? '00:00',
      endTime: json['end_time'] ?? '00:00',
      days: json['days'] ?? '0000000',
      action: json['action'] ?? 'on',
      value: json['value'] ?? 100,
      enabled: json['enabled'] ?? true,
    );
  }

  Map<String, dynamic> toJson() {
    return {
      'id': id,
      'user_id': userId,
      'device_uid': deviceUid,
      'channel': channel,
      'name': name,
      'start_time': startTime,
      'end_time': endTime,
      'days': days,
      'action': action,
      'value': value,
      'enabled': enabled,
    };
  }

  List<String> getDayNames() {
    List<String> dayNames = [];
    List<String> allDays = ['Sun', 'Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat'];
    
    for (int i = 0; i < days.length && i < 7; i++) {
      if (days[i] == '1') {
        dayNames.add(allDays[i]);
      }
    }
    
    return dayNames;
  }
}

class Scene {
  final String id;
  final String userId;
  final String name;
  final List<SceneDevice> devices;

  Scene({
    required this.id,
    required this.userId,
    required this.name,
    required this.devices,
  });

  factory Scene.fromJson(Map<String, dynamic> json) {
    List<SceneDevice> devicesList = [];
    if (json['devices'] != null && json['devices'] is List) {
      devicesList = (json['devices'] as List)
          .map((d) => SceneDevice.fromJson(d))
          .toList();
    }

    return Scene(
      id: json['id'] ?? '',
      userId: json['user_id'] ?? '',
      name: json['name'] ?? '',
      devices: devicesList,
    );
  }

  Map<String, dynamic> toJson() {
    return {
      'id': id,
      'user_id': userId,
      'name': name,
      'devices': devices.map((d) => d.toJson()).toList(),
    };
  }
}

class SceneDevice {
  final String uid;
  final int channel;
  final bool state;
  final int value;

  SceneDevice({
    required this.uid,
    this.channel = 0,
    required this.state,
    this.value = 100,
  });

  factory SceneDevice.fromJson(Map<String, dynamic> json) {
    return SceneDevice(
      uid: json['uid'] ?? '',
      channel: json['channel'] ?? 0,
      state: json['state'] ?? false,
      value: json['value'] ?? 100,
    );
  }

  Map<String, dynamic> toJson() {
    return {
      'uid': uid,
      'channel': channel,
      'state': state,
      'value': value,
    };
  }
}
