#include "genos/datastruct/bits.h"
#include "avr/io.h"
#include "hal/arch.h"
#include "kernel/diag.h"
#include "genos/time/sysclock.h"

#include "cpu/usart_control_struct.h"

extern struct diag_ops usart0_diag;

extern volatile int64_t supertick;

ISR(TIMER0_OVF_vect) 
{
	//supertick++;
	sysclock_tick();
};


void arch_init()
{
	bits_set(TCCR0B, _BV(CS01) | _BV(CS00));
	set_clock_param(F_CPU, 256 * 64, 256);
	bits_set(TIMSK0, _BV(TOIE0));

	current_diag = &usart0_diag;
}

unsigned int arch_systimer_state()
{
	return TCNT0;
};


void arch_idle()
{
	
}

void arch_halt()
{
	emergency_stop();
	cli();
	while(1);
}

void arch_shutdown(arch_shutdown_mode_t mode){

	arch_halt();
}