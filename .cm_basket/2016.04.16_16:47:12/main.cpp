#include "hal/arch.h"
#include "kernel/diag.h"
#include "genos/debug/debug.h"
#include "hal/gpio.h"
#include "genos/time/sysclock.h"

#include "genos/io/Serial.h"
#include "genos/terminal/autom_terminal.h"
#include "syscontext/syscontext.h"

void emergency_stop()
{
	debug_print("EMERGENCY_STOP\n");
};

Serial_t<128, 512> Serial;

//#include "util/vt102/vt102.h"

syscontext syscntxt;

void setup();
void loop();

int main(){
	setup();
	while(1) loop();
};

automTerminal automTerm;

void setup(){
	arch_init();
	diag_init();
	current_syscontext(&syscntxt);
	gpio_mode_out(13);

	Serial.init(&ASerial0);

	stdout.direct(&Serial);
	stdin.direct(&Serial);


	debug_delay(30000);
	
	usart_rx_isr_enable(&usart0);
	arch_deatomic();

	gpio_mode_out(13);	
	gpio_mode_out(30);
	gpio_mode_out(31);
};


void loop(){
	//automTerm.exec();
	gpio_change(13);
	delay(100);
	gpio_change(31);
	delay(100);
	gpio_change(30);
	delay(100);
};




