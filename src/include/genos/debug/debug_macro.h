

#ifndef GENOS_DEBUG_MACRO
	#define GENOS_DEBUG_MACRO
	
	#include "genos/debug/iteration_counter.h"
	
	//Настройки поиска потеряных макросов
	
	#include <genos/debug/debug_print.h>
	#include <genos/debug/helper_macro.h>
	
	#define debug_delay(a) for (volatile uint64_t i = 0; i < a; i++);
	
	
#ifdef DEBUG_PLACE
	#define debug_place() 				    \
		debug_print(__FILE__);				\
		debug_putchar(':'); 				\
		debug_printdec_uint64(__LINE__); 	\
		debug_putchar(':'); 				\
		debug_print(__FUNCTION__); 			\
		debug_putchar('\n');				\
	}			
#else
	#define debug_place()
#endif
	
	
	//Макросы debug_print
	#define dpl debug_place();
	#define dwr(a,b) {debug_write(a,b); debug_place();}
//	#define dpr_hex(a) {debug_printhex_uint32((int32_t)a);  debug_place();}
//	#define dpr_dec(a) {debug_printdec_uint32((int32_t)a);  debug_place();}
//	#define dpr(a) {debug_print(a);  debug_place();}
	#define dpc(a) {debug_putchar(a); debug_place();}
	#define dln {debug_write((char*)"\r\n",2);}
	#define dtb {debug_putchar('\t');  debug_place();}
	
	
	//Макросы debug_errserv
	#define dstp {debug_place();debug_halt();}
	//debug_panic(a) 
	
	#define debug_print_type_func() {debug_print(__PRETTY_FUNCTION__); debug_place();};
	
	
	//Аппаратная часть:
	//debug_delay(a) 
	//ATOMIC(temp)
	//DEATOMIC(temp)
#endif