
#include "syscontext/syscontext.h"
#include "genos/schedproc/switchScheduler.h"
	

delegate<void>* volatile starter_delegate; 
	
		void switchScheduler::init(){bits_clr(sched_flags, NO_INIT);};					
		
		extern context* schedule_context;
		void switchScheduler::reschedule()
		{
			context_switch(current_context_get(), schedule_context);
		};

		void switchScheduler::schedule(){	//вызов планировщика 
			switchScheduler::process_switch* proc;

			while (!dlist_is_empty(&zombie_list))
				{
//				debug_print("delete_zombie!");
				proc = 
				dlist_entry(zombie_list.next, switchScheduler::process_switch, lst);
				dlist_del(&proc->lst);
				delete proc;
				};

			_start:

			if (!ready()) debug_panic("subst_shed not ready");
			
			if(dlist_is_empty(&running_list)) return; 
			
			proc = 
			dlist_entry(running_list.next, switchScheduler::process_switch, lst);
			dlist_move_prev(&proc->lst, &running_list);
		
			current_schedee(proc);		
			
			if (bits_mask(proc->status, STATEMASK) == INIT)
			{
				starter_delegate = &proc->dlg;
				bits_mask_assign(proc->status, RUN, STATEMASK);
			}

			bits_set(proc->status, EXECUTE);
			context* tempcntxt = current_context_get();
			current_context_set(&proc->cntxt);	
			context_switch(tempcntxt, &proc->cntxt);
			current_context_set(schedule_context);
			bits_clr(proc->status, EXECUTE);

			
			//debug_print("schedule exit");

			return;
		};
		
		void switchScheduler::schedee_set_running(schedee* sch)
		{
			switchScheduler::process_switch* proc = reinterpret_cast<switchScheduler::process_switch*>(sch);
			bits_mask_assign(proc->status, RUN, STATEMASK);
			dlist_move_prev(&proc->lst, &running_list);
		};
		
		void switchScheduler::schedee_set_wait(schedee* sch)
		{
			switchScheduler::process_switch* proc = reinterpret_cast<switchScheduler::process_switch*>(sch);
			bits_mask_assign(proc->status, WAIT, STATEMASK);
			dlist_move_prev(&proc->lst, &waiting_list);
		};
		
		void switchScheduler::schedee_set_wait_child(schedee* sch)
		{
			switchScheduler::process_switch* proc = reinterpret_cast<switchScheduler::process_switch*>(sch);
			bits_mask_assign(proc->status, WAITCHILD, STATEMASK);
			dlist_move_prev(&proc->lst, &child_wait_list);
		};

		void switchScheduler::schedee_set_zombie(schedee* sch)
		{
//			debug_print("set_zombie");
//			debug_printhex_int32((int32_t)sch);
			switchScheduler::process_switch* proc = reinterpret_cast<switchScheduler::process_switch*>(sch);
			bits_mask_assign(proc->status, ZOMBIE, STATEMASK);
			//dpr("z:");dprln((int32_t)dlist_count(&zombie_list));
			//dpr("r:");dprln((int32_t)dlist_count(&running_list));
			dlist_move_prev(&proc->lst, &zombie_list);
			//dpr("z:");dprln((int32_t)dlist_count(&zombie_list));
			//dpr("r:");dprln((int32_t)dlist_count(&running_list));
			//dlist_del(&proc->lst);
			//delete proc;
		};
		
		void switchScheduler::schedee_unwait(schedee* sch)
		{
			switchScheduler::process_switch* proc = reinterpret_cast<switchScheduler::process_switch*>(sch);
			//bits_mask_assign(proc->status, RUN, STATEMASK);
			//dlist_move_prev(&proc->lst, &running_list);
			switchScheduler::process_switch* pos;
			dlist_for_each_entry(pos, &waiting_list, lst)
			{
				if (pos == proc) 
				{
					schedee_set_running(sch);
					return;
				}
			};
			return;
		};


		void switchScheduler::schedee_set_stop(schedee* sch)
		{
			debug_panic("B");
		};
		
		//void switchScheduler::child_end(schedee*)
		//{
		//}; 

		void switchScheduler::schedee_exit(schedee* sch)
		{
/*			process_switch* ptr;
			process_switch* parent;

			int result = 0;
			switchScheduler::process_switch* proc = reinterpret_cast<switchScheduler::process_switch*>(sch);
			if (proc->parent != 0)
				{
					parent = reinterpret_cast<switchScheduler::process_switch*>(proc->parent);
					if (bits_mask(parent->status, STATEMASK) == WAITCHILD) schedee_set_running(parent);
				};
			dlist_del(&proc->lst);
			delete proc;*/
			schedee_set_zombie(sch);

		};

		void switchScheduler::schedee_init(switchScheduler::process_switch* proc)
		{
			proc->parent = current_schedee();
			schedee_set_running(proc);
			bits_mask_assign(proc->status, INIT, STATEMASK);
		};

		switchScheduler::process_switch* switchScheduler::registry(void(*f)())
		{
			process_switch* proc = new process_switch;			
			proc->dlg = f;
			context_init(&proc->cntxt, 0, starter, proc->stack.top());	
			schedee_init(proc);
			return proc;
		};
/*
		switchScheduler::process_switch* switchScheduler::registry(const delegate<void>& d)
		{
			process_switch* proc = new process_switch;			
			schedee_init(proc);
			return proc;
		};
		
		switchScheduler::process_switch* switchScheduler::registry(delegate<void>&& d)
		{
			process_switch* proc = new process_switch;			
			proc->dlg = d;
			schedee_init(proc);
			return proc;
		};
*/		
		//Утилиты:
		//void proc_delete(process* proc);//Уничтожить процесс. (Активный процесс уничтожать запрещено.)
		
		//Конструктор
		switchScheduler::switchScheduler() : running_list(), waiting_list() { init(); };
	
	
void starter()
{
//	debug_print("starter");
	(*starter_delegate)();
	debug_panic("process_end!!! switchScheduler error.");
};

