#ifndef GENOS_AUTOM_SCHED
	#define GENOS_AUTOM_SCHED
	
	#include <genos/datastruct/list.h>
	
	class taskScheduler
	{ 
	public:

		class task
		{		
			public:
			void(*func)();
			list_head lst;
		};
	
		list_head running_list;

		void add(void(*newTask)(void));
		void schedule();
		
	};
	
	extern taskScheduler taskSched;
	
#endif				