
#ifndef GENOS_DEBUG_HELPER_MACRO
	#define GENOS_DEBUG_HELPER_MACRO



//Вспомогательные функции.
	
	#define _concate(a, b) __concate(a, b)
	#define __concate(a, b) a ## b
		
	#define _var_generate(a) _concate(_concate(__internal_var_, __LINE__),a)
	
	
	#define _str_generate(a) __str_generate(a)
	#define __str_generate(a) #a
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	#endif