#ifndef GENOS_DEBUG_ERRSERV
	#define GENOS_DEBUG_ERRSERV
	
	//#include "asm/genos/debug.h"
	#include "hal/arch.h"
	
	#define debug_halt() \
	{\
		debug_print("system halt");\
		arch_halt();\
	};
	
	
	

	#define debug_panic(str) \
	{\
		uint8_t temp;\
		NOTEMP_ATOMIC(); \
		debug_print_init(); \
		debug_print(str); \
		debug_putchar('\n');\
		debug_print("Kernel panic! "); \
		debug_halt();	\
	};
	
#endif
