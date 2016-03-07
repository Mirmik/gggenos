#include "hal/arch.h"
#include "kernel/diag.h"
#include "genos/debug/debug.h"
#include "hal/gpio.h"
#include "genos/time/sysclock.h"
#include "genos/time/timeserv.h"
#include "genos/wait/waitserv.h"
#include "genos/schedproc/automSched.h"

#include "genos/io/stream.h"

#include "asm/Serial.h"
#include "genos/debug/iteration_counter.h"
int a=2, b=3;

int main(){
arch_init();
diag_init();

arch_deatomic();

delay(1000);

Serial0.printhexln(0XABCDEF);
Serial0.println(123);

Serial0.printptr(&a);

Serial0.printptr(&b);
Serial0.printdump(&a,100);

while(1) {
	delay(500);
	gpio_mode_out(13);
	gpio_change(13);
};

};