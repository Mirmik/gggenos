
#ifndef _READLINE_H_
#define _READLINE_H_

#define READLINE_SIZE 40

#include "assert.h"
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
		*cursor++=c;
		if (cursor - cmdline >= READLINE_SIZE) return -1;
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

	Readline() {init();};
};
#endif
