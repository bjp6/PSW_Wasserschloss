#include <FastLED.h>

#define LED_PIN     8           // Pin connected to the data line of the WS2812B matrix
#define WIDTH       8           // Matrix width (8 LEDs per row)
#define HEIGHT      64         // Matrix height (32 rows)
#define NUM_LEDS    WIDTH * HEIGHT  // Total number of LEDs

CRGB leds[NUM_LEDS];            // Array to hold LED data
#define BRIGHTNESS  100          // Brightness level (0-255)
#define DELAY_TIME  50          // Delay between each row turning on/off (in milliseconds)
#define DAMP_FACTOR 0.9          // Dampening factor for oscillation (smaller number = more dampening)

int mutt_50;
int currentLevel;
int levelInitdropKSA = 5;
int levelTodrop = 10;             // Percentage of the full height (initially 100%)
int rowToClearTo;                 // Number of rows to turn off based on drop level
       // Current level of LEDs
bool oscillating = false;        // Flag for oscillation state

void setup() {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();               // Clear all LEDs to start
  FastLED.show();                // Initialize LEDs in the OFF state
}

void loop() {
  mutt_50 = HEIGHT*0.7;
  currentLevel = mutt_50;

  //case 1 Kugelschieber geschlossen, WS_level ist 50 m unter Muttsee
  // Light up the matrix in blue up to mutt_50
  for (int row = 0; row < mutt_50; row++) {
    lightRow(row);
  }
  delay(10000); // wait to demonstrate case 1

  // case 2: Kugelschieber wird geöffnet, drop to the initial low level
  // Calculate the number of rows to clear based on the percentage
  rowToClearTo = levelInitdropKSA;
  dropToLevel(rowToClearTo);
  delay(DELAY_TIME); //hold at the low position

  // then begin oscillation
  oscillating = true;
  dampenedOscillation();
  delay(10000);
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

// Function to drop LEDs from the current level to the desired level
void dropToLevel(int targetRowsToClear) {
  for (int row = currentLevel - 1; row >= targetRowsToClear; row--) {
    clearRow(row);
    delay(DELAY_TIME);
  }
  currentLevel = targetRowsToClear;   // Update the current level after the drop
}

// Function to implement dampened oscillation
void dampenedOscillation() {
  int amplitude = (HEIGHT - rowToClearTo) / 2;   // Initial amplitude for the oscillation

  while (amplitude > 0) {
    // Rise up by the amplitude
    for (int row = currentLevel; row < currentLevel + amplitude && row < HEIGHT; row++) {
      lightRow(row);
      delay(DELAY_TIME);
    }

    // Fall back by the amplitude
    for (int row = currentLevel + amplitude - 1; row >= currentLevel; row--) {
      clearRow(row);
      delay(DELAY_TIME);
    }

    // Reduce amplitude by the dampening factor
    amplitude = amplitude * DAMP_FACTOR;
  }

  // Once the oscillation dampens, rise back up to the target level
  for (int row = currentLevel; row < rowToClearTo; row++) {
    lightRow(row);
    delay(DELAY_TIME);
  }
  currentLevel = rowToClearTo;  // Set current level to the final level
  oscillating = false;         // End oscillation
}
