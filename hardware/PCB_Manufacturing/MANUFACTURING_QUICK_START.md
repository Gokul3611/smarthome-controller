# 🚀 QUICK START GUIDE - PCB Manufacturing

## Ready to Order? Follow These Steps!

### ⚡ FAST TRACK (Recommended)

**Option 1: JLCPCB (China) - Fastest & Cheapest**
1. Go to https://jlcpcb.com
2. Click "Order Now" → "Add Gerber File"
3. Upload `hardware/PCB_Manufacturing/Gerber_Files/` (ZIP all 7 files)
4. Select options:
   - PCB Qty: 5 (minimum)
   - Layers: 2
   - Dimensions: 70mm × 50mm
   - PCB Thickness: 1.6mm
   - PCB Color: Green
   - Surface Finish: HASL (Lead free)
   - Copper Weight: 2 oz
5. Optional: Enable "SMT Assembly" and upload BOM.csv + PickAndPlace.csv
6. Checkout - Total: ~$10 for 5 bare PCBs (or ~$150 for 5 assembled)
7. Lead time: 5-7 days + 5-10 days shipping

**Option 2: PCBWay (China) - Good Quality**
Similar process, slightly higher cost but better quality control

**Option 3: OSH Park (USA) - Premium Quality**
- Higher cost (~$50 for 3 boards)
- Purple PCBs (their standard)
- Excellent quality, US-based

---

## 📦 WHAT YOU NEED TO ORDER

### Files to Upload
```
Gerber_Files/
├── TopCopper.gtl          ← Copper layer (component side)
├── BottomCopper.gbl       ← Copper layer (ground plane)
├── TopSolderMask.gts      ← Soldermask (component side)
├── BottomSolderMask.gbs   ← Soldermask (bottom)
├── TopSilkscreen.gto      ← Labels and text
├── BottomSilkscreen.gbo   ← Bottom markings
└── BoardOutline.gko       ← PCB shape and mounting holes

DrillFile.drl              ← All holes (vias, components, mounting)

For Assembly Service (optional):
BOM.csv                    ← Parts list with supplier codes
PickAndPlace.csv           ← SMD component positions
```

### Ordering Options

| Option | Cost (5 pcs) | Lead Time | Pros | Cons |
|--------|-------------|-----------|------|------|
| **Bare PCB** | $10-20 | 5-7 days | Cheapest | Manual assembly required |
| **PCB + SMD** | $150-200 | 10-15 days | Only through-hole left | Still need hand soldering |
| **Full Assembly** | $250-350 | 15-20 days | Ready to use | Most expensive |

---

## 🛠️ ASSEMBLY OPTIONS

### Option A: DIY Assembly (Cheapest)
**What you need:**
- Bare PCBs ($10-20)
- Components from BOM.csv (~$40)
- Soldering iron, solder paste, hot plate/oven
- Total: ~$60 for 5 units

**Time:** 2-4 hours per board  
**Skill:** Intermediate to advanced  
**Tools:** Soldering station, hot air, tweezers, magnifier

### Option B: PCBA Service (Recommended)
**What you need:**
- Order PCB + SMD assembly from JLCPCB
- Buy through-hole parts separately (ESP32, TRIACs, connectors)
- Hand-solder through-hole components
- Total: ~$150-200 for 5 units

**Time:** 30 minutes per board (just through-hole)  
**Skill:** Beginner to intermediate  
**Tools:** Basic soldering iron

### Option C: Full Turnkey Assembly (Easiest)
**What you need:**
- Order complete assembly service
- Manufacturer sources all parts
- Total: ~$250-350 for 5 units

**Time:** Zero (arrives ready)  
**Skill:** None required  
**Tools:** None needed

---

## 📋 JLCPCB ASSEMBLY GUIDE (Step-by-Step)

### Step 1: Upload Gerber Files
1. Create ZIP of all Gerber files: `SmartHome_Gerbers.zip`
2. Upload to JLCPCB
3. System auto-detects: 70mm × 50mm, 2-layer

### Step 2: Select PCB Options
```
✓ Base Material: FR-4
✓ Layers: 2
✓ PCB Thickness: 1.6mm
✓ PCB Color: Green (or your choice)
✓ Silkscreen: White
✓ Surface Finish: HASL (Lead free) or ENIG
✓ Copper Weight: 2 oz
✓ Via Covering: Tented
✓ Remove Order Number: Yes (or specify location)
✓ Flying Probe Test: Yes (recommended)
```

### Step 3: Enable SMT Assembly (Optional)
1. Toggle "SMT Assembly" ON
2. Select side: Top Side
3. Upload BOM: `BOM.csv`
4. Upload CPL (Pick-and-place): `PickAndPlace.csv`
5. JLCPCB will auto-match parts from their library
6. Review and confirm parts (some may need manual selection)
7. Parts not in JLCPCB library: Order separately

### Step 4: Review & Checkout
1. Review PCB preview (check for errors)
2. Review assembly preview (component positions)
3. Add to cart
4. Checkout with PayPal/Credit Card
5. Select shipping (DHL faster, ePacket cheaper)

### Step 5: Wait for Delivery
- Manufacturing: 5-7 days
- Shipping: 5-10 days (ePacket) or 2-5 days (DHL)
- Total: 10-17 days to your door

---

## 🔧 COMPONENT SOURCING

### Parts in JLCPCB Library (Auto-sourced)
✅ Most resistors and capacitors (0805, 1206, 2512)  
✅ PC817C optocoupler  
✅ DF04M bridge rectifier  
✅ Some MOC3041 variants  

### Parts to Order Separately
❌ ESP32-WROOM-32 (through-hole) - DigiKey, Mouser, AliExpress  
❌ BTA16-600B TRIACs (TO-220) - DigiKey, Mouser  
❌ HLK-PM01 power module - AliExpress, LCSC  
❌ Screw terminals - Amazon, DigiKey  
❌ JST connectors - DigiKey, AliExpress  

### Where to Buy Components

| Supplier | Best For | Shipping | Notes |
|----------|----------|----------|-------|
| **DigiKey** | Fast US delivery | 1-3 days | Free >$50 |
| **Mouser** | Large selection | 1-3 days | Free >$50 |
| **LCSC** | Cheap from China | 7-15 days | Min $5 |
| **AliExpress** | Bulk cheap parts | 15-45 days | Very cheap |
| **Amazon** | Immediate | 1-2 days | Prime |

**Recommended:** Order HLK-PM01 and MOC3041 from LCSC or AliExpress (3-4 weeks)  
Order ESP32 and TRIACs from DigiKey/Mouser (fast, reliable)

---

## 💰 COST BREAKDOWN

### Bare PCB Only (DIY Assembly)
```
5× PCBs from JLCPCB           $12
5× Component sets              $200 ($40 each)
Solder paste, flux, misc       $10
──────────────────────────────────
TOTAL for 5 units:             $222
Per unit:                      $44
```

### PCB + SMD Assembly
```
5× PCBs + SMD assembly         $180
5× Through-hole parts          $50 ($10 each)
──────────────────────────────────
TOTAL for 5 units:             $230
Per unit:                      $46
```

### Bulk Order (100 units)
```
100× PCBs                      $300
100× Component sets            $1,600 (volume discount)
Assembly service               $500
──────────────────────────────────
TOTAL for 100 units:           $2,400
Per unit:                      $24
```

---

## ⏱️ TIMELINE

### Fastest Path (3 weeks total)
```
Day 0:    Order PCBs + components
Day 7:    PCBs arrive
Day 14:   Components arrive from AliExpress
Day 15-16: Assembly (2 hours per board × 5 boards)
Day 17:   Testing and flashing firmware
Day 18:   Installation in switch boxes
Day 21:   System operational
```

### Lazy Path (4-6 weeks)
```
Week 1:   Order everything
Week 2-3: Manufacturing & shipping
Week 4:   Assembly service or DIY when free
Week 5-6: Testing and deployment
```

---

## ✅ QUALITY CHECKS BEFORE ORDERING

### Files Checklist
- [x] All 7 Gerber files present
- [x] DrillFile.drl included
- [x] BOM.csv has all parts
- [x] PickAndPlace.csv has coordinates
- [x] Board dimensions correct (70mm × 50mm)
- [x] Mounting holes correct (M2.5 / 2.7mm)

### Design Verification
- [x] Isolation barrier visible (6mm gap)
- [x] High voltage traces adequate width (2mm)
- [x] Thermal vias under TRIACs
- [x] Silkscreen readable
- [x] Soldermask openings correct
- [x] Component footprints verified

### Use Gerber Viewer
1. Download: https://www.pcbway.com/project/OnlineGerberViewer.html
2. Upload Gerber files
3. Check visually:
   - No shorts between AC and DC
   - All pads accessible
   - Silkscreen not on pads
   - Drill holes aligned

---

## 🆘 TROUBLESHOOTING

### Issue: Gerber files rejected
**Solution:** ZIP all files without folder structure
```bash
cd hardware/PCB_Manufacturing/Gerber_Files/
zip -j SmartHome_Gerbers.zip *.gtl *.gbl *.gts *.gbs *.gto *.gbo *.gko
```

### Issue: BOM parts not found in JLCPCB
**Solution:** 
1. Note which parts are missing
2. Order those separately
3. Continue with available parts
4. Hand-solder missing components later

### Issue: Drill file not recognized
**Solution:** Ensure DrillFile.drl is in Excellon format (it is!)

### Issue: Assembly costs too high
**Solution:**
1. Disable assembly
2. Order bare PCBs only ($12)
3. Buy components separately
4. Assemble yourself or local service

---

## 📞 SUPPORT

### Before Ordering
- Review `FAILSAFE_DESIGN.md` for technical details
- Check `README.md` for manufacturing specifications
- Review `PCB_Stackup.txt` for layer details

### During Manufacturing
- JLCPCB Support: support@jlcpcb.com
- PCBWay Support: support@pcbway.com

### After Assembly
- GitHub Issues: https://github.com/Gokul3611/smarthome-controller/issues
- Label: "PCB:" prefix

---

## 🎓 TIPS FOR SUCCESS

1. **Start Small:** Order 5 PCBs first, not 100
2. **Test One:** Assemble and test one board completely before doing all
3. **Use Assembly Service:** Save time, worth the cost for SMD
4. **Order Extra Parts:** Get 10-20% extra components for mistakes
5. **Document Everything:** Take photos during assembly
6. **Test Progressively:** Power → Programming → Zero-cross → TRIACs → Full system
7. **Have Backup:** Keep one spare programmed board
8. **Safety First:** Double-check AC wiring before power-on

---

## 🚀 READY TO ORDER!

**Recommended First Order:**
- 5× PCBs with SMD assembly from JLCPCB (~$180)
- 5× ESP32 modules from DigiKey (~$25)
- 5× TRIAC sets from Mouser (~$12)
- 5× HLK-PM01 from AliExpress (~$18)
- Misc connectors and terminals (~$15)

**Total: ~$250 for 5 fully assembled and tested boards**

**Time: 3-4 weeks from order to operational**

---

**Good luck with your build! 🎉**

For questions, open a GitHub issue with "PCB:" prefix.

---

Document Version: 1.0  
Last Updated: December 2024  
Status: Ready for Production ✅
