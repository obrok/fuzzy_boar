#include "FuzzyPIDController.h"
#include "Indexes.h"

extern FuzzyLogger logger;

void FuzzyPIDController::setup(FuzzyEngine* _engine, FuzzyGyro* _gyro, FuzzyCom *_com) {
  FuzzyAbstractController::setup(_engine, _gyro);
  com = _com;
  setReactionInterval(50);
  errorSum = 0.0;
  lastError = 0.0;
}

// input: input angle
// output: output angle
// setpoint: desired angle
void FuzzyPIDController::react() {
  double kp = com -> get(VAR_IDX_KP) / 100.0;
  double ki = com -> get(VAR_IDX_KI) / 100.0;
  double kd = com -> get(VAR_IDX_KD) / 100.0;

  double input = (double)currentPitch;
  double setpoint = 0.0;
  double error = setpoint - input;
  errorSum += error;
  double errorChange = error - lastError;
  double output = kp * error + ki * errorSum + kd * errorChange;
  lastError = error;
  output = constrain((int)output, -1000, 1000);
  output = map(output, -1000, 1000, -100, 100);
  int left = 1200 - (int)output;
  int right = 1200 + (int)output;
  engine -> setLeft(left);
  engine -> setRight(right);
  logger.log("ctrl", "cp: %d, kp: %d, ki: %d, kd: %d", currentPitch, (int)(kp * 100), (int)(ki * 100), (int)(kd * 100));
}
