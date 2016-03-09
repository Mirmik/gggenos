#include "inttypes.h"
#include "avr/io.h"

struct 

struct pwm_regs
{
	struct timer_regs* tim;
	volatile uint8_t ocr;	
};

