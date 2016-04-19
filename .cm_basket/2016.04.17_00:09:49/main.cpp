#include "genos.h"
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

string str = "HelloWorld";

void serinfo()
{
	dpr(Serial.rx_ring.to_str());
	dpr(Serial.tx_ring.to_str());
	dpr("rx_");dpr(Serial.rx_ring.to_info());
	dpr("tx_");dpr(Serial.tx_ring.to_info());
};

void red_on(){gpio_hi(13);};
void green_on(){gpio_hi(30);};
void yellow_on(){gpio_hi(31);};
void red_off(){gpio_lo(13);};
void green_off(){gpio_lo(30);};
void yellow_off(){gpio_lo(31);};

#include "genos/decoration.h"
void set_name(int n, char** c)
{
	if (n==1) machine_name = "kekeke";
	else machine_name = c[1];
};

void set_name2(int n, char** c)
{
	if (n==1) machine_name = "kekeke";
	else machine_name = c[1];
};

void setup(){
	arch_init();
	diag_init();
	current_syscontext(&syscntxt);
	gpio_mode_out(13);
	gpio_mode_out(30);
	gpio_mode_out(31);

	Serial.init(&ASerial0);

	stdout.direct(&Serial);
	stdin.direct(&Serial);


	debug_delay(30000);
	
	usart_rx_isr_enable(&usart0);
//	usart_udr_empty_isr_enable(&usart0);

	arch_deatomic();

	//diag_putc('C');
	

	central_cmdlist.add("serinfo", serinfo);
	central_cmdlist.add("red_on", red_on);
	central_cmdlist.add("green_on", green_on);
	central_cmdlist.add("yellow_on", yellow_on);
	central_cmdlist.add("red_off", red_off);
	central_cmdlist.add("green_off", green_off);
	central_cmdlist.add("yellow_off", yellow_off);
	central_cmdlist.add("setname", set_name);
	central_cmdlist.add("setname2", set_name2);
	
	//vt102.backspace();

//	vt102.cursor_left(4);
//	vt102.delchar(2);
};


void loop(){
	automTerm.exec();
};




