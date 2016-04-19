#include "hal/arch.h"
#include "kernel/diag.h"
#include "util/stub.h"

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_rcc.h"

static int usart2_diag_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

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
	
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx |USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);

	USART_Cmd(USART2, ENABLE); // enable USART2
		};
		

		int usart2_diag_putchar(int c)
		{
			while((USART2->SR & USART_FLAG_TC)==0);
  			USART2->DR = c;
			while((USART2->SR & USART_FLAG_TC)==0);
		};

		struct diag_ops usart2_diag = {
			usart2_diag_putchar, 
			(diag_getc_t) do_nothing, 
			diag_write_stub, 
			(diag_read_t) do_nothing, 
			usart2_diag_init
		}; 

int usart6_diag_putchar(int c)
		{
			while((USART6->SR & USART_FLAG_TC)==0);
  			USART6->DR = c;
			while((USART6->SR & USART_FLAG_TC)==0);
		};

		struct diag_ops usart6_diag = {
			usart6_diag_putchar, 
			(diag_getc_t) do_nothing, 
			diag_write_stub, 
			(diag_read_t) do_nothing, 
			(diag_init_t) do_nothing
		}; 

				
		