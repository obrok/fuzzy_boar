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

  protected:
    int pitchDifference;
    int previousPitch;
    int currentPitch;

  private:
    virtual void react() = 0;

    FuzzyEngine* engine;
    FuzzyGyro* gyro;
    long lastReactionMillis;
    int reactInterval;
};

#endif
