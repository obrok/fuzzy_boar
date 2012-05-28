#include <Servo.h>

Servo myservo;

void setup() {
	myservo.attach(3);
	Serial.begin(9600);
}

void loop() {
	if (Serial.available() > 0) {
		int b = Serial.read();
		myservo.write(b);
		Serial.println(b, DEC);
	}
}
