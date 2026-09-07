```cpp
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// =====================================================
// ESP32 NeoPixel Color Controller
// =====================================================

// ---------------- PIN CONFIGURATION ------------------

#define LED_PIN     14
#define BUTTON_PIN  27
#define NUM_LEDS    11

// OLED
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 64

#define OLED_SDA 21
#define OLED_SCL 22
#define OLED_ADDRESS 0x3C

// ---------------- COMPONENTS -------------------------

Adafruit_NeoPixel strip(
  NUM_LEDS,
  LED_PIN,
  NEO_GRB + NEO_KHZ800
);

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  -1
);

// ---------------- COLOR STRUCTURE --------------------

struct Color {
  const char* name;
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

// ---------------- COLOR LIST -------------------------

Color colors[] = {
  {"RED",     255, 0,   0},
  {"GREEN",   0,   255, 0},
  {"BLUE",    0,   0,   255},
  {"YELLOW",  255, 255, 0},
  {"CYAN",    0,   255, 255},
  {"PURPLE",  128, 0,   128},
  {"WHITE",   255, 255, 255}
};

const int COLOR_COUNT =
  sizeof(colors) / sizeof(colors[0]);

int colorIndex = 0;

// ---------------- BUTTON SETTINGS --------------------

bool lastButtonState = HIGH;

unsigned long lastDebounceTime = 0;

const unsigned long debounceDelay = 250;

// ---------------- SETUP -----------------------------

void setup() {

  Serial.begin(115200);

  // Button
  // Button connects GPIO27 to GND
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // NeoPixel
  strip.begin();

  // Limit brightness to reduce power consumption
  strip.setBrightness(120);

  // Make sure LEDs start off
  strip.clear();
  strip.show();

  // OLED I2C
  Wire.begin(OLED_SDA, OLED_SCL);

  if (!display.begin(
        SSD1306_SWITCHCAPVCC,
        OLED_ADDRESS
      )) {

    Serial.println("OLED initialization failed!");

    while (true) {
      delay(100);
    }
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  // Display startup message
  display.setTextSize(2);
  display.setCursor(0, 20);
  display.println("Starting...");
  display.display();

  delay(1000);

  // Show initial color
  updateColor();
}

// ---------------- MAIN LOOP --------------------------

void loop() {

  bool buttonState = digitalRead(BUTTON_PIN);

  // Detect button press
  if (
    buttonState == LOW &&
    lastButtonState == HIGH &&
    millis() - lastDebounceTime > debounceDelay
  ) {

    lastDebounceTime = millis();

    // Move to next color
    colorIndex++;

    // Return to first color after last color
    if (colorIndex >= COLOR_COUNT) {
      colorIndex = 0;
    }

    updateColor();
  }

  // Remember current button state
  lastButtonState = buttonState;
}

// ---------------- UPDATE COLOR -----------------------

void updateColor() {

  Color current = colors[colorIndex];

  // Set every NeoPixel to the selected color
  for (int i = 0; i < NUM_LEDS; i++) {

    strip.setPixelColor(
      i,
      strip.Color(
        current.r,
        current.g,
        current.b
      )
    );
  }

  // Send data to the strip
  strip.show();

  // Update OLED
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 20);
  display.println(current.name);

  display.display();

  // Serial debugging
  Serial.print("Current color: ");
  Serial.println(current.name);
}
```
