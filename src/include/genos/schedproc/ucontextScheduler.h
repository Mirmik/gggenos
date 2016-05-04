#ifndef GENOS_UCONTEXT_SCHEDULER
#define GENOS_UCONTEXT_SCHEDULER

#include "ucontext.h"

#include <genos/container/dlist.h>
#include "genos/schedproc/scheduler_base.h"
#include "genos/schedproc/schedee_base.h"
#include "genos/sigslot/delegate.h"
#include "util/stub.h"	

class uScheduler : public scheduler
{
	struct thread : public schedee
	{
		dlist_head lst;
		ucontext uc;
		void* mem_stack;
	};
public:
	dlist<thread, &thread::lst> running_list; 
	dlist<thread, &thread::lst> waiting_list; 
	dlist<thread, &thread::lst> zombie_list;

	ucontext scheduler_context;
	ucontext* current_context;

public:

	void init()
	{
		sched_flags = 0;
		getcontext(&scheduler_context);
		current_context = &scheduler_context;
	};

	void schedule()
	{
		thread* ptr;

		while (!zombie_list.is_empty())
		{
			ptr = &*zombie_list.begin();
			zombie_list.pop(*ptr);
			free(ptr->mem_stack);
			delete ptr;
		};

		_start:

		if (!ready()) debug_panic("subst_shed not ready");
		
		if(running_list.is_empty()) return; 
		
		ptr = &*running_list.begin();

		current_schedee(ptr);
		current_context = &ptr->uc;		
		
		//if ()
		//{
		//	starter_delegate = &proc->dlg;
		//	bits_mask_assign(proc->status, RUN, STATEMASK);
		//}
		running_list.round_left();

		ptr->state.execute();
		
		//dprln("here");
		//dprptrln(ptr->uc.uc_stack.ss_sp);

		//while(1);
		swapcontext(&scheduler_context, &ptr->uc);
		ptr->state.unexecute();
		
	
		current_context = &scheduler_context;		

		//dprln("schedule exit");

		return;
	};

	void reschedule()
	{
		swapcontext(current_context, &scheduler_context);
	};

	void schedee_set_running(schedee* proc)
	{
		thread* t = (thread*) proc;
		running_list.push_back(*t);
		t->state.run(); 		
	};
	
	void schedee_set_wait(schedee* proc)
	{
		thread* t = (thread*) proc;
		waiting_list.push_back(*t);
		t->state.wait();
	};
	
	void schedee_set_zombie(schedee* proc)
	{
		thread* t = (thread*) proc;
		zombie_list.push_back(*t);
		t->state.zombie();
	};
	
	void schedee_exit(schedee* proc)
	{
		schedee_set_zombie(proc);
	};
	
	void schedee_unwait(schedee* proc)
	{
		if (proc->state.is_wait())
		{
			thread* t = (thread*) proc;
			running_list.push_front(*t);
			t->state.run();
		}
	};
	
	schedee* registry(void (*func)())
	{
		thread* t = new thread;			
		getcontext(&t->uc);
		t->mem_stack = malloc(1024 * 64);
		t->uc.uc_stack.ss_sp = t->mem_stack;
		t->uc.uc_stack.ss_size = 1024 * 64; 
		t->uc.uc_stack.ss_flags = 0;
		makecontext(&t->uc, func, 0);	
		t->parent = current_schedee();
		schedee_set_running(t);
		return (schedee*) t;
	};

	schedee* registry(gstd::pair<AbstractDelegate*, void (AbstractDelegate::*)()> pr)
	{
		thread* t = new thread;			
		getcontext(&t->uc);
		t->uc.uc_stack.ss_sp = malloc(1024 * 64);
		t->uc.uc_stack.ss_size = 1024 * 64; 
		t->uc.uc_stack.ss_flags = 0;
		makecontext(&t->uc, reinterpret_cast<void(*)()>(pr.second), 1, pr.first);	
		t->parent = current_schedee();
		schedee_set_running(t);
		return (schedee*) t;
	};
};

#endif
