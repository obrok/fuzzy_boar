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
}

void FuzzyEngine::setAll(int speed) {
  setFront(speed);
  setBack(speed);
  setLeft(speed);
  setRight(speed);
}

int FuzzyEngine::setFront(int speed) {
  return setSpeed(&front, speed, &frontSpeed);
}

int FuzzyEngine::setBack(int speed) {
  return setSpeed(&back, speed, &backSpeed);
}

int FuzzyEngine::setLeft(int speed) {
  return setSpeed(&left, speed, &leftSpeed);
}

int FuzzyEngine::setRight(int speed) {
  return setSpeed(&right, speed, &rightSpeed);
}

double FuzzyEngine::setFront(double speed) {
  return setSpeed(&front, speed, &frontSpeed);
}

double FuzzyEngine::setBack(double speed) {
  return setSpeed(&back, speed, &backSpeed);
}

double FuzzyEngine::setLeft(double speed) {
  return setSpeed(&left, speed, &leftSpeed);
}

double FuzzyEngine::setRight(double speed) {
  return setSpeed(&right, speed, &rightSpeed);
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

double FuzzyEngine::setSpeed(Servo *engine, double speed, int *speedContainer) {
  int intSpeed = speed * (high - low) + low;
  return (double)setSpeed(engine, intSpeed, speedContainer);
}

int FuzzyEngine::setSpeed(Servo *engine, int speed, int *speedContainer) {
  speed = constrain(speed, low, 1300);
  *speedContainer = speed;
  engine -> writeMicroseconds(speed);
  return speed;
}

void FuzzyEngine::stop() {
  setLeft(0.0);
  setRight(0.0);
  setFront(0.0);
  setBack(0.0);
}

void FuzzyEngine::changeRight(int delta) {
  setRight(getRight() + delta);
}

void FuzzyEngine::changeLeft(int delta) {
  setLeft(getLeft() + delta);
}
