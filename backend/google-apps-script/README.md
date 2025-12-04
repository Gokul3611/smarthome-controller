# Smart Home Controller - Google Apps Script Backend

This directory contains the Google Apps Script backend that provides:
- RESTful API for ESP32 devices and mobile apps
- Google Sheets as a database
- Web dashboard for device control
- User authentication
- Scheduling and scenes management

## 📁 Files

- **Code.gs** - Main backend logic and API handlers
- **Database.gs** - Google Sheets database helper functions
- **Dashboard.html** - Web dashboard UI
- **appsscript.json** - Apps Script manifest

## 🚀 Deployment Steps

### 1. Create Google Sheets Database

1. Go to [Google Sheets](https://sheets.google.com)
2. Create a new spreadsheet named "Smart Home Controller DB"
3. Note the Spreadsheet ID from the URL: 
   ```
   https://docs.google.com/spreadsheets/d/[SPREADSHEET_ID]/edit
   ```

### 2. Create Apps Script Project

1. Go to [Google Apps Script](https://script.google.com)
2. Click "New Project"
3. Name it "Smart Home Controller Backend"

### 3. Add the Code Files

1. Delete the default `Code.gs` content
2. Copy the content from each `.gs` file in this directory to the script editor
3. Create new files using File → New → HTML file for `Dashboard.html`
4. Create `appsscript.json` using File → Project Settings → Show "appsscript.json"

### 4. Set Script Properties

1. Click Project Settings (gear icon)
2. Scroll to Script Properties
3. Add the following properties:
   - **SHEET_ID**: Your spreadsheet ID from step 1
   - **API_KEY**: Create a secure key (e.g., `smarthome-2024-RANDOM`)

### 5. Deploy as Web App

1. Click Deploy → New deployment
2. Select type: **Web app**
3. Configuration:
   - Description: "Smart Home Controller v1.0"
   - Execute as: **Me**
   - Who has access: **Anyone** (for public access) or **Anyone with Google account**
4. Click Deploy
5. Authorize the app (review permissions)
6. Copy the **Web app URL** - this is your API endpoint!

### 6. Set Up Custom Domain (Optional)

To get a memorable URL like `home.yourname.com`:

1. Go to your domain registrar (e.g., GoDaddy, Namecheap)
2. Add a CNAME record:
   ```
   Type: CNAME
   Name: home (or smarthome)
   Value: script.google.com
   ```
3. In Apps Script deployment, note your deployment ID
4. Access via: `https://script.google.com/macros/s/[DEPLOYMENT_ID]/exec`

**Note**: Apps Script URLs cannot be fully customized, but you can use URL shorteners or domain forwards.

## 📊 Database Schema

The backend automatically creates these sheets in your Google Sheets:

### Devices Sheet
| Column | Description |
|--------|-------------|
| UID | Unique device identifier (MAC address) |
| Name | Device display name |
| Type | Device type (light/fan) |
| IP | Current IP address |
| Version | Firmware version |
| Uptime | Device uptime in seconds |
| RSSI | WiFi signal strength |
| Heap | Free heap memory |
| Last Seen | Last communication timestamp |
| User ID | Owner user ID |
| Created | Registration timestamp |

### DeviceStates Sheet
| Column | Description |
|--------|-------------|
| UID | Device UID |
| Channel | Channel number (0-3) |
| State | Power state (true/false) |
| Value | Brightness/speed value (0-100) |
| Type | Device type |
| Timestamp | Last update time |

### Schedules Sheet
| Column | Description |
|--------|-------------|
| ID | Schedule UUID |
| User ID | Owner user ID |
| Device UID | Target device |
| Channel | Target channel |
| Name | Schedule name |
| Start Time | Start time (HH:MM) |
| End Time | End time (HH:MM) |
| Days | Days of week (bitmask) |
| Action | Action to perform |
| Value | Value for action |
| Enabled | Schedule enabled status |
| Created | Creation timestamp |

### Scenes Sheet
| Column | Description |
|--------|-------------|
| ID | Scene UUID |
| User ID | Owner user ID |
| Name | Scene name |
| Devices | JSON array of device states |
| Created | Creation timestamp |

### Users Sheet
| Column | Description |
|--------|-------------|
| ID | User UUID |
| Name | User name |
| Email | User email |
| Password | Hashed password |
| Created | Registration timestamp |

### Commands Sheet
| Column | Description |
|--------|-------------|
| UID | Target device UID |
| Command | JSON command object |
| Timestamp | Queue timestamp |
| Executed | Execution status |

## 🔌 API Endpoints

All endpoints use the web app URL as base: `https://script.google.com/macros/s/[YOUR_DEPLOYMENT_ID]/exec`

### Device Endpoints

#### Poll for Commands
```
GET/POST ?action=poll&uid=AA:BB:CC:DD:EE:FF&api_key=YOUR_KEY
```

Response:
```json
{
  "success": true,
  "config": {
    "name": "Living Room",
    "type": "fan"
  },
  "commands": [],
  "ota_update": false
}
```

#### Update Device State
```
POST ?action=update_state
Body: {
  "api_key": "YOUR_KEY",
  "uid": "AA:BB:CC:DD:EE:FF",
  "states": [
    {"channel": 0, "state": true, "value": 75}
  ]
}
```

### User Endpoints

#### Login
```
POST ?action=login
Body: {
  "api_key": "YOUR_KEY",
  "email": "user@example.com",
  "password": "password123"
}
```

#### Signup
```
POST ?action=signup
Body: {
  "api_key": "YOUR_KEY",
  "name": "John Doe",
  "email": "user@example.com",
  "password": "password123"
}
```

### Device Management

#### Get Devices
```
GET/POST ?action=get_devices&api_key=YOUR_KEY&user_id=USER_ID
```

#### Update Device
```
POST ?action=update_device
Body: {
  "api_key": "YOUR_KEY",
  "uid": "AA:BB:CC:DD:EE:FF",
  "name": "Bedroom Fan",
  "type": "fan"
}
```

#### Reset WiFi
```
POST ?action=reset_wifi
Body: {
  "api_key": "YOUR_KEY",
  "uid": "AA:BB:CC:DD:EE:FF"
}
```

### Schedule Management

#### Get Schedules
```
GET/POST ?action=get_schedules&api_key=YOUR_KEY&user_id=USER_ID
```

#### Save Schedule
```
POST ?action=save_schedule
Body: {
  "api_key": "YOUR_KEY",
  "schedule": {
    "user_id": "USER_ID",
    "device_uid": "AA:BB:CC:DD:EE:FF",
    "channel": 0,
    "name": "Morning Light",
    "start_time": "06:00",
    "end_time": "08:00",
    "days": "1111100",
    "action": "on",
    "value": 100,
    "enabled": true
  }
}
```

#### Delete Schedule
```
POST ?action=delete_schedule
Body: {
  "api_key": "YOUR_KEY",
  "schedule_id": "SCHEDULE_UUID"
}
```

### Scene Management

Similar endpoints for scenes (get_scenes, save_scene, delete_scene)

## 🌐 Web Dashboard

Access the web dashboard at:
```
https://script.google.com/macros/s/[YOUR_DEPLOYMENT_ID]/exec?page=dashboard
```

### Dashboard Features

- **Real-time Device Control**: Toggle devices on/off, adjust speed/brightness
- **Device Management**: Rename devices, change device types
- **Scheduling**: Create time-based automations
- **Scenes**: Save and activate device combinations
- **WiFi Reset**: Reset device WiFi credentials remotely
- **Download App**: Direct link to download the mobile app
- **Responsive Design**: Works on desktop, tablet, and mobile

## 🔒 Security Considerations

1. **API Key**: Change the default API key in Script Properties
2. **HTTPS**: Apps Script uses HTTPS by default
3. **Password Hashing**: Passwords are hashed using SHA-256
4. **Rate Limiting**: Consider implementing rate limiting for production
5. **CORS**: Apps Script allows cross-origin requests
6. **Authentication**: Implement token-based auth for production

## 🔧 ESP32 Integration

Configure your ESP32 to use this backend:

```cpp
// In your ESP32 code
String GOOGLE_SCRIPT_URL = "https://script.google.com/macros/s/YOUR_DEPLOYMENT_ID/exec";
String API_KEY = "smarthome-2024-YOUR_KEY";

void pollServer() {
  String url = GOOGLE_SCRIPT_URL + "?action=poll&uid=" + WiFi.macAddress() + "&api_key=" + API_KEY;
  HTTPClient http;
  http.begin(url);
  int httpCode = http.GET();
  
  if (httpCode == 200) {
    String payload = http.getString();
    // Parse JSON and execute commands
  }
  
  http.end();
}
```

## 📱 Flutter App Integration

Configure the Flutter app to use this backend:

```dart
// In lib/config/api_config.dart
class ApiConfig {
  static const String baseUrl = 'https://script.google.com/macros/s/YOUR_DEPLOYMENT_ID/exec';
  static const String apiKey = 'smarthome-2024-YOUR_KEY';
}
```

## 🐛 Troubleshooting

### Issue: "Script function not found"
- Make sure all `.gs` files are added to the project
- Verify function names match exactly

### Issue: "Unauthorized"
- Check API key is set correctly in Script Properties
- Verify API key is passed in requests

### Issue: "Spreadsheet not found"
- Verify SHEET_ID in Script Properties
- Make sure the script has access to the spreadsheet

### Issue: "Rate limit exceeded"
- Apps Script has quotas (URL Fetch calls: 20,000/day)
- Implement caching and reduce polling frequency

## 📈 Performance Tips

1. **Caching**: Use Apps Script CacheService for frequently accessed data
2. **Batch Operations**: Group database operations to reduce execution time
3. **Async Processing**: Use time-driven triggers for background tasks
4. **Optimize Queries**: Minimize sheet reads/writes
5. **CDN**: Host static assets (images, CSS) on a CDN

## 📝 License

MIT License - Same as the main project

## 🆘 Support

For issues or questions:
- GitHub Issues: [smarthome-controller/issues](https://github.com/Gokul3611/smarthome-controller/issues)
- Documentation: Check the main README.md

---

**Deployment Checklist:**
- [ ] Create Google Sheets database
- [ ] Create Apps Script project
- [ ] Copy all code files
- [ ] Set Script Properties (SHEET_ID, API_KEY)
- [ ] Deploy as web app
- [ ] Test API endpoints
- [ ] Configure ESP32 with deployment URL
- [ ] Configure Flutter app with deployment URL
- [ ] Test dashboard access
- [ ] Set up custom domain (optional)
