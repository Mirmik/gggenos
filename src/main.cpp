



#include "hal/arch.h"
#include "kernel/diag.h"
#include "genos/debug/debug.h"
#include "genos/time/sysclock.h"
#include "util/hashtable.h"
#include "aiop2560.h"

#include <string.h>


extern char __heap_start;

#include "stdlib.h"

#include "genos/allocator/allocator.h"
#include "genos/allocator/la_alloc.h"
#include <genos/allocator/heap_bm.h>


int main(){
	arch_init();
	diag_init();
	leds_init();
	//la_init(&__heap_start);
	bm_init(&__heap_start, 0x1000);

	sei();
	
	debug_delay(20000);

	debug_print_test();

	malloc(28);

	while(1) 
	{
		delay(1000);
		red_change();
	};


while(1);
}