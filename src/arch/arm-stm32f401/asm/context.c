
#include "asm/context.h"

struct context* _current_context;

void current_context_set(struct context* cntxt)
{
	_current_context = cntxt;
};

struct context* current_context_get()
{
	return _current_context;
};

void context_init(struct context *ctx, unsigned int flags,
		void (*routine_fn)(void), void *sp) {
	ctx->lr = (uint32_t) routine_fn;
	ctx->sp = (uint32_t) sp;

//	if (flags & CONTEXT_PRIVELEGED) {
		ctx->control = 0;
//	} else {
//		ctx->control = CM3_CONTROL_NPRIV;
//	}
}
