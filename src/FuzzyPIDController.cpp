#include "FuzzyPIDController.h"

extern FuzzyLogger logger;

void FuzzyPIDController::react() {
  logger.log("controller", "pitch: %d, diff: %d", currentPitch, pitchDifference);
}
