#include "FuzzyLogger.h"

FuzzyLogger::FuzzyLogger() {
  working = false;
  timeout = 0;
}

void FuzzyLogger::start(unsigned int secs) {
  working = true;
  timeout = millis() + secs * 1000;
}

void FuzzyLogger::stop() {
  if (working) {
    Serial.println("endlog");
  }
  working = false;
}

bool FuzzyLogger::isAfterTimeout() {
  return timeout < millis();
}

void FuzzyLogger::log(char *topic, char *fmt, ...) {
  if (!isAfterTimeout()) {
    char tmp[256];
    char logMsg[256];

    va_list args;
    va_start(args, fmt);
    vsnprintf(tmp, 128, fmt, args);
    va_end(args);
    sprintf(logMsg, "[%ld][%s] %s", millis(), topic, tmp);
    Serial.println(logMsg);
  }
}
