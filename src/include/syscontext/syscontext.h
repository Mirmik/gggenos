#ifndef  GENOS_SYSCONTEXT_H
	#define GENOS_SYSCONTEXT_H
	
	#include "genos/iostream/iostream.h"
	#include "genos/defs.h"
	
	//Структура переменных и объектов окружения.
	struct syscontext
	{
		//Потоки ввода-вывода
		#ifdef SYSCONTEXT_IOSTREAM
		genos::ostream __cout;
		genos::istream __cin;
		#endif
		
		

		//Extended return.
		//Может использоваться для создания аналога исключений.
		int extreturn = 0;
	};
	
	//"Мягкая" блокировка вытеснения.
	//Переменная системная, но не структурная:
	//нет смысла её копировать, ибо она как раз копирование запрещает.
	extern bool __subst;
	
	extern syscontext current_syscontext ;
	
	#define cout 	current_syscontext.__cout
	#define cin 	current_syscontext.__cin
	#define cerr 	current_syscontext.__cerr
	
	#define extret 	current_syscontext.extreturn
	
	#define CAN_SUBST 	__subst
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
#endif