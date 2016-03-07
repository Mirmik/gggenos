#ifndef GENOS_SCHEDEE_BASE
#define GENOS_SCHEDEE_BASE
	
	#include "sys/types.h"
	#include "genos/datastruct/list.h"
	#include "genos/datastruct/bits.h"
	
	extern atomic_t schedee_operation;
		
	static constexpr uint8_t SCHEDEE_ZOMBIE 	=	0x01;
	static constexpr uint8_t SCHEDEE_RUNNING 	=	0x02;
	static constexpr uint8_t SCHEDEE_WAIT 		=	0x03;
	static constexpr uint8_t SCHEDEE_STOP 		=	0x04;
	
	static constexpr uint8_t SCHEDEE_STATE_MASK = 	0x0F;
	
	static constexpr uint8_t SCHEDEE_EXECUTED=	0x10;
	
	extern list_head schedee_list;
	
	class schedee_base{
		public:
		uint8_t schedee_flags;				//Какие-то возможные состояния отмечаются в флагах.
		list_head schedee_list;				//Используем связные списки в стиле Линукс.	
		schedee_base() : schedee_flags(0) {list_add(&schedee_list, &schedee_list);}; 

	};
	
	
	class schedee_sts_list{
		public:
		list_head sts_list;
	};
	
	
	//class process_base_with_thread : public process_base , public thread_constructor{
	//	};
	
	
	void debug_schedeelist_print();
	
	
#endif