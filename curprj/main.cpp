#include "hal/arch.h"
#include "genos/debug/debug.h" 
#include "genos/time/sysclock.h"
#include "genos/debug/delay.h"
#include "hal/gpio.h"
#include "genos/kernel/waitserver.h"
#include "asm/Serial.h"
#include "genos/schedproc/automSched.h"

#include "configure.h"
#include "stm32f4xx_rcc.h"
#include "asm/SerialHD.h"

#include "genos/schedproc/automSched.h"
#include "genos/terminal/autom_terminal.h"
#include "genos/terminal/command_list.h"


automScheduler automSched;
automTerminal automTerm;

void emergency_stop()
{
};

void setup();
void loop();

int main(){
	setup();
	while(1) loop();
};


class A
{
int state = 0;
TimWaiter t;

public:
	void exec()
	{
		switch(state)
		{
			case 0:
				gpio_port_tgl_pin(GPIOD,13);
				state = 1;
//				debug_printdec_uint32(millis());dln;
				msleep_autom_bias(&t, 100);
				break;
			case 1:
				gpio_port_tgl_pin(GPIOD,12);
				msleep_autom_bias(&t, 100);
				state = 0;
				break;
		};
	};
};

A a;

HardwareSerial Serial2;
HardwareSerialHD SerialHD6;
syscontext syscntxt;

void* operator new(size_t, void* ptr) {return ptr;};
void global_constructor_stub()
{
	new(&a) A;
	new(&automSched) automScheduler;
	new(&automTerm) automTerminal;
	new(&waitserver) WaitServer;
	new(&Serial2) HardwareSerial;
	//new(&SerialHD6) HardwareSerialHD;
	new(&syscntxt) syscontext;
	new(&central_cmdlist) command_list;
};

void setup(){
	global_constructor_stub();
	arch_init();
	project_configure();

	current_syscontext(&syscntxt);
	stdout.direct(&Serial2);
	stdin.direct(&Serial2);

	Serial2.usart = USART2;

	//debug_print("JJJORKEY!!!");

	SerialHD6.usart = USART6;
	SerialHD6.changedir_port = GPIOC;
	SerialHD6.changedir_pin = GPIO_Pin_8;

	//debug_print("setup");
	usart2_interrupt_enable();
	usart6_interrupt_enable();
	usart2_rx_interrupt_enable();

	//while(1);

	//Serial2.print("allgood");
	automSched.registry(&a, &A::exec);
	//automSched.registry(&automTerm, &automTerminal::exec);

	//RCC_ClocksTypeDef RCC_in;
	//RCC_GetClocksFreq(&RCC_in);

	//Serial2.print((uint64_t)RCC_in.PCLK1_Frequency);
	//Serial2.print((uint64_t)RCC_in.PCLK2_Frequency);

	SerialHD6.configure_session("HelloWorld", 6, (char)3);
	SerialHD6.start_session();

	  //USART6->DR = 'g';    
};

void loop()
{
	automSched.schedule();
	waitserver.check();
};

