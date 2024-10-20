#include <FastLED.h>

#define LED_PIN     8           // Pin connected to the data line of the WS2812B matrix
#define WIDTH       8           // Matrix width (8 LEDs per row)
#define HEIGHT      64         // Matrix height (32 rows)
#define NUM_LEDS    WIDTH * HEIGHT  // Total number of LEDs

CRGB leds[NUM_LEDS];            // Array to hold LED data
#define BRIGHTNESS  150          // Brightness level (0-255)
#define DELAY_TIME  22          // Delay between each row turning on/off (in milliseconds)

int dampFactor;         // Dampening factor for oscillation (smaller number = more dampening)
float alpha;             // Winkel in radians für sinus Schwingung
int wsNormlevel;
int oscLowlevel;      //untere Grenze der Oszillation   
int oscHighlevel;     //obere Grenze der Oszilattion
int wsKsopen;         // Wasserschloss Level wenn der Kugelschieber offen ist

void setup() {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();               // Clear all LEDs to start
  FastLED.show();                // Initialize LEDs in the OFF state
}

void loop() {
  wsNormlevel = HEIGHT-5;  //WS_level ist 5.0 m unter Muttsee
  wsKsopen = 45;           // nach dem Öffnen der KS schwingt sich das System auf diese Niveau ein

  //case 1 Kugelschieber geschlossen, 
  // Matrix bis wsNormlevel blau beleuchten
  for (int row = 0; row < wsNormlevel; row++) {
    lightRow(row);
  }
  delay(5000); // wait to demonstrate case 1

  // case 2: Kugelschieber wird geöffnet, Abfall zu oscLowlevel und gedämpfte Schwingung
  dampenedOscillation(wsKsopen);
  delay(5000);
}

// Function to light up a specific row with blue color
void lightRow(int row) {
  for (int col = 0; col < WIDTH; col++) {
    int ledIndex = (row * WIDTH) + col;
    leds[ledIndex] = CRGB::Blue;   // Set the LED color to blue
  }
  FastLED.show();                   // Update the LEDs to show the changes
}

// Function to turn off a specific row
void clearRow(int row) {
  for (int col = 0; col < WIDTH; col++) {
    int ledIndex = (row * WIDTH) + col;
    leds[ledIndex] = CRGB::Black;   // Turn off the LED
  }
  FastLED.show();                   // Update the LEDs to show the changes
}

// Function to implement dampened oscillation
void dampenedOscillation(int midLevel) {
  int startLevel = wsNormlevel;
  alpha = 0; // start bei max. Amplitude, Winkel 0, cos = 1

  float amplitude = startLevel - midLevel;   // Initial amplitude for the oscillation
  
  while (amplitude > 0) {
    // Drop by the amplitude
    for (int row = startLevel; row >= midLevel-amplitude; row--) {
      clearRow(row);
      delay(DELAY_TIME);
    }
    startLevel = midLevel - amplitude;
    alpha = alpha+0.1;
    amplitude = amplitude*cos(alpha);
    // Rise up by the amplitude
    for (int row = startLevel; row < midLevel+amplitude; row++) {
      lightRow(row);
      delay(DELAY_TIME);
    }
    startLevel =  midLevel + amplitude;
    // Reduce amplitude by the dampening factor
    amplitude = amplitude * cos(alpha);
  }
}
