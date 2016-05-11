
#ifndef _READLINE_H_
#define _READLINE_H_

#include "assert.h"
template<unsigned int READLINE_SIZE>
class  Readline
{
private:
	char line[READLINE_SIZE];
	char* cursor;								
public:
	
	void init() 
	{
		cursor = line;
	};
	
	int putc(char c) {
		if (length() >= READLINE_SIZE) return -1;
		*cursor++=c;
		return 1;
	};
	
	int backspace()
	{
		if (cursor == line) return 0;
		--cursor;
		return 1;
	};

	char* get_line()
	{
		*cursor = '\0'; return line;
	};

	inline uint16_t length()
	{
		return cursor - line;
	};

	char& operator[](int i)
	{
		return line[i];
	};

	Readline() {init();};
};
#endif
