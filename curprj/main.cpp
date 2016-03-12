#include "hal/arch.h"
#include "kernel/diag.h"
#include "genos/debug/debug.h"
#include "asm/Serial.h"
#include "hal/gpio.h"
#include "genos/time/sysclock.h"
#include "genos/sigslot/delegate.h"

#include "genos/kernel/waitserver.h"
#include "genos/schedproc/automSched.h"
#include "genos/terminal/autom_terminal.h"
#include "util/ring.h"

HardwareSerial Serial0;
void emergency_stop()
{
	debug_print("EMERGENCY_STOP\n");
};
class A{
public:
TimWaiter timer;
	void blinker()
	{
		gpio_change(13);
		//debug_printdec_uint32(millis());dln;
		msleep_autom_bias(&timer, b);
		b=b+10;
	};

	void t2()
	{
		wait_autom(&Serial0);
	};
	int b = 100;
A() {timer._timer.init_bias(millis());};
} a;

automScheduler automSched;
automTerminal automTerm;

void setup();
void loop();


int main(){
	setup();
	arch_deatomic();
	while(1) loop();
};

//void task(){debug_print("HelloWorld"); return;};

syscontext syscntxt;	

void setup(){
	arch_init();
	diag_init();

	//usart_drop(&usart0);
	usart_baudrate_set(&usart0, 9600);
	usart_parity_set(&usart0, usartParityDisabled);
	usart_stopbits_set(&usart0, usartOneStopBit);
	usart_databits_set(&usart0, usartEightDataBit);
	
	usart_udr_empty_isr_disable(&usart0);
	usart_rx_isr_enable(&usart0);
	
	usart_rx_enable(&usart0);
	usart_tx_enable(&usart0);

	Serial0.connect(&usart0);

	current_syscontext(&syscntxt);
	stdout.direct(&Serial0);
	stdin.direct(&Serial0);

	debug_delay(20000);
	
	gpio_mode_out(13);
	gpio_hi(13);

	automSched.registry(&a, &A::blinker);
	automSched.registry(&automTerm, &automTerminal::exec);
};

void loop(){
	automSched.schedule();
	waitserver.check();
	//debug_printhex_uint32(Serial0.available());
};

