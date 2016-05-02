#ifndef GENOS_LIST_HEAD
#define GENOS_LIST_HEAD

#include <inttypes.h>
#include "util/assert.h"
#include "sys/cdefs.h"
#include "defines/decltypeof.h"
#include "util/member.h"

struct list_head
{
	list_head* next;
	list_head* prev;
	
	__if_cplusplus(
	list_head() : next(this), prev(this), top(0) {};
	)
};

static inline void list_init(struct list_head* head)
{
	head->next = head;
	head->prev = head;
};

static inline void list_init_list(struct list_head* head)
{
	head->next = head;
	head->prev = head;
};

static inline int8_t list_is_empty(struct list_head* head)
{
	return head == head->next;
};

static inline void __list_add(struct list_head *_new, struct list_head *next, struct list_head *prev) 
{
	_new->prev = prev;
	_new->next = next;
	next->prev = _new;
	prev->next = _new;
}

static inline void list_add_next(struct list_head* _new, struct list_head* head)
{
	//assert(head->top);
	_new->top = head->top;
	__list_add(_new, head->next, head);
};

static inline void list_add_prev(struct list_head* _new, struct list_head* head)
{
	//assert(head->top);
	_new->top = head->top;
	__list_add(_new, head, head->prev);
};

static inline void __list_del(struct list_head* head)
{
	head->prev->next = head->next;
	head->next->prev = head->prev;
};

static inline void list_del(struct list_head* head)
{
	__list_del(head);
	list_init(head);	
};
#define list_del_init(x) list_del(x)

static inline void list_move_next(struct list_head* lst, struct list_head* head)
{
	__list_del(lst);
	list_add_next(lst, head);
};

static inline void list_move_prev(struct list_head* lst, struct list_head* head)
{
	__list_del(lst);
	list_add_prev(lst, head);
};

#define list_entry(ptr, type, member) \
container_of(ptr, type, member)

#define list_next_entry(pos, member) \
list_entry((pos)->member.next, decltypeof(*(pos)), member)

#define list_first_entry(ptr, type, member) \
list_entry((ptr)->next, type, member)

#define list_for_each(pos, head) \
for (pos = (head)->next; pos != (head); pos = pos->next)

#define list_for_each_safe(pos, n, head) \
for (pos = (head)->next, n = pos->next; pos != (head); \
pos = n, n = pos->next)

#define list_for_each_entry(pos, head, member)				\
for (pos = list_first_entry(head, decltypeof(*pos), member);	\
&pos->member != (head);					\
pos = list_next_entry(pos, member))

#define list_for_each_entry_safe(pos, n, head, member)			\
for (pos = list_first_entry(head, decltypeof(*pos), member),	\
n = list_next_entry(pos, member);			\
&pos->member != (head); 					\
pos = n, n = list_next_entry(n, member))
	
#endif