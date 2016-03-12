#include "cpu/usart_control_struct.h" 
#include "avr/io.h"

#include "genos/debug/debug.h"

void usart_drop(struct usart_regs* usart)
{
	*usart->ucsr_a = 0;
	*usart->ucsr_b = 0;
	*usart->ucsr_c = 0;
	*usart->ubrr_h = 0;
	*usart->ubrr_l = 0;	
};

void usart_baudrate_set(struct usart_regs* usart, uint32_t baud)
{
  *usart->ucsr_a |= 1 << U2X0;
  uint16_t baud_setting = (F_CPU / 4 / baud - 1) / 2;
  
  *usart->ubrr_h = baud_setting >> 8;
  *usart->ubrr_l = baud_setting;
};

#include "util/bits.h"
void usart_parity_set(struct usart_regs* usart, enum usartParityMode mode)
{
	bits_mask_assign_bias(*usart->ucsr_c, mode, 0b11, 4);
};

void usart_stopbits_set(struct usart_regs* usart, enum usartStopBitsMode mode)
{
	bits_mask_assign_bias(*usart->ucsr_c, mode, 0b1, 3);
};


void usart_databits_set(struct usart_regs* usart, enum usartDataBitsMode mode)
{
	bits_mask_assign_bias(*usart->ucsr_c, mode, 0b011, 1);
	bits_mask_assign(*usart->ucsr_b, mode, 0b100);
};


uint8_t usart_udr_is_empty(struct usart_regs* usart)
{
	return bits_bit_is_set(*usart->ucsr_a, UDRE0);
};

void usart_udr_empty_isr_enable(struct usart_regs* usart)
{
	bits_bit_set(*usart->ucsr_b, UDRIE0);
};
void usart_udr_empty_isr_disable(struct usart_regs* usart)
{
	bits_bit_clr(*usart->ucsr_b, UDRIE0);
};
void usart_rx_isr_enable(struct usart_regs* usart)
{
	bits_bit_set(*usart->ucsr_b, RXCIE0);
};
void usart_rx_isr_disable(struct usart_regs* usart)
{
	bits_bit_clr(*usart->ucsr_b, RXCIE0);
};
void usart_tx_end_isr_enable(struct usart_regs* usart)
{
	bits_bit_set(*usart->ucsr_b, TXCIE0);
};
void usart_tx_end_isr_disable(struct usart_regs* usart)
{
	bits_bit_clr(*usart->ucsr_b, TXCIE0);
};


void usart_rx_enable(struct usart_regs* usart)
{
	bits_bit_set(*usart->ucsr_b, RXEN0);
};
void usart_rx_disable(struct usart_regs* usart)
{
	bits_bit_clr(*usart->ucsr_b, RXEN0);
};


void usart_tx_enable(struct usart_regs* usart)
{
	bits_bit_set(*usart->ucsr_b, TXEN0);
};
void usart_tx_disable(struct usart_regs* usart)
{
	bits_bit_clr(*usart->ucsr_b, TXEN0);
};
