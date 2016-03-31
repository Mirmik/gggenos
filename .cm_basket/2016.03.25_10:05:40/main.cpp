#include "genos/debug/debug.h"
#include "kernel/diag.h"
#include "hal/arch.h"

#include "genos/sigslot/delegate.h"

delegate<void> d;

class A
{public:
	void task1()
	{
		debug_printdec_uint32(28);
	};
} a;

void task1()
{
	debug_printdec_uint32(28);
};


void task(void* i)
{
	debug_printdec_uint32((uint32_t) i);
};
 
int main()
{
arch_init();
diag_init(); 

//d.set_ext(task, (void*) 28);
d.set(&a, &A::task1);
//debug_print_dump(&d, 12);dln;
//debug_printhex_uint32((uint32_t)&a);dln;
//debug_printhex_uint32((uint32_t)&a);dln;


d();
//debug_print("kekeke");
};