#ifndef GENOS_READER
	#define GENOS_READER
	
	template<typename T>
	class Reader{
	public:
				
	virtual	int read();
	virtual int available();
			
	};
		
		
	
#endif