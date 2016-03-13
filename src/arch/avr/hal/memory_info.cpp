#include "syscontext/syscontext.h"
#include "avr/io.h"

 extern int __heap_start, *__brkval; 
 
void memory_info(int argc, char** argv)
{
	stdout.print("brkval:");stdout.printhexln((uint16_t)__brkval);
	stdout.print("heap_start:");stdout.printhexln((uint16_t)&__heap_start);
	stdout.print("SP:");stdout.printhexln((uint16_t)SP);
};
