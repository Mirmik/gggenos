#ifndef GENOS_STRING_FUNC_H
#define GENOS_STRING_FUNC_H


	class Strless
	{
		public:
		int operator()(const char* a, const char* b)
		{	
			if (strcmp(a,b) < 0) return true;
			return false;
		};
	};





#endif