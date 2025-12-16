# Hardware Design Specification
**Smart Home Controller v3.1**

## Document Information
- **Document Number:** HW-SPEC-001
- **Revision:** 3.1
- **Classification:** Technical Specification
- **Date:** December 2024

## 1. Overview

This document describes the hardware design for a four-channel AC load controller based on the ESP32 microcontroller platform. The design emphasizes safety, compactness, and manufacturability for residential installation.

## 2. Design Requirements

### 2.1 Functional Requirements
- Control four independent AC loads (110-240V, 50/60Hz)
- Per-channel current capacity: 4A continuous (resistive load)
- Galvanic isolation between AC and DC circuits
- Physical switch input interface (4 channels)
- WiFi connectivity via ESP32 module

### 2.2 Safety Requirements
- Reinforced isolation per IEC 60950-1
- Overcurrent protection (fusible links)
- Surge protection (metal oxide varistor)
- Thermal management for power components
- UL94 V-0 flammability rating (PCB material)

### 2.3 Physical Requirements
- Form factor: Compatible with standard 2-gang electrical box
- Mounting: DIN rail or direct screw mount
- Operating temperature: -20°C to +70°C
- Storage temperature: -40°C to +85°C

## 3. System Architecture

### 3.1 Block Diagram
See `schematics/block_diagram.txt` for detailed architecture.

### 3.2 Major Subsystems
1. **Power supply:** Isolated AC-DC converter (HLK-PM01)
2. **Control unit:** ESP32-WROOM-32 microcontroller
3. **Load switching:** TRIAC-based AC switches (4 channels)
4. **Isolation:** Optocoupler-based gate drive
5. **Zero-cross detection:** Phase-synchronized switching

## 4. Circuit Design

### 4.1 Schematics
Complete circuit diagrams located in `schematics/` directory:
- `main_schematic.txt` - Full system schematic
- `compact_snubberless_schematic.txt` - Optimized layout design

### 4.2 Key Design Features
- Zero-cross switching using MOC3041 optocouplers
- Dual-resistor voltage divider for zero-cross detection
- Thermal vias under power components
- 6mm creepage distance (AC to DC isolation)

## 5. PCB Design

### 5.1 Specifications
- **Dimensions:** 70mm × 50mm
- **Layers:** 2 (signal + ground plane)
- **Material:** FR-4, Tg 130-140°C
- **Copper weight:** 2oz (70μm) both sides
- **Surface finish:** HASL lead-free or ENIG

### 5.2 Manufacturing Files
Complete Gerber, drill, and assembly files in `PCB_Manufacturing/` directory.

### 5.3 Design Standards
- IPC-2221: Generic PCB design standard
- IPC-2222: Rigid organic PCB standard
- IPC-6012 Class 2: Fabrication specification

## 6. Bill of Materials

See `bom/BOM.md` for complete component list with:
- Part numbers and manufacturers
- Quantities and unit costs
- Procurement sources
- Assembly notes

## 7. Assembly and Testing

### 7.1 Assembly Process
Refer to `PCB_Manufacturing/TESTING_VALIDATION.md` for:
- Assembly procedures (IPC-A-610 compliance)
- Inspection criteria
- Electrical testing protocols

### 7.2 Quality Assurance
- Visual inspection per IPC-A-610
- Isolation testing (500V megohm meter)
- Functional validation
- Thermal characterization

## 8. Installation Guidelines

### 8.1 Safety Precautions
⚠️ **WARNING:** This device operates at hazardous AC voltage levels. Installation must be performed by qualified personnel in accordance with local electrical codes.

### 8.2 Connection Guide
See `CONNECTION_GUIDE.md` for detailed wiring instructions.

## 9. Compliance and Certification

### 9.1 Safety Standards
- IEC 60950-1: IT equipment safety
- UL 94 V-0: Flammability rating

### 9.2 EMC Standards (Target)
- EN 55022 Class B: Emissions
- FCC Part 15 Class B: RF emissions

### 9.3 Environmental
- RoHS Directive 2011/65/EU
- REACH Regulation (EC) 1907/2006

## 10. Revision History

| Revision | Date | Description |
|----------|------|-------------|
| 3.1 | Dec 2024 | Compact design (70×50mm), snubberless topology |
| 3.0 | - | Initial production design |

## References

1. IEC 60950-1:2005, Information Technology Equipment - Safety
2. IPC-2221B, Generic Standard on Printed Board Design  
3. ESP32 Datasheet, Espressif Systems
4. MOC3041 Datasheet, ON Semiconductor

---

**Document Control:** HW-SPEC-001 Rev 3.1  
**Classification:** Technical Documentation  
**Distribution:** Engineering, Manufacturing
