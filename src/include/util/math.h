//Некоторые математические функции.

#ifndef UTIL_MATH_H_
#define UTIL_MATH_H_

//Возвращает наибольшее из двух вариантов.
#define max(a, b) \
	({                                         \
		typeof(a) __max_a = (a);               \
		typeof(b) __max_b = (b);               \
		__max_a > __max_b ? __max_a : __max_b; \
	})

//Возвращает наименьшее из двух вариантов.
#define min(a, b) \
	({                                         \
		typeof(a) __min_a = (a);               \
		typeof(b) __min_b = (b);               \
		__min_a < __min_b ? __min_a : __min_b; \
	})

//Полочки. Вычисляет значение первого заданного аргумента, 
//ограниченное диапазоном, определенным вторым и третьим 
//заданными аргументами.
#define clamp(val, lo, hi) \
	({                                          \
		typeof(val) __clamp_val = (val);        \
		typeof(lo)  __clamp_lo  = (lo);         \
		typeof(hi)  __clamp_hi  = (hi);         \
		__clamp_val < __clamp_lo ? __clamp_lo : \
		__clamp_val > __clamp_hi ? __clamp_hi : \
		__clamp_val;                            \
	})

//Проверяет, находится ли число в диапазоне [lo,hi).
#define check_range(val, lo, hi) \
	({                                          \
		typeof(val) __check_val = (val);        \
		typeof(lo)  __check_lo  = (lo);         \
		typeof(hi)  __check_hi  = (hi);         \
		__check_val >= __check_lo &&            \
		__check_val < __check_hi;               \
	})

//Проверяет, находится ли число в диапазоне [lo,hi].
#define check_range_incl(val, lo, hi) \
	({                                          \
		typeof(val) __check_val = (val);        \
		typeof(lo)  __check_lo  = (lo);         \
		typeof(hi)  __check_hi  = (hi);         \
		__check_val >= __check_lo &&            \
		__check_val <= __check_hi;              \
	})

//Проверяет, находится ли число в диапазоне (lo,hi).
#define check_range_excl(val, lo, hi) \
	({                                          \
		typeof(val) __check_val = (val);        \
		typeof(lo)  __check_lo  = (lo);         \
		typeof(hi)  __check_hi  = (hi);         \
		__check_val > __check_lo &&             \
		__check_val < __check_hi;               \
	})

#endif /* UTIL_MATH_H_ */
