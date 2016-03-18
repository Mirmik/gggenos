
#include "asm/Serial_half_duplex.h"
#include "util/bits.h"
#include "util/ring.h"
#include "defines/decltypeof.h"
#include "avr/io.h"
#include "hal/gpio.h"
#include "string.h"

Serial_HD_simple::Serial_HD_simple() {};


void Serial_HD_simple::break_end(void*)
{
  usart_rx_disable(usart);
  usart_tx_disable(usart);
  usart_udr_empty_isr_disable(usart);
  usart_rx_isr_disable(usart);
  usart_tx_end_isr_disable(usart);

  flag = -1;
  stdout.printhex(flag);
  stdout.print("RS_485_BreakEnd");
};

void hd_rx_irq(void* data)
{
  	Serial_HD_simple* serial = reinterpret_cast<Serial_HD_simple*>(data);
    usart_regs* usart = serial->usart;

     if (bits_bit_is_clr(*usart->ucsr_a, UPE0)) 
     {
      unsigned char c = *usart->udr;
      *serial->answer_ptr++ = c;
      serial->answer_len++;
      if (serial->answer_len >= 50) serial->end_session();
      if (c == serial->answer_term || serial->mode == 1)
      {serial->count++; serial->mode = 1;} 
      if (serial->count == 3) 
      {
        serial->end_session();
      };
    } 
      else 
      {
       debug_print("Parrity_error");
       // Parity error, read byte but discard it
       *usart->udr;
      };
};

void Serial_HD_simple::input_mode()
{
  waitserver.delegate_on_simple_timer(BreakEndDelegate, (void*)0, &watchDog, 100);

  usart_tx_disable(usart);
  usart_udr_empty_isr_disable(usart);
  usart_tx_end_isr_disable(usart);

  usart_rx_enable(usart);
  usart_rx_isr_enable(usart);
  gpio_lo(changedir_pin);


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
  waitserver.unwait(&watchDog);
  usart_rx_disable(usart);
  usart_tx_disable(usart);
  usart_udr_empty_isr_disable(usart);
  usart_rx_isr_disable(usart);
  usart_tx_end_isr_disable(usart);
  *answer_ptr = 0;
        
  char v = *usart->udr;
  char* ptr = answer;
  uint8_t i = 0;
  while(*ptr++ == 0) i++;
  memmove(answer, ptr, answer_len - i);

  if (answer_len >= 50) {flag = -3; return;}; 

  flag = 1;

};

void Serial_HD_simple::start_session()
{
  if (message_len == 0) return;

  gpio_hi(changedir_pin);
  flag = 0;  
  usart_tx_enable(usart);
  usart_udr_empty_isr_enable(usart);
  usart_tx_end_isr_enable(usart);
};

void Serial_HD_simple::configure_session(char* _message, int len, char _answer_term)
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
  

  gpio_mode_out(changedir_pin);

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