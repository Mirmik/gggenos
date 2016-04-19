#include "genos/debug/debug.h"
#include "kernel/diag.h"
#include "hal/arch.h"

#include "genos/datastruct/ring_basic.h"

int main()
{
arch_init();
diag_init(); 

dprln("all good");
};