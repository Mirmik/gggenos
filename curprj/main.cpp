#include "hal/arch.h"
#include "kernel/diag.h"
#include "genos/decoration.h"
#include "genos/debug/debug.h"
#include "hal/gpio.h"
#include "genos/time/sysclock.h"
#include "genos/time/timeserv.h"
#include "genos/wait/waitserv.h"
#include "genos/schedproc/automSched.h"
#include "genos/io/stream.h"
#include "asm/Serial.h"
#include "genos/debug/iteration_counter.h"

#include "syscontext/syscontext.h"
#include "genos/terminal/autom_terminal.h"
#include "genos/terminal/command_list.h"

syscontext scntxt;
automTerminal automTerm;
automScheduler automSched;

command_list cmdlist;

void emergency_stop()
{
	debug_print("EMERGENCY_STOP\n");
};

timer tmr1;
void blinker()
{
	gpio_change(13);
	msleep_autom_bias(&tmr1, 1000);
};

void setup();
void loop();

int main(){
	setup();
	arch_deatomic();
	delay(500);
	while(1) loop();
};

void setup(){
	arch_init();
	diag_init();

	machine_name = "aiop2";

	current_syscontext(&scntxt);
	scntxt.__stdout.direct(&Serial0);	
	scntxt.__stdin.direct(&Serial0);

	gpio_mode_out(13);
	gpio_mode_out(31);

	cdelegate<void> d = makedfunc(&blinker);
	automSched.registry(d);

	cdelegate<void> d2 = makedelegate(&automTerm, &automTerminal::exec);
	automSched.registry(d2);
};

void loop(){
	timerserv_check();
	waitserv_check();
	automSched.schedule();
};

