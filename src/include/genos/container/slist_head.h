#ifndef SLIST_HEAD_H
#define SLIST_HEAD_H

#include <inttypes.h>
#include "util/assert.h"
#include "sys/cdefs.h"
#include "defines/decltypeof.h"
#include "util/member.h"

struct slist_head
{
	slist_head* next;

	__if_cplusplus(
	slist_head() : next(this) {};
	)
};

static inline void slist_init(struct slist_head* head)
{
	head->next = head;
};

static inline void slist_add_next(struct slist_head* _new, struct slist_head* head)
{
	_new->next = head->next;
	head->next = _new; 
};

static inline void slist_del_next(struct slist_head* head)
{
	slist_head* _del = head->next;
	head->next = head->next->next;
	slist_init(_del);	
};

#define slist_entry(ptr, type, member) \
container_of(ptr, type, member)

#define slist_next_entry(pos, member) \
slist_entry((pos)->member.next, decltypeof(*(pos)), member)

#define slist_first_entry(ptr, type, member) \
slist_entry((ptr)->next, type, member)

#define slist_for_each(pos, head) \
for (pos = (head)->next; pos != (head); pos = pos->next)

#define slist_for_each_with_prev(pos, prev, head) \
for (pos = (head)->next, prev = (head); pos != (head); \
	prev = pos, pos = pos->next)

#define slist_for_each_safe_with_prev(pos, n, prev, head) \
for (pos = (head)->next, n = pos->next, prev = (head); \
	pos != (head); pos = n, n = pos->next, \
	prev = prev->next == pos ? prev : prev->next)

#endif