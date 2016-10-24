#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 50

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806, define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 6

int RELAY1 = 8;
int RELAY2 = 9;

int pirPin = 7;

// Define the array of leds
CRGB leds[NUM_LEDS];

void relaySetup() {
  // put your setup code here, to run once:
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);

  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, HIGH);
}

void pirSetup() {
  pinMode(pirPin, INPUT);
}

void setup() { 
  Serial.begin(57600);
  Serial.println("resetting");
  LEDS.addLeds<WS2811,DATA_PIN,RGB>(leds,NUM_LEDS);
  LEDS.setBrightness(84);
  relaySetup();
  pirSetup();
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

void set_relay() {
  Serial.println("Relay set");
  digitalWrite(RELAY1, LOW);
}

void reset_relay() {
  Serial.println("Relay reset");
  digitalWrite(RELAY1, HIGH);
}

void flash_word_and_relay(String str) {
  Serial.println(str);

  for (int flash=0; flash < 40; flash++) {
    for(int x = 0; x < str.length(); x++) {
      show_letter(str.charAt(x));
    }
    delay(50);

    if (flash % 4 == 0) {
      set_relay();
    }
  
    for(int x = 0; x < str.length(); x++) {
      clear_letter(str.charAt(x));
    }
    delay(50);

    if (flash % 4 == 0) {
      reset_relay();
    }
  }
}

void randomLoop() {
  int z = random (0, 6);
    
  switch(z) {
    case 0:
      blackout();
      fadein_word("run");
      flash_word_and_relay("run");
      break;
    case 1:
      blackout();
      fadein_word("help");
      flash_word_and_relay("help");
      break;      
    case 2:
      blackout();
      fadein_word("boo");
      flash_word_and_relay("boo");
      break;      
    case 3:
      flash_word("abcdefghijklmnopqrstuvwxy");
      break;      
    default: 
      color_loop();
  }
}

void motion_triggered() {
  Serial.println("Movement detected");

  // Show "run"
  fadein_word("run");

  // Flash "run" and relay
  flash_word_and_relay("run");

  // Now run our random loop a few times
  for (int i=0; i < 1; i++) {
    randomLoop();
  }
  
  blackout();
  reset_relay();
}

bool was_motion_detected() {
  if (digitalRead(pirPin) == HIGH) {
    return true;
  }
  else {
    return false;
  }
}

void loop() {
  if (was_motion_detected()) {
    motion_triggered();
  }
  else {
    Serial.println("Movement not detected");
  }

  delay(200);
}

