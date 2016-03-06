#ifndef GENOS_STATES_LISTS_H
	#define GENOS_STATES_LISTS_H
	
	
	
	#include "genos/datastruct/list.h"
	#include <genos/datastruct/bits.h>
	
	template <typename processType>
	class state_lists{
		public:
		list_head process_list;
		list_head running_list;
		list_head waiting_list;
		list_head zombie_list;
		list_head stop_list;
		
		
		void process_set_running(processType* proc)
		{
			list_move(&proc->sts_list, &running_list);
			bits_mask_assign(proc->prc_flags, PROC_STATE_MASK, PROC_RUNNING);
		};	
		
		void process_set_wait(processType* proc)
		{
			list_move(&proc->sts_list, &waiting_list);
			bits_mask_assign(proc->prc_flags, PROC_STATE_MASK, PROC_WAIT);
		};
		
		void process_set_zombie(processType* proc)
		{
			list_move(&proc->sts_list, &zombie_list);
			bits_mask_assign(proc->prc_flags, PROC_STATE_MASK, PROC_ZOMBIE);
		};
		
		void process_set_stop(processType* proc)
		{
			list_move(&proc->sts_list, &stop_list);
			bits_mask_assign(proc->prc_flags, PROC_STATE_MASK, PROC_STOP);
		};
		
		
		void process_unbind(processType* proc)
		{
			list_del(&proc->sts_list);
			list_del(&proc->prc_list);	
		};
		
		
		void move_tail_running_list(processType* proc){
			list_move_tail(&proc->sts_list,&running_list);
		};
		
				
		
		void debug_all_list_print()
		{
			debug_process_list_print();
			debug_running_list_print();
			debug_waiting_list_print();
			debug_zombie_list_print();
			debug_stop_list_print();
		};
		
		void debug_process_list_print()
		{debug_print("process_list");debug_putchar('\n');
			processType* proc;
			list_for_each_entry(proc,&process_list,prc_list)
			{
				debug_printhex_ptr(proc);debug_putchar(':');debug_printhex_uint8(proc->prc_flags);debug_putchar('\n');
			};
		};
		void debug_running_list_print()
		{debug_print("running_list");debug_putchar('\n');
			processType* proc;
			list_for_each_entry(proc,&running_list,prc_list)
			{
				debug_printhex_ptr(proc);debug_putchar('\n');
			};
		};
		void debug_waiting_list_print()
		{debug_print("waiting_list");debug_putchar('\n');
			processType* proc;
			list_for_each_entry(proc,&waiting_list,prc_list)
			{
				debug_printhex_ptr(proc);debug_putchar('\n');
			};
		};
		void debug_zombie_list_print()
		{debug_print("zombie_list");debug_putchar('\n');
			processType* proc;
			list_for_each_entry(proc,&zombie_list,prc_list)
			{
				debug_printhex_ptr(proc);debug_putchar('\n');
			};
		};
		void debug_stop_list_print()
		{debug_print("stop_list");debug_putchar('\n');
			processType* proc;
			list_for_each_entry(proc,&stop_list,prc_list)
			{
				debug_printhex_ptr(proc);debug_putchar('\n');
			};
		};
		
	};
	
#endif