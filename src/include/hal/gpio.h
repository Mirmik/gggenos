#ifndef HAL_GPIO_H
#define HAL_GPIO_H

#include "inttypes.h"
#include "sys/cdefs.h"
#include "asm/gpio.h"

__BEGIN_DECLS

//Для работы с таблицей пинов.
void gpio_mode_in(int gpio_number);
void gpio_mode_out(int gpio_number);
void gpio_mode_mode(int gpio_number);

void gpio_hi(int gpio_number);
void gpio_lo(int gpio_number);
void gpio_change(int gpio_number);
void gpio_level(int gpio_number, uint8_t level);

__END_DECLS

//Для работы с портами.
#define gpio_port_set_mask(x, y) __gpio_port_set_mask(x, y) 
#define gpio_port_clr_mask(x, y) __gpio_port_clr_mask(x, y)
#define gpio_port_tgl_mask(x, y) __gpio_port_tgl_mask(x, y)

#define gpio_port_set_pin(x, y) __gpio_port_set_pin(x, y) 
#define gpio_port_clr_pin(x, y) __gpio_port_clr_pin(x, y)
#define gpio_port_tgl_pin(x, y) __gpio_port_tgl_pin(x, y)

#endif