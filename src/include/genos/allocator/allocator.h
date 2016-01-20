#ifndef GENOS_ALLOCATOR
#define GENOS_ALLOCATOR

#include "sys/types.h"

struct allocator 
{
	void* (*allocate)(struct allocator* alc, size_t sz);
	void (*deallocate)(struct allocator* alc,  void* ptr);
	void* (*reallocate)(struct allocator* alc, void* ptr, size_t sz);
	void* info;
};
extern struct allocator* current_allocator;

#endif