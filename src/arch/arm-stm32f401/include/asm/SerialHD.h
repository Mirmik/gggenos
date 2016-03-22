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

    //const delegate<void,void*> BreakEndDelegate = 
    //delegate<void,void*>(this, &Serial_HD_simple::break_end);

    char message[64];
    char* message_ptr;
    int message_len;
    int mode = 0;
    int count = 0;

    char answer[32];
    char* answer_ptr;
    char answer_term;
    uint8_t answer_len;

    GPIO_TypeDef* changedir_port;
    uint32_t changedir_pin;

    uint8_t flag;

    TimWaiter watchDog;

  public:
    HardwareSerialHD();
    
    void break_end(void*);
    void input_mode();
    void configure_session(char* _message, int len, char _answer_term);

    void start_session();
    void end_session();

    void irq_txe() volatile;
    void irq_tc() volatile;
}; 

#endif 