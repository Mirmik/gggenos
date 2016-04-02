#ifndef GENOS_CONTEXT_H
#define GENOS_CONTEXT_H

//struct context;

#include "asm/context.h"

//#define CONTEXT_PRIVELEGED (1 << 0) /* TODO: ? */
//#define CONTEXT_IRQDISABLE (1 << 1)

__BEGIN_DECLS

void current_context_set(struct context*);
struct context* current_context_get();

extern void context_init(struct context *ctx, unsigned int flags,
		void (*routine_fn)(void), void *sp);

extern void context_switch(struct context *prev, struct context *next);

__END_DECLS

#define CONTEXT_JMP_NEW_STACK(routine, sp) \
	struct context ctx##__LINE__, old_ctx##__LINE__; \
	context_init(&ctx##__LINE__, 0, routine, sp); \
	context_switch(&old_ctx##__LINE__, &ctx##__LINE__)

#endif