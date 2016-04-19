#ifndef GENOS_SWITCH_SCHED
#define GENOS_SWITCH_SCHED
	
#include <genos/container/dlist_head.h>
#include "genos/schedproc/scheduler_base.h"
#include "genos/schedproc/schedee_base.h"
#include "genos/sigslot/delegate.h"
#include "util/stub.h"	
#include "kernel/context.h"
#include "genos/stack/static_stack.h"

#define STATIC_STACK_SIZE 500


void starter();

	class switchScheduler : public scheduler
	{
	
		static constexpr uint8_t INIT = 0x00;	
		static constexpr uint8_t RUN = 0x01;
		static constexpr uint8_t WAIT = 0x02;
		static constexpr uint8_t WAITCHILD = 0x03;
		static constexpr uint8_t ZOMBIE = 0x04;
		static constexpr uint8_t STATEMASK = 0x0F;		
		static constexpr uint8_t EXECUTE = 0x80;

	public:
		//Структура, содержащая информацию о процессе.
		struct process_switch : public schedee
		{		
			static_stack<STATIC_STACK_SIZE> stack;
			delegate<void> dlg;
			context cntxt;
			dlist_head lst;
			uint8_t status;
		};		
	
	public:
	
		dlist_head running_list;
		dlist_head waiting_list;
		dlist_head zombie_list;
		dlist_head child_wait_list;
	
		//Методы:
		void init();				
		
		void schedule();		
		void schedee_set_running(schedee* sch);
		void schedee_set_wait(schedee* sch);
		void schedee_set_wait_child(schedee* sch);
		void schedee_set_zombie(schedee* sch);
		void schedee_set_stop(schedee* sch);
		void schedee_exit(schedee* sch);
		void schedee_unwait(schedee* sch);
		void schedee_init(process_switch* proc);
	
//		void child_end(schedee*);

		process_switch* registry(void(*f)());
		//process_switch* registry(const delegate<void>& d);
		//process_switch* registry(delegate<void>&& d);


		template<typename T>
		process_switch* registry(T* a, void (T::*ptr)())
		{
			process_switch* proc = new process_switch;			
			proc->dlg = delegate_mtd(a,ptr);
			context_init(&proc->cntxt, 0, starter, proc->stack.top());	
			schedee_init(proc);
			return proc;
		};
		
		//Утилиты:
		//void proc_delete(process* proc);//Уничтожить процесс. (Активный процесс уничтожать запрещено.)
		
		//Конструктор
		switchScheduler();

	};	


#endif				