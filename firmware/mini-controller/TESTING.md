# 🧪 Mini Controller Testing Guide

Complete testing procedures to verify the mini controller works with dashboard and app.

---

## 🎯 Test Objectives

1. ✅ Verify ESP32 connectivity
2. ✅ Test LED control via dashboard
3. ✅ Test LED control via Flutter app
4. ✅ Verify bidirectional synchronization
5. ✅ Test direct API access
6. ✅ Validate WebSocket real-time updates

---

## 📋 Pre-Test Checklist

Before starting tests:

- [ ] ESP32 powered on and connected to WiFi
- [ ] LED wired correctly (or using built-in LED)
- [ ] Serial Monitor open at 115200 baud
- [ ] Dashboard URL accessible
- [ ] Flutter app installed (if testing app)
- [ ] Same network for all devices (optional for cloud testing)

---

## Test 1: Basic Connectivity ✅

**Objective:** Verify ESP32 is online and communicating

### Steps:
1. Open Serial Monitor
2. Press ESP32 reset button
3. Observe startup messages

### Expected Output:
```
========================================
Mini Smart Home Controller v1.0
Build: MINI-001
========================================
[INFO] WiFi connected: 192.168.1.XXX
[INFO] Signal strength: Good (-65 dBm)
[INFO] mDNS started: mini_controller.local
[INFO] Redirect server started on port 80
[INFO] Local API server started on port 8080
[INFO] WebSocket server started on port 81
========================================
[INFO] System ready - LED connected to GPIO 2
========================================
```

### Success Criteria:
- ✅ WiFi connected
- ✅ All servers started
- ✅ No error messages
- ✅ IP address displayed

### If Failed:
- Check WiFi credentials
- Verify 2.4GHz network
- Move closer to router
- Re-upload firmware

**Status:** [ ] PASS [ ] FAIL

---

## Test 2: Dashboard Discovery ✅

**Objective:** Verify device appears in dashboard

### Steps:
1. Open web browser
2. Navigate to dashboard URL (from main project)
3. Wait 10 seconds for device to register
4. Look for device in list

### Expected Result:
```
Device List:
┌─────────────────────────────────┐
│ 🟢 Test LED                     │
│    Online | Mini Controller     │
│    Signal: Good                 │
│    IP: 192.168.1.XXX           │
└─────────────────────────────────┘
```

### Success Criteria:
- ✅ Device appears in list
- ✅ Status shows "Online" (green)
- ✅ Device name is "Test LED"
- ✅ IP address matches Serial Monitor

### If Failed:
- Wait longer (up to 30 seconds)
- Check GOOGLE_SCRIPT_URL in code
- Verify internet connectivity
- Check Serial Monitor for sync errors
- Refresh dashboard page

**Status:** [ ] PASS [ ] FAIL

---

## Test 3: Dashboard Control - ON/OFF ✅

**Objective:** Turn LED on and off from dashboard

### Steps:
1. Locate "Test LED" in dashboard
2. Find the ON/OFF switch
3. Click switch to turn ON
4. Observe LED and Serial Monitor
5. Click switch to turn OFF
6. Observe LED and Serial Monitor

### Expected Serial Output:
```
[INFO] LED ON - Brightness: 100%
[INFO] LED OFF
```

### Expected LED Behavior:
- ON command → LED lights up
- OFF command → LED turns off
- Response time: < 2 seconds

### Success Criteria:
- ✅ LED turns on when switch ON
- ✅ LED turns off when switch OFF
- ✅ Serial Monitor confirms state changes
- ✅ Dashboard UI updates immediately

### If Failed:
- Check LED wiring
- Verify GPIO pin is correct
- Test with built-in LED
- Check browser console for errors
- Refresh dashboard

**Status:** [ ] PASS [ ] FAIL

---

## Test 4: Dashboard Control - Brightness ✅

**Objective:** Control LED brightness from dashboard

### Steps:
1. Turn LED ON
2. Find brightness slider (if available)
3. Set brightness to 75%
4. Observe LED brightness
5. Set brightness to 25%
6. Observe LED dimming
7. Try 100%, 50%, 0%

### Expected Serial Output:
```
[INFO] LED ON - Brightness: 75%
[INFO] LED ON - Brightness: 25%
[INFO] LED ON - Brightness: 100%
[INFO] LED ON - Brightness: 50%
```

### Expected LED Behavior:
- LED brightness changes smoothly
- 100% = full bright
- 50% = half bright
- 25% = dim
- 0% = off

### Success Criteria:
- ✅ LED brightness adjusts to slider
- ✅ Visible difference between levels
- ✅ Serial Monitor shows correct values
- ✅ Smooth transitions

### If Failed:
- Some LEDs show PWM better than others
- Try different LED color
- Brightness differences more visible in dim room
- White/Blue LEDs show PWM best

**Status:** [ ] PASS [ ] FAIL

---

## Test 5: Flutter App Control ✅

**Objective:** Control LED from mobile app

### Steps:
1. Open Flutter app
2. Login (if required)
3. Navigate to Devices page
4. Find "Test LED"
5. Toggle switch ON
6. Verify LED turns on
7. Toggle switch OFF
8. Verify LED turns off

### Expected Behavior:
- App connects to cloud
- "Test LED" visible in device list
- Switch responds to taps
- LED follows app commands

### Success Criteria:
- ✅ Device appears in app
- ✅ Can control from app
- ✅ LED responds to app commands
- ✅ Status updates in app

### If Failed:
- Check app is connected to internet
- Verify login credentials
- Force close and reopen app
- Check app has correct API endpoint

**Status:** [ ] PASS [ ] FAIL

---

## Test 6: Bidirectional Sync ✅

**Objective:** Verify changes sync across all interfaces

### Steps:
1. Open dashboard on computer
2. Open app on phone
3. Place them side-by-side to watch both

4. **Test A: Dashboard → App**
   - Change LED state from dashboard
   - Watch app update automatically

5. **Test B: App → Dashboard**
   - Change LED state from app
   - Watch dashboard update automatically

6. **Test C: Both updating**
   - Rapidly toggle from dashboard
   - Verify app follows
   - Rapidly toggle from app
   - Verify dashboard follows

### Expected Behavior:
- Changes appear on other interface within 3 seconds
- No conflicts or desyncs
- Both show same state

### Success Criteria:
- ✅ Dashboard changes → App updates
- ✅ App changes → Dashboard updates
- ✅ Both stay synchronized
- ✅ LED state matches both

### If Failed:
- Check internet connectivity
- Verify cloud backend is working
- Wait longer (cloud polling interval)
- Check WebSocket connection

**Status:** [ ] PASS [ ] FAIL

---

## Test 7: Direct API Access ✅

**Objective:** Control LED via direct HTTP requests

### Prerequisites:
- Note ESP32 IP from Serial Monitor
- Install curl, Postman, or use browser

### Test 7A: Get Status

**Request:**
```bash
curl http://192.168.1.XXX:8080/status
```

**Expected Response:**
```json
{
  "devices": [
    {
      "id": 0,
      "name": "Test LED",
      "type": 0,
      "state": false,
      "brightness": 100,
      "runtime": 123,
      "locked": false
    }
  ]
}
```

### Test 7B: Turn ON

**Request:**
```bash
curl -X POST http://192.168.1.XXX:8080/control \
  -H "Content-Type: application/json" \
  -d '{"id":0,"state":true,"brightness":100}'
```

**Expected Response:**
```json
{"success": true}
```

**Expected Behavior:**
- LED turns ON
- Serial Monitor: "[INFO] LED ON - Brightness: 100%"

### Test 7C: Change Brightness

**Request:**
```bash
curl -X POST http://192.168.1.XXX:8080/control \
  -H "Content-Type: application/json" \
  -d '{"id":0,"state":true,"brightness":50}'
```

**Expected:** LED dims to 50%

### Test 7D: Turn OFF

**Request:**
```bash
curl -X POST http://192.168.1.XXX:8080/control \
  -H "Content-Type: application/json" \
  -d '{"id":0,"state":false}'
```

**Expected:** LED turns OFF

### Test 7E: Get System Info

**Request:**
```bash
curl http://192.168.1.XXX:8080/info
```

**Expected Response:**
```json
{
  "name": "Mini_Controller",
  "firmware": "1.0",
  "build": "MINI-001",
  "mac": "AA:BB:CC:DD:EE:FF",
  "ip": "192.168.1.XXX",
  "uptime": 3600,
  "rssi": -45,
  "signal": "Excellent",
  "heap": 250000,
  "cloud_connected": true
}
```

### Success Criteria:
- ✅ All endpoints respond
- ✅ JSON format is correct
- ✅ LED responds to commands
- ✅ System info is accurate

**Status:** [ ] PASS [ ] FAIL

---

## Test 8: WebSocket Real-Time Updates ✅

**Objective:** Verify WebSocket broadcasts state changes

### Tools Needed:
- WebSocket client (browser extension or tool)
- Or use browser console

### Test Using Browser Console:

1. Open browser console (F12)
2. Paste this code:
```javascript
const ws = new WebSocket('ws://192.168.1.XXX:81');

ws.onopen = () => {
  console.log('WebSocket Connected!');
};

ws.onmessage = (event) => {
  console.log('Received:', event.data);
};

ws.onerror = (error) => {
  console.log('Error:', error);
};
```

3. Control LED from dashboard or API
4. Watch console for messages

### Expected Messages:
```json
{
  "type": "device_update",
  "id": 0,
  "state": true,
  "brightness": 75,
  "name": "Test LED"
}
```

### Success Criteria:
- ✅ WebSocket connects successfully
- ✅ Receives messages when LED changes
- ✅ Messages contain correct data
- ✅ Real-time updates (< 500ms delay)

**Status:** [ ] PASS [ ] FAIL

---

## Test 9: Stress Test ✅

**Objective:** Verify system stability under load

### Steps:
1. Rapidly toggle LED 20 times
2. Change brightness repeatedly
3. Control from multiple sources simultaneously
4. Monitor for errors or crashes

### Expected Behavior:
- No crashes
- No missed commands
- Serial Monitor clean (no errors)
- LED responds to all commands

### Success Criteria:
- ✅ Handles rapid commands
- ✅ No memory leaks (heap stable)
- ✅ No connection drops
- ✅ Smooth operation

**Status:** [ ] PASS [ ] FAIL

---

## Test 10: Recovery Test ✅

**Objective:** Verify system recovers from failures

### Test 10A: WiFi Recovery
1. Disconnect WiFi router
2. Wait 10 seconds
3. Reconnect router
4. Verify ESP32 reconnects automatically

### Test 10B: Power Cycle
1. Unplug ESP32
2. Wait 5 seconds
3. Plug back in
4. Verify boots correctly
5. Verify reconnects to WiFi

### Test 10C: Dashboard Disconnect
1. Close dashboard
2. Wait 1 minute
3. Reopen dashboard
4. Verify device still appears

### Success Criteria:
- ✅ Auto-reconnects to WiFi
- ✅ Survives power cycles
- ✅ Maintains state after reconnect
- ✅ No manual intervention needed

**Status:** [ ] PASS [ ] FAIL

---

## 📊 Test Summary

| Test | Description | Status | Notes |
|------|-------------|--------|-------|
| 1 | Basic Connectivity | [ ] | |
| 2 | Dashboard Discovery | [ ] | |
| 3 | Dashboard ON/OFF | [ ] | |
| 4 | Dashboard Brightness | [ ] | |
| 5 | Flutter App Control | [ ] | |
| 6 | Bidirectional Sync | [ ] | |
| 7 | Direct API Access | [ ] | |
| 8 | WebSocket Updates | [ ] | |
| 9 | Stress Test | [ ] | |
| 10 | Recovery Test | [ ] | |

**Overall Result:** [ ] ALL PASS [ ] SOME FAILED

---

## 🐛 Common Issues and Solutions

### Issue: LED not responding
**Solution:** Check wiring, verify GPIO pin, test with built-in LED

### Issue: Device not appearing in dashboard
**Solution:** Check cloud URL, verify internet, wait 30 seconds, check Serial Monitor

### Issue: App not connecting
**Solution:** Check app API configuration, verify login, restart app

### Issue: Slow updates
**Solution:** Normal with 2.5s polling, check WiFi signal, move closer to router

### Issue: WebSocket not connecting
**Solution:** Check firewall, try IP instead of hostname, verify port 81

---

## ✅ Final Validation

All tests passed? Congratulations! 🎉

You have successfully:
- ✅ Built and configured mini controller
- ✅ Verified hardware connectivity
- ✅ Tested dashboard integration
- ✅ Tested mobile app integration
- ✅ Confirmed bidirectional sync
- ✅ Validated API endpoints
- ✅ Verified real-time updates

**Next Steps:**
1. Study the code to understand implementation
2. Customize for your needs
3. Progress to full production firmware
4. Build the complete smart home system

---

## 📝 Test Notes

**Date:** _______________

**Tester:** _______________

**ESP32 Board:** _______________

**LED Color:** _______________

**WiFi Network:** _______________

**Issues Found:**
```



```

**Additional Comments:**
```



```

---

**Happy Testing! 🚀**

Record all test results and keep this document for reference.
