#include "genos/debug/delay.h"

#define SYSTEM_CLOCK 84000000

static inline void processor_delay(uint32_t n_clock3)
{
        asm volatile (
        "\n"
       "\tmov r3, %0\n"
        "Lab%=:" "\n\t"
        "subs r3, r3, 1" "\n\t"
        "bne.n Lab%=" "\n\t"
	:  
	: "r" (n_clock3) 
	: "r3"
        );
        return;
}


void processor_delay_us(uint32_t s) 
{
	processor_delay(s*(float)SYSTEM_CLOCK/3/1000000);
};
void processor_delay_ms(uint32_t s)
{ 
processor_delay(s*(float)SYSTEM_CLOCK/3/1000);
};