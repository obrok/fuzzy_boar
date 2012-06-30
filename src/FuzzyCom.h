#ifndef _FUZZY_COM_H
#define _FUZZY_COM_H

class FuzzyCom {
    public:
		FuzzyCom();
		int read();
		char *message();
		bool hasMessage();
		int get(int var);
		int set(int var, int val);

	private:
		bool messageReceived;
		bool isValidHeader();
		void setMessage(const char *);
		void parseMessage();
		int currentPos;
		char buffer[255];
		void clearBuffer();
		void emptySerial();
		int variables[64];
		void parseSet(char **tokens, int size);
		void parseGet(char **tokens, int size);
};

#endif
