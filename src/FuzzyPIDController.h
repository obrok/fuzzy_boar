#ifndef _FUZZY_PID_CONTROLLER_H
#define _FUZZY_PID_CONTROLLER_H

#include "FuzzyAbstractController.h"
#include "FuzzyCom.h"

class FuzzyPIDController : public FuzzyAbstractController {
  public:
    void react();
    void setup(FuzzyEngine* _engine, FuzzyGyro* _gyro, FuzzyCom* _com);

  protected:
    FuzzyCom *com;
    double errorSum, lastError;
    int i;
};

#endif
