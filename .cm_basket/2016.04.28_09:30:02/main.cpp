#include "genos/debug/debug.h"
#include "kernel/diag.h"
#include "hal/arch.h"

int main()
{
arch_init();
diag_init(); 
};