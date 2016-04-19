#include "genos/debug/debug.h"
#include "kernel/diag.h"
#include "hal/arch.h"
#include "utilxx/type_analize.h"

#include "genos/container/dlist_head.h"
#include "genos/container/dlist.h"
#include "utilxx/member.h"

#include "genos/gstl/algorithm.h"
class A
{
public:
	dlist_head lst;
	int i;
} a, b, c, d;

dlist<A, &A::lst> dl;

int main()
{
arch_init();
diag_init(); 

dl.push_back(a);
dl.push_back(b);
dl.push_back(c);
dl.push_next(d, b);

dl.pop_back();
dl.pop_front();

auto dl2 = dlist_head_to_class<A,&A::lst>(&dl.list);

dl2->pop_back();


a.i = 1;
b.i = 2;
c.i = 3;
d.i = 4;

gstd::for_each(dl.begin(), dl.end(), [](A& a)
	{
		dprln(a.i);
	});

dpr(dl.to_info());
};