#include "FuzzyCom.h"
#include "Arduino.h"

FuzzyCom::FuzzyCom() {
	currentPos = 0;
	clearBuffer();
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
