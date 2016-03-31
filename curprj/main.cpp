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

#include "stdlib.h"
void setup(){
	arch_init();
	//project_configure();

	//debug_printhex_uint16(12);
	debug_print("SPI INFO:");dln;
};


void loop()
{
};











