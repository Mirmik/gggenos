#include "genos/debug/debug.h"
#include "kernel/diag.h"
#include "hal/arch.h"

//#include "genos/sigslot/delegate.h"

//#include "utilxx/ring.h"

#include "stdlib.h"

#include "genos/gstl/tuple.h"
#include "genos/sigslot/function.h"

void task( int i ) {dprln("HelloWorld");dprln(i);};
void task2(int i, int j, int k) {
	dprln("HelloWorld");dprln(i);dprln(j);dprln(k);};


int main()
{
arch_init();
diag_init(); 

//tuple t;

dprln(27);

//function<void> f(task);

//f();

function<void,int> f2(task,89);
function_param_first(f2) = 13;
//f2.first = 2 * f2.second;
f2();
dprln(sizeof(f2));
};