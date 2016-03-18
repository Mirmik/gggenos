#ifndef AVR_TIMERS_H
#define AVR_TIMERS_H

	#include "sys/cdefs.h"	
	#include "inttypes.h"
	
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

	enum timer16Wgm
	{
		timerWgm16Normal = 0,
		timerWgm16PWMPhaseCorrectTop00FF = 1,
		timerWgm16PWMPhaseCorrectTop01FF = 2,
		timerWgm16PWMPhaseCorrectTop03FF = 3,
		timerWgm16CTCTopOCRnA = 4,
		timerWgm16FastPWMTop00FF = 5,
		timerWgm16FastPWMTop01FF = 6,
		timerWgm16FastPWMTop03FF = 7,
		timerWgm16PWMPhaseFreqTopICR = 8,
		timerWgm16PWMPhaseFreqTopOCRnA = 9,
		timerWgm16PWMPhaseTopICR = 10,
		timerWgm16PWMPhaseTopOCRnA = 11,
		timerWgm16CTCTopICR = 12,
		timerWgm16RESERVED = 13,
		timerWgm16FastPWMTopICR = 14,
		timerWgm16FastPWMTopOCRnA = 15
	};


	enum timerWgm
	{
		timerWgmNormal = 0,
		timerWgmPWMPhaseCorrectTopFF = 1,
		timerWgmCTC = 2,
		timerWgmFastPWMTopFF = 3,
		timerWgmFastRESERVED1 = 4,
		timerWgmPWMPhaseCorrectTopOCRA = 5,
		timerWgmFastRESERVED2 = 6,
		timerWgmFastPWMTopOCRA = 7
	};


	enum timerCom
	{
		timerComNC = 0,
		timerComSpec = 1,
		timerComLoHalf = 2,
		timerComHiHalf = 3
	};

	__BEGIN_DECLS

	void timer0_source_set(enum timerSource src);
	void timer0_wgm_set(enum timerWgm src);
	void timer0_comA_set(enum timerCom);	
	void timer0_comB_set(enum timerCom);	

	uint8_t timer0_source_get();
	int timer0_divider_get();
	uint8_t timer0_wgm_get();
	uint8_t timer0_comA_get();	
	uint8_t timer0_comB_get();		
	uint8_t timer0_comC_get();

	void timer1_source_set(enum timerSource src);
	void timer1_wgm_set(enum timer16Wgm src);
	void timer1_comA_set(enum timerCom);	
	void timer1_comB_set(enum timerCom);		
	void timer1_comC_set(enum timerCom);

	uint8_t timer1_source_get();
	int timer1_divider_get();
	uint8_t timer1_wgm_get();
	uint8_t timer1_comA_get();	
	uint8_t timer1_comB_get();		
	uint8_t timer1_comC_get();

	void timer3_source_set(enum timerSource src);
	void timer3_wgm_set(enum timer16Wgm src);
	void timer3_comA_set(enum timerCom);	
	void timer3_comB_set(enum timerCom);		
	void timer3_comC_set(enum timerCom);

	uint8_t timer3_source_get();
	int timer3_divider_get();
	uint8_t timer3_wgm_get();
	uint8_t timer3_comA_get();	
	uint8_t timer3_comB_get();		
	uint8_t timer3_comC_get();


	int32_t timer_get_freq(int i);

	__END_DECLS

	#ifdef __cplusplus

	void timer_info(int argc, char** argv);
	void timer_info(int i);

	#endif

#endif