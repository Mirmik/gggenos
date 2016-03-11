#ifndef AVR_USART_STREAM
#define AVR_USART_STREAM

#include "usart_control_struct.h"
#include "genos/io/stream.h"

#define 

class AvrUsartStream : public stream
{
	usart_regs* _usart;


public:
	AvrUsartStream(usart_regs* usart): _usart(usart) {};

	volatile uint8_t _rx_buffer_head;
    volatile uint8_t _rx_buffer_tail;
    volatile uint8_t _tx_buffer_head;
    volatile uint8_t _tx_buffer_tail;

    unsigned char _rx_buffer[SERIAL_RX_BUFFER_SIZE];
    unsigned char _tx_buffer[SERIAL_TX_BUFFER_SIZE];

    int available(void);
    int peek(void);
    int getc(void);
    int flush(void);
    int putc(char t);
    
    inline void _rx_complete_irq(void);
    void _tx_udr_empty_irq(void);
}; 	
};


#endif