#ifndef GENOS_TIMER_SERV
	#define GENOS_TIMER_SERV
	
	#include "genos/time/basic_timer.h"
	#include "genos/schedproc/wait.h"
	#include "genos/gstl/container/uncreated_list.h"
	#include "genos/debug/debug.h"
	#include "genos/base_struct/bits.h"
	
	namespace genos {
		
		static constexpr uint8_t NULLER = 0x00;
		static constexpr uint8_t PRCWAIT = 0x01;
		static constexpr uint8_t WAITER = 0x02;
		
		static constexpr uint8_t ROUND = 0x10;
		
		
		
		class timerserv_record
		{public:	 
			basic_timer tmr;
			uint8_t trait;
			//process_base* proc;
			uint64_t info;
		};
		
		
		////////////TODO///////////////
		typedef int32_t signed_time_t;
		static inline auto get_time () {return millis();}; 
		/////////////////////////////////
		
		
		
		class timer_service_base
		{public:
			static timer_service_base* __tmrserv;
			timer_service_base() {if (__tmrserv!=nullptr) debug_panic("Second TimerService");
			__tmrserv=this;	};
			
			virtual void set_timer_pause(time_t t)=0;
			virtual void check()=0;
			
		friend timer_service_base& tmrserv();
		friend void tmrserv(timer_service_base& _tmrserv);
		};
		
		timer_service_base& tmrserv();
		void tmrserv(timer_service_base& _tmrserv);
		
		
		
		
		
		
		
		
		
		
		class timer_service : public timer_service_base{
			public:
			uncreated_list<timerserv_record> lst;
			
			
			void exec(timerserv_record* tsr)
			{
				if (tsr->trait == PRCWAIT) 
				{ATOMIC(temp);
				process_base** arch = (process_base**) &(tsr->info);
				process_set_running(*arch);
				DEATOMIC(temp);
				};
				
				if (tsr->trait == WAITER) 
				{
				waiter_basic** arch = (waiter_basic**) &(tsr->info);
				(*arch)->unwait();				
				};
				
				if (tsr->trait == NULLER) dpr("timer"); 			
			};
			
			
			void set_timer_pause(time_t t)
			{
				timerserv_record* tsr = new timerserv_record;
				tsr -> trait = PRCWAIT;
				tsr->tmr.set(get_time(),t);
				process_base* proc = current_process(); 
				process_base** arch = (process_base**) &(tsr->info);
				*arch = proc;
				
				auto it = lst.begin();
				auto end = lst.end();
				ATOMIC(temp);
				scheduler().proc_go_wait();
				process_set_wait(proc);
				DEATOMIC(temp);
				
				while(((signed_time_t)it->tmr.finish() - (signed_time_t)tsr->tmr.finish() < 0) && it!=end) ++it;
				lst.insert_before(*tsr, it);
		};
			
			void set_timer(time_t t)
			{
				timerserv_record* tsr = new timerserv_record;
				tsr -> trait = NULLER;
				tsr->tmr.set(get_time(),t);
				//*((waiter_basic**)(&tsr->info)) = wtr;
				
				auto it = lst.begin();
				auto end = lst.end();
				while(((signed_time_t)it->tmr.finish() - (signed_time_t)tsr->tmr.finish() < 0) && it!=end) ++it;
				lst.insert_before(*tsr, it);
			};
			
			void check()
			{
				if (lst.empty()) return;
				auto it = lst.begin();
				if (! it->tmr.check(get_time())) return;
				exec(&*it);
				
			/*	if (bits_mask_internal_or(it->trait, ROUND)) 
				{
					//lst.unbind(it); lst.insert_()
				}
					else
			{*/
						timerserv_record* tsr = &*it;
						delete tsr;
						lst.unlist(it);	
			/*		};
							
			*/};
					
				};
				
			};
			
			#define msleep(a) {tmrserv().set_timer_pause(a); scheduler().schedule();}
			
		#endif		