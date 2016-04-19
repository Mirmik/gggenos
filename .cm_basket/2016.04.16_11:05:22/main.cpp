#include "genos/debug/debug.h"
#include "kernel/diag.h"
#include "hal/arch.h"

//#include "genos/sigslot/delegate.h"

//#include "utilxx/ring.h"

#include "stdlib.h"

#include "util/stub.h"
//#include "genos/gstl/tuple.h"
//#include "genos/sigslot/function.h"


class Aaa
{
public:
	int i;
	Aaa(int _i):i(_i){dprln("constructor");};
	~Aaa(){dpr("destructor");dprln(i);};
};


int main()
{
arch_init();
diag_init(); 

ret_char() = Aaa(2);

dprln("here2");

//tuple<int, int, int> t(4, 3, 2);
//t2 = t;

//t.get<2>() = 33;
//dprln(get<0>(t));
//dprln(t.get<2>());

//dprhex_dump(&t, 12);

};