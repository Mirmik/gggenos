#include "asm/diag.h"
#include "hal/arch.h"
#include "stdlib.h"

void arch_halt(void)
{
	exit(0);
}

void arch_init()
{
	current_diag = &stdout_diag;
}

void arch_idle()
{
	
}

void arch_shutdown(arch_shutdown_mode_t mode){

	arch_halt();
}