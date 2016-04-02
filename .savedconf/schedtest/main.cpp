#include "hal/arch.h"
#include "genos/debug/debug.h" 
#include "genos/time/sysclock.h"
#include "genos/debug/delay.h"
#include "hal/gpio.h"

#include "configure.h"

#include "genos/kernel/waitserver.h"
#include "asm/Serial.h"
#include "genos/terminal/command_list.h"
#include "syscontext/syscontext.h"

#include "genos/stack/static_stack.h"
#include "kernel/context.h"

#include "genos/schedproc/switchScheduler.h"

void global_schedule();

void emergency_stop()
{
	usart2_interrupt_disable();
//	usart6_interrupt_disable();
	
	debug_print("EMERGENCY_STOP");
};


context cntxt1, cntxt2, cntxt3; 
static_stack<500> stck2, stck3;

HardwareSerial Serial2;
syscontext syscntxt;
switchScheduler switchSched;

void* operator new(size_t, void* ptr) {return ptr;};
void global_constructor_stub()
{
	new(&waitserver) WaitServer;
	new(&Serial2) HardwareSerial;
	new(&syscntxt) syscontext;
	new(&central_cmdlist) command_list;
	new(&switchSched) switchScheduler;
};


void task1()
{
while(1)
	{
	gpio_port_tgl_pin(GPIOD, 13);
	msleep_subst(200);
	};
};


void task2()
{
while(1)
	{
	gpio_port_tgl_pin(GPIOD, 15);
	msleep_subst(1000);
	};
};

context schedule_context;
void setup(){
	global_constructor_stub();
	arch_init();
	project_configure();

	current_context_set(&schedule_context);

	current_syscontext(&syscntxt);
	stdout.direct(&Serial2);
	stdin.direct(&Serial2);

	Serial2.usart = USART2;
	gpio_port_set_pin(GPIOD, 12);

//	context_init(&cntxt2, 0, task1, stck2.top());
//	context_init(&cntxt3, 0, task2, stck3.top());
//	context_switch(&cntxt1, &cntxt2);
//	context_switch(&cntxt1, &cntxt3);

	switchSched.registry(task2);
	switchSched.registry(task1);
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
	};
};