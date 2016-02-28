
#include <genos/debug/debug_print.h>

	//debug_putchar реализуется в основной программе.
	
	void debug_write(char*c,int i)__attribute__((weak));
	void debug_write(char*c,int i)
	{
		int k;
		for (k=0;k<i;k++)
		debug_putchar(*(c+k));
	};
	
	