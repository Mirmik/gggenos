#ifndef GENOS_UTIL_RING
#define GENOS_UTIL_RING

/*
#define ring_next(index, len)	\
({								\
	(index + 1) % len;			\
})

#define ring_available_to_putc(head, tail, len) 				\
({																\
	(head >= tail) ? len - 1 - head + tail : tail - head - 1;	\
})

#define ring_available_to_getc(head, tail, len)					\
({																\
	(head >= tail) ? head - tail : len + head - tail; 		  	\
})

#define ring_full(head, tail, len)				\
({												\
	(ring_next(head, len) == tail) ? 1 : 0;		\
})

#define ring_empty(head, tail)							\
({														\
	head == tail;										\
})

#define ring_putc(head, tail, buffer, len, c)			\
({														\
	buffer[head] = c;									\
	head = ring_next(head, len);						\
})

#define ring_putc_if_available(head, tail, buffer, len, c)	\
({															\
	if (ring_avaible_to_putc(len, head, tail))				\
	{														\
		buffer[head] = c;									\
		head = ring_next(head, len);						\
	};														\
})

#define ring_getc(head, tail, buffer, len)				\
({														\
	char c = buffer[tail];								\
	tail = ring_next(tail, len);						\
	c;													\
})
*/

#endif