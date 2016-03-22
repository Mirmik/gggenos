#include "configure.h"

#include "stm32f4xx_gpio.h" 
#include "stm32f4xx_usart.h" 
#include "stm32f4xx_rcc.h" 
#include "stm32f4xx_tim.h" 
#include "misc.h" 


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
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
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

  TIM_TimeBaseStructure.TIM_Prescaler = 0;
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
  TIM_Cmd(TIM5, ENABLE);

  NVIC_EnableIRQ(TIM5_IRQn);


};


extern "C" void TIM5_IRQHandler();
void TIM5_IRQHandler()
{
  //if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
    //{
     // ittick++;
     // TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
    //};

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


void project_configure()
{
  usart6_configure(115200);
  usart2_configure(115200);
	tim5_configure();
	leds_configure();
};

#include "hal/gpio.h"
#include "genos/debug/debug.h"
#include "asm/Serial.h"
#include "asm/SerialHD.h"
extern HardwareSerial Serial2;
extern HardwareSerialHD SerialHD6;

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


  debug_panic("USART_DEBUG_PANIC");
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

        SerialHD6.irq_tc();
        gpio_port_set_pin(GPIOD, 15);
        return;
    };

if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
    {
        gpio_port_set_pin(GPIOD, 14);
        //SerialHD6.irq_rxne();
        return;
    };


  //debug_panic("USART_DEBUG_PANIC");
};