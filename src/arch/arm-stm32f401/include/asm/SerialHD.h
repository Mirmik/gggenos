#ifndef HardwareSerialHD_h
#define HardwareSerialHD_h

#include "genos/sigslot/delegate.h"
#include "genos/kernel/waitserver.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_gpio.h"

class HardwareSerialHD
{
  public:
    USART_TypeDef* usart;

    const delegate<void,void*> BreakEndDelegate = 
    delegate<void,void*>(this, &HardwareSerialHD::break_end);

    delegate<void, void*> callback =
    delegate<void, void*>(this, &HardwareSerialHD::print_answer);

    void drop_callback();
    void print_callback();

    void* callback_data = 0;

    char message[64];
    volatile char* volatile message_ptr;
    volatile int message_len;
    volatile int mode = 0;
    volatile int count = 0;

    volatile uint8_t rx_mode = 0;

    volatile char answer_buff[64];
    volatile char* volatile answer = answer_buff;
    volatile char* volatile answer_ptr;
    char answer_term;
    volatile uint8_t answer_len;

    GPIO_TypeDef* changedir_port;
    uint32_t changedir_pin;

    uint8_t flag;

    TimWaiter watchDog;

  public:
    HardwareSerialHD();
    
    void break_end(void*);
    void input_mode();
    void configure_session(char* _message, int len, char _answer_term);

    void print_answer(void*);

    void start_session();
    void restart_session();
    void end_session()  volatile; 

	void success_check_result() volatile;
	void success_session()  volatile;
	void bad_session(uint8_t f)  volatile;


    void irq_txe() volatile;
    void irq_tc() volatile;
    void irq_rxne() volatile;
}; 


#include "util/dlist.h"
class SerialHDDriver
{
public:
	volatile int error_count;
	uint8_t max_error;
	dlist_head list;

	class Task
	{
	public:

	dlist_head list;
	char message[64];
    int message_len;
    char answer[32];
    uint8_t answer_len;
    uint8_t flag = 8;
    delegate<void, void*> callback = (void(*)(void*))do_nothing;
	};
Task* task;

void exec();
void broken_session();
void active(Task* _task) {_task->flag = 0;}

int state = 0;

};









#endif 