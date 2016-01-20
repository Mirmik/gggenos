


#ifndef GENOS_BASIC_TIME
	#define GENOS_BASIC_TIME
	
	#include "genos/types.h"
	
	//Базовый класс таймера. 
	
	namespace genos{
		typedef unsigned long time_t;
		
		class basic_timer
		{public:
			time_t s;
			time_t i;
			basic_timer(): s(0), i(0) {};
			basic_timer(time_t _s, time_t _i): s(_s), i(_i) {};
			
			uint8_t check(const time_t& n) {if (n - s >= i) return 1; else return 0;};
			void set(const time_t _s, const time_t _i) {s=_s; i=_i;};
			void set_bias(const time_t _i) {s=s + i; i=_i;};
			void reset() {s = s + i;};
			
			time_t start() {return s;};
			time_t interval() {return i;};
			time_t finish() {return i + s;};
		};
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
	
	
	
	
	
	
	
	
	};
	
	
	#endif	