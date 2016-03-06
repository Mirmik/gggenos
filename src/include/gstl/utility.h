
#ifndef GENOS_GSTL_UTILITY
	#define GENOS_GSTL_UTILITY


#include <cstddef>

//Реализации разбиты по нескольким файлам, дабы не было лапши из кода.
//Некоторые шаблоны могут врать... Извините.
//UNDER_CONSTRUCTED

#include <gstl/algorithm.h>

namespace std{
#include "gstl/utility/prototype.hpp"

#include "gstl/utility/pair.hpp"
#include "gstl/utility/utility.hpp"
#include "gstl/utility/type_traits.hpp"
#include "gstl/utility/type_transform.hpp"
#include "gstl/utility/type_relation.hpp"
#include "gstl/utility/type_obj_magic.hpp"
#include "gstl/utility/move.hpp"
};

template <typename T, T Ptr>
struct function_pack;
 
template <typename R, typename S, typename ...Args, R(S::*Ptr)(Args...)>
struct function_pack<R(S::*)(Args...), Ptr>
{
	using function_ptr_type = R(*)(Args...);
	static constexpr function_ptr_type ptr = Ptr;
	void* obj;
	function_pack(S* _obj) : obj((void*)_obj) {};
};
template <typename R, typename ...Args, R(*Ptr)(Args...)>
struct function_pack<R(*)(Args...), Ptr>
{
	using function_ptr_type = R(*)(Args...);
	static constexpr function_ptr_type ptr = Ptr;
	void* obj = 0;
};
 
 
 
		template<typename T> struct mtd_parser{};
		
		template<typename R, typename T, typename ... V>
		struct mtd_parser<R(T::*)(V...)>
		{
		typedef R output_type;
		typedef T basic_type;
		//typedef V variable_type;
		};
 
		template<typename T> struct func_parser{};
		
		template<typename R, typename ... V>
		struct func_parser<R(*)(V...)>
		{
		typedef R output_type;
		};
 
 
 
 
 
 
#define DECL_FUNC(a, F) function_pack<decltype(F), (F)>(a)




#endif