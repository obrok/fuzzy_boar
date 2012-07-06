#include "FuzzyCom.h"
#include "Arduino.h"
#include "FuzzyLogger.h"

extern FuzzyLogger logger;

FuzzyCom::FuzzyCom() {
  currentPos = 0;
  clearBuffer();
  for (int i = 0; i < 64; i++) {
    variables[i] = 0;
  }
  setResponse("No messages yet");
}

void FuzzyCom::read() {
  if (currentPos == 0) {
    messageReceived = false;
    clearBuffer();
  }
  buffer[currentPos] = Serial.read();
  if (buffer[currentPos] == '\0') {
    currentPos = 0;
    parseMessage();
    messageReceived = true;
  } else {
    currentPos++;
  }
}

char *FuzzyCom::getResponse() {
  if (currentPos == 0) {
    return response;
  } else {
    return "Message not parsed yet";
  }
}

bool FuzzyCom::hasMessage() {
  return messageReceived;
}

bool FuzzyCom::isValidHeader() {
  return buffer[0] == 'f' && buffer[1] == 'c' && buffer[2] == 'o' && buffer[3] == 'm';
}

void FuzzyCom::parseMessage() {
  int messageLength;

  if (!isValidHeader()) {
    setResponse("Header mismatch");
    return;
  }
  messageLength = buffer[4];
  if (messageLength < 5 || messageLength > 255) {
    sprintf(response, "Message length invalid (%d)", messageLength);
    return;
  }


  if (strlen(buffer) + 1 != messageLength) {
    sprintf(response, "Message length mismatch (%d != %d)", messageLength, strlen(buffer) + 1);
    return;
  }

  char bufcopy[255];
  strcpy(bufcopy, buffer + 5);
  strcpy(buffer, bufcopy);

  char *tokens[10];
  char *token;
  int total = 0;
  token = strtok(bufcopy, " ");
  while (token != NULL) {
    tokens[total] = token;
    total++;
    token = strtok(NULL, " ");
  }

  if (strcmp(tokens[0], "set") == 0) {
    parseSet(tokens, total);
  } else if (strcmp(tokens[0], "get") == 0) {
    parseGet(tokens, total);
  } else {
    setResponse("Command unknown");
  }
}

void FuzzyCom::parseSet(char **tokens, int size) {
  if (size == 3) {
    int var = atoi(tokens[1]);
    int val = atoi(tokens[2]);
    set(var, val);
    sprintf(response, "set %d = %d", var, val);
  } else {
    setResponse("Invalid set command");
  }
}

void FuzzyCom::parseGet(char **tokens, int size) {
  if (size == 2) {
    int var = atoi(tokens[1]);
    int val = get(var);
    sprintf(response, "get %d = %d", var, val);
  } else {
    setResponse("Invalid get command");
  }
}

void FuzzyCom::setResponse(const char *msg) {
  strcpy(response, msg);
}

void FuzzyCom::clearBuffer() {
  int i;
  for (i = 0; i < 255; i++) {
    buffer[i] = '\0';
  }
}

int FuzzyCom::get(int var) {
  return variables[var];
}

int FuzzyCom::set(int var, int val) {
  variables[var] = val;
  return val;
}

char *FuzzyCom::getRequest() {
  return buffer;
}

int FuzzyCom::update() {
  if (Serial.available() > 0) {
    read();
    return hasMessage();
  } else {
    return false;
  }
}
