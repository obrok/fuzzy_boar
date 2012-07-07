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
    FuzzyEngine* engine;
    FuzzyGyro* gyro;
    int pitchDifference;
    int previousPitch;
    int currentPitch;
    void setReactionInterval(unsigned int interval);
    unsigned int reactInterval;

  private:
    virtual void react() = 0;

    long lastReactionMillis;
};

#endif
