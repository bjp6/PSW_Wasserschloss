/* Servosteuerung für UWS Display Version vom 4.10.2025

 Es werden 2 Servomotoren DM996 angesteuert. Es gibt 4 Zustände:
 Schieber 1 offen
 Schieber 1 Zu
 Schieber 2 offen
 Schieber 2 Zu

 Der jeweilige Zustand wird über Knöpfe/ Schalter gesteuert, die genau Vorgehensweise und Ansteuerung ist noch nicht klar und auch nicht realisiert.
 */

#include <ESP32Servo.h>

Servo uwsServo1;  // servo objekte ersetllen um Motoren zu steuern
Servo uwsServo2;

int pos = 0;    // variable für die Servowinkelposition


int servoPin1 = 13;  //servo motor 1 mit pin D13 am ESP32 verbinden
int servoPin2 = 12;  //servo motor 2 mit Pin D12 am ESP32 verbinden

void setup() {

	uwsServo1.setPeriodHertz(50);    // standard 50 hz servo frequenz
	uwsServo1.attach(servoPin1, 544, 2400); // verbindet und aktiviert den Servo an servoPin1 zum servo objekt 1 mit pulse width min 544us und max 2400us
	uwsServo2.setPeriodHertz(50);    // standard 50 hz servo frequenz
	uwsServo2.attach(servoPin2, 544, 2400); // verbindet und aktiviert den Servo an servoPin2 zum servo objekt 2 mit pulse width min 544us und max 2400us
	uwsServo1.write(180); // Anfangshaltepunkt im offenen Zustand
	uwsServo2.write(0); //da die Motoren seitenverkehrt montiert sind ist ein unterschiedlicher Startpunkt nötig
	delay(2000);
}

void loop() {
	// anpassen für Realisierung mit Schaltern oder Knöpfen
	for (pos = 180; pos >= 115; pos -= 1) { // Bewegung des servo von 180 Grad bis 115 Grad in Schritten von 1 Grad, 65 Grad Winkel für vollen Schieberhub
		uwsServo1.write(pos);       // bewegt den servo zur aktuell festgelegten Position
		uwsServo2.write(180-pos);   // wegen der seitenverkejrten Montage ist diese Ansteuerung nötig um Gleichlauf zu erzielen
		delay(50);             // wartet 50 ms bis zum nächsten Schritt, anpassen um die Bewegungsgeschwindigkeit zu steuern
	}
	for (pos = 115; pos <= 180; pos += 1) { // Bewegung des servo von 180 Grad bis 0 Grad in Schritten von 1 Grad
		uwsServo1.write(pos);    // bewegt den servo zur aktuell festgelegten Position
		uwsServo2.write(180-pos);
		delay(50);             // wartet 50 ms bis zum nächsten Schritt, anpassen um die Bewegungsgeschwindigkeit zu steuern
	}
}

