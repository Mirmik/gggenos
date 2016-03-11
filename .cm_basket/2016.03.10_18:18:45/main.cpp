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

#include "cpu/timers.h"

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
	OCR1A = 0xFFFF;
	TIMSK1 = 0;
	debug_print("EMERGENCY_STOP\n");
};

timer tmr1;
void blinker()
{
	gpio_change(31);
	msleep_autom_bias(&tmr1, 1000);
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

void setup(){
	arch_init();
	diag_init();

	machine_name = "aiop2";

	current_syscontext(&scntxt);
	scntxt.__stdout.direct(&Serial0);	
	scntxt.__stdin.direct(&Serial0);

	gpio_mode_out(13);
	gpio_mode_out(12);
	gpio_mode_out(31);

	cdelegate<void> d = makedfunc(&blinker);
	automSched.registry(d);

	cdelegate<void> d2 = makedelegate(&automTerm, &automTerminal::exec);
	automSched.registry(d2);

	central_cmdlist.add("task", &task);

	debug_delay(20000);

	arch_deatomic();

	timer1_source_set(timerSourceCK);
	timer1_wgm_set(timerWgm16CTCTopOCRnA);
	timer1_comB_set(timerComSpec);

	//gpio_hi(12);
	ocr_new = 0xFFFF;
	OCR1A = 0xFFFF; 
	//debug_printdec_uint32(ocr_to_pps(125000));dln;


	debug_printdec_uint32(ocr_to_pps(0x0FFF));dln;
	debug_printhex_uint32(pps_to_ocr(1953));dln;
	TIMSK1 |= 1 << OCIE1A;
	//OCR1A = 0x400;


	//while(1);
	debug_printdec_uint32(OCR1A);dln;
	debug_printdec_uint32(ocr_to_pps(OCR1A));dln;

	//while(1);

	timer_info(0);
	timer_info(1);

	stdout.println();
};

uint16_t temp;
float pps;
void loop(){
	
	pps = 6000 + 4000 * sin((float)millis() / 1000);
	assert(pps >=0 );
	temp = pps_to_ocr(pps);
	debug_print("pulses:"); debug_printdec_uint32(pulse_summary);dtb; 
	debug_print("ocr:"); debug_printdec_uint32(temp);dtb;
	debug_print("pps:"); debug_printdec_uint32(pps);dln;
	arch_atomic();
	ocr_new = temp;
	arch_deatomic();
	

	//timerserv_check();
	//waitserv_check();
	//automSched.schedule();
};

