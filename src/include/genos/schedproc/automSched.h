#ifndef GENOS_AUTOM_SCHED
#define GENOS_AUTOM_SCHED
	
#include <genos/container/dlist_head.h>
#include "genos/schedproc/scheduler_base.h"
#include "genos/schedproc/schedee_base.h"
#include "genos/sigslot/delegate.h"
#include "util/stub.h"	
	
	class automScheduler : public scheduler
	{
		
		static constexpr uint8_t RUN = 0x01;
		static constexpr uint8_t WAIT = 0x02;
		static constexpr uint8_t WAITCHILD = 0x03;
		static constexpr uint8_t STATEMASK = 0x0F;		
		static constexpr uint8_t EXECUTE = 0x80;

		//Структура, содержащая информацию о процессе.
		struct process_autom : public schedee
		{		
			dlist_head lst;
			delegate<void> dlg;
			uint8_t status;
		};		
	
	public:
	
		dlist_head running_list;
		dlist_head waiting_list;
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
		void schedee_init(process_autom* proc);
	
		void child_end(schedee*);

		process_autom* registry(void(*f)());
		process_autom* registry(const delegate<void>& d);
		process_autom* registry(delegate<void>&& d);

		template<typename T>
		process_autom* registry(T* a, void (T::*ptr)())
		{
			delegate<void> d(a, ptr);
			process_autom* proc = new process_autom;			
			proc->dlg = d;
			schedee_init(proc);
			return proc;
		};
		
		//Утилиты:
		//void proc_delete(process* proc);//Уничтожить процесс. (Активный процесс уничтожать запрещено.)
		
		//Конструктор
		automScheduler();





		};	

#endif				