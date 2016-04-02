
//2012-2015 Mirmik 
//Реализация функций вытеснения контекста процессора.
//Протестировано на AVR2560

//#pragma GCC optimize "-Og"
#include "genos/debug/debug_print.h"
#include "genos/subst/subst.h"
#include "avr/interrupt.h"

extern "C" {

//Внешние переменные:
thread* __current;			//Указатель на активную нить.	

//Внутренние переменные:
size_t sp_temp;						//В эту переменную временно сохраняется указатель стека.
thread* __operated;		//Указатель на нить относительно которой выполняется операция.

//Доступ к указателю активной нити.
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
 *	Функции инициализации
 */


//init_current_thread
//Используется для связывания объекта нити с активной нитью.
//@Warn: Без инициализации активной нити попытка вытеснения контекста приведет
//к краху сиистемы.
//@thr Объект нити, связываемой с текущей.
//@stcktop Вершина стека текущей нити.
void init_current_thread(thread* thr, size_t stcktop)
{	
	__current = thr;					
	set_stack_thread(thr, stcktop);		
};


//set_stack_thread
//Заносит в поля объекта нити информацию об используемой нитью стеке.
//стек считается пустым.
//@thr Объект нити.
//@stcktop Вершина стека этой нити.
void set_stack_thread(thread* thr, size_t stcktop)
{
	thr -> stack_top      = stcktop;
	thr -> stack_pointer  = stcktop;
};


/**
 *	Вытеснение контекста.
 */

//change_thread 
//Менятель контекста. Сохраняет состояние одной нити и загружает состояние другой.
//@operated Указатель на объект устанавливаемой нити.
//@Warn Строго noinline для создания подменяемой точки возврата.
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
 *	Копирование состояние нитей контекста.
 */

//copy_thread
//Полностью копирует стек нити dst в стек нити src.
//Расчитывает новый указатель стека нити приёмника.
//@Warn: Должен быть noinline для коректной работы.
//@dst Нить приёмник
//@src Нить источник
//@copy_count Количество копируемых байтов.
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
//Копирует нижнюю часть стека нити dst в стек нити src.
//Расчитывает новый указатель стека нити приёмника.
//@Warn: Должен быть noinline для коректной работы.
//@dst Нить приёмник
//@src Нить источник
//@copy_count Количество копируемых байтов.
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
 *	Реализация ветвлений.
 */

//clone_thread
//Создание полной копии активной нити в объекте другой нити.
//@Warn: Обратите внимания, что сигнатура и аттрибуты функции
//clone_thread должны повторять сигнатуру и аттрибуты функции substitution
//для коректной подмены точек возврата из функции. 
//@Warn:Cтрого noinline для создания подменяемой точки возврата.
//@Warn:sp_temp объявлена глобальной, так как после выполнения 
//PUSH_PROCESSOR_STATE_TO_STACK доступ к локальному контексту м.б. некоректным.
//@dst Указатель на объект устанавливаемой нити.
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
//Создание ветвления. Обёртка над clone_thread, позволяющая различить пару нитей.
//Создаёт клон нити и возвращает разные значения в зависимости от того, 
//кем является выходящий.
//@dst Указатель на объект нити потомка.
//@return 1 для предка. 0 для потомка.
int fork_thread(thread* dst)
{	
	thread* parent = __current;				//Запоминаем дескриптор родителя в локальном контексте.
	clone_thread(dst);			
	return (__current == parent ? 1 : 0);	//Сравниваем текущую активную нить с дескриптором родителя.
};

};

/**
 *	Реализация невозвращающего ветвления.
 */

//clone_thread_noret
//Невозвращающий аналог clone_thread. 
//@Warn: Обратите внимания, что сигнатура и аттрибуты функции
//clone_thread должны повторять сигнатуру и аттрибуты функции substitution
//для коректной подмены точек возврата из функции. 
//@Warn:Cтрого noinline для создания подменяемой точки возврата.
//@Warn:sp_temp объявлена глобальной, так как после выполнения 
//PUSH_PROCESSOR_STATE_TO_STACK доступ к локальному контексту м.б. некоректным.
//@dst Указатель на объект устанавливаемой нити.
//@THREAD_NORET_COPYSIZE см. subst.h
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

	
	//Оболочка функции копирования активной нити.
	//Пролог и эпилог, обеспечивают возврат различных значений.
	//Возвращает ноль для потомка, единицу для предка
	int exec_thread(thread* dst, void(* func)())
	{	
		thread* parent = __current;				//Запоминаем дескриптор родителя.			
		clone_thread_noret(dst);					    //Выполняем копирование активной нити.
		if (__current == parent) return 0;	//Сравниваем текущую активную нить с дескриптором родителя.
		else long_goto(func);
	};


int exec_current_thread(void(* func)())
{	
	drop_stack();
	long_goto(func);
};

int exec_thread_safe(thread* dst, void(* func)())
	{	
		thread* parent = __current;				//Запоминаем дескриптор родителя.			
		clone_thread_noret(dst);					    //Выполняем копирование активной нити.
		if (__current == parent) return 0;	//Сравниваем текущую активную нить с дескриптором родителя.
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

