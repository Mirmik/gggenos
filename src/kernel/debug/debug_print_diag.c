
#include <genos/debug/debug_print.h>
#include <kernel/diag.h>

	void debug_putchar(char c) {diag_putc(c);};
	
	void debug_write(char*c, int i)
	{
		diag_write(c, i);
	};
	
	
	
