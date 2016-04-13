

#include "KeyCom2ASCII.h"

KeyCom2ASCII::KeyCom2ASCII(Print *a)
{init(a);}

void KeyCom2ASCII::init(Print * a)
{out=a;}

size_t KeyCom2ASCII::write(uint8_t c)
{	
	switch(c)
	{
	case '\b': out->print ("\033[D \033[D"); break;
	default:
	out->write(c);
	}
}


int KeyCom2ASCII::left(int i) { for(int j=0;j<i;j++) out->print("\033[D");}
int KeyCom2ASCII::right(int i) {for(int j=0;j<i;j++) out->print("\033[C");}
int KeyCom2ASCII::up(int i) {}
int KeyCom2ASCII::down(int i) {}
int KeyCom2ASCII::del(int i) {
	if (i==0) {write(' '); left(1);}
	if (i>0) {for (int j=0;j<i+1;j++) {write(' ');}; left(i+1);}
	//if (i<0) {for (int j=0;j>i;j--) {left ( 1);	write(' ');};right(i);}
	}
