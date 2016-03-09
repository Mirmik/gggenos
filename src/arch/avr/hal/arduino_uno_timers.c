#include "cpu/timers.h"
#include "avr/io.h"
#include "util/bits.h"
#include "util/stub.h"

void timer0_source_set(enum timerSource src)
{
	TCCR0B = (TCCR0B & 0xF8) | (src & 0x7);
};

void timer0_wgm_set(enum timerWgm wgm)
{
	 error_stub();
};

void timer0_comA_set(enum timerCom com)
{
	 bits_mask_assign_bias(TCCR0A, com, 0b0011, 6);
};

void timer0_comB_set(enum timerCom com)
{
	 error_stub();
};

void timer0_comC_set(enum timerCom com)
{
	 error_stub();
};


uint8_t timer0_source_get()
{
	return TCCR0B & 0b00000111;
};

int timer0_divider_get()
{
	switch(timer0_source_get())
	{
		case timerSourceCK: return 1;
		case timerSourceCKDiv8: return 8;
		case timerSourceCKDiv64: return 64;
		case timerSourceCKDiv256: return 256;
		case timerSourceCKDiv1024: return 1024;
		default: return -1;
	};
};

uint8_t timer0_wgm_get()
{
	return ((TCCR0B & (1 << WGM02)) >> 1) & (TCCR0A & 0b11);
};


void timer1_source_set(enum timerSource src)
{
	TCCR1B = (TCCR1B & 0xF8) | (src & 0x7);
};

void timer1_wgm_set(enum timerWgm wgm)
{
	error_stub();
};

void timer1_comA_set(enum timerCom com)
{
	error_stub();
};
