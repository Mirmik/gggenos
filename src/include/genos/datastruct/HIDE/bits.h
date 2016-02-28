#ifndef GENOS_BASE_STRUCT_FLAGS_H
	#define GENOS_BASE_STRUCT_FLAGS_H
	
	#include "genos/types.h"
	//bits
	
	
	//***Базовая логика.
	template<typename T> 
	static inline T bits_inv (const T& a) {return ~a;};
	
	template<typename T> 
	static inline T bits_and (const T& a, const T& b) {return a & b;};
	
	template<typename T> 
	static inline T bits_or  (const T& a, const T& b) {return a | b;};
	
	template<typename T> 
	static inline T bits_xor (const T& a, const T& b)  {return a ^ b;};
	
	
	//set, clr, rev операции (для реализации флагов)
	template<typename T> 
	static inline void bits_rev (T& a)  { a = ~a;};
	
	template<typename T> 
	static inline void bits_set (T& a, const T& b) { a |= b;};
	
	template<typename T> 
	static inline void bits_clr (T& a, const T& b)  { a &= ~b;};
	
	template<typename T> 
	static inline void bits_set_all (T& a) { a = ~(T)0;};
	
	
	template<typename T> 
	static inline void bits_clr_all (T& a)  { a = 0;};
	
	template<typename T> 
	static inline void bits_mask_set_all (T& a, const T& m) { a |= m;};
	
	template<typename T> 
	static inline void bits_mask_clr_all (T& a, const T& m)  { a &= ~m;};
	
	
	//***Маскированные операции.
	template<typename T>  
	static inline T bits_mask (const T& a, const T& m) 
	{
		return a & m;
	};
	
	template<typename T>  
	static inline T bits_mask_inv (const T& a, const T& m) 
	{
		return a ^ m;
	};
	
	template<typename T>  
	static inline T bits_mask_and (const T& a, const T& b, const T& m) 
	{
		return a & (~m | b);
	};
	
	template<typename T>  
	static inline T bits_mask_or (const T& a, const T& b, const T& m) 
	{
		return a | (b & m);
	};
	
	template<typename T>  
	static inline T bits_mask_change (const T& a, const T& b, const T& m) 
	{
		return (a & ~m) | (b & m);
	};
	
	template<typename T>  
	static inline T bits_mask_xor (const T& a, const T& b, const T& m) 
	{
		return a ^ (b & m);
	};
	
	
	
	template<typename T> 
	static inline void bits_mask_rev (T& a, const T& m)  
	{ a ^= m;};
	
	template<typename T>  
	static inline void bits_mask_set (T& a, const T& b, const T& m) 
	{
		a |= b & m;
	};
	
	template<typename T>  
	static inline void bits_mask_clr (T& a, const T& b, const T& m) 
	{
		a &= ~(b & m);
	};
	
	template<typename T>  
	static inline void bits_mask_assign (T& a, const T& b, const T& m) 
	{
		a = (a & ~m) | (b & m);
	};
	
	//***Операции сравнения и проверки.
	//Равенство.
	template<typename T>  
	static inline bool bits_eq (const T& a, const T& b) 
	{
		return (a == b);
	};
	
	template<typename T>  
	static inline bool bits_mask_eq (const T& a, const T& b, const T& m) 
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
	
	template<typename T>  
	static inline uint8_t bits_amount (T a) 
	{
		uint8_t i = 0;
		while(a != 0) {if (a & 1) i++; a >>= 1;};
		return i;
	};
	
	template<typename T>  
	static inline uint8_t bits_mask_amount (T a, const T& m) 
	{
		uint8_t i = 0;
		a &= m; 
		while(a != 0) {if (a & 1) i++; a >>= 1;};
		return i;
	};
	
	
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