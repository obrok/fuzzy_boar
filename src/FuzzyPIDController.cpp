#include "FuzzyPIDController.h"

extern FuzzyLogger logger;

void FuzzyPIDController::setup(FuzzyEngine* _engine, FuzzyGyro* _gyro) {
  FuzzyAbstractController::setup(_engine, _gyro);
  setReactionInterval(50);
  errorSum = 0.0;
  lastError = 0.0;
  kp = 6.0;
  kd = 0.6;
  ki = 0.2;
}

// input: input angle
// output: output angle
// setpoint: desired angle
void FuzzyPIDController::react() {
  double input = (double)currentPitch;
  double setpoint = 0.0;
  double error = setpoint - input;
  errorSum += error;
  double errorChange = error - lastError;
  double output = kp * error + ki * errorSum + kd * errorChange;
  lastError = error;
  logger.log("ctrl", "cp: %d, ang: %d ri: %d", currentPitch, (int)output, reactInterval);
}
