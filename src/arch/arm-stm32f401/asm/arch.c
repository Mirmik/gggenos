
#include "genos/datastruct/bits.h"
#include "asm/genos/debug.h"
#include "hal/arch.h"
#include "kernel/diag.h"
#include "genos/time/sysclock.h"

#include "stm32f4xx_rcc.h" 


//extern "C" void SysTick_Handler(); 
void SysTick_Handler()
{
  sysclock_tick();
};

//void interrupt_connector();

extern struct diag_ops usart2_diag;
void arch_init()
{
	SysTick_Config(84000);
	current_diag = &usart2_diag;
	//interrupt_connector();
}

unsigned int arch_systimer_state()
{
};


void arch_idle()
{	
}

void arch_halt()
{
	emergency_stop();
	//cli();
	while(1);
}

void arch_shutdown(arch_shutdown_mode_t mode){

	arch_halt();
}


void putChar(char c){diag_putc(c);};