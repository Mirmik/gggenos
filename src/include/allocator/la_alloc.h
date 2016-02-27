#ifndef GENOS_LIST_ALLOC
	#define GENOS_LIST_ALLOC
	
	#include "sys/cdefs.h"
	__BEGIN_DECLS

	void* la_alloc(size_t sz);
	void la_free(void* ptr);	
	
	void la_debug_info();
	
	void la_init(char* heap_start);

	extern uint8_t dynamic_memory_debug;
	
	void dynamic_memory_debug_enable();
	
	__END_DECLS
	
	
#endif