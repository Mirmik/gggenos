
#include <stdarg.h>
#include "stdio.h"
#include "assert.h"

int printk_level = 7;

int printk(int level, const char *format, ...) {
	int ret;
	va_list args;

	assert(format != NULL);
	if (level > printk_level) return 0;

	va_start(args, format);
	ret = vprintf(format, args);
	va_end(args);

	return ret;
}

int vprintk(int level, const char *format, va_list args) {
	if (level > printk_level) return 0;
	return vprintf(format, args);
}

