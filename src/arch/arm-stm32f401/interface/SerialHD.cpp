
#include "asm/SerialHD.h"
#include "hal/gpio.h"

HardwareSerialHD::HardwareSerialHD(){};

void HardwareSerialHD::configure_session(char* _message, int len, char _answer_term)
{
  memcpy(message, _message, len);
  message_len = len;
  answer_term = _answer_term;
  flag = 0;
  answer_len = 0;
  answer_ptr = answer;
  message_ptr = message;  
  mode = 0;
  count = 0;
};

void HardwareSerialHD::start_session()
{
  if (message_len == 0) return;

  gpio_port_set_mask(changedir_port, changedir_pin);
  flag = 0;  

  usart->DR = *message_ptr++;	 
	USART_ITConfig(USART6, USART_IT_RXNE, DISABLE);
  USART_ITConfig(USART6, USART_IT_TXE, ENABLE); 
  USART_ITConfig(USART6, USART_IT_TC, ENABLE);
};

void HardwareSerialHD::irq_txe() volatile
{
	usart->DR = *message_ptr++;
  	if (message_ptr - message >= message_len) USART_ITConfig(USART6, USART_IT_TXE, DISABLE); 
};

void HardwareSerialHD::irq_tc() volatile
{
  	gpio_port_clr_mask(changedir_port, changedir_pin);
	USART_ITConfig(USART6, USART_IT_TC, DISABLE);
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);
};