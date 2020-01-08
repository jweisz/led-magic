#include <Adafruit_NeoPixel.h>
#include <HSBColor.h>

#define PIN               4
#define LED_COUNT         120
#define INTERVAL_TIME_MIN 1

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
	//doColorTest();
	
	// doColorWheelTest();
	// doRandomColorTest();
	doPingPongTest();
	//doCollatz();
}

void doColorTest() {
	int hue = randomHue();
	// int hue = 320;
	
	uint32_t c = rgbColorFromHSB(hue, MAX_SATURATION, MAX_BRIGHTNESS);
	for ( int i = 0; i < LED_COUNT; i++ ) {
		leds.setPixelColor(i, c);
	}
	leds.show();
	delay(1000 * 60);
}

void doColorWheelTest() {
	int hue = 0;
	int hueStep = 360 / LED_COUNT;
	for ( int i = 0; i < LED_COUNT; i++ ) {
		uint32_t c = rgbColorFromHSB(hue, MAX_SATURATION, MAX_BRIGHTNESS);
		
		Serial.print("hue ");
		Serial.println(hue);
		Serial.print("color ");
		Serial.println(c, HEX);
		
		leds.setPixelColor(i, c);
		leds.show();
		
		hue = hue + hueStep;
		delay(300);
	}
}

void doRandomColorTest() {
	for ( int i = 0; i < LED_COUNT; i++ ) {
		int hue = random(0, 360);
		uint32_t c = rgbColorFromHSB(hue, MAX_SATURATION, MAX_BRIGHTNESS);
		
		leds.setPixelColor(i, c);
		leds.show();
		
		delay(300);
	}
}

void doPingPongTest() {
	int hue = 0;
        int d = 25;
	
	hue = random(0, 360);
	for ( int i = 0; i < LED_COUNT; i++ ) {
		uint32_t c = rgbColorFromHSB(hue, MAX_SATURATION, MAX_BRIGHTNESS);
		
		leds.setPixelColor(i, c);
		
		if ( i > 0 ) {
			leds.setPixelColor(i-1, 0);
		}
		
		leds.show();
		delay(d);
	}
	hue = random(0, 360);
	for ( int i = LED_COUNT-1; i >= 0; i-- ) {
		uint32_t c = rgbColorFromHSB(hue, MAX_SATURATION, MAX_BRIGHTNESS);
		
		leds.setPixelColor(i, c);
		
		if ( i < LED_COUNT - 1 ) {
			leds.setPixelColor(i+1, 0);
		}
		
		leds.show();
		delay(d);
	}
}

void doCollatz() {
	// [1, LED_COUNT]
	long base = random(1, LED_COUNT+1);
	int collatz[LED_COUNT];
	int maxCollatz = 0;

	// choose a random hue
	uint8_t hue = randomHue();
	Serial.println(hue);

	// flash the starting seed
	flashLEDs(base, hue, MAX_BRIGHTNESS, 1, 1500);

	// compute collatz values for each LED pixel
	for ( int i = 0; i < LED_COUNT; i++ ) {
		collatz[i] = computeCollatz(base + i);

		if ( collatz[i] > maxCollatz ) {
			maxCollatz = collatz[i];
		}
	}

	// flash the starting seed
	flashLEDs(base, hue, MAX_BRIGHTNESS, 1, 1500);
	clearLEDs();

	// set the LEDs one by one to the collatz values
	for ( int i = 0; i < LED_COUNT; i++ ) {
		// brightness is proportional to the normalized collatz value
		float norm = collatz[i] / maxCollatz;
		int bright = (int) (norm * MAX_BRIGHTNESS);
		uint8_t c = rgbColorFromHSB(hue, MAX_SATURATION, bright);

		leds.setPixelColor(i, c);
		leds.show();

		delay(200);
	}
}

void clearLEDs() {
	for ( int i = 0; i < LED_COUNT; i++ ) {
		leds.setPixelColor(i, 0);
	}
}

void flashLEDs(int numLEDs, int hue, int brightness, int numFlashes, int pauseMsec) {
	// turn all LEDs off
	clearLEDs();
	leds.show();
	
	// full brightness for the hue
	uint8_t c = rgbColorFromHSB(hue, 0, brightness);
	
	// flash N times
	for ( int i = 0; i < numFlashes; i++ ) {
		// turn on numLEDs
		for ( int i = 0; i < numLEDs; i++ ) {
			leds.setPixelColor(i, c);
		}
		leds.show();
		delay(pauseMsec);
		
		// turn them off for the next iteration
		clearLEDs();
		leds.show();
		delay(pauseMsec);
	}
}

void flashLEDsColor(int numLEDs, uint8_t color, int numFlashes, int pauseMsec) {
	// turn all LEDs off
	clearLEDs();
	leds.show();

	// flash N times
	for ( int i = 0; i < numFlashes; i++ ) {
		// turn on numLEDs
		for ( int i = 0; i < numLEDs; i++ ) {
			leds.setPixelColor(i, color);
		}
		leds.show();
		delay(pauseMsec);

		// turn them off for the next iteration
		clearLEDs();
		leds.show();
		delay(pauseMsec);
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

int computeCollatz(int k) {
	int count = 0;
	while ( k > 1 ) {
		if ( k % 2 == 0 ) {
			k = k / 2;
		} else {
			k = 3 * k + 1;
		}
		count = count + 1;
	}
	
	return count;
}


