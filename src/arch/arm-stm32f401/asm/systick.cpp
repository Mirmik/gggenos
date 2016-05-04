#include "genos/kernel/time.h"
#include "genos/time/sysclock.h"
#include "hal/arch.h"
#include "inttypes.h"
//#include "kernel/printk.h"

volatile uint64_t systick;

void sysclock_tick()
{
	systick++;
};

clock_t clock()
{
	return systick ;
};

time_t seconds()
{
	return systick / 60;
};

time_t millis()
{
	return systick;
};



//void udelay(unsigned int d)
//{
//};