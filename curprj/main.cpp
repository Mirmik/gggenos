#include "genos/debug/debug.h"
#include "kernel/diag.h"
#include "hal/arch.h"

#include "asm/Serial.h"
#include "genos/kernel/time.h"

LinuxFileStream drv;
LinuxFileStream mrs;

int main()
{
	arch_init();	
	diag_init();

	mrs.open_in("/dev/ttyS3");
	//while(1)
	//	{
	//		std::cout << lfs.available() << std::endl;
	//	};
	mrs.print("\001201\00200\003F4");

dprln(millis());
delay(1000);

	mrs.close_in();
};