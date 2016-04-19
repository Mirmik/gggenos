#ifndef GENOS_UTIL_RING_HEAD_H
#define GENOS_UTIL_RING_HEAD_H

#include "sys/cdefs.h"

struct ring_head
{
	size_t head;
	size_t tail;
	size_t size;

	__if_cplusplus(
		ring_head(size_t sz):head(0),tail(0),size(sz){};
		)
};

__BEGIN_DECLS

inline static struct ring_head* ring_init(struct ring_head* r, size_t r_size)
{
	r->size = r_size;
	r->head = 0;
	r->tail = 0;
	return r;
}

inline static struct ring_head* ring_fixup_head(struct ring_head* r)
{
	while (r->head >= r->size) r->head -= r->size;
	return r; 
};

inline static struct ring_head* ring_fixup_tail(struct ring_head* r)
{
	while (r->tail >= r->size) r->tail -= r->size;
	return r; 
};

inline static int8_t ring_is_empty(struct ring_head* r)
{
	return r->head == r->tail;
};

inline static int8_t ring_is_full(struct ring_head* r)
{
	return r->head == (r->tail ? r->tail : r->size) - 1;
};

inline static size_t ring_data_size(struct ring_head* r) 
{ 
		return (r->head >= r->tail) ? 
		r->head - r->tail : 
		r->size + r->head - r->tail; 
};

inline static size_t ring_room_size(struct ring_head* r)
{
	return (r->head >= r->tail) ? 
	r->size - 1 + ( r->tail - r->head ) : 
	( r->tail - r->head ) - 1;
};

inline static struct ring_head* ring_move_head_one(struct ring_head* r)
{
	++r->head;
	if (r->head == r->size) r->head = 0;
	return r;
}

inline static struct ring_head* ring_move_tail_one(struct ring_head* r)
{
	++r->tail;
	if (r->tail == r->size) r->tail = 0;
	return r;
}

inline static struct ring_head* ring_move_head_bias(struct ring_head* r, size_t bias)
{
	r->head += bias;
	ring_fixup_head(r);
	return r;
}

inline static struct ring_head* ring_move_tail_bias(struct ring_head* r, size_t bias)
{
	r->tail += bias;
	ring_fixup_tail(r);
	return r;
}

__END_DECLS

#endif