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
  lastReactionMillis = 0;
  reactInterval = 0; //ms
}

void FuzzyAbstractController::update() {
  unsigned long now = millis();

  gyro -> update();
  currentPitch = (int)(gyro -> getPitch() * 1800 / M_PI);
  pitchDifference = currentPitch - previousPitch;
  if (now - lastReactionMillis >= reactInterval) {
    react();
    lastReactionMillis = now;
  }

  previousPitch = currentPitch;
}

void FuzzyAbstractController::stop() {
  engine -> stop();
}

void FuzzyAbstractController::setReactionInterval(unsigned int interval) {
  logger.log("ctrl", "Setting reaction interval %d", interval);
  reactInterval = interval;
}
