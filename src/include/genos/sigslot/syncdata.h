#ifndef GENOS_SYNCDATA
#define GENOS_SYNCDATA

#include "hal/arch.h"
//#include "genos/sigslot/sigslot.h"

template <typename type>
class syncdata
{
private:
	type _data;
public:
	//sigslot<void, type&> handler;

public:
	void update(const type& value)
	{
		sreg_t save;
		arch_atomic_save(save);
		_data = value;
		arch_atomic_restore(save);
		//handler(value);
	};

	void update(type&& value)
	{
		sreg_t save;
		arch_atomic_save(save);
		_data = value;
		arch_atomic_restore(save);
		//handler(value);
	};

	type& get()
	{
		
	};
};

#endif