#include "hal/arch.h"

#include "kernel/diag.h"
#include "genos/debug/debug.h"
//#include "genos/schedproc/taskSched.h"
#include "stdlib.h"

#include "genos/terminal/automTerminal.h"
//#include "asm/Serial.h"
#include "hal/gpio.h"
#include "asm/hal/timers.h"

#include "genos/time/sysclock.h"
#include "genos/gstl/container/vector.h"

#include "genos/sigslot/delegate2.h"
#include "genos/time/timeserv.h"

#include "genos/schedproc/automSched.h"
//#include "genos/gstl/container/map.h"

automScheduler automSched;

//automTerminal aTerm(Serial0, Serial0);

void task()
{
	debug_print("kkk");

};


void task2()
{
	debug_print("aaa");
};

gstd::vector<int> vi;

int main(){
arch_init();
diag_init();

debug_print("here");

arch_deatomic();

debug_print("here2");

gpio_mode_out(13);
gpio_hi(13);

gpio_mode_out(30);
gpio_hi(30);

cdelegate<void> d = makedfunc(&task);
automSched.registry(d);

d = makedfunc(&task2);
automSched.registry(d);

gpio_mode_out(31);
gpio_hi(31);

debug_delay(20000);

vi.push_back(3);
vi.push_back(4);

debug_printhex_uint32(vi[0]);

while (1) {automSched.schedule();};

};