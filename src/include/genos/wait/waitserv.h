#ifndef GENOS_WAITSERV
	#define GENOS_WAITSERV
	
	#include "genos/defs.h"	
	
	#include "sys/types.h"
	#include "genos/datastruct/list.h"
	#include "genos/gstl/hassign.h"
	//#include "genos/debug/debug.h"
	#include "hal/arch.h"
	#include "genos/gstl/utility.h"
	//#include "genos/syscontext/syscontext.h"
	
	
	#ifdef DELEGATE_WT
		#include "genos/sigslot/delegate2.h"		
		template<typename T1, typename ... T2>
		using waitserv_d = cdelegate<T1,T2...>;
	#endif
	
	#ifdef PROCESS_MODE
		#include "genos/schedproc/schedee_base.h"
		#include "genos/schedproc/scheduler_base.h"
	#endif
	
	#ifdef IOSTREAM_MODE
		#include "genos/iostream/iostream.h"
	#endif
	
	namespace wt
	{
		static constexpr uint8_t FLAG8 = 0x01;	
		static constexpr uint8_t INPUT_STREAM = 0x02;	
		static constexpr uint8_t WAITTYPE = 0x0F;
		
		static constexpr uint8_t DELEGATE_VOID = 0x10;
		static constexpr uint8_t FUNC_VOID = 0x20;
		static constexpr uint8_t PROC_WAIT = 0x30;
		
		
		static constexpr uint8_t TASKTYPE = 0xF0;
	};
	
	
	
	
	
	class wait{
		public:
		list_head wt_list;
		uint8_t trait = 0;
		///////////////TODO///////////////
		char info_task[4 * sizeof(size_t)];
		char info_cond[2 * sizeof(int)];
		//////////////////////////////////
	};
	
	
	
	
	//Список ожиданий.
	extern list_head wait_list;
	
	
	void waitserv_check();
	
	template<typename T1, typename T2>
	void wait_create(T1* flag, T2&& d, uint8_t _trait)
	{
		arch_atomic_temp(temp);
		wait* wt = new wait;
		wt->trait = _trait;
		hassign(wt->info_task, d);
		hassign(wt->info_cond, flag);
		list_add(&wt->wt_list, &wait_list);
		arch_deatomic_temp(temp);
	};
	
	#ifdef PROCESS_MODE
		template<typename T1>
		void wait_create_procwait(schedee_base* _proc, scheduler_base* _sched, T1* flag, uint8_t _trait)
		{
			arch_atomic_temp(temp);
			wait* wt = new wait;
			wt->trait = _trait | wt::PROC_WAIT;
			hassign(wt->info_task, gstd::make_pair(_proc, _sched));
			hassign(wt->info_cond, flag);
			list_add(&wt->wt_list, &wait_list);
			arch_deatomic_temp(temp);
		};	
	#endif
	
	
	void wait_autom(uint8_t* flag);
	void wait_subst(uint8_t* flag);
	
	#include "genos/io/stream.h"
	#ifdef IOSTREAM_MODE
	//void wait_autom(Reader<char>* flag);
	//void wait_subst(Reader<char>* flag);
	void wait_autom(stream* strm);
	#endif
	
	
	
	
#endif
