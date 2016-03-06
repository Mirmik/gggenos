#ifndef HAL_GPIO_H
#define HAL_GPIO_H

#include "inttypes.h"
#include "sys/cdefs.h"

__BEGIN_DECLS

void gpio_mode_in(int gpio_number);
void gpio_mode_out(int gpio_number);
void gpio_mode_mode(int gpio_number);

void gpio_hi(int gpio_number);
void gpio_lo(int gpio_number);
void gpio_change(int gpio_number);
void gpio_level(int gpio_number, uint8_t level);

__END_DECLS

#endif