#ifndef GENOS_SUBST_SCHED
	#define GENOS_SUBST_SCHED
	

	#include <genos/subst/subst.h>
	#include <genos/stack/dynamic_stack.h>
	#include <genos/base_struct/list.h>
	
	#include "genos/schedproc/scheduler_base.h"
	#include "genos/schedproc/process_base.h"
	
	
	//Структура, содержащая информацию о процессе.
	class process : public process_base_with_thread 
	{};
	
	class gScheduler : public subst_scheduler_base
	{
		//Ресурсы:
		list_head* current;		
		
		public:
		//Методы:
		void init();					//Регистрация текущей нити (создаётся нить, топ стека 
										//которой принимается за RAMEND), 
										//подготовка к запуску
										
		process* fork(size_t stcksize);	//Создаёт новую нить
										//принимает размер отводимого стека
										//возвращает родителю адрес порождаемого процесса  
										//возвращает предку ноль
		
		void exit();					//Завершение текущей нити.
		
		void schedule(); 				//Собственно, вызов планировщика (обычно из прерывания)
		void proc_go_wait();
		
		//Для дебага:
		void print_list();				//Вывод в Serial списка процессов
		
		//Утилиты:
		void proc_delete(process* proc);//Уничтожить процесс. (Активный процесс уничтожать запрещено.)
		
		//Конструктор
		gScheduler() {};
	};
		
	
	
#endif				