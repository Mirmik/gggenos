#include "asm/Serial.h"
#include "util/ring.h"
#include "genos/debug/debug.h"
#include "stm32f4xx_usart.h"

HardwareSerial::HardwareSerial(){};

int HardwareSerial::putc(char c)
{
if (
	ring_empty(tx_head, tx_tail) && 
	(USART_GetFlagStatus(usart, USART_FLAG_TC) == SET)
	) 
  {
    usart->DR = c;
    return 1;
  }
  
  while (ring_full(tx_head, tx_tail, SERIAL_TX_BUFFER_SIZE)) 
  {
    debug_panic("HardwareSerial Full");
  };

  ring_putc(tx_head, tx_tail, tx_buffer, SERIAL_TX_BUFFER_SIZE, c);
  USART_ITConfig(usart, USART_IT_TXE, ENABLE);

  return 1;  
};

int HardwareSerial::getc()
{
	return ring_getc(rx_head, rx_tail, rx_buffer, SERIAL_RX_BUFFER_SIZE);
};
int HardwareSerial::available()
{
	return ring_available_to_getc(rx_head, rx_tail, SERIAL_RX_BUFFER_SIZE);
};


void HardwareSerial::irq_txe()
{
	if (ring_empty(tx_head, tx_tail)) USART_ITConfig(usart, USART_IT_TXE, DISABLE);
	else 
		{
			USART_SendData(usart, ring_getc(tx_head, tx_tail, tx_buffer, SERIAL_TX_BUFFER_SIZE));
		};
};

void HardwareSerial::irq_rxne()
{
	if (usart->SR & (USART_FLAG_NE|USART_FLAG_FE|USART_FLAG_PE|USART_FLAG_ORE)) {char drop = usart->DR; return;};
	if (ring_full(rx_head, rx_tail, SERIAL_RX_BUFFER_SIZE))	{debug_panic("USART_ERROR_2");};
	ring_putc(rx_head, rx_tail, rx_buffer, SERIAL_RX_BUFFER_SIZE, usart->DR & 0xFF);
};