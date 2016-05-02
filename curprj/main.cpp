#include "genos/debug/debug.h"
#include "kernel/diag.h"
#include "hal/arch.h"


#include <iostream>
#include "genos/sigslot/sigslot.h"

//#include "gmath/matrix.h"
#include "genos/pubsub/core.h"

MessageCore Core;

int main()
{
	arch_init();	
	diag_init();
};