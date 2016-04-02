
#include "asm/SerialHD.h"
#include "hal/gpio.h"
#include "genos/time/sysclock.h"
#include "syscontext/syscontext.h"

#define ERROR_REPEAT 40
#define TIMEOUT_RS485 2
#define BREAK_END 20

HardwareSerialHD::HardwareSerialHD(){};


void HardwareSerialHD::break_end(void*)
{
	rx_mode = 0;
  //USART_ITConfig(USART6, USART_IT_RXNE, DISABLE);
  USART_ITConfig(USART6, USART_IT_TXE, DISABLE); 
  USART_ITConfig(USART6, USART_IT_TC, DISABLE);
 
  //debug_print("\r\nRS_485_BreakEnd");
  end_session();
  bad_session(-1);
};


void HardwareSerialHD::configure_session(char* _message, int len, char _answer_term)
{
	rx_mode = 0;
USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);
  memcpy(message, _message, len);
  message_len = len;
  answer_term = _answer_term;
  flag = 0;
  answer = answer_buff; 
  answer_len = 0;
  answer_ptr = answer;
  message_ptr = message;  
  mode = 0;
  count = 0;
};


void HardwareSerialHD::print_answer(void*)
{
	stdout.write((char*)answer, answer_len);	
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
  answer = answer_buff;
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

void HardwareSerialHD::end_session() volatile
{
  waitserver.unwait((TimWaiter*)&watchDog);
  
  //USART_ITConfig(USART6, USART_IT_RXNE, DISABLE); 
  USART_ITConfig(USART6, USART_IT_TXE, DISABLE); 
  USART_ITConfig(USART6, USART_IT_TC, DISABLE);
  *answer_ptr = 0;
  rx_mode = 0;
  gpio_port_set_mask(changedir_port, changedir_pin);
};

void HardwareSerialHD::success_check_result() volatile
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

void HardwareSerialHD::success_session() volatile
{
//	debug_print(answer);
  ((delegate<void,void*>)callback)(callback_data);
  flag = 1;
};

void HardwareSerialHD::bad_session(uint8_t f) volatile
{
  flag = f;
};



void HardwareSerialHD::irq_txe() volatile
{
	usart->DR = *message_ptr++;
  	if (message_ptr - message >= message_len) USART_ITConfig(USART6, USART_IT_TXE, DISABLE); 
};

void HardwareSerialHD::irq_tc()  volatile
{
	gpio_port_clr_mask(changedir_port, changedir_pin);
	rx_mode = 1;
	waitserver.delegate_on_simple_timer(BreakEndDelegate, (void*)0, (TimWaiter*)&watchDog, BREAK_END);
  	USART_ITConfig(USART6, USART_IT_TC, DISABLE);
  	//USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);   
};

void HardwareSerialHD::irq_rxne() volatile
{
	volatile char* ptr;
	int zerolen;
	volatile unsigned char c = usart->DR;
	if (rx_mode == 0) return;

	//debug_print("irq_rxne");
	if (usart->SR & (USART_FLAG_NE|USART_FLAG_FE|USART_FLAG_PE|USART_FLAG_ORE)) {debug_panic("USART_ERROR_6_1");};

    
    *answer_ptr++ = c;
    answer_len++;
    if (answer_len >= 50) {end_session(); success_check_result();};
    if (c == answer_term || mode == 1)
    {
    	++count; 
    	mode = 1;
    	do_nothing();
      	//debug_print("here"); 
    	if (count >= 3) 
    	{
      		//debug_printdec_uint32(ans_len);

      		answer = answer_buff;
      	//debug_print("here");
      		ptr = answer;
      		while(*ptr != 2) {ptr++;};
      		answer = ptr;
      		answer_len = answer_len - (answer - answer_buff);
      		end_session();
      		success_check_result();
    	};
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
  			debug_print("ans:"); debug_write((char*)SerialHD6.answer,SerialHD6.answer_len);dln;
  			debug_print("dmp:"); debug_print_dump((void*)SerialHD6.answer,SerialHD6.answer_len);dln;
  			debug_print("ans[2]:"); debug_putchar(((char*)SerialHD6.answer)[2]);dln;


			debug_panic("SerialHDDriverErrorCount");
		};
	SerialHD6.restart_session();
	wait_subst(&SerialHD6.flag);	
};

#include "genos/debug/debug.h"
void SerialHDDriver::exec()
{
dlist_head  *ptr;

while(1)
{

	search:

	while (dlist_empty(&list)) msleep_subst(1);;
		
	ptr = list.next;
	task = dlist_entry(ptr, Task, list);


	if (task->flag == 0)
	{
		//debug_print("task:"); debug_printhex_uint32((uint32_t) task);dln;
		SerialHD6.configure_session(task->message , task->message_len, 3);
		SerialHD6.callback = task->callback;
		SerialHD6.start_session();
		wait_subst(&SerialHD6.flag);
		goto start;
	}
	else 
	{
		dlist_move_tail(dlist_next(&list), &list); 
		msleep_subst(1);
		goto search;
	};

	debug_panic("???");

	start:
	if (SerialHD6.flag != 1)
		{
			goto error;
		};

		memcpy(task->answer, (char*)SerialHD6.answer, SerialHD6.answer_len);
		task->answer_len = SerialHD6.answer_len;
		task->flag = 1;
		dlist_move_tail(dlist_next(&list), &list);
		if (error_count > max_error) max_error = error_count; 
		error_count = 0;

//debug_print("2");		
		msleep_subst(TIMEOUT_RS485);
//debug_print("1");

		goto search;

	error:
		gpio_port_set_mask(SerialHD6.changedir_port, SerialHD6.changedir_pin);
		msleep_subst(1);
		USART6->DR = 3;
		msleep_subst(1);
		broken_session();
		goto start;		


	debug_panic("???");	
};
};





