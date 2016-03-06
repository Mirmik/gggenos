/**
 * @file
 * @brief Printk declaration
 *
 * @date 13.02.10
 * @author Eldar Abusalimov
 */

#ifndef KERNEL_PRINTK_H_
#define KERNEL_PRINTK_H_

#include <stdarg.h>
#include <compiler.h>
#include <sys/cdefs.h>
#include "kernel/kern_level.h"

__BEGIN_DECLS

extern int printk(int level, const char *format, ...) _PRINTF_FORMAT(2, 3);
extern int vprintk(int level, const char *format, va_list args) _PRINTF_FORMAT(2, 0);

//extern int printu(void (*printchar_handler)(struct printchar_handler_data *d, int c), const char *format, ...) _PRINTF_FORMAT(1, 2);
//extern int vprintu(void (*printchar_handler)(struct printchar_handler_data *d, int c), const char *format, va_list args) _PRINTF_FORMAT(1, 0);

__END_DECLS

#endif /* KERNEL_PRINTK_H_ */
