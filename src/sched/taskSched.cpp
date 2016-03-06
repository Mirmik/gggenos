
#include "genos/schedproc/taskSched.h"

void taskScheduler::schedule()
{	 
	taskScheduler::task* proc;

	if(list_empty(&running_list)) return; 
	proc = list_entry(running_list.next, taskScheduler::task, lst);
	list_move_tail(&proc->lst, &running_list);
	proc->func();		
	return;
};

void taskScheduler::add(void(*newTask)(void))
{
	task* proc = new task;
	proc->func = newTask;
	list_add(&proc->lst, &running_list);
};

taskScheduler taskSched;