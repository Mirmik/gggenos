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

#include "hal/arch.h"

#include "kernel/diag.h"
#include "genos/debug/debug.h"

int main(){
arch_init();
diag_init();

debug_delay(20000);

diag_write((void*)"Mirmik",6);
}