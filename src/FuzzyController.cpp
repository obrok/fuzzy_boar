#include "FuzzyController.h"

extern FuzzyLogger logger;

FuzzyController::FuzzyController(FuzzyEngine* _engine, FuzzyGyro* _gyro) {
  engine = engine;
  gyro = gyro;

  left = 1400;
  right = 1400;
}

void FuzzyController::update() {
  shiftAngles();

  float angle = angles[0];
  float speed = (angles[4] - angles[0]) / (float)(times[4] - times[0]) * 1000;
  float desiredSpeed = constrain(-angle, -10, 10);

  if (speed < desiredSpeed) {
    left -= 10;
    right += 10;
  } else {
    left += 10;
    right -= 10;
  }

  engine->setRight(right);
  engine->setLeft(left);
}

void FuzzyController::shiftAngles() {
  for (int i = 0; i < 3; i++) {
    angles[i + 1] = angles[i];
    times[i + 1] = times[i];
  }

  times[0] = millis();
  angles[0] = gyro->getPitch();
}
