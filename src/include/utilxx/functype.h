#ifndef GENOS_UTIL_FUNCTYPE
#define GENOS_UTIL_FUNCTYPE

	template<typename R, typename... A>
	using func_t = R(*)(A ...);

#endif