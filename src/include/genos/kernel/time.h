#ifndef GENOS_KERNEL_TIME
#define GENOS_KERNEL_TIME

#include "defines/clock_t.h"
#include "defines/time_t.h"

time_t millis();
time_t micros();
time_t seconds();

void delay(unsigned int);
void udelay(unsigned int);

#endif