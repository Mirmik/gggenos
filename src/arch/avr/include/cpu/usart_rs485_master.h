#ifndef AVR_USART_STREAM
#define AVR_USART_STREAM

#include "usart_control_struct.h"
#include "genos/io/stream.h"
#include "genos/time/timeserv.h"
#include "genos/datastruct/list.h"


//HalfDuplexPacketMaster
class AvrUsartHDPack
{

    struct session
    {
        char* message;
        char* answer;
        int answer_stop_cond;
        int answer_stop_data;
        uint8_t* flag;
        list_head lst;
    };

    list_head task_list;
    
    timer tim;    
    usart_regs* _usart;
    session* active_session;
    char* answer_ptr;
    char* message_ptr;

public:

    void query(
        char* message, 
        char* answer, 
        int answer_stop_cond, 
        int answer_stop_data, 
        uint8_t* outflag
    ){
        session* s = malloc(sizeof(session));
        s->message = message;
        s->answer = answer;
        s->flag = outflag;
        list_add_tail(&s->lst, &task_list);
    };

    AvrUsartHDPack(usart_regs* usart): _usart(usart) {};

    void _rx_complete_irq(char c);
    void _tx_irq(void);
    void _udre_irq(void);

    void begin()
    {
        usart->rx_headler = _rx_complete_irq;
        usart->tx_headler = _tx_irq;
        usart->udre_headler = _udre_irq;
    }; 	
};


#endif