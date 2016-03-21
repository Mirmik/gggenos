#include "hal/arch.h"
#include "stm32f4xx_gpio.h" 
#include "stm32f4xx_usart.h" 
#include "stm32f4xx_rcc.h" 
#include "stm32f4xx_tim.h" 
#include "genos/debug/debug.h" 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void emergency_stop()
{
};

void setup();
void loop();

int main(){
	setup();
	while(1) loop();
};

#define SYSTEM_CLOCK 84000000

static inline void Delay(uint32_t n_clock3)
{
        asm volatile (
        "\n"
       "\tmov r3, %0\n"
        "Lab%=:" "\n\t"
        "subs r3, r3, 1" "\n\t"
        "bne.n Lab%=" "\n\t"
	:  
	: "r" (n_clock3) 
	: "r3"
        );
        return;
}


#define delay_us(s) Delay((s)*(float)SYSTEM_CLOCK/3/1000000)

#define delay_ms(s) Delay((s)*(float)SYSTEM_CLOCK/3/1000)

#define delay_xs(s) Delay((s)*(float)SYSTEM_CLOCK/3/2000000)


void usart2_init(uint32_t baud)
{
GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;

//RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //for USART1 and USART6
RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //for USART2, USART3, UART4 or UART5

/* GPIOA clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

/* GPIOA Configuration:  USART2 TX on PA2, RX on PA3 */
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


void print(USART_TypeDef* USARTx, char *t)
{
while(*t)
	{
	//assert_param(IS_USART_ALL_PERIPH(USARTx));
	//  assert_param(IS_USART_DATA(Data));
	while((USARTx->SR & USART_FLAG_TC)==0);
  	USARTx->DR = *t++;
	}
}

uint64_t tick = 0;

extern "C" void SysTick_Handler(); 
void SysTick_Handler()
{
  tick++;
};




void setup(){

char c[64];

	GPIO_InitTypeDef  GPIO_InitStructure;

SysTick_Config(84001);

RCC_ClocksTypeDef RCC_ClocksStatus;
  /* GPIOD Periph clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  
  usart2_init(115200 * 1);
  //RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //for USART2, USART3, UART4 or UART5

  /* Configure PD12, PD13, PD14 and PD15 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  RCC_MCO1Config(RCC_MCO1Source_PLLCLK, RCC_MCO1Div_1);

  	RCC_GetClocksFreq(&RCC_ClocksStatus);
  print(USART2, "uart testik\n\r"); 
//sprintf(c,"HSE_VALUE %u HSI_VALUE %u\n",HSE_VALUE,HSI_VALUE); print(USART2,c);

sprintf(c,"RCC->CFGR & RCC_CFGR_SWS %d\n\r",RCC->CFGR & RCC_CFGR_SWS); print(USART2,c);
uint32_t pllsource = (RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) >> 22;
uint32_t pllm = RCC->PLLCFGR & RCC_PLLCFGR_PLLM;
uint32_t plln = (RCC->PLLCFGR & RCC_PLLCFGR_PLLN)>>6;
uint32_t pllp = (((RCC->PLLCFGR & RCC_PLLCFGR_PLLP) >>16) + 1 ) *2;
uint32_t pllvco = (HSE_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6);
sprintf(c,"pllsource %u\n\r",pllsource); print(USART2,c);
sprintf(c,"pllm %u plln %u pllp %u\n\r",pllm,plln,pllp); print(USART2,c);
sprintf(c,"pllvco %u\n\r",pllvco); print(USART2,c);
sprintf(c,"FINAL = pllvco/pllp %u\n\r",pllvco/pllp); print(USART2,c);

sprintf(c,"SYSCLK_Frequency %u\n\r",RCC_ClocksStatus.SYSCLK_Frequency); print(USART2,c);
sprintf(c,"HCLK_Frequency %u\n\r",RCC_ClocksStatus.HCLK_Frequency); print(USART2,c);
sprintf(c,"PCLK1_Frequency %u\n\r",RCC_ClocksStatus.PCLK1_Frequency); print(USART2,c);
sprintf(c,"PCLK2_Frequency %u\n\r",RCC_ClocksStatus.PCLK2_Frequency); print(USART2,c);
  





TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_ICInitTypeDef  TIM_ICInitStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
//GPIO_InitTypeDef GPIO_InitStructure;

//#define   ENCODER_TIM             TIM5
//#define   MAX_COUNT               100
GPIO_SetBits(GPIOD, GPIO_Pin_12);

  int i=0;

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

  TIM_OCStructInit(&TIM_OCInitStructure);
  TIM_OCInitStructure.TIM_Pulse = 50000;
  TIM_OC1Init(TIM5, &TIM_OCInitStructure);

  TIM_ITConfig(TIM5, TIM_IT_CC1, ENABLE);

  TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
//TIM_TIxExternalClockConfig(TIM5, TIM_TIxExternalCLK1Source_TI1, TIM_ICPolarity_Rising, 0);
  TIM5->CNT = 0;
  TIM_Cmd(TIM5, ENABLE);

  NVIC_EnableIRQ(TIM5_IRQn);

};

uint64_t ittick=0;

extern "C" void TIM5_IRQHandler();
void TIM5_IRQHandler()
{
  //if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
    //{
     // ittick++;
     // TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
    //};

    if (TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)
    {
      print(USART2,(char*) "Z");
    };
  };

void loop(){
char c[64];

  sprintf(c, "%u\n\r", TIM5->CNT);
  print (USART2,c);    
  //sprintf(c, "%u\n\r", ittick);
    //print (USART2,c);    
  Delay(1000);
};

