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
    char* message_ptr;
    int message_len;
    int mode = 0;
    int count = 0;

    uint8_t rx_mode = 0;

    char answer[32];
    char* answer_ptr;
    char answer_term;
    uint8_t answer_len;

    GPIO_TypeDef* changedir_port;
    uint32_t changedir_pin;

    uint8_t flag;
    uint8_t error_count;

    TimWaiter watchDog;

  public:
    HardwareSerialHD();
    
    void break_end(void*);
    void input_mode();
    void configure_session(char* _message, int len, char _answer_term);

    void print_answer(void*);

    void start_session();
    void restart_session();
    void end_session(); 

	void success_check_result();
	void success_session();
	void bad_session(int f);


    void irq_txe();
    void irq_tc();
    void irq_rxne();
}; 

#endif 