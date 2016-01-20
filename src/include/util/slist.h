
#ifndef GENOS_GMONOLIST_BASIC
	#define GENOS_GMONOLIST_BASIC
	
	#include "util/member.h"



		class slist_head{
		
			public:
			slist_head* next;
		
			__if_cplusplus(
				slist_head() : next(this) {};
			)
			__if_cplusplus(
				slist_head(slist_head* _next) : next(_next) {};
			) 
		};
	
	
	
	
	void static inline slist_add(slist_head *_new, slist_head *head)
	{
		_new->next = head->next; 
		head->next = _new;
	}
	
	
	
	//Расчитывает структуру, содержащую элемент списка ptr.
	#define slist_entry(ptr, type, member) \
	container_of(ptr, type, member)
	
	//Расчитывает первую структуру, содержащуюся в списке ptr,
	//или структуру, содержащую элемент списка следующий за ptr.
	//лист не должен быть пустым.
	#define slist_first_entry(ptr, type, member) \
	slist_entry((ptr)->next, type, member)
	
	//Расчитывает следующую структуру.
	#define slist_next_entry(pos, member) \
	slist_entry((pos)->member.next, typeof(*(pos)), member)
	
		
	//Цикл, выполняющийся по всем структурам списка.
	#define slist_for_each_entry(pos, head, member)				\
	for (pos = slist_first_entry(head, typeof(*pos), member);	\
	&pos->member != (head);					\
	pos = slist_next_entry(pos, member))
	
	//Расчитывает последнюю структуру, содержащуюся в списке ptr,
	//лист не должен быть пустым.
	#define slist_last_entry(pos, head, member) \
	for (pos = slist_first_entry(head, typeof(*pos), member); \
	pos->member.next != (head);					\
	pos = slist_next_entry(pos, member))
	
	
	//Цикл, выполняющийся по всем элементам списка.
	//Безопасен при использовании операций удаления.
	#define slist_for_each_safe(p, pos, n, head) \
	p = head;					\
	for (pos = (head)->next, n = pos->next; pos != (head); \
	pos = n, n = pos->next)
	//Стоек к удалению.
	
	#define __list_del(p, n)\
	p->next = n
	
#endif