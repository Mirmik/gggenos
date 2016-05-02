#include "genos/debug/debug.h"
#include "kernel/diag.h"
#include "hal/arch.h"

#include "genos/container/slist_head.h"

slist_head s;

int main()
{
arch_init();
diag_init(); 
};