# Circuit Design Documentation
**Smart Home Controller v3.1 - Hardware Schematics**

## Schematic Set

This directory contains circuit design documentation in ASCII format for version control and review purposes.

| Document | Content | Standard |
|----------|---------|----------|
| `main_schematic.txt` | Complete circuit topology | IEEE 315 symbols |
| `compact_snubberless_schematic.txt` | Optimized 70×50mm layout | IPC-2221 guidelines |
| `block_diagram.txt` | System architecture | IEEE 991 functional blocks |

## Design Methodology

### Circuit Analysis
Design validated using:
- **SPICE simulation:** Zero-cross detection timing
- **Thermal analysis:** IPC-2152 current capacity calculations
- **Safety analysis:** Isolation barriers per IEC 60950-1

### CAD Tools
Professional EDA software recommended for production:
- KiCad (open-source, IPC-2581 export)
- Altium Designer (enterprise-grade)
- EAGLE (Autodesk, hobbyist-friendly)

## Safety-Critical Features

### Isolation Architecture
- **Primary isolation:** Optocoupler (5kV dielectric)
- **Secondary isolation:** 6mm creepage distance
- **Reinforced isolation:** Exceeds IEC 60950-1 requirements

### Protection Circuitry
- Fusing: Three-level overcurrent protection
- Surge suppression: MOV transient voltage clamping
- Zero-cross switching: EMI reduction, component longevity

---

**Document Control:** HW-SCH-001  
**Revision:** 3.1  
**Classification:** Technical Reference
