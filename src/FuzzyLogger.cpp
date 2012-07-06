#include "FuzzyLogger.h"

FuzzyLogger::FuzzyLogger() {
  showAllTopics = true;
}

void FuzzyLogger::setTopic(char *topic) {
  showAllTopics = false;
  strcpy(logTopic, topic);
}

void FuzzyLogger::setAllTopics() {
  showAllTopics = true;
}

void FuzzyLogger::log(char *topic, char *fmt, ...) {
  char tmp[256];
  char logMsg[256];

  if (showAllTopics || strcmp(logTopic, topic) == 0 || strcmp(topic, "com") == 0) {
    va_list args;
    va_start(args, fmt);
    vsnprintf(tmp, 256, fmt, args);
    va_end(args);
    sprintf(logMsg, "[%5ld][%s] %s", millis(), topic, tmp);
    Serial.println(logMsg);
  }
}
