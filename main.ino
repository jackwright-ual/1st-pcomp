#include "FastLED.h"
#define COLOR_ORDER GRB
#define NUM_STRIPS 7
#define NUM_LEDS_PER_STRIP 20
CRGB leds[NUM_STRIPS][NUM_LEDS_PER_STRIP];

int strobePin  = 9;    // Strobe Pin on the MSGEQ7
int resetPin   = 10;    // Reset Pin on the MSGEQ7
int outPin     = A1;   // Output Pin on the MSGEQ7

int level[7];          // An array to hold the values from the 7 frequency bands
int led = 0;

void setup() {
  FastLED.addLeds&lt;WS2812, 2, COLOR_ORDER>(leds[0], 18);
  FastLED.addLeds&lt;WS2812, 3, COLOR_ORDER>(leds[1], 20);
  FastLED.addLeds&lt;WS2812, 4, COLOR_ORDER>(leds[2], 20);
  FastLED.addLeds&lt;WS2812, 5, COLOR_ORDER>(leds[3], 20);
  FastLED.addLeds&lt;WS2812, 6, COLOR_ORDER>(leds[4], 20);
  FastLED.addLeds&lt;WS2812, 7, COLOR_ORDER>(leds[5], 20);
  FastLED.addLeds&lt;WS2812, 8, COLOR_ORDER>(leds[6], 18);
  
  FastLED.setBrightness(255);
  Serial.begin (9600);

  // Define our pin modes
  pinMode      (strobePin, OUTPUT);
  pinMode      (resetPin,  OUTPUT);
  pinMode      (outPin,    INPUT);
  
  pinMode      (2,    OUTPUT);
  pinMode      (3,    OUTPUT);
  pinMode      (4,    OUTPUT);
  pinMode      (5,    OUTPUT);
  pinMode      (6,    OUTPUT);
  pinMode      (7,    OUTPUT);


  // Create an initial state for our pins
  digitalWrite (resetPin,  LOW);
  digitalWrite (strobePin, LOW);
  delay        (1);

  // Reset the MSGEQ7 as per the datasheet timing diagram
  digitalWrite (resetPin,  HIGH);
  delay        (1);
  digitalWrite (resetPin,  LOW);
  digitalWrite (strobePin, HIGH);
  delay        (1);

}


void loop() {
    // Cycle through each frequency band by pulsing the strobe.
    for (int i = 0; i &lt; 7; i++) {
      digitalWrite       (strobePin, LOW);
      delayMicroseconds  (100);                    // Delay necessary due to timing diagram
      level[i] =         analogRead (outPin);
      digitalWrite       (strobePin, HIGH);
      delayMicroseconds  (100);                    // Delay necessary due to timing diagram
    }

    for (int i = 0; i &lt; 7; i++) {
      Serial.print       (level[i]);
      Serial.print       ("   ");
      int r = 0;
      int g = 0;
      int b = 0;
      for (int j = 20; j >= 0; j--) {
        int startLED = i * 20;
        int endLED = (i * 20) + 20;
        int multiplier = 255;
        int val = level[i] / multiplier * (j + 1);
        if (i == 0 || i == 3 || i == 6) {
          if (i == 3) {
            leds[i][j] = CRGB(val, val, 0);
          } else {
            leds[i][j] = CRGB(val, 0, 0);
          }
        } else if (i == 1 || i == 4 || i == 7) {
          if (i == 1) {
            leds[i][j] = CRGB(val, 0, val);
          } else {
            leds[i][j] = CRGB( 0,  val, 0);
          }
          if (i == 4) {
            leds[i][j] = CRGB( 0,  val, val);
          }
        } else if (i == 2 || i == 5) {
          if (i == 5) {
            leds[i][j] = CRGB(val, 0,  val);
          } else {
            leds[i][j] = CRGB( 0, 0,  val);
          }
        }
      }
    }

    Serial.println();
    FastLED.show();
  }
