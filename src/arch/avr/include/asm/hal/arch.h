#ifndef HAL_ARCH_H_
# error "Do not include this file directly!"
#endif /* HAL_ARCH_H_ */

#ifndef __BYTE_ORDER__
#define __BYTE_ORDER __LITTLE_ENDIAN
#define BYTE_ORDER    __BYTE_ORDER
#endif

#define __PLATFORM_ARCH "avr"


#include "avr/interrupt.h"

typedef uint8_t sreg_t;

#define __arch_atomic_temp(temp) ({temp = SREG; cli();})
#define __arch_deatomic_temp(temp) SREG = temp;

#define __arch_atomic() cli()
#define __arch_deatomic() sei()
