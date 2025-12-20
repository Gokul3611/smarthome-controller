# 📐 Mini Controller Wiring Guide

## 🎯 Super Simple Setup

### Option 1: No External Components (Easiest!)

Most ESP32 boards have a built-in LED on GPIO 2. Just upload the code and you're done!

**Test Steps:**
1. Upload firmware
2. Watch Serial Monitor
3. Control from dashboard
4. See built-in LED blink!

---

## 🔌 Option 2: External LED (More Visible)

### Components Needed
```
□ 1x LED (any color)
□ 1x 220Ω Resistor (Red-Red-Brown)
□ 2x Jumper wires
□ 1x Breadboard (optional)
```

### Wiring

```
                ESP32
              ┌─────┐
              │     │
              │  2  ├──────┐
              │     │      │
              │     │    ┌─▼─┐
              │     │    │220│ Resistor
              │ GND ├──┐ │ Ω │
              │     │  │ └─┬─┘
              └─────┘  │   │
                       │  ┌▼┐
                       │  │ │ LED
                       │  │►│ (Longer leg to resistor)
                       │  └┬┘
                       └───┘
```

### Step-by-Step Connection

```
Step 1: Insert LED into breadboard
   - Longer leg (positive/anode) in one row
   - Shorter leg (negative/cathode) in another row

Step 2: Add resistor
   - One end to same row as LED's longer leg
   - Other end to a new row

Step 3: Connect to ESP32
   - Jumper from GPIO 2 to resistor's free end
   - Jumper from GND to LED's shorter leg
```

### Visual Breadboard Layout

```
    Breadboard Top View
    
 +5V ○─○─○─○─○─○─○─○─○─○
     ○─○─○─○─○─○─○─○─○─○
  J  ○─○─○─○─○─○─○─○─○─○
  10 ●─────────[220Ω]────● ◄─── From GPIO 2
   9 ○─○─○─○─○─○─○─○─○─○
   8 ●─────[LED +]────────
   7 ○─○─○─○─○─○─○─○─○─○
   6 ●─────[LED -]────────● ◄─── To GND
   5 ○─○─○─○─○─○─○─○─○─○
     ○─○─○─○─○─○─○─○─○─○
 GND ○─○─○─○─○─○─○─○─○─○
 
 ● = Connection point
```

---

## 🎨 LED Color Guide

Any color works! Choose based on visibility:

- **🔴 Red:** Most common, very visible
- **🟢 Green:** Easy to see, commonly available
- **🔵 Blue:** Cool look, slightly dimmer
- **🟡 Yellow:** High visibility
- **⚪ White:** Bright, modern look

**Note:** All LEDs work the same way with this circuit!

---

## ⚡ GPIO Pin Reference

### Default Pin: GPIO 2
- Works on all ESP32 boards
- Usually has built-in LED
- Safe for testing

### Alternative Pins (if GPIO 2 is busy)
You can use any GPIO pin! Just change in code:

```cpp
#define LED_PIN 2  // Change to any GPIO: 4, 5, 12, 13, 14, 15, etc.
```

**Safe pins to use:**
- GPIO 2, 4, 5, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23

**Avoid these pins:**
- GPIO 0 (Boot mode pin)
- GPIO 1, 3 (Serial TX/RX)
- GPIO 6-11 (Internal flash)

---

## 🔧 Connection Verification

### Quick Test (No Multimeter Needed)

**Test 1: Visual Check**
```
✓ LED longer leg → Resistor → GPIO 2
✓ LED shorter leg → GND
✓ No loose wires
✓ Resistor is connected (not bridged)
```

**Test 2: Code Upload Check**
```
Upload code → Watch Serial Monitor
Should see: "System ready - LED connected to GPIO 2"
```

**Test 3: Manual Test**
```cpp
// Add this to setup() temporarily:
digitalWrite(LED_PIN, HIGH);
delay(1000);
digitalWrite(LED_PIN, LOW);
```
LED should blink once on startup

### With Multimeter (Advanced)

1. **Check LED polarity:**
   - Set multimeter to diode test mode
   - Touch probes to LED legs
   - LED glows dimly → correct polarity
   - Reverse probes → no glow

2. **Check connections:**
   - Measure resistance between GPIO 2 and LED: ~220Ω
   - Check continuity from LED to GND

---

## 🚨 Troubleshooting

### LED Not Lighting Up

**Problem:** LED stays off

**Solutions:**
1. Check LED polarity (flip it around)
2. Verify resistor is 220Ω (Red-Red-Brown)
3. Check GPIO 2 connection
4. Test with built-in LED first
5. Try a different LED (might be faulty)

### LED Always On

**Problem:** LED won't turn off

**Solutions:**
1. Check for short circuits
2. Verify ground connection
3. Re-upload firmware
4. Check Serial Monitor for errors

### LED Very Dim

**Problem:** Can barely see LED

**Solutions:**
1. Normal at low brightness settings
2. Try increasing brightness in dashboard
3. Use a brighter color LED (white, red)
4. Check resistor value (should be 220Ω, not higher)

### LED Flickering

**Problem:** LED flickers randomly

**Solutions:**
1. Check for loose connections
2. Verify WiFi stability (weak WiFi can cause issues)
3. Move ESP32 closer to router
4. Check power supply (use good USB cable)

---

## 📷 Photo Reference Guide

### LED Identification

```
  LED (Light Emitting Diode)
  
  Longer leg = Positive (+)
  │
  │  ┌─┐
  └──┤●│  ← Rounded edge (negative side)
     │●│
  ┌──┤●│  ← Flat edge
  │  └─┘
  │
  Shorter leg = Negative (-)
```

### Resistor Color Codes

```
220Ω Resistor (Red-Red-Brown)

  ┌──────────────┐
  │ █ █   █  │█││  Red-Red-Brown-Gold
  │ R R   B  │G││  = 220Ω ±5%
  └──────────────┘
    1st 2nd Multiplier Tolerance
    2   2   ×10      ±5%
```

**Other acceptable values:**
- 180Ω (Brown-Gray-Brown)
- 220Ω (Red-Red-Brown) ← Best
- 270Ω (Red-Purple-Brown)
- 330Ω (Orange-Orange-Brown)

**Don't use:**
- Below 150Ω (too much current)
- Above 1kΩ (LED too dim)

---

## 🎓 Understanding the Circuit

### Why Do We Need a Resistor?

**Without resistor:**
```
GPIO 2 → LED → GND
Result: LED burns out! 💥
```

**With resistor:**
```
GPIO 2 → Resistor → LED → GND
Result: Safe, controlled current ✅
```

### Current Calculation

```
ESP32 GPIO: 3.3V
LED forward voltage: ~2.0V
LED safe current: ~10-20mA

Resistor = (3.3V - 2.0V) / 0.015A
         = 1.3V / 0.015A
         = ~87Ω minimum

220Ω = Safe with margin!
```

### PWM Brightness Control

The mini controller uses PWM (Pulse Width Modulation):

```
100% Brightness: ████████████ (always on)
75%  Brightness: █████████░░░ (on 75% of time)
50%  Brightness: ██████░░░░░░ (on 50% of time)
25%  Brightness: ███░░░░░░░░░ (on 25% of time)
0%   Brightness: ░░░░░░░░░░░░ (always off)
```

The LED appears to dim because it's switching on/off very fast (thousands of times per second) - too fast for your eye to see individual flashes!

---

## 💡 Pro Tips

1. **Start with built-in LED**
   - No wiring needed
   - Verify code works first
   - Add external LED later

2. **Use a breadboard**
   - Easy to connect/disconnect
   - No soldering needed
   - Can reuse components

3. **Keep wires short**
   - Reduces interference
   - Looks neater
   - Easier to debug

4. **Label your connections**
   - Use tape and marker
   - Note GPIO pins used
   - Document modifications

5. **Take photos**
   - Before and after wiring
   - Help troubleshooting later
   - Share your setup!

---

## 📦 Shopping List

### Minimum Purchase (Amazon/AliExpress)

```
□ ESP32 Dev Board ($3-10)
□ LED Assortment Kit ($2-5)
□ Resistor Kit ($3-7)
□ Breadboard ($2-5)
□ Jumper Wire Kit ($2-5)

Total: ~$15-30
```

### Recommended Kit

Look for "ESP32 Starter Kit" or "Arduino Starter Kit" which includes:
- ESP32 board
- LEDs (multiple colors)
- Resistors (various values)
- Breadboard
- Jumper wires
- USB cable

Price: $20-40 (has everything you need!)

---

## 🔗 Video Tutorials

**Search YouTube for:**
- "ESP32 LED blink tutorial"
- "How to connect LED to ESP32"
- "ESP32 breadboard basics"

These show the exact same wiring (just different code).

---

## ✅ Final Checklist

Before powering on:

- [ ] LED connected with correct polarity
- [ ] 220Ω resistor in series with LED
- [ ] No short circuits (LED legs not touching)
- [ ] GPIO 2 connected to resistor
- [ ] GND connected to LED negative
- [ ] Code uploaded successfully
- [ ] Serial Monitor open (115200 baud)

**All checked? Power it on! 🎉**

---

**Need Help?**

Check the main README.md or open an issue on GitHub with:
- Photos of your wiring
- Serial Monitor output
- ESP32 board model
- What you've tried

We're here to help! 🚀
