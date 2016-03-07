

#include "genos/schedproc/schedee_base.h"
#include "genos/schedproc/scheduler_base.h"

list_head schedee_list;

static schedee_base* _current_schedee;

schedee_base* current_schedee(){return _current_schedee;};
void current_schedee(schedee_base* proc){_current_schedee = proc;};

scheduler_base* _current_scheduler;

scheduler_base* scheduler(){return _current_scheduler;};
	void scheduler(scheduler_base* _sched){_current_scheduler = _sched;};
	
	
	void debug_proclist_print()
	{
		list_head* it;
	list_for_each(it,&schedee_list) { debug_printhex_ptr(it); debug_putchar('\n'); };
		
	};