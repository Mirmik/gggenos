








#include "aiop2560.h"
#include "defines/clock_t.h"
#include "genos/time/sysclock.h"
#include "avr/io.h"
#include "avr/interrupt.h"


ISR(TIMER0_OVF_vect)
{
	sysclock_tick();
}

ISR(USART0_RX_vect)
{
	char c = UDR0;
	UDR0 = c;
}