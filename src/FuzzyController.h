#ifndef _FUZZY_CONTROLLER_H
#define _FUZZY_CONTROLLER_H

#include "FuzzyEngine.h"
#include "FuzzyGyro.h"

class FuzzyController {
  public:
    FuzzyController(FuzzyEngine *engine, FuzzyGyro *gyro);
    void update();
  private:
    double right;
    double left;

    FuzzyEngine* engine;
    FuzzyGyro* gyro;

    unsigned long times[5];
    float angles[5];
    void shiftAngles();
};

#endif
