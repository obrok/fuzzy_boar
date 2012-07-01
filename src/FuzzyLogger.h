#ifndef _FUZZY_LOGGER_H
#define _FUZZY_LOGGER_H

#include "Arduino.h"

class FuzzyLogger {
  public:
    FuzzyLogger();
    void start(unsigned int secs);
    void stop();
    bool isAfterTimeout();
    void log(char *msg);
    void log(char *topic, char *fmt, ...);

  private:
    bool working;
    unsigned long timeout;
};

#endif
