#ifndef HardwareSerial_h
#define HardwareSerial_h

#include <inttypes.h>
#include "genos/io/stream.h"
#include "stm32f4xx_gpio.h"

#define SERIAL_RX_BUFFER_SIZE 256
#define SERIAL_TX_BUFFER_SIZE 512

class HardwareSerial : public stream
{
  public:
    USART_TypeDef* usart;

    volatile uint16_t rx_head = 0;
    volatile uint16_t rx_tail = 0;

    volatile uint16_t tx_head = 0;
    volatile uint16_t tx_tail = 0;

    char rx_buffer[SERIAL_RX_BUFFER_SIZE];
    char tx_buffer[SERIAL_TX_BUFFER_SIZE];

  public:
    HardwareSerial();
    
    void connect(USART_TypeDef* _usart) 
    {
        
    };
    
    int putc(char t);
    int getc(void);
    int available(void);

    void irq_txe();
    void irq_rxne();
};

extern HardwareSerial Serial2; 

#endif
