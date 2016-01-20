#ifndef GENOS_SCHEDULER_1_H
	#define GENOS_SCHEDULER_1_H
	
	#include "genos/debug/debug.h"
	#include "genos/schedproc/scheduler_base.h"
	#include "genos/schedproc/process_base.h"
	#include "genos/memory.h"
	#include "genos/base_struct/bits.h"
	
	//TODO требуется переработка.
	
	//extern int dbgint;
	extern uint8_t brsee;
		static int bbbbn=0;
		
	static const uint8_t THR_NEED_CREATE_STARTER = 0x80; // thr_flags
	
	static const uint8_t FUNC_INIT = 0x00;
	static const uint8_t AUTOM = 0x01;
	static const uint8_t FUNC_VOID = 0x02;
	static const uint8_t FUNC_INT = 0x03;
	static const uint8_t FUNC_VOID_LOOP = 0x04;
	
	static const uint8_t NOT_NEED_RESCHEDULE = 0x00;
	static const uint8_t THREAD_EXIT = 0x01;
	static const uint8_t RESCHEDULE = 0x02;
	
	__attribute__((noinline)) volatile static void do_nothing(){};
	
	void maScheduler_idle();
	
	extern "C" void __starter();//__attribute__((noexit, naked));
	class maScheduler : public subst_scheduler_base
	{	
		public:
		class atom_process : public process_base
		{	
			public:
			void *execobj ;
			uint8_t prctype;	
			size_t param1;
			size_t param2;			
			int8_t priority;
			char* name;
		};
		
		
		
		
		class thread_manager : public thread_constructor{
			public:
			
			void starter()// __attribute__((noexit))
			{
				//debug_print("starter");
				strt:
				if (proc != 0) {
					//debug_printhex_ptr(proc->execobj);dln;
					//dstp;
					
					sei();
					
					switch (proc->prctype)
					{
						case AUTOM:  goto noexit;
						case FUNC_VOID: reinterpret_cast<void(*)()>(proc->execobj)(); goto exit;
						case FUNC_VOID_LOOP: reinterpret_cast<void(*)()>(proc->execobj)(); goto noexit;		
						case FUNC_INT: break;		
						case FUNC_INIT: break;		
						default: break;
					};
					exit:
					proc->prc_flags |= PROC_ZOMBIE;
					maScheduler::sched->need_reschedule=THREAD_EXIT;
					ready_to_restart = 1;
					goto sch;
					noexit:
					maScheduler::sched->need_reschedule=RESCHEDULE;
					move_tail_running_list(current_process());;
					goto sch;
					sch:
					cli(); maScheduler::sched->schedule(); 
					goto strt;
				}
				else debug_panic("maSched_starter was invoked with thread_without_proc");
				debug_panic("maSched_starter was ended.");
			};
			
			atom_process* proc;
			uint8_t ready_to_restart;
		};
		
		
		#define THREAD_NUM 4
		#define THREAD_STCK_SIZE 400
		thread_manager thr_array[THREAD_NUM];
		
		void init()
		{
			init_create_threads();
			create_func_loop_process(maScheduler_idle, 0, "idle");
			bits_clr(sched_flags, NO_INIT);
		};
		
		
		void proc_go_wait(){need_reschedule = RESCHEDULE;};
		
		atom_process* create_func_loop_process(void(* _func)(),uint8_t _priority, char* _name)
		{
			block_sched();
			atom_process* proc = new atom_process;
			proc->prctype = FUNC_VOID_LOOP;
			proc->priority = _priority;
			proc->execobj = (void*)_func;
			need_reschedule = RESCHEDULE;
			process_init(proc);
			proc->name = _name;
			
			//	debug_printbin_uint8(proc->prc_flags);dln;
			unblock_sched();
			return proc;
		};
		
		inline atom_process* create_init_process(uint8_t _priority , char* _name)
		{
			atom_process* proc = new atom_process;
			proc->prctype = FUNC_INIT;
			proc->priority = _priority;
			need_reschedule = RESCHEDULE;
			proc->execobj = 0;
			proc->name = _name;
			process_init(proc);
			bits_set(proc->prc_flags,PROC_EXECUTED);
			
			//	debug_printbin_uint8(proc->prc_flags);dln;
			return proc;
		};
		
		void init_create_threads()
		{	
			thr_array[0].construct_current_thread(RAMEND);
			thr_array[0].proc = create_init_process(10, "init");
			thr_array[0].ready_to_restart = 0;
			for(int i=1;i<THREAD_NUM;i++)
			{
				thr_array[i].construct_thread(THREAD_STCK_SIZE);
				bits_set(thr_array[i].thr_flags, THR_NEED_CREATE_STARTER);
				thr_array[i].proc = 0;
				thr_array[i].ready_to_restart = 1;
			};					
		};
		
		
		
		void set_proc_to_current_thread(atom_process* ap)
		{
			thr_array[current_thread].proc = ap;
		};
		
		
		
		void schedule(){
			int8_t high_process_priority = -1;
			int8_t high_thread_priority = -1;
			atom_process* ready_proc = 0;
			atom_process* prroc, *prrroc; //Вспомогательные указатели.
			
			
			//Возврат, если обработка не требуется
			if (need_reschedule == NOT_NEED_RESCHEDULE) {
				//debug_print("label6\n"); 
				return;};
				
			//Текущий процесс приостановлен.
			bits_clr(thr_array[current_thread].proc->prc_flags, PROC_EXECUTED);
			
			
			//Расчет старшей нити.
			while(thr_array[current_thread].ready_to_restart == 1 
			&& current_thread != -1) current_thread--;
			
			//Выявляем готовый к выполнению процесс с максимальным приоритетом
			list_for_each_entry_safe(prroc, prrroc, &running_list, sts_list) 
			{
				/*if (bits_mask_internal_or(prroc->prc_flags, PROC_ZOMBIE)) 
				{
					list_del(&prroc->prc_list);
					delete prroc;
					continue;
				};*/
				if (high_process_priority < prroc->priority)		
				if (bits_mask_eq(prroc->prc_flags, PROC_RUNNING, PROC_STATE_MASK))
				if (!bits_mask_internal_or(prroc->prc_flags, PROC_EXECUTED)) 
				{high_process_priority = prroc->priority; 
				ready_proc = prroc;};
			};
			
			//Вычисляем приоритет старшей нити
			if (current_thread != -1)  high_thread_priority = thr_array[current_thread].proc->priority;
			else high_thread_priority = -1;
			
			//Логика, записывающая задачу в процесс 
			
			//Если старшая нить приоритетней выбранного процесса, запускаем старшую нить
			if (high_thread_priority > high_process_priority) 
			{
				dprcond(brsee,"br1");
				goto rethread;
			};			
			
			
			//Если приоритет старшей нити равен приоритету выбранного процесса
			//проверяем, не выполняется ли он уже в этой нити,
			//и если это не так заменяем исполняемый процесс.
			if (high_thread_priority == high_process_priority) 
			if (ready_proc != thr_array[current_thread].proc)
			{
				dprcond(brsee,"br2");
				//debug_printhex_ptr(ready_proc);
				//debug_printhex_ptr(thr_array[current_thread].proc);
				set_proc_to_current_thread(ready_proc);
				goto rethread;
			}
			else //Если процесс уже исполняется ничего не делаем. Запускаем нить.
			{
				dprcond(brsee,"br3");
				goto rethread;
			};
			
			// Если выбранный процесс более приоритетен, запускаем его в новой нити.
			// если нитей слишком много, выдаём ошибку.
			if (high_thread_priority < high_process_priority)   
			{
				dprcond(brsee,"br4");
				current_thread++;
				if (current_thread == THREAD_NUM) {debug_panic("Need more threads! HALT.");};
				set_proc_to_current_thread(ready_proc);
				goto rethread;
			};
			
		
		rethread:		
		need_reschedule=NOT_NEED_RESCHEDULE;
		
		current_process(thr_array[current_thread].proc);
//debug_printhex_ptr(current_process());dln;
		
		bits_set(thr_array[current_thread].proc->prc_flags, PROC_EXECUTED);
		thr_array[current_thread].ready_to_restart = 0;
			
		if (::current_thread() == thr_array[current_thread].thr) 
		{
			if (bits_mask_internal_and(thr_array[current_thread].thr_flags, THR_NEED_CREATE_STARTER))
			{	
				dprcond(brsee,"branch1\n");
				bits_clr(thr_array[current_thread].thr_flags, THR_NEED_CREATE_STARTER);
				sei();
				//exec_current_thread(__starter);
				__starter();
			}	
			else
			{
				dprcond(brsee,"branch2\n");
				return;
			}
			
		}
		else 
		{
			if (bits_mask_internal_and(thr_array[current_thread].thr_flags, THR_NEED_CREATE_STARTER))
			{
				dprcond(brsee,"branch3\n");//dstp;
				bits_clr(thr_array[current_thread].thr_flags, THR_NEED_CREATE_STARTER);
				//sei();
				//if (!fork_thread(thr_array[current_thread].thr)) while(1) __starter;
				exec_thread(thr_array[current_thread].thr, __starter);
				sei();
				change_thread(thr_array[current_thread].thr);
			}
			else
			{
				dprcond(brsee,"branch4\n");dstp;
				sei();
				change_thread(thr_array[current_thread].thr);
			};
		};
	};
	
	void exit()
	{		
		thr_array[current_thread].proc->prc_flags |= PROC_ZOMBIE;
		thr_array[current_thread].ready_to_restart = 1;
		bits_set(thr_array[current_thread].thr_flags, THR_NEED_CREATE_STARTER);
		need_reschedule=THREAD_EXIT;
		cli();
		schedule();
	};
	
	static int8_t current_thread;
	uint8_t need_reschedule;
	maScheduler() : need_reschedule(0) {sched = this; current_thread = 0;};
	static maScheduler* sched; 

	
	
	
	
	
	
	
	
	
	
	
	};	





#endif