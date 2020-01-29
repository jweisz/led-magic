#include <Adafruit_NeoPixel.h>
#include <HSBColor.h>
#include <FHT.h>

#define PIN               4
#define LED_COUNT         60

#define FHT_N             256
#define LOG_OUT           1

#define MAX_SATURATION    99
#define MAX_BRIGHTNESS    99


// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)
Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

//Metro modeSwitch = Metro(1000 * INTERVAL_TIME_MIN);

void setup() {
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
  while (1) {
    cli();
    for (int i = 0; i < FHT_N; i++) {
      while (!(ADCSRA & 0x10));
      ADCSRA = 0xf5;
      byte m = ADCL;
      byte j = ADCH;
      int k = (j << 8) | m;
      k -= 0x0200;
      k <<= 6;
      fht_input[i] = k;
    }
    fht_window();
    fht_reorder();
    fht_run();
    fht_mag_log();
    
    for (int i = 0; i < FHT_N/2; i++) {
      uint8_t v = fht_log_out[i];
      int b = v / MAX_BRIGHTNESS;
      uint32_t c = rgbColorFromHSB(0, b, b);
      leds.setPixelColor(i, c);
    }
    leds.show();
  }
}

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
