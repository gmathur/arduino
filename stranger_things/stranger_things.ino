#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 50

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806, define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 6

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() { 
  Serial.begin(57600);
  Serial.println("resetting");
  LEDS.addLeds<WS2811,DATA_PIN,RGB>(leds,NUM_LEDS);
  LEDS.setBrightness(84);
}

void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } }

void color_loop() {
  static uint8_t hue = 0;
  Serial.println("running color loop");
  // First slide the led in one direction
  for(int i = 0; i < NUM_LEDS; i++) {
    // Set the i'th led to red 
    leds[i] = CHSV(hue++, 255, 255);
    // Show the leds
    FastLED.show(); 
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(10);
  }

  // Now go in the other direction.  
  for(int i = (NUM_LEDS)-1; i >= 0; i--) {
    // Set the i'th led to red 
    leds[i] = CHSV(hue++, 255, 255);
    // Show the leds
    FastLED.show();
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(10);
  }
}

void fadein_letter(char c) {
  int letter = c - 'a';
  
  // Set the i'th led to red 
  for( int i = 60; i <= 255; i+=2 ) {
    leds[letter*2] = CRGB(i, 0, 0);
    leds[letter*2+1] = CRGB(i, 0, 0);

    // Show the leds
    FastLED.show();
    delay(30);
  }

  delay(50);
}

void show_letter(char c) {
  int letter = c - 'a';
  
  // Set the i'th led to red 
  leds[letter*2] = CRGB(255, 0, 0);
  leds[letter*2+1] = CRGB(255, 0, 0);

  // Show the leds
  FastLED.show();
}

void fadeout_letter(char c) {
  int letter = c - 'a';
  
  // Set the i'th led to red 
  for( int i = 255; i >= 0; i-= 5 ) {
    leds[letter*2] = CRGB(i, 0, 0);
    leds[letter*2+1] = CRGB(i, 0, 0);
      
    // Show the leds
    FastLED.show();
    delay(30);
  }
}

void clear_letter(char c) {
  int letter = c - 'a';
  
  // Set the i'th led to red 
  leds[letter*2] = CRGB(0, 0, 0);
  leds[letter*2+1] = CRGB(0, 0, 0);

  // Show the leds
  FastLED.show();
}

void fadein_word(String str) {
  Serial.println(str);

  for(int x = 0; x < str.length(); x++) {
    fadein_letter(str.charAt(x));
    delay(400);
    fadeout_letter(str.charAt(x));
    delay(400);
  }
}

void flash_word(String str) {
  Serial.println(str);

  for (int flash=0; flash < 40; flash++) {
    for(int x = 0; x < str.length(); x++) {
      show_letter(str.charAt(x));
    }
    delay(50);
  
    for(int x = 0; x < str.length(); x++) {
      clear_letter(str.charAt(x));
    }
    delay(50);
  }
}

void blackout() {
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(0, 0, 0);
  }

  delay(500);
}

void loop() {
  int z = random (0, 6);

  String boo = "boo";
  String help = "help";
  String runString = "run";
  String flashall = "abcdefghijklmnopqrstuvwxy";  
  
  switch(z) {
    case 0:
      blackout();
      fadein_word(runString);
      flash_word(runString);
    case 1:
      blackout();
      fadein_word(help);
      flash_word(help);
    case 2:
      blackout();
      fadein_word(boo);
      flash_word(boo);
    case 3:
      flash_word(flashall);
    default: 
      color_loop();
  }
}
