
#ifndef GENOS_FARSEER
	#define GENOS_FARSEER
	
	#include "genos/types.h"
	//#include "genos/gstl/container/uncreated_list.h"
	#include "genos/gstl/hassign.h"
	//#include "genos/debug/helper_macro.h"
	#include "genos/base_struct/evident.h"
	#include "genos/base_struct/list.h"
	
	
	class farseer_variable;
	extern list_head farseer_list;	
	
	class farseer_variable
	{public:
		const char* name;	
		const char* func;	
		evident var;
		list_head list;
		
		farseer_variable();
		
		template<typename T>
		farseer_variable(const char* _name, const char* _func, T _var) : 
		name(_name), func(_func), var(_var) 
		{
			list_add(&list, &farseer_list);
		};
	};
	
	void farseer_all_see();
	
	
	
	
	
	
	#define global_mirror(local_var) {debug_place(); static farseer_variable _concate(global_mirror_,local_var)(_str_generate(local_var), __FUNCTION__, local_var); hassign(_concate(global_mirror_,local_var).var,local_var);};
	
	#define update_mirror(local_var) debug_place(); hassign(_concate(global_mirror_,local_var).var,local_var);
	
	
	
	
	
	
	
	
	
#endif