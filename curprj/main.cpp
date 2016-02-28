

#include "genos/debug/debug.h"
#include "kernel/diag.h"
#include "stddef.h"
#include "stdio.h"
#include "util/stub.h"

//#include "kernel/diag.h"

//extern "C" void debug_putchar(char c);
//void debug_putchar(char c) {putchar(c);};


diag_ops sss = {
	putchar, 
	(diag_getc_t) do_nothing, 
	diag_write_stub, 
	(diag_read_t) do_nothing, 
	(diag_init_t) do_nothing
};

int main(){
	current_diag = &sss;
//	diag_write((void*)"Mirmik", 6);



	debug_print("Here you are\n");

	debug_delay(20000000);

	debug_print("Mirmik\n");
	debug_print_test();
}