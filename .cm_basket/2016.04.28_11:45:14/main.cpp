#include "genos/debug/debug.h"
#include "kernel/diag.h"
#include "hal/arch.h"

#include <iostream>
#include <functional>
#include <utilxx/type_analize.h>

using namespace std;


function<int()> f() {
	//int x = 0;
	return [=] () mutable {return 0; };
};

int main()
{
arch_init();
diag_init(); 

auto fun = f();
for (int i = 0; i < 5; ++i) {
	cout << fun() << endl;
}

dprln(sizeof(f()));

};