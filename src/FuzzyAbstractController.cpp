#include "FuzzyAbstractController.h"

extern FuzzyLogger logger;

FuzzyAbstractController::FuzzyAbstractController() {
}

void FuzzyAbstractController::setup(FuzzyEngine* _engine, FuzzyGyro* _gyro) {
  engine = _engine;
  gyro = _gyro;
  previousPitch = 0;
  currentPitch = 0;
  pitchDifference = 0;
}

void FuzzyAbstractController::update() {
  gyro -> update();
  currentPitch = (int)(gyro -> getPitch() * 180 / M_PI);
  pitchDifference = currentPitch - previousPitch;

  react();

  previousPitch = currentPitch;
}
