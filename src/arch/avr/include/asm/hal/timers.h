#ifndef AVR_TIMERS_H
#define AVR_TIMERS_H

	#include "sys/cdefs.h"
	
	enum timerSource
	{
		timerSourceStop = 0,
		timerSourceCK = 1,
		timerSourceCKDiv8 = 2,
		timerSourceCKDiv64 = 3,
		timerSourceCKDiv256 = 4,
		timerSourceCKDiv1024 = 5,
		timerSourceTaktRising = 6,
		timerSourceTaktFalling = 7
	};

	//enum timerWgm
	//{
	//	timerWgm
	//};

	enum timerCom
	{
		timerComNC = 0,
		//
		timerComLoHalf = 2,
		timerComHiHalf = 3
	};

	__BEGIN_DECLS

	void timer1_source(enum timerSource src);	
	//void timer1_wgm(enum timerWgm);	

	void timer1_comA(enum timerCom);	

	__END_DECLS

#endif