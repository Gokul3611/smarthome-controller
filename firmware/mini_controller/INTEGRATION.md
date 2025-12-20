# Integration Guide: Mini LED Controller with Main Dashboard

This guide shows how to integrate the Mini LED Controller with the main Smart Home Controller dashboard.

## Overview

The Mini LED Controller uses the same API format as the main controller, making it easy to integrate with the existing Google Apps Script backend and Flutter app.

## Cloud Integration

### Step 1: Identify Mini Controllers

The mini controller sends a `device_type` field in its cloud sync requests:

```json
{
  "action": "poll",
  "uid": "AA:BB:CC:DD:EE:FF",
  "ver": "1.0",
  "device_type": "mini_controller",
  "uptime": 3600,
  "rssi": -45,
  "heap": 280000,
  "d1": {
    "s": 1,
    "v": 75,
    "t": "LED",
    "runtime": 1800
  }
}
```

### Step 2: Update Google Apps Script Backend

Add mini controller detection to your `Code.gs`:

```javascript
function doPost(e) {
  var params = JSON.parse(e.postData.contents);
  var uid = params.uid;
  var deviceType = params.device_type || "main_controller";
  
  // Handle mini controller
  if (deviceType === "mini_controller") {
    return handleMiniController(params);
  }
  
  // Handle main controller
  return handleMainController(params);
}

function handleMiniController(params) {
  // Get device from database
  var device = getDeviceByUID(params.uid);
  
  // Update device state
  if (device) {
    updateMiniControllerState(device, params);
  } else {
    // Register new mini controller
    registerNewMiniController(params);
  }
  
  // Build response
  var response = {
    sys_name: device ? device.name : "Mini LED Controller",
    d1: {
      update: false,  // Set to true to send commands
      state: 1,       // 1 = ON, 0 = OFF
      val: 100        // Brightness 0-100%
    }
  };
  
  return ContentService.createTextOutput(JSON.stringify(response))
    .setMimeType(ContentService.MimeType.JSON);
}
```

### Step 3: Database Schema

Add mini controller devices to your Google Sheets database:

**Devices Sheet Columns:**
- UID (unique identifier - MAC address)
- Type (`mini_controller` or `main_controller`)
- Name (display name)
- LED State (ON/OFF)
- LED Brightness (0-100)
- Last Seen (timestamp)
- IP Address
- Signal Strength (RSSI)
- Uptime
- Runtime Statistics

## Dashboard Integration

### Option 1: Separate Section for Mini Controllers

In your `Dashboard.html`, add a dedicated section:

```html
<div class="section">
  <h2>💡 Mini LED Controllers</h2>
  <div id="miniControllers"></div>
</div>

<script>
function loadMiniControllers() {
  fetch('YOUR_SCRIPT_URL?action=get_mini_controllers')
    .then(response => response.json())
    .then(devices => {
      const container = document.getElementById('miniControllers');
      container.innerHTML = '';
      
      devices.forEach(device => {
        const card = createMiniControllerCard(device);
        container.appendChild(card);
      });
    });
}

function createMiniControllerCard(device) {
  const card = document.createElement('div');
  card.className = 'device-card';
  card.innerHTML = `
    <div class="device-header">
      <span class="device-icon">💡</span>
      <span class="device-name">${device.name}</span>
    </div>
    <div class="device-controls">
      <button class="toggle-btn ${device.state ? 'on' : 'off'}" 
              onclick="toggleMiniController('${device.uid}', ${!device.state})">
        ${device.state ? 'ON' : 'OFF'}
      </button>
      <input type="range" min="0" max="100" value="${device.brightness}"
             oninput="setMiniControllerBrightness('${device.uid}', this.value)">
      <span class="brightness-value">${device.brightness}%</span>
    </div>
    <div class="device-info">
      <span>IP: ${device.ip}</span>
      <span>Signal: ${device.rssi} dBm</span>
      <span>Runtime: ${formatRuntime(device.runtime)}</span>
    </div>
  `;
  return card;
}

function toggleMiniController(uid, state) {
  fetch('YOUR_SCRIPT_URL', {
    method: 'POST',
    body: JSON.stringify({
      action: 'control_mini',
      uid: uid,
      state: state
    })
  })
  .then(response => response.json())
  .then(() => loadMiniControllers());
}

function setMiniControllerBrightness(uid, brightness) {
  fetch('YOUR_SCRIPT_URL', {
    method: 'POST',
    body: JSON.stringify({
      action: 'control_mini',
      uid: uid,
      state: true,
      brightness: parseInt(brightness)
    })
  });
}
</script>
```

### Option 2: Mixed Device View

Integrate mini controllers alongside main controllers:

```javascript
function loadAllDevices() {
  fetch('YOUR_SCRIPT_URL?action=get_all_devices')
    .then(response => response.json())
    .then(devices => {
      const container = document.getElementById('allDevices');
      container.innerHTML = '';
      
      devices.forEach(device => {
        let card;
        if (device.type === 'mini_controller') {
          card = createMiniControllerCard(device);
        } else {
          card = createMainControllerCard(device);
        }
        container.appendChild(card);
      });
    });
}
```

## Flutter App Integration

### Update API Service

In `lib/services/api_service.dart`:

```dart
class DeviceType {
  static const String mainController = 'main_controller';
  static const String miniController = 'mini_controller';
}

class Device {
  final String uid;
  final String type;
  final String name;
  final bool state;
  final int brightness;
  final String? ip;
  final int? rssi;
  
  Device({
    required this.uid,
    required this.type,
    required this.name,
    required this.state,
    required this.brightness,
    this.ip,
    this.rssi,
  });
  
  bool get isMiniController => type == DeviceType.miniController;
  
  factory Device.fromJson(Map<String, dynamic> json) {
    return Device(
      uid: json['uid'],
      type: json['type'] ?? DeviceType.mainController,
      name: json['name'],
      state: json['state'] == 1,
      brightness: json['brightness'] ?? 100,
      ip: json['ip'],
      rssi: json['rssi'],
    );
  }
}
```

### Update Device List UI

In `lib/pages/devices_page.dart`:

```dart
class DeviceCard extends StatelessWidget {
  final Device device;
  
  @override
  Widget build(BuildContext context) {
    return Card(
      child: ListTile(
        leading: Icon(
          device.isMiniController 
            ? Icons.lightbulb 
            : Icons.power,
          color: device.state ? Colors.green : Colors.grey,
        ),
        title: Text(device.name),
        subtitle: Text(
          device.isMiniController 
            ? 'Mini LED Controller' 
            : 'Main Controller',
        ),
        trailing: Row(
          mainAxisSize: MainAxisSize.min,
          children: [
            if (!device.isMiniController)
              Text('${device.brightness}%'),
            Switch(
              value: device.state,
              onChanged: (value) => _toggleDevice(device),
            ),
          ],
        ),
        onTap: () => _showDeviceDetails(device),
      ),
    );
  }
  
  void _toggleDevice(Device device) {
    ApiService.controlDevice(
      device.uid,
      !device.state,
      device.brightness,
    );
  }
  
  void _showDeviceDetails(Device device) {
    Navigator.push(
      context,
      MaterialPageRoute(
        builder: (context) => DeviceDetailsPage(device: device),
      ),
    );
  }
}
```

## Local Network Discovery

For local control (when on the same network), you can use mDNS discovery:

```dart
import 'package:multicast_dns/multicast_dns.dart';

Future<List<String>> discoverMiniControllers() async {
  final mdns = MulticastDns();
  await mdns.start();
  
  final devices = <String>[];
  
  await for (final ptr in mdns.lookup<PtrResourceRecord>(
    ResourceRecordQuery.serverPointer('_http._tcp.local'),
  )) {
    final name = ptr.domainName;
    if (name.contains('mini-led')) {
      // Resolve IP address
      await for (final srv in mdns.lookup<SrvResourceRecord>(
        ResourceRecordQuery.service(name),
      )) {
        devices.add('http://${srv.target}:8080');
      }
    }
  }
  
  mdns.stop();
  return devices;
}
```

## Direct WebSocket Connection

For real-time updates when on the same network:

```dart
import 'package:web_socket_channel/web_socket_channel.dart';

class MiniControllerWebSocket {
  late WebSocketChannel channel;
  
  void connect(String ip) {
    channel = WebSocketChannel.connect(
      Uri.parse('ws://$ip:81'),
    );
    
    channel.stream.listen((message) {
      final data = jsonDecode(message);
      if (data['type'] == 'device_update') {
        _handleDeviceUpdate(data);
      }
    });
  }
  
  void sendCommand(bool state, int brightness) {
    channel.sink.add(jsonEncode({
      'action': 'control',
      'state': state,
      'brightness': brightness,
    }));
  }
  
  void _handleDeviceUpdate(Map<String, dynamic> data) {
    // Update UI with new device state
    final state = data['state'];
    final brightness = data['brightness'];
    // ... update your app state
  }
  
  void close() {
    channel.sink.close();
  }
}
```

## Example: Complete Integration Flow

```javascript
// Backend - Code.gs
function processDeviceData(params) {
  var sheet = SpreadsheetApp.getActiveSpreadsheet().getSheetByName('Devices');
  var uid = params.uid;
  var deviceType = params.device_type || 'main_controller';
  
  // Find or create device row
  var row = findDeviceRow(sheet, uid);
  if (!row) {
    row = sheet.getLastRow() + 1;
    sheet.getRange(row, 1).setValue(uid);
  }
  
  // Update device data
  sheet.getRange(row, 2).setValue(deviceType);
  sheet.getRange(row, 3).setValue(params.ver);
  sheet.getRange(row, 4).setValue(new Date());
  sheet.getRange(row, 5).setValue(params.rssi);
  sheet.getRange(row, 6).setValue(params.uptime);
  
  if (deviceType === 'mini_controller' && params.d1) {
    sheet.getRange(row, 7).setValue(params.d1.s);
    sheet.getRange(row, 8).setValue(params.d1.v);
    sheet.getRange(row, 9).setValue(params.d1.runtime);
  }
  
  // Get commands for device
  var commands = getDeviceCommands(uid);
  
  return {
    success: true,
    d1: commands.d1 || {}
  };
}
```

## Monitoring and Analytics

Track mini controller statistics:

```javascript
function getMiniControllerStats() {
  var sheet = SpreadsheetApp.getActiveSpreadsheet().getSheetByName('Devices');
  var data = sheet.getDataRange().getValues();
  
  var stats = {
    totalMiniControllers: 0,
    onlineCount: 0,
    totalRuntime: 0,
    averageBrightness: 0
  };
  
  var brightnessSum = 0;
  var now = new Date();
  
  for (var i = 1; i < data.length; i++) {
    if (data[i][1] === 'mini_controller') {
      stats.totalMiniControllers++;
      
      var lastSeen = new Date(data[i][3]);
      var minutesAgo = (now - lastSeen) / 60000;
      
      if (minutesAgo < 5) {
        stats.onlineCount++;
      }
      
      stats.totalRuntime += data[i][8] || 0;
      brightnessSum += data[i][7] || 0;
    }
  }
  
  if (stats.totalMiniControllers > 0) {
    stats.averageBrightness = Math.round(brightnessSum / stats.totalMiniControllers);
  }
  
  return stats;
}
```

## Troubleshooting

### Mini Controller Not Appearing in Dashboard

1. Check that `GOOGLE_SCRIPT_URL` is correctly configured
2. Verify cloud polling is working (check Serial Monitor)
3. Ensure device has internet connectivity
4. Check that backend is processing `device_type` field

### Commands Not Reaching Mini Controller

1. Verify the backend is setting `update: true` in response
2. Check device is polling at regular intervals
3. Review Serial Monitor for cloud sync errors

### WebSocket Not Connecting (Local)

1. Ensure devices are on same network
2. Check firewall is not blocking port 81
3. Try accessing web interface first to verify connectivity

## Best Practices

1. **Use mDNS for Discovery**: Discover mini controllers automatically on local network
2. **Fallback to Cloud**: If local connection fails, use cloud API
3. **Group by Type**: Show mini controllers in a separate section or with clear badges
4. **Real-time Updates**: Use WebSocket for local control, polling for cloud
5. **Statistics Tracking**: Monitor runtime, energy usage, and connection status
6. **Bulk Control**: Allow controlling all mini controllers at once
7. **Scene Integration**: Include mini controllers in automation scenes

## Security Considerations

1. **Authentication**: Add authentication to mini controller API in production
2. **HTTPS**: Use HTTPS for cloud communication (Apps Script provides this)
3. **Rate Limiting**: Implement rate limiting to prevent abuse
4. **Input Validation**: Always validate brightness values (0-100)
5. **Firmware Updates**: Plan for OTA updates using cloud backend

---

For more information, see:
- [Mini Controller README](README.md)
- [Main Project README](../../README.md)
- [Backend Documentation](../../backend/google-apps-script/README.md)
