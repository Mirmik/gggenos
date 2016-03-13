#ifndef GENOS_AUTOM_SCHED
#define GENOS_AUTOM_SCHED
	
#include <genos/datastruct/list.h>
#include "genos/schedproc/scheduler_base.h"
#include "genos/schedproc/schedee_base.h"
#include "genos/sigslot/delegate.h"
#include "util/stub.h"	
	
	class automScheduler : public scheduler
	{
		
		static constexpr uint8_t RUN = 0x01;
		static constexpr uint8_t WAIT = 0x02;
		static constexpr uint8_t STATEMASK = 0x0F;		
		static constexpr uint8_t EXECUTE = 0x80;

		//Структура, содержащая информацию о процессе.
		struct process_autom : public schedee
		{		
			list_head lst;
			delegate<void> dlg;
			uint8_t status;
		};		
	
	public:
	
		list_head running_list;
		list_head waiting_list;
	
		//Методы:
		void init();				
		
		void schedule();		
		void schedee_set_running(schedee* sch);
		void schedee_set_wait(schedee* sch);
		void schedee_set_zombie(schedee* sch);
		void schedee_set_stop(schedee* sch);
		void schedee_exit(schedee* sch);
		void schedee_init(process_autom* proc);
		
		process_autom* registry(void(*f)());
		process_autom* registry(const delegate<void>& d);
		process_autom* registry(delegate<void>&& d);

		
		//Утилиты:
		//void proc_delete(process* proc);//Уничтожить процесс. (Активный процесс уничтожать запрещено.)
		
		//Конструктор
		automScheduler();



		template<typename T>
		process_autom* registry(T* a, void (T::*ptr)())
		{
			delegate<void> d(a, ptr);
			process_autom* proc = new process_autom;			
			proc->dlg = d;
			schedee_init(proc);
			return proc;
		};


		};	

#endif				