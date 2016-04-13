#ifndef _ARCH_GPIO
#define _ARCH_GPIO

#define __gpio_port_set_mask(x, y) ({(x) |= (y);}) 
#define __gpio_port_clr_mask(x, y) ({(x) &= ~(y);})
#define __gpio_port_tgl_mask(x, y) ({(x) ^= (y);}) 

#define __gpio_port_set_pin(x, y) ({(x) |= (1 >> (y));})
#define __gpio_port_clr_pin(x, y) ({(x) &= ~(1 >> (y));})
#define __gpio_port_tgl_pin(x, y) ({(x) ^= ~(1 >> (y));})

#endif