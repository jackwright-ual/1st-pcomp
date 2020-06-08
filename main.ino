#include "FastLED.h"
#define NUM_LEDS 140
#define LED_PIN 4
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

int strobePin  = 2;    // Strobe Pin on the MSGEQ7
int resetPin   = 3;    // Reset Pin on the MSGEQ7
int outPin     = A0;   // Output Pin on the MSGEQ7
int ledOut     = 4;

int level[7];          // An array to hold the values from the 7 frequency bands
int led = 0;

void setup() {
  FastLED.addLeds&lt;WS2812, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
  Serial.begin (9600);

  // Define our pin modes
  pinMode      (strobePin, OUTPUT);
  pinMode      (resetPin,  OUTPUT);
  pinMode      (outPin,    INPUT);

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
      int currentLED = startLED + j;
      int endLED = (i * 20) + 20;
      int multiplier = 500;
      int val = level[i] / multiplier * (j + 1);
      //Serial.println(val);
      if (i == 0 || i == 3 || i == 6) {
        if (i == 3) {
          leds[currentLED] = CRGB(val, val, 0);
        } else {
          leds[currentLED] = CRGB(val, 0, 0);
        }
      } else if (i == 1 || i == 4 || i == 7) {
        if (i == 1) {
          /* pinkish */
          leds[currentLED] = CRGB(val, 0, val);
        } else {
          leds[currentLED] = CRGB( 0,  val, 0);
        }
        if (i == 4) {
          leds[currentLED] = CRGB( 0,  val, val);
        }
      } else if (i == 2 || i == 5) {
        if (i == 5) {
          leds[currentLED] = CRGB(val, 0,  val);
        } else {
          leds[currentLED] = CRGB( 0, 0,  val);
        }
      }
    }
  }


  Serial.println();
  FastLED.show();
}
