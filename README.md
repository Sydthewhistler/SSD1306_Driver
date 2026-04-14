# SSD1306 Driver — ESP32

I²C driver for SSD1306 128×64 OLED display, written from scratch without the Adafruit library.
Implemented in embedded C++ for ESP32 with PlatformIO.

---

## Hardware Requirements

- ESP32 (tested on ESP32 Dev Module)
- SSD1306 128×64 OLED display (I²C)

### Wiring

| OLED | ESP32   |
|------|---------|
| VCC  | 3.3V    |
| GND  | GND     |
| SDA  | GPIO 21 |
| SCL  | GPIO 22 |

---s

## Installation

Copy the `SSD1306/` folder into your project's `lib/` directory (PlatformIO) or alongside your sketch (Arduino IDE).

---

## Usage

```cpp
#include "SSD1306_driver.hpp"

Display* display = nullptr;

void setup() {
    display = new Display(0x3C);
    display->init();
}

void loop() {
    display->clear();
    display->setCursor(0, 0);
    display->setTextSize(1);
    display->print("Hello, World!");
    display->display();
}
```

---

## Internal Architecture

### Framebuffer

The driver maintains a 1024-byte framebuffer in RAM (128 columns × 8 pages).
All drawing operations modify the framebuffer — the screen is only updated when `display()` is called.

### SSD1306 Memory Layout

The GDDRAM is organized into 8 horizontal pages of 8 pixels each.
Each byte represents a vertical column of 8 pixels within a page.

```
Page 0 : rows  0 →  7
Page 1 : rows  8 → 15
Page 2 : rows 16 → 23
...
Page 7 : rows 56 → 63
```

### Font

5×7 pixel font stored in Flash (`const`), based on Adafruit GFX (`glcdfont.c`).
Each character is encoded as 5 bytes — one byte per vertical column.

---

## Key Concepts

- I²C protocol — 7-bit addressing, D/C# control byte, Co bit
- Datasheet reading — SSD1306 registers, initialization sequence
- Framebuffer — RAM/screen double buffering
- GDDRAM page organization
- Constants stored in Flash