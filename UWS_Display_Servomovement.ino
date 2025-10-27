/* Servosteuerung für UWS Display Version vom 4.10.2025

 Es werden 2 Servomotoren DM996 angesteuert. Es gibt 4 Zustände:
 Schieber 1 offen
 Schieber 1 Zu
 Schieber 2 offen
 Schieber 2 Zu

 Der jeweilige Zustand wird über Knöpfe/ Schalter gesteuert, die genau Vorgehensweise und Ansteuerung ist noch nicht klar und auch nicht realisiert.
 */

#include <ESP32Servo.h> // madhephaestus/ESP32Servo@^3.0.9
#include <Button2.h>    // lennarthennigs/Button2 @ ^2.4.1

Servo uwsServo1; // servo objekte ersetllen um Motoren zu steuern
Servo uwsServo2;

Button2 open1Button; // Button2 Objekte erstellen
Button2 close1Button;
Button2 open2Button;
Button2 close2Button;

int pos = 0;        // variable für die Servowinkelposition
int servoPin1 = 2;  // servo motor 1 mit pin D13 am ESP32 verbinden
int servoPin2 = 12; // servo motor 2 mit Pin D12 am ESP32 verbinden

int open1ButtonPin = 25;
int close1ButtonPin = 27;
int open2ButtonPin = 26;
int close2ButtonPin = 22;

int debounceTime = 50; // Entprellzeit für die Knöpfe in ms
int servoDelay = 20;   // Verzögerung zwischen den Servobewegungen in ms

// Button Handler Funktionen
void open1ButtonHandler(Button2 &open1Button)
{
  Serial.println("Schieber 1 öffnen");
  for (pos = 180; pos >= 115; pos -= 1)
  {                       // Bewegung des servo von 180 Grad bis 115 Grad in Schritten von 1 Grad, 65 Grad Winkel für vollen Schieberhub
    uwsServo1.write(pos); // bewegt den servo zur aktuell festgelegten Position
    delay(servoDelay);    // wartet 20 ms bis zum nächsten Schritt, anpassen um die Bewegungsgeschwindigkeit zu steuern
  }
}

void close1ButtonHandler(Button2 &close1Button)
{
  Serial.println("Schieber 1 schliessen");
  for (pos = 115; pos <= 180; pos += 1)
  {                       // Bewegung des servo von 115 Grad bis 180 Grad in Schritten von 1 Grad
    uwsServo1.write(pos); // bewegt den servo zur aktuell festgelegten Position
    delay(servoDelay);    // wartet 20 ms bis zum nächsten Schritt, anpassen um die Bewegungsgeschwindigkeit zu steuern
  }
}

void open2ButtonHandler(Button2 &open2Button)
{
  Serial.println("Schieber 2 öffnen");
  for (pos = 0; pos <= 65; pos += 1)
  {                       // Bewegung des servo von 0 Grad bis 65 Grad in Schritten von 1 Grad, 65 Grad Winkel für vollen Schieberhub
    uwsServo2.write(pos); // bewegt den servo zur aktuell festgelegten Position
    delay(servoDelay);    // wartet 20 ms bis zum nächsten Schritt, anpassen um die Bewegungsgeschwindigkeit zu steuern
  }
}

void close2ButtonHandler(Button2 &close2Button)
{
  Serial.println("Schieber 2 schliessen");
  for (pos = 65; pos >= 0; pos -= 1)
  {                       // Bewegung des servo von 65 Grad bis 0 Grad in Schritten von 1 Grad
    uwsServo2.write(pos); // bewegt den servo zur aktuell festgelegten Position
    delay(servoDelay);    // wartet 20 ms bis zum nächsten Schritt, anpassen um die Bewegungsgeschwindigkeit zu steuern
  }
}

void setup()
{
  Serial.begin(9600);

  uwsServo1.setPeriodHertz(50);           // standard 50 hz servo frequenz
  uwsServo1.attach(servoPin1, 544, 2400); // verbindet und aktiviert den Servo an servoPin1 zum servo objekt 1 mit pulse width min 544us und max 2400us
  uwsServo2.setPeriodHertz(50);           // standard 50 hz servo frequenz
  uwsServo2.attach(servoPin2, 544, 2400); // verbindet und aktiviert den Servo an servoPin2 zum servo objekt 2 mit pulse width min 544us und max 2400us
  uwsServo1.write(180);                   // Anfangshaltepunkt im offenen Zustand
  uwsServo2.write(0);                     // da die Motoren seitenverkehrt montiert sind ist ein unterschiedlicher Startpunkt nötig
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

  open2Button.begin(open2ButtonPin, INPUT_PULLUP);
  open2Button.setPressedHandler(open2ButtonHandler);
  open2Button.setDebounceTime(debounceTime);

  close2Button.begin(close2ButtonPin, INPUT_PULLUP);
  close2Button.setPressedHandler(close2ButtonHandler);
  close2Button.setDebounceTime(debounceTime);

  delay(1000);
  Serial.println("Servosteuerung gestartet");
}

void loop()
{
  // In der loop Funktion werden nur die Button2 Objekte abgefragt
  open1Button.loop();
  close1Button.loop();
  open2Button.loop();
  close2Button.loop();
}
