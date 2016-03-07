

#include "genos/wait/waitserv.h"
#include "genos/datastruct/bits.h"
//#include "genos/debug/debug_info.h"


list_head wait_list;

bool wait_check(wait* wt){
	switch(bits_mask(wt->trait, wt::WAITTYPE))
	{
		case wt::FLAG8:
		return (*htreat<uint8_t*>(wt->info_cond) != 0);		
		break;
		
		#ifdef IOSTREAM_MODE
			case wt::INPUT_STREAM:
			return (htreat<stream*>(wt->info_cond)->available() != 0);		
			break;
		#endif
		
		#ifdef DEBUG_MODE
			default: debug_panic("waitserv, check"); break;
		#endif
	};
	
	
	
};

void wait_exec(wait* wt){
	switch(bits_mask(wt->trait, wt::TASKTYPE))
	{
		case wt::FUNC_VOID:
		htreat< void(*)() >(wt->info_task)();		
		break;
		
		#ifdef DELEGATE_WT
			case wt::DELEGATE_VOID:
			htreat<waitserv_d<void>>(wt->info_task)();		
			break;
		#endif
		
		#ifdef PROCESS_MODE
			case wt::PROC_WAIT:
			htreat< gstd::pair <schedee_base*, scheduler_base*>>(wt->info_task).second
			-> schedee_set_running
			(htreat< gstd::pair <schedee_base*, scheduler_base*>>(wt->info_task).first); 		
			break;
		#endif
		
		#ifdef DEBUG_MODE
			default: debug_panic("waitserv, exec"); break;
		#endif
	};	
};

void destroy_wait(wait* wt){
	list_del(&wt->wt_list); delete(wt);
};

void waitserv_check()
{	
	arch_atomic_temp(temp);
	wait* wt, *tmp;
	list_for_each_entry_safe(wt, tmp, &wait_list, wt_list)
	{
		if (wait_check(wt)) {wait_exec(wt); destroy_wait(wt);};
	};
	arch_deatomic_temp(temp);
};	




#ifdef PROCESS_MODE
	void wait_autom(uint8_t* flag){
		arch_atomic_temp(temp);
		scheduler()->schedee_set_wait(current_schedee()); 
		wait_create_procwait(current_schedee(), scheduler(), flag, wt::FLAG8);
		arch_deatomic_temp(temp);
	};
	
	void wait_subst(uint8_t* flag){
		arch_atomic_temp(temp);
		scheduler()->schedee_set_wait(current_schedee()); 
		wait_create_procwait(current_schedee(), scheduler(), flag, wt::FLAG8);
		scheduler()->schedule();
		arch_deatomic_temp(temp);
	};
	#ifdef IOSTREAM_MODE
		/*void wait_autom(Reader<char>* flag){
			arch_atomic_temp(temp);
			scheduler()->schedee_set_wait(current_schedee()); 
			wait_create_procwait(current_schedee(), scheduler(), flag, wt::INPUT_STREAM);
			arch_deatomic_temp(temp);
		};
		
		void wait_subst(Reader<char>* flag){
			arch_atomic_temp(temp);
			scheduler()->schedee_set_wait(current_schedee()); 
			wait_create_procwait(current_schedee(), scheduler(), flag, wt::INPUT_STREAM);
			scheduler()->schedule();
			arch_deatomic_temp(temp);
		};*/

		void wait_autom(stream* strm)
		{
			arch_atomic_temp(temp);
			scheduler()->schedee_set_wait(current_schedee()); 
			wait_create_procwait(current_schedee(), scheduler(), strm, wt::INPUT_STREAM);
			arch_deatomic_temp(temp);
		};
	#endif
	#else
	void wait_autom(uint8_t* flag){};
	void wait_subst(uint8_t* flag){};
	#ifdef IOSTREAM_MODE
		void wait_autom(genos::Reader<char>* flag){};
		void wait_subst(genos::Reader<char>* flag){};
	#endif
#endif







