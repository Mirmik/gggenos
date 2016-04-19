#include "genos/debug/debug.h"
#include "kernel/diag.h"
#include "hal/arch.h"
#include "utilxx/type_analize.h"

#include "genos/container/dlist_head.h"
#include "genos/container/dlist.h"
#include "utilxx/member.h"

#include "genos/gstl/algorithm.h"
#include "asm/Serial.h"

#include "genos/io/SerToSer.h"

LinuxStream strm;

class A
{
public:
	int a;
	dlist_head lst;
} a,b;

dlist<A, &A::lst> dl;
dlist<A, &A::lst> dl2;

int main()
{
arch_init();
diag_init(); 

strm.println("hhhh");

dl.push_back(a);
dl.push_back(b);

dl2.push_back(b);

dl.lite_pop(b);

dprptrln(&dl);
dprptrln(&dl2);
dprptrln(dlist_topic(a, lst));
dprptrln(dlist_topic(b, lst));

dprln(dl.to_info());
dprln(dl2.to_info());

};