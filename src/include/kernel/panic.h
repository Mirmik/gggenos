/**
 * @file
 * @brief Defines panic macros
 *
 * @date 30.01.10
 * @author Eldar Abusalimov
 */

#ifndef KERNEL_PANIC_H_
#define KERNEL_PANIC_H_

#include <hal/arch.h>
//#include <hal/ipl.h>
#include <util/location.h>
#include <kernel/printk.h>
//#include <debug/whereami.h>

#define panic(...) \
	do { \
		__arch_atomic(); \
		printk(__VA_ARGS__); \
		arch_shutdown(ARCH_SHUTDOWN_MODE_ABORT); \
	} while (0)

#endif /* KERNEL_PANIC_H_ */
