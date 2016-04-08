#ifndef GENOS_MESSAGE_LIST_H
#define GENOS_MESSAGE_LIST_H

#include "util/dlist.h"
#include "genos/debug/debug.h"
#include "compiler.h"
#include "hal/arch.h"
#include "stdlib.h"

struct message
{
	void* buffer;
	uint16_t length;
};

struct listed_message
{
	struct dlist_head lst;
	struct message msg;
};

struct meslst
{
	struct dlist_head head;
	uint16_t count;

	__if_cplusplus(meslst() : head(), count(0) {};)
};

static inline void INIT_meslst(struct meslst* ml) 
{
	INIT_dlist_HEAD(&ml->head); 
	ml->count=0;
};

static inline void meslst_push_front(struct meslst* ml, void* message, uint16_t len) 
{
	struct listed_message* lmes =(listed_message*) malloc(sizeof(listed_message));
	void* buff = malloc(len);
	memcpy(buff, message, len);
	lmes->msg.buffer = buff;
	lmes->msg.length = len;
	ml->count++;
	dlist_add(&lmes->lst, &ml->head);
};

static inline void meslst_push_back(struct meslst* ml, void* message, uint16_t len)
{
	struct listed_message* lmes =(listed_message*) malloc(sizeof(listed_message));
	void* buff = malloc(len);
	memcpy(buff, message, len);
	lmes->msg.buffer = buff;
	lmes->msg.length = len;
	ml->count++;
	dlist_add_tail(&lmes->lst, &ml->head);
};

static inline void meslst_pop_front(struct meslst* ml)
{
	if (dlist_empty(&ml->head)) return;
	listed_message* lmes = dlist_first_entry(&ml->head, listed_message, lst);
	dlist_del(&lmes->lst);
	free(lmes->msg.buffer);
	free(lmes);
};

static inline void meslst_pop_back(struct meslst* ml)
{
	if (dlist_empty(&ml->head)) return;
	listed_message* lmes = dlist_last_entry(&ml->head, listed_message, lst);
	dlist_del(&lmes->lst);
	free(lmes->msg.buffer);
	free(lmes);
};

static inline struct message* meslst_pick_front(struct meslst* ml)
{
	if (dlist_empty(&ml->head)) return (struct message*) -1;
	listed_message* lmes = dlist_first_entry(&ml->head, listed_message, lst);
	return &lmes->msg;
};

static inline struct message* meslst_pick_back(struct meslst* ml)
{
	if (dlist_empty(&ml->head)) return (struct message*) -1;
	listed_message* lmes = dlist_first_entry(&ml->head, listed_message, lst);
	return &lmes->msg;
};

static inline void meslst_debug_print(struct meslst* ml)
{
	listed_message* pos;
	dlist_for_each_entry(pos, &ml->head, lst)
	{
		debug_write((char*)pos->msg.buffer, pos->msg.length); 
		debug_print("\n\r");
	};
};

static inline void meslst_atomic_push_back(struct meslst* ml, void* message, uint16_t len) 
{
	sreg_t save;
	arch_atomic_temp(save);
	meslst_push_back(ml,message,len);
	arch_deatomic_temp(save);
};

static inline void meslst_atomic_push_front(struct meslst* ml, void* message, uint16_t len) 
{
	sreg_t save;
	arch_atomic_temp(save);
	meslst_push_front(ml,message,len);
	arch_deatomic_temp(save);
};


static inline void meslst_atomic_pop_front(struct meslst* ml)
{
	sreg_t save;
	arch_atomic_temp(save);
	meslst_pop_front(ml);
	arch_deatomic_temp(save);
};

static inline void meslst_atomic_pop_back(struct meslst* ml)
{
	sreg_t save;
	arch_atomic_temp(save);
	meslst_pop_back(ml);
	arch_deatomic_temp(save);
};

static inline struct message* meslst_atomic_pick_front(struct meslst* ml)
{
	sreg_t save;
	arch_atomic_temp(save);
	struct message* msg = meslst_pick_front(ml);
	arch_deatomic_temp(save);
	return msg;
};

static inline struct message* meslst_atomic_pick_back(struct meslst* ml)
{
	sreg_t save;
	arch_atomic_temp(save);
	struct message* msg = meslst_pick_back(ml);
	arch_deatomic_temp(save);
	return msg;
};


#endif