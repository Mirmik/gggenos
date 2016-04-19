#ifndef GENOS_RING_BASIC
#define GENOS_RING_BASIC

#include "inttypes.h"
#include "stdlib.h"
#include "util/assert.h"

class ring_untailed
{
public:
	uint8_t* buffer = 0;
	uint8_t* head = 0;
	uint8_t* roof = 0; 
	size_t size = 0;

	inline bool need_fixup(uint8_t* ptr)
	{
		return ptr >= roof; 
	};

	inline uint8_t* next(uint8_t* ptr, size_t bias)
	{
		ptr += bias;
		while (need_fixup(ptr)) ptr -= size;
		return ptr;
	};

	inline uint8_t* next(uint8_t* ptr)
	{
		++ptr;
		if (need_fixup(ptr)) ptr = buffer;
		return ptr; 
	};

	inline size_t to_top(uint8_t* ptr)
	{
		return top - ptr - 1; ; 
	};

	inline size_t data_size()
	{

	};

	inline size_t room_size()
	{

	};

	void allocate(size_t _size)
	{
		if (buffer) free((void*)buffer);
		buffer = (uint8_t*)malloc(_size);
		size = _size;
		head = buffer;
		top = buffer + size;
	};

	virtual int putc(char c)
	{
		*head = c;
		head = next(head);
		return 1;
	};

	virtual int write(const char* dst, size_t length)
	{
		assert(length <= size);
		size_t ttop = to_top(head);
		if (ttop > length)
		{
			memcpy(head, dst, length);
			head = next(length);
			return length;
		}
		else
		{
			memcpy(head, dst, ttop);
			memcpy(buffer, dst + ttop, length - ttop);
			head = next(length);
			return length;
		};
	};

	~ring_basic()
	{
		if (buffer) free((void*)buffer);
	};
};

#endif