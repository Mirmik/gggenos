

#include "genos/time/sysclock.h"
#include "hal/arch.h"
//#include "kernel/printk.h"

volatile clock_t systick;
unsigned int cceil; 

double mmtime_inc;
double mmtime_inc_wide;
volatile unsigned long long _minutes;
volatile double mmtime;



void set_clock_param(clock_t f_cpu, clock_t timer_divide, unsigned int systimer_ceil)
{	
	sreg_t temp;
	arch_atomic_temp(temp);

	double s_dvd = double(timer_divide) / double(f_cpu);

	mmtime_inc = s_dvd * 1000000; 
	mmtime_inc_wide = mmtime_inc / 256; 
	cceil = systimer_ceil;

	arch_deatomic_temp(temp); 
};

void sysclock_tick()
{
	systick++;
	mmtime += mmtime_inc;
	if (mmtime >= 60000000) {mmtime -= 60000000; _minutes++;}
};

clock_t clock()
{
	return systick ;
};

time_t minutes()
{
	return _minutes;	
};


time_t seconds()
{
	return mmtime / 1000000 + _minutes * 60;	
};

time_t millis()
{
	return mmtime / 1000  + _minutes * 60000;
};

time_t micros()
{
	return mmtime + mmtime_inc_wide * arch_systimer_state() + _minutes * 60000000;	
};

void delay(unsigned int d)
{
	time_t start = millis();
	while( millis() - start < d);
};


void udelay(unsigned int d)
{
	time_t start = micros();
	while( micros() - start < d);
};