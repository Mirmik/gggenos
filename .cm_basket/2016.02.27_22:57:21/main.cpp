



//#include "hal/arch.h"
//#include "kernel/diag.h"
//#include "genos/debug/debug.h"
//#include "genos/time/sysclock.h"
//#include "util/hashtable.h"
//#include "aiop2560.h"

//#include <string.h>


//extern char __heap_start;

//#include "stdlib.h"
//#include "string.h"
//#include "genos/allocator/allocator.h"
//#include "genos/allocator/la_alloc.h"
//#include <genos/allocator/heap_bm.h>

#include "kernel/diag.h"
#include "genos/debug/debug_print.h"

int main(){

//	printf("HelloWorld");
//	sei();
	
//strcmp("aa","bb");

diag_putc('d');
current_diag->putc('d');
//	debug_delay(20000);

debug_print_test();

//	malloc(28);

//	while(1) 
//	{
//		delay(1000);
//		red_change();
//	};


//while(1);
}