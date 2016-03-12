
#ifndef HardwareSerial_h
#define HardwareSerial_h

#include <inttypes.h>
#include "cpu/usart_control_struct.h"
#include "genos/io/stream.h"

void rx_irq(void* data);
void udre_irq(void* data);

class HardwareSerial : public stream
{
  public:
     usart_regs* usart;

#if SERIAL_RX_BUFFER_SIZE < 256
    volatile uint8_t rx_head = 0;
    volatile uint8_t rx_tail = 0;
#else
    volatile uint16_t rx_head = 0;
    volatile uint16_t rx_tail = 0;
#endif
#if SERIAL_TX_BUFFER_SIZE < 256
    volatile uint8_t tx_head = 0;
    volatile uint8_t tx_tail = 0;
#else
    volatile uint16_t tx_head = 0;
    volatile uint16_t tx_tail = 0;
#endif

    char rx_buffer[SERIAL_RX_BUFFER_SIZE];
    char tx_buffer[SERIAL_TX_BUFFER_SIZE];

  public:
    HardwareSerial();
    
    void connect(usart_regs* _usart) 
    {
        usart = _usart;
        usart->data = (void*) this;
        usart->rx_headler = rx_irq;
        usart->udre_headler = udre_irq;
    };
    
    int putc(char t);
    int getc(void);
    int available(void);
    //int peek(void);
    //int flush(void);
};


extern HardwareSerial Serial0; 

#endif
