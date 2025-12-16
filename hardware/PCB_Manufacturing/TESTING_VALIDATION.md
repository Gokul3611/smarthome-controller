# Testing and Validation Protocol
## Smart Home Controller v3.1 - Production Quality Assurance

### Document Control
- **Version:** 1.0
- **Date:** December 2024
- **Classification:** Internal Use
- **Prepared by:** Hardware Engineering Team

---

## 1. SCOPE AND OBJECTIVES

This document establishes the testing and validation procedures for the Smart Home Controller v3.1 PCB assembly. The procedures ensure:

- Electrical safety compliance
- Functional operation across all subsystems
- Reliability under specified operating conditions
- Integration compatibility with firmware, backend, and frontend systems

**Test Coverage:** Hardware, firmware integration, system integration, safety compliance, and reliability validation.

---

## 2. TEST EQUIPMENT REQUIREMENTS

### 2.1 Essential Equipment

| Equipment | Specification | Purpose |
|-----------|--------------|---------|
| Digital Multimeter | 6.5 digit, 0.01% accuracy | Voltage, current, resistance measurement |
| Oscilloscope | 100MHz, 4-channel | Waveform analysis, timing verification |
| Isolation Tester | 500V-1000V DC | Insulation resistance testing |
| Power Supply | 5V 2A, regulated | ESP32 power for testing |
| Variac Transformer | 0-250V AC, 10A | Variable AC voltage testing |
| Temperature Sensor | K-type thermocouple | Thermal verification |
| Soldering Station | Temperature controlled | Assembly and rework |

### 2.2 Optional Equipment

- Logic analyzer (timing analysis)
- Thermal camera (hot spot identification)
- EMI receiver (compliance testing)
- High-pot tester (dielectric withstand)

---

## 3. INCOMING INSPECTION

### 3.1 PCB Inspection

**Visual Inspection Criteria:**
- Board dimensions: 70mm ± 0.2mm × 50mm ± 0.2mm
- Soldermask: No scratches, complete coverage
- Silkscreen: Legible, no smearing
- Copper traces: No breaks, adequate width
- Mounting holes: 2.7mm ± 0.05mm diameter

**Electrical Tests:**
```
Test 1: Isolation Resistance (AC to DC sections)
  Method: 500V megohm meter
  Pass Criteria: >10MΩ
  
Test 2: Continuity (Ground plane)
  Method: 4-wire resistance
  Pass Criteria: <1Ω between any two ground points
```

### 3.2 Component Inspection

Verify components against BOM:
- Correct part numbers
- Proper orientation markings
- No physical damage
- Date codes within specification

---

## 4. ASSEMBLY VERIFICATION

### 4.1 Post-Solder Inspection

**Critical Checks:**

1. **Polarity-sensitive components:**
   - Electrolytic capacitor C1 (positive marking)
   - Diodes in bridge rectifier BR1
   - Optocouplers U1-U5 (pin 1 marking)

2. **Component orientation:**
   - ESP32 module antenna pointing outward
   - TRIAC pinout: MT1, MT2, Gate verified
   - MOC3041 pin 1 indicators aligned

3. **Solder joint quality:**
   - Smooth, shiny appearance
   - Complete wetting
   - No bridges between adjacent pins
   - Proper fillet shape

### 4.2 Visual Defect Classification

| Defect | Class | Action |
|--------|-------|--------|
| Solder bridge | Critical | Rework required |
| Cold solder joint | Major | Rework required |
| Tombstoned component | Major | Rework required |
| Insufficient solder | Minor | Rework recommended |
| Flux residue | Minor | Clean if excessive |

---

## 5. ELECTRICAL TESTING

### 5.1 Power-Off Tests

**Test 5.1.1: Isolation Resistance**
```
Procedure:
1. Apply 500V DC between AC Live and ESP32 GND
2. Measure resistance
3. Pass: >10MΩ

Expected: 50-100MΩ (typical for new PCB)
Failure: <10MΩ indicates isolation fault
```

**Test 5.1.2: Ground Continuity**
```
Procedure:
1. Measure resistance between all GND points
2. Use 4-wire Kelvin measurement
3. Pass: <1Ω

Expected: 0.1-0.5Ω (copper plane resistance)
Failure: >1Ω indicates open connection
```

**Test 5.1.3: Power Rail Shorts**
```
Procedure:
1. Measure resistance: 5V to GND
2. Measure resistance: 3.3V to GND
3. Pass: >100kΩ

Expected: >1MΩ (no power applied)
Failure: <100kΩ indicates potential short
```

### 5.2 Power-On Tests (DC Only)

**Test 5.2.1: Power Supply Output**
```
Setup:
- Connect 5V 2A supply to HLK-PM01 input
- No AC connected
- ESP32 not installed

Procedure:
1. Apply 5V DC input
2. Measure output voltage at ESP32 VIN
3. Pass: 4.95V - 5.05V

Expected: 5.00V ± 50mV
Load test: Apply 500mA load, voltage should remain within spec
```

**Test 5.2.2: Current Consumption**
```
Procedure:
1. Measure current from 5V supply (no ESP32)
2. Pass: <50mA

Expected: 10-20mA (power module quiescent)
Failure: >50mA indicates fault in power section
```

### 5.3 ESP32 Programming Test

**Test 5.3.1: Firmware Upload**
```
Procedure:
1. Install ESP32 module
2. Connect USB-to-serial adapter
3. Upload test firmware (blink sketch)
4. Verify Serial output at 115200 baud
5. Pass: Upload successful, LED blinks

Expected: Clean upload, no errors
Failure: Upload errors indicate connection/power issues
```

**Test 5.3.2: GPIO Functionality**
```
Procedure:
1. Upload GPIO test sketch
2. Verify each GPIO toggles:
   - GPIO 13 (input with pull-down)
   - GPIO 16-19 (outputs to MOC3041)
   - GPIO 32,33,25,26 (inputs with pull-up)
3. Pass: All GPIOs functional

Measurement: Use multimeter or oscilloscope
Expected: 3.3V HIGH, 0V LOW
```

---

## 6. FUNCTIONAL TESTING

### 6.1 Zero-Cross Detection Test

**Test 6.1.1: Signal Verification (12V AC)**
```
Setup:
- Apply 12V AC to zero-cross transformer input
- Connect oscilloscope to GPIO 13
- ESP32 powered via USB

Procedure:
1. Observe waveform on oscilloscope
2. Verify frequency: 100Hz (50Hz mains) or 120Hz (60Hz)
3. Verify amplitude: 3.3V
4. Verify pulse width: 1-3ms
5. Pass: Signal present and stable

Expected: Clean square pulses at line frequency × 2
Failure: No signal or irregular waveform indicates zero-cross fault
```

**Test 6.1.2: Firmware Recognition**
```
Procedure:
1. Monitor Serial output
2. Verify firmware detects zero-cross events
3. Check for error messages: "Zero-cross signal lost"
4. Pass: Firmware reports zero-cross detection

Expected: Console shows zero-cross count incrementing
```

### 6.2 TRIAC Switching Test (Low Voltage)

**Test 6.2.1: 12V AC Load Test**
```
Setup:
- Connect 12V AC automotive bulb as load
- Series ammeter to measure current
- Do NOT use 230V yet

Procedure for each channel:
1. Command channel ON via Serial/API
2. Verify bulb illuminates
3. Measure current draw
4. Command channel OFF
5. Verify bulb extinguishes
6. Pass: All 4 channels switch properly

Expected: Smooth on/off, no flickering
Failure: No switching indicates TRIAC or optocoupler fault
```

**Test 6.2.2: Dimming Verification**
```
Procedure:
1. Set brightness to 25%, 50%, 75%, 100%
2. Visually verify brightness changes
3. Use oscilloscope to verify TRIAC firing angle
4. Pass: Smooth dimming at all levels

Expected: Proportional brightness to setting
Measurement: GPIO pulse timing should match calculated fire tick
```

### 6.3 Physical Switch Test

**Test 6.3.1: Switch Response**
```
Procedure for each switch:
1. Short switch input to GND momentarily
2. Verify Serial output shows switch event
3. Verify device toggles state
4. Pass: All 4 switches responsive

Expected: <50ms response time
Debounce verification: Rapid presses should not cause false triggers
```

---

## 7. SYSTEM INTEGRATION TESTING

### 7.1 WiFi Connectivity Test

**Test 7.1.1: Network Connection**
```
Procedure:
1. Configure WiFi credentials
2. Verify connection to network
3. Check RSSI value: Should be >-80dBm
4. Pass: Stable connection for 5 minutes

Expected: Connection within 10 seconds
Ping test: <50ms latency to gateway
```

**Test 7.1.2: Auto-Reconnect**
```
Procedure:
1. Disconnect WiFi AP
2. Wait 30 seconds
3. Restore AP
4. Verify automatic reconnection
5. Pass: Reconnects within 1 minute

Expected: Exponential backoff, eventual success
```

### 7.2 Cloud Communication Test

**Test 7.2.1: Backend Polling**
```
Procedure:
1. Configure Google Apps Script URL
2. Monitor Serial output for polling events
3. Verify 2.5 second interval
4. Check for successful HTTP responses
5. Pass: Consistent polling, no errors

Expected: HTTP 200 responses
Failure modes: Network errors should trigger retry
```

**Test 7.2.2: State Synchronization**
```
Procedure:
1. Change device state via backend
2. Verify firmware receives update on next poll
3. Verify TRIAC responds
4. Check state reported back to backend
5. Pass: Bidirectional sync working

Expected: <5 second latency (2 × poll interval)
```

### 7.3 WebSocket Test

**Test 7.3.1: Client Connection**
```
Procedure:
1. Connect WebSocket client to port 81
2. Verify connection accepted
3. Change device state
4. Verify client receives update message
5. Pass: Real-time updates functional

Expected: <100ms update latency
Message format: JSON with device state
```

### 7.4 REST API Test

**Test 7.4.1: Endpoint Verification**
```
Test each endpoint:

GET /status
  Expected: 200 OK, JSON device states

POST /control
  Body: {"id":0, "state":true, "brightness":75}
  Expected: 200 OK, device changes state

GET /info
  Expected: 200 OK, system information

Pass: All endpoints respond correctly
```

### 7.5 Voice Assistant Test

**Test 7.5.1: Alexa Discovery**
```
Procedure:
1. Open Alexa app
2. Discover devices
3. Verify all 4 channels appear
4. Test voice command: "Alexa, turn on [device name]"
5. Pass: Voice control functional

Expected: Discovery <30 seconds
Control latency: <2 seconds
```

---

## 8. MAINS VOLTAGE TESTING (230V/110V AC)

### ⚠️ SAFETY PRECAUTIONS

**DANGER: LETHAL VOLTAGE**

Before proceeding:
- [ ] GFCI/RCD protection installed
- [ ] Emergency power cutoff accessible
- [ ] Insulated tools only
- [ ] Dry work surface
- [ ] Safety observer present
- [ ] Fire extinguisher nearby
- [ ] First aid kit available
- [ ] Enclosure closed and secured

### 8.1 Pre-Power Checks

**Critical Verification:**
```
1. Visual inspection: No exposed conductors
2. Isolation test: AC to DC >10MΩ confirmed
3. Continuity: All grounds connected
4. Polarity: Live, Neutral, Ground correct
5. Load rating: Within TRIAC specifications
6. Fuses: Properly rated and installed
```

### 8.2 Initial Power Application

**Test 8.2.1: No-Load Power-On**
```
Procedure:
1. Disconnect all loads
2. Apply AC mains slowly via variac
3. Start at 50V, increase in 50V steps
4. Monitor for sparks, smoke, unusual sounds
5. Reach full voltage (230V or 110V)
6. Pass: No faults observed

Expected: Quiet operation, no visible issues
Measure: AC voltage at all expected points
```

**Test 8.2.2: Power Supply Verification**
```
Procedure:
1. With AC applied, measure DC voltages:
   - HLK-PM01 output: 5.0V ± 0.1V
   - ESP32 3.3V rail: 3.3V ± 0.1V
2. Measure current draw from AC
3. Pass: All voltages within specification

Expected AC current: 50-100mA no-load
```

### 8.3 Load Testing

**Test 8.3.1: Single Channel Load Test**
```
Setup:
- Connect 40W incandescent bulb to Channel 1
- All other channels disconnected

Procedure:
1. Turn on Channel 1
2. Verify bulb illuminates
3. Measure AC current
4. Check TRIAC temperature
5. Run for 5 minutes
6. Pass: Stable operation

Expected current: 40W / 230V = 0.17A
TRIAC temperature: <50°C above ambient
```

**Test 8.3.2: Multi-Channel Load Test**
```
Setup:
- Connect appropriate loads to all 4 channels
- Total load <10A (2.5A per channel)

Procedure:
1. Turn on all channels sequentially
2. Measure current per channel
3. Check for voltage drop
4. Monitor TRIAC temperatures
5. Run for 30 minutes
6. Pass: All channels stable

Temperature limits:
  TRIACs: <60°C above ambient
  ESP32: <50°C above ambient
  Power supply: <70°C above ambient
```

**Test 8.3.3: Dimming Under Load**
```
Procedure:
1. Set each channel to 25%, 50%, 75%, 100%
2. Verify smooth dimming
3. Check for flickering
4. Measure AC waveform with oscilloscope
5. Pass: Clean dimming, no instability

Expected: Proportional power delivery
No flicker: MOC3041 zero-cross prevents it
```

### 8.4 Thermal Testing

**Test 8.4.1: Steady-State Thermal**
```
Procedure:
1. All channels at 75% with resistive loads
2. Run for 1 hour
3. Measure temperatures every 10 minutes:
   - TRIAC junctions (IR thermometer)
   - ESP32 module
   - HLK-PM01 power supply
   - PCB hottest spot
4. Pass: Temperatures stabilize <limits

Temperature limits (ambient + rise):
  TRIACs: <85°C
  ESP32: <70°C
  HLK-PM01: <80°C
  PCB: <75°C
```

**Test 8.4.2: Thermal Cycling**
```
Procedure:
1. Cycle loads: 100% for 10 min, OFF for 10 min
2. Repeat 10 cycles
3. Monitor for thermal stress failures
4. Pass: No failures, stable operation

Expected: Temperatures cycle predictably
Failure modes: Solder joint cracks, component drift
```

### 8.5 Stress Testing

**Test 8.5.1: Overvoltage Test**
```
Setup:
- Variac to provide variable AC input
- Monitor with oscilloscope

Procedure:
1. Slowly increase voltage to 110% nominal
2. Monitor for MOV activation
3. Verify continued operation
4. Return to nominal voltage
5. Pass: Operates within 90-110% voltage range

MOV activation: Should clamp voltage >275V (230V system)
```

**Test 8.5.2: Rapid Switching Test**
```
Procedure:
1. Command all channels ON/OFF rapidly
2. Rate: 10 cycles per second for 1 minute
3. Monitor for failures or instability
4. Pass: No missed commands, stable operation

Expected: Zero-cross switching handles rapid commands
Failure: May indicate race condition or timing issue
```

**Test 8.5.3: Inductive Load Test**
```
Setup:
- Connect inductive load (fan motor, transformer)
- Series wattmeter for power factor measurement

Procedure:
1. Turn on inductive load
2. Measure voltage and current
3. Check TRIAC for overheating
4. Monitor for voltage spikes
5. Pass: Stable operation with inductive load

Derate: 50% current for inductive loads
Power factor: Expect 0.5-0.8 (lagging)
```

---

## 9. SAFETY COMPLIANCE TESTING

### 9.1 Electrical Safety

**Test 9.1.1: Dielectric Withstand (Hi-Pot)**
```
Equipment: AC hi-pot tester

Procedure:
1. Apply 1000V AC between AC and DC sections
2. Duration: 60 seconds
3. Current limit: 10mA
4. Pass: No breakdown, current <1mA

Warning: Destructive test if fails
Perform on sample units, not every board
```

**Test 9.1.2: Insulation Resistance (Megger)**
```
Procedure:
1. Apply 500V DC between AC and DC
2. Read after 60 seconds
3. Pass: >10MΩ

Expected: 50-100MΩ for new assemblies
Minimum acceptable: 10MΩ per safety standards
```

**Test 9.1.3: Ground Continuity**
```
Procedure:
1. Measure resistance from AC ground terminal to all exposed metal
2. Use 4-wire measurement
3. Pass: <0.1Ω

Critical: Ensures safety ground effective
Failure: Indicates poor connection, safety hazard
```

### 9.2 EMC Testing (Optional but Recommended)

**Test 9.2.1: Radiated Emissions**
```
Standard: FCC Part 15 Class B or EN 55022

Procedure:
1. Place unit in semi-anechoic chamber
2. Measure radiated emissions 30MHz-1GHz
3. Compare to limits
4. Pass: Emissions below Class B limits

Typical issues: WiFi harmonics, TRIAC switching noise
Mitigation: Ground plane design reduces emissions
```

**Test 9.2.2: Conducted Emissions**
```
Standard: FCC Part 15 or EN 55022

Procedure:
1. Use LISN (Line Impedance Stabilization Network)
2. Measure noise on AC mains 150kHz-30MHz
3. Pass: Below limits

Typical issues: TRIAC switching transients
Mitigation: MOC3041 zero-cross reduces conducted noise
```

---

## 10. RELIABILITY TESTING

### 10.1 Burn-In Test

**Test 10.1.1: Extended Operation**
```
Procedure:
1. Operate at 75% load for 72 hours
2. Monitor for failures:
   - Firmware crashes
   - Network disconnects
   - TRIAC failures
   - Temperature excursions
3. Log all events
4. Pass: Zero critical failures

Acceptance criteria:
  Critical failures: 0
  Minor issues: <3 per unit
  Uptime: >99%
```

### 10.2 Temperature Cycling

**Test 10.2.1: Environmental Stress**
```
Equipment: Temperature chamber

Procedure:
1. Cycle: -20°C for 1 hour, +70°C for 1 hour
2. Repeat 100 cycles
3. Functional test after every 10 cycles
4. Pass: No parametric drift or failures

Failure modes to watch:
  - Solder joint cracks
  - Capacitor ESR increase
  - Connector loosening
```

### 10.3 On/Off Cycling

**Test 10.3.1: Power Cycle Endurance**
```
Procedure:
1. Power on for 1 minute
2. Power off for 1 minute
3. Repeat 1000 cycles
4. Pass: Boots reliably every time

Tests: Inrush current handling, capacitor charging
Firmware: NVS wear leveling (flash writes)
```

### 10.4 Switch Endurance

**Test 10.4.1: Mechanical Switch Life**
```
Procedure:
1. Actuate each physical switch
2. Rate: 1 press per second
3. Duration: 10,000 cycles per switch
4. Pass: Switches remain responsive

Expected life: >100,000 cycles for quality switches
Failure mode: Contact wear, spring fatigue
```

---

## 11. FIRMWARE INTEGRATION VALIDATION

### 11.1 Pin Mapping Verification

**Test 11.1.1: GPIO Mapping**
```
Verification Matrix:

config.h Pin | Hardware Connection | Test Result
─────────────────────────────────────────────────
GPIO 13      | Zero-cross detect   | [ ]
GPIO 16      | MOC3041 U2 (Ch1)   | [ ]
GPIO 17      | MOC3041 U3 (Ch2)   | [ ]
GPIO 18      | MOC3041 U4 (Ch3)   | [ ]
GPIO 19      | MOC3041 U5 (Ch4)   | [ ]
GPIO 32      | Switch 1 input      | [ ]
GPIO 33      | Switch 2 input      | [ ]
GPIO 25      | Switch 3 input      | [ ]
GPIO 26      | Switch 4 input      | [ ]

Method: Trace from ESP32 pin to component, verify continuity
Pass: All mappings correct
```

### 11.2 Timing Verification

**Test 11.2.1: Zero-Cross Timing**
```
Measurement:
1. Capture zero-cross signal on oscilloscope
2. Capture TRIAC firing pulse
3. Measure delay from zero-cross to firing
4. Pass: Delay matches firmware calculation

Formula: delay = fireTick × 100µs
Example: 75% brightness → fireTick=25 → delay=2.5ms
Tolerance: ±50µs
```

**Test 11.2.2: Phase Angle Control**
```
Procedure:
1. Set brightness to known value
2. Capture AC waveform with current probe
3. Measure conduction angle
4. Pass: Matches expected power delivery

Verification: RMS power ≈ brightness percentage
Measurement: Oscilloscope with math functions
```

### 11.3 Watchdog Verification

**Test 11.3.1: Core 0 Watchdog**
```
Procedure:
1. Inject infinite loop in Core 0 task
2. Monitor for watchdog reset
3. Pass: System reboots within 15 seconds

Expected: Firmware logs watchdog event
Verification: Serial output shows reset reason
```

**Test 11.3.2: Zero-Cross Health Check**
```
Procedure:
1. Disconnect zero-cross signal
2. Monitor for safety shutdown
3. Pass: TRIACs disabled within 100ms

Expected: Error message: "Zero-cross signal lost"
Verification: All GPIO 16-19 go LOW
```

---

## 12. BACKEND INTEGRATION VALIDATION

### 12.1 API Compatibility Test

**Test 12.1.1: Polling Request Format**
```
Firmware sends to backend:
{
  "action": "poll",
  "uid": "AA:BB:CC:DD:EE:FF",
  "ver": 3.0,
  "uptime": 3600,
  "rssi": -45,
  "heap": 180000,
  "d1": {"s": 1, "v": 75, "t": 1, "runtime": 1800},
  "d2": {"s": 0, "v": 100, "t": 0, "runtime": 0},
  "d3": {"s": 1, "v": 50, "t": 2, "runtime": 900},
  "d4": {"s": 0, "v": 100, "t": 0, "runtime": 0}
}

Verification:
1. Capture HTTP POST with Wireshark
2. Verify JSON structure matches spec
3. Pass: Backend accepts request
```

**Test 12.1.2: Response Parsing**
```
Backend response:
{
  "sys_name": "Living Room Hub",
  "ota_update": false,
  "d1": {"type": "FAN", "name": "Ceiling Fan", "update": true, "state": 1, "val": 80},
  "schedules": [...],
  "scenes": [...]
}

Verification:
1. Firmware receives response
2. Device name updates
3. State changes if update=true
4. Pass: All fields processed correctly
```

### 12.2 State Synchronization Test

**Test 12.2.1: Bidirectional Sync**
```
Procedure:
1. Change state via Dashboard
2. Verify backend database updates
3. Verify firmware receives update on next poll
4. Verify hardware (TRIAC) responds
5. Pass: End-to-end sync <5 seconds

Measurement: Timestamp each step
Expected: Dashboard → Backend: <500ms
          Backend → Firmware: <2.5s (next poll)
          Firmware → Hardware: <50ms
```

---

## 13. FRONTEND INTEGRATION VALIDATION

### 13.1 Dashboard Testing

**Test 13.1.1: WebSocket Real-Time Updates**
```
Procedure:
1. Open Dashboard in browser
2. Toggle device via physical switch
3. Verify Dashboard updates immediately
4. Pass: Update latency <500ms

Verification: Browser console logs WebSocket messages
Expected: JSON message with device state
```

**Test 13.1.2: Control Commands**
```
Test Matrix:

Dashboard Action    | Expected Result           | Pass/Fail
────────────────────────────────────────────────────────
Toggle switch ON    | TRIAC activates          | [ ]
Toggle switch OFF   | TRIAC deactivates        | [ ]
Slider to 50%       | Brightness changes       | [ ]
Scene activation    | All devices update       | [ ]
Schedule creation   | Stored and executes      | [ ]

Pass: All actions result in expected hardware response
```

### 13.2 Flutter App Testing

**Test 13.2.1: Device Discovery**
```
Procedure:
1. Open Flutter app on same network
2. Wait for mDNS discovery
3. Verify device appears in list
4. Pass: Discovery within 30 seconds

Expected: Device name, IP address shown
```

**Test 13.2.2: Local REST API Control**
```
Procedure:
1. Send POST /control from app
2. Verify device responds
3. Verify app receives status update
4. Pass: Control loop functional

Latency measurement: <200ms for local control
```

---

## 14. ACCEPTANCE CRITERIA

### 14.1 Minimum Acceptance Requirements

**Hardware:**
- [ ] All electrical tests pass
- [ ] Isolation resistance >10MΩ
- [ ] Zero-cross detection functional
- [ ] All 4 TRIAC channels operational
- [ ] Thermal limits not exceeded

**Firmware:**
- [ ] Uploads successfully
- [ ] WiFi connects reliably
- [ ] Cloud polling active
- [ ] WebSocket server running
- [ ] All GPIO mappings correct

**Integration:**
- [ ] Backend API communication functional
- [ ] Dashboard control works
- [ ] Flutter app discovers and controls
- [ ] Voice assistant responds (if configured)
- [ ] Physical switches override

**Safety:**
- [ ] No exposed live conductors
- [ ] Fuses properly rated
- [ ] Ground continuity verified
- [ ] Temperature limits complied
- [ ] No arcing or sparking observed

### 14.2 Quality Gates

**Gate 1: Post-Assembly**
- Visual inspection pass
- Basic electrical tests pass
- Ready for programming

**Gate 2: Post-Programming**
- Firmware uploads successfully
- Basic functionality verified
- Ready for integration testing

**Gate 3: System Integration**
- WiFi connectivity stable
- Backend communication established
- Frontend control verified
- Ready for mains voltage testing

**Gate 4: Mains Voltage Testing**
- All channels switch under load
- Thermal limits satisfied
- No safety violations
- Ready for reliability testing

**Gate 5: Final Release**
- All reliability tests pass
- Documentation complete
- Ready for installation

---

## 15. TEST REPORTING

### 15.1 Test Record Template

```
TEST RECORD
===========
Date: ____________
Tester: ____________
Board Serial Number: ____________
Firmware Version: ____________

RESULTS SUMMARY:
[ ] Pass  [ ] Fail  [ ] Conditional Pass

Critical Failures: ____
Major Issues: ____
Minor Issues: ____

DETAILED RESULTS:
Section 3 (Incoming): [ ] Pass [ ] Fail
Section 4 (Assembly): [ ] Pass [ ] Fail
Section 5 (Electrical): [ ] Pass [ ] Fail
Section 6 (Functional): [ ] Pass [ ] Fail
Section 7 (Integration): [ ] Pass [ ] Fail
Section 8 (Mains Voltage): [ ] Pass [ ] Fail
Section 9 (Safety): [ ] Pass [ ] Fail

NOTES:
_________________________________________________
_________________________________________________

DISPOSITION:
[ ] Released for installation
[ ] Rework required
[ ] Scrap

Signature: ____________  Date: ____________
```

### 15.2 Failure Analysis

**Failure Classification:**

| Category | Examples | Action |
|----------|----------|--------|
| Design flaw | Incorrect trace width | Engineering change order |
| Assembly defect | Cold solder joint | Rework and retest |
| Component failure | Defective TRIAC | Replace and retest |
| Test setup error | Incorrect voltage applied | Repeat test |

**Root Cause Analysis:**
1. Identify failure mode
2. Trace to root cause
3. Implement corrective action
4. Verify effectiveness
5. Update procedures

---

## 16. REVISION HISTORY

| Version | Date | Changes | Author |
|---------|------|---------|--------|
| 1.0 | Dec 2024 | Initial release | Hardware Team |

---

## APPENDIX A: TEST CHECKLISTS

### Quick Inspection Checklist

**5-Minute Quick Test (Post-Assembly):**
- [ ] Visual inspection clean
- [ ] Power supply outputs 5V
- [ ] ESP32 programs successfully
- [ ] All GPIO toggle correctly
- [ ] Zero-cross signal present (12V AC test)

**30-Minute Integration Test:**
- [ ] WiFi connects
- [ ] Backend polling works
- [ ] WebSocket active
- [ ] Dashboard controls device
- [ ] Physical switches respond

**Full Validation (4 hours):**
- [ ] All sections 3-9 complete
- [ ] Mains voltage testing done
- [ ] Thermal testing satisfactory
- [ ] Documentation complete

---

## APPENDIX B: TROUBLESHOOTING GUIDE

| Symptom | Probable Cause | Check | Fix |
|---------|---------------|-------|-----|
| No 5V output | Power supply failure | Input voltage, component orientation | Replace HLK-PM01 |
| ESP32 won't program | Connection issue | USB cable, drivers, EN/GPIO0 | Check connections |
| No zero-cross signal | Circuit fault | Resistor values, optocoupler | Replace PC817 |
| TRIAC won't switch | Multiple possible | GPIO output, optocoupler, TRIAC | Test systematically |
| Overheating | Insufficient cooling | Thermal vias, load current | Add heatsink or reduce load |
| WiFi drops | Power supply noise | Capacitor values, routing | Add filtering |
| Intermittent faults | Cold solder joint | Reflow suspected joints | Rework |

---

**END OF TESTING AND VALIDATION PROTOCOL**

Document prepared in accordance with IPC-A-610 (Acceptability of Electronic Assemblies) and ISO 9001 (Quality Management Systems).

For questions or clarification, contact: hardware-team@project.local
