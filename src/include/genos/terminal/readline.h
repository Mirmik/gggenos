
#ifndef _READLINE_H_
#define _READLINE_H_

#include "genos/terminal/keycom.h"

#undef ENDL
#define ENDL '\n'


#define true  1
#define false 0

// microrl struct, contain internal library data
class  readline_t : public KeyCom
{public:
char cmdline[160];
int cursor;
int cmdlen;

char* get_line();
int move_cursor (int offset);
								
readline_t();
void init();

int backspace();
int del(int i);
int right(int i);
int left(int i);
//int del(int i);

int execute(int i);

int write(char c);
//void set_execute_callback (int (*_execute)(int, const char* const*));
};
#endif
