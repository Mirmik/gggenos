#ifndef HAL_ARCH_H_
#define HAL_ARCH_H_

#include <asm/hal/arch.h>
#include <compiler.h>
#include <sys/cdefs.h>

typedef enum {
	ARCH_SHUTDOWN_MODE_HALT,
	ARCH_SHUTDOWN_MODE_REBOOT,
	ARCH_SHUTDOWN_MODE_ABORT,
} arch_shutdown_mode_t;


__BEGIN_DECLS

unsigned int arch_systimer_state(void);

void arch_init(void);

void arch_idle(void);

void _NORETURN arch_halt(void);

void _NORETURN arch_shutdown(arch_shutdown_mode_t mode);

void emergency_stop();

__END_DECLS


#define arch_atomic_temp(temp) __arch_atomic_temp(temp)
#define arch_deatomic_temp(temp) __arch_deatomic_temp(temp)

#define arch_atomic() __arch_atomic()
#define arch_deatomic() __arch_deatomic()


#endif /* HAL_ARCH_H_ */
