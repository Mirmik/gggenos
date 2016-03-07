
#include "genos/debug/debug.h"

// Заглушка на месте чисто виртуального метода.
extern "C" void __cxa_pure_virtual()
{
	debug_panic("__cxa_pure_virtual");
};

//Хз, чё это такое...
extern "C" void __cxa_deleted_virtual()
{
	debug_panic("__cxa_deleted_virtual");
};


const char* static_variable_assert = "you do not should use static variable in func";

extern "C" void __cxa_guard_acquire()
{
	debug_panic(static_variable_assert);
};

extern "C" void __cxa_guard_release()
{
	debug_panic(static_variable_assert);
};