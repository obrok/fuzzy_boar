#include "FuzzyController.h"

extern FuzzyLogger logger;

FuzzyController::FuzzyController() {
}

void FuzzyController::setup(FuzzyEngine* _engine, FuzzyGyro* _gyro) {
  engine = _engine;
  gyro = _gyro;
  previousPitch = 0;
  currentPitch = 0;
  pitchDifference = 0;
  iteration = 0;
  step = 0;
  side = true;
}

void FuzzyController::update() {
  gyro -> update();
  currentPitch = (int)(gyro -> getPitch() * 180 / M_PI);
  pitchDifference = currentPitch - previousPitch;

  if (iteration == 0) {
    react();
    iteration = 0;
  } else {
    iteration++;
  }
  logger.log("engine", "cp: %3d, pp: %3d, pd: %3d, le: %4d, ri: %4d", currentPitch, previousPitch, pitchDifference, engine->getLeft(), engine->getRight());

  previousPitch = currentPitch;
}

void FuzzyController::react() {
  if (!isEven()) {
    if (!isLeveling()) {
      initializeKick();
    } else {
      easeKick();
    }
  }
}

bool FuzzyController::isLeveling() {
  if (isRightLower()) {
    return hasRightRisen();
  } else {
    return hasLeftRisen();
  }
}

void FuzzyController::initializeKick() {
  step = abs(currentPitch * 3);
  logger.log("engine", "############################### %d", step);
  if (isRightLower()) {
    engine->changeRight(step);
    engine->changeLeft(-step);
  } else {
    engine->changeRight(-step);
    engine->changeLeft(step);
  }
}

void FuzzyController::easeKick() {
  step = step / 2;
  if (isRightLower()) {
    engine->changeRight(-step);
    engine->changeLeft(step);
  } else {
    engine->changeRight(step);
    engine->changeLeft(-step);
  }
}

void FuzzyController::stop() {
  engine -> stop();
}

bool FuzzyController::isRightLower() {
  return currentPitch > 0;
}

bool FuzzyController::isEven() {
  return currentPitch == 0;
}

bool FuzzyController::hasRightRisen() {
  return pitchDifference < 0;
}

bool FuzzyController::hasLeftRisen() {
  return pitchDifference > 0;
}

bool FuzzyController::hasSwitchedSide() {
  return previousPitch * currentPitch <= 0;
}

bool FuzzyController::changedSideSinceLastKick() {
  side != isRightLower();
}
