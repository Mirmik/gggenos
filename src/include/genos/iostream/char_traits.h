#ifndef GENOS_CHAR_TRAITS_H
	#define GENOS_CHAR_TRAITS_H
	
#include <sys/types.h>
	
	
	namespace genos{
		
		template<typename T>
		class char_traits{
			
			public:
			typedef const T& arg_type;
			typedef 	  T* ptr_type;
			typedef const T* const_ptr_type;
			typedef       T& reference;
			typedef const T& const_reference;
			typedef 	  size_t size_type;
			typedef 	  size_t ptrdiff_t;
			
			
			
			size_t length(const T* c);
			
		};
		
	};
	
	
			
	
#endif