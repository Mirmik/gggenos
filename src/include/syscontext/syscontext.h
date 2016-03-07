#ifndef  GENOS_SYSCONTEXT_H
#define GENOS_SYSCONTEXT_H
	
#include "genos/iostream/writer.h"
#include "genos/iostream/reader.h"

	//Структура переменных и объектов окружения.
	struct syscontext
	{
		Writer<char> __stdout;
		Reader<char> __stdin;
	};
	
	//"Мягкая" блокировка вытеснения.
	//Переменная системная, но не структурная:
	//нет смысла её копировать, ибо она как раз копирование запрещает.
	//extern bool __subst;
	
	extern syscontext* current_syscontext ;
	
	#define stdout 	current_syscontext.stdout
	#define stdin 	current_syscontext.stdin
	
#endif