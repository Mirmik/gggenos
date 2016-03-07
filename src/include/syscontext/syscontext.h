#ifndef  GENOS_SYSCONTEXT_H
#define GENOS_SYSCONTEXT_H
	
#include "genos/io/stream.h"

	//Структура переменных и объектов окружения.
	struct syscontext
	{
		direct_stream __stdout;
		direct_stream __stdin;
	};
	
	//"Мягкая" блокировка вытеснения.
	//Переменная системная, но не структурная:
	//нет смысла её копировать, ибо она как раз копирование запрещает.
	//extern bool __subst;
	
	syscontext* current_syscontext();
	void current_syscontext(syscontext*);
	
	#define stdout 	(current_syscontext()->__stdout)
	#define stdin 	(current_syscontext()->__stdin)
	
#endif