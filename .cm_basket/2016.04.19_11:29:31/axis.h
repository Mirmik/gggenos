#ifndef RFMEASURMENT_AXIS_H
#define RFMEASURMENT_AXIS_H
#include "genos/schedproc/switchScheduler.h"
#include "genos/kernel/waitserver.h"

#include "genos/sigslot/delegate.h"
#include "util/stub.h"

#define X_MAX_SPEED 1000
#define X_ENCODER_RESOLUTION 50000 

class Axis
{
private:
	uint32_t TimerPPS;
public:

	static constexpr bool FORWARD = 1;
	static constexpr bool BACKWARD = 0;

	delegate<void> enable_compare;
	delegate<void> disable_compare;

	volatile bool generator_reverse;
	volatile bool encoder_reverse;

	float encoder_resolution; 
	float generator_resolution;
	float multiplier;

	volatile uint32_t* encoder_reg;
	volatile uint32_t* genfreq_reg;
	volatile uint32_t* compare_reg;
	volatile uint32_t generator;
	volatile int32_t count_generated;
	volatile bool direction;
	volatile uint8_t pstate;
	delegate<void> trigger_callback;

	void init(
		volatile uint32_t* encreg, 
		volatile uint32_t* spdreg, 
		volatile uint32_t* cmpreg, 
		uint32_t _TimerPPS,
		float _encoder_resolution,
		float _generator_resolution);
		//void init(
		//volatile uint32_t* encreg, 
		//volatile uint32_t* spdreg, 
		//uint32_t _TimerPPS);
	void set_speed(uint32_t speed);
	void set_generator(uint64_t pulse);
	void set_direction(bool dir);
	void set_compare(int32_t cmp);

	int32_t get_compare();
	uint32_t get_speed();

	bool check_planed_position(
		int32_t start_encoder, 
		int32_t start_generator, 
		int32_t E);

	int32_t error_pulses(
		int32_t start_encoder, 
		int32_t start_generator);

	bool in_position(
		int32_t start_encoder, 
		int32_t start_generator,
		uint32_t E);
	
	bool internal_direction();
	
	int32_t read_encoder();
	int32_t read_generator();
	
	void stop();
};

extern Axis axis_x;

class MoveController
{
public:
	virtual void remove() = 0;
};


class HomePositionController : public MoveController
{
public:
	switchScheduler::process_switch* proc;

	Axis* axis;
	int32_t start_enc;
	int32_t start_gen;

	void init (
		Axis* _axis
	);
	void exec();
	void remove();
};

//#include "genos/schedproc/scheduler_base.h"
//#include "genos/schedproc/schedee_base.h"
extern switchScheduler switchSched; 
class PeriodicMoveController : public MoveController
{
public:

	//class Task
	//{
		uint32_t start_bias;
		uint32_t stop_bias;
		uint32_t points;
		uint32_t count;
	//};
		float now_compare;
		float period;

public:
	switchScheduler::process_switch* proc;



	Axis* axis;
	delegate<void> pulse_generate;

	int32_t start_enc;
	int32_t start_gen;
	int32_t distance;
	uint32_t speed;

	void trigger_handler();

	void init (
		Axis* _axis,
		delegate<void> _pulse_generator
	);

	void set_task(
		uint32_t _distance,
		uint32_t _start_bias, 
		uint32_t _stop_bias,
		uint32_t _points,
		uint32_t _speed);

	void exec();
	void remove();

};






class TestDirectionController : public MoveController
{
public:
	switchScheduler::process_switch* proc;

	Axis* axis;
	int32_t start_imp;

	void init (
		Axis* _axis
	);
	void exec();
	void remove();
};





#endif