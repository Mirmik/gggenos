#ifndef GENOS_UROBOROS_H
#define GENOS_UROBOROS_H

#include "genos.h"
#include "string.h"
#include "util/future/uroboros_head.h"

class uroboros
{public:
	char* buffer;
	uroboros_head u;
	
	uroboros(char* _buffer, size_t sz):
	u(sz), buffer((char*)_buffer)
	{};

	int putc(char c)
	{
		*(buffer + u.head) = c;
		uroboros_move_head_one(&u);
		return 1;
	};

	int read(char* dst, size_t sz)
	{
	//	if (sz >= u.size) sz = u.size;
	//	size_t to_top = uroboros_how_many_head_to_top(&u);
	//	size_t first_iter = sz < to_top ? sz : to_top;
	//	size_t second_iter = sz - first_iter;
	//	memcpy(dst, buffer + u.head, first_iter);
	//	if (second_iter) 
	//		memcpy(dst + first_iter, buffer, second_iter);
	//	*(dst+sz) = 0;
	//	return sz;
	};

	string to_info()
	{
		string str;
		str.reserve(128);
		str << "uroboros info:\r\n";
		str << "size: " << u.size << "\r\n";
		str << "head: " << u.head << "\r\n";
		return str;
	};

	string to_str()
	{
		string str;
		str.reserve(32);
		str << "<uroboros>" << string(this, 16) << "\r\n";
		return str;
	};

};


#endif