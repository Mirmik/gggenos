

#include "asm/hal/timers.h"
#include "avr/io.h"

void timer1_source(enum timerSource src)
{
	TCCR1B = (TCCR1B & 0xF8) | (src & 0x7);
};