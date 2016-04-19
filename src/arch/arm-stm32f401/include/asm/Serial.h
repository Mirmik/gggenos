#ifndef GENOS_SERIAL_ADAPTER_H
#define GENOS_SERIAL_ADAPTER_H

#include "stm32f4xx_usart.h"
#include "misc.h"
//#include "genos/io/Serial.h"
#include "genos/sigslot/delegate.h"
#include "utilxx/stub.h"
#include "genos/debug/debug.h"

class AdapterSerial
{
public:
	void putc(char c) { usart->DR = c; };
	char getc() { return usart->DR; };

	bool tx_ready() { return USART_GetFlagStatus(usart, USART_FLAG_TC) == SET; };
	
	void tx_interrupt_disable() { USART_ITConfig(usart, USART_IT_TXE, DISABLE); };
	void tx_interrupt_enable() { USART_ITConfig(usart, USART_IT_TXE, ENABLE); };
	void rx_interrupt_disable() { USART_ITConfig(usart, USART_IT_RXNE, DISABLE); };
	void rx_interrupt_enable() { USART_ITConfig(usart, USART_IT_RXNE, ENABLE); };
	
	bool correct_receive()
	{
		return !(usart->SR & (
			USART_FLAG_NE |
			USART_FLAG_FE |
			USART_FLAG_PE |
			USART_FLAG_ORE
			));
	};

	//ресурсы:
	fastdelegate<void> sended;
	fastdelegate<void> recv;
    USART_TypeDef* usart;	
};

#endif