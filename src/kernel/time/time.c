#include "genos/time/sysclock.h"

void delay(unsigned int d)
{
	unsigned int start = millis();
	while(millis() < start + d);
};