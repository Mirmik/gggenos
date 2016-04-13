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
//AdapterSerial ASerial0;

#include "util/vt102/vt102.h"
//VT102_output vt102;

syscontext syscntxt;

void setup();
void loop();

int main(){
	setup();
	while(1) loop();
};

automTerminal automTerm;

#include "util/macro.h"
#include "utilxx/WString.h"

String str = "HelloWorld";

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
//	usart_udr_empty_isr_enable(&usart0);

	arch_deatomic();
	//diag_putc('C');

	debug_print(str.c_str());
	debug_print(str.c_str());
	debug_print(str.c_str());
	debug_print(str.c_str());
	debug_print(str.c_str());
	debug_print(str.c_str());
	
	//vt102.backspace();

//	vt102.cursor_left(4);
//	vt102.delchar(2);
};


void loop(){
	automTerm.exec();
};




