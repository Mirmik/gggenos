#include "movecontroller.h"
#include "genos/debug/debug.h"
#include "genos/terminal/command_list.h"


GlobalMode global_mode = PULSE;

PositionController position_controller;

void PositionController::exec()
	{
		switch(state)
		{
			case INIT:
			mpulse_impulse(0);
			mpulse_speed(_spd);		
			pulsemode_set();
			state = INIT + 1;
			break;

			case INIT + 1:
			debug_print("here0");
			abspos_ret();
			state = INIT + 2;
			break;

			case INIT + 2:
			debug_print("here1");
			init_abspos = get_ret();
			debug_printdec_int64(init_abspos);dln;
			debug_printhex_uint64(init_abspos);dln;
			state = EXIT;
			break;		

			case EXIT:
			exit_autom();
			break;				
		};		
	};





#define TIM4_TPS 8400000
void mpulse_speed(uint32_t speed)
{
	float speed_rev_min;
	float speed_rev_sec;
	float pps;

	speed_rev_min = speed;

	if (speed_rev_min > MAX_SPEED) {speed_rev_min = MAX_SPEED; debug_print("speed decrease ");
	debug_printdec_uint32(MAX_SPEED); dln;};
	
	speed_rev_sec = speed_rev_min / 60;
	pps = speed_rev_sec * ENCODER_RESOLUTION;

	int res = TIM4_TPS / pps * 4 * 2 * 311 / 200;
	debug_printdec_uint32(res);

	TIM4->ARR = res;
};

extern uint8_t pulse_direction; 
void mpulse_direction(uint32_t dir)
{	
	pulse_direction = dir;
};


extern uint32_t impulse_count;
void mpulse_impulse(uint32_t pls)
{
	impulse_count = pls;
};

uint32_t mpulse_impulse()
{
	return impulse_count;
};