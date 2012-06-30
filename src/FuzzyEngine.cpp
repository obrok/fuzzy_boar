#include "FuzzyEngine.h"

FuzzyEngine::FuzzyEngine(int frontPin, int backPin, int leftPin, int rightPin) {
	front.attach(frontPin);
	back.attach(backPin);
	left.attach(leftPin);
	right.attach(rightPin);
}

void FuzzyEngine::setup(int _low, int _high) {
	low = _low;
	high = _high;

	left.writeMicroseconds(high);
	right.writeMicroseconds(high);

	delay(4000);

	left.writeMicroseconds(low);
	right.writeMicroseconds(low);

	delay(2000);
}

void FuzzyEngine::setLeft(int speed) {
	left.writeMicroseconds(constrainSpeed(speed));
}

void FuzzyEngine::setRight(int speed) {
	right.writeMicroseconds(constrainSpeed(speed));
}

int FuzzyEngine::constrainSpeed(int speed) {
	constrain(speed, low, high);
}
