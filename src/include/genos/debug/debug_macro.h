

#ifndef GENOS_DEBUG_MACRO
	#define GENOS_DEBUG_MACRO
	
	#include "genos/debug/iteration_counter.h"
	
	//Настройки поиска потеряных макросов
	#ifndef DEBUG_PLACE
	#define DEBUG_PLACE 0
	#endif
	
	#include <genos/debug/debug_print.h>
	#include <genos/debug/helper_macro.h>
	
	#define debug_delay(a) for (volatile uint64_t i = 0; i < a; i++);
	
	
	#define debug_place() if(DEBUG_PLACE) { \
		debug_print(__FILE__);				\
		debug_putchar(':'); 				\
		debug_printdec_uint64(__LINE__); 	\
		debug_putchar(':'); 				\
		debug_print(__FUNCTION__); 			\
		debug_putchar('\n');				\
	}				
	
	
	//Макросы debug_print
	#define dwr(a,b) {debug_write(a,b); debug_place();}
	#define dpr_hex(a) {debug_printhex_uint32((int32_t)a);  debug_place();}
	#define dpr_dec(a) {debug_printdec_uint32((int32_t)a);  debug_place();}
	#define dpr(a) {debug_print(a);  debug_place();}
	#define dpc(a) {debug_putchar(a); debug_place();}
	#define dln {debug_write((char*)"\r\n",2);  debug_place();}
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