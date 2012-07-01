#ifndef _FUZZY_COM_H
#define _FUZZY_COM_H

class FuzzyCom {
    public:
		FuzzyCom();
		void read();
		char *getResponse();
		char *getRequest();
		bool hasMessage();
		int get(int var);
		int set(int var, int val);

	private:
		bool messageReceived;
		bool isValidHeader();
		void setResponse(const char *);
		void parseMessage();
		int currentPos;
		char buffer[255];
		char response[255];
		void clearBuffer();
		void emptySerial();
		int variables[64];
		void parseSet(char **tokens, int size);
		void parseGet(char **tokens, int size);
};

#endif
