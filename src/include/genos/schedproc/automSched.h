#ifndef GENOS_AUTOM_SCHED
	#define GENOS_AUTOM_SCHED
	
	//#include <genos/stack/dynamic_stack.h>
	#include <genos/datastruct/list.h>
	
	#include "genos/schedproc/scheduler_base.h"
	#include "genos/schedproc/schedee_base.h"
	#include "genos/sigslot/delegate2.h"
	#include "genos/schedproc/state_lists.h"
	
	class automScheduler : public scheduler_base
	{
		template<typename R, typename ... V>
		using delegate = cdelegate<R,V...>;
		
		//Структура, содержащая информацию о процессе.
		class process_autom : public schedee_sts_list, public schedee_base
		{		
			public:
			delegate<void> dlg;
		};
			
		//Ресурсы:
		uint8_t proc_is_unbind;
		state_lists<process_autom> stlst; 
		
		public:
	
		list_head running_list;
		list_head waiting_list;
	
		//Методы:
		void init(){bits_clr(sched_flags, NO_INIT);};					
		
		
		void proc_go_wait()
		{
			proc_is_unbind = 1;
		};
	
		void schedule(){	//вызов планировщика 
			process_autom* proc;
			
			if(list_empty(&stlst.running_list)) return; 
			proc = 
			list_entry(stlst.running_list.next, process_autom, sts_list);
			list_move_tail(&proc->sts_list, &stlst.running_list);
			current_schedee(proc);			
			proc->dlg();
			
			return;
		};
		
		
		
		void schedee_set_running(schedee_base* proc){stlst.schedee_set_running((process_autom*)proc);};
		void schedee_set_wait(schedee_base* proc){stlst.schedee_set_wait((process_autom*)proc);};
		void schedee_set_zombie(schedee_base* proc){stlst.schedee_set_zombie((process_autom*)proc);};
		void schedee_set_stop(schedee_base* proc){stlst.schedee_set_stop((process_autom*)proc);};
		
		
		
		void schedee_init(process_autom* proc)
		{
			stlst.schedee_set_running(proc);
		};
		
		
		
		/*void registry(void(*_fnc)(void))
		{
			process_autom* proc = new process_autom;
			proc->dlg.set( _fnc );
			process_init(proc);
		};*/
		
		
		
		void registry(const delegate<void>& d)
		{
			process_autom* proc = new process_autom;			
			proc->dlg = d;
			schedee_init(proc);
		};
		
		void registry(delegate<void>&& d)
		{
			process_autom* proc = new process_autom;			
			proc->dlg = d;
			schedee_init(proc);
		};
		
		
		//Утилиты:
		//void proc_delete(process* proc);//Уничтожить процесс. (Активный процесс уничтожать запрещено.)
		
		//Конструктор
		automScheduler() : proc_is_unbind(0) {init();};
		};
	
	
	
#endif				