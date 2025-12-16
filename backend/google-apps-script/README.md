# Cloud Backend Services Specification
**Google Apps Script Implementation**

## Document Information
- **Module:** Backend Services
- **Platform:** Google Apps Script
- **API Version:** 3.0
- **Document Number:** BE-SPEC-001

## 1. System Overview

The backend subsystem provides cloud-based services for device management, state synchronization, and user interface delivery. Implementation leverages Google Apps Script for serverless deployment with Google Sheets as the persistence layer.

## 2. Architecture

### 2.1 Service Components
- RESTful API endpoints (HTTP POST)
- Data persistence (Google Sheets)
- Web application serving (Dashboard.html)
- Authentication and session management

### 2.2 Data Model
- Device state records
- Schedule definitions  
- Scene configurations
- System settings

## 3. API Specification

### 3.1 Polling Endpoint
**URL:** `<deployment-url>?action=poll`  
**Method:** POST  
**Content-Type:** application/json

**Request Format:**
```json
{
  "action": "poll",
  "uid": "device-mac-address",
  "ver": 3.0,
  "uptime": 3600,
  "rssi": -45,
  "d1": {"s": 1, "v": 75, "t": 1},
  "d2": {"s": 0, "v": 100, "t": 0},
  "d3": {"s": 1, "v": 50, "t": 2},
  "d4": {"s": 0, "v": 100, "t": 0}
}
```

**Response Format:**
```json
{
  "sys_name": "Living Room Hub",
  "ota_update": false,
  "d1": {"type": "FAN", "name": "Ceiling Fan", "update": true, "state": 1, "val": 80},
  "schedules": [...],
  "scenes": [...]
}
```

### 3.2 Control Endpoint
Bi-directional state synchronization via polling mechanism (2.5s interval).

## 4. Deployment

### 4.1 Prerequisites
- Google account with Apps Script access
- Google Sheets for data storage

### 4.2 Installation Steps
1. Create new Google Apps Script project
2. Import Code.gs and Dashboard.html
3. Deploy as web application
4. Configure permissions (anyone with link)
5. Copy deployment URL for firmware configuration

### 4.3 Configuration
Update firmware config.h with deployment URL:
```cpp
const char* cloudURL = "your-deployment-url";
```

## 5. Data Persistence

### 5.1 Google Sheets Schema
- Sheet 1: Device states
- Sheet 2: User configurations  
- Sheet 3: Schedule definitions
- Sheet 4: System logs (optional)

### 5.2 Data Retention
- Real-time data: Immediate persistence
- Historical data: Configurable retention policy

## 6. Security Considerations

### 6.1 Authentication
- Apps Script deployment authentication
- URL-based access control
- HTTPS encryption (enforced by Google)

### 6.2 Best Practices
- Limit deployment access scope
- Implement rate limiting (if required)
- Validate all input data
- Sanitize HTML output

## 7. Performance

### 7.1 Specifications
- Polling interval: 2.5 seconds
- Response time: <500ms (typical)
- Concurrent connections: Google Apps Script quotas apply

### 7.2 Scalability
- Single deployment supports multiple devices
- Google Sheets row limit: 10 million cells

## 8. Monitoring and Diagnostics

### 8.1 Logging
- Apps Script execution logs
- Device connection history
- Error tracking

### 8.2 Debugging
- Apps Script debugger
- Network traffic analysis (browser dev tools)

## References

1. Google Apps Script Documentation, Google LLC
2. RESTful API Design Guidelines, various
3. JSON Data Interchange Standard, ECMA-404

---

**Document Control:** BE-SPEC-001  
**Status:** Production
