



#include "genos/datastruct/bits.h"
#include "asm/genos/debug.h"
#include "hal/arch.h"
#include "kernel/diag.h"
#include "genos/time/sysclock.h"

void arch_init()
{
}

unsigned int arch_systimer_state()
{
};


void arch_idle()
{
	
}

void arch_halt()
{
}

void arch_shutdown(arch_shutdown_mode_t mode){

	arch_halt();
}


void assert_param(int x){};

void putChar(char c){};