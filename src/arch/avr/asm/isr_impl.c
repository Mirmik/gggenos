








#include "defines/clock_t.h"
#include "genos/time/sysclock.h"
#include "avr/io.h"
#include "avr/interrupt.h"
#include "genos/debug/debug.h"

/*
ISR(USART0_RX_vect)
{
	char c = UDR0;
	UDR0 = c;
}*/