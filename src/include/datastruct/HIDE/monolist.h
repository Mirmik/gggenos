
#ifndef GENOS_GMONOLIST_BASIC
	#define GENOS_GMONOLIST_BASIC
	
	#include "genos/base_struct/base_struct_def.h"



		class monolist_head{
		
		public:
		monolist_head* next;
		
		monolist_head() : next(this) {};
		monolist_head(monolist_head* _next) : next(_next) {}; 
		};
	
	
	
	
	void static inline monolist_add(monolist_head *_new, monolist_head *head)
	{
		_new->next = head->next; 
		head->next = _new;
	}
	
	
	
	//Расчитывает структуру, содержащую элемент списка ptr.
	#define monolist_entry(ptr, type, member) \
	container_of(ptr, type, member)
	
	//Расчитывает первую структуру, содержащуюся в списке ptr,
	//или структуру, содержащую элемент списка следующий за ptr.
	//лист не должен быть пустым.
	#define monolist_first_entry(ptr, type, member) \
	monolist_entry((ptr)->next, type, member)
	
	//Расчитывает следующую структуру.
	#define monolist_next_entry(pos, member) \
	monolist_entry((pos)->member.next, typeof(*(pos)), member)
	
		
	//Цикл, выполняющийся по всем структурам списка.
	#define monolist_for_each_entry(pos, head, member)				\
	for (pos = monolist_first_entry(head, typeof(*pos), member);	\
	&pos->member != (head);					\
	pos = monolist_next_entry(pos, member))
	
	//Расчитывает последнюю структуру, содержащуюся в списке ptr,
	//лист не должен быть пустым.
	#define monolist_last_entry(pos, head, member) \
	for (pos = monolist_first_entry(head, typeof(*pos), member); \
	pos->member.next != (head);					\
	pos = monolist_next_entry(pos, member))
	
	
	//Цикл, выполняющийся по всем элементам списка.
	//Безопасен при использовании операций удаления.
	#define monolist_for_each_safe(p, pos, n, head) \
	p = head;					\
	for (pos = (head)->next, n = pos->next; pos != (head); \
	pos = n, n = pos->next)
	//Стоек к удалению.
	
	#define __list_del(p, n)\
	p->next = n
	
#endif