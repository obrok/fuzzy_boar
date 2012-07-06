#ifndef _FUZZY_ABSTRACT_CONTROLLER_H
#define _FUZZY_ABSTRACT_CONTROLLER_H

#include "FuzzyEngine.h"
#include "FuzzyGyro.h"

class FuzzyAbstractController {
  public:
    FuzzyAbstractController();
    void setup(FuzzyEngine* _engine, FuzzyGyro* _gyro);
    void update();
    void stop();
  private:
    virtual void react() = 0;

    FuzzyEngine* engine;
    FuzzyGyro* gyro;
    int pitchDifference;
    int previousPitch;
    int currentPitch;
};

#endif
