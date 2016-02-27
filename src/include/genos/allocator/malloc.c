

#include "stdlib.h"
#include "genos/allocator/allocator.h"
#include "assert.h"
#include "genos/debug/debug.h"


	void* g_malloc(size_t sz)
	{
		assert(current_allocator);
		assert(current_allocator->allocate);
		return current_allocator->allocate(current_allocator, sz);
	};

	void g_free(void* ptr)
	{
		assert(current_allocator);
		assert(current_allocator->deallocate);
		current_allocator->deallocate(current_allocator, ptr);
	};

	void* g_realloc(void* ptr, size_t sz)
	{
		assert(current_allocator);
		assert(current_allocator->reallocate);
		return current_allocator->reallocate(current_allocator, ptr, sz);
	};

struct allocator* current_allocator = NULL;