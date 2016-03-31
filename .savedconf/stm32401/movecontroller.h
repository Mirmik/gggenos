#ifndef MOVECONTROLLER_H
#define MOVECONTROLLER_H

#include "communication_func.h"

#define ENCODER_FULL_RESOLUTION 4194304
#define MAX_SPEED 1000

#define ENCODER_RESOLUTION 50000
#define ELECTRONIC_GEAR 1000

enum GlobalMode
{
	JOG = 0,
	POSITIONING = 1,
	PULSE = 2
};

extern GlobalMode global_mode;


class PositionController
{
public:

int32_t init_abspos;
uint32_t _spd = 200;

static constexpr int INIT = 0;
static constexpr int LOOP = 20;
static constexpr int EXIT = 100;

int state = 0;

	void exec();
};

void mpulse_speed(uint32_t speed);
void mpulse_direction(uint32_t dir);
void mpulse_impulse(uint32_t pls);
uint32_t mpulse_impulse();

extern PositionController position_controller;

#endif