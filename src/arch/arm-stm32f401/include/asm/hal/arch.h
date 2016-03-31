#ifndef HAL_ARCH_H_
# error "Do not include this file directly!"
#endif /* HAL_ARCH_H_ */

#ifndef __BYTE_ORDER__
#define __BYTE_ORDER __LITTLE_ENDIAN
#define BYTE_ORDER    __BYTE_ORDER
#endif

#define __PLATFORM_ARCH "stm32f401"

#include "stm32f4xx.h"
typedef unsigned int ipl_t;

#define __arch_atomic_temp(temp) ipl_t temp; __asm__ __volatile__ ( "mrs %0, PRIMASK;\n\t" "cpsid i \n\t" : "=r"(r));
#define __arch_deatomic_temp(temp)  __asm__ __volatile__ ( "msr PRIMASK, %0;\n\t" : : "r"(temp) );

#define __arch_atomic() __asm__ __volatile__ ( "cpsid i \n\t" );
#define __arch_deatomic() __asm__ __volatile__ ( "cpsie i \n\t" );
