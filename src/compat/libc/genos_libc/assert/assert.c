
#include "genos/debug/debug.h"
#include "assert.h"

void __assert_handler(struct __assertion_point point)
{
#ifndef NDEBUG_ASSERT
	debug_print("assert!\n\rfunc: ");	
	debug_print(point.location.func); 
	debug_print("\n\rfile: ");	
	debug_print(point.location.at.file); 
	debug_print("\n\rline: ");	
	debug_printdec_uint16(point.location.at.line); 
	debug_print("\n\rexpr: ");	
	debug_print(point.expression); 
	debug_print("\n\r");	
	arch_halt();
#endif
};