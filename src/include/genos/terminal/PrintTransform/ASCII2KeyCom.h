#ifndef ASCII2KeyCom_H
#define ASCII2KeyCom_H

#include "genos/terminal/keycom.h"

class ASCII2KeyCom : public stream 
{	
	static constexpr int NORMALMODE = 0; 
	static constexpr int ESCAPEMODE = 1;
	static constexpr int MEMORY_SIZE = 16;

public:
	KeyCom * out;
	char memory[MEMORY_SIZE];
	char* mcursor;

	uint8_t mode;
	uint8_t escape_state;

	ASCII2KeyCom(KeyCom * _out);
	int putc(uint8_t c);
	

	void wrong_escape();
	void begin_escape_mode();
	void end_escape_mode();	

	int escape_mode_handler(char c);
	int normal_mode_handler(char c);

	
	int up (int);
	int down (int);
	int left (int);
	int right (int);
};

#endif
