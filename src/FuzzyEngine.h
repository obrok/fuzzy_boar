#ifndef _FUZZY_ENGINE_H
#define _FUZZY_ENGINE_H

#include "Arduino.h"
#include "Servo.h"

class FuzzyEngine {
    public:
		FuzzyEngine(int frontPin, int backPin, int leftPin, int rightPin);
		void setup(int low, int high);
		void setFront(int speed);
		void setBack(int speed);
		void setLeft(int speed);
		void setRight(int speed);

		int getFront();
		int getBack();
		int getLeft();
		int getRight();

	private:
		Servo front;
		Servo back;
		Servo left;
		Servo right;
		int low;
		int high;
		int frontSpeed;
		int backSpeed;
		int leftSpeed;
		int rightSpeed;
		void setSpeed(Servo *servo, int speed, int *speedContainer);
};

#endif
