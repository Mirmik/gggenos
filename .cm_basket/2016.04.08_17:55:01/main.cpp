#include "hal/arch.h"
#include "genos/debug/debug.h" 
#include "genos/time/sysclock.h"
#include "genos/debug/delay.h"
#include "hal/gpio.h"

#include "stm32f4x7_eth.h"
#include "stm32f4x7_eth_bsp.h"

#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_rcc.h"

#include "configure.h"

#include "genos/kernel/waitserver.h"
//#include "asm/Serial.h"
#include "genos/terminal/command_list.h"
#include "syscontext/syscontext.h"

#include "genos/stack/static_stack.h"
#include "kernel/context.h"

#include "genos/schedproc/switchScheduler.h"
#include "genos/terminal/autom_terminal.h"

#include "genos/decoration.h"

#include "util/meslst.h"

#include "genos/io/Serial.h"

volatile uint32_t LocalTime = 0;
context schedule_context;


void global_schedule();

void emergency_stop()
{
	usart2_interrupt_disable();
//	usart6_interrupt_disable();
	
	debug_print("EMERGENCY_STOP");
};


context cntxt1, cntxt2, cntxt3; 
static_stack<500> stck2, stck3;

//HardwareSerial Serial2;


Serial_t Serial2;
AdapterSerial ASerial2;


syscontext syscntxt;
switchScheduler switchSched;
automTerminal automTerm;

void* operator new(size_t, void* ptr) {return ptr;};
void global_constructor_stub()
{
	new(&waitserver) WaitServer;
	new(&Serial2) Serial_t;
	new(&ASerial2) AdapterSerial;
	new(&syscntxt) syscontext;
	new(&central_cmdlist) command_list;
	new(&switchSched) switchScheduler;
	new(&automTerm) automTerminal;
};


void task1()
{
while(1)
	{
	gpio_port_tgl_pin(GPIOD, 13);
	msleep_subst(200);
	//debug_print("all good\r\n");
	};
};

void server()
{
volatile uint32_t m;
  	
//     while(1)
 // {
  	m = millis();
};


void task2()
{
while(1)
	{
	gpio_port_tgl_pin(GPIOD, 15);
	msleep_subst(1000);
	USART6->DR = 'H';
	};
};

void setup(){
	global_constructor_stub();
	arch_init();
	project_configure();

	current_context_set(&schedule_context);

	current_syscontext(&syscntxt);
	(current_syscontext()->__stdout).direct(&Serial2);
	(current_syscontext()->__stdin).direct(&Serial2);

	gpio_port_set_pin(GPIOD, 12);

	usart2_interrupt_enable();

	dprln("DebugPrint");
	
	ASerial2.usart = USART2;
	Serial2.init(&ASerial2);

	meslst ml;

	//delay(10);
	//stdout.print("Allgood");
	//delay(100);

	//stdout.print("Deletegood");
	
//	while(1);

	switchSched.registry(task2);
	switchSched.registry(task1);
//	switchSched.registry(server);
	switchSched.registry(&automTerm, &automTerminal::exec);

	
	USART6->DR = 'H';
};


int main()
{
	setup();
	global_schedule();
};


void global_schedule()
{
while(1)
	{
		waitserver.check();
		switchSched.schedule();
		server();
	};
};