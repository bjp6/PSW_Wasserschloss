/* Bernhard Pleschko, Makers im Zigerschlitz, 18.4.2025
Animation für das Wasserschloss mit Arduino Uno R3
Es werden 4 Fälle gezeigt, gesteuert durch HIGH Signal auf dem jeweiligen Pin:
* Input 1 = Wasser schwankt nach oben 
* Input 2 = Wasser schwankt nach unten 
* Input 3 = Wasser schwankt stark nach oben 
* Input 4 = Wasser schwankt stark nach unten
Dazwischen wird jeweils dasselbe Ruheniveau eingestellt.
*/
#include <FastLED.h>

#define LED_PIN     8           // Pin für Data line der WS2812B LED matrix - gelbes Kabel
#define WIDTH       8           // Matrix Breite (8 LEDs pro Reihe)
#define HEIGHT      64         // Matrix Höhe (32 Reihen)
#define NUM_LEDS    WIDTH * HEIGHT  // Gesamte Anzahl LEDs

#define IN1_PIN     9           // Pin für Input 1, grünes Kabel 1
#define IN2_PIN     10          // Pin für Input 2, grünes Kabel 2
#define IN3_PIN     11          // Pin für Input 3, grünes Kabel 3
#define IN4_PIN     12          // Pin für Input 4, grünes Kabel 4

CRGB leds[NUM_LEDS];            // Array to hold LED data
#define BRIGHTNESS  150          // anpassen um LED Matrix heller oder dunkler leuchten zu lassen (Brightness level (0-255))
#define DELAY_TIME  22          // Wartezeit zwischen dem ein und ausschalten einzelner LED Reihen (in milliseconds) - anpassen um Animation flüssiger oder ruckeliger zu machen

float alpha;                    // Winkel in radians für sinus Schwingung
int wsNiveau_Ruhe;              // Wasserniveu im Ruhezustand: Ruhe_niveau
int schwingungsHoehe;           // Amplitude für die verschiedenen cases

void setup() {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();               // alle LEDs am Start ausschalten
  FastLED.show();                // Initializieren LEDs im OFF status
}

void loop() {
  wsNiveau_Ruhe = 45;               //damit man die Linie gut sieht wird Wasserniveu im Ruhezustand auf Reihe 45 eingestellt,anpassen wenn es nicht gut aussieht

  //nach dem Einschalten Ruhe_niveau einschalten: Matrix bis wsNiveauRuhe blau beleuchten
  for (int row = 0; row <= wsNiveau_Ruhe; row++) {
    lightRow(row);
  }
  delay(5000); // Pause um default zu zeigen

  // case 1: Input 1 ist HIGH: Schwingung nach oben sanft
  schwingungsHoehe = 10;
  dampenedOscillation_up(schwingungsHoehe);
  delay(5000);

  // case 2: Input 2 ist HIGH: Schwingung nach unten sanft
  schwingungsHoehe = 10;
  dampenedOscillation_down(schwingungsHoehe);
  delay(5000);
  
  // case 3: Input 3 ist HIGH: Schwingung nach oben stark
  schwingungsHoehe = 18;
  dampenedOscillation_up(schwingungsHoehe);
  delay(5000);

  // case 4: Input 4 ist HIGH: Schwingung nach unten stark
  schwingungsHoehe = 18;
  dampenedOscillation_down(schwingungsHoehe);
  delay(5000);
}

// Function um eine spezifische Reihe gezielt blau leuchten zu lassen
void lightRow(int row) {
  for (int col = 0; col < WIDTH; col++) {
    int ledIndex = (row * WIDTH) + col;
    leds[ledIndex] = CRGB::Blue;   // LED Farbe blau einstellen, Farbe falls nötig hier verändern!
  }
  FastLED.show();                   // Update LEDs um die Veränderung zu zeigen
}

// Function um eine spezifische Reihe gezielt auszuschalten
void clearRow(int row) {
  for (int col = 0; col < WIDTH; col++) {
    int ledIndex = (row * WIDTH) + col;
    leds[ledIndex] = CRGB::Black;   // LED ausschalten
  }
  FastLED.show();                   // Update LEDs um die Veränderung zu zeigen
}

// Function für gedämpfte Schwingung um das Ruheniveau beginnend mit Abfall
void dampenedOscillation_down(int input_amplitude) {
  float alpha = 0.0; // start bei max. Amplitude, Winkel 0, cos = 1
  float amplitude = input_amplitude;   // Start Amplitude für Schwingung
  while (amplitude > 0) {
    int highLevel = wsNiveau_Ruhe + amplitude;
    int lowLevel = wsNiveau_Ruhe - amplitude;
    // Abfall um die Amplitude
    for (int row = wsNiveau_Ruhe; row >= lowLevel; row--) {
      clearRow(row);
      delay(DELAY_TIME);
    }
    // Anstieg um die Amplitude
    for (int row = lowLevel; row <= wsNiveau_Ruhe; row++) {
      lightRow(row);
      delay(DELAY_TIME);
    }
    //Anstieg bis maximales Niveau
    for (int row = wsNiveau_Ruhe; row <= highLevel; row++) {
      lightRow(row);
      delay(DELAY_TIME);
    }
    // Abfall um die Amplitude
    for (int row = highLevel; row >= wsNiveau_Ruhe; row--) {
      clearRow(row);
      delay(DELAY_TIME);
    }
    alpha = alpha+0.1;
    amplitude = amplitude*cos(alpha);
  }
  //das Ruhe Niveau sicherstellen
  for (int row = 0; row <= wsNiveau_Ruhe; row++) {
    lightRow(row);
  }
}

// Function für gedämpfte Schwingung um das Ruheniveau beginnend mit Anstieg
void dampenedOscillation_up(int input_amplitude) {
  float alpha = 0.0; // start bei max. Amplitude, Winkel 0, cos = 1
  float amplitude = input_amplitude;   // Start Amplitude für Schwingung
  while (amplitude > 0) {
    int highLevel = wsNiveau_Ruhe + amplitude;
    int lowLevel = wsNiveau_Ruhe - amplitude;
    // Anstieg um die Amplitude
    for (int row = wsNiveau_Ruhe; row <= highLevel; row++) {
      lightRow(row);
      delay(DELAY_TIME);
    }
    // Abfall um die Amplitude bis Ruheniveau
    for (int row = highLevel; row >= wsNiveau_Ruhe; row--) {
      clearRow(row);
      delay(DELAY_TIME);
    }
    // Abfall um die Amplitude auf minimales Niveau
    for (int row = wsNiveau_Ruhe; row >= lowLevel; row--) {
      clearRow(row);
      delay(DELAY_TIME);
    }
    //Anstieg um Amplitude auf Ruheniveau
    for (int row = lowLevel; row <= wsNiveau_Ruhe; row++) {
      lightRow(row);
      delay(DELAY_TIME);
    }
    alpha = alpha+0.1;
    amplitude = amplitude*cos(alpha);
  }
  //das Ruhe Niveau sicherstellen
  for (int row = 0; row <= wsNiveau_Ruhe; row++) {
    lightRow(row);
  }
}
