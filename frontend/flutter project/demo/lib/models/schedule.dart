/// Schedule data model for automating device operations
/// 
/// This class represents a scheduled automation that can turn devices
/// on/off at specific times on specific days of the week.
class Schedule {
  /// Unique identifier for the schedule
  final String id;
  
  /// User ID who owns this schedule
  final String userId;
  
  /// Device UID this schedule controls
  final String deviceUid;
  
  /// Channel number (for multi-channel devices)
  final int channel;
  
  /// User-friendly name for the schedule
  final String name;
  
  /// Start time in HH:MM format (24-hour)
  final String startTime;
  
  /// End time in HH:MM format (24-hour)
  final String endTime;
  
  /// Days of week as bitmask string: "1111100" (Mon-Sun, 1=active, 0=inactive)
  final String days;
  
  /// Action to perform: 'on', 'off', 'set'
  final String action;
  
  /// Value to set (brightness/speed) - Range: 0-100
  final int value;
  
  /// Whether the schedule is currently enabled
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

  /// Creates a Schedule instance from JSON data
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

  /// Converts the Schedule instance to JSON
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

  /// Gets a list of active day names for this schedule
  /// 
  /// Returns: List of day abbreviations (e.g., ['Mon', 'Tue', 'Wed'])
  List<String> getDayNames() {
    final dayNames = <String>[];
    const allDays = ['Sun', 'Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat'];
    
    for (var i = 0; i < days.length && i < 7; i++) {
      if (days[i] == '1') {
        dayNames.add(allDays[i]);
      }
    }
    
    return dayNames;
  }
}

/// Scene data model for grouping multiple device states
/// 
/// This class represents a scene that can apply predefined states
/// to multiple devices simultaneously.
class Scene {
  /// Unique identifier for the scene
  final String id;
  
  /// User ID who owns this scene
  final String userId;
  
  /// User-friendly name for the scene
  final String name;
  
  /// List of devices and their states in this scene
  final List<SceneDevice> devices;

  Scene({
    required this.id,
    required this.userId,
    required this.name,
    required this.devices,
  });

  /// Creates a Scene instance from JSON data
  factory Scene.fromJson(Map<String, dynamic> json) {
    var devicesList = <SceneDevice>[];
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

  /// Converts the Scene instance to JSON
  Map<String, dynamic> toJson() {
    return {
      'id': id,
      'user_id': userId,
      'name': name,
      'devices': devices.map((d) => d.toJson()).toList(),
    };
  }
}

/// Device configuration within a scene
/// 
/// This class represents the desired state of a device
/// when a scene is activated.
class SceneDevice {
  /// Device UID
  final String uid;
  
  /// Channel number (for multi-channel devices)
  final int channel;
  
  /// Desired power state
  final bool state;
  
  /// Desired value (brightness/speed) - Range: 0-100
  final int value;

  SceneDevice({
    required this.uid,
    this.channel = 0,
    required this.state,
    this.value = 100,
  });

  /// Creates a SceneDevice instance from JSON data
  factory SceneDevice.fromJson(Map<String, dynamic> json) {
    return SceneDevice(
      uid: json['uid'] ?? '',
      channel: json['channel'] ?? 0,
      state: json['state'] ?? false,
      value: json['value'] ?? 100,
    );
  }

  /// Converts the SceneDevice instance to JSON
  Map<String, dynamic> toJson() {
    return {
      'uid': uid,
      'channel': channel,
      'state': state,
      'value': value,
    };
  }
}
