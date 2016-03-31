
#include "asm/context.h"


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
