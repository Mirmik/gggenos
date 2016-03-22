#ifndef GENOS_SYSCLOCK_H
#define GENOS_SYSCLOCK_H

#include "sys/cdefs.h"
#include "defines/clock_t.h"
#include "defines/time_t.h"

//extern clock_t sysclock_clocks_per_second;


extern unsigned int _seconds;
extern unsigned int _millis;
extern unsigned int _micros;

extern unsigned int seconds_inc;
extern unsigned int millis_inc;
extern unsigned int micros_inc;


__BEGIN_DECLS

//void set_clock_param(clock_t f_cpu, clock_t timer_divide, unsigned int systimer_ceil);

void sysclock_tick();
clock_t clock();

time_t millis();
time_t micros();
time_t seconds();

void delay(unsigned int);
void udelay(unsigned int);

__END_DECLS

#endif