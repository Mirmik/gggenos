
#include "syscontext/syscontext.h"
#include "genos/schedproc/automSched.h"
	
	
		void automScheduler::init(){bits_clr(sched_flags, NO_INIT);};					
		
		void automScheduler::schedule(){	//вызов планировщика 
			if (!ready()) return;
			automScheduler::process_autom* proc;

			if(dlist_empty(&running_list)) return; 
			
			proc = 
			dlist_entry(running_list.next, automScheduler::process_autom, lst);
			dlist_move_tail(&proc->lst, &running_list);
		
			current_schedee(proc);		
			//stdout.printhex((uint16_t)proc);

			bits_set(proc->status, EXECUTE);	
			proc->dlg();
			bits_clr(proc->status, EXECUTE);
			
			return;
		};
		
		void automScheduler::schedee_set_running(schedee* sch)
		{
			automScheduler::process_autom* proc = reinterpret_cast<automScheduler::process_autom*>(sch);
			bits_mask_assign(proc->status, RUN, STATEMASK);
			dlist_move_tail(&proc->lst, &running_list);
		};
		
		void automScheduler::schedee_set_wait(schedee* sch)
		{
			automScheduler::process_autom* proc = reinterpret_cast<automScheduler::process_autom*>(sch);
			bits_mask_assign(proc->status, WAIT, STATEMASK);
			dlist_move_tail(&proc->lst, &waiting_list);
		};
		
		void automScheduler::schedee_set_wait_child(schedee* sch)
		{
			automScheduler::process_autom* proc = reinterpret_cast<automScheduler::process_autom*>(sch);
			bits_mask_assign(proc->status, WAITCHILD, STATEMASK);
			dlist_move_tail(&proc->lst, &child_wait_list);
		};

		void automScheduler::schedee_set_zombie(schedee* sch)
		{
			error_stub();
		};
		
		void automScheduler::schedee_set_stop(schedee* sch)
		{
			error_stub();
		};
		
		//void automScheduler::child_end(schedee*)
		//{
		//}; 

		void automScheduler::schedee_exit(schedee* sch)
		{
			process_autom* ptr;
			process_autom* parent;

			int result = 0;
			automScheduler::process_autom* proc = reinterpret_cast<automScheduler::process_autom*>(sch);
			if (proc->parent != 0)
				{
					parent = reinterpret_cast<automScheduler::process_autom*>(proc->parent);
					if (bits_mask(parent->status, STATEMASK) == WAITCHILD) schedee_set_running(parent);
				};
			dlist_del(&proc->lst);
			delete proc;
		};

		void automScheduler::schedee_init(automScheduler::process_autom* proc)
		{
			proc->parent = current_schedee();
			schedee_set_running(proc);
		};

		automScheduler::process_autom* automScheduler::registry(void(*f)())
		{
			delegate<void> d(f);
			process_autom* proc = new process_autom;			
			proc->dlg = d;
			schedee_init(proc);
			return proc;
		};

		automScheduler::process_autom* automScheduler::registry(const delegate<void>& d)
		{
			process_autom* proc = new process_autom;			
			proc->dlg = d;
			schedee_init(proc);
			return proc;
		};
		
		automScheduler::process_autom* automScheduler::registry(delegate<void>&& d)
		{
			process_autom* proc = new process_autom;			
			proc->dlg = d;
			schedee_init(proc);
			return proc;
		};
		
		//Утилиты:
		//void proc_delete(process* proc);//Уничтожить процесс. (Активный процесс уничтожать запрещено.)
		
		//Конструктор
		automScheduler::automScheduler() : running_list(), waiting_list() { init(); };
	
	
	