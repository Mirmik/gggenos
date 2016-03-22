#include "genos/debug/delay.h"
#include "genos/debug.h"

void processor_delay_ms(uint32_t u)
{
	debug_delay(u);
};


void processor_delay_us(uint32_t u)
{
	debug_delay(u);
};