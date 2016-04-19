#include "genos/debug/debug.h"
#include "asm/Serial.h"
#include "hal/gpio.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "misc.h"
#include "axis.h"
extern Axis axis_x;

extern "C" void TIM5_IRQHandler();
void TIM5_IRQHandler()
{
  if (TIM_GetITStatus(TIM5, TIM_IT_CC3) != RESET)
    {
      TIM_Cmd(TIM10, ENABLE);
      axis_x.trigger_callback();
      TIM_ClearITPendingBit(TIM5, TIM_IT_CC3);
      return;
    };
    
    debug_panic("TIMER5INTERRUPT");
  };


void state0()
{
  gpio_port_set_mask(GPIOD, GPIO_Pin_2 | GPIO_Pin_6);
  gpio_port_clr_mask(GPIOD, GPIO_Pin_0 | GPIO_Pin_4);
};

void state1()
{
  gpio_port_set_mask(GPIOD, GPIO_Pin_0 | GPIO_Pin_6);
  gpio_port_clr_mask(GPIOD, GPIO_Pin_2 | GPIO_Pin_4);
};

void state2()
{
  
  gpio_port_set_mask(GPIOD, GPIO_Pin_0 | GPIO_Pin_4);
  gpio_port_clr_mask(GPIOD, GPIO_Pin_2 | GPIO_Pin_6);
};

void state3()
{
  gpio_port_set_mask(GPIOD, GPIO_Pin_2 | GPIO_Pin_4);
  gpio_port_clr_mask(GPIOD, GPIO_Pin_0 | GPIO_Pin_6);
};


extern "C" void TIM4_IRQHandler();
void TIM4_IRQHandler()
{
  if(TIM4->SR & 0x1) //Прерывание по переполнению
    {
      if (axis_x.generator <= 0) {goto _exit;};

      axis_x.pstate = (axis_x.direction == Axis::FORWARD ? ++axis_x.pstate : --axis_x.pstate) & 0x3;
      switch(axis_x.pstate)
      {
        case 0: state0(); break;
        case 1: state1(); break;
        case 2: state2(); break;
        case 3: state3(); break;
      };
      axis_x.generator--;
      
      _exit:
      TIM4->SR &= ~0x1;    
      return; 
    };

  dln;
  debug_printhex_uint32(TIM4->CR1);dln;
  debug_printhex_uint32(TIM4->SR);dln;
  debug_panic("TIM4INTERRUPT");
};




#include "asm/Serial.h"
extern AdapterSerial ASerial2;
extern "C" void USART2_IRQHandler();
void USART2_IRQHandler()
{
  //Действия по опустошению регистра передачи.
  if (USART_GetITStatus(USART2, USART_IT_TXE) == SET)
    {
    	//dpr("interr");
        ASerial2.sended();
        return;
    };

if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
    {
        ASerial2.recv(USART2->DR);
        return;
    };


  debug_panic("USART_DEBUG_PANIC2");
};
