#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <inttypes.h>
#include "genos/debug/debug.h"

template<typename type, size_t size>
class ring_buffer
{
	public:
	volatile type* tail;
	volatile type* head;
	volatile int total;
	volatile type buffer[size];

	public:
	ring_buffer() : tail(buffer), head(buffer), total(0) {};

	int peek(int n) volatile
	{
		if (total == 0) return -1;
		return *(head - 1);
	};

	int available() volatile
	{ 
		return total; 
	};

	int push(type c) volatile
	{
		if (total == size) return -1;
		*head++ = c;
		total++;
		if (head == buffer + sizeof(type) * total ) head = buffer;		
	};

	int pop() volatile
	{
		type ret;
		if (total == 0) return -1;
		ret = *tail++;
		total--;
		if (tail == buffer + sizeof(type) * total ) tail = buffer;		
	};

	~ring_buffer(){};
};

#endif