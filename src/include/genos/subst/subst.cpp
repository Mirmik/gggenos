
//2012-2015 Mirmik 
//���������� ������� ���������� ��������� ����������.
//�������������� �� AVR2560

//#pragma GCC optimize "-Og"
#include "genos/debug/debug_print.h"
#include "genos/subst/subst.h"
#include "avr/interrupt.h"

extern "C" {

//������� ����������:
thread* __current;			//��������� �� �������� ����.	

//���������� ����������:
size_t sp_temp;						//� ��� ���������� �������� ����������� ��������� �����.
thread* __operated;		//��������� �� ���� ������������ ������� ����������� ��������.

//������ � ��������� �������� ����.
thread* current_thread() {return __current;};

#define current_thread_dump(){ \
	PUSH_PROCESSOR_STATE_TO_STACK();\
	current_thread() -> stack_pointer = readSP();\
	debug_print("stub_byte:\n");\
	debug_print_dump((void*)current_thread()->stack_pointer, 1);	\
	debug_print("processor_state:\n");\
	debug_print_dump((void*)current_thread()->stack_pointer + 1, PROC_STATE_SIZE);	\
	debug_print("stack_state:\n");\
	debug_print_dump((void*)current_thread()->stack_pointer + PROC_STATE_SIZE + 1, current_thread()->stack_top - current_thread()->stack_pointer - 1 - PROC_STATE_SIZE);	\
	POP_PROCESSOR_STATE_FROM_STACK();	\
};\

/**
 *	������� �������������
 */


//init_current_thread
//������������ ��� ���������� ������� ���� � �������� �����.
//@Warn: ��� ������������� �������� ���� ������� ���������� ��������� ��������
//� ����� ��������.
//@thr ������ ����, ����������� � �������.
//@stcktop ������� ����� ������� ����.
void init_current_thread(thread* thr, size_t stcktop)
{	
	__current = thr;					
	set_stack_thread(thr, stcktop);		
};


//set_stack_thread
//������� � ���� ������� ���� ���������� �� ������������ ����� �����.
//���� ��������� ������.
//@thr ������ ����.
//@stcktop ������� ����� ���� ����.
void set_stack_thread(thread* thr, size_t stcktop)
{
	thr -> stack_top      = stcktop;
	thr -> stack_pointer  = stcktop;
};


/**
 *	���������� ���������.
 */

//change_thread 
//�������� ���������. ��������� ��������� ����� ���� � ��������� ��������� ������.
//@operated ��������� �� ������ ��������������� ����.
//@Warn ������ noinline ��� �������� ����������� ����� ��������.
void change_thread (thread* operated)__attribute__((noinline));	
void change_thread (thread* operated)
{				
	__operated = operated;
	PUSH_PROCESSOR_STATE_TO_STACK();		
	__current -> stack_pointer = readSP();	
	__current = __operated;					
	setSP(__current -> stack_pointer);		
	POP_PROCESSOR_STATE_FROM_STACK();	
};

/**
 *	����������� ��������� ����� ���������.
 */

//copy_thread
//��������� �������� ���� ���� dst � ���� ���� src.
//����������� ����� ��������� ����� ���� ��������.
//@Warn: ������ ���� noinline ��� ��������� ������.
//@dst ���� �������
//@src ���� ��������
//@copy_count ���������� ���������� ������.
void copy_thread(thread* dst, thread* src)__attribute__((noinline));
void copy_thread(thread* dst, thread* src)
{		
	uint8_t* begin = 	(uint8_t*)src->stack_top + 1 ;
	uint8_t* end =	 	(uint8_t*)src->stack_pointer;
	uint8_t* dst_ptr = 	(uint8_t*)dst->stack_top + 1;
	uint8_t* src_ptr = 	begin;
	
	for(; src_ptr != end ;) *--dst_ptr = *--src_ptr;
	dst->stack_pointer = (size_t)dst_ptr;
	
	//debug_print_dump((void*)dst->stack_pointer, dst->stack_top - dst->stack_pointer);
	//debug_print_dump((void*)src->stack_pointer, src->stack_top - src->stack_pointer);
	//while(1);	
};

//copy_thread_part
//�������� ������ ����� ����� ���� dst � ���� ���� src.
//����������� ����� ��������� ����� ���� ��������.
//@Warn: ������ ���� noinline ��� ��������� ������.
//@dst ���� �������
//@src ���� ��������
//@copy_count ���������� ���������� ������.
void copy_thread_part(thread* dst, thread* src, size_t copy_count)__attribute__((noinline));
void copy_thread_part(thread* dst, thread* src, size_t copy_count)
{		
	uint8_t* begin = 	(uint8_t*)src->stack_pointer + copy_count + 1;
	uint8_t* end =	 	(uint8_t*)src->stack_pointer;
	uint8_t* dst_ptr = 	(uint8_t*)dst->stack_top + 1;
	uint8_t* src_ptr = 	begin;
	
	for(; src_ptr != end ;) *--dst_ptr = *--src_ptr;
	dst->stack_pointer = (size_t)dst_ptr;	
};



/**
 *	���������� ���������.
 */

//clone_thread
//�������� ������ ����� �������� ���� � ������� ������ ����.
//@Warn: �������� ��������, ��� ��������� � ��������� �������
//clone_thread ������ ��������� ��������� � ��������� ������� substitution
//��� ��������� ������� ����� �������� �� �������. 
//@Warn:C����� noinline ��� �������� ����������� ����� ��������.
//@Warn:sp_temp ��������� ����������, ��� ��� ����� ���������� 
//PUSH_PROCESSOR_STATE_TO_STACK ������ � ���������� ��������� �.�. �����������.
//@dst ��������� �� ������ ��������������� ����.
void clone_thread(thread* dst)__attribute__((noinline));	
void clone_thread(thread* dst)
{	
	sp_temp = readSP();	
	__operated = dst;
	
	PUSH_PROCESSOR_STATE_TO_STACK();
	__current -> stack_pointer = readSP();
	copy_thread(__operated, __current);
	setSP(sp_temp);		
};

//fork_thread
//�������� ���������. ������ ��� clone_thread, ����������� ��������� ���� �����.
//������ ���� ���� � ���������� ������ �������� � ����������� �� ����, 
//��� �������� ���������.
//@dst ��������� �� ������ ���� �������.
//@return 1 ��� ������. 0 ��� �������.
int fork_thread(thread* dst)
{	
	thread* parent = __current;				//���������� ���������� �������� � ��������� ���������.
	clone_thread(dst);			
	return (__current == parent ? 1 : 0);	//���������� ������� �������� ���� � ������������ ��������.
};

};

/**
 *	���������� ��������������� ���������.
 */

//clone_thread_noret
//�������������� ������ clone_thread. 
//@Warn: �������� ��������, ��� ��������� � ��������� �������
//clone_thread ������ ��������� ��������� � ��������� ������� substitution
//��� ��������� ������� ����� �������� �� �������. 
//@Warn:C����� noinline ��� �������� ����������� ����� ��������.
//@Warn:sp_temp ��������� ����������, ��� ��� ����� ���������� 
//PUSH_PROCESSOR_STATE_TO_STACK ������ � ���������� ��������� �.�. �����������.
//@dst ��������� �� ������ ��������������� ����.
//@THREAD_NORET_COPYSIZE ��. subst.h
void clone_thread_noret(thread* dst)__attribute__((noinline));	
void clone_thread_noret(thread* dst)
{	
	__operated=dst;
	sp_temp = readSP();
	
	PUSH_PROCESSOR_STATE_TO_STACK();
	__current -> stack_pointer = readSP();
	copy_thread_part(__operated, __current, THREAD_NORET_COPYSIZE);
	setSP(sp_temp);					
};

	
	//�������� ������� ����������� �������� ����.
	//������ � ������, ������������ ������� ��������� ��������.
	//���������� ���� ��� �������, ������� ��� ������
	int exec_thread(thread* dst, void(* func)())
	{	
		thread* parent = __current;				//���������� ���������� ��������.			
		clone_thread_noret(dst);					    //��������� ����������� �������� ����.
		if (__current == parent) return 0;	//���������� ������� �������� ���� � ������������ ��������.
		else long_goto(func);
	};


int exec_current_thread(void(* func)())
{	
	drop_stack();
	long_goto(func);
};

int exec_thread_safe(thread* dst, void(* func)())
	{	
		thread* parent = __current;				//���������� ���������� ��������.			
		clone_thread_noret(dst);					    //��������� ����������� �������� ����.
		if (__current == parent) return 0;	//���������� ������� �������� ���� � ������������ ��������.
		func();
		debug_print("exec_thread_safe_return");
		cli(); while(1);
	};


int exec_current_thread_safe(void(* func)())
{	
	drop_stack();
	func();
	debug_print("exec_thread_safe_return");
	cli(); while(1);
};

