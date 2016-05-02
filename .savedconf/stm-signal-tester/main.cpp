#include "stdlib.h"
#include "hal/arch.h"
#include "hal/gpio.h"
#include "genos/debug/debug.h" 
#include "genos/time/sysclock.h"
#include "configure.h"

extern"C" void emergency_stop();
void emergency_stop()
{};

void setup(){
	arch_init();
	project_configure();
	gpio_port_clr_pin(GPIOD, 6);
};

void loop()
{
	gpio_port_set_pin(GPIOD, 13);
	delay(500);
	gpio_port_clr_pin(GPIOD, 13);

	gpio_port_set_pin(GPIOD, 6);
	delay(1);
	gpio_port_clr_pin(GPIOD, 6);
		
	delay(500);	
};



int main()
{
	setup();
	while(1) loop();
};