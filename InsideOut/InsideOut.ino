#include <Adafruit_NeoPixel.h>
#include <HSBColor.h>
#include <Easing.h>

#define PIN               4
#define LED_COUNT         60
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

int randomHue() {
  return random(0, 360);
}

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
  
  if ( in == true ) {
    insideOut(c);
  } else {
    outsideIn(c);
  }
  
  in = !in;

//  if ( random(0, 100) % 2 == 0 ) {
//    insideOut(c);
//  } else {
//    outsideIn(c);
//  }
  
  delay(50);
}

// ------------------------------------------------------------------

void clearLEDs() {
  for ( int i = 0; i < LED_COUNT; i++ ) {
    leds.setPixelColor(i, 0);
  }
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

void insideOut(uint32_t color) {
  for ( int i = 0; i < mid; i++ ) {
    int ledA = mid + i;
    int ledB = mid - i - 1;
    leds.setPixelColor(ledA, color);
    leds.setPixelColor(ledB, color);
    leds.show();
    
    float ease = Easing::easeOutExpo(i, 0, 30, mid);
    delay(ease);
  }
}

void outsideIn(uint32_t color) {
  for ( int i = 0; i < mid; i++ ) {
    int ledA = i;
    int ledB = LED_COUNT - i;
    leds.setPixelColor(ledA, color);
    leds.setPixelColor(ledB, color);
    leds.show();
    
    float ease = Easing::easeOutExpo(i, 0, 30, mid);
    delay(ease);
  }
}
