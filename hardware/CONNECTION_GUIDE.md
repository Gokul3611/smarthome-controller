# Connection Guide - Smart Home Controller

Complete step-by-step guide for hardware connections and setup.

---

## ⚠️ SAFETY FIRST

**DANGER: HIGH VOLTAGE**

This project involves mains AC voltage which can be **LETHAL**. Follow these safety rules:

1. ✅ **Always disconnect power** before making connections
2. ✅ **Use insulated tools** and work on non-conductive surface
3. ✅ **Double-check all connections** before applying power
4. ✅ **Use proper gauge wires** (minimum 18 AWG for loads)
5. ✅ **Never touch live circuits** - wait 5 minutes after power off
6. ✅ **Have someone nearby** when testing with mains voltage
7. ✅ **Use GFCI/RCD protection** on the AC supply
8. ✅ **Wear safety glasses** when working

**If unsure, consult a licensed electrician.**

---

## 📦 Required Materials

### Components (per channel)

- [ ] ESP32-WROOM-32 development board
- [ ] BT136 TRIAC (or equivalent 4A/600V)
- [ ] MOC3021 or MOC3041 optocoupler
- [ ] 330Ω resistor (1/4W) - for ESP32 to optocoupler
- [ ] 100Ω resistor (1/2W) - for TRIAC gate
- [ ] 10kΩ resistor (1/4W) - pull-down for zero-cross
- [ ] 0.1µF ceramic capacitor
- [ ] Heatsink for TRIAC (TO-220)
- [ ] Screw terminal blocks
- [ ] Enclosure (non-conductive)

### Zero-Cross Detection

- [ ] 230V to 12V AC transformer (or voltage divider)
- [ ] 1N4007 diodes (x4) - bridge rectifier
- [ ] 4N25 optocoupler
- [ ] 1kΩ and 10kΩ resistors

### Power Supply

- [ ] 5V 2A power supply (isolated)
- [ ] AMS1117-3.3 regulator (if not on ESP32 board)
- [ ] Electrolytic capacitors (100µF, 10µF)

### Tools

- [ ] Soldering iron and solder
- [ ] Wire strippers
- [ ] Multimeter
- [ ] Screwdrivers
- [ ] Heat shrink tubing
- [ ] Cable ties

---

## 🔧 Step 1: Zero-Cross Detection Circuit

### Circuit Diagram

```
AC Mains → [Fuse 1A] → Step-down Transformer (230V to 12V AC)
                              ↓
                    Bridge Rectifier (1N4007 x4)
                              ↓
                         [1kΩ Resistor]
                              ↓
                    4N25 Optocoupler (pins 1,2)
                              ↓
                         [10kΩ to GND]
                              ↓
                    4N25 Output (pins 4,5) → ESP32 GPIO13
```

### Connections

1. **AC Input**:
   ```
   Mains Live → Fuse (1A) → Transformer Primary
   Mains Neutral → Transformer Primary
   ```

2. **Bridge Rectifier**:
   ```
   Transformer Secondary (AC) → Bridge Rectifier Input
   Bridge (+) → 1kΩ resistor → 4N25 pin 1 (anode)
   Bridge (-) → 4N25 pin 2 (cathode)
   ```

3. **Optocoupler Output**:
   ```
   4N25 pin 5 → ESP32 3.3V
   4N25 pin 4 → ESP32 GPIO13
   10kΩ resistor → GPIO13 to GND (pull-down)
   ```

### Testing Zero-Cross

1. Power the circuit (no ESP32 yet)
2. Use oscilloscope on 4N25 pin 4
3. Should see pulses at 100Hz (50Hz mains) or 120Hz (60Hz mains)
4. Pulse width: ~1-2ms

---

## 🔧 Step 2: TRIAC Driver Circuit (Per Channel)

### Circuit Diagram (for one channel)

```
ESP32 GPIO16 → [330Ω] → MOC3021 pin 1 (anode)
ESP32 GND → MOC3021 pin 2 (cathode)

MOC3021 pin 6 → [100Ω] → TRIAC Gate
MOC3021 pin 4 → TRIAC MT1

TRIAC MT2 → Load → AC Neutral
AC Live → TRIAC MT1
```

### Detailed Connections

#### For Channel 1 (Repeat for channels 2-4)

1. **ESP32 to Optocoupler**:
   ```
   ESP32 GPIO16 → 330Ω resistor → MOC3021 pin 1
   ESP32 GND → MOC3021 pin 2
   ```

2. **Optocoupler to TRIAC**:
   ```
   MOC3021 pin 6 → 100Ω resistor → BT136 Gate
   MOC3021 pin 4 → BT136 MT1
   ```

3. **TRIAC to Load** (HIGH VOLTAGE):
   ```
   AC Live Wire → BT136 MT1
   BT136 MT2 → Load (Light/Fan)
   Load → AC Neutral
   ```

4. **Add Snubber Circuit** (optional but recommended):
   ```
   Between MT1 and MT2:
   - 100Ω resistor (2W) in series with 0.1µF (400V) capacitor
   ```

### Pin Numbering

**MOC3021 (DIP-6)**:
```
  1 [●]   6  ← Pin 1 has dot/notch
  2       5
  3       4
```

**BT136 (TO-220)**:
```
Looking at flat face with legs down:
Left: MT1
Center: MT2 (to heatsink)
Right: Gate
```

---

## 🔧 Step 3: Physical Switches

Connect tactile switches to ESP32:

```
Switch 1: GPIO32 to GND (internal pull-up enabled)
Switch 2: GPIO33 to GND (internal pull-up enabled)
Switch 3: GPIO25 to GND (internal pull-up enabled)
Switch 4: GPIO26 to GND (internal pull-up enabled)
```

### Optional: Add debounce capacitors

```
Each switch pin: 0.1µF capacitor to GND
```

---

## 🔧 Step 4: Power Supply

### Using External 5V Supply

1. Connect 5V supply positive to ESP32 VIN (or 5V pin)
2. Connect 5V supply ground to ESP32 GND
3. **IMPORTANT**: Ensure power supply is isolated from AC mains

### Using On-Board Regulation

If using transformer:
```
12V AC → Bridge Rectifier → 7805 Regulator → 5V to ESP32
                           ↓
                    Capacitors (100µF input, 10µF output)
```

---

## 🔧 Step 5: Complete Wiring Diagram

### Low Voltage Side (ESP32)

```
ESP32 Pin Connections:
├── GPIO13 ← Zero-Cross Detection (from 4N25 pin 4)
├── GPIO16 → Channel 1 Optocoupler (MOC3021 pin 1)
├── GPIO17 → Channel 2 Optocoupler (MOC3021 pin 1)
├── GPIO18 → Channel 3 Optocoupler (MOC3021 pin 1)
├── GPIO19 → Channel 4 Optocoupler (MOC3021 pin 1)
├── GPIO32 ← Physical Switch 1
├── GPIO33 ← Physical Switch 2
├── GPIO25 ← Physical Switch 3
├── GPIO26 ← Physical Switch 4
├── 5V ← Power Supply (+)
└── GND → All grounds (common)
```

### High Voltage Side (AC)

```
AC Live Wire:
├── Fuse (10A) → Common connection
    ├── TRIAC1 MT1 → Load1 → Neutral
    ├── TRIAC2 MT1 → Load2 → Neutral
    ├── TRIAC3 MT1 → Load3 → Neutral
    └── TRIAC4 MT1 → Load4 → Neutral
```

---

## 🧪 Testing Procedure

### Step 1: Visual Inspection

- [ ] Check all solder joints
- [ ] Verify no shorts between high and low voltage
- [ ] Confirm proper wire gauge
- [ ] Check polarity of electrolytic capacitors
- [ ] Verify TRIAC orientation

### Step 2: Continuity Tests (Power OFF)

- [ ] Test ground continuity
- [ ] Verify isolation between AC and DC (should be infinite)
- [ ] Check switch continuity

### Step 3: Low Voltage Testing

1. Power ESP32 with 5V USB (no AC connected)
2. Flash test firmware
3. Verify GPIO outputs toggle
4. Check zero-cross detection with function generator (simulate AC)

### Step 4: AC Testing (CAREFUL!)

**Use isolation transformer and current limiter for first test**

1. Connect 12V AC to zero-cross circuit first
2. Verify zero-cross pulses on oscilloscope
3. Connect one channel with 40W incandescent bulb as load
4. Test on/off control from ESP32
5. Test dimming (for compatible loads)
6. Repeat for all channels

### Step 5: Integration Testing

- [ ] WiFi connectivity
- [ ] Cloud communication
- [ ] Physical switches working
- [ ] Voice control (Alexa/Google)
- [ ] Mobile app control

---

## 📐 PCB Layout Tips

### Isolation Requirements

- Maintain **8mm minimum** clearance between AC and DC traces
- Use **thick traces** for AC paths (minimum 2mm width)
- Add **isolation slots** in PCB for extra safety
- Place optocouplers at the **isolation barrier**

### Thermal Management

- Use **copper pour** under TRIACs for heatsinking
- Add **thermal vias** under power components
- Mount TRIACs on edge of PCB for external heatsink access
- Keep high-current traces short and wide

### Grounding

- Use **star ground** topology
- Separate analog and digital grounds
- Connect at single point near power supply

---

## 📦 Enclosure Design

### Requirements

- **Material**: Non-conductive (ABS plastic recommended)
- **Ventilation**: Slots for airflow (not large enough to touch components)
- **Mounting**: DIN rail or wall mount brackets
- **Access**: Removable cover with screws (not clips)
- **Cable Entry**: Strain relief glands
- **Labeling**: Clear warning labels for high voltage

### Dimensions

- Minimum internal space: 120mm × 80mm × 40mm
- Allow clearance for wire bending radius
- Consider future expansion

---

## 🔍 Troubleshooting

### Problem: Zero-cross not detected

- **Check**: Transformer connections and polarity
- **Check**: Bridge rectifier orientation
- **Check**: Optocoupler pins and resistor values
- **Test**: With oscilloscope at each stage

### Problem: TRIAC not switching

- **Check**: Gate resistor value (should be 100-180Ω)
- **Check**: Optocoupler orientation
- **Check**: ESP32 GPIO output voltage (should be 3.3V)
- **Test**: TRIAC with multimeter (should show near-zero resistance when triggered)

### Problem: Loads flickering

- **Check**: Zero-cross timing
- **Check**: TRIAC gate current
- **Add**: Snubber circuit across TRIAC
- **Check**: Load compatibility (some LEDs don't dim well)

### Problem: WiFi not connecting

- **Check**: 2.4GHz network availability
- **Check**: ESP32 power supply (needs >500mA peak)
- **Check**: Antenna connection
- **Reset**: WiFi credentials via physical switch

---

## 📸 Visual Reference

See **schematics/** folder for:
- Complete circuit diagram
- PCB layout
- 3D renders
- Assembly photos

---

## ✅ Final Checklist

Before permanent installation:

- [ ] All connections double-checked
- [ ] Proper fusing on AC input
- [ ] GFCI/RCD protection in place
- [ ] Enclosure properly sealed
- [ ] Warning labels affixed
- [ ] All tests passed
- [ ] Documentation completed
- [ ] Photos taken for reference

---

## 📞 Support

For connection issues:
- Review hardware README
- Check schematic diagrams
- Consult licensed electrician if unsure

---

**Safety Notice**: This guide is for educational purposes. Always follow local electrical codes and regulations. Have a licensed electrician verify your work before permanent installation.

---

**Version**: 1.0  
**Last Updated**: December 2024
