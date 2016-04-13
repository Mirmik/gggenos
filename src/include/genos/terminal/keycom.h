
#ifndef _KEY_COMMAND_H
#define _KEY_COMMAND_H

#include "genos/io/stream.h"

class KeyCom : public stream
{
public:
int putc(char c){};

virtual int delchar(uint8_t i){};
//virtual int cursor_home(int i){};
//virtual int cursor_end(int i){};
virtual int cursor_up(uint8_t n){};
virtual int cursor_down(uint8_t n){};
virtual int cursor_left(uint8_t n){};
virtual int cursor_right(uint8_t n){};

virtual int cursor_up(){};
virtual int cursor_down(){};
virtual int cursor_left(){};
virtual int cursor_right(){};

virtual int newline(){};
};

#endif
