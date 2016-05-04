#include "genos/debug/debug.h"
#include "kernel/diag.h"
#include "hal/arch.h"

#include "asm/Serial.h"
#include "genos/schedproc/ucontextScheduler.h"
#include "genos/kernel/waitserver.h"
#include "genos/kernel/time.h"

#include "mrs.h"

LinuxFileStream drv;

uScheduler uSched;

MitsubishiCommunicator mcommunicator;

void loop();
void setup();

int main()
{	setup();
	while(1) loop();
};

void setup()
{

	arch_init();	
	diag_init();

	uSched.init();

	mcommunicator.Open("/dev/ttyS3");
	mcommunicator.Send("\001201\00200\003F8",10);

//delay(100);

//	while(mcommunicator.mrs.available()) debug_putchar(mcommunicator.mrs.getc());
	//mcommunicator.mrs.close_out();
	//mcommunicator.mrs.close_in();

	//uSched.registry(task);
	uSched.registry(delegate_mtd(&mcommunicator, &MitsubishiCommunicator::exec));
};

void loop()
{
	uSched.schedule();
	waitserver.check();
};