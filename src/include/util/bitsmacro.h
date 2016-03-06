


#ifndef UTIL_BITS_MACRO
#define UTIL_BITS_MACRO


#define sbi(port, num) (port |= 1 << num)
#define cbi(port, num) (port &= ~(1 << num))








#endif