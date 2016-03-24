
#include "asm/SerialHD.h"
#include "hal/gpio.h"
#include "genos/time/sysclock.h"
#include "syscontext/syscontext.h"

#define ERROR_REPEAT 40
#define TIMEOUT_RS485 7
#define BREAK_END 8

HardwareSerialHD::HardwareSerialHD(){};


void HardwareSerialHD::break_end(void*)
{
  //USART_ITConfig(USART6, USART_IT_RXNE, DISABLE);
  USART_ITConfig(USART6, USART_IT_TXE, DISABLE); 
  USART_ITConfig(USART6, USART_IT_TC, DISABLE);
 
  //debug_print("\r\nRS_485_BreakEnd");
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
  //error_count = 0;
	
	rx_mode = 0;
  
  char drop = usart->DR; 
  flag = 0;  
  //USART_ITConfig(USART6, USART_IT_RXNE, DISABLE);
  USART_ITConfig(USART6, USART_IT_TXE, ENABLE); 
  USART_ITConfig(USART6, USART_IT_TC, ENABLE);
  //USART_ITConfig(USART6, USART_IT_RXNE, DISABLE);
};


void HardwareSerialHD::restart_session()
{
  char drop = usart->DR;
  answer_len = 0;
  answer_ptr = answer;
  message_ptr = message;  
  rx_mode = 0;
  flag = 0;
  mode = 0;
  count = 0;
  USART_ITConfig(USART6, USART_IT_TXE, ENABLE); 
  USART_ITConfig(USART6, USART_IT_TC, ENABLE);
  //USART_ITConfig(USART6, USART_IT_RXNE, DISABLE);
};

void HardwareSerialHD::end_session()
{
  waitserver.unwait(&watchDog);
  
  //USART_ITConfig(USART6, USART_IT_RXNE, DISABLE); 
  USART_ITConfig(USART6, USART_IT_TXE, DISABLE); 
  USART_ITConfig(USART6, USART_IT_TC, DISABLE);
  *answer_ptr = 0;
  rx_mode = 0;
  gpio_port_set_mask(changedir_port, changedir_pin);
};

void HardwareSerialHD::success_check_result()
{
//debug_print("HERE");
//debug_putchar(answer[2]);

	//debug_print("\r\nsuccesscheck:");
	//	debug_write(answer, answer_len);
	//	debug_print("\t");
	//	debug_printdec_uint32(answer_len);
		
  if (answer_len >= 50) {bad_session(0xF7); return;};
  if (answer[2] != 'A') {bad_session(0xF6); return;};
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

void HardwareSerialHD::bad_session(uint8_t f)
{
  flag = f;
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
	waitserver.delegate_on_simple_timer(BreakEndDelegate, (void*)0, &watchDog, BREAK_END);
  	USART_ITConfig(USART6, USART_IT_TC, DISABLE);
  	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);   
};

void HardwareSerialHD::irq_rxne()
{
	char* ptr;
	int zerolen;
	volatile unsigned char c = usart->DR;
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
      answer_len = answer_len - zerolen;
      end_session();
      success_check_result();
    };
};

extern HardwareSerialHD SerialHD6;
void SerialHDDriver::broken_session()
{
	//debug_print("\r\nerrcount++:");
	//debug_printdec_uint32(SerialHD6.flag);
	error_count++;
	if (error_count == ERROR_REPEAT) 
		{
			
			debug_print("\n\r");
	  		debug_print("flag:"); debug_printdec_int8(SerialHD6.flag); dln;
  			debug_print("mes:"); debug_write(SerialHD6.message,SerialHD6.message_len);dln; 
  			debug_print("ans:"); debug_write(SerialHD6.answer,SerialHD6.answer_len);dln;
  			debug_print("ans[2]:"); debug_putchar(SerialHD6.answer[2]);dln;


			debug_panic("SerialHDDriverErrorCount");
		};
	SerialHD6.restart_session();
	wait_autom(&SerialHD6.flag);	
};

#include "genos/debug/debug.h"
void SerialHDDriver::exec()
{
dlist_head  *ptr;
	switch(state)
	{
		case 0:
		error_count = 0;
			
		if (dlist_empty(&list)) return;
		
		ptr = list.next;
		task = dlist_entry(ptr, Task, list);


		if (task->flag == 0)
		{
		//debug_print("task:"); debug_printhex_uint32((uint32_t) task);dln;
			SerialHD6.configure_session(task->message , task->message_len, 3);
			SerialHD6.callback = task->callback;
			SerialHD6.start_session();
			state = 1;
			wait_autom(&SerialHD6.flag);
			return;
		}
		else {dlist_move_tail(dlist_next(&list), &list); return;}

		debug_panic("???");
		break;

		case 1:

		//debug_printhex_uint32(SerialHD6.flag);
		//debug_write(SerialHD6.answer, SerialHD6.answer_len);
			//debug_print("flag:");debug_printdec_uint32(SerialHD6.flag);
		if (SerialHD6.flag != 1)
		{
			//debug_print("here");
			state = 78;
			msleep_autom(TIMEOUT_RS485);
			return;
		};


		//debug_print("here");

		memcpy(task->answer, SerialHD6.answer, SerialHD6.answer_len);
		task->answer_len = SerialHD6.answer_len;
		task->flag = 1;
		dlist_move_tail(dlist_next(&list), &list);
		state = 2;
		 if (error_count > max_error) max_error = error_count; 
		error_count = 0;
		
		//debug_print("flag:");
		//debug_printdec_uint32();
		break;

		case 2:
		//debug_print("here");
		msleep_autom(TIMEOUT_RS485);
		state = 0;
		break;

		case 78:

		//debug_printdec_uint32(error_count);
		broken_session();
		state =1;

		break;

	};
};





