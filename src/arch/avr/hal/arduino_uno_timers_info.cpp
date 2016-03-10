#include "cpu/timers.h"

#include "genos/io/stream.h"
#include "syscontext/syscontext.h"

#include "stdlib.h"
#include "utilxx/functype.h"
#include "util/stub.h"

#define TOTAL_TIMERS 2

class timer_ops 
{
	public:
	func_t<uint8_t> source = func_t<uint8_t>(do_nothing);
	func_t<uint8_t> wgm  = func_t<uint8_t>(do_nothing);
	func_t<uint8_t> comA = func_t<uint8_t>(do_nothing);
	func_t<uint8_t> comB = func_t<uint8_t>(do_nothing);
	func_t<uint8_t> comC = func_t<uint8_t>(do_nothing);
	func_t<int> Div = func_t<int>(do_nothing);
	
	//ctor
	timer_ops
	(
		func_t<uint8_t> s,
		func_t<uint8_t> w,
		func_t<uint8_t> cA,
		func_t<uint8_t> cB,
		func_t<uint8_t> cC,
		func_t<int> d
	) :	source(s), wgm(w), comA(cA), comB(cB), comC(cC), Div(d) {};
};

timer_ops timer0(
		timer0_source_get,
		timer0_wgm_get,
		timer0_comA_get,
		timer0_comB_get,
		(func_t<uint8_t>) do_nothing,
		timer0_divider_get
		);

timer_ops timer1(
		timer1_source_get,
		timer1_wgm_get,
		timer1_comA_get,
		timer1_comB_get,
		(func_t<uint8_t>) do_nothing,
		timer1_divider_get
		);

timer_ops* tms[TOTAL_TIMERS] = 
{
	&timer0,
	&timer1
};


void timer_info(int argc, char** argv)
{
	//debug_printdec_uint32(argc);

	if (argc != 2)
	{
		stdout.println("Wrong param count");
		return;
	};
	timer_info(atoi(argv[1]));
};

void timer_info(int tnum)
{
	if (tnum >= TOTAL_TIMERS)
	{
		stdout.println("Wrong timer number");
		return;
	};

	stdout.print("Timer:\t\t\t");
	stdout.println(tnum);

	stdout.print("Source:\t\t\t");
	stdout.printhexln(tms[tnum]->source());

	stdout.print("Wgm:\t\t\t");
	stdout.printhexln(tms[tnum]->wgm());

	stdout.print("ComA:\t\t\t");
	stdout.printhexln(tms[tnum]->comA());

	stdout.print("ComB:\t\t\t");
	stdout.printhexln(tms[tnum]->comB());

	stdout.print("Freq/Takt (t/s): \t");
	switch(tms[tnum]->source())
	{
		case timerSourceStop: stdout.print("STOPPED"); break;
		case timerSourceTaktRising: stdout.print("T0 Rissign"); break;
		case timerSourceTaktFalling: stdout.print("T0 Falling"); break;
		default: stdout.print(timer_get_freq(tnum)); break;
	};
	stdout.println();
};

int32_t timer_get_freq(int num)
{
	return F_CPU / tms[num]->Div();
};