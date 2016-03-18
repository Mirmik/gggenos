#include "cpu/timers.h"
#include "avr/io.h"
#include "util/bits.h"
#include "util/stub.h"

//****************TIMER0_SETERS*************

void timer0_source_set(enum timerSource src)
{
	TCCR0B = (TCCR0B & 0xF8) | (src & 0x7);
};

void timer0_wgm_set(enum timerWgm wgm)
{
 	//return ((TCCR0B & (1 << WGM02)) >> 1) & (TCCR0A & 0b11);
	bits_mask_assign_bias(TCCR0B, wgm, 0b100, 1);
	bits_mask_assign(TCCR0A, wgm, 0b011);
};

void timer0_comA_set(enum timerCom com)
{
	 bits_mask_assign_bias(TCCR0A, com, 0b0011, 6);
};

void timer0_comB_set(enum timerCom com)
{
	 bits_mask_assign_bias(TCCR0A, com, 0b0011, 4);
};

void timer0_comC_set(enum timerCom com)
{
	 error_stub();
};


//****************TIMER0_GETERS*************

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
	return ((TCCR0B & (1 << WGM02)) >> 1) | (TCCR0A & 0b11);
};

uint8_t timer0_comA_get()
{
	 return bits_mask_bias(TCCR0A, 0b0011, 6);	 
};

uint8_t timer0_comB_get()
{
	 return bits_mask_bias(TCCR0A, 0b0011, 4);
};

uint8_t timer0_comC_get()
{
	 error_stub();
};


//***********TIMER1_SETTERS*******************/

void timer1_source_set(enum timerSource src)
{
	TCCR1B = (TCCR1B & 0xF8) | (src & 0x7);
};

void timer1_wgm_set(enum timer16Wgm wgm)
{
	bits_mask_assign_bias(TCCR1B, wgm, 0b1100, 1);
	bits_mask_assign(TCCR1A, wgm, 0b011);
};

void timer1_comA_set(enum timerCom com)
{
	 bits_mask_assign_bias(TCCR1A, com, 0b0011, 6);
};

void timer1_comB_set(enum timerCom com)
{
	 bits_mask_assign_bias(TCCR1A, com, 0b0011, 4);
};

void timer1_comC_set(enum timerCom com)
{
	 error_stub();
};


//***********TIMER1_GETTERS*******************/

uint8_t timer1_source_get()
{
	return TCCR1B & 0b00000111;
};

int timer1_divider_get()
{
	switch(timer1_source_get())
	{
		case timerSourceCK: return 1;
		case timerSourceCKDiv8: return 8;
		case timerSourceCKDiv64: return 64;
		case timerSourceCKDiv256: return 256;
		case timerSourceCKDiv1024: return 1024;
		default: return -1;
	};
};

uint8_t timer1_wgm_get()
{
	return ((TCCR1B & (0b11 << 3)) >> 1) | (TCCR1A & 0b11);
};

uint8_t timer1_comA_get()
{
	 return bits_mask_bias(TCCR1A, 0b0011, 6);	 
};

uint8_t timer1_comB_get()
{
	 return bits_mask_bias(TCCR1A, 0b0011, 4);
};

uint8_t timer1_comC_get()
{
	 error_stub();
};



//***********TIMER3_SETTERS*******************/

void timer3_source_set(enum timerSource src)
{
	TCCR3B = (TCCR3B & 0xF8) | (src & 0x7);
};

void timer3_wgm_set(enum timer16Wgm wgm)
{
	bits_mask_assign_bias(TCCR3B, wgm, 0b1100, 1);
	bits_mask_assign(TCCR3A, wgm, 0b011);
};

void timer3_comA_set(enum timerCom com)
{
	 bits_mask_assign_bias(TCCR3A, com, 0b0011, 6);
};

void timer3_comB_set(enum timerCom com)
{
	 bits_mask_assign_bias(TCCR3A, com, 0b0011, 4);
};

void timer3_comC_set(enum timerCom com)
{
	 error_stub();
};


//***********TIMER3_GETTERS*******************/

uint8_t timer3_source_get()
{
	return TCCR3B & 0b00000111;
};

int timer3_divider_get()
{
	switch(timer3_source_get())
	{
		case timerSourceCK: return 1;
		case timerSourceCKDiv8: return 8;
		case timerSourceCKDiv64: return 64;
		case timerSourceCKDiv256: return 256;
		case timerSourceCKDiv1024: return 1024;
		default: return -1;
	};
};

uint8_t timer3_wgm_get()
{
	return ((TCCR3B & (0b11 << 3)) >> 1) | (TCCR3A & 0b11);
};

uint8_t timer3_comA_get()
{
	 return bits_mask_bias(TCCR3A, 0b0011, 6);	 
};

uint8_t timer3_comB_get()
{
	 return bits_mask_bias(TCCR3A, 0b0011, 4);
};

uint8_t timer3_comC_get()
{
	 error_stub();
};
