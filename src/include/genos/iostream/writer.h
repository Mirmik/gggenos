#ifndef GENOS_WRITER
	#define GENOS_WRITER
	
	#include "string.h"

	template<typename T>
	class Writer{
	public:
		virtual	int write(const T c) = 0;
		virtual	int write(const T* c, int n) = 0;
		virtual	int flush() = 0;	

		int print(const T* c) {return write(c, strlen(c));};
	};

#endif