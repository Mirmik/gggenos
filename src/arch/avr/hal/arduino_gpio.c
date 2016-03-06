
#include "hal/gpio.h"
#include "inttypes.h"
#include "avr/io.h"

struct regs_trait_t {
	volatile uint8_t* port;
	volatile uint8_t* pin;
	volatile uint8_t* ddr;
};

static struct regs_trait_t Bregs = 
{
	&PORTB,
	&PINB,
	&DDRB
};


static struct regs_trait_t Cregs = 
{	
	&PORTC,
	&PINC,
	&DDRC
};

static struct regs_trait_t Dregs = 
{
	&PORTD,
	&PIND,
	&DDRD
};

struct gpio_t
{
	struct regs_trait_t * regs;
	uint8_t offset;
};


struct gpio_t gpio_table[] =
{
	{&Dregs, 0}, //0 //RX
	{&Dregs, 1}, //1	
	{&Dregs, 2}, //2
	{&Dregs, 3}, //3
	{&Dregs, 4}, //4
	{&Dregs, 5}, //5
	{&Dregs, 6}, //6
	{&Dregs, 7}, //7
	{&Bregs, 0}, //8
	{&Bregs, 1}, //9
	{&Bregs, 2}, //10
	{&Bregs, 3}, //11
	{&Bregs, 4}, //12
	{&Bregs, 5}, //13
};

#define RPORT *gpio_table[gpio_number].regs->port
#define RDDR *gpio_table[gpio_number].regs->ddr
#define RPIN *gpio_table[gpio_number].regs->pin 
#define ROFFSET gpio_table[gpio_number].offset

#include "genos/debug/debug.h"

void gpio_mode_out(int gpio_number) {RDDR |= 1 << ROFFSET;};
void gpio_mode_in(int gpio_number) {RDDR &= ~(1 << ROFFSET);};
void gpio_mode_mode(int gpio_number) {debug_panic("gpio_stub");};

void gpio_hi(int gpio_number) {RPORT |= 1 << ROFFSET;};
void gpio_lo(int gpio_number) {RPORT &= ~(1 << ROFFSET);};
void gpio_change(int gpio_number) {debug_panic("gpio_stub");};
void gpio_level(int gpio_number, uint8_t level) {debug_panic("gpio_stub");};