#ifndef GENOS_UTILXX_STUB_H
	#define GENOS_UTILXX_STUB_H
	
	#include "genos/debug/debug.h"
	
	template <typename R,typename ... V> R tg_do_nothing(V...){};
	template <typename R,typename ... V> R tg_error_stub(V...){debug_panic("error_stub");};
	
	
#endif