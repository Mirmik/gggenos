#include "genos/debug/debug.h"
#include "kernel/diag.h"
#include "hal/arch.h"

#include "genos/sigslot/future/delegate.h"


class A
{
public:
	void task(int i){dpr("hello world2");dpr(i);};
} a;

int ttt(float f){ return 32; };

int main()
{
arch_init();
diag_init(); 

fastdelegate<void,int> fd = delegate_mtd(&a, &A::task);

delegate<void,int> d = fd;
callback<int, float> c = ttt;

dpr(ttt(0.2));

d(22);

};