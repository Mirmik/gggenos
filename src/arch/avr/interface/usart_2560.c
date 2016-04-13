#include "cpu/usart_control_struct.h"
#include "avr/io.h"
#include "util/stub.h"

struct usart_regs usart0 = 
{
	&UBRR0H,
	&UBRR0L,
	&UCSR0A,
	&UCSR0B,
	&UCSR0C,
	&UDR0,
	do_nothing,
	do_nothing,
	do_nothing,
	do_nothing
};

struct usart_regs usart1 = 
{
	&UBRR1H,
	&UBRR1L,
	&UCSR1A,
	&UCSR1B,
	&UCSR1C,
	&UDR1,
	do_nothing,
	do_nothing,
	do_nothing,
	do_nothing
};

struct usart_regs usart2 = 
{
	&UBRR2H,
	&UBRR2L,
	&UCSR2A,
	&UCSR2B,
	&UCSR2C,
	&UDR2,
	do_nothing,
	do_nothing,
	do_nothing,
	do_nothing
};
	
struct usart_regs usart3 = 
{
	&UBRR3H,
	&UBRR3L,
	&UCSR3A,
	&UCSR3B,
	&UCSR3C,
	&UDR3,
	do_nothing,
	do_nothing,
	do_nothing,
	do_nothing
};

//ISR(USART0_RX_vect){usart0.rx_headler(usart0.data);};
//ISR(USART0_TX_vect){usart0.tx_headler(usart0.data);};
//ISR(USART0_UDRE_vect){usart0.udre_headler(usart0.data);};

//ISR(USART1_RX_vect){usart1.rx_headler(usart1.data);};
//ISR(USART1_TX_vect){usart1.tx_headler(usart1.data);};
//ISR(USART1_UDRE_vect){usart1.udre_headler(usart1.data);};

//ISR(USART2_RX_vect){usart2.rx_headler(usart2.data);};
//ISR(USART2_TX_vect){usart2.tx_headler(usart2.data);};
//ISR(USART2_UDRE_vect){usart2.udre_headler(usart2.data);};

//ISR(USART3_RX_vect){usart3.rx_headler(usart3.data);};
//ISR(USART3_TX_vect){usart3.tx_headler(usart3.data);};
//ISR(USART3_UDRE_vect){usart3.udre_headler(usart3.data);};