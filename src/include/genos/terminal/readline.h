
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
	void init() {cursor = cmdline;};
	int putc(char c) {
		*cursor++=c;
		assert (cursor - cmdline < READLINE_SIZE) ;
	};
	char* get_line(){*cursor = '\0'; return cmdline;};

	Readline() {init();};
};
#endif
