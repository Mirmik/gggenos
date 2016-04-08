#ifndef GENOS_DEBUG_PR_H
#define GENOS_DEBUG_PR_H

#include "asm/float_to_uint.h"
#include "genos/gstl/horrible_cast.h"


template<typename T> void dpr(T obj);
template<typename T> void dprbin(T obj);
template<typename T> void dprhex(T obj);
template<typename T> void dprhex_dump(T* obj, uint32_t size) { debug_print_dump((void*)obj, size); dpl; };
template<typename T> void dprln(T obj) { dpr(obj); dln; dpl; };
template<typename T> void dprhexln(T obj) { dprhex(obj); dln; dpl; };

#endif