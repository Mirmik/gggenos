

#ifndef GMODULE_AIOP2560
#define GMODULE_AIOP2560

#include "genos/datastruct/bits.h"

#define red_on() bits_set(PORTB,0x80)
#define red_off() bits_clr(PORTB,0x80)
#define red_change() bits_mask_rev(PORTB,0x80)

#define red_mode_out() bits_set(DDRB,0x80)
#define red_mode_in() bits_clr(DDRB,0x80)


#define green_on() bits_set(PORTC,0x80)
#define green_off() bits_clr(PORTC,0x80)
#define green_change() bits_mask_rev(PORTC,0x80)

#define green_mode_out() bits_set(DDRC,0x80)
#define green_mode_in() bits_clr(DDRC,0x80)


#define yellow_on() bits_set(PORTC,0x40)
#define yellow_off() bits_clr(PORTC,0x40)
#define yellow_change() bits_mask_rev(PORTC,0x40)

#define yellow_mode_out() bits_set(DDRC,0x40)
#define yellow_mode_in() bits_clr(DDRC,0x40)


#define leds_init() {yellow_mode_out(); green_mode_out(); red_mode_out();}


#endif