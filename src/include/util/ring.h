#ifndef GENOS_UTIL_RING_H
#define GENOS_UTIL_RING_H

#include "util/ring_head.h"
#include "genos.h"

class ring
{public:
	char* buffer;
	ring_head r;
	
	ring(char* _buffer, size_t sz):
	r(sz), buffer((char*)_buffer)
	{};

	inline int _putc(char c)
	{
		*(buffer + r.head) = c;
		ring_move_head_one(&r);
		return 1;
	}

	int putc(char c)
	{
		if (ring_is_full(&r)) return -1;
		return _putc(c);	
	}

	inline int _getc()
	{
		char c = *(buffer + r.tail);
		ring_move_tail_one(&r);
		return (int)c;
	}

	int getc()
	{
		if (ring_is_empty(&r)) return -1; 
		return _getc();	
	}

	int data_size()
	{
		return ring_data_size(&r);
	};

	int room_size()
	{
		return ring_room_size(&r);
	};

	bool is_empty()
	{
		return ring_is_empty(&r);
	};

	bool is_full()
	{
		return ring_is_full(&r);
	};

#ifdef TO_STR
	string to_info()
	{
		string str;
		str.reserve(128);
		str << "ring info:\r\n";
		str << "size: " << r.size << "\r\n";
		str << "head: " << r.head << "\r\n";
		str << "tail: " << r.tail << "\r\n";
		return str;
	};

	string to_str()
	{
		string str;
		str.reserve(32);
		str << "<ring>" << string(this, 16) << "\r\n";
		return str;
	};
#endif //TO_STR
};

#endif