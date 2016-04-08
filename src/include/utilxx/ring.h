#ifndef GENOS_UTILXX_RING
#define GENOS_UTILXX_RING

#include "util/ring.h"

template<typename type>
class ring
{
private:
	type* next(type* ptr){ ++ptr; return ptr >= buffer + size ? buffer : ptr; };

public:
	type* buffer;
	uint16_t size;
	type* head;
	type* tail;

	void putc(type c)
	{ 
		*head++ = c;
		if (head >= buffer + size) head = buffer;
	};

	type getc()
	{ 
		type c = *tail++;
		if (tail >= buffer + size) tail = buffer;
		return c;
	};

	bool full() 
	{ 
		return next(head) == tail ? 1 : 0; 
	};
	
	bool empty() 
	{ 
		return head == tail;
	};

	int available_to_getc() 
	{ 
		return (head >= tail) ? head - tail : size + head - tail; 
	};

	int available_to_putc()
	{
		return (head >= tail) ? size - 1 + ( tail - head ) : ( tail - head ) - 1;
	};

	void init(void* _buffer, uint16_t _size)
	{ 
		buffer = (type*)_buffer;
		size = _size;
		head = buffer;
		tail = buffer;
	};
};

#endif