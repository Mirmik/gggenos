
#include "asm/SerialHD.h"
#include "hal/gpio.h"
#include "genos/time/sysclock.h"
#include "syscontext/syscontext.h"

HardwareSerialHD::HardwareSerialHD(){};


void HardwareSerialHD::break_end(void*)
{
  //USART_ITConfig(USART6, USART_IT_RXNE, DISABLE);
  USART_ITConfig(USART6, USART_IT_TXE, DISABLE); 
  USART_ITConfig(USART6, USART_IT_TC, DISABLE);
 
  //stdout.print("RS_485_BreakEnd");
  end_session();
  bad_session(-1);
};


void HardwareSerialHD::configure_session(char* _message, int len, char _answer_term)
{
USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);
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


void HardwareSerialHD::print_answer(void*)
{
	stdout.write(answer, answer_len);	
};

void HardwareSerialHD::start_session()
{
  if (message_len == 0) return;
  error_count = 0;

  char drop = usart->DR; 
  flag = 0;  
  //USART_ITConfig(USART6, USART_IT_RXNE, DISABLE);
  USART_ITConfig(USART6, USART_IT_TXE, ENABLE); 
  USART_ITConfig(USART6, USART_IT_TC, ENABLE);
};


void HardwareSerialHD::restart_session()
{
  char drop = usart->DR;
  answer_len = 0;
  answer_ptr = answer;
  message_ptr = message;  
  mode = 0;
  count = 0;
  USART_ITConfig(USART6, USART_IT_TXE, ENABLE); 
  USART_ITConfig(USART6, USART_IT_TC, ENABLE);
};

void HardwareSerialHD::end_session()
{
  waitserver.unwait(&watchDog);
  
  USART_ITConfig(USART6, USART_IT_TXE, DISABLE); 
  USART_ITConfig(USART6, USART_IT_TC, DISABLE);
  *answer_ptr = 0;
  rx_mode = 0;
  gpio_port_set_mask(changedir_port, changedir_pin);
};

void HardwareSerialHD::success_check_result()
{
  if (answer_len >= 50) bad_session(-3);
  if (answer[2] != 'A') bad_session(-4);
  success_session();
};

#include "util/stub.h"
void HardwareSerialHD::drop_callback()
{
	callback_data = (void*)0;
	callback = (void(*)(void*))do_nothing;
};

void HardwareSerialHD::print_callback()
{
	callback_data = (void*)0;
	callback = delegate_mtd(this, &HardwareSerialHD::print_answer);
};

void HardwareSerialHD::success_session()
{
  callback(callback_data);
  flag = 1;
};

void HardwareSerialHD::bad_session(int f)
{
//debug_print("BadSession485"); 
  error_count++;
  if (error_count == 8) { 

  	debug_print("\n\r");
  	debug_print("flag:"); debug_printdec_int8(f); dln;
  	debug_print("mes:"); debug_write(message,message_len);dln; 
  	debug_print("ans:"); debug_write(answer,answer_len);dln;
  	debug_print("ans[2]:"); debug_putchar(answer[2]);dln;


  	debug_print("\n\r");

	debug_panic("BadSession485"); 

  }
  else restart_session();
};



void HardwareSerialHD::irq_txe()
{
	usart->DR = *message_ptr++;
  	if (message_ptr - message >= message_len) USART_ITConfig(USART6, USART_IT_TXE, DISABLE); 
};

void HardwareSerialHD::irq_tc() 
{
	gpio_port_clr_mask(changedir_port, changedir_pin);
	rx_mode = 1;
	waitserver.delegate_on_simple_timer(BreakEndDelegate, (void*)0, &watchDog, 18);
  	USART_ITConfig(USART6, USART_IT_TC, DISABLE);
};

void HardwareSerialHD::irq_rxne()
{
	char* ptr;
	int zerolen;
	unsigned char c = usart->DR;
	if (rx_mode == 0) return;

	//debug_print("irq_rxne");
	if (usart->SR & (USART_FLAG_NE|USART_FLAG_FE|USART_FLAG_PE|USART_FLAG_ORE)) {debug_panic("USART_ERROR_6_1");};

    
    *answer_ptr++ = c;
    answer_len++;
    if (answer_len >= 50) {end_session(); success_check_result();};
    if (c == answer_term || mode == 1)
    {count++; mode = 1;} 
    if (count == 3) 
    {
      ptr = answer;
      while(*ptr == 0) {ptr++;};
      zerolen = ptr - answer;
      memmove(answer, ptr, answer_len -zerolen);
      answer_len = answer_len -zerolen;
      end_session();
      success_check_result();
    };
};