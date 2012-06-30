#ifndef _FUZZY_ENGINE_H
#define _FUZZY_ENGINE_H

#include "Arduino.h"
#include "Servo.h"

class FuzzyEngine {
    public:
		FuzzyEngine(int frontPin, int backPin, int leftPin, int rightPin);
		void setup(int low, int high);
		void setLeft(int speed);
		void setRight(int speed);

	private:
		Servo front;
		Servo back;
		Servo left;
		Servo right;
		int low;
		int high;
		int constrainSpeed(int speed);
};

#endif
