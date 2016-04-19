
#ifndef _READLINE_H_
#define _READLINE_H_

#include "assert.h"
template<unsigned int READLINE_SIZE>
class  Readline
{
private:
	char cmdline[READLINE_SIZE];
	char* cursor;								
public:
	
	void init() 
	{
		cursor = cmdline;
	};
	
	int putc(char c) {
		if (length() >= READLINE_SIZE) return -1;
		*cursor++=c;
		return 1;
	};
	
	int backspace()
	{
		if (cursor == cmdline) return 0;
		--cursor;
		return 1;
	};

	char* get_line()
	{
		*cursor = '\0'; return cmdline;
	};

	inline uint16_t length()
	{
		return cursor - cmdline;
	};

	Readline() {init();};
};
#endif
