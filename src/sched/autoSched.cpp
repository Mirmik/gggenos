
	
#include "genos/schedproc/automSched.h"
	
	
		void automScheduler::init(){bits_clr(sched_flags, NO_INIT);};					
		
		void automScheduler::schedule(){	//вызов планировщика 

			if (!ready()) return;

			process_autom* proc;

			if(dlist_empty(&running_list)) return; 
			proc = 
			dlist_entry(running_list.next, process_autom, lst);
			dlist_move_tail(&proc->lst, &running_list);
		
			current_schedee(proc);		

			bits_set(proc->status, EXECUTE);	
			proc->dlg();
			bits_clr(proc->status, EXECUTE);
			
			return;
		};
		
		void automScheduler::schedee_set_running(schedee* sch)
		{
			process_autom* proc = static_cast<process_autom*>(sch);
			if (bits_mask(proc->status, RUN)) 
			{
				bits_mask_assign(proc->status, RUN, STATEMASK);
				dlist_add_tail(&proc->lst, &running_list);
			};
		};
		
		void automScheduler::schedee_set_wait(schedee* sch)
		{
			process_autom* proc = static_cast<process_autom*>(sch);
			bits_mask_assign(proc->status, WAIT, STATEMASK);
			dlist_add_tail(&proc->lst, &waiting_list);
		};
		
		void automScheduler::schedee_set_zombie(schedee* sch)
		{
			error_stub();
		};
		
		void automScheduler::schedee_set_stop(schedee* sch)
		{
			error_stub();
		};
		
		void automScheduler::schedee_init(process_autom* proc)
		{
			schedee_set_running(proc);
		};
		
		process_autom* automScheduler::registry(void(*f)())
		{
			delegate<void> d(f);
			process_autom* proc = new process_autom;			
			proc->dlg = d;
			schedee_init(proc);
			return proc;
		};
/*
		process_autom* registry(const delegate<void>& d)
		{
			process_autom* proc = new process_autom;			
			proc->dlg = d;
			schedee_init(proc);
			return proc;
		};
		
		process_autom* registry(delegate<void>&& d)
		{
			process_autom* proc = new process_autom;			
			proc->dlg = d;
			schedee_init(proc);
			return proc;
		};
*/		
		
		//Утилиты:
		//void proc_delete(process* proc);//Уничтожить процесс. (Активный процесс уничтожать запрещено.)
		
		//Конструктор
		automScheduler::automScheduler() { init(); };
	
	
	