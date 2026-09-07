# Project SNL

A simple ESP32-based RGB NeoPixel controller that uses a pushbutton to cycle through solid colors while displaying the current color on an OLED screen.

## Project Overview

This project uses an **ESP32**, an **11-LED NeoPixel/WS2812 RGB strip**, a **pushbutton**, and a **0.96-inch I2C OLED display**.

The pushbutton is used to cycle through a selection of solid RGB colors. Each color remains active after the button is released, and the OLED displays the name of the currently selected color.

The project is designed as a simple introduction to:

* ESP32 GPIO control
* Addressable RGB LEDs
* Pushbutton input
* Button debouncing
* I2C OLED displays
* Basic embedded programming
* Combining multiple components into one system

---

## Features

* 11 addressable RGB LEDs
* Solid colors across the entire strip
* Single button press changes the color
* Color remains active after the button is released
* OLED displays the current color
* Button debouncing to prevent accidental multiple selections
* Adjustable LED brightness
* Uses the Adafruit NeoPixel library
* No FastLED dependency

### Available Colors

The controller cycles through:

1. Red
2. Green
3. Blue
4. Yellow
5. Cyan
6. Purple
7. White

After White, the controller returns to Red.

---

# Hardware

## Components

| Component                  | Quantity |
| -------------------------- | -------: |
| ESP32 Dev Module           |        1 |
| WS2812 / NeoPixel RGB LEDs |       11 |
| Pushbutton                 |        1 |
| I2C OLED SSD1306 128×64    |        1 |
| 330Ω resistor              |        1 |
| 1000µF capacitor           |        1 |
| Breadboard                 |        1 |
| Jumper wires               |  Several |
| USB cable                  |        1 |

---

# Wiring

## NeoPixel Strip

| NeoPixel | ESP32                         |
| -------- | ----------------------------- |
| +5V      | VIN                           |
| GND      | GND                           |
| DIN      | GPIO 14 through 330Ω resistor |

The resistor should be placed **in series with the data line**:

```text
ESP32 GPIO 14
      |
      |
   330Ω
      |
      |
     DIN
   NeoPixel
```

### Important

Connect to the **DIN / input side** of the NeoPixel strip.

The arrows on the strip should point **away from the ESP32**.

```text
ESP32 → DIN → LED → LED → LED → ... → LED
```

Do not connect the ESP32 data wire to the DOUT/output side.

---

## Capacitor

Place the 1000µF capacitor across the NeoPixel power input:

```text
       +5V
        |
        |
       (+)
    1000µF
       (-)
        |
        |
       GND
```

The capacitor's **positive (+) terminal goes to +5V**.

The capacitor's **negative (-) terminal goes to GND**.

---

## Pushbutton

The button uses the ESP32's internal pull-up resistor, so an external resistor is not required.

```text
GPIO 27 ─── Pushbutton ─── GND
```

The code uses:

```cpp
pinMode(BUTTON_PIN, INPUT_PULLUP);
```

Therefore:

* Button released = HIGH
* Button pressed = LOW

---

## OLED Display

For an I2C SSD1306 OLED:

| OLED | ESP32   |
| ---- | ------- |
| VCC  | 3.3V    |
| GND  | GND     |
| SDA  | GPIO 21 |
| SCL  | GPIO 22 |

The OLED normally uses I2C address:

```text
0x3C
```

---

# Complete Wiring Summary

```text
                         ESP32
                  ┌─────────────────┐
                  │                 │
             VIN  ├─────────────── +5V ───── NeoPixel
             GND  ├─────────────── GND ───── NeoPixel
                  │                 │
           GPIO14 ├── 330Ω ─────── DIN
                  │                 │
           GPIO27 ├──── Button ─── GND
                  │                 │
           GPIO21 ├─────────────── SDA ───── OLED
           GPIO22 ├─────────────── SCL ───── OLED
             3V3  ├─────────────── VCC ───── OLED
                  │                 │
                  └─────────────────┘
```

Make sure the NeoPixel ground and ESP32 ground are connected together.

---

# Software

## Arduino IDE

The project was developed for:

* Arduino IDE 1.8.19
* ESP32 Dev Module

## Required Libraries

Install these libraries through:

**Arduino IDE → Sketch → Include Library → Manage Libraries**

Install:

* **Adafruit NeoPixel**
* **Adafruit GFX Library**
* **Adafruit SSD1306**

FastLED is **not required**.

---

# How It Works

When the ESP32 starts, the controller begins with Red.

The program continuously checks GPIO 27 for a button press.

When the button changes from released to pressed:

```text
Red
 ↓
Green
 ↓
Blue
 ↓
Yellow
 ↓
Cyan
 ↓
Purple
 ↓
White
 ↓
Red
```

The selected color is sent to all 11 NeoPixels, and the OLED is updated with the color name.

The button is debounced in software so that one physical press is interpreted as one selection.

---

# Power

For this short 11-LED strip, the project can be powered from the ESP32's **VIN** pin when the ESP32 is powered through USB.

The NeoPixels can draw significant current, especially at high brightness and white.

The project therefore uses:

```cpp
strip.setBrightness(120);
```

This keeps the brightness at a reasonable level and reduces power consumption.

For larger LED strips, use a separate suitable 5V power supply and connect its ground to the ESP32 ground.

---

# Project Code

The complete Arduino sketch is included in:

```text
ESP32_NeoPixel_Color_Controller.ino
```

## Basic Operation

1. Connect the ESP32 to USB.
2. Upload the Arduino sketch.
3. The OLED displays `RED`.
4. The NeoPixel strip lights red.
5. Press the button once.
6. The LEDs change to green.
7. Release the button.
8. The LEDs remain green.
9. Continue pressing to cycle through the colors.

---

# Troubleshooting

## LEDs do not light

Check:

* NeoPixel +5V is connected to VIN.
* NeoPixel GND is connected to ESP32 GND.
* Data is connected to GPIO 14.
* The data wire is connected to **DIN**, not DOUT.
* The 330Ω resistor is in series with the data line.
* The strip contains 11 LEDs in the code.

```cpp
#define NUM_LEDS 11
```

---

## OLED works but button does nothing

Check:

```text
GPIO27 → button → GND
```

The button uses the internal pull-up resistor.

Also make sure the button is positioned correctly across the breadboard's center gap if using a standard 4-pin tactile switch.

---

## ESP32 keeps resetting

If the Serial Monitor reports:

```text
Brownout detector was triggered
```

the ESP32 is detecting a drop in supply voltage.

Check:

* Power connections
* Short circuits
* NeoPixel current consumption
* USB cable and USB power source
* Correct capacitor polarity

Do not connect the NeoPixels to the ESP32's 3.3V pin.

---

# Future Improvements

Possible additions include:

* Adjustable brightness
* More color selections
* Rainbow mode
* Smooth color transitions
* Multiple buttons
* Long-press functionality
* OLED color preview
* Saving the selected color after restart
* Automatic demonstration mode
* Rotary encoder control

---

# Project Status

**Status: Working ✅**

Current hardware configuration:

* ESP32 Dev Module
* 11 NeoPixels
* Pushbutton on GPIO 27
* NeoPixel data on GPIO 14
* OLED SDA on GPIO 21
* OLED SCL on GPIO 22
* NeoPixel powered from VIN
* Solid-color selection

---

## Author

**Kavish**

Built as an ESP32 electronics and embedded-programming project.
