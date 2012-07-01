#include "FuzzyEngine.h"
#include "FuzzyLogger.h"

extern FuzzyLogger logger;

FuzzyEngine::FuzzyEngine(int _frontPin, int _backPin, int _leftPin, int _rightPin) {
  frontPin = _frontPin;
  backPin = _backPin;
  leftPin = _leftPin;
  rightPin = _rightPin;
}

void FuzzyEngine::setup(int _low, int _high) {
  front.attach(frontPin);
  back.attach(backPin);
  left.attach(leftPin);
  right.attach(rightPin);

  low = _low;
  high = _high;

  logger.log("engine", "Setting high level to %d", high);

  left.writeMicroseconds(high);
  right.writeMicroseconds(high);
  front.writeMicroseconds(high);
  back.writeMicroseconds(high);

  delay(7000);

  logger.log("engine", "Setting low level to %d", low);

  left.writeMicroseconds(low);
  right.writeMicroseconds(low);
  front.writeMicroseconds(high);
  back.writeMicroseconds(high);

  delay(2000);
}

void FuzzyEngine::setFront(int speed) {
  logger.log("engine", "Setting front to %d", speed);
  setSpeed(&front, speed, &frontSpeed);
}

void FuzzyEngine::setBack(int speed) {
  setSpeed(&back, speed, &backSpeed);
}

void FuzzyEngine::setLeft(int speed) {
  setSpeed(&left, speed, &leftSpeed);
}

void FuzzyEngine::setRight(int speed) {
  setSpeed(&right, speed, &rightSpeed);
}

void FuzzyEngine::setFront(double speed) {
  setSpeed(&front, speed, &frontSpeed);
}

void FuzzyEngine::setBack(double speed) {
  setSpeed(&back, speed, &backSpeed);
}

void FuzzyEngine::setLeft(double speed) {
  setSpeed(&left, speed, &leftSpeed);
}

void FuzzyEngine::setRight(double speed) {
  setSpeed(&right, speed, &rightSpeed);
}

int FuzzyEngine::getFront() {
  return frontSpeed;
}

int FuzzyEngine::getBack() {
  return backSpeed;
}

int FuzzyEngine::getLeft() {
  return leftSpeed;
}

int FuzzyEngine::getRight() {
  return rightSpeed;
}

void FuzzyEngine::setSpeed(Servo *engine, double speed, int *speedContainer) {
  logger.log("engine", "set speed %d %d", high, low);
  int intSpeed = speed * (high - low) + low;
  setSpeed(engine, intSpeed, speedContainer);
}

void FuzzyEngine::setSpeed(Servo *engine, int speed, int *speedContainer) {
  logger.log("engine", "setting speed of %p to %d", engine, speed);
  speed = constrain(speed, low, high);
  speedContainer = &speed;
  engine -> writeMicroseconds(speed);
}
