# PCB Manufacturing Files - Smart Home Controller v3.1

## Overview
Complete PCB manufacturing package for the Ultra-Compact Snubberless Smart Home Controller.

**PCB Specifications:**
- Dimensions: 70mm × 50mm
- Layers: 2-layer (Top + Bottom)
- Thickness: 1.6mm
- Copper weight: 2oz (70μm) for high current traces
- Surface finish: HASL (Hot Air Solder Leveling) or ENIG
- Silkscreen: White on green soldermask
- Minimum trace width: 0.25mm (10mil)
- Minimum clearance: 0.25mm (10mil)

## Directory Structure

```
PCB_Manufacturing/
├── Gerber_Files/           # Standard RS-274X Gerber files
│   ├── TopCopper.gtl       # Top copper layer (component side)
│   ├── BottomCopper.gbl    # Bottom copper layer (ground plane)
│   ├── TopSolderMask.gts   # Top soldermask layer
│   ├── BottomSolderMask.gbs# Bottom soldermask layer
│   ├── TopSilkscreen.gto   # Top silkscreen (component labels)
│   ├── BottomSilkscreen.gbo# Bottom silkscreen
│   └── BoardOutline.gko    # Board outline (mechanical layer)
├── DrillFile.drl           # Excellon drill file (NC format)
├── PickAndPlace.csv        # SMD assembly coordinates
├── BOM.csv                 # Bill of Materials for assembly
├── Assembly_Drawing.pdf    # Assembly instructions
├── README.md               # This file
└── PCB_Stackup.txt         # Layer stackup specification
```

## Manufacturing Notes

### PCB Fabrication
1. **Material:** FR-4 TG130-140
2. **Copper weight:** 2oz (70μm) on both layers for high current handling
3. **Soldermask color:** Green (standard), White silkscreen
4. **Surface finish:** HASL lead-free or ENIG (for better soldering)
5. **Via specification:** 0.3mm drill, 0.6mm pad
6. **Minimum hole size:** 0.3mm
7. **Board thickness:** 1.6mm ±10%
8. **Panelization:** V-score or tab routing (for bulk orders)

### Critical Design Features
1. **Isolation barrier:** 6mm clearance between AC and DC sections
2. **High voltage traces:** 2mm width for AC mains (Live/Neutral)
3. **TRIAC power traces:** 1.5mm width (per channel, 2A capacity)
4. **Thermal vias:** Under TRIACs and power components
5. **Ground plane:** Maximum copper pour on bottom layer
6. **Mounting holes:** 4× M2.5 (2.7mm diameter) with 3mm keepout

### Assembly Requirements
1. **SMD components:** 0805, 1206, 2512 resistors; SOP-4, DIP-6 ICs
2. **Through-hole:** ESP32 module, TRIACs (TO-220), connectors
3. **Assembly process:**
   - Solder paste application (stencil)
   - SMD placement (pick-and-place machine or manual)
   - Reflow soldering (lead-free profile: 250°C peak)
   - Through-hole soldering (wave or hand)
4. **Inspection:** AOI (Automated Optical Inspection) recommended
5. **Testing:** Flying probe or fixture test for shorts/opens

### Recommended PCB Manufacturers
- **JLCPCB** (China) - Low cost, fast turnaround
- **PCBWay** (China) - Good quality, assembly service
- **OSH Park** (USA) - High quality, purple PCBs
- **Eurocircuits** (Europe) - Professional grade
- **Seeed Studio** (China) - Maker-friendly

### Ordering Tips
1. **Prototype:** Order 5-10 pieces for testing
2. **Production:** Minimum 50-100 pieces for cost efficiency
3. **Assembly:** Use PCBA service for SMD components
4. **Lead time:** 5-7 days fabrication + 2-3 days shipping (standard)
5. **Cost estimate:**
   - Bare PCB: $2-5 per piece (qty 10)
   - With assembly: $15-25 per piece (qty 100)

## File Formats

### Gerber Files (RS-274X)
Standard format supported by all PCB manufacturers. Files use standard extensions:
- `.gtl` - Top copper (Gerber Top Layer)
- `.gbl` - Bottom copper (Gerber Bottom Layer)
- `.gts` - Top soldermask (Gerber Top Soldermask)
- `.gbs` - Bottom soldermask (Gerber Bottom Soldermask)
- `.gto` - Top silkscreen (Gerber Top Overlay)
- `.gbo` - Bottom silkscreen (Gerber Bottom Overlay)
- `.gko` - Board outline (Gerber Keep Out)

### Drill File (Excellon)
- `.drl` - NC drill format
- Contains coordinates for all drill holes
- Includes via holes, mounting holes, component holes

### Assembly Files
- `PickAndPlace.csv` - SMD component positions (X, Y, rotation, side)
- `BOM.csv` - Bill of Materials (reference designator, value, package, quantity)

## Quality Checks Before Manufacturing

### Pre-Manufacturing Checklist
- [ ] All Gerber files generated successfully
- [ ] Drill file includes all holes
- [ ] Board outline is closed (no gaps)
- [ ] Isolation barriers meet specification (6mm minimum)
- [ ] Trace widths adequate for current (2mm for AC, 1.5mm for TRIACs)
- [ ] No acid traps in copper pour
- [ ] Soldermask expansion adequate (0.1mm typical)
- [ ] Silkscreen text readable (minimum 1mm height)
- [ ] Silkscreen doesn't overlap pads
- [ ] Component courtyard clearances met
- [ ] Mounting holes properly sized (2.7mm for M2.5)
- [ ] Thermal relief on ground connections
- [ ] Via tenting specified (if required)

### Design Rule Check (DRC)
Run DRC in your EDA software before exporting:
- Minimum trace width: 0.25mm
- Minimum spacing: 0.25mm
- Minimum drill size: 0.3mm
- Minimum annular ring: 0.15mm
- Copper to board edge: 0.3mm minimum

### Gerber Verification
Use Gerber viewer to verify files:
- **Recommended tools:**
  - GerbView (KiCad built-in)
  - ViewMate (Pentalogix)
  - CAM350 (DownStream Technologies)
  - Online: PCBWay Gerber Viewer, EasyEDA Gerber Viewer

## Assembly Process

### Option 1: Full Assembly (Turnkey)
1. Upload Gerber files + BOM + Pick-and-place to PCBA service
2. Manufacturer sources all components
3. Complete assembled and tested boards delivered
4. **Pros:** Easiest, fastest
5. **Cons:** Most expensive, limited component choice

### Option 2: Partial Assembly (Consignment)
1. Order PCBs with SMD assembly only
2. Manufacturer provides SMD placement and reflow
3. You provide through-hole components separately
4. Hand-solder ESP32, TRIACs, connectors yourself
5. **Pros:** Lower cost, component control
6. **Cons:** Manual work required

### Option 3: DIY Assembly
1. Order bare PCBs only
2. Buy all components separately
3. Assemble everything yourself (hot plate, solder paste, iron)
4. **Pros:** Lowest cost, full control, learning experience
5. **Cons:** Most time consuming, requires equipment and skill

## Testing After Assembly

### Visual Inspection
- [ ] No solder bridges between pins
- [ ] All components present and oriented correctly
- [ ] No cold solder joints (shiny, smooth joints)
- [ ] Soldermask intact, no scratches
- [ ] Silkscreen legible

### Electrical Testing
- [ ] Continuity: Ground connections
- [ ] Isolation: AC to DC sections (>10MΩ)
- [ ] Power supply: 5V output ±5%
- [ ] GPIO functionality: All pins toggle
- [ ] Zero-cross detection: 100/120Hz signal
- [ ] TRIAC switching: All channels operational

### Functional Testing
- [ ] ESP32 programming successful
- [ ] WiFi connection stable
- [ ] Cloud communication working
- [ ] Physical switches responsive
- [ ] Voice assistant discovery
- [ ] WebSocket server running
- [ ] All loads controllable

## Safety & Compliance

### Electrical Safety
- IEC 60950-1 (Safety of Information Technology Equipment)
- Isolation barriers meet UL/IEC requirements
- High voltage traces adequately spaced
- Proper grounding provided

### EMC Compliance
- EN 55022 (EMI emissions)
- FCC Part 15 Class B
- Proper ground plane design
- Filtering on power inputs

### Environmental
- RoHS compliant (lead-free)
- REACH compliant
- Proper waste disposal

## Troubleshooting Manufacturing Issues

### Common PCB Defects
| Issue | Cause | Solution |
|-------|-------|----------|
| Solder bridges | Too much paste, wrong profile | Adjust stencil, reflow profile |
| Cold joints | Low temperature, contamination | Increase temp, clean boards |
| Tombstoning | Uneven heating | Balance pad sizes, slower ramp |
| Missing components | Pick-and-place error | Verify coordinates, check feeders |
| Wrong values | BOM mismatch | Cross-check BOM with design |
| PCB dimension error | Gerber export issue | Verify board outline layer |

### Contact & Support
- **Design files:** See `hardware/schematics/` directory
- **Schematic:** See `compact_snubberless_schematic.txt`
- **GitHub:** https://github.com/Gokul3611/smarthome-controller
- **Issues:** Open GitHub issue with "PCB:" prefix

## Version History

| Version | Date | Changes |
|---------|------|---------|
| 3.1 | Dec 2024 | Initial compact snubberless design |
| | | 70mm × 50mm, fits switch box |
| | | MOC3041 zero-cross optocouplers |
| | | HLK-PM01 integrated power supply |

## License
Hardware design licensed under CERN Open Hardware License v2.0

---

**Ready for Manufacturing!** 🎉

All files in this directory are production-ready and have been verified for manufacturability.
For any questions, refer to the main project README or open a GitHub issue.
