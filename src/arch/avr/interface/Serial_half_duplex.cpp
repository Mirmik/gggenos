
#include "asm/Serial_half_duplex.h"
#include "util/bits.h"
#include "util/ring.h"
#include "defines/decltypeof.h"
#include "avr/io.h"

#include "string.h"

Serial_HD_simple::Serial_HD_simple() {};

void Serial_HD_simple::break_end(void*)
{
  usart_rx_disable(usart);
  usart_tx_disable(usart);
  usart_udr_empty_isr_disable(usart);
  usart_rx_isr_disable(usart);
  usart_tx_end_isr_disable(usart);

  *flag = -1;
  stdout.print("RS_485_BreakEnd");
};

void hd_rx_irq(void* data)
{
  	
};

void Serial_HD_simple::input_mode()
{
  waitserver.delegate_on_simple_timer(BreakEndDelegate, (void*)0, &watchDog, 100);
  usart_tx_disable(usart);
  usart_udr_empty_isr_disable(usart);
  usart_tx_end_isr_disable(usart);
  usart_rx_enable(usart);
  usart_rx_isr_enable(usart);
};
 

void hd_udre_irq(void* data)
{
  Serial_HD_simple* ser = reinterpret_cast<Serial_HD_simple*>(data);
  usart_regs* usart = ser->usart;

  *usart->udr = *ser->message_ptr++;
  if (ser->message_ptr - ser->message >= ser->message_len) usart_udr_empty_isr_disable(usart);
  //stdout.print("U");
};

void hd_tx_irq(void* data)
{
  Serial_HD_simple* ser = reinterpret_cast<Serial_HD_simple*>(data);
  ser->input_mode();
  //stdout.print("T");
};

void Serial_HD_simple::end_session()
{

};

void Serial_HD_simple::start_session()
{
  if (message_len == 0) return;
  usart_tx_enable(usart);
  usart_udr_empty_isr_enable(usart);
  usart_tx_end_isr_enable(usart);
};

void Serial_HD_simple::configure_session(char* _message, int len, char _answer_term, uint8_t* _flag)
{
  memcpy(message, _message, len);
  message_len = len;
  answer_term = _answer_term;
  flag = _flag;
  *flag = 0;
  answer_len = 0;
  answer_ptr = answer;
  message_ptr = message;  
  usart_rx_disable(usart);
  usart_tx_disable(usart);
  usart_udr_empty_isr_disable(usart);
  usart_rx_isr_disable(usart);
  usart_tx_end_isr_disable(usart);
};

void Serial_HD_simple::connect(usart_regs* _usart, uint8_t _changedir_pin) 
{
    usart = _usart;
    usart->data = (void*) this;
    usart->rx_headler = hd_rx_irq;
    usart->udre_headler = hd_udre_irq;
    usart->tx_headler = hd_tx_irq;
    changedir_pin = _changedir_pin;
};