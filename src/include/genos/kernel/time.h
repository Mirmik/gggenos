#ifndef GENOS_KERNEL_TIME
#define GENOS_KERNEL_TIME

#include "defines/clock_t.h"
#include "defines/time_t.h"

#include "sys/cdefs.h"

__BEGIN_DECLS

time_t millis();
time_t micros();
time_t seconds();

void delay(unsigned int);
void udelay(unsigned int);

__END_DECLS

#endif