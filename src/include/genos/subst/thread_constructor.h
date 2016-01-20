#ifndef GENOS_THREAD_CONSTRUCTOR
	#define GENOS_THREAD_CONSTRUCTOR
	
	
	#include <genos/stack/dynamic_stack.h>
	#include <genos/debug/debug.h>
	//#include <genos/base_struct/list.h>
	#include <genos/subst/subst.h>
	
	
	static constexpr uint8_t THR_NODELETED 	= 	0x01;
	
	class thread_constructor{
		public:
		thread* thr;				//Указатель на объект нити.
		dynamic_stack* stack;		//Указатель на объект стека. (Сохраняется для возможного удаления)
		thread_constructor() : thr(0), stack(0), thr_flags(0) {}; 
		
		void destroy_thread();							//Уничтожает нить и стек (Если разрешено).
		void construct_thread(size_t size_of_stack);	//Конструирует нить и стек.
		void construct_current_thread(size_t stack_top);	//Инициализирует объект текущей нить
		uint8_t thr_flags;
		
		
	};
	
	
	
	
	
	
	
	
#endif 