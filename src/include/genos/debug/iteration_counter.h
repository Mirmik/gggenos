#ifndef ITERATION_COUNTER_H
	#define ITERATION_COUNTER_H
	//Макросы счетного исполнения. by Mirmik
	
	#include "genos/debug/helper_macro.h"
	#include "genos/debug/debug_macro.h"
	
	
	
	//Выполнить однократно.
	#define do_once \
    debug_place(); static int _var_generate(0) = 1; \
    if (_var_generate(0)) if (_var_generate(0)--) 
	
	//Выполнять b итераций.
	#define do_iteration(b) \
    debug_place(); static int _var_generate(0) = 0; \
    if (_var_generate(0) < b) if (++_var_generate(0)) 
	
	//Выполнять после b итераций (невключительно).
	#define do_iteration_after(b) \
	debug_place(); static int _var_generate(0) = 0; \
	if (_var_generate(0) < b) ++_var_generate(0); else
	
	//Выполнять от a итерации до b итерации (a включительно, b невключительно).
	#define do_iteration_between(a, b) \
	debug_place(); static int _var_generate(0) = 0; \
	if (_var_generate(0) < a) ++_var_generate(0); else \
	if (_var_generate(0) < b) if (++_var_generate(0))
	
	//Выполнять после only_once, do_iteration, do_between_iteration
	#define do_after else;else
	
	/*
	//Структура счётчик инструкций.
	struct iteration_counter
	{
		long long iteration;
		long long helper;
	};
	
	#define iter_start(obj) \
	static iteration_counter obj = {0, 0}; \
	obj.iteration++;\
	switch(obj.helper){case 0:
	
	//
	#define iter(obj,a) if (obj.iteration == a) obj.helper = a; break; case a:
	
	//Завершение процессора.
	#define iter_end(obj,a) if (obj.iteration == a) obj.helper = a; break; default: break; }
	
	//Неостанавливающие завершение.
	#define iter_end_unstoped(obj) if (obj.iteration >= obj.helper) obj.iteration = obj.helper; break;}
	
	//Формат использования.
	//iter_start(obj)     do_something0();	//выполнять до второй итерации 
	//iter(obj,2)         do_something1();	//выполнять со второй до четвертой итерации
	//iter(obj,4)         do_something2();	//выполнять с четвертой по десятую итерации
	//iter_end(obj,10); 
	
	*/
	
#endif