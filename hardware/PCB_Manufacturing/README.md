# PCB Manufacturing Package
**Smart Home Controller Hardware Design v3.1**

## Technical Specification

### Board Parameters
| Parameter | Specification | Standard |
|-----------|--------------|----------|
| Dimensions | 70.0mm × 50.0mm ±0.2mm | IPC-2221 |
| Layer Count | 2 (signal + ground) | - |
| Board Thickness | 1.60mm ±0.16mm | IPC-4101 |
| Copper Weight | 2oz (70μm) both sides | IPC-4562 |
| Surface Finish | HASL lead-free or ENIG | IPC-4552/4556 |
| Soldermask | LPI, green, both sides | IPC-SM-840 |
| Minimum Track Width | 0.25mm (10mil) | IPC Class 2 |
| Minimum Clearance | 0.25mm (10mil) | IPC Class 2 |

### Package Contents

The manufacturing package contains industry-standard files in accordance with IPC-D-356A and Gerber X2 format specifications:

```
PCB_Manufacturing/
├── Gerber_Files/
│   ├── TopCopper.gtl           RS-274X format, layer L1
│   ├── BottomCopper.gbl        RS-274X format, layer L2
│   ├── TopSolderMask.gts       Negative image, solder stop
│   ├── BottomSolderMask.gbs    Negative image, solder stop
│   ├── TopSilkscreen.gto       Component designators, warnings
│   ├── BottomSilkscreen.gbo    Reference marks
│   └── BoardOutline.gko        Mechanical profile, routing path
├── DrillFile.drl               Excellon format, NC drill data
├── PickAndPlace.csv            Centroid data for assembly
├── BOM.csv                     Component procurement data
└── PCB_Stackup.txt             Material and layer specification
```

## Manufacturing Requirements

### Base Material Specification
- **Substrate:** FR-4, Tg 130-140°C minimum, IEC 60249-2-2
- **Flammability Rating:** UL94 V-0
- **CTI Value:** ≥175V (Material Group IIIa per IEC 60112)
- **Dielectric Constant:** εr = 4.5 ±0.1 @ 1MHz
- **Copper Foil:** Electro-deposited, 2oz (70μm), both sides

### Design Constraints (IPC Class 2)
1. **Electrical Isolation:** 6.0mm clearance between AC (250V) and DC (5V) sections per IEC 60950-1
2. **Current Capacity:** AC mains traces 2.0mm width (10A @ 10°C rise), TRIAC outputs 1.5mm (4A)
3. **Thermal Management:** Multiple 0.3mm thermal vias (0.6mm pad) under power components
4. **Via Specification:** Plated through-hole, 0.3mm minimum drill, aspect ratio <8:1
5. **Annular Ring:** 0.15mm minimum (Class 2)
6. **Mounting:** 4× M2.5 clearance holes, 2.7mm diameter, 5mm edge distance

### Assembly Process (IPC-A-610 Class 2)
1. **Solder Paste Application:** 0.125mm stencil, SAC305 lead-free solder paste
2. **Component Placement:** Automated pick-and-place per centroid data (PickAndPlace.csv)
3. **Reflow Profile:** IPC/JEDEC J-STD-020, peak 250°C, 60-90s above liquidus
4. **Through-Hole Assembly:** ESP32 module, TRIACs (TO-220), terminal blocks
5. **Inspection:** AOI per IPC-A-610 workmanship standards
6. **Testing:** ICT (In-Circuit Test) or flying probe, functional validation

### Fabrication Partners
Qualified manufacturers with IPC-6012 Class 2 certification:
- JLCPCB (Shenzhen, China) - ISO 9001, UL
- PCBWay (Shenzhen, China) - ISO 9001, RoHS
- Eurocircuits (Belgium) - ISO 9001, IPC-6012
- Advanced Circuits (USA) - IPC-6012 Class 3, AS9100

### Production Economics
| Quantity | Unit Cost | Lead Time | Application |
|----------|-----------|-----------|-------------|
| 5-10 | $5-8 | 5-7 days | Prototype validation |
| 50-100 | $3-4 | 7-10 days | Pilot production |
| 500+ | $2-3 | 10-15 days | Volume manufacturing |

## File Format Standards

### Gerber X2 (RS-274X Extended)
Photoplotter files conforming to Ucamco Gerber Format Specification, revision 2023.03:

| Extension | Layer Function | IPC-2581 Equivalent |
|-----------|----------------|---------------------|
| .gtl | Conductive pattern, L1 (top) | LAYER_FEATURE_CONDUCTOR |
| .gbl | Conductive pattern, L2 (bottom) | LAYER_FEATURE_CONDUCTOR |
| .gts | Solder resist, top | LAYER_FEATURE_SOLDERMASK |
| .gbs | Solder resist, bottom | LAYER_FEATURE_SOLDERMASK |
| .gto | Legend, top | LAYER_FEATURE_SILKSCREEN |
| .gbo | Legend, bottom | LAYER_FEATURE_SILKSCREEN |
| .gko | Profile, routing path | BOARD_OUTLINE |

### NC Drill Data (Excellon Format)
- **Format:** Excellon CNC-7 format, 2.4 coordinate precision
- **Units:** Metric (mm)
- **Content:** PTH (plated through-hole) and NPTH (non-plated) coordinate data
- **Tool definitions:** T-codes with diameter specifications

### Assembly Data
- **Centroid File:** IPC-D-356A compliant, XY coordinates relative to board origin
- **BOM:** IPC-2581 component list with manufacturer part numbers

## Quality Assurance

### Design Validation (Pre-Fabrication)
Verification against IPC-2221 and IPC-2222 design standards:

| Check Item | Requirement | Verification Method |
|------------|-------------|---------------------|
| Isolation barriers | 6.0mm AC-DC clearance | DRC, visual inspection |
| Current capacity | 2.0mm @ 10A, 1.5mm @ 4A | Thermal analysis per IPC-2152 |
| Drill specifications | 0.3-2.7mm range | NC drill file audit |
| Annular ring | ≥0.15mm Class 2 | DRC per IPC-6012 |
| Soldermask registration | ±0.10mm tolerance | Manufacturing capability |
| Silkscreen legibility | ≥1.0mm character height | Visual standards |

### CAM File Validation
Pre-submission verification using industry-standard tools:

1. **Gerber Verification:** GerbView (KiCad), CAM350, or ViewMate
2. **DRC Analysis:** Minimum track/space 0.25mm, copper-to-edge ≥0.3mm
3. **NC Drill Audit:** Tool count, hole-to-hole spacing, aspect ratio
4. **Netlist Comparison:** IPC-D-356 netlist cross-check (if available)

## Assembly Options

### Turnkey Assembly
Full PCBA service with component procurement and assembly per IPC-A-610 Class 2:
- **Process:** Automated SMT + manual THT assembly
- **Delivery:** Fully assembled, tested boards
- **Lead Time:** 15-20 business days
- **Cost:** Highest per-unit cost, suitable for ≥50 units

### Consignment Assembly
Partial assembly with customer-supplied components:
- **SMT Assembly:** Machine placement and reflow per IPC/JEDEC J-STD-001
- **THT Components:** Customer-supplied for hand assembly
- **Suitable for:** Prototype quantities (5-25 units)
- **Cost:** Moderate, requires component inventory management

### Bare Board Procurement
PCB fabrication only, customer assembly:
- **Delivery:** Bare printed circuit boards, no components
- **Assembly:** Customer responsibility, requires SMT capability
- **Cost:** Minimum per-unit cost
- **Application:** Engineering evaluation, very low volume

## Post-Assembly Validation

### Visual Inspection (IPC-A-610 Class 2)
Workmanship criteria per Section 5 (Soldering) and Section 10 (Assembly):
- Solder joint acceptability: heel fillet, toe fillet, side fillet formation
- Component placement: ±0.5mm positional tolerance
- Soldermask integrity: no damage, complete coverage
- Legend legibility: component designators readable

### Electrical Characterization
| Parameter | Test Method | Acceptance Criteria |
|-----------|-------------|---------------------|
| Ground continuity | 4-wire Kelvin | <1.0Ω between any points |
| AC-DC isolation | Megohm meter, 500V DC | >10MΩ minimum |
| Power rail voltage | Precision DVM | 5.00V ±0.05V |
| GPIO logic levels | Logic analyzer | HIGH: 3.0-3.6V, LOW: 0-0.4V |
| Zero-cross frequency | Oscilloscope | 100Hz ±1% (50Hz mains) |

### Functional Validation
System-level testing per design specification:
- Microcontroller programming and boot sequence
- Network connectivity (WiFi, WebSocket, REST API)
- Cloud service integration
- Physical interface operation (switches)
- TRIAC control and load switching

## Regulatory Compliance

### Safety Standards
| Standard | Scope | Compliance Notes |
|----------|-------|------------------|
| IEC 60950-1 | IT equipment safety | 6mm clearance AC-DC exceeds requirement |
| UL 94 V-0 | Flammability | FR-4 base material certified |
| IEC 60112 | Tracking resistance | CTI ≥175V (Material Group IIIa) |

### Electromagnetic Compatibility
- **Emissions:** EN 55022 Class B, FCC Part 15 Class B
- **Immunity:** IEC 61000-4 series
- **Design measures:** Ground plane, zero-cross switching, capacitive filtering

### Environmental Regulations
- **RoHS:** Directive 2011/65/EU (lead-free assembly)
- **REACH:** Compliance with substance restrictions
- **WEEE:** Design for recyclability

## Manufacturing Defect Analysis

| Defect Mode | Root Cause | Corrective Action |
|-------------|------------|-------------------|
| Solder bridging | Excessive paste volume | Reduce stencil aperture by 10% |
| Component tombstoning | Thermal imbalance | Equal pad areas, slow ramp rate |
| Voiding in thermal vias | Outgassing during reflow | Via plugging or capping |
| Pad non-wetting | Surface contamination | Pre-reflow bake, storage controls |

## Technical Support

### Documentation References
- Circuit schematics: `hardware/schematics/`
- Test procedures: `TESTING_VALIDATION.md`
- Reliability analysis: `FAILSAFE_DESIGN.md`

### Design Queries
Submit technical questions via GitHub Issues tracker with tag `hardware`.

## Revision Control

| Revision | Date | ECN | Description |
|----------|------|-----|-------------|
| 3.1 | 2024-12 | - | Production release: compact design, 70×50mm |

**Document Number:** HW-MFG-PCB-001  
**Issue Date:** December 2024  
**Prepared by:** Hardware Engineering  
**Approved by:** [Signature block]

---

This manufacturing package conforms to IPC-2581 data transfer standard and contains all information necessary for PCB fabrication and assembly per industry specifications.
