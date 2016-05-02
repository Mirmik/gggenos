#include "genos/debug/debug.h"
#include "kernel/diag.h"
#include "hal/arch.h"

//#include <iostream>
//#include <functional>
#include <utilxx/type_analize.h>
#include <genos/sigslot/syncdata.h>

#include <genos/container/mvector.h>

#include <genos/gstl/algorithm.h>

//syncdata<float> fl;
mvector<uint16_t> v;

int main()
{
arch_init();
diag_init(); 

//fl.update(0.2);

v.push_back(0xAA);
v.push_back(0xBB);
v.push_back(0xCC);

dprhexln(v[0]);
dprhexln(v[1]);
dprhexln(v[2]);

v.remove(1);

gstd::for_each(v.begin(), v.end(), [](uint16_t& h)
	{
		dprhexln(h);
	});

for (auto &a : v)
	{
		dprhexln(a);
	};

};