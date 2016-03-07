#ifndef GENOS_GSTL_ALLOCATOR
	#define GENOS_GSTL_ALLOCATOR
	
	#include "genos/gstl/traits.h"
	
	namespace gstd{
	
			
	template<typename T>	T* 		sys_allocate		(size_t n)			
	{return static_cast<T*>(malloc(sizeof(T) * n));};
	
	template<typename T>	void 	sys_deallocate	(T* ptr)	{free((void*)ptr);};

		
		template <typename T, typename traits_type = elem_traits<T>>
		class allocator { 
			public:
			typedef typename traits_type::	ptr_type 			pointer;
			typedef typename traits_type::	const_ptr_type 		const_pointer;
			typedef typename traits_type::	reference 			reference;
			typedef typename traits_type::	const_reference 	const_reference;
			typedef 						T 					value_type;
			typedef typename traits_type::	size_type 			size_type;
			typedef typename traits_type::	ptrdiff_t 			difference_type;
			
			allocator(){};
			~allocator(){};
			//pointer address(reference x);
			//const_pointer const_address(const_reference x);
			//size_type init_page_size();
			//size_type max_size();
			
			pointer allocate(size_type n) {return sys_allocate<T>(n);};
			void deallocate(pointer p){sys_deallocate(p);};
		};
		
		
		template<>
		class allocator<void, elem_traits<void>> { 
			public:
			typedef typename elem_traits<void>::ptr_type pointer;
			allocator(){};
			~allocator(){};
		};

	
		
	};
	
	
	
#endif