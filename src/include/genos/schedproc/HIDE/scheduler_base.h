#ifndef GENOS_SCHEDULER_BASE
	#define GENOS_SCHEDULER_BASE
	
	#include <genos/debug/debug.h>
	#include <genos/base_struct/list.h>
	#include <genos/subst/subst.h>
	#include <genos/schedproc/process_base.h>
	
	#include "genos/subst/thread_constructor.h"
	
	
	
	static constexpr uint8_t USER_BLOCK	=	0x01;
	static constexpr uint8_t SCHED_BLOCK=	0x02;
	static constexpr uint8_t NO_INIT	=	0x04;
	static constexpr uint8_t BLOCK_MASK	=	0x0F;
	
	
		//Заголовок списка процессов
		extern list_head process_list;
		extern list_head running_list;
		extern list_head waiting_list;
		extern list_head zombie_list;
		extern list_head stop_list;
	
	
	process_base* current_process();
	void current_process(process_base*);
	
	
	///Планировщик. Базовый класс.
	class scheduler_base
	{public:
		uint8_t sched_flags;					//Ресурсы планировщика
		
		static scheduler_base* __sched;
		
		
		friend scheduler_base& scheduler();
		friend void scheduler(scheduler_base& _sched);
				
		void block();					//Внешняя блокировка планирования
		void unblock();				
		
		//Методы:
		bool ready();					//Возвращает готовность к планированию.
										//Планировщик не готов, если установлен 
										//Один из битов в BLOCK_MASK либо 
										//не обнулена переменная process_operation
		
		
		//Обобщенный интерфейсы для работы внешних сервисов, типа waiter
		virtual void schedule()=0;
		virtual void proc_go_wait(){};
		scheduler_base() : sched_flags(NO_INIT) {if (__sched!= nullptr) debug_panic("Second scheduler!");__sched=this;};
		};
	
	
	
	
	
	///Планировщик вытесняющего планирования. Базовый класс.
	class subst_scheduler_base : public scheduler_base
	{public:
		//Ресурсы:	
		
		//Утилиты:
		void block_sched();				//Внутренняя блокировка планирования
		void unblock_sched();			
		
		//Конструктор
		subst_scheduler_base() {bits_set(sched_flags, USER_BLOCK);}; 
	};	
	
	
	void debug_all_list_print();
	void debug_process_list_print();
	void debug_running_list_print();
	void debug_waiting_list_print();
	void debug_zombie_list_print();
	void debug_stop_list_print();
	
		
	
	scheduler_base& scheduler();
	void scheduler(scheduler_base& _sched);
	void move_tail_running_list(process_base* proc);
	
#endif