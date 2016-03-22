/**
 * @file
 * @brief
 *
 * @date 10.11.10
 * @author Anton Bondarev
 */

#include <util/assert.h>
#include <kernel/diag.h>
#include <stdarg.h>

#include "sys/cdefs.h"
#include "stddef.h"

static void printk_printchar(struct printchar_handler_data *d, int c) {
	diag_putc(c);
}


EXTERN_C int __print(void (*printchar_handler)(struct printchar_handler_data *d, int c),
		struct printchar_handler_data *printchar_data,
		const char *format, va_list args);


int printk(const char *format, ...) {
	int ret;
	va_list args;

	assert(format != NULL);

	va_start(args, format);
	ret = __print(printk_printchar, NULL, format, args);
	va_end(args);

	return ret;
}

int vprintk(const char *format, va_list args) {
	return __print(printk_printchar, NULL, format, args);
}

}
