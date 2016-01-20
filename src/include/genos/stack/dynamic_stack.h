#ifndef DYNAMIC_STACK_H
	#define DYNAMIC_STACK_H
	
	#include "inttypes.h"
	#include "stdlib.h"
		
		
		
		class dynamic_stack{
		
			public:
			//Возврат указателя на последний байт стека (стек растет 
			//от старших байт к младшим).  
			dynamic_stack(size_t _size) : size(_size) {};
			size_t top(){return (reinterpret_cast<size_t>(this + 1) + size - 1);};
			
			
			
			public:
			size_t size;
		};
		
		
			dynamic_stack* construct_dynamic_stack(size_t size);
		
#endif