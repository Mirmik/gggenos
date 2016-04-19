#ifndef GENOS_UROBOROS_HEAD_H
#define GENOS_UROBOROS_HEAD_H

#include "sys/cdefs.h"

//Голова Уробороса...
//Лучше вам не знать.
struct uroboros_head
{
	size_t head;
	size_t size;

	__if_cplusplus(
		uroboros_head(size_t sz):size(sz),head(0){};
		)
};

__BEGIN_DECLS

//Инициализация Уробороса. Начало бесконечного.
//Прям мурашки по коже.
inline static struct uroboros_head* uroboros_init(struct uroboros_head* u, size_t sz)
{
	u->size = sz;
	u->head = 0;
};

//А не зажрался ли ты?
inline static struct uroboros_head* uroboros_fixup_head(struct uroboros_head* u)
{
	while (u->head >= u->size) u->head -= u->size;
	return u;
};

//Пожрать чуть-чуть...
inline static struct uroboros_head* uroboros_move_head_one(struct uroboros_head* u)
{
	++u->head;
	if (u->head == u->size) u->head = 0;
	return u;
}

//Пожрать больше, чем чуть-чуть...
inline static struct uroboros_head* uroboros_move_head_bias(struct uroboros_head* u, size_t bias)
{
	u->head += bias;
	uroboros_fixup_head(u);
	return u;
}

inline static size_t uroboros_how_many_head_to_top(struct uroboros_head* u)
{
	return u->size - u->head;
}

__END_DECLS

//Всё... А выдумали, что файл длинный? Этож только голова.

#endif
