#ifndef GENOS_MUTEX_H
#define GENOS_MUTEX_H

#include "hal/arch.h"
#include "genos/kernel/waitserver.h"


class mutex
{
	volatile uint8_t not_busy;

public:
	mutex() : not_busy(1) {};	

	volatile uint8_t is_busy() { return not_busy == 0; };

	void enter()
	{
		sreg_t save;
		arch_atomic_save(save);
		while (is_busy())
			{
				wait_subst((uint8_t*)&not_busy);
			};
		not_busy = 0;
		arch_atomic_restore(save);
	};

	void leave()
	{
		sreg_t save;
		arch_atomic_save(save);
		not_busy = 1;
		arch_atomic_restore(save);
	};	
};

#endif