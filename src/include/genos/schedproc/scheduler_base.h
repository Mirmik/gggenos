#ifndef GENOS_SCHEDULER_BASE
	#define GENOS_SCHEDULER_BASE
	
	#include <genos/debug/debug.h>
	#include <genos/datastruct/list.h>
//	#include <genos/subst/subst.h>
	#include <genos/schedproc/process_base.h>
	
	//#include "genos/subst/thread_constructor.h"
	
	
	
	static constexpr uint8_t USER_BLOCK	=	0x01;
	static constexpr uint8_t SCHED_BLOCK=	0x02;
	static constexpr uint8_t NO_INIT	=	0x04;
	static constexpr uint8_t BLOCK_MASK	=	0x0F;
	
	
	process_base* current_process();
	void current_process(process_base*);
	
	class scheduler_base;
		
	scheduler_base* scheduler();
	void scheduler(scheduler_base* _sched);
	
	///Планировщик. Базовый класс.
	class scheduler_base
	{public:
		uint8_t sched_flags;					//Ресурсы планировщика
		
				
		void block();					//Внешняя блокировка планировани€
		void unblock();				
		
		//Методы:
		bool ready();					//Возвращает готовность к планированию.
										//Планировщик не готов, если установлен 
										//Один из битов в BLOCK_MASK либо 
										//не обнулена переменна€ process_operation
		
		virtual void process_set_running(process_base* proc)=0;
		virtual void process_set_wait(process_base* proc)=0;
		virtual void process_set_zombie(process_base* proc)=0;
		virtual void process_set_stop(process_base* proc)=0;
		
		//TODO
		//virtual void change_process_state(process_base* process,uint8_t state){};
	
			//void process_executed(process_base* proc);
			//void process_unexecuted(process_base* proc);
		
			void process_unbind(process_base* proc);
		
		//Обобщенный интерфейсы дл€ работы внешних сервисов, типа waiter
		virtual void schedule()=0;
		
		scheduler_base() : sched_flags(NO_INIT) {if (scheduler()==nullptr) scheduler(this);};
		};
	
	
	
	
	
	///ѕланировщик вытесн€ющего планировани€. Ѕазовый класс.
	class subst_scheduler_base : public scheduler_base
	{public:
		//Ресурсы:	
		
		//Утилиты:
		void block_sched();				//Внутренняя блокировка планирования
		void unblock_sched();			
		
		//Конструктор
		subst_scheduler_base() {bits_set(sched_flags, USER_BLOCK);}; 
	};	
	
	
	
#endif