
#include "genos/time/timeserv.h"
#include "genos/gstl/hassign.h"

	
	
	list_head timer_list;
	
	void put_timer_to_timerlist(timer* tsr) 
	{
		list_head* it;
		list_for_each(it, &timer_list)
		{
			if (
			((time_t)list_entry(it, timer, tmr_list)->tmr.finish() - 
			(time_t)tsr->tmr.finish() > 0) 
			|| 
			it==&timer_list
			)
			break;
		};
		bits_clr(tsr->trait, tmr::DONE);
		list_move_tail(&tsr->tmr_list, &list_entry(it, timer, tmr_list)->tmr_list);
	};
	
	
	timer* timer_create(time_t t, uint8_t state)
	{
		timer* tsr = new timer;
		tsr -> trait = tmr::NULLER;
		bits_set(tsr->trait, state);
		tsr->tmr.set(get_time(),t);
		put_timer_to_timerlist(tsr);
		return tsr;
	};
	
	
	
	timer* timer_create(void(*ptr)(void), time_t t, uint8_t state)
	{
		timer* tsr = new timer;
		tsr -> trait = tmr::FUNC_VOID;
		hassign(tsr->info, ptr);
		bits_set(tsr->trait, state);
		tsr->tmr.set(get_time(),t);
		put_timer_to_timerlist(tsr);
		return tsr;
	};
	
	#ifdef DELEGATE_MODE
		timer* timer_create(tmrserv_d<void> d, time_t t, uint8_t state)
		{
			timer* tsr = new timer;
			tsr -> trait = tmr::DELEGATE_VOID;
			hassign(tsr->info, d);
			bits_set(tsr->trait, state);
			tsr->tmr.set(get_time(),t);
			put_timer_to_timerlist(tsr);
			return tsr;
		};
		
		timer* timer_create(tmrserv_d<void, int> d, int param, time_t t, uint8_t state)
		{
			timer* tsr = new timer;
			tsr -> trait = tmr::DELEGATE_INT;
			hassign(tsr->info, gstd::make_pair(d, param));
			bits_set(tsr->trait, state);
			tsr->tmr.set(get_time(),t);
			put_timer_to_timerlist(tsr);
			return tsr;
		};
		
		
		timer* timer_create(tmrserv_d<void, void*> d, void* param, time_t t, uint8_t state)
		{
			timer* tsr = new timer;
			tsr -> trait = tmr::DELEGATE_PTR;
			hassign(tsr->info, gstd::make_pair(d, param)); 
			bits_set(tsr->trait, state);
			tsr->tmr.set(get_time(),t);
			put_timer_to_timerlist(tsr);
			return tsr;
		};
	#endif //DELEGATE_MODE
	
	#ifdef PROCESS_MODE
		timer* timer_create_proc_wait
		(schedee_base* _proc, scheduler_base* _sched, time_t t, uint8_t strategy)
		{
			timer* tsr = new timer;
			tsr -> trait = tmr::PROC_WAIT;
			hassign(tsr->info, gstd::make_pair(_proc, _sched));
			bits_set(tsr->trait, strategy);
			tsr->tmr.set(get_time(),t);
			put_timer_to_timerlist(tsr);
			return tsr;
		};
		
		
		timer* timer_set_proc_wait(timer* tsr, schedee_base* _proc, scheduler_base* _sched, uint8_t strategy)
		{
			tsr -> trait = tmr::PROC_WAIT;
			hassign(tsr->info, gstd::make_pair(_proc, _sched));
			bits_set(tsr->trait, strategy);
			return tsr;
		};
	#endif
	
	
	void __delete_timer(timer* tsr)
	{
		if (bits_mask_internal_or(tsr->trait, tmr::DELETEINFO))
		delete htreat_bias<void*, 6>(tsr->info); //Отсчитываем размер делегата
		//И удаляем данные
		delete tsr;			
	};
	
	void delete_timer(timer* tsr)
	{
		__list_del_entry(&tsr->tmr_list);
		__delete_timer(tsr);
	};
	
	void timerserv_check()
	{	
		//dpr_hex(get_time());
		arch_atomic_temp(temp);
		if (list_empty(&timer_list)) {arch_deatomic_temp(temp); return;};
		timer* tsr = list_first_entry(&timer_list, timer, tmr_list);
		if (! tsr->tmr.check(get_time())) {arch_deatomic_temp(temp); return;};
		timer_exec(tsr);
		
		auto strategy = bits_and(tsr->trait, tmr::STRATEGY);
		switch (strategy) 
		{
			case tmr::ROUND:
			tsr->tmr.reset();
			put_timer_to_timerlist(tsr);
			break;
			
			case tmr::ONCE:
			__list_del_entry(&tsr->tmr_list);
			__delete_timer(tsr);
			break;
			
			case tmr::MANUAL:
			bits_set(tsr->trait, tmr::DONE);
			list_del_init(&tsr->tmr_list);
			break;
			
			#ifdef DEBUG_MODE
			default: debug_panic("timer_list.1");
			#endif
		};
		arch_deatomic_temp(temp);
	};	
	
	void timer_exec(timer* tsr)
	{		
		++tsr->count;
		auto state = bits_and(tsr->trait, tmr::TMRTYPE);
		switch (state)
		{
			case tmr::NULLER: 
			#ifdef DEBUG_MODE
			debug_print("timer_nuller");
			#endif
			break;
			
			case tmr::FUNC_VOID: 
			htreat< void(*)() >(tsr->info)();
			break; 
			
			#ifdef DELEGATE_MODE
				case tmr::DELEGATE_VOID: 
				htreat< tmrserv_d<void> >(tsr->info)();
				break; 	
				
				case tmr::DELEGATE_INT: 
				htreat< gstd::pair <tmrserv_d<void, int>, int>>(tsr->info).first
				(htreat< gstd::pair <tmrserv_d<void, int>, int>>(tsr->info).second); 
				break; 
				
				case tmr::DELEGATE_PTR: 
				htreat< gstd::pair <tmrserv_d<void, void*>, void*>>(tsr->info).first
				(htreat< gstd::pair <tmrserv_d<void, void*>, void*>>(tsr->info).second);
				break; 
			#endif
			
			#ifdef PROCESS_MODE
				case tmr::PROC_WAIT: 
				htreat< gstd::pair <schedee_base*, scheduler_base*>>(tsr->info).second
				-> schedee_set_running
				(htreat< gstd::pair <schedee_base*, scheduler_base*>>(tsr->info).first); 
				break; 
			#endif
		};
	};
	
	
	
	void timer_set(timer* tsr, time_t t)
	{
		tsr->tmr.set(get_time (), t);	
		put_timer_to_timerlist(tsr);
	};
	
	void timer_set_bias(timer* tsr, time_t t)
	{
		tsr->tmr.set_bias(t);	
		put_timer_to_timerlist(tsr);
	};
	
	void timer_trait(timer* tsr, uint8_t state)
	{
		tsr->trait = state;
	};
	
	uint8_t timer_trait(timer* tsr)
	{
		return tsr->trait;
	};
	
	bool timer_is_done(timer* tsr){
		return bits_mask(tsr->trait, tmr::DONE);
	};
	
	
	
	
	
	#ifdef PROCESS_MODE
		timer* msleep_subst(long int a) 
		{
			arch_atomic_temp(temp);
			scheduler()->schedee_set_wait(current_schedee()); 
			timer* tmr = timer_create_proc_wait(current_schedee(), scheduler(), a, tmr::ONCE); 
			scheduler()->schedule();
			arch_deatomic_temp(temp);
			return tmr;
		}
		
		timer* msleep_subst(timer* t, long int a) 
		{
			arch_atomic_temp(temp);
			scheduler()->schedee_set_wait(current_schedee()); 
			timer* tmr = timer_set_proc_wait(t, current_schedee(), scheduler(), tmr::MANUAL);
			timer_set(t, a);
			scheduler()->schedule();
			arch_deatomic_temp(temp);
			return tmr;
		}
		
		timer* msleep_subst_bias(timer* t, long int a) 
		{
			arch_atomic_temp(temp);
			scheduler()->schedee_set_wait(current_schedee()); 
			timer* tmr = timer_set_proc_wait(t, current_schedee(), scheduler(), tmr::MANUAL);
			timer_set_bias(t, a);
			scheduler()->schedule();
			arch_deatomic_temp(temp);
			return tmr;
		};
		
		timer* msleep_autom(long int a) 
		{ 
			scheduler()->schedee_set_wait(current_schedee()); 
			return	timer_create_proc_wait(current_schedee(), scheduler(), a, tmr::ONCE); 
		}

		timer* msleep_autom_bias(timer* t, long int a) 
		{ 
			scheduler()->schedee_set_wait(current_schedee()); 
			timer_set_proc_wait(t, current_schedee(), scheduler(), tmr::MANUAL);
			timer_set_bias(t, a);
			return t;	 
		}


		#else
		timer* msleep_subst(long int a){};
		timer* msleep_subst(timer* t, long int a){};
		timer* msleep_subst_bias(timer* t, long int a){};
		timer* msleep_autom(long int a){};
	#endif
