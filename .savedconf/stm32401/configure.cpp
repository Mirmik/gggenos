#include "configure.h"

#include "stm32f4xx_gpio.h" 
#include "stm32f4xx_usart.h" 
#include "stm32f4xx_rcc.h" 
#include "stm32f4xx_tim.h" 
#include "misc.h" 
#include "hal/gpio.h"


void usart2_configure(uint32_t baud)
{
GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;

RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //for USART2, USART3, UART4 or UART5
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 


/* Connect USART2 pins to AF */ 
	// TX = PA2, RX = PA3
 	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
 	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx |USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);

	USART_Cmd(USART2, ENABLE); // enable USART2
}

void usart2_rx_interrupt_enable()
{
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
};

void usart2_interrupt_enable()
{
NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
};

void usart2_interrupt_disable()
{
NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
  NVIC_Init(&NVIC_InitStructure);
};








void usart6_configure(uint32_t baud)
{
GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;

RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE); //for USART2, USART3, UART4 or UART5
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);


  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOC, &GPIO_InitStructure); 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOC, &GPIO_InitStructure); 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure); 


/* Connect USART2 pins to AF */ 
  // TX = PA2, RX = PA3
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);
  USART_InitStructure.USART_BaudRate = baud;
  USART_InitStructure.USART_WordLength = USART_WordLength_9b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_Even;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx |USART_Mode_Tx;
  USART_Init(USART6, &USART_InitStructure);

  USART_Cmd(USART6, ENABLE); // enable USART1
}

void usart6_rx_interrupt_enable()
{
  USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);
};


void usart6_interrupt_enable()
{
NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
};


void usart6_interrupt_disable()
{
NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
  NVIC_Init(&NVIC_InitStructure);
};

void tim10_configure()
{
GPIO_InitTypeDef GPIO_InitStructure;
//USART_InitTypeDef USART_InitStructure;

//RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //for USART2, USART3, UART4 or UART5
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOB, &GPIO_InitStructure); 

  GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_TIM10);
 


TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_ICInitTypeDef  TIM_ICInitStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);

  TIM_TimeBaseStructure.TIM_Prescaler = 84-1;
  TIM_TimeBaseStructure.TIM_Period = 1000 - 1;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;
  TIM_TimeBaseInit(TIM10, &TIM_TimeBaseStructure);

  TIM10->CNT = 0;

  TIM_OCStructInit(&TIM_OCInitStructure);
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 1;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
 // TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
  TIM_OC1Init(TIM10, &TIM_OCInitStructure);

  TIM10->CR1 |= (1<<3);

  //TIM_ITConfig(TIM10, TIM_IT_Update, ENABLE);

//  TIM_Cmd(TIM10, ENABLE);
  //NVIC_EnableIRQ(TIM4_IRQn);
};









void tim4_configure()
{
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_ICInitTypeDef  TIM_ICInitStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

  TIM_TimeBaseStructure.TIM_Prescaler = 10-1;
  //TIM_TimeBaseStructure.TIM_Period = 50000;
  TIM_TimeBaseStructure.TIM_Period = 16000 -1;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

  //TIM_OCStructInit(&TIM_OCInitStructure);
  //TIM_OCInitStructure.TIM_Pulse = 50000;
  //TIM_OC1Init(TIM5, &TIM_OCInitStructure);

  //SET ARPE/ Чтоб обновлялся после того, как досчитает.
  //TIM4->CR1 |= (1 << 7); 

  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

  //TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
//TIM_TIxExternalClockConfig(TIM5, TIM_TIxExternalCLK1Source_TI1, TIM_ICPolarity_Rising, 0);
  TIM4->CNT = 0;
  TIM_Cmd(TIM4, ENABLE);
  NVIC_EnableIRQ(TIM4_IRQn);
};





#include "genos/debug/debug.h"
void tim5_configure()
{
GPIO_InitTypeDef GPIO_InitStructure;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_ICInitTypeDef  TIM_ICInitStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;


  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOA,&GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5);

  //TIM_TimeBaseStructure.TIM_Prescaler = 100000-1;
  //TIM_TimeBaseStructure.TIM_Period = 50000;
  TIM_TimeBaseStructure.TIM_Period = 0xFFFFFFFF;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

  //TIM_OCStructInit(&TIM_OCInitStructure);
  //TIM_OCInitStructure.TIM_Pulse = 50000;
  //TIM_OC1Init(TIM5, &TIM_OCInitStructure);

  //TIM_ITConfig(TIM5, TIM_IT_CC1, ENABLE);


TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
//TIM_TIxExternalClockConfig(TIM5, TIM_TIxExternalCLK1Source_TI1, TIM_ICPolarity_Rising, 0);
  TIM5->CNT = 0;

  TIM_OCStructInit(&TIM_OCInitStructure);
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Active;
  TIM_OCInitStructure.TIM_Pulse = 5000;
  TIM_OC3Init(TIM5, &TIM_OCInitStructure);

  TIM_ITConfig(TIM5, TIM_IT_CC3, ENABLE);
  //TIM

  TIM_Cmd(TIM5, ENABLE);

 NVIC_EnableIRQ(TIM5_IRQn);


};

int ittick;
extern "C" void TIM5_IRQHandler();
void TIM5_IRQHandler()
{
  if (TIM_GetITStatus(TIM5, TIM_IT_CC3) != RESET)
    {
      TIM5->CCR3 += 500;
      TIM_Cmd(TIM10, ENABLE);

      //ittick++; debug_printdec_int32(ittick);
      TIM_ClearITPendingBit(TIM5, TIM_IT_CC3);
      return;
      //TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
    };
    
    debug_panic("TIMER5INTERRUPT");
  };


void leds_configure()
{

GPIO_InitTypeDef  GPIO_InitStructure;


  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

};

void motor_pins_configure()
{
GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2| GPIO_Pin_4| GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  gpio_port_set_mask(GPIOD, GPIO_Pin_2 | GPIO_Pin_6);
  gpio_port_clr_mask(GPIOD, GPIO_Pin_0 | GPIO_Pin_4);

};


void project_configure()
{
  FLASH_Unlock();
  usart6_configure(38400);
  usart2_configure(9600);
	
  tim10_configure(); //ONE_PULSE_GENERATOR
  tim5_configure(); //ENCODER X
	tim4_configure(); //GENERATOR X
  motor_pins_configure();
  leds_configure();
};

#include "genos/debug/debug.h"
#include "asm/Serial.h"
#include "asm/SerialHD.h"
extern HardwareSerial Serial2;
extern HardwareSerialHD SerialHD6;



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

uint8_t pulse_state;
extern uint8_t pulse_direction;

uint32_t impulse_count = 0;

extern "C" void TIM4_IRQHandler();
void TIM4_IRQHandler()
{
  
  if(TIM4->SR & 0x1) //Прерывание по переполнению
    {
      if (impulse_count <= 0) {goto _exit;};

      //do_once {debug_print("TimInterrupt");};      
      pulse_state = (pulse_state + pulse_direction) & 0x3;
      switch(pulse_state)
      {
        case 0: state0(); break;
        case 1: state1(); break;
        case 2: state2(); break;
        case 3: state3(); break;
      };
      impulse_count--;
      
      _exit:
      TIM4->SR &= ~0x1;    
      return; 
    };

  dln;
  debug_printhex_uint32(TIM4->CR1);dln;
  debug_printhex_uint32(TIM4->SR);dln;
  debug_panic("TIM4INTERRUPT");
};




extern "C" void USART2_IRQHandler();
void USART2_IRQHandler()
{
  //Действия по опустошению регистра передачи.
  if (USART_GetITStatus(USART2, USART_IT_TXE) == SET)
    {
        Serial2.irq_txe();
        return;
    };

if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
    {
        Serial2.irq_rxne();
        return;
    };


  debug_panic("USART_DEBUG_PANIC2");
};

extern "C" void USART6_IRQHandler();
void USART6_IRQHandler()
{
  //Действия по опустошению регистра передачи.
  if (USART_GetITStatus(USART6, USART_IT_TXE) == SET)
    {
        SerialHD6.irq_txe();
        return;
    };

  if (USART_GetITStatus(USART6, USART_IT_TC) == SET)
    {
        gpio_port_set_pin(GPIOD, 15);
        SerialHD6.irq_tc();
        return;
    };

if (USART_GetITStatus(USART6, USART_IT_RXNE) == SET)
    {

        gpio_port_set_pin(GPIOD, 14);
        SerialHD6.irq_rxne();
        return;
    };

  debug_printhex_uint16(USART6->SR)   ; 
  debug_panic("USART_DEBUG_PANIC6");
};