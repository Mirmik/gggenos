#include "genos/debug/debug.h"
#include "kernel/diag.h"
#include "hal/arch.h"

//#include <iostream>
//#include <functional>
#include <utilxx/type_analize.h>
#include <genos/sigslot/syncdata.h>

#include <genos/container/mvector.h>

#include <genos/gstl/algorithm.h>

class A
{
public:
	string to_str() const
	{
		return string("aaa");
	};
};

//syncdata<float> fl;
mvector<int16_t> v, y;

int main()
{
arch_init();
diag_init(); 
//v.reserve(10);
v << 8 << 9 << 7 << 5 << 78 << -56 << 89 << 98 << 76 << 9 << 7 
<< 5 << 78 << 56 << 89;
dpr(v.to_str());
};