#ifndef GENOS_SCHEDEE_BASE
#define GENOS_SCHEDEE_BASE
	
#include "genos/sigslot/delegate.h"
#include "util/stub.h"

	//Базовый класс единицы планирования.
	class schedee 
	{
	public:
		schedee* parent;
	};
	
#endif