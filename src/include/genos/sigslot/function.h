#ifndef GENOS_FUNCTION_H
#define GENOS_FUNCTION_H

#include "genos/gstl/tuple.h"
#include "genos/sigslot/delegate.h"

template<typename R, typename ... Args> class function;
class function
{
public:
	delegate<R, Args ...> func;
	tuple<Args ...> tpl;
	R operator(Args ... args)() { return func(tpl); };
	function(delegate<R> _func, Args ... args) : func(_func), tpl(args ...) {};
};

#endif