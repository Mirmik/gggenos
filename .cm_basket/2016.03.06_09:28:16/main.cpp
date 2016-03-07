#include "hal/arch.h"

#include "kernel/diag.h"
#include "genos/debug/debug.h"
#include "genos/schedproc/taskSched.h"
#include "stdlib.h"

#include "genos/terminal/automTerminal.h"
#include "asm/Serial.h"
#include "hal/gpio.h"
#include "asm/hal/timers.h"

#include "avr/io.h"

automTerminal aTerm(Serial0, Serial0);

void task(){aTerm.exec();};

int main(){
arch_init();
diag_init();

gpio_mode_out(13);
//gpio_hi(13);

Serial0.begin(9600);
arch_deatomic();

taskSched.add(task);

while(1) taskSched.schedule();

};