#include <Adafruit_NeoPixel.h>
#include <HSBColor.h>
#include <Easing.h>

#define PIN               4
#define LED_COUNT         120
#define INTERVAL_TIME_MIN 1

#define MAX_SATURATION    99
#define MAX_BRIGHTNESS    99

#define HUE_DIFF          30

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)
Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

int oldHue = randomHue();
int mid = floor(LED_COUNT / 2);
bool in = true;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  Serial.println("hello, world");
  
  randomSeed(analogRead(0));
  
  // begin using LEDs
  leds.begin();
  
  // starting state is off
  clearLEDs();
  leds.show();
}

void loop() {
  int hue = 0;
  do {
    hue = (oldHue + randomHue()) % 360;
  } while ( abs(hue - oldHue) < HUE_DIFF );
  
  oldHue = hue;
  
  Serial.println(hue);
  
  uint32_t c = rgbColorFromHSB(hue, MAX_SATURATION, MAX_BRIGHTNESS);
  
  clearLEDs();
  leds.show();
  
  turnThemOn(c, 20);
  delay(20);
  turnThemOn(0, 20);
  //flashy(c, 3, 80);
    
  delay(50);
}

// ------------------------------------------------------------------

void clearLEDs() {
  for ( int i = 0; i < LED_COUNT; i++ ) {
    leds.setPixelColor(i, 0);
  }
}

int randomHue() {
  return random(0, 360);
}

uint32_t rgbColorFromHSB(int hue, int saturation, int brightness) {
  int rgb[3];
  H2R_HSBtoRGB(hue, saturation, brightness, rgb);
  
  // Serial.print("rgb: ");
  // Serial.print(rgb[0], HEX);
  // Serial.print(" ");
  // Serial.print(rgb[1], HEX);
  // Serial.print(" ");
  // Serial.print(rgb[2], HEX);
  // Serial.println();
  
  return leds.Color(rgb[0], rgb[1], rgb[2]);
}

void turnThemOn(uint32_t color, int d) {
  int pixels[LED_COUNT] = {0};
  for ( int i = 0; i < LED_COUNT; i++ ) {
    pixels[i] = i;
  }
  for ( int i = LED_COUNT - 1; i > 1; i-- ) {
    int j = random(0, i);
    int tmp = pixels[i];
    pixels[i] = pixels[j];
    pixels[j] = tmp;
  }
  for ( int i = 0; i < LED_COUNT; i++ ) {
    leds.setPixelColor(pixels[i], color);
    leds.show();
    delay(d);
  }
}

void flashy(uint32_t color, int count, int d) {
  for ( int c = 0; c < count; c++ ) {
    for ( int i = 0; i < LED_COUNT; i++ ) {
      leds.setPixelColor(i, 0);
    }
    leds.show();
    delay(d);
    for ( int i = 0; i < LED_COUNT; i++ ) {
      leds.setPixelColor(i, color);
    }
    leds.show();
    delay(d);
  }
}

