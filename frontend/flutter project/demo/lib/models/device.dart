/// Device data model representing a smart home device
/// 
/// This class encapsulates all device properties including:
/// - Unique identifier (UID)
/// - Device name and type (light/fan)
/// - Network information (IP, version, RSSI)
/// - Device state (power, value/brightness/speed)
/// - System metrics (uptime, heap memory)
/// - Connection status
class Device {
  /// Unique identifier for the device (MAC address or custom ID)
  final String uid;
  
  /// User-friendly name for the device
  final String name;
  
  /// Device type: 'light' or 'fan'
  final String type;
  
  /// IP address of the device
  final String ip;
  
  /// Firmware version
  final String version;
  
  /// Device uptime in seconds
  final int uptime;
  
  /// WiFi signal strength (RSSI)
  final int rssi;
  
  /// Available heap memory in bytes
  final int heap;
  
  /// Last time the device was seen/updated
  final DateTime? lastSeen;
  
  /// Whether the device is currently online
  final bool online;
  
  /// Device power state (on/off)
  bool power;
  
  /// Brightness (for lights) or speed (for fans) - Range: 0-100
  int value;

  Device({
    required this.uid,
    required this.name,
    required this.type,
    this.ip = '',
    this.version = '1.0',
    this.uptime = 0,
    this.rssi = 0,
    this.heap = 0,
    this.lastSeen,
    this.online = false,
    this.power = false,
    this.value = 0,
  });

  /// Creates a Device instance from JSON data
  factory Device.fromJson(Map<String, dynamic> json) {
    return Device(
      uid: json['uid'] ?? '',
      name: json['name'] ?? 'Unknown Device',
      type: json['type'] ?? 'light',
      ip: json['ip'] ?? '',
      version: json['version'] ?? '1.0',
      uptime: json['uptime'] ?? 0,
      rssi: json['rssi'] ?? 0,
      heap: json['heap'] ?? 0,
      lastSeen: json['last_seen'] != null 
          ? DateTime.tryParse(json['last_seen'].toString())
          : null,
      online: json['online'] ?? false,
      power: json['power'] ?? false,
      value: json['value'] ?? 0,
    );
  }

  /// Converts the Device instance to JSON
  Map<String, dynamic> toJson() {
    return {
      'uid': uid,
      'name': name,
      'type': type,
      'ip': ip,
      'version': version,
      'uptime': uptime,
      'rssi': rssi,
      'heap': heap,
      'last_seen': lastSeen?.toIso8601String(),
      'online': online,
      'power': power,
      'value': value,
    };
  }

  /// Creates a copy of this Device with specified fields updated
  Device copyWith({
    String? uid,
    String? name,
    String? type,
    String? ip,
    String? version,
    int? uptime,
    int? rssi,
    int? heap,
    DateTime? lastSeen,
    bool? online,
    bool? power,
    int? value,
  }) {
    return Device(
      uid: uid ?? this.uid,
      name: name ?? this.name,
      type: type ?? this.type,
      ip: ip ?? this.ip,
      version: version ?? this.version,
      uptime: uptime ?? this.uptime,
      rssi: rssi ?? this.rssi,
      heap: heap ?? this.heap,
      lastSeen: lastSeen ?? this.lastSeen,
      online: online ?? this.online,
      power: power ?? this.power,
      value: value ?? this.value,
    );
  }
}
