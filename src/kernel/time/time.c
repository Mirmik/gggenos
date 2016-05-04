#include "genos/time/sysclock.h"
#include "genos/kernel/time.h"

void delay(unsigned int d)
{
	unsigned int start = millis();
	while(millis() < start + d);
};