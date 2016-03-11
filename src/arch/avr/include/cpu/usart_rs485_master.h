#ifndef AVR_USART_STREAM
#define AVR_USART_STREAM

#include "usart_control_struct.h"
#include "genos/io/stream.h"

#define 

//HalfDuplexPacketMaster
class AvrUsartHDPack
{
	   usart_regs* _usart;

    public:
	   AvrUsartHDPack(usart_regs* usart): _usart(usart) {};

        void _rx_complete_irq(void);
        void _tx_irq(void);
        void _udre_irq(void);

        void begin()
        {
            usart->rx_headler = _rx_complete_irq;
            usart->tx_headler = _tx_irq;
            usart->udre_headler = _udre_irq;
        };
    }; 	
};


#endif