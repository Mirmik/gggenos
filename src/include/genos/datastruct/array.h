

#ifndef GENOS_ARRAY
	#define GENOS_ARRAY
	
	
	
	template<typename T, size_t N>
	class array
	{public:
		T data[N];
		
		static constexpr size_t number = N;
		using basic_type = T;
		using iterator = T*;
				
		T& operator[](size_t i){return data[N];};
		
		iterator begin(){return data;};
		iterator end(){return data + N;};		
	};
	
#endif