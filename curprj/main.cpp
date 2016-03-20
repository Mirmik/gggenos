#include "hal/arch.h"
#include "stm32f4xx_gpio.h" 
#include "stm32f4xx_rcc.h" 
#include "genos/debug/debug.h" 

void emergency_stop()
{
};

void setup();
void loop();

int main(){
	setup();
	while(1) loop();
};

void setup(){

	GPIO_InitTypeDef  GPIO_InitStructure;

  /* GPIOD Periph clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

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

  while (1)
  {
    /* PD12 to be toggled */
    GPIO_SetBits(GPIOD, GPIO_Pin_12);
    debug_delay(2000000);
   
    /* PD13 to be toggled */
    GPIO_SetBits(GPIOD, GPIO_Pin_13);
   debug_delay(2000000); 
    
    /* PD14 to be toggled */
    GPIO_SetBits(GPIOD, GPIO_Pin_14);
    debug_delay(2000000);
    

    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)) GPIO_SetBits(GPIOD, GPIO_Pin_15);

    /* PD15 to be toggled */
    //GPIO_SetBits(GPIOD, GPIO_Pin_15);
    //debug_delay(2000000);
    
    GPIO_ResetBits(GPIOD, GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14);
    debug_delay(2000000);
     }


};

void loop(){
};

