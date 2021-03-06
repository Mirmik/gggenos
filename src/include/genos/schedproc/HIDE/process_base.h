#ifndef GENOS_BASE_PROC
	#define GENOS_BASE_PROC
	
	#include "genos/types.h"
	#include "genos/subst/thread_constructor.h"
	#include "genos/base_struct/list.h"
	#include "genos/base_struct/bits.h"
	
	extern atomic_t process_operation;
		
	static constexpr uint8_t PROC_ZOMBIE 	=	0x01;
	static constexpr uint8_t PROC_RUNNING 	=	0x02;
	static constexpr uint8_t PROC_WAIT 		=	0x03;
	static constexpr uint8_t PROC_STOP 		=	0x04;
	
	static constexpr uint8_t PROC_STATE_MASK = 	0x0F;
	
	static constexpr uint8_t PROC_EXECUTED=	0x10;
	
	extern list_head process_list;
	
	class process_base{
		public:
		uint8_t prc_flags;				//Какие-то возможные состояния отмечаются в флагах.
		list_head prc_list;				//Используем связные списки в стиле Линукс.	
		process_base() : prc_flags(0), sts_list() {list_add(&prc_list, &process_list);}; 
	};
	
	
	class process_base_with_thread : public process_base , public thread_constructor{
		};
	
	
	void process_set_running(process_base* proc);
	void process_set_wait(process_base* proc);
	void process_set_zombie(process_base* proc);
	void process_set_stop(process_base* proc);
	void process_init(process_base* proc);
	
	void process_executed(process_base* proc);
	void process_unexecuted(process_base* proc);
		
	void process_unbind(process_base* proc);
	
	
#endif