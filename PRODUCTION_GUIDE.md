# Production Deployment Guide

Complete guide for deploying the Smart Home Controller system to production environment.

---

## 🎯 Overview

This guide covers the deployment process from development to production, including testing, security hardening, and scaling considerations.

---

## 📋 Pre-Deployment Checklist

### Development Complete
- [ ] All features implemented and tested
- [ ] Code reviewed and approved
- [ ] Documentation complete
- [ ] Unit tests passing
- [ ] Integration tests passing
- [ ] Hardware validated
- [ ] Security audit completed

### Infrastructure Ready
- [ ] Google Apps Script deployed
- [ ] Google Sheets database configured
- [ ] Domain/URL configured
- [ ] SSL certificates valid
- [ ] Backup systems in place
- [ ] Monitoring tools configured

### Production Hardware
- [ ] Final PCB manufactured and tested
- [ ] Enclosures prepared
- [ ] All components procured
- [ ] Assembly completed
- [ ] Quality control passed
- [ ] Labeling and documentation included

---

## 🔧 Step 1: Backend Deployment

### 1.1 Google Sheets Setup

```
1. Create production spreadsheet: "SmartHome_Production_DB"
2. Set appropriate permissions:
   - Owner: Company Google account
   - Editors: Development team
   - Viewers: Stakeholders
3. Enable version history
4. Set up scheduled backups
```

### 1.2 Apps Script Deployment

```
1. Create new Apps Script project: "SmartHome_Production_Backend"
2. Copy all code from backend/google-apps-script/
3. Configure Script Properties:
   - SHEET_ID: Production spreadsheet ID
   - API_KEY: Generate secure key (min 32 characters)
   - ENV: "production"
4. Deploy as web app:
   - Version: New
   - Execute as: Me (your account)
   - Access: Anyone
5. Save deployment URL
6. Test all API endpoints
```

### 1.3 Security Configuration

```javascript
// Add to Code.gs for production
const CONFIG = {
  ENVIRONMENT: 'production',
  API_KEY: PropertiesService.getScriptProperties().getProperty('API_KEY'),
  RATE_LIMIT: 100, // requests per minute per device
  ENABLE_LOGGING: true,
  LOG_RETENTION_DAYS: 90,
  MAX_DEVICES_PER_USER: 50
};

// Add rate limiting
function rateLimitCheck(deviceId) {
  const cache = CacheService.getScriptCache();
  const key = 'rate_' + deviceId;
  const count = cache.get(key) || 0;
  
  if (count > CONFIG.RATE_LIMIT) {
    throw new Error('Rate limit exceeded');
  }
  
  cache.put(key, parseInt(count) + 1, 60); // 60 seconds
}
```

---

## 🌐 Step 2: Domain Configuration

### Option A: Custom Domain (Recommended for Production)

```
1. Register domain (e.g., smarthome.company.com)
2. Set up Cloudflare or similar CDN
3. Configure forwarding to Apps Script URL
4. Enable HTTPS/SSL
5. Test accessibility
```

### Option B: URL Shortener

```
1. Use bit.ly or custom shortener
2. Create memorable link: https://bit.ly/company-smarthome
3. Point to Apps Script URL
4. Test and document
```

---

## 📱 Step 3: Mobile App Deployment

### 3.1 Production Configuration

Update `lib/config/api_config.dart`:

```dart
class ApiConfig {
  // Production URL
  static const String baseUrl = 'https://smarthome.company.com';
  static const String apiKey = 'PRODUCTION_API_KEY';
  
  // Production settings
  static const int pollInterval = 5000; // 5 seconds
  static const Duration timeout = Duration(seconds: 30);
  static const String environment = 'production';
  
  // Analytics (optional)
  static const String analyticsId = 'UA-XXXXXXXX-X';
  
  // Error reporting
  static const String sentryDsn = 'https://...@sentry.io/...';
}
```

### 3.2 Build Production APK

```bash
# Clean build
flutter clean
flutter pub get

# Build release APK with signing
flutter build apk --release \
  --dart-define=ENVIRONMENT=production \
  --dart-define=API_URL=https://smarthome.company.com

# Or split by ABI for smaller size
flutter build apk --release --split-per-abi

# Result:
# - app-armeabi-v7a-release.apk (32-bit, ~15MB)
# - app-arm64-v8a-release.apk (64-bit, ~18MB)
# - app-x86_64-release.apk (emulator, ~20MB)
```

### 3.3 App Signing

```bash
# Create production keystore (do this once)
keytool -genkey -v -keystore ~/smarthome-production.jks \
  -keyalg RSA -keysize 2048 -validity 10000 \
  -alias smarthome-prod

# Store keystore safely!
# Add to .gitignore
# Backup to secure location

# Configure in android/key.properties:
storePassword=<production-password>
keyPassword=<production-password>
keyAlias=smarthome-prod
storeFile=/path/to/smarthome-production.jks
```

### 3.4 App Store Preparation (Future)

For Play Store/App Store release:

```bash
# Build App Bundle for Play Store
flutter build appbundle --release

# Result: build/app/outputs/bundle/release/app-release.aab

# For iOS:
flutter build ios --release
```

---

## 🔌 Step 4: ESP32 Firmware Deployment

### 4.1 Production Firmware Configuration

Update firmware configuration:

```cpp
// config.h
#define ENVIRONMENT "production"
#define GOOGLE_SCRIPT_URL "https://smarthome.company.com"
#define API_KEY "PRODUCTION_API_KEY"
#define DEVICE_NAME_PREFIX "SmartHome"
#define OTA_ENABLED true
#define DEBUG_ENABLED false // Disable for production

// Increase watchdog timeout for production
#define WATCHDOG_TIMEOUT_PRODUCTION 30000 // 30 seconds

// Enable security features
#define REQUIRE_HTTPS true
#define VERIFY_SSL_CERT true
```

### 4.2 Build Production Firmware

```
Arduino IDE:
1. Tools → Board: ESP32 Dev Module
2. Tools → Partition Scheme: Default 4MB with OTA
3. Tools → Core Debug Level: None
4. Sketch → Export Compiled Binary
5. Rename: smarthome_v3.0.0_production.bin
```

### 4.3 Mass Programming

For multiple devices:

```bash
# Using esptool.py
esptool.py --chip esp32 --port /dev/ttyUSB0 --baud 921600 \
  write_flash 0x1000 bootloader.bin \
  0x8000 partitions.bin \
  0x10000 smarthome_v3.0.0_production.bin

# Script for batch programming
#!/bin/bash
for port in /dev/ttyUSB*; do
  echo "Programming $port..."
  esptool.py --port $port write_flash 0x10000 firmware.bin
  echo "Done: $port"
done
```

### 4.4 Initial Configuration

Each device needs unique configuration:

```cpp
// First boot setup
void firstBootSetup() {
  // Generate unique device ID from MAC
  String deviceId = WiFi.macAddress();
  
  // Store in EEPROM
  Preferences prefs;
  prefs.begin("config", false);
  prefs.putString("device_id", deviceId);
  prefs.putString("api_url", GOOGLE_SCRIPT_URL);
  prefs.putString("api_key", API_KEY);
  prefs.end();
  
  // Register with backend
  registerDevice();
}
```

---

## 🧪 Step 5: Testing & Validation

### 5.1 System Integration Test

```
Test Matrix:

1. Backend Tests:
   [ ] API endpoints respond correctly
   [ ] Database operations work
   [ ] Authentication functions
   [ ] Rate limiting active
   [ ] Error handling works

2. Dashboard Tests:
   [ ] Loads correctly
   [ ] Device control works
   [ ] Real-time updates function
   [ ] All modals work
   [ ] Responsive on mobile/desktop

3. Mobile App Tests:
   [ ] Login/signup works
   [ ] Device listing displays
   [ ] Control commands execute
   [ ] Schedules save correctly
   [ ] Scenes activate properly

4. ESP32 Tests:
   [ ] Connects to WiFi
   [ ] Registers with backend
   [ ] Responds to commands
   [ ] Physical switches work
   [ ] Zero-cross detection active
   [ ] TRIACs switching correctly

5. Integration Tests:
   [ ] End-to-end device control
   [ ] Voice assistant integration
   [ ] OTA updates work
   [ ] Multi-user scenarios
   [ ] Network failover
```

### 5.2 Load Testing

```javascript
// Apps Script load test
function loadTest() {
  const numRequests = 1000;
  const startTime = new Date();
  
  for (let i = 0; i < numRequests; i++) {
    // Simulate device poll
    handleDevicePoll({uid: 'TEST_' + i, api_key: API_KEY});
  }
  
  const endTime = new Date();
  const duration = (endTime - startTime) / 1000;
  
  Logger.log('Requests: ' + numRequests);
  Logger.log('Duration: ' + duration + 's');
  Logger.log('Rate: ' + (numRequests / duration) + ' req/s');
}
```

### 5.3 Security Testing

```
Security Checklist:

[ ] SQL injection tests (N/A for Sheets, but test input validation)
[ ] XSS attempts in dashboard
[ ] CSRF protection verified
[ ] API key validation works
[ ] Rate limiting prevents abuse
[ ] SSL/HTTPS enforced
[ ] Sensitive data encrypted
[ ] Authentication timeout works
[ ] Authorization checks in place
[ ] File upload validation (if any)
```

---

## 📊 Step 6: Monitoring & Analytics

### 6.1 Apps Script Logging

```javascript
function logEvent(type, message, data) {
  const sheet = getSheet('Logs');
  sheet.appendRow([
    new Date(),
    type,
    message,
    JSON.stringify(data),
    Session.getActiveUser().getEmail()
  ]);
}

// Usage
logEvent('ERROR', 'Device poll failed', {uid: deviceId, error: e.toString()});
logEvent('INFO', 'User login', {email: user.email});
logEvent('WARNING', 'High API usage', {device: uid, count: requestCount});
```

### 6.2 Google Sheets Dashboard

Create monitoring sheet:

```
Sheet: "Monitoring"

Columns:
- Total Devices
- Active Devices (last 1 hour)
- API Calls (last 24 hours)
- Error Rate
- Average Response Time
- Storage Used

Formulas:
=COUNTUNIQUE(Devices!A:A)
=COUNTIFS(Devices!I:I,">"&NOW()-1/24)
=COUNTIFS(Logs!A:A,">"&NOW()-1)
```

### 6.3 Alerting

```javascript
function checkSystemHealth() {
  const errorRate = getErrorRate24h();
  const activeDevices = getActiveDeviceCount();
  
  if (errorRate > 0.05) { // 5% error rate
    sendAlert('High error rate: ' + (errorRate * 100) + '%');
  }
  
  if (activeDevices < expectedDeviceCount * 0.8) { // 80% threshold
    sendAlert('Low device count: ' + activeDevices);
  }
}

function sendAlert(message) {
  MailApp.sendEmail({
    to: 'admin@company.com',
    subject: 'SmartHome Alert',
    body: message
  });
}

// Schedule to run hourly
```

---

## 🔒 Step 7: Security Hardening

### 7.1 API Key Rotation

```javascript
// Rotate API key quarterly
function rotateAPIKey() {
  const newKey = Utilities.getUuid();
  PropertiesService.getScriptProperties().setProperty('API_KEY_NEW', newKey);
  
  // Grace period: accept both old and new keys
  setTimeout(() => {
    PropertiesService.getScriptProperties().setProperty('API_KEY', newKey);
    PropertiesService.getScriptProperties().deleteProperty('API_KEY_NEW');
  }, 7 * 24 * 60 * 60 * 1000); // 7 days
}
```

### 7.2 Rate Limiting

Already implemented in Step 1.3

### 7.3 Input Validation

```javascript
function validateDeviceData(data) {
  if (!data.uid || typeof data.uid !== 'string') {
    throw new Error('Invalid UID');
  }
  
  if (data.uid.length > 50) {
    throw new Error('UID too long');
  }
  
  // Sanitize input
  data.name = sanitizeString(data.name);
  
  return data;
}

function sanitizeString(str) {
  return String(str).replace(/[<>\"']/g, '');
}
```

---

## 📦 Step 8: Backup & Recovery

### 8.1 Automated Backups

```javascript
function backupDatabase() {
  const ss = SpreadsheetApp.openById(CONFIG.SHEET_ID);
  const backupName = 'SmartHome_Backup_' + Utilities.formatDate(new Date(), 'UTC', 'yyyy-MM-dd_HHmmss');
  
  const backup = ss.copy(backupName);
  
  // Move to backup folder
  const backupFolder = DriveApp.getFolderById('BACKUP_FOLDER_ID');
  DriveApp.getFileById(backup.getId()).moveTo(backupFolder);
  
  // Delete old backups (keep 30 days)
  const oldDate = new Date();
  oldDate.setDate(oldDate.getDate() - 30);
  
  const files = backupFolder.getFiles();
  while (files.hasNext()) {
    const file = files.next();
    if (file.getDateCreated() < oldDate) {
      file.setTrashed(true);
    }
  }
}

// Schedule daily backup
```

### 8.2 Recovery Procedure

```
1. Identify backup file (latest before incident)
2. Make copy of backup
3. Open backup spreadsheet
4. Copy data to production sheet
5. Verify data integrity
6. Test system functionality
7. Document incident and recovery
```

---

## 📈 Step 9: Scaling Considerations

### 9.1 Google Apps Script Limits

```
Quotas (free tier):
- Script runtime: 6 min/execution
- URL Fetch calls: 20,000/day
- Trigger total runtime: 90 min/day

For >100 devices:
- Use Google Cloud Project
- Increase quotas
- Consider Apps Script add-on billing
```

### 9.2 Database Optimization

```
When rows >10,000:
- Archive old data
- Use separate sheets per month
- Implement pagination
- Consider migrating to Cloud SQL
```

### 9.3 Caching Strategy

```javascript
function getCachedDeviceList() {
  const cache = CacheService.getScriptCache();
  let devices = cache.get('device_list');
  
  if (!devices) {
    devices = fetchDevicesFromSheet();
    cache.put('device_list', JSON.stringify(devices), 300); // 5 min cache
  } else {
    devices = JSON.parse(devices);
  }
  
  return devices;
}
```

---

## 🎓 Step 10: User Training

### 10.1 Administrator Training

Topics:
- Backend access and management
- Dashboard usage
- Device provisioning
- Troubleshooting common issues
- Security best practices

Materials:
- Video tutorials
- PDF guides
- Hands-on workshop

### 10.2 End User Documentation

Provide:
- Quick start guide
- App user manual
- FAQ document
- Video demonstrations
- Support contact info

---

## 📞 Step 11: Support Setup

### 11.1 Support Channels

- Email: support@company.com
- Phone: +1-XXX-XXX-XXXX
- GitHub Issues (for bugs)
- Documentation site
- Live chat (optional)

### 11.2 Issue Tracking

Use GitHub Issues with labels:
- `bug` - Software bugs
- `hardware` - Hardware issues
- `question` - User questions
- `enhancement` - Feature requests
- `documentation` - Doc updates

---

## ✅ Production Launch Checklist

Final verification before going live:

- [ ] All tests passed
- [ ] Security audit complete
- [ ] Backups configured
- [ ] Monitoring active
- [ ] Documentation complete
- [ ] Support team trained
- [ ] Rollback plan ready
- [ ] Stakeholders informed
- [ ] Launch communication sent
- [ ] First production devices deployed
- [ ] 24-hour monitoring period
- [ ] Issue response team on standby

---

## 📊 Success Metrics

Track these KPIs:

- **Uptime**: Target 99.9%
- **Response Time**: <2 seconds average
- **Error Rate**: <1%
- **User Satisfaction**: >4.5/5
- **Device Online Rate**: >95%
- **Support Tickets**: <10/month per 100 devices

---

## 🚨 Incident Response

### Severity Levels

**P0 - Critical**
- System down
- Data loss
- Security breach
Response: Immediate (24/7)

**P1 - High**
- Partial outage
- Performance degradation
Response: <4 hours

**P2 - Medium**
- Non-critical features broken
- Workaround available
Response: <24 hours

**P3 - Low**
- Minor issues
- Enhancement requests
Response: <1 week

---

## 📞 Support

For production deployment questions:
- Technical Lead: [contact]
- DevOps: [contact]
- Security: [contact]

---

**Version**: 1.0  
**Last Updated**: December 2024  
**Status**: Production Ready
