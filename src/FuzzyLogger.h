#ifndef _FUZZY_LOGGER_H
#define _FUZZY_LOGGER_H

#include "Arduino.h"

class FuzzyLogger {
  public:
    FuzzyLogger();
    void log(char *topic, char *fmt, ...);
    void setTopic(char *topic);
    void setAllTopics();

  private:
    char logTopic[255];
    bool showAllTopics;
};

#endif
