# Bill of Materials (BOM)

Complete parts list for building the Smart Home Controller.

## Main Components

| Item | Part Number | Description | Qty | Unit Price | Total | Supplier |
|------|-------------|-------------|-----|------------|-------|----------|
| 1 | ESP32-WROOM-32 | WiFi+BT MCU Module | 1 | $5.00 | $5.00 | DigiKey, Mouser |
| 2 | BT136-600E | 4A 600V TRIAC TO-220 | 4 | $0.50 | $2.00 | DigiKey, Mouser |
| 3 | MOC3021 | Random Phase Optocoupler | 4 | $0.40 | $1.60 | DigiKey, Mouser |
| 4 | 4N25 | Optocoupler for Zero-Cross | 1 | $0.30 | $0.30 | DigiKey, Mouser |
| 5 | 1N4007 | 1A 1000V Rectifier Diode | 4 | $0.10 | $0.40 | DigiKey, Mouser |
| 6 | AMS1117-5.0 | 5V 1A Linear Regulator | 1 | $0.50 | $0.50 | DigiKey, Mouser |
| 7 | Transformer | 230V to 12V 2VA | 1 | $3.00 | $3.00 | Local supplier |

## Resistors (1/4W unless noted)

| Value | Description | Qty | Notes |
|-------|-------------|-----|-------|
| 100Ω | TRIAC gate resistor (1/2W) | 4 | Must be 1/2W or higher |
| 330Ω | LED current limit | 4 | For optocoupler drive |
| 1kΩ | Zero-cross limiting | 1 | 1/2W recommended |
| 10kΩ | Pull-down/Pull-up | 8 | General purpose |
| 100kΩ | High impedance | 2 | Optional, for RC snubber |

## Capacitors

| Value | Type | Voltage | Qty | Description |
|-------|------|---------|-----|-------------|
| 0.1µF | Ceramic | 50V | 8 | Bypass capacitors |
| 0.1µF | Film | 400V | 4 | Snubber capacitors (optional) |
| 10µF | Electrolytic | 16V | 2 | Power supply filtering |
| 100µF | Electrolytic | 16V | 2 | Power supply filtering |
| 1000µF | Electrolytic | 16V | 1 | Main power reservoir |

## Connectors & Terminals

| Item | Description | Qty | Notes |
|------|-------------|-----|-------|
| Screw terminals | 2-position 5mm pitch | 8 | For AC connections |
| Pin headers | 2.54mm pitch | 20 | For ESP32 mounting |
| USB connector | Micro-B or Type-C | 1 | For programming |

## Mechanical Parts

| Item | Description | Qty | Notes |
|------|-------------|-----|-------|
| Heatsink | TO-220 compatible | 4 | For TRIACs |
| Thermal pad | Silicone pad | 4 | Between TRIAC and heatsink |
| Mounting screws | M3 × 6mm | 8 | For heatsinks |
| Standoffs | M3 × 10mm | 4 | For PCB mounting |
| Enclosure | ABS plastic 120×80×40mm | 1 | Non-conductive |

## Optional Components

| Item | Description | Qty | Purpose |
|------|-------------|-----|---------|
| Status LED | 5mm red LED | 1 | Power indicator |
| Tactile switch | 6×6mm | 4 | Physical controls |
| Buzzer | 5V piezo | 1 | Audio feedback |
| Temperature sensor | DS18B20 | 1 | Thermal monitoring |
| Current sensor | ACS712 5A | 4 | Power monitoring |

## Tools & Consumables

| Item | Description | Notes |
|------|-------------|-------|
| Solder | 60/40 or lead-free | ~50g required |
| Flux | Rosin flux pen | For rework |
| Wire | 18AWG stranded | For AC connections |
| Wire | 22-24AWG solid | For DC connections |
| Heat shrink | Assorted sizes | For insulation |
| Cable ties | 100mm | For wire management |

## Cost Summary

| Category | Estimated Cost |
|----------|----------------|
| Electronic Components | $15.00 |
| Connectors & Hardware | $5.00 |
| Enclosure | $8.00 |
| PCB (prototype) | $10.00 |
| Optional Components | $10.00 |
| Tools & Consumables | $15.00 |
| **Total (single unit)** | **$48.00** |
| **Total (10 units)** | **$200.00** |
| **Total (100 units)** | **$1,200.00** |

*Prices approximate, as of December 2024. Bulk pricing available for quantities >10.*

## Supplier Information

### Recommended Suppliers

**International:**
- DigiKey (digikey.com) - Fast shipping, large inventory
- Mouser (mouser.com) - Good for small quantities
- LCSC (lcsc.com) - Low cost, based in China
- Arrow (arrow.com) - Good for professional quantities

**Local (India):**
- Robu.in
- ElectronicsPro.in
- Amazon.in (limited selection)

### Bulk Purchasing

For quantities >50 units:
- Contact manufacturers directly
- Use Alibaba for Chinese suppliers
- Negotiate pricing with local distributors

## Substitutions

### Acceptable Alternatives

| Original Part | Alternative | Notes |
|---------------|-------------|-------|
| BT136 | BTA16, Q4008L | Similar specs |
| MOC3021 | MOC3041 | 3041 has zero-cross |
| 4N25 | PC817, LTV817 | Any optocoupler |
| ESP32-WROOM-32 | ESP32-WROVER-32 | More RAM |
| AMS1117 | LM1117, LD1117 | Same footprint |

### NOT Recommended

- Don't use SCRs instead of TRIACs (won't work for AC)
- Don't use transistors instead of optocouplers (no isolation)
- Don't use undersized TRIACs (minimum 4A rating)

## Procurement Notes

1. **Order extra components** (10-20% surplus) for prototyping and repairs
2. **Check lead times** - some components may have 4-8 week delivery
3. **Buy from authorized distributors** to avoid counterfeit parts
4. **Keep datasheets** - download and archive for reference
5. **Test components** before assembly (especially TRIACs and optocouplers)

## Quality Control

### Incoming Inspection

For critical components, test:
- [ ] TRIAC forward voltage drop
- [ ] Optocoupler CTR (current transfer ratio)
- [ ] ESP32 basic functionality
- [ ] Power supply output voltage
- [ ] Capacitor ESR (for electrolytics)

## Revision History

| Version | Date | Changes |
|---------|------|---------|
| 1.0 | Dec 2024 | Initial BOM |

## Notes

- All prices in USD
- Minimum order quantities (MOQ) may apply
- Check local availability and import duties
- Consider lead-free requirements for commercial products
- Add margin for damaged/lost components

---

**Contact**: For BOM questions, open GitHub issue with "BOM:" prefix
