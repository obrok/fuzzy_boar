#ifndef _FUZZY_CONTROLLER_H
#define _FUZZY_CONTROLLER_H

#include "FuzzyEngine.h"
#include "FuzzyGyro.h"

class FuzzyController {
  public:
    FuzzyController();
    void setup(FuzzyEngine* _engine, FuzzyGyro* _gyro);
    void update();
    void stop();
    bool isEven();
    bool isRightLower();
    bool hasRightRisen();
    bool hasLeftRisen();
    bool hasSwitchedSide();
    bool changedSideSinceLastKick();
    void easeKick();
    void initializeKick();
    bool isLeveling();
  private:
    int right;
    int left;

    FuzzyEngine* engine;
    FuzzyGyro* gyro;

    void react();
    int pitchDifference;
    int previousPitch;
    int currentPitch;
    int iteration;
    int step;
    bool side;
};

#endif
