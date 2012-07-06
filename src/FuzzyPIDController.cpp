#include "FuzzyPIDController.h"

extern FuzzyLogger logger;

void FuzzyPIDController::react() {
  logger.log("pid", "reacting");
}
