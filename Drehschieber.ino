/* Servosteuerung für Drehschieber Display Version vom 6.11.2025 Bernhard Pleschko (Basierend auf sketch von Tillo!)

 Es wird 1 Servomotor DM996 angesteuert. Es gibt 2 Zustände:
 Schieber  offen
 Schieber  Zu - 90 Grad Drehung des Schiebers

 Der jeweilige Zustand wird über Knöpfe gesteuert, evtl. an die realisierte Verdrahtung anpassen
 */

#include <ESP32Servo.h> // madhephaestus/ESP32Servo@^3.0.9
#include <Button2.h>    // lennarthennigs/Button2 @ ^2.4.1

Servo dsServo; // servo Objekt erstellen um Motor zu steuern

Button2 open1Button; // Button2 Objekte erstellen, die die Abläufe pro Knopfdruck festlegen
Button2 close1Button;

int pos = 0;        // variable für die Servowinkelposition
int servoPin1 = 13;  // servo motor 1 mit pin D13 am ESP32 verbinden

int open1ButtonPin = 25;
int close1ButtonPin = 27;

int debounceTime = 50; // Entprellzeit für die Knöpfe in ms
int servoDelay = 20;   // Verzögerung zwischen den Servobewegungen in ms

// Button Handler Funktionen
void open1ButtonHandler(Button2 &open1Button)
{
  Serial.println("Schieber öffnen");
  for (pos = 90; pos >= 0; pos -= 1)
  {                       // Bewegung des servo von 90 Grad bis 0 Grad in Schritten von 1 Grad
    dsServo.write(pos); // bewegt den servo zur aktuell festgelegten Position
    delay(servoDelay);    // wartet 20 ms bis zum nächsten Schritt, anpassen um die Bewegungsgeschwindigkeit zu steuern
  }
}

void close1ButtonHandler(Button2 &close1Button)
{
  Serial.println("Schieber schliessen");
  for (pos = 0; pos <= 90; pos += 1)
  {                       // Bewegung des servo von 0 Grad bis 90 Grad in Schritten von 1 Grad
    dsServo.write(pos); // bewegt den servo zur aktuell festgelegten Position
    delay(servoDelay);    // wartet 20 ms bis zum nächsten Schritt, anpassen um die Bewegungsgeschwindigkeit zu steuern
  }
}

void setup()
{
  Serial.begin(9600);

  dsServo.setPeriodHertz(50);           // standard 50 hz servo frequenz
  dsServo.attach(servoPin1, 544, 2400); // verbindet und aktiviert den Servo an servoPin1 zum servo objekt 1 mit pulse width min 544us und max 2400us
  dsServo.write(0);                   // Anfangshaltepunkt im offenen Zustand
  delay(2000);

  // Button2 initialisieren
  open1Button.begin(open1ButtonPin, INPUT_PULLUP);
  // Die Handler Funktion dem PressedHandler zuweisen.
  // Die Button2 Bibliothek hat auch Handler für Released, Clicked, LongClicked etc.
  open1Button.setPressedHandler(open1ButtonHandler);
  // Entprellzeit setzen
  open1Button.setDebounceTime(debounceTime);

  close1Button.begin(close1ButtonPin, INPUT_PULLUP);
  close1Button.setPressedHandler(close1ButtonHandler);
  close1Button.setDebounceTime(debounceTime);

  delay(1000);
  Serial.println("Servosteuerung gestartet");
}

void loop()
{
  // In der loop Funktion werden nur die Button2 Objekte abgefragt
  open1Button.loop();
  close1Button.loop();
}
