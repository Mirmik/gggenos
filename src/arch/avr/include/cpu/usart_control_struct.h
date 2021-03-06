#ifndef AVR_USART_CONTROL_STRUCT
#define AVR_USART_CONTROL_STRUCT

#include "sys/cdefs.h"
#include "inttypes.h"

//enum usartDirection
//{
//	usartDirectionInput,
//	usartDirectionOutput
//};

struct usart_regs {
	volatile uint8_t* ubrr_h;
	volatile uint8_t* ubrr_l;
	volatile uint8_t* ucsr_a;
	volatile uint8_t* ucsr_b;
	volatile uint8_t* ucsr_c;
	volatile uint8_t* udr;
	void* data;
	void (*rx_headler)(void* data);
	void (*tx_headler)(void* data);
	void (*udre_headler)(void* data);
	//void (*change_direction)(void* data, enum usartDirection);
};

extern struct usart_regs usart0;
extern struct usart_regs usart1;
extern struct usart_regs usart2;
extern struct usart_regs usart3;

enum usartParityMode
{
	usartParityDisabled = 0,
	usartParityEven = 2,
	usartParityOdd = 3
};

enum usartStopBitsMode
{
	usartOneStopBit = 0,
	usartTwoStopBit = 1	
};

enum usartDataBitsMode
{
	usartFiveDataBit = 0,
	usartSixDataBit = 1,
	usartSevenDataBit = 2,
	usartEightDataBit = 3,
	usartNineDataBit = 8
};

__BEGIN_DECLS

void usart_drop(struct usart_regs* usart);
void usart_baudrate_set(struct usart_regs* usart, uint32_t baud);
void usart_baudrate_get();

void usart_parity_set(struct usart_regs* usart, enum usartParityMode);
void usart_parity_get(struct usart_regs* usart, enum usartParityMode);

void usart_stopbits_set(struct usart_regs* usart, enum usartStopBitsMode);
void usart_stopbits_get(struct usart_regs* usart, enum usartStopBitsMode);

void usart_databits_set(struct usart_regs* usart, enum usartDataBitsMode);
void usart_databits_get(struct usart_regs* usart, enum usartDataBitsMode);

uint8_t usart_udr_is_empty(struct usart_regs* usart);


void usart_rx_enable(struct usart_regs* usart);
void usart_rx_disable(struct usart_regs* usart);

void usart_tx_enable(struct usart_regs* usart);
void usart_tx_disable(struct usart_regs* usart);

void usart_udr_empty_isr_enable(struct usart_regs* usart);
void usart_udr_empty_isr_disable(struct usart_regs* usart);
void usart_rx_isr_enable(struct usart_regs* usart);
void usart_rx_isr_disable(struct usart_regs* usart);
void usart_tx_end_isr_enable(struct usart_regs* usart);
void usart_tx_end_isr_disable(struct usart_regs* usart);

__END_DECLS

#endif