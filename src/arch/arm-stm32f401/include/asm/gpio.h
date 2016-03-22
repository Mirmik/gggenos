#ifndef _ARCH_GPIO
#define _ARCH_GPIO

#include "stm32f4xx_gpio.h"

#define __gpio_port_set_mask(x, y) GPIO_SetBits(x, y)
#define __gpio_port_clr_mask(x, y) GPIO_ResetBits(x, y)
#define __gpio_port_tgl_mask(x, y) GPIO_ToggleBits(x, y)

#define __gpio_port_set_pin(x, y) GPIO_SetBits(x, (1 << y))
#define __gpio_port_clr_pin(x, y) GPIO_ResetBits(x, (1 << y))
#define __gpio_port_tgl_pin(x, y) GPIO_ToggleBits(x, (1 << y))


#endif