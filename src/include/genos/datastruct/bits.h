#ifndef GENOS_BASE_STRUCT_FLAGS_H
	#define GENOS_BASE_STRUCT_FLAGS_H
	
	//#include "genos/types.h"
	//bits
	
	#ifdef __cplusplus
	
	//***Базовая логика.
	template<typename T1, typename T2> 
	static inline T1 bits_inv (const T2& a) {return ~a;};
	
	template<typename T2, typename T3>
	static inline T2 bits_and (const T2& a, const T3& b) {return a & b;};
	
	template<typename T1, typename T2, typename T3>
	static inline T1 bits_or  (const T2& a, const T3& b) {return a | b;};
	
	template<typename T1, typename T2, typename T3>
	static inline T1 bits_xor (const T2& a, const T3& b)  {return a ^ b;};
	
	
	//set, clr, rev операции (для реализации флагов)
	template<typename T>
	static inline void bits_rev (T& a)  { a = ~a;};
	
	template<typename T1, typename T2> 
	static inline void bits_set (T1& a, const T2& b) { a |= b;};
	
	template<typename T1, typename T2> 
	static inline void bits_clr (T1& a, const T2& b)  { a &= ~b;};
	

	template<typename T> 
	static inline void bits_set_all (T& a) { a = ~(T)0;};
	
	template<typename T> 
	static inline void bits_clr_all (T& a)  { a = 0;};
	

	template<typename T1, typename T2> 
	static inline void bits_mask_set_all (T1& a, const T2& m) { a |= m;};
	
	template<typename T1, typename T2> 
	static inline void bits_mask_clr_all (T1& a, const T2& m)  { a &= ~m;};
	
	
	//***Маскированные операции.
	template<typename T2, typename T3>
	static inline T2 bits_mask (const T2& a, const T3& m) 
	{
		return a & m;
	};
	
	template<typename T1, typename T2, typename T3>
	static inline T1 bits_mask_inv (const T2& a, const T3& m) 
	{
		return a ^ m;
	};
	
	template<typename T1, typename T2, typename T3, typename T4>
	static inline T1 bits_mask_and (const T2& a, const T3& b, const T4& m) 
	{
		return a & (~m | b);
	};
	
	template<typename T1, typename T2, typename T3, typename T4>
	static inline T1 bits_mask_or (const T2& a, const T3& b, const T4& m) 
	{
		return a | (b & m);
	};
	
	template<typename T1, typename T2, typename T3, typename T4> 
	static inline T1 bits_mask_change (const T2& a, const T3& b, const T4& m) 
	{
		return (a & ~m) | (b & m);
	};
	
	template<typename T1, typename T2, typename T3, typename T4>
	static inline T1 bits_mask_xor (const T2& a, const T3& b, const T4& m) 
	{
		return a ^ (b & m);
	};
	
	
	
	template<typename T1, typename T2> 
	static inline void bits_mask_rev (T1& a, const T2& m)  
	{ a ^= m;};

	template<typename T1, typename T2, typename T3>
	static inline void bits_mask_set (T1& a, const T2& b, const T3& m) 
	{
		a |= b & m;
	};
	
	template<typename T1, typename T2, typename T3>
	static inline void bits_mask_clr (T1& a, const T2& b, const T3& m) 
	{
		a &= ~(b & m);
	};
	
	template<typename T1, typename T2, typename T3>
	static inline void bits_mask_assign (T1& a, const T2& b, const T3& m) 
	{
		a = (a & ~m) | (b & m);
	};
	
	//***Операции сравнения и проверки.
	//Равенство.
	template<typename T1, typename T2> 
	static inline bool bits_eq (const T1& a, const T2& b) 
	{
		return (a == b);
	};
	
	template<typename T1, typename T2, typename T3>
	static inline bool bits_mask_eq (const T1& a, const T2& b, const T3& m) 
	{
		return ((a & m) == (b & m));
	};
	//Внутренняя логика контейнера.
	//Хотя бы один масскированный бит установлен
	//@a - контейнер.
	//@m - маска.
	template<typename T1, typename T2>  
	static inline bool bits_mask_internal_or (const T1& a, const T2& m) 
	{
		return (a & m);
	};
	
	//Все масскированные биты установлены
	//@a - контейнер.
	//@m - маска.
	template<typename T1, typename T2>  
	static inline bool bits_mask_internal_and (const T1& a, const T2& m) 
	{
		return (a & m) == m;
	};
	
	#include "genos/gstl/utility.h"
	#include "genos/debug/debug_templates.h"
	template<typename T>  
	static inline uint8_t bits_amount (T a) 
	{
		typename  gstd::make_unsigned<T>::type b = (typename gstd::make_unsigned<T>::type)(a);

		debug_print_type<typename gstd::make_unsigned<T>::type>();

		uint8_t i = 0;
		while(b != 0) {if (b & 1) i++; b >>= 1;};
		return i;
	};

	#else


	#define bits_set(a, b) {a |= (b);}
	#define bits_clr(a, b) {a &= ~(b);}
	#define bits_rev(a) {a = ~(a);}


	#define bits_mask_rev(a, b) {a ^= (b);}





	#endif

	/*
	template<typename T>  
	static inline uint8_t bits_mask_amount (T a, const T& m) 
	{
		uint8_t i = 0;
		a &= m; 
		while(a != 0) {if (a & 1) i++; a >>= 1;};
		return i;
	};*/
	
	
	/*
	//type generic опрации для работы с флагами
	template<typename T> static inline void flags_set(T& flags, const T& f){bits_set(flags,f);};
	template<typename T> static inline void flags_clr(T& flags, const T& f){bits_clr(flags,f);};
	
	template<typename T1, typename T2> 
	static inline bool flags_mask_internal_or(const T1& flags, const T2& mask) 
	{return bits_mask_internal_or(flags, mask);};
	
	template<typename T1, typename T2> 
	static inline bool flags_mask_internal_and(const T1& flags, const T2& mask) 
	{return bits_mask_internal_and(flags, mask);};
	
	
	template<typename T> 
	static inline void flags_mask_assign(T& flags, const T& f, const T& mask){
		flags = (f & mask) | (flags & ~mask);
	};*/
	
#endif