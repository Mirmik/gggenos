#include "genos/kernel/time.h"
#include "genos/debug/debug.h"

void delay(time_t d)
{
	time_t start = millis();
	while(millis() < start + d);
};