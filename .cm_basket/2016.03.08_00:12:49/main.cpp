#include "hal/arch.h"

#include "kernel/diag.h"
#include "genos/debug/debug.h"
#include "genos/schedproc/taskSched.h"
#include "stdlib.h"

#include "genos/terminal/automTerminal.h"
//#include "asm/Serial.h"
#include "hal/gpio.h"
#include "asm/hal/timers.h"

//automTerminal aTerm(Serial0, Serial0);

void task(){debug_print("kekek");};

int main(){
arch_init();
diag_init();

debug_delay(20000);

taskSched.add(task);
timer1_source(timerSourceCK);

gpio_mode_out(13);
gpio_hi(13);

debug_printhex_uint8(DDRB);dln;
debug_printhex_uint8(PORTB);dln;

dpr_hex(WGM00);


//while(1) taskSched.schedule();
};