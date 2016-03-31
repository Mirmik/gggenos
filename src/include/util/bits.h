#ifndef GENOS_UTIL_BITS
#define GENOS_UTIL_BITS

	#include "defines/decltypeof.h"
	#include "util/macro.h"

	

	#define bits_bit_set(a, num) ({(a) |= 1 << (num);})
	#define bits_bit_clr(a, num) ({(a) &= ~(1 << (num);})

	#define bits_bit_is_set(a, num) ({(a) & (1 << (num);})
	#define bits_bit_is_clr(a, num) ({!((a) & (1 << (num));})

	#define bits_set(a, b) ({(a) |= (b);})
	#define bits_clr(a, b) ({(a) &= ~(b);})
	#define bits_rev(a) ({(a) = ~(a);})

	#define bits_mask_rev(a, b) ({(a) ^= (b);})
	
	#define bits_mask(src, mask)			\
	({										\
		(src) & (mask);						\
	})
	
	#define bits_mask_bias(src, mask, bias)		\
	({											\
		((src) & ((mask) << (bias)) >> (bias);			\
	})


	//Установка битов в соответствии с маской.
	//@ dest
	//@	src
	//@	mask
	#define bits_mask_assign(obj, src, mask) 				\
	({														\
		decltypeof(obj) _mask = mask;						\
		decltypeof(obj) _src = src;			 				\
		obj = ((obj) & ~(_mask)) | ((_src) & (_mask));		\
	})

	//Установка битов в соответствии с маской по смещению.
	//@ dest
	//@	src
	//@	mask
	//@ bias
	#define bits_mask_assign_bias(obj, src, mask, bias) \
	({												\
		decltypeof(obj) MACRO_GUARD(_mask) = (mask) << (bias);	\
		decltypeof(obj) MACRO_GUARD(_src) = (src) << (bias); 	\
		bits_mask_assign((obj), MACRO_GUARD(_src), MACRO_GUARD(_mask));			\
	})
		
#endif