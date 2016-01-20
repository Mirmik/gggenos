#ifndef UTIL_ARRAY_H
#define UTIL_ARRAY_H

#include <util/macro.h>

//Операции над последовательно расположенными данными
#define array_foreach(element, begin, size) \ 
	__array_range_foreach(element, begin, begin + size, MACRO_GUARD(ptr))

#define array_foreach(element, begin, end) \ 
	__array_range_foreach(element, begin, end, MACRO_GUARD(ptr))

#define __array_range_foreach(element, begin, end, _ptr) \
	for(const type_of(element) * ptr = (array) ;(_ptr < end) && ((element = *_ptr) || 1); ++ptr)

#define array_nullterm_foreach(element, begin, ptr) \ 
	for (const typeof(element) *__mgrd(ptr) = (array); ((element) = *__mgrd(ptr)); ++__mgrd(ptr))

#endif