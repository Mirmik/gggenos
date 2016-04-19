#include "configure.h"

#include "stm32f4xx_gpio.h" 
#include "stm32f4xx_usart.h" 
#include "stm32f4xx_rcc.h" 
#include "stm32f4xx_tim.h" 
#include "misc.h" 
#include "hal/gpio.h"

//Отладочная консоль.
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







//Связь с контроллером по rs232.
void usart6_configure(uint32_t baud)
{
GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;

RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE); //for USART2, USART3, UART4 or UART5
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

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
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx |USART_Mode_Tx;
  USART_Init(USART6, &USART_InitStructure);

  USART_Cmd(USART6, ENABLE); // enable USART2
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









//Таймер для генерации одиночного импульса. OneShoot.
void tim10_configure()
{
GPIO_InitTypeDef GPIO_InitStructure;

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


void one_pulse_generator_start()
{
  TIM_Cmd(TIM10, ENABLE);        
};


//Генератор импульсов.
void tim4_configure()
{
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_ICInitTypeDef  TIM_ICInitStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

  TIM_TimeBaseStructure.TIM_Prescaler = 10-1;
  TIM_TimeBaseStructure.TIM_Period = 16000 -1;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

  TIM4->CNT = 0;
  TIM_Cmd(TIM4, ENABLE);
  NVIC_EnableIRQ(TIM4_IRQn);
};




//Декодер импульсов 
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
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOA,&GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5);

  TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
  TIM5->CNT = 0;

  TIM_OCStructInit(&TIM_OCInitStructure);
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Active;
  TIM_OCInitStructure.TIM_Pulse = 5000;
  TIM_OC3Init(TIM5, &TIM_OCInitStructure);

  //TIM_ITConfig(TIM5, TIM_IT_CC3, ENABLE);
  //TIM

  TIM_Cmd(TIM5, ENABLE);

 NVIC_EnableIRQ(TIM5_IRQn);
};

void tim5_compare_enable()
{
  TIM_ITConfig(TIM5, TIM_IT_CC3, ENABLE);
};


void tim5_compare_disable()
{
  TIM_ITConfig(TIM5, TIM_IT_CC3, DISABLE);
};









void tim2_configure()
{
GPIO_InitTypeDef GPIO_InitStructure;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_ICInitTypeDef  TIM_ICInitStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;


  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOA,&GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOB,&GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_TIM2);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_TIM2);

  TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
  TIM2->CNT = 0;

  //TIM_OCStructInit(&TIM_OCInitStructure);
  //TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Active;
  //TIM_OCInitStructure.TIM_Pulse = 5000;
  //TIM_OC3Init(TIM2, &TIM_OCInitStructure);

  //TIM_ITConfig(TIM5, TIM_IT_CC3, ENABLE);
  //TIM

  TIM_Cmd(TIM2, ENABLE);

 NVIC_EnableIRQ(TIM2_IRQn);
};

void tim2_compare_enable()
{
  //TIM_ITConfig(TIM2, TIM_IT_CC3, ENABLE);
};


void tim2_compare_disable()
{
  //TIM_ITConfig(TIM2, TIM_IT_CC3, ENABLE);
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
  usart2_configure(9600);
	usart6_configure(9600);
  
  tim10_configure(); //ONE_PULSE_GENERATOR
  tim5_configure(); //ENCODER X
//	tim2_configure(); //ENCODER2 X
  
  tim4_configure(); //GENERATOR X
  motor_pins_configure();
  leds_configure();
};