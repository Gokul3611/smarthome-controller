# OTA (Over-The-Air) Update Guide

Complete guide for implementing and using OTA firmware updates for the Smart Home Controller.

---

## Overview

OTA updates allow you to update ESP32 firmware remotely without physical access to the device. This is essential for:
- Bug fixes and security patches
- New feature deployment
- Configuration updates
- Emergency fixes

---

## System Architecture

```
Developer → GitHub Release → Google Apps Script → ESP32 Device
              (Binary)         (Distribution)      (Downloads & Installs)
```

### Update Flow

1. **Compile** new firmware binary (.bin file)
2. **Upload** to hosting (GitHub Releases, Google Drive, or Apps Script)
3. **Configure** backend to signal update availability
4. **ESP32 polls** backend, detects update
5. **Download** firmware binary
6. **Verify** checksum/signature
7. **Install** to OTA partition
8. **Reboot** and test
9. **Rollback** if boot fails (automatic)

---

## ESP32 OTA Implementation

### 1. Partition Scheme

ESP32 requires special partition layout for OTA:

```
# partitions.csv
Name,     Type, SubType, Offset,  Size
nvs,      data, nvs,     0x9000,  0x5000
otadata,  data, ota,     0xe000,  0x2000
app0,     app,  ota_0,   0x10000, 0x140000
app1,     app,  ota_1,   0x150000,0x140000
spiffs,   data, spiffs,  0x290000,0x70000
```

**Key Points:**
- `app0` and `app1` are two firmware slots
- System alternates between them
- Failed boot automatically rolls back

### 2. Arduino Code

Add to your ESP32 firmware:

```cpp
#include <Update.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

// OTA Configuration
const char* OTA_URL = "https://your-backend/firmware.bin";
const String CURRENT_VERSION = "3.0.0";

void checkForUpdates() {
  HTTPClient http;
  WiFiClientSecure client;
  client.setInsecure(); // For testing only! Use proper certificate in production
  
  // Check if update available
  String checkUrl = GOOGLE_SCRIPT_URL + "?action=check_ota&current_version=" + CURRENT_VERSION;
  http.begin(client, checkUrl);
  
  int httpCode = http.GET();
  if (httpCode == 200) {
    String payload = http.getString();
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payload);
    
    if (doc["update_available"].as<bool>()) {
      String binUrl = doc["bin_url"].as<String>();
      String newVersion = doc["version"].as<String>();
      
      Serial.println("Update available: " + newVersion);
      performOTAUpdate(binUrl);
    }
  }
  http.end();
}

void performOTAUpdate(String binUrl) {
  HTTPClient http;
  WiFiClientSecure client;
  client.setInsecure();
  
  Serial.println("Starting OTA update from: " + binUrl);
  http.begin(client, binUrl);
  
  int httpCode = http.GET();
  if (httpCode == 200) {
    int contentLength = http.getSize();
    bool canBegin = Update.begin(contentLength);
    
    if (canBegin) {
      WiFiClient * stream = http.getStreamPtr();
      size_t written = Update.writeStream(*stream);
      
      if (written == contentLength) {
        Serial.println("Written : " + String(written) + " successfully");
      } else {
        Serial.println("Written only : " + String(written) + "/" + String(contentLength));
      }
      
      if (Update.end()) {
        Serial.println("OTA done!");
        if (Update.isFinished()) {
          Serial.println("Update successfully completed. Rebooting...");
          delay(1000);
          ESP.restart();
        } else {
          Serial.println("Update not finished? Something went wrong!");
        }
      } else {
        Serial.println("Error Occurred. Error #: " + String(Update.getError()));
      }
    } else {
      Serial.println("Not enough space to begin OTA");
    }
  }
  http.end();
}

// Call in loop() or use timer
void loop() {
  // Check for updates every 24 hours
  static unsigned long lastCheck = 0;
  if (millis() - lastCheck > 86400000) { // 24 hours
    checkForUpdates();
    lastCheck = millis();
  }
  
  // ... rest of your code
}
```

### 3. Build Firmware Binary

**Using Arduino IDE:**
```
1. Sketch → Export Compiled Binary
2. Find .bin file in build folder
3. File name: firmware_v3.0.0.bin
```

**Using PlatformIO:**
```bash
pio run
# Binary at: .pio/build/esp32dev/firmware.bin
```

**Using ESP-IDF:**
```bash
idf.py build
# Binary at: build/project_name.bin
```

---

## Backend OTA Configuration

### Google Apps Script Implementation

Add to `Code.gs`:

```javascript
/**
 * Check for OTA updates
 */
function handleCheckOTA(data) {
  const currentVersion = data.current_version;
  const latestVersion = "3.0.1"; // Update this when new version available
  const binUrl = "https://github.com/Gokul3611/smarthome-controller/releases/download/v3.0.1/firmware.bin";
  
  const updateAvailable = compareVersions(latestVersion, currentVersion) > 0;
  
  return jsonResponse({
    success: true,
    update_available: updateAvailable,
    version: latestVersion,
    bin_url: binUrl,
    changelog: "Bug fixes and improvements",
    mandatory: false,
    release_notes_url: "https://github.com/Gokul3611/smarthome-controller/releases/latest"
  });
}

/**
 * Compare semantic versions
 */
function compareVersions(v1, v2) {
  const parts1 = v1.split('.').map(Number);
  const parts2 = v2.split('.').map(Number);
  
  for (let i = 0; i < 3; i++) {
    if (parts1[i] > parts2[i]) return 1;
    if (parts1[i] < parts2[i]) return -1;
  }
  return 0;
}
```

---

## Hosting Firmware Binaries

### Option 1: GitHub Releases (Recommended)

**Advantages:**
- Free
- Reliable
- Version controlled
- Public or private repos

**Steps:**
1. Create new release on GitHub
2. Tag with version (e.g., v3.0.1)
3. Upload firmware.bin file
4. Get download URL
5. Configure backend with URL

**URL Format:**
```
https://github.com/USER/REPO/releases/download/v3.0.1/firmware.bin
```

### Option 2: Google Drive

**Advantages:**
- Free 15GB storage
- Easy to manage
- Works with Apps Script

**Steps:**
1. Upload firmware.bin to Google Drive
2. Get shareable link
3. Convert to direct download link:
```
Original: https://drive.google.com/file/d/FILE_ID/view
Direct: https://drive.google.com/uc?export=download&id=FILE_ID
```

### Option 3: Apps Script as Host

**Advantages:**
- Same infrastructure as backend
- Integrated

**Limitations:**
- 50MB file size limit
- Bandwidth quotas

**Implementation:**
```javascript
// In Apps Script
function doGet(e) {
  if (e.parameter.file === 'firmware') {
    const blob = DriveApp.getFileById('YOUR_FILE_ID').getBlob();
    return ContentService.createTextOutput(blob.getBytes())
      .setMimeType(ContentService.MimeType.BINARY);
  }
}
```

---

## Security Best Practices

### 1. Firmware Signing

Generate signature for firmware:

```bash
# Generate SHA256 hash
sha256sum firmware.bin > firmware.bin.sha256

# Or use openssl
openssl dgst -sha256 firmware.bin
```

Include in update check:
```javascript
{
  "bin_url": "...",
  "checksum": "abc123...",
  "signature": "def456..."
}
```

Verify in ESP32:
```cpp
#include <mbedtls/md.h>

bool verifyChecksum(uint8_t* data, size_t len, const char* expectedHash) {
  byte hash[32];
  mbedtls_md_context_t ctx;
  
  mbedtls_md_init(&ctx);
  mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(MBEDTLS_MD_SHA256), 0);
  mbedtls_md_starts(&ctx);
  mbedtls_md_update(&ctx, data, len);
  mbedtls_md_finish(&ctx, hash);
  mbedtls_md_free(&ctx);
  
  // Compare hash with expectedHash
  // Return true if match
}
```

### 2. HTTPS Only

Always use HTTPS for firmware downloads:
```cpp
WiFiClientSecure client;
client.setCACert(root_ca); // Production: use proper certificate
```

### 3. Version Control

Implement strict version checking:
```cpp
bool isNewerVersion(String current, String available) {
  // Parse and compare semantic versions
  // Only allow upgrades, not downgrades (unless forced)
}
```

### 4. Rollback Protection

Firmware should mark itself as valid:
```cpp
void setup() {
  // After successful boot
  if (Update.canRollBack()) {
    Update.rollBack(); // Only if boot fails
  }
  
  // Mark this version as good
  // (implement your own logic)
}
```

---

## Dashboard OTA Management

Add to dashboard:

```html
<div class="ota-section">
  <h3>Firmware Updates</h3>
  <p>Current Version: <span id="currentVersion">3.0.0</span></p>
  <p>Latest Version: <span id="latestVersion">3.0.1</span></p>
  <button onclick="triggerOTA()">Update All Devices</button>
</div>

<script>
async function triggerOTA() {
  const confirm = window.confirm('Update all devices? This may take a few minutes.');
  if (!confirm) return;
  
  // Signal backend to mark update as available
  await fetch(API_URL, {
    method: 'POST',
    body: JSON.stringify({
      action: 'trigger_ota',
      version: '3.0.1',
      bin_url: 'https://...'
    })
  });
  
  alert('Update triggered. Devices will update on next poll.');
}
</script>
```

---

## Testing OTA Updates

### Test Procedure

1. **Prepare Test Device**
   - Flash current firmware via USB
   - Connect to WiFi
   - Note current version

2. **Prepare Update**
   - Make small change (e.g., LED blink pattern)
   - Increment version number
   - Build binary

3. **Upload Binary**
   - Upload to GitHub/Drive
   - Get URL
   - Update backend configuration

4. **Trigger Update**
   - Force device to check for updates
   - Monitor serial output
   - Watch for download progress

5. **Verify**
   - Device should reboot
   - Check new version
   - Test functionality

6. **Test Rollback**
   - Upload corrupted binary
   - Device should detect and rollback
   - Should boot to previous version

### Monitoring

Add logging:
```cpp
Serial.println("=== OTA UPDATE LOG ===");
Serial.printf("Current: %s\n", CURRENT_VERSION);
Serial.printf("Available: %s\n", newVersion.c_str());
Serial.printf("URL: %s\n", binUrl.c_str());
Serial.printf("Size: %d bytes\n", contentLength);
Serial.printf("Progress: %d%%\n", (written * 100) / contentLength);
Serial.println("======================");
```

---

## Troubleshooting

### Issue: Update Fails to Download

**Causes:**
- Poor WiFi signal
- Firewall blocking
- URL incorrect
- File too large

**Solutions:**
- Check signal strength (RSSI > -70dBm)
- Test URL in browser
- Reduce binary size
- Retry with exponential backoff

### Issue: Update Downloaded but Won't Install

**Causes:**
- Not enough space
- Corrupted download
- Wrong partition scheme

**Solutions:**
- Check partition table
- Verify checksum
- Clear OTA partition

### Issue: Device Boots but Functions Wrong

**Causes:**
- Configuration mismatch
- Library version changes
- EEPROM format changed

**Solutions:**
- Preserve configuration during update
- Migrate settings if needed
- Test thoroughly before deployment

### Issue: Device Won't Boot After Update

**Causes:**
- Corrupted firmware
- Wrong ESP32 variant
- Missing bootloader

**Solutions:**
- Rollback should happen automatically
- If not, flash via USB
- Check partition table

---

## OTA Update Statistics

Track in Google Sheets:

| Device UID | Current Version | Update Time | Success | Error |
|------------|----------------|-------------|---------|-------|
| AA:BB:CC... | 3.0.0 → 3.0.1 | 2024-12-04 10:30 | ✅ | - |
| DD:EE:FF... | 3.0.0 → 3.0.1 | 2024-12-04 10:35 | ❌ | Download failed |

---

## Best Practices

1. **Test First**: Always test on one device before mass rollout
2. **Gradual Rollout**: Update 10% → 50% → 100% over time
3. **Monitor**: Watch for errors and rollbacks
4. **Backup**: Keep previous firmware version available
5. **Changelog**: Document all changes
6. **Version Scheme**: Use semantic versioning (MAJOR.MINOR.PATCH)
7. **Mandatory vs Optional**: Mark critical security updates as mandatory
8. **Maintenance Window**: Schedule updates during low-usage periods
9. **Notification**: Alert users before major updates
10. **Rollback Plan**: Always have a way to revert

---

## Support

For OTA issues:
- Check ESP32 serial output
- Review backend logs
- Test binary locally first
- Open GitHub issue

---

**Version**: 1.0  
**Last Updated**: December 2024  
**Status**: Production Ready
