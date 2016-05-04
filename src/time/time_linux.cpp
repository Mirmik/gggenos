#include "genos/kernel/time.h"
#include "time.h"

time_t millis()
{
	struct timespec current_time;
	clock_gettime(CLOCK_REALTIME, &current_time);
	return current_time.tv_nsec / 1000000 + current_time.tv_sec * 1000;
};