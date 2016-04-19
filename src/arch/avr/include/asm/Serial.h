#ifndef GENOS_SERIAL_ADAPTER_H
#define GENOS_SERIAL_ADAPTER_H

#include "cpu/usart_control_struct.h"
#include "genos/sigslot/delegate.h"
#include "utilxx/stub.h"
#include "genos/debug/debug.h"
#include "util/bits.h"
#include "avr/io.h"

class AdapterSerial 
{

    public:
    void putc(char c) { *usart->udr = c; };
    char getc() { return *usart->udr; };

    bool tx_ready() { return usart_udr_is_empty(usart); };
    
    void tx_interrupt_disable() { usart_udr_empty_isr_disable(usart); };
    void tx_interrupt_enable() { usart_udr_empty_isr_enable(usart); };
    void rx_interrupt_disable() { usart_rx_isr_disable(usart); };
    void rx_interrupt_enable() { usart_rx_isr_enable(usart); };
    
    bool correct_receive()
    {
        return bits_bit_is_clr(*usart->ucsr_a, UPE0);
    };

    //ресурсы:
    fastdelegate<void> sended;
    fastdelegate<void, char> recv;
    usart_regs* usart;   
};

extern AdapterSerial ASerial0;
extern AdapterSerial ASerial1;
extern AdapterSerial ASerial2;
extern AdapterSerial ASerial3;

#endif
