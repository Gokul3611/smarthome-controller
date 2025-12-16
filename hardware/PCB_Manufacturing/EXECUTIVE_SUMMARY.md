# Smart Home Controller - PCB Manufacturing Executive Summary

## Project Overview

**Project Name**: Smart Home Controller  
**Version**: 1.0  
**PCB Revision**: Rev A  
**Design Tool**: KiCad 6.0  
**Manufacturer**: JLCPCB  
**Date**: December 2024

---

## Circuit Specifications

### Main Controller
- **Microcontroller**: ESP32-WROOM-32
- **Operating Voltage**: 3.3V logic, 5V power input
- **WiFi**: 802.11 b/g/n (2.4GHz)
- **Bluetooth**: BLE 4.2

### GPIO Pin Assignments

| Function | GPIO Pin | Direction | Notes |
|----------|----------|-----------|-------|
| Zero-Cross Detection | GPIO 13 | Input | Rising edge interrupt, pull-down 10kΩ |
| TRIAC Channel 1 | GPIO 16 | Output | Active HIGH, drives MOC3021 via 330Ω |
| TRIAC Channel 2 | GPIO 17 | Output | Active HIGH, drives MOC3021 via 330Ω |
| TRIAC Channel 3 | GPIO 18 | Output | Active HIGH, drives MOC3021 via 330Ω |
| TRIAC Channel 4 | GPIO 19 | Output | Active HIGH, drives MOC3021 via 330Ω |
| Physical Switch 1 | GPIO 32 | Input | Pull-up, active LOW, debounced |
| Physical Switch 2 | GPIO 33 | Input | Pull-up, active LOW, debounced |
| Physical Switch 3 | GPIO 25 | Input | Pull-up, active LOW, debounced |
| Physical Switch 4 | GPIO 26 | Input | Pull-up, active LOW, debounced |
| Status LED | GPIO 2 | Output | Built-in LED |

### TRIAC Driver Circuits (4 Channels)

Each channel consists of:
1. **Optocoupler**: MOC3021 (random phase) or MOC3041 (zero-cross)
   - Input: 330Ω current limiting resistor from ESP32 GPIO
   - Isolation: 5000V RMS
   
2. **TRIAC**: BT136-600E
   - Voltage Rating: 600V
   - Current Rating: 4A RMS
   - Package: TO-220 with heatsink
   - Gate Resistor: 100Ω (1/2W)
   - Snubber Circuit: 0.1µF/400V film capacitor + 100kΩ resistor (optional)

3. **Thermal Management**:
   - Heatsinks on all TRIACs (TO-220 compatible)
   - Thermal vias under TRIAC mounting pads (0.5mm diameter, filled)
   - Minimum 9 thermal vias per TRIAC in 3x3 grid pattern
   - Thermal pad connection to bottom copper pour

### Zero-Cross Detection Circuit

- **Input**: AC mains via step-down transformer (230V to 12V)
- **Rectifier**: Bridge rectifier using 4x 1N4007 diodes
- **Optocoupler**: 4N25 or PC817
- **Current Limiting**: 1kΩ resistor (1/2W) on AC side
- **Pull-down**: 10kΩ resistor on ESP32 side
- **Output**: Digital pulse at each zero-crossing (100Hz for 50Hz mains, 120Hz for 60Hz mains)
- **Protection**: 0.1µF ceramic capacitor for noise filtering

### Power Supply Circuit

- **Input**: AC mains via isolation transformer (230V/110V to 12V AC, 2VA minimum)
- **Rectifier**: Full-wave bridge rectifier (1N4007 x4)
- **Filter**: 1000µF/16V electrolytic capacitor
- **Regulator**: AMS1117-5.0 (5V, 1A)
- **Output Filter**: 100µF/16V + 10µF/16V electrolytic capacitors
- **Bypass**: 0.1µF ceramic capacitors at ESP32 power pins
- **Protection**: Reverse polarity protection diode, input fuse (2A)

---

## PCB Design Requirements

### Board Specifications

- **Dimensions**: 100mm x 80mm
- **Layers**: 2-layer PCB
- **Copper Weight**: 2oz (70µm) on both layers for high current handling
- **Board Thickness**: 1.6mm (standard)
- **Surface Finish**: HASL (Hot Air Solder Leveling) or ENIG
- **Solder Mask**: Green (standard) or other color
- **Silkscreen**: White text on both sides

### Critical Design Rules

#### AC/DC Isolation Barrier
- **Location**: Y = 26mm from bottom edge
- **Barrier Width**: 6mm minimum clearance zone
- **No Traces**: Absolutely no copper traces crossing the barrier
- **No Vias**: No vias within the isolation barrier zone
- **Keepout**: Defined keepout area in PCB layout
- **Creepage**: Minimum 6mm creepage distance between AC and DC sides
- **Clearance**: Minimum 6mm clearance in air

#### Copper Clearances
- **High Voltage AC traces**: 0.8mm minimum spacing
- **Low Voltage DC traces**: 0.2mm minimum spacing
- **AC to DC clearance**: 6mm minimum (enforced by isolation barrier)
- **Trace Width (AC)**: 1.5mm minimum for 4A loads
- **Trace Width (DC, 5V)**: 0.8mm for power, 0.3mm for signals

#### Thermal Management
- **TRIAC Thermal Vias**: 
  - 9 vias per TRIAC in 3x3 grid pattern
  - Via diameter: 0.5mm drill, 0.8mm pad
  - Via spacing: 2mm center-to-center
  - Connected to bottom layer ground/heat sink plane
  - Filled or tented vias recommended

- **Copper Pours**:
  - Bottom layer: Ground plane with thermal relief for TRIACs
  - Top layer: Ground pour in non-AC areas
  - Thermal connection from TRIAC tab to ground plane

### Component Placement

#### High Voltage Side (Below Y=26mm barrier)
- AC input screw terminals
- Transformer
- Bridge rectifier
- TRIACs (BT136) with heatsinks
- MOC3021 optocouplers (straddling barrier)
- AC load output screw terminals
- Fuses and protection components

#### Low Voltage Side (Above Y=26mm barrier)
- ESP32-WROOM-32 module
- 5V voltage regulator (AMS1117)
- Zero-cross detection optocoupler output
- Physical switches
- Status LEDs
- Bypass capacitors
- Programming header (USB/UART)

#### Optocoupler Placement
- MOC3021 optocouplers should straddle the isolation barrier
- Input pins (1-2) on low voltage side
- Output pins (4-6) on high voltage side
- Package oriented perpendicular to barrier

### Net Connections

#### Power Nets
- **AC_LIVE**: Mains live input
- **AC_NEUTRAL**: Mains neutral input
- **AC_L1, AC_L2, AC_L3, AC_L4**: TRIAC switched outputs to loads
- **+5V**: Regulated 5V power rail
- **+3V3**: 3.3V from ESP32 regulator
- **GND**: Common ground (DC side only, isolated from AC)

#### Signal Nets
- **ZCD**: Zero-cross detection signal to GPIO13
- **TRIAC1**: GPIO16 to MOC3021 channel 1
- **TRIAC2**: GPIO17 to MOC3021 channel 2
- **TRIAC3**: GPIO18 to MOC3021 channel 3
- **TRIAC4**: GPIO19 to MOC3021 channel 4
- **SW1**: Physical switch 1 to GPIO32
- **SW2**: Physical switch 2 to GPIO33
- **SW3**: Physical switch 3 to GPIO25
- **SW4**: Physical switch 4 to GPIO26

---

## Manufacturing Specifications (JLCPCB)

### PCB Fabrication Parameters

```
Board Type: Single PCB
Layers: 2
Dimensions: 100mm x 80mm
PCB Qty: 5 (minimum order)
PCB Thickness: 1.6mm
PCB Color: Green
Silkscreen: White
Surface Finish: HASL (Lead Free)
Outer Copper Weight: 2oz
Via Covering: Tented
Via Process: Tented vias (recommended for thermal vias)
Plating Thickness: 1oz (25µm) minimum on via walls
Gold Fingers: No
Castellated Holes: No
Material: FR-4 TG150
Dielectric Constant: 4.5 @ 1MHz
Remove Order Number: Yes (specify location on back silkscreen)
Controlled Impedance: No (not required for this design)
```

### Stackup Configuration

```
Layer 1 (Top):    0.07mm copper (2oz) - F.Cu - Signal/Power layer
Dielectric:       1.46mm FR-4 core (εr = 4.5)
Layer 2 (Bottom): 0.07mm copper (2oz) - B.Cu - Ground plane
```

### Via Specifications for Thermal Management

- **Thermal Via Fill**: Tented (covered with solder mask)
- **Via Plating**: Standard through-hole plating (1oz min)
- **Thermal Via Pattern**: 3×3 grid under each TRIAC
- **Via Spacing**: 2mm center-to-center
- **Via Function**: Heat transfer from TRIAC to bottom ground plane

### Gerber Files Required

1. **GTL** - Top copper layer
2. **GBL** - Bottom copper layer
3. **GTS** - Top solder mask
4. **GBS** - Bottom solder mask
5. **GTO** - Top silkscreen
6. **GBO** - Bottom silkscreen
7. **TXT** - Drill file (Excellon format)
8. **GML/GKO** - Board outline

### Design Rule Constraints (JLCPCB Standard)

- **Minimum Track Width**: 0.127mm (use 0.3mm minimum for this design)
- **Minimum Track Spacing**: 0.127mm (use 0.8mm for AC, 0.2mm for DC)
- **Minimum Via Hole Size**: 0.3mm (use 0.5mm)
- **Minimum Via Diameter**: 0.5mm (use 0.8mm)
- **PTH Drill Sizes**: 0.3mm - 6.3mm
- **NPTH Drill Sizes**: 0.5mm - 6.3mm
- **Board Outline Tolerance**: ±0.2mm

### Assembly (If Using JLCPCB Assembly Service)

- Basic assembly available for common SMD components
- Through-hole components (TRIACs, transformers) require manual assembly
- Extended parts library available at additional cost
- BOM and Pick-and-Place (CPL) files required

---

## Safety & Compliance

### Electrical Safety

1. **Isolation Requirements**:
   - Minimum 6mm AC/DC isolation barrier enforced
   - Optocoupler isolation rating: 5000V RMS minimum
   - No direct electrical connection between AC and DC sides

2. **Protection Features**:
   - Input fuse on AC side (2A slow-blow)
   - Reverse polarity protection on DC input
   - ESD protection on exposed connectors
   - Thermal shutdown in firmware

3. **Grounding**:
   - AC ground terminal provided
   - Metal enclosure must be grounded
   - DC ground isolated from AC ground

### Regulatory Compliance

- **Target Standards**:
  - IEC 60950-1: Safety of IT equipment
  - EN 55022: EMI emissions
  - FCC Part 15: Radio frequency devices
  - RoHS: Restriction of hazardous substances

- **Testing Required**:
  - Hipot testing (isolation voltage)
  - Ground bond testing
  - EMI/RFI emissions testing
  - Safety agency approval (UL/CE) for commercial use

### Warnings

⚠️ **HIGH VOLTAGE WARNING**: This device operates with mains AC voltage (110-240V) which can cause serious injury or death. Assembly and installation should only be performed by qualified personnel familiar with high voltage safety procedures.

⚠️ **ISOLATION CRITICAL**: The 6mm isolation barrier is a critical safety feature. Do not modify or bypass this barrier. Any design changes must maintain minimum isolation requirements.

---

## Testing & Quality Control

### Pre-Power Testing
1. Visual inspection of PCB assembly
2. Continuity testing of all connections
3. Isolation resistance testing (AC to DC >10MΩ)
4. Short circuit check on power rails
5. Verify correct component orientation (especially diodes, ICs)

### Low-Voltage Testing
1. Apply 5V power (no AC connected)
2. Verify 3.3V regulator output
3. Program ESP32 with test firmware
4. Test GPIO inputs/outputs
5. Verify WiFi connectivity

### High-Voltage Testing (With Safety Precautions)
1. Connect low-voltage AC (12V) to test TRIACs
2. Verify zero-cross detection timing
3. Test TRIAC switching at various duty cycles
4. Monitor TRIAC temperatures under load
5. Test with full mains voltage (final step)

### Production Testing
- [ ] PCB visual inspection (IPC-A-610 standards)
- [ ] Automated optical inspection (AOI)
- [ ] In-circuit testing (ICT) if available
- [ ] Functional testing with test fixture
- [ ] Burn-in testing (24 hours at full load)
- [ ] Final QC inspection and packaging

---

## Bill of Materials Reference

See `BOM.csv` for complete component list including:
- Part numbers
- Manufacturer part numbers (MPN)
- Quantities
- Reference designators
- Footprints
- Supplier information
- Pricing

---

## Revision History

| Version | Date | Changes | Author |
|---------|------|---------|--------|
| 1.0 | Dec 2024 | Initial release for production | Engineering Team |

---

## Contact & Support

**Project Repository**: [GitHub Repository URL]  
**Technical Support**: Open GitHub issue with "Hardware:" prefix  
**Safety Questions**: Consult licensed electrician  

---

**Document Status**: Production Ready  
**Approved By**: Engineering Team  
**Approval Date**: December 2024
