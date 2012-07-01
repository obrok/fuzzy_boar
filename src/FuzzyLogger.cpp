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

void FuzzyLogger::log(char *msg) {
	if (!isAfterTimeout()) {
		char logMsg[512];
		sprintf(logMsg, "[%ld] %s", millis(), msg);
		Serial.println(logMsg);
	}
}
