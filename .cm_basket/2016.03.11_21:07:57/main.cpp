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
//#include "asm/Serial.h"
#include "genos/debug/iteration_counter.h"

#include "syscontext/syscontext.h"
#include "genos/terminal/autom_terminal.h"
#include "genos/terminal/command_list.h"

#include "cpu/timers.h"

#include "cpu/usart_control_struct.h"

syscontext scntxt;
automTerminal automTerm;
automScheduler automSched;

uint64_t pulse_summary=0;

uint16_t ocr_new;

#ifndef TIMER1_COMPA_vect
#error "compa name"
#endif
ISR(TIMER1_COMPA_vect)
{
	OCR1A = ocr_new;
	pulse_summary++;
};

void emergency_stop()
{
	TCCR1B = 0;
	OCR1A = 0xFFFF;
	TIMSK1 = 0;
	debug_print("EMERGENCY_STOP\n");
};

timer tmr_blinker;
void blinker()
{
	gpio_change(13);
	msleep_autom_bias(&tmr_blinker, 1000);
};

void setup();
void loop();

int main(){
	setup();
	arch_deatomic();
	while(1) loop();
};

uint32_t ocr_to_pps(uint32_t ocr)
{
	return timer_get_freq(1) / ocr / 2;
};


uint16_t pps_to_ocr(uint64_t pps)
{
	uint64_t ret = (uint64_t)timer_get_freq(1) / pps / 2;
	assert (ret <= 0xFFFF);
	return ret;
};

void task(int, char**) 
{stdout.print("allgood");};

timer tmr1;
void manual_pulse()
{
	gpio_change(12);
	msleep_autom_bias(&tmr1, 1000);	
};


class SignalManipulier
{
	int8_t state = 0;
	float pps;
	uint16_t temp;

public:
	void exec() 
	{
		switch(state)
		{
			case 0: 
				TIMSK1 |= 1 << OCIE1A;
				OCR1A = 0xFFFF;
				ocr_new = 0xFFFF;
				state=1;
	
				timer1_source_set(timerSourceCK);
				timer1_wgm_set(timerWgm16CTCTopOCRnA);
				timer1_comB_set(timerComSpec);


				break;

			case 1: 
					pps = 6000 + 4000 * sin((float)millis() / 1000);
					
					//pps = 2000;

					debug_printdec_uint64(pps);dtb;
					debug_printdec_uint64(pulse_summary);dln;
					//debug_print("kekke\n");
					assert(pps >=0 );
					temp = pps_to_ocr(pps);
					arch_atomic();
					ocr_new = temp;
					arch_deatomic();
			break;
			case -1:
				stdout.print("case -1");
			break;
		};
	};
};

SignalManipulier sgmnlpr;

void setup(){
	arch_init();
	diag_init();

	machine_name = "aiop2";

	current_syscontext(&scntxt);
//	scntxt.__stdout.direct(&Serial0);	
//	scntxt.__stdin.direct(&Serial0);

	gpio_mode_out(13);
	gpio_mode_out(12);
	gpio_mode_out(31);

	cdelegate<void> d = makedfunc(&blinker);
	automSched.registry(d);

	cdelegate<void> d2 = makedelegate(&automTerm, &automTerminal::exec);
	automSched.registry(d2);

	cdelegate<void> d3 = makedelegate(&sgmnlpr, &SignalManipulier::exec);
	automSched.registry(d3);

	//cdelegate<void> d3 = makedfunc(&manual_pulse);
	//automSched.registry(d3);

	central_cmdlist.add("task", &task);
	central_cmdlist.add("timer", &timer_info);

	debug_delay(20000);

	arch_deatomic();
	usart_baudrate_set(&usart0, 9600);
	//usart_parity_set(&usart0, usartParityEven);
};

void loop(){
//	timerserv_check();
//	waitserv_check();
//	automSched.schedule();

	*usart0.udr = 'd';
};

