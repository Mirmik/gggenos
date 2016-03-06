#ifndef GENOS_WRITER
	#define GENOS_WRITER
	
	template<typename T>
	class Writer{
	public:
		virtual	int write(T c) = 0;
		virtual	int write(T* c, int n) = 0;
		virtual	int flush() = 0;	
	};

#endif