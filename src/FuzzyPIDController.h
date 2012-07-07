#ifndef _FUZZY_PID_CONTROLLER_H
#define _FUZZY_PID_CONTROLLER_H

#include "FuzzyAbstractController.h"

class FuzzyPIDController : public FuzzyAbstractController {
  public:
    void react();
    void setup(FuzzyEngine* _engine, FuzzyGyro* _gyro);

  protected:
    double errorSum, lastError;
    double kp, ki, kd;
};

#endif
