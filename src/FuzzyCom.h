#ifndef _FUZZY_COM_H
#define _FUZZY_COM_H

class FuzzyCom {
    public:
		FuzzyCom();
		int read();
		char *message();
		bool hasMessage();
	private:
		bool messageReceived;
		bool isValidHeader();
		void setMessage(const char *);
		void parseMessage();
		int currentPos;
		char buffer[255];
		void clearBuffer();
		void emptySerial();
};

#endif
