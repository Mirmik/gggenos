#ifndef GENOS_SCHEDULER_BASE
#define GENOS_SCHEDULER_BASE
	
#include <genos/container/dlist_head.h>
#include <genos/schedproc/schedee_base.h>
#include "util/bits.h"
#include "inttypes.h"
#include "genos/debug/debug.h"
		
static constexpr uint8_t USER_BLOCK			=	0x01;
static constexpr uint8_t SCHED_BLOCK		=	0x02;
static constexpr uint8_t NO_INIT			=	0x04;
static constexpr uint8_t PROCESS_OPERATION 	=	0x08;
static constexpr uint8_t BLOCK_MASK			=	0x0F;
	
//Текущая единица планирования.
schedee* current_schedee();
void current_schedee(schedee*);
	
class scheduler;
		
scheduler* current_scheduler();
void current_scheduler(scheduler* _sched);
	
///Планировщик. Базовый класс.
class scheduler
{public:
	uint8_t sched_flags;					//Ресурсы планировщика
			
	//Методы:
	void block() { sched_flags |= USER_BLOCK; };	//Внешняя блокировка планирования
	void unblock() { sched_flags &= ~USER_BLOCK; };
				
	bool ready() { return bits_mask(sched_flags, BLOCK_MASK) ? false : true; };	
									//Возвращает готовность к планированию.
									//Планировщик не готов, если установлен 
									//Один из битов в BLOCK_MASK либо 
									
	virtual void schedee_set_running(schedee* proc)=0;
	virtual void schedee_set_wait(schedee* proc)=0;
	virtual void schedee_set_wait_child(schedee* sch)=0;
	virtual void schedee_set_zombie(schedee* proc)=0;
	virtual void schedee_set_stop(schedee* proc)=0;
	virtual void schedee_exit(schedee* proc)=0;
	virtual void schedee_unwait(schedee* proc)=0;
	
	virtual void schedule()=0;
	
	scheduler() : sched_flags(NO_INIT) {
		assert(current_scheduler()==nullptr);
		current_scheduler(this);
	};
};
	

	/*
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
	*/
	
void exit_autom();
void exit_subst();
void schedule_subst();
void wait_child(schedee*);

#endif