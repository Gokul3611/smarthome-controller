class Device {
  final String uid;
  final String name;
  final String type; // 'light' or 'fan'
  final String ip;
  final String version;
  final int uptime;
  final int rssi;
  final int heap;
  final DateTime? lastSeen;
  final bool online;
  
  // Device state
  bool power;
  int value; // brightness/speed 0-100

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
