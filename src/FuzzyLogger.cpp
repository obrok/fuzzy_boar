#include "FuzzyLogger.h"

FuzzyLogger::FuzzyLogger() {
}

void FuzzyLogger::log(char *topic, char *fmt, ...) {
  char tmp[256];
  char logMsg[256];

  va_list args;
  va_start(args, fmt);
  vsnprintf(tmp, 256, fmt, args);
  va_end(args);
  sprintf(logMsg, "[%5ld][%s] %s", millis(), topic, tmp);
  Serial.println(logMsg);
}
