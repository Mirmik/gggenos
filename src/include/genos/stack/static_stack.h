#ifndef STATIC_STACK_H
	#define STATIC_STACK_H
	
	#include "defines/size_t.h"
	
	
	//Класс-дескриптор стека.
	//По сути ни делает ничего, кроме резервирования места.
	template<size_t N>
	class static_stack{
		
		public:
		//Возврат указателя на последний байт стека (стек растет 
		//от старших байт к младшим).  
		void* top(){return (reinterpret_cast<void*>(this + 1) - 1);};
		
		protected:
		uint8_t size[N];
	};
	
	
	
#endif