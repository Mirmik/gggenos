

//2012-2015 Mirmik 

#ifndef GENOS_SUBST_SCHEDULER
	#define GENOS_SUBST_SCHEDULER
	
	#include "inttypes.h"
	#include "stdio.h"
	#include "subst/SubstMacro.h"
		
	//Для невозврощающего копирования: минимальное количество байтов, которое необходимо скопировать,
	//для возвращения на один уровень стека вызовов. 
	//Может варьироваться в зависимости от процессора. Возможно, даже внутри семейства avr.
	#define THREAD_NORET_COPYSIZE PROC_STATE_SIZE + 2*sizeof(void*) 
	
	//Дескриптор нити.
	struct _thread 
	{
		size_t stack_pointer;	//Поле для сохранения указателя стека.
		size_t stack_top;		//Вершина стека.		
	};
	typedef struct _thread thread;
	extern "C" {		
		//Настроить нить как текущую.
		void init_current_thread(thread* thr, size_t ptr);
		
		//Инициализация стека.
		void set_stack_thread(thread* thr, size_t ptr);
		
		//Вызов смены нити.			
		void change_thread (thread* operated);
		
		//Функция копирует состояние нити из src в dst
		void copy_thread(thread* dst, thread* src);
		void copy_thread_part(thread* dst, thread* src, size_t count);
		
		void clone_thread(thread* dst);
		void clone_thread_noret(thread* dst);
		
		//Функция копирует состояние текущей нити в inited.
		//Возвращает 1 для родительской нити и 0 для inited нити.
		int fork_thread(thread* inited);
		
		//Функция обнуляет состояние нити inited и запускает в ней ф-ю func.
		//При возврате выдаёт системную ошибку
		int exec_thread(thread* inited, void(*)());
		int exec_thread_safe(thread* inited, void(*)());
		
		//Возвращает адрес дескриптора активной нити.
		thread* current_thread();
		
		//Указатель на активную нить. 
		//Раскомментировать для прямого использования.
		//extern thread* __current;
		
		int exec_current_thread(void(* func)());
		int exec_current_thread_safe(void(* func)());
		
		//drop_stack defined in platform/subst.h
		//long_goto defined in platform/subst.h
		
		//void current_thread_dump();
	};	
#endif
