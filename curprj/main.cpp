#include "genos/debug/debug.h"
#include "kernel/diag.h"
#include "hal/arch.h"

#include "genos/schedproc/ucontextScheduler.h"
#include "genos/kernel/waitserver.h"

uScheduler uSched;

volatile uint8_t flag = 0;

void task0()
{
while(1)
{
	msleep(1000);
	flag = 1;
	while(1);
};
};

void task1()
{
while(1)
{
	dprln("task1");
	while(1);
};
};

int main()
{
	arch_init();	
	diag_init();

	uSched.init();
	uSched.registry(&task0);
	uSched.registry(&task1);

	while(1)
	{
	//	dprln((uint32_t)millis());
		uSched.schedule();
		waitserver.check();
		//dprln(uSched.running_list.to_info());
	};
};