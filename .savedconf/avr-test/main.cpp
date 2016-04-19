#include "hal/arch.h"
#include "kernel/diag.h"
#include "genos/debug/debug.h"
#include "hal/gpio.h"
#include "genos/time/sysclock.h"

#include "genos/sigslot/future/delegate.h"
void emergency_stop()
{
	debug_print("EMERGENCY_STOP\n");
};

void setup();
void loop();

int main(){
	setup();
	while(1) loop();
};

class A
{
public:
	void func(){dpr("HelloWorld");};
} a;

fastdelegate<void> fd = delegate_mtd(&a, &A::func);

void setup(){
	arch_init();
	diag_init();
	fd();
};


void loop(){
};




