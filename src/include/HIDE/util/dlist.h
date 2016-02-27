#ifndef GENOS_LINUX_DLIST_BASIC
	#define GENOS_LINUX_DLIST_BASIC
	
	#include "util/member.h"
	
	#define DLIST_POISON1 ((struct dlist_head*)-1)
	#define DLIST_POISON2 ((struct dlist_head*)-2)
	
	
	
	struct dlist_head{		
		struct dlist_head* next;
		struct dlist_head* prev;
		
		__if_cplusplus(
			dlist_head() : next(this), prev(this) {};
		)
		__if_cplusplus(
			dlist_head(dlist_head* _next, dlist_head* _prev) : 
			next(_next), prev(_prev) {};
		) 
	};
	
	static inline struct dlist_head * INIT_DLIST_HEAD(struct dlist_head *list)
	{
		list->next = list;
		list->prev = list;
		return list;
	}
	
	#define DLIST_HEAD_INIT(name) { &(name), &(name) }
	
	#define DLIST_HEAD(name) \
	struct dlist_head name = DLIST_HEAD_INIT(name)

	#define dlist_init(list) INIT_DLIST_HEAD(list)
	#define dlist_head_init(list) INIT_DLIST_HEAD(list)
	
	/*
		* Simple doubly linked list implementation.
		*
		* Some of the internal functions ("__xxx") are useful when
		* manipulating whole lists rather than single entries, as
		* sometimes we already know the next/prev entries and we can
		* generate better code by using them directly rather than
		* using the generic single-entry routines.
	*/
	
	/*
		* Insert a _new entry between two known consecutive entries.
		*
		* This is only for internal list manipulation where we know
		* the prev/next entries already!
	*/
	//Добавляет элемент _new между двумя последовательно расположенными элементами.
	//Только для внутреннего использования, когда известны prev/next элементы. 
	static inline void __dlist_add(struct dlist_head *_new,
	struct dlist_head *prev,
	struct dlist_head *next)
	{
		next->prev = _new;
		_new->next = next;
		_new->prev = prev;
		prev->next = _new;
	}
	
	
	/**
		* dlist_add - add a _new entry
		* @_new: _new entry to be added
		* @head: list head to add it after
		*
		* Insert a _new entry after the specified head.
		* This is good for implementing stacks.
	*/
	//Добавить _new элемент после head.
	//Применяется для добавления в стэк.
	static inline void dlist_add(struct dlist_head *_new, struct dlist_head *head)
	{
		__dlist_add(_new, head, head->next);
	}
	#define dlist_add_next(_new, head) dlist_add(_new, head)
	
	
	/**
		* dlist_add_tail - add a _new entry
		* @_new: _new entry to be added
		* @head: list head to add it before
		*
		* Insert a _new entry before the specified head.
		* This is useful for implementing queues.
	*/
	//Добавить _new элемент перед head.
	//Применяется для добавления в очередь.
	static inline void dlist_add_tail(struct dlist_head *_new, struct dlist_head *head)
	{
		__dlist_add(_new, head->prev, head);
	}
	#define dlist_add_prev(_new, head) dlist_add_tail(_new, head)

	/*
		* Delete a list entry by making the prev/next entries
		* point to each other.
		*
		* This is only for internal list manipulation where we know
		* the prev/next entries already!
	*/
	//Удаляет элемент находящийся между prev/next элементами.
	//Только для внутреннего использования, когда известны prev/next элементы. 
	static inline void __dlist_del(struct dlist_head * prev, struct dlist_head * next)
	{
		next->prev = prev;
		prev->next = next;
	}
	
	/**
		* dlist_del - deletes entry from list.
		* @entry: the element to delete from the list.
		* Note: dlist_empty() on entry does not return true after this, the entry is
		* in an undefined state.
	*/
	//Удаляет entry из списка.
	//entry остаётся в неопределенном состоянии.
	static inline void __dlist_del_entry(struct dlist_head *entry)
	{
		__dlist_del(entry->prev, entry->next);
	}
	
	static inline void dlist_del(struct dlist_head *entry)
	{
		__dlist_del(entry->prev, entry->next);
		entry->next = DLIST_POISON1;
		entry->prev = DLIST_POISON2;
	}
	
	
	/**
		* dlist_replace - replace old entry by _new one
		* @old : the element to be replaced
		* @_new : the _new element to insert
		*
		* If @old was empty, it will be overwritten.
	*/
	//Заменяет элемент old в списке на элемент new.
	//entry остаётся в неопределенном состоянии.
	static inline void dlist_replace(struct dlist_head *old,
	struct dlist_head *_new)
	{
		_new->next = old->next;
		_new->next->prev = _new;
		_new->prev = old->prev;
		_new->prev->next = _new;
	}
	
	static inline void dlist_replace_init(struct dlist_head *old,
	struct dlist_head *_new)
	{
		dlist_replace(old, _new);
		INIT_DLIST_HEAD(old);
	}
	
	/**
		* dlist_del_init - deletes entry from list and reinitialize it.
		* @entry: the element to delete from the list.
	*/
	//Удаляет entry и реинициализирует его.
	static inline void dlist_del_init(struct dlist_head *entry)
	{
		__dlist_del_entry(entry);
		INIT_DLIST_HEAD(entry);
	}
	
	/**
		* dlist_move - delete from one list and add as another's head
		* @list: the entry to move
		* @head: the head that will precede our entry
	*/
	//Удаляет элемент из одного списка и добавляя в другой
	//после элемента head.
	static inline void dlist_move(struct dlist_head *list, struct dlist_head *head)
	{
		__dlist_del_entry(list);
		dlist_add(list, head);
	}
	
	/**
		* dlist_move_tail - delete from one list and add as another's tail
		* @list: the entry to move
		* @head: the head that will follow our entry
	*/
	//Удаляет элемент из одного списка и добавляя в другой
	//перед элементом head.
	static inline void dlist_move_tail(struct dlist_head *list,
	struct dlist_head *head)
	{
		__dlist_del_entry(list);
		dlist_add_tail(list, head);
	}
	
	/**
		* dlist_is_last - tests whether @list is the last entry in list @head
		* @list: the entry to test
		* @head: the head of the list
	*/
	//Проверяет, является ли list последним в списке.
	static inline int dlist_is_last(const struct dlist_head *list,
	const struct dlist_head *head)
	{
		return list->next == head;
	}
	
	/**
		* dlist_empty - tests whether a list is empty
		* @head: the list to test.
	*/
	//Проверяет, является ли head пустым списком.
	static inline int dlist_empty(const struct dlist_head *head)
	{
		return head->next == head;
	}
	
	
	
	/**
		* dlist_empty_careful - tests whether a list is empty and not being modified
		* @head: the list to test
		*
		* Description:
		* tests whether a list is empty _and_ checks that no other CPU might be
		* in the process of modifying either member (next or prev)
		*
		* NOTE: using dlist_empty_careful() without synchronization
		* can only be safe if the only activity that can happen
		* to the list entry is dlist_del_init(). Eg. it cannot be used
		* if another CPU could re-dlist_add() it.
	*/ 
	//Проверяет, является ли head пустым списком.
	//Также убеждается в его коректности.
	static inline int dlist_empty_careful(const struct dlist_head *head)
	{
		struct dlist_head *next = head->next;
		return (next == head) && (next == head->prev);
	}
	
	/**
		* dlist_rotate_left - rotate the list to the left
		* @head: the head of the list
	*/
	//Проворачивает список на один элемент.
	//Первый становится последним.
	static inline void dlist_rotate_left(struct dlist_head *head)
	{
		struct dlist_head *first;
		
		if (!dlist_empty(head)) {
			first = head->next;
			dlist_move_tail(first, head);
		}
	}
	
	/**
		* dlist_is_singular - tests whether a list has just one entry.
		* @head: the list to test.
	*/
	//Возвращает истину, если в списке остался только один элемент.
	static inline int dlist_is_singular(const struct dlist_head *head)
	{
		return !dlist_empty(head) && (head->next == head->prev);
	}
	
	static inline void __dlist_cut_position(struct dlist_head *list,
	struct dlist_head *head, struct dlist_head *entry)
	{
		struct dlist_head *new_first = entry->next;
		list->next = head->next;
		list->next->prev = list;
		list->prev = entry;
		entry->next = list;
		head->next = new_first;
		new_first->prev = head;
	}
	
	
	//Вырезает часть листа между head и entry(послений - включительно)
	//list становится новым заголовком вырезанной части. 
	//Предыдущее его состояние теряется.
	/**
		* dlist_cut_position - cut a list into two
		* @list: a _new list to add all removed entries
		* @head: a list with entries
		* @entry: an entry within head, could be the head itself
		*	and if so we won't cut the list
		*
		* This helper moves the initial part of @head, up to and
		* including @entry, from @head to @list. You should
		* pass on @entry an element you know is on @head. @list
		* should be an empty list or a list you do not care about
		* losing its data.
		*
	*/
	static inline void dlist_cut_position(struct dlist_head *list,
	struct dlist_head *head, struct dlist_head *entry)
	{
		if (dlist_empty(head))
		return;
		if (dlist_is_singular(head) &&
		(head->next != entry && head != entry))
		return;
		if (entry == head)
		INIT_DLIST_HEAD(list);
		else
		__dlist_cut_position(list, head, entry);
	}
	
	static inline void __dlist_splice(const struct dlist_head *list,
	struct dlist_head *prev,
	struct dlist_head *next)
	{
		struct dlist_head *first = list->next;
		struct dlist_head *last = list->prev;
		
		first->prev = prev;
		prev->next = first;
		
		last->next = next;
		next->prev = last;
	}
	
	/**
		* dlist_splice - join two lists, this is designed for stacks
		* @list: the _new list to add.
		* @head: the place to add it in the first list.
	*/
	//Соединяет два списка. Содержимое списка list оказывается  
	// в начале списка head
	static inline void dlist_splice(const struct dlist_head *list,
	struct dlist_head *head)
	{
		if (!dlist_empty(list))
		__dlist_splice(list, head, head->next);
	}
	
	/**
		* dlist_splice_tail - join two lists, each list being a queue
		* @list: the _new list to add.
		* @head: the place to add it in the first list.
	*/
	//Соединяет два списка. Содержимое списка list оказывается  
	// в хвосте списка head
	static inline void dlist_splice_tail(struct dlist_head *list,
	struct dlist_head *head)
	{
		if (!dlist_empty(list))
		__dlist_splice(list, head->prev, head);
	}
	
	/**
		* dlist_splice_init - join two lists and reinitialise the emptied list.
		* @list: the _new list to add.
		* @head: the place to add it in the first list.
		*
		* The list at @list is reinitialised
	*/
	//Соединяет два списка. Содержимое списка list оказывается  
	// в начале списка head. Реинициализирует элемент list
	static inline void dlist_splice_init(struct dlist_head *list,
	struct dlist_head *head)
	{
		if (!dlist_empty(list)) {
			__dlist_splice(list, head, head->next);
			INIT_DLIST_HEAD(list);
		}
	}
	
	/**
		* dlist_splice_tail_init - join two lists and reinitialise the emptied list
		* @list: the _new list to add.
		* @head: the place to add it in the first list.
		*
		* Each of the lists is a queue.
		* The list at @list is reinitialised
	*/
	//Соединяет два списка. Содержимое списка list оказывается  
	// в хвосте списка head. Реинициализирует элемент list
	static inline void dlist_splice_tail_init(struct dlist_head *list,
	struct dlist_head *head)
	{
		if (!dlist_empty(list)) {
			__dlist_splice(list, head->prev, head);
			INIT_DLIST_HEAD(list);
		}
	}
	
	/**
		* dlist_entry - get the struct for this entry
		* @ptr:	the &struct dlist_head pointer.
		* @type:	the type of the struct this is embedded in.
		* @member:	the name of the dlist_head within the struct.
	*/
	//Расчитывает структуру, содержащую элемент списка ptr.
	#define dlist_entry(ptr, type, member) \
	container_of(ptr, type, member)
	
	/**
		* dlist_first_entry - get the first element from a list
		* @ptr:	the list head to take the element from.
		* @type:	the type of the struct this is embedded in.
		* @member:	the name of the dlist_head within the struct.
		*
		* Note, that list is expected to be not empty.
	*/
	//Расчитывает первую структуру, содержащуюся в списке ptr,
	//или структуру, содержащую элемент списка следующий за ptr.
	//лист не должен быть пустым.
	#define dlist_first_entry(ptr, type, member) \
	dlist_entry((ptr)->next, type, member)
	
	/**
		* dlist_last_entry - get the last element from a list
		* @ptr:	the list head to take the element from.
		* @type:	the type of the struct this is embedded in.
		* @member:	the name of the dlist_head within the struct.
		*
		* Note, that list is expected to be not empty.
	*/
	//Расчитывает последнюю структуру, содержащуюся в списке ptr,
	//или структуру, содержащую элемент списка предшествующий ptr.
	//лист не должен быть пустым.
	#define dlist_last_entry(ptr, type, member) \
	dlist_entry((ptr)->prev, type, member)
	
	/**
		* dlist_first_entry_or_null - get the first element from a list
		* @ptr:	the list head to take the element from.
		* @type:	the type of the struct this is embedded in.
		* @member:	the name of the dlist_head within the struct.
		*
		* Note that if the list is empty, it returns NULL.
	*/
	//Расчитывает первую структуру, содержащуюся в списке ptr.
	//возвращает NULL, если список пустой.
	#define dlist_first_entry_or_null(ptr, type, member) \
	(!dlist_empty(ptr) ? dlist_first_entry(ptr, type, member) : NULL)
	
	/**
		* dlist_next_entry - get the next element in list
		* @pos:	the type * to cursor
		* @member:	the name of the dlist_head within the struct.
	*/
	//Расчитывает следующую структуру.
	#define dlist_next_entry(pos, member) \
	dlist_entry((pos)->member.next, typeof(*(pos)), member)
	
	/**
		* dlist_prev_entry - get the prev element in list
		* @pos:	the type * to cursor
		* @member:	the name of the dlist_head within the struct.
	*/
	//Расчитывает предыдущую структуру.
	#define dlist_prev_entry(pos, member) \
	dlist_entry((pos)->member.prev, typeof(*(pos)), member)
	
	/**
		* dlist_for_each	-	iterate over a list
		* @pos:	the &struct dlist_head to use as a loop cursor.
		* @head:	the head for your list.
	*/
	//Цикл, выполняющийся по всем элементам списка.
	#define dlist_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)
	
	/**
		* dlist_for_each_prev	-	iterate over a list backwards
		* @pos:	the &struct dlist_head to use as a loop cursor.
		* @head:	the head for your list.
	*/
	//Цикл, выполняющийся по всем элементам списка в обратном
	//направлении
	#define dlist_for_each_prev(pos, head) \
	for (pos = (head)->prev; pos != (head); pos = pos->prev)
	
	/**
		* dlist_for_each_safe - iterate over a list safe against removal of list entry
		* @pos:	the &struct dlist_head to use as a loop cursor.
		* @n:		another &struct dlist_head to use as temporary storage
		* @head:	the head for your list.
	*/
	//Цикл, выполняющийся по всем элементам списка.
	//Безопасен при использовании операций удаления.
	#define dlist_for_each_safe(pos, n, head) \
	for (pos = (head)->next, n = pos->next; pos != (head); \
	pos = n, n = pos->next)
	
	/**
		* dlist_for_each_prev_safe - iterate over a list backwards safe against removal of list entry
		* @pos:	the &struct dlist_head to use as a loop cursor.
		* @n:		another &struct dlist_head to use as temporary storage
		* @head:	the head for your list.
	*/
	//Цикл, выполняющийся по всем элементам списка в обратном
	//направлении
	//Безопасен при использовании операций удаления.
	#define dlist_for_each_prev_safe(pos, n, head) \
	for (pos = (head)->prev, n = pos->prev; \
	pos != (head); \
	pos = n, n = pos->prev)
	
	/**
		* dlist_for_each_entry	-	iterate over list of given type
		* @pos:	the type * to use as a loop cursor.
		* @head:	the head for your list.
		* @member:	the name of the dlist_head within the struct.
	*/
	//Цикл, выполняющийся по всем структурам списка.
	#define dlist_for_each_entry(pos, head, member)				\
	for (pos = dlist_first_entry(head, typeof(*pos), member);	\
	&pos->member != (head);					\
	pos = dlist_next_entry(pos, member))
	#define dlist_foreach_entry(pos, head, member)	dlist_for_each_entry(pos, head, member)
	
	
	/**
		* dlist_for_each_entry_reverse - iterate backwards over list of given type.
		* @pos:	the type * to use as a loop cursor.
		* @head:	the head for your list.
		* @member:	the name of the dlist_head within the struct.
	*/
	//Цикл, выполняющийся по всем структурам списка, движущийся в
	//обратном направлении.
	#define dlist_for_each_entry_reverse(pos, head, member)			\
	for (pos = dlist_last_entry(head, typeof(*pos), member);		\
	&pos->member != (head); 					\
	pos = dlist_prev_entry(pos, member))
	
	/**
		* dlist_prepare_entry - prepare a pos entry for use in dlist_for_each_entry_continue()
		* @pos:	the type * to use as a start point
		* @head:	the head of the list
		* @member:	the name of the dlist_head within the struct.
		*
		* Prepares a pos entry for use as a start point in dlist_for_each_entry_continue().
	*/
	//Подготавливает pos для использования в качестве курсора  dlist_for_each_entry_continue
	#define dlist_prepare_entry(pos, head, member) \
	((pos) ? : dlist_entry(head, typeof(*pos), member))
	
	/**
		* dlist_for_each_entry_continue - continue iteration over list of given type
		* @pos:	the type * to use as a loop cursor.
		* @head:	the head for your list.
		* @member:	the name of the dlist_head within the struct.
		*
		* Continue to iterate over list of given type, continuing after
		* the current position.
	*/
	//выполнять цикл, начиная со следующего элемента.
	#define dlist_for_each_entry_continue(pos, head, member) 		\
	for (pos = dlist_next_entry(pos, member);			\
	&pos->member != (head);					\
	pos = dlist_next_entry(pos, member))
	
	/**
		* dlist_for_each_entry_continue_reverse - iterate backwards from the given point
		* @pos:	the type * to use as a loop cursor.
		* @head:	the head for your list.
		* @member:	the name of the dlist_head within the struct.
		*
		* Start to iterate over list of given type backwards, continuing after
		* the current position.
	*/
	//выполнять цикл, начиная с предыдущего элемента в обратном направлении. 
	#define dlist_for_each_entry_continue_reverse(pos, head, member)		\
	for (pos = dlist_prev_entry(pos, member);			\
	&pos->member != (head);					\
	pos = dlist_prev_entry(pos, member))
	
	/**
		* dlist_for_each_entry_from - iterate over list of given type from the current point
		* @pos:	the type * to use as a loop cursor.
		* @head:	the head for your list.
		* @member:	the name of the dlist_head within the struct.
		*
		* Iterate over list of given type, continuing from current position.
	*/
	//Начать с этой точки.
	#define dlist_for_each_entry_from(pos, head, member) 			\
	for (; &pos->member != (head);					\
	pos = dlist_next_entry(pos, member))
	
		
	/**
		* dlist_for_each_entry_safe - iterate over list of given type safe against removal of list entry
		* @pos:	the type * to use as a loop cursor.
		* @n:		another type * to use as temporary storage
		* @head:	the head for your list.
		* @member:	the name of the dlist_head within the struct.
	*/
	//Стоек к удалению.
	#define dlist_for_each_entry_safe(pos, n, head, member)			\
	for (pos = dlist_first_entry(head, typeof(*pos), member),	\
	n = dlist_next_entry(pos, member);			\
	&pos->member != (head); 					\
	pos = n, n = dlist_next_entry(n, member))
	
	/**
		* dlist_for_each_entry_safe_continue - continue list iteration safe against removal
		* @pos:	the type * to use as a loop cursor.
		* @n:		another type * to use as temporary storage
		* @head:	the head for your list.
		* @member:	the name of the dlist_head within the struct.
		*
		* Iterate over list of given type, continuing after current point,
		* safe against removal of list entry.
	*/
	//Стоек к удалению, начинает со следующей позиции.
	#define dlist_for_each_entry_safe_continue(pos, n, head, member) 		\
	for (pos = dlist_next_entry(pos, member), 				\
	n = dlist_next_entry(pos, member);				\
	&pos->member != (head);						\
	pos = n, n = dlist_next_entry(n, member))
	
	/**
		* dlist_for_each_entry_safe_from - iterate over list from current point safe against removal
		* @pos:	the type * to use as a loop cursor.
		* @n:		another type * to use as temporary storage
		* @head:	the head for your list.
		* @member:	the name of the dlist_head within the struct.
		*
		* Iterate over list of given type from current point, safe against
		* removal of list entry.
	*/
	#define dlist_for_each_entry_safe_from(pos, n, head, member) 			\
	for (n = dlist_next_entry(pos, member);					\
	&pos->member != (head);						\
	pos = n, n = dlist_next_entry(n, member))
	
	/**
		* dlist_for_each_entry_safe_reverse - iterate backwards over list safe against removal
		* @pos:	the type * to use as a loop cursor.
		* @n:		another type * to use as temporary storage
		* @head:	the head for your list.
		* @member:	the name of the dlist_head within the struct.
		*
		* Iterate backwards over list of given type, safe against removal
		* of list entry.
	*/
	#define dlist_for_each_entry_safe_reverse(pos, n, head, member)		\
	for (pos = dlist_last_entry(head, typeof(*pos), member),		\
	n = dlist_prev_entry(pos, member);			\
	&pos->member != (head); 					\
	pos = n, n = dlist_prev_entry(n, member))
	
	/**
		* dlist_safe_reset_next - reset a stale dlist_for_each_entry_safe loop
		* @pos:	the loop cursor used in the dlist_for_each_entry_safe loop
		* @n:		temporary storage used in dlist_for_each_entry_safe
		* @member:	the name of the dlist_head within the struct.
		*
		* dlist_safe_reset_next is not safe to use in general if the list may be
		* modified concurrently (eg. the lock is dropped in the loop body). An
		* exception to this is if the cursor element (pos) is pinned in the list,
		* and dlist_safe_reset_next is called after re-taking the lock and before
		* completing the current iteration of the loop body.
	*/
	#define dlist_safe_reset_next(pos, n, member)				\
	n = dlist_next_entry(pos, member)
	

	static inline struct dlist_head *dlist_next(const struct dlist_head *list) {
	assert(!dlist_empty(list));
	return list->next;
}

static inline struct dlist_head *dlist_prev(const struct dlist_head *list) {
	assert(!dlist_empty(list));
	return list->prev;
}
	

#define dlist_first dlist_next
#define dlist_last  dlist_prev	
	
	
#endif




