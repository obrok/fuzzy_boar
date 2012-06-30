#include "FuzzyCom.h"
#include "Arduino.h"

FuzzyCom::FuzzyCom() {
	currentPos = 0;
	clearBuffer();
	for (int i = 0; i < 64; i++) {
		variables[i] = 0;
	}
	setMessage("No messages yet");
}

int FuzzyCom::read() {
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

char *FuzzyCom::message() {
	if (currentPos == 0) {
		return buffer;
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
		setMessage("Header mismatch");
		return;
	}
	messageLength = buffer[4];
	if (messageLength < 5 || messageLength > 255) {
		sprintf(buffer, "Message length invalid (%d)", messageLength);
		return;
	}


	if (strlen(buffer) + 1 != messageLength) {
		sprintf(buffer, "Message length mismatch (%d != %d)", messageLength, strlen(buffer) + 1);
		return;
	}

	strcpy(buffer, buffer + 5);

	char *tokens[10];
	char *token;
	int total = 0;
	token = strtok(buffer, " ");
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
		setMessage("Command unknown");
	}
}

void FuzzyCom::parseSet(char **tokens, int size) {
	if (size == 3) {
		int var = atoi(tokens[1]);
		int val = atoi(tokens[2]);
		set(var, val);
		sprintf(buffer, "set %d = %d", var, val);
	} else {
		setMessage("Invalid set command");
	}
}

void FuzzyCom::parseGet(char **tokens, int size) {
	if (size == 2) {
		int var = atoi(tokens[1]);
		int val = get(var);
		sprintf(buffer, "get %d = %d", var, val);
	} else {
		setMessage("Invalid get command");
	}
}

void FuzzyCom::setMessage(const char *msg) {
	strcpy(buffer, msg);
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
}
