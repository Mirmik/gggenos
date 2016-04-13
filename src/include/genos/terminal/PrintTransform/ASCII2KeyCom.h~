#ifndef ASCII2KeyCom_H
#define ASCII2KeyCom_H

#include "prototype/KeyCom.h"
#include "genoslib.h"

class ASCII2KeyCom : public Print 
{public:
ASCII2KeyCom(KeyCom * _out);
KeyCom * out;
size_t write(uint8_t c);


	char escape_seq;
	char escape;
	
	int escape_process (char ch);
	
	int up (int);
	int down (int);
	int left (int);
	int right (int);
};

#endif
