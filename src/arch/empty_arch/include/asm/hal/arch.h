#ifndef HAL_ARCH_H_
# error "Do not include this file directly!"
#endif /* HAL_ARCH_H_ */

#ifndef __BYTE_ORDER__
#define __BYTE_ORDER __LITTLE_ENDIAN
#define BYTE_ORDER    __BYTE_ORDER
#endif

#define __PLATFORM_ARCH "empty"

#define __arch_atomic_temp(temp) ((void*)0)
#define __arch_deatomic_temp(temp) ((void*)0)

#define __arch_atomic() ((void*)0)
#define __arch_deatomic() ((void*)0)
