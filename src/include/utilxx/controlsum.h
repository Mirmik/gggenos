#ifndef GENOS_CONTROL_SUM
#define GENOS_CONTROL_SUM

uint16_t controlsum(const string& str)
{
	const char* buf = str.c_str();
	uint16_t sum = 0;
	while(*buf)
		sum += *buf++;
	return sum;
}; 

#endif