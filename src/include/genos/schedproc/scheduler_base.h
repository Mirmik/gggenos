#ifndef GENOS_SCHEDULER_BASE
	#define GENOS_SCHEDULER_BASE
	
	#include <genos/debug/debug.h>
	#include <genos/datastruct/list.h>
//	#include <genos/subst/subst.h>
	#include <genos/schedproc/schedee_base.h>
	
	//#include "genos/subst/thread_constructor.h"
	
	
	
	static constexpr uint8_t USER_BLOCK	=	0x01;
	static constexpr uint8_t SCHED_BLOCK=	0x02;
	static constexpr uint8_t NO_INIT	=	0x04;
	static constexpr uint8_t BLOCK_MASK	=	0x0F;
	
	
	schedee_base* current_schedee();
	void current_schedee(schedee_base*);
	
	class scheduler_base;
		
	scheduler_base* scheduler();
	void scheduler(scheduler_base* _sched);
	
	///Џланировщик. Ѓазовый класс.
	class scheduler_base
	{public:
		uint8_t sched_flags;					//ђесурсы планировщика
		
				
		void block();					//‚нешнЯЯ блокировка планирования
		void unblock();				
		
		//Њетоды:
		bool ready();					//‚озвращает готовность к планированию.
										//Џланировщик не готов, если установлен 
										//Ћдин из битов в BLOCK_MASK либо 
										//не обнулена переменная process_operation
		
		virtual void schedee_set_running(schedee_base* proc)=0;
		virtual void schedee_set_wait(schedee_base* proc)=0;
		virtual void schedee_set_zombie(schedee_base* proc)=0;
		virtual void schedee_set_stop(schedee_base* proc)=0;
		
		//TODO
		//virtual void change_process_state(process_base* process,uint8_t state){};
	
			//void process_executed(process_base* proc);
			//void process_unexecuted(process_base* proc);
		
			void schedee_unbind(schedee_base* proc);
		
		//Ћбобщенный интерфейсы для работы внешних сервисов, типа waiter
		virtual void schedule()=0;
		
		scheduler_base() : sched_flags(NO_INIT) {if (scheduler()==nullptr) scheduler(this);};
		};
	
	
	
	
	
	///Планировщик вытесняющего планирования. Базовый класс.
	class subst_scheduler_base : public scheduler_base
	{public:
		//ђесурсы:	
		
		//“тилиты:
		void block_sched();				//‚нутреннЯЯ блокировка планированиЯ
		void unblock_sched();			
		
		//Љонструктор
		subst_scheduler_base() {bits_set(sched_flags, USER_BLOCK);}; 
	};	
	
	
	
#endif