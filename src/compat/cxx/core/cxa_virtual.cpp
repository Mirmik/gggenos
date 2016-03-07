
#include "genos/debug/debug.h"

extern "C" void __cxa_pure_virtual()
{
	debug_panic("__cxa_pure_virtual");
};