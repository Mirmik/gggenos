#include "hal/gpio.h"
#include "inttypes.h"
#include "avr/io.h"

struct regs_trait_t {
	volatile uint8_t* port;
	volatile uint8_t* pin;
	volatile uint8_t* ddr;
};

static struct regs_trait_t A = 
{
	&PORTA,
	&PINA,
	&DDRA
};

static struct regs_trait_t B = 
{
	&PORTB,
	&PINB,
	&DDRB
};


static struct regs_trait_t C = 
{	
	&PORTC,
	&PINC,
	&DDRC
};

static struct regs_trait_t D = 
{
	&PORTD,
	&PIND,
	&DDRD
};

static struct regs_trait_t E = 
{
	&PORTE,
	&PINE,
	&DDRE
};


static struct regs_trait_t F = 
{
	&PORTF,
	&PINF,
	&DDRF
};


static struct regs_trait_t G = 
{
	&PORTG,
	&PING,
	&DDRG
};


static struct regs_trait_t H = 
{
	&PORTH,
	&PINH,
	&DDRH
};

static struct regs_trait_t J = 
{
	&PORTJ,
	&PINJ,
	&DDRJ
};

static struct regs_trait_t K = 
{
	&PORTK,
	&PINK,
	&DDRK
};

static struct regs_trait_t L = 
{
	&PORTL,
	&PINL,
	&DDRL
};

struct gpio_t
{
	struct regs_trait_t * regs;
	uint8_t offset;
};

struct gpio_t gpio_table[] =
{
	{&E, 0}, //0 //RX0
	{&E, 1}, //1 //TX0	
	{&E, 4}, //2 
	{&E, 5}, //3
	{&G, 5}, //4
	{&E, 3}, //5
	{&H, 3}, //6
	{&H, 4}, //7
	{&H, 5}, //8
	{&H, 6}, //9
	{&B, 4}, //10
	{&B, 5}, //11
	{&B, 6}, //12
	{&B, 7}, //13		//RED_LED
	{&J, 1}, //14 //TX3
	{&J, 0}, //15 //RX3
	{&H, 1}, //16 //TX2
	{&H, 0}, //17 //RX2
	{&D, 3}, //18 //TX1 
	{&D, 2}, //19 //RX1
	{&D, 1}, //20 //SDA
	{&D, 0}, //21 //SCL
	{&A, 0}, //22
	{&A, 1}, //23
	{&A, 2}, //24
	{&A, 3}, //25
	{&A, 4}, //26
	{&A, 5}, //27
	{&A, 6}, //28
	{&A, 7}, //29
	{&C, 7}, //30		//AIOP : GREEN
	{&C, 6}, //31		//AIOP : YELLOW
	{&C, 5}, //32
	{&C, 4}, //33
	{&C, 3}, //34
	{&C, 2}, //35
	{&C, 1}, //36
	{&C, 0}, //37
	{&D, 7}, //38
	{&G, 2}, //39
	{&G, 1}, //40 		
	{&G, 0}, //41 		
	{&L, 7}, //42
	{&L, 6}, //43
	{&L, 5}, //44
	{&L, 4}, //45
	{&L, 3}, //46
	{&L, 2}, //47
	{&L, 1}, //48
	{&L, 0}, //49
	{&B, 3}, //50
	{&B, 2}, //51
	{&B, 1}, //52
	{&B, 0}, //53
};

#define RPORT *gpio_table[gpio_number].regs->port
#define RDDR *gpio_table[gpio_number].regs->ddr
#define RPIN *gpio_table[gpio_number].regs->pin 
#define ROFFSET gpio_table[gpio_number].offset

#include "genos/debug/debug.h"
#include "genos/datastruct/bits.h"

void gpio_mode_out(int gpio_number) {RDDR |= 1 << ROFFSET;};
void gpio_mode_in(int gpio_number) {RDDR &= ~(1 << ROFFSET);};
//void gpio_mode_mode(int gpio_number) {debug_panic("gpio_stub");};

void gpio_hi(int gpio_number) {RPORT |= 1 << ROFFSET;};
void gpio_lo(int gpio_number) {RPORT &= ~(1 << ROFFSET);};
void gpio_change(int gpio_number) {bits_mask_rev(RPORT, (1 << ROFFSET));};
void gpio_level(int gpio_number, uint8_t level) {debug_panic("gpio_stub");};