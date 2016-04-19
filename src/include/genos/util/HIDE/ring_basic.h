#ifndef GENOS_RING_WRITER
#define GENOS_RING_WRITER

#include <util/assert.h>
#include <stddef.h>
#include <util/math.h>

struct ring_h
{
	size_t head;
	size_t size;
};

struct ring_ht
{
	struct ring_h shead;
	size_t tail;
};

static inline