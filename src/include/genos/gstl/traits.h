#ifndef GENOS_GSTL_TRAITS_H
	#define GENOS_GSTL_TRAITS_H
	
	#include "sys/types.h"
	
	namespace genos{
		
		
		template <typename T>
		class elem_traits {
		public:
		typedef const T& arg_type;
		typedef 	  T* ptr_type;
		typedef const T* const_ptr_type;
		typedef       T& reference;
		typedef const T& const_reference;
		typedef 	  size_t size_type;
		typedef 	  size_t ptrdiff_t;
		};
	
		template <>
		class elem_traits<char> {
		public:
		typedef const char  arg_type;
		typedef 	  char* ptr_type;
		typedef const char* const_ptr_type;
		typedef       char& reference;
		typedef const char& const_reference;
		typedef 	  size_t size_type;
		typedef 	  size_t ptrdiff_t;
		};
		
		
		template <>
		class elem_traits<int> {
		public:
		typedef const int  arg_type;
		typedef 	  int* ptr_type;
		typedef const int* const_ptr_type;
		typedef       int& reference;
		typedef const int& const_reference;
		typedef 	  size_t size_type;
		typedef 	  size_t ptrdiff_t;
		};

		template <>
		class elem_traits<unsigned int> {
		public:
		typedef const unsigned int  arg_type;
		typedef 	  unsigned int* ptr_type;
		typedef const unsigned int* const_ptr_type;
		typedef       unsigned int& reference;
		typedef const unsigned int& const_reference;
		typedef 	  size_t size_type;
		typedef 	  size_t ptrdiff_t;
		};

		
		template <>
		class elem_traits <void>{
		public:
		typedef 	  void* ptr_type;
		};
		
		};	
	
#endif