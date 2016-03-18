
#ifndef HardwareSerial_Half_duplex_h
#define HardwareSerial_Half_duplex_h

#include <inttypes.h>
#include "cpu/usart_control_struct.h"
#include "genos/io/stream.h"
#include "genos/kernel/waitserver.h"
#include "syscontext/syscontext.h"

void hd_rx_irq(void* data);
void hd_tx_irq(void* data);
void hd_udre_irq(void* data);

class Serial_HD_simple
{
  public:
    usart_regs* usart;

    const delegate<void,void*> BreakEndDelegate = 
    delegate<void,void*>(this, &Serial_HD_simple::break_end);

    char message[52];
    char* message_ptr;
    int message_len;
    int mode = 0;
    int count = 0;

    char answer[32];
    char* answer_ptr;
    char answer_term;
    uint8_t answer_len;

    uint8_t changedir_pin;
    uint8_t flag;

    TimWaiter watchDog;

  public:
    Serial_HD_simple();
    
    void connect(usart_regs* _usart, uint8_t _changedir_pin);

    void break_end(void*);
    void input_mode();
    void configure_session(char* _message, int len, char _answer_term);

    void start_session();
    void end_session();


}; 

#endif
