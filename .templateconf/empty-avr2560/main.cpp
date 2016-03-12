#include "hal/arch.h"
#include "kernel/diag.h"
#include "genos/debug/debug.h"
#include "hal/gpio.h"
#include "genos/time/sysclock.h"

void emergency_stop()
{
	debug_print("EMERGENCY_STOP\n");
};

void setup();
void loop();

int main(){
	setup();
	arch_deatomic();
	while(1) loop();
};

void setup(){
};

void loop(){
};

