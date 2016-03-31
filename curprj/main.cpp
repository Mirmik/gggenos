#include "hal/arch.h"
#include "genos/debug/debug.h" 
#include "genos/time/sysclock.h"
#include "configure.h"
#include "hal/gpio.h"

#include "hal/arch.h"
#include "genos/debug/delay.h"
#include "genos/stack/static_stack.h"
#include "stm32f4xx_sdio.h"
#include "stm32f4xx_spi.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

void emergency_stop()
{
//	usart2_interrupt_disable();
	debug_print("EMERGENCY_STOP");
};

void setup();
void loop();


int main(){
	setup();
	while(1) loop();
};


void spiW5500_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef SPI_InitStructure;
 
  // Тактирование модуля SPI1 и порта А
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
 
  // Настраиваем ноги SPI1 для работы в режиме альтернативной функции
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_5;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
 
  //Заполняем структуру с параметрами SPI модуля
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //полный дуплекс
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; // передаем по 8 бит
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; // Полярность и
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; // фаза тактового сигнала
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; // Управлять состоянием сигнала NSS программно
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32; // Предделитель SCK
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; // Первым отправляется старший бит
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master; // Режим - мастер
  SPI_InitStructure.SPI_CRCPolynomial = 0x07; // Задаем значение параметра CRCPolynomial

//  SPI_CalculateCRC(SPI1,ENABLE); //Включаем расчёт CRC
  SPI_Init(SPI1, &SPI_InitStructure); //Настраиваем SPI1
  SPI_Cmd(SPI1, ENABLE); // Включаем модуль SPI1....
 
  // Поскольку сигнал NSS контролируется программно, установим его в единицу
  // Если сбросить его в ноль, то наш SPI модуль подумает, что
  // у нас мультимастерная топология и его лишили полномочий мастера.
  SPI_NSSInternalSoftwareConfig(SPI1, SPI_NSSInternalSoft_Set);
}



#include "stdlib.h"
void setup(){
	arch_init();
	project_configure();
	gpio_port_set_pin(GPIOD, 12);

	spiW5500_init();

	//debug_printhex_uint16(12);
	debug_print("SPI INFO:");dln;
	debug_print("SR ");debug_printhex_uint16(SPI1->SR);dln;
	debug_print("CR1 ");debug_printhex_uint16(SPI1->CR1);dln;
	debug_print("CR2 ");debug_printhex_uint16(SPI1->CR2);dln;
	debug_print("CRCPR ");debug_printhex_uint16(SPI1->CRCPR);dln;
	debug_print("TXCRCR ");debug_printhex_uint16(SPI1->TXCRCR);dln;


	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)==RESET);
	SPI_I2S_SendData(SPI1,0x11);
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)==RESET);
	SPI_I2S_SendData(SPI1,0x22);
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)==RESET);
	SPI_I2S_SendData(SPI1,0x33);
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)==RESET);
	SPI_I2S_SendData(SPI1,0x44);
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)==RESET);
	SPI_I2S_SendData(SPI1,0x55);


	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)==RESET);
	debug_printhex_uint32(SPI_I2S_ReceiveData(SPI1));
};


void loop()
{
	gpio_port_tgl_pin(GPIOD, 12);
  debug_print("here");
	delay(500);
	gpio_port_tgl_pin(GPIOD, 12);
	delay(500);
};











