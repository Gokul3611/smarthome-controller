# Hardware Documentation

This folder contains all hardware-related documentation for the Smart Home Controller project.

## 📁 Contents

- **schematics/** - Circuit diagrams and schematics
- **pcb/** - PCB layouts and Gerber files
- **bom/** - Bill of Materials
- **README.md** - This file
- **CONNECTION_GUIDE.md** - Detailed connection instructions
- **SPECIFICATIONS.md** - Hardware specifications

---

## 🔌 Hardware Overview

The Smart Home Controller is built around the ESP32 microcontroller and controls 4 channels of AC loads using TRIAC-based dimmer circuits.

### Main Components

1. **ESP32-WROOM-32** - Main microcontroller
2. **TRIAC BT136** (x4) - AC switching elements
3. **MOC3021/3041** (x4) - Optocouplers for isolation
4. **Zero-Cross Detection Circuit** - AC waveform synchronization
5. **5V Power Supply** - Regulated power for ESP32
6. **Physical Switches** (x4) - Manual control buttons

---

## 📐 Circuit Architecture

```
AC Mains → Zero-Cross Detection → ESP32 GPIO13 (Interrupt)
                ↓
         Isolation Transformer
                ↓
         Rectifier & Regulator (5V)
                ↓
            ESP32 Power

AC Load 1-4 ← TRIAC 1-4 ← MOC3021 ← ESP32 GPIO 16-19
```

### Signal Flow

1. **Zero-Cross Detection**: Monitors AC waveform zero-crossing points
2. **ESP32 Processing**: Calculates firing angle for TRIAC
3. **Optocoupler Isolation**: Electrically isolates control from high voltage
4. **TRIAC Switching**: Controls AC power to loads

---

## 🎛️ Pin Configuration

### ESP32 GPIO Mapping

| Function | GPIO Pin | Direction | Notes |
|----------|----------|-----------|-------|
| Zero-Cross Detection | GPIO 13 | Input | Rising edge interrupt, pull-down |
| TRIAC Channel 1 | GPIO 16 | Output | Active HIGH, controls optocoupler |
| TRIAC Channel 2 | GPIO 17 | Output | Active HIGH, controls optocoupler |
| TRIAC Channel 3 | GPIO 18 | Output | Active HIGH, controls optocoupler |
| TRIAC Channel 4 | GPIO 19 | Output | Active HIGH, controls optocoupler |
| Physical Switch 1 | GPIO 32 | Input | Pull-up, active LOW |
| Physical Switch 2 | GPIO 33 | Input | Pull-up, active LOW |
| Physical Switch 3 | GPIO 25 | Input | Pull-up, active LOW |
| Physical Switch 4 | GPIO 26 | Input | Pull-up, active LOW |
| Status LED | GPIO 2 | Output | Built-in LED |

### Power Pins

| Pin | Connection | Notes |
|-----|------------|-------|
| 3V3 | 3.3V Output | For sensors (max 500mA) |
| 5V | 5V Input | From power supply |
| GND | Ground | Common ground |

---

## ⚠️ Safety Considerations

### Electrical Safety

1. **High Voltage Warning**: This circuit operates with mains AC voltage (110V/220V)
2. **Isolation Required**: Always use optocouplers for high voltage isolation
3. **Proper Grounding**: Ensure all metal parts are properly grounded
4. **Enclosure**: Use non-conductive, fire-resistant enclosure
5. **Fusing**: Include appropriate fuses (2A-10A depending on load)

### Design Safety Features

- ✅ Optocoupler isolation (2.5kV minimum)
- ✅ Zero-cross detection for smooth switching
- ✅ Watchdog timers in firmware
- ✅ Auto-shutoff after timeout
- ✅ Brownout detection
- ✅ Overcurrent protection (via fuses)

---

## 🔧 Component Specifications

### ESP32-WROOM-32

- **CPU**: Dual-core Xtensa LX6 @ 240MHz
- **RAM**: 520KB SRAM
- **Flash**: 4MB (minimum)
- **WiFi**: 802.11 b/g/n (2.4GHz)
- **Bluetooth**: BLE 4.2
- **GPIO**: 34 programmable pins
- **ADC**: 12-bit, 18 channels
- **Operating Voltage**: 3.3V (5V tolerant on some pins)
- **Operating Current**: 80mA (average), 500mA (peak)

### TRIAC BT136

- **Type**: Bidirectional thyristor
- **Voltage Rating**: 600V or 800V
- **Current Rating**: 4A RMS
- **Gate Trigger Current**: 5-50mA
- **Package**: TO-220
- **Operating Temperature**: -40°C to +125°C

### MOC3021/3041 Optocoupler

- **Type**: TRIAC driver optocoupler
- **Isolation Voltage**: 5000V RMS
- **Forward Current**: 10-30mA
- **Zero-Cross**: 3041 has built-in zero-cross detection
- **Package**: DIP-6
- **LED Forward Voltage**: 1.5V typical

### Zero-Cross Detection Circuit

- **Input**: AC mains (via step-down transformer)
- **Output**: Digital pulse at zero-crossing
- **Frequency**: 100/120 Hz (50/60Hz mains × 2)
- **Components**: Bridge rectifier, resistor divider, optocoupler

---

## 📊 Power Requirements

### ESP32 Power Budget

| Component | Typical | Peak | Notes |
|-----------|---------|------|-------|
| ESP32 (WiFi active) | 80mA | 500mA | During transmission |
| ESP32 (WiFi idle) | 20mA | 50mA | Background processes |
| Zero-cross circuit | 5mA | 10mA | Continuous |
| Optocouplers (per channel) | 10mA | 20mA | When active |
| Status LEDs | 20mA | 20mA | Optional |
| **Total (all channels ON)** | **165mA** | **630mA** | Use 1A supply |

**Recommended**: 5V 2A power supply (with margin)

### Load Capacity

Each TRIAC channel can handle:
- **Resistive loads**: Up to 4A (880W @ 220V, 440W @ 110V)
- **Inductive loads**: Derate to 2A (440W @ 220V, 220W @ 110V)
- **LED loads**: Use compatible TRIACs or add snubber circuit

---

## 🛠️ Assembly Instructions

### PCB Assembly Order

1. **Solder SMD components first** (if any)
2. **Low-profile components** (resistors, diodes, small capacitors)
3. **Optocouplers** (DIP-6 packages)
4. **Power supply components** (transformer, rectifier, regulator)
5. **TRIACs** (TO-220 packages with heatsinks)
6. **Connectors** (screw terminals, headers)
7. **ESP32 module** (last, to avoid heat damage)

### Testing Procedure

1. **Visual Inspection**: Check for shorts, cold solder joints
2. **Continuity Test**: Verify connections with multimeter
3. **Power Supply Test**: Measure 5V and 3.3V rails (NO AC connected)
4. **ESP32 Programming**: Flash test firmware via USB
5. **Zero-Cross Test**: Monitor GPIO13 with oscilloscope
6. **Low-Voltage TRIAC Test**: Test with 12V AC before mains
7. **Final Test**: Connect mains AC (with proper safety precautions)

---

## 📋 Bill of Materials (BOM)

See **bom/BOM.csv** for complete component list with:
- Part numbers
- Quantities
- Suppliers
- Approximate costs

---

## 🔌 Connection Guide

See **CONNECTION_GUIDE.md** for detailed step-by-step connection instructions with diagrams.

---

## 📐 PCB Design Files

Located in **pcb/** folder:
- PCB layout files (KiCad/Eagle format)
- Gerber files for manufacturing
- 3D models
- Assembly drawings

---

## 🧪 Testing & Validation

### Functional Tests

- [ ] Zero-cross detection working (100/120Hz pulses)
- [ ] All 4 TRIAC channels switching correctly
- [ ] Physical switches responding
- [ ] WiFi connectivity established
- [ ] Cloud communication functional
- [ ] Voice assistant discovery working
- [ ] OTA update successful

### Safety Tests

- [ ] Isolation resistance > 10MΩ
- [ ] Ground continuity < 1Ω
- [ ] No voltage on low-voltage side
- [ ] Proper fuse operation
- [ ] Thermal testing (1 hour full load)
- [ ] EMI/RFI within limits

---

## 📞 Support

For hardware questions:
- Check **CONNECTION_GUIDE.md**
- Review **SPECIFICATIONS.md**
- See main project README
- Open GitHub issue

---

## ⚖️ Compliance

This design follows:
- IEC 60950-1 (Safety)
- EN 55022 (EMI)
- FCC Part 15 (Radio)
- RoHS Compliance

**Note**: Professional electrical certification required before commercial use.

---

## 📜 License

Hardware design licensed under CERN Open Hardware License v2.0

---

**Version**: 1.0
**Last Updated**: December 2024
