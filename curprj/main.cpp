#include "hal/arch.h"
#include "kernel/diag.h"
#include "genos/debug/debug.h"
#include "asm/Serial.h"
#include "asm/Serial_half_duplex.h"
#include "hal/gpio.h"
#include "cpu/timers.h"
#include "genos/time/sysclock.h"
#include "genos/sigslot/delegate.h"

#include "genos/kernel/waitserver.h"
#include "genos/schedproc/automSched.h"
#include "genos/terminal/autom_terminal.h"
#include "util/ring.h"
#include "avr/io.h"
#include "cpu/memory_info.h"

#include "configuration.h"

HardwareSerial Serial0;
Serial_HD_simple SerialHD1;

class RS485_Controller
{
public:
int state = 0;
uint8_t rs485flag = 0;
	void exec()
	{
		switch (state)
		{
			case 0:
				//msleep_autom(1000);
				state = 1;
				break;
			case 1:
				SerialHD1.configure_session("HelloWorld", 5, '0', &rs485flag);
				SerialHD1.start_session();
				//stdout.print("fasdfasf");
				//wait_autom(&rs485flag);
				state = 2;
				break;
			case 2:
				//stdout.print("allgood");
				//state = 2;
				break;
			case 3:

				break;
		};
	};
} rs485;

void emergency_stop()
{
	debug_print("EMERGENCY_STOP\n");
};


void blinker()
	{
		gpio_change(13);
		msleep_autom(1000);
		//stdout.println(millis());
	};

automScheduler automSched;
automTerminal automTerm;
syscontext syscntxt;	

void setup();
void loop();

int main(){

	setup();
	arch_deatomic();
	while(1) loop();
};

void usart_info()
{
	stdout.printhexln(UCSR0A);
	stdout.printhexln(UCSR0B);
	stdout.printhexln(UCSR0C);
	stdout.printhexln(UCSR1A);
	stdout.printhexln(UCSR1B);
	stdout.printhexln(UCSR1C);
};

uint8_t flag_usart1;

void start()
{
};

void setup(){
	arch_init();
	diag_init();

	debug_delay(20000);
	load_configuration();

	//usart_drop(&usart0);
	usart_baudrate_set(&usart0, conf.usart0_speed);
	usart_parity_set(&usart0, conf.usart0_parity);
	usart_stopbits_set(&usart0, conf.usart0_stopbits);
	usart_databits_set(&usart0, conf.usart0_databits);

	usart_baudrate_set(&usart0, conf.usart1_speed);
	usart_parity_set(&usart0, conf.usart1_parity);
	usart_stopbits_set(&usart0, conf.usart1_stopbits);
	usart_databits_set(&usart0, conf.usart1_databits);
	
	usart_udr_empty_isr_disable(&usart0);
	usart_rx_isr_enable(&usart0);
	
	usart_rx_enable(&usart0);
	usart_tx_enable(&usart0);

	Serial0.connect(&usart0);
	SerialHD1.connect(&usart1, 6);

	//usart_udr_empty_isr_enable(&usart1);

	current_syscontext(&syscntxt);
	stdout.direct(&Serial0);
	stdin.direct(&Serial0);

	central_cmdlist.add("timer", timer_info);
	central_cmdlist.add("start", start);
	central_cmdlist.add("print_conf", print_conf);
	central_cmdlist.add("print_writed_conf", print_writed_conf);
	central_cmdlist.add("write_conf", write_conf);
	central_cmdlist.add("memory_info", memory_info);
	central_cmdlist.add("usart_info", usart_info);

	
	gpio_mode_out(13);
	gpio_hi(13);

	automSched.registry(blinker);
	automSched.registry(&automTerm, &automTerminal::exec);
	automSched.registry(&rs485, &RS485_Controller::exec);
	//debug_delay(20000);
};

void loop(){
	automSched.schedule();
	waitserver.check();
	//debug_printhex_uint32(Serial0.available());
};

