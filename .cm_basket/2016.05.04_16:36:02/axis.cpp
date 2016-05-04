#include "axis.h"
#include "genos/io/Serial.h"
#include "genos/io/SerToSer.h"
#include "util/math.h"
#include "hal/gpio.h"

Axis axis_x;
	extern Serial_t<512,512> Serial6;
	extern Serial_t<512,512> Serial2;
	extern SerToSer S2S;

	uint32_t start_between_trig;
	uint32_t start_between_eth;
	uint32_t betwtrigger;

	void PeriodicMoveController::trigger_handler()
	{
		//Serial2.println((uint64_t)(millis() - start_between_trig));
		pulse_generate();
		gpio_port_tgl_pin(GPIOD,14);
		send_flag = 1;
		//Serial2.println(millis());
		count++;
		now_compare = now_compare + period;
		axis->set_compare(start_enc + now_compare);
		if (count == points) axis->disable_compare();
		start_between_trig = millis();
	};


	void PeriodicMoveController::set_task(
		uint32_t _distance,
		uint32_t _start_bias, 
		uint32_t _stop_bias,
		uint32_t _points,
		uint32_t _speed)
	{
		start_bias = _start_bias;
		stop_bias = _stop_bias;
		points = _points;
		distance = _distance;
		speed = _speed;
		period = (float)(distance - start_bias - stop_bias) / (points-1);
		now_compare = start_bias;
		count = 0;		
	};


	void PeriodicMoveController::init (
		Axis* _axis,
		delegate<void> _pulse_generator
	)	
	{
	axis = _axis;
	pulse_generate = _pulse_generator;
	axis->trigger_callback = 
	delegate_mtd(this, &PeriodicMoveController::trigger_handler); 
	proc = switchSched.registry(this, &PeriodicMoveController::exec);
	start_enc = axis->read_encoder();
	start_gen = axis->read_generator();
	};

	#include "syscontext/syscontext.h"
	#include "genos/terminal/command_list.h"
	void PeriodicMoveController::exec()
	{
		stdout.println("PeriodicMoveControllerStart");
		axis->set_compare(start_enc + start_bias);
		axis->enable_compare();
		axis->set_speed(speed);
		axis->set_direction(Axis::FORWARD);
		axis->set_generator(distance / axis->multiplier);
		//while(1)
		while(!axis->in_position(start_enc, start_gen, 200)) 
		{
			if (send_flag)
				{
					Serial2.print("betw:");
					Serial2.println((uint64_t)(millis() - start_between_eth));
					start_between_eth = millis();
					send_flag = 0;
					if (S2S.send(0xDD,0)) debug_panic("Communication_error");
				};

			if (!axis->check_planed_position(start_enc, start_gen, 60000))
			{debug_print("AllBad"); remove();};
			//stdout.println((int32_t)axis->error_pulses(start_enc,start_gen));
			//stdout.print("f");
			schedule_subst();
			//schedule_subst();
		};
		remove();
	};	
	

	extern MoveController* current_controller;
	void PeriodicMoveController::remove()
	{
		stdout.println("PeriodicMoveControllerRemove");
		axis->stop();
		axis->disable_compare();
		current_controller = 0;
		if (proc == current_schedee()) exit_subst();
		else switchSched.schedee_set_zombie(proc);
	};




	bool Axis::check_planed_position(
		int32_t start_encoder, 
		int32_t start_generator, 
		int32_t E)
	{
		int32_t planed = (read_generator() - start_generator) * multiplier;
		int32_t real = read_encoder() - start_encoder;
		return check_range_incl(real - planed, -E, E);
	};

	bool Axis::in_position(
		int32_t start_encoder, 
		int32_t start_generator,
		uint32_t E)
	{
		return
		generator == 0 
		&&
		abs(error_pulses(start_encoder,start_generator)) < E;
	};

	int32_t Axis::error_pulses(
		int32_t start_encoder, 
		int32_t start_generator)
	{
		//dpr("p:");dpr((int32_t)((read_generator() - start_generator) * multiplier));
		//dpr("r:");dpr((int32_t)(read_encoder() - start_encoder));
	//	dpr("p:");dpr((int32_t)(read_generator()));
	//	dpr("r:");dpr((int32_t)(read_encoder()));
	//	dln;
		int32_t planed = (read_generator() - start_generator) * multiplier;
		int32_t real = read_encoder() - start_encoder;
		return planed - real;
	};


	void Axis::init(
		volatile uint32_t* encreg, 
		volatile uint32_t* spdreg, 
		volatile uint32_t* cmpreg, 
		uint32_t _TimerPPS,
		float _encoder_resolution,
		float _generator_resolution
		)
	{
		encoder_reg = encreg;
		genfreq_reg = spdreg;
		compare_reg = cmpreg;
		trigger_callback = do_nothing;
		generator = 0;
		TimerPPS = _TimerPPS;
		direction = FORWARD;
		encoder_resolution = _encoder_resolution;
		generator_resolution = _generator_resolution;
		multiplier = encoder_resolution / generator_resolution;
		encoder_reverse = false;
		generator_reverse = false;
	};

	void Axis::set_speed(uint32_t speed)
	{
		float speed_rev_min;
		float speed_rev_sec;
		float pps;

		speed_rev_min = speed;

		if (speed_rev_min > X_MAX_SPEED) {speed_rev_min = X_MAX_SPEED; debug_print("speed decrease ");
		debug_printdec_uint32(X_MAX_SPEED); dln;};
	
		speed_rev_sec = speed_rev_min / 60.0;
		pps = speed_rev_sec * (float)X_ENCODER_RESOLUTION;

		int res = (float)TimerPPS / pps * 4.0 * 2.0 * 311.0 / 200.0;
	
		assert(res > 10);

		*genfreq_reg = res;
	};

	uint32_t Axis::get_speed()
	{
		float speed_rev_min;
		float speed_rev_sec;
		float pps;

		pps = (float)TimerPPS / *genfreq_reg  * 4.0 * 2.0 * 311.0 / 200.0;
		speed_rev_sec = pps / (float)X_ENCODER_RESOLUTION;
		return speed_rev_sec * 60.0;
	};



	void Axis::set_generator(uint64_t pulse)
	{
		generator = pulse;
	};

	int32_t Axis::read_encoder()
	{
		int32_t ret = *encoder_reg; 
		return encoder_reverse ? -ret : ret;
	};
	
	int32_t Axis::read_generator()
	{
		int32_t ret = count_generated; 
		return generator_reverse ? -ret : ret;
	};
		

	void Axis::set_direction(bool dir)
	{
		direction = dir;
	};

	void Axis::set_compare(int32_t cmp)
	{
		*compare_reg = encoder_reverse ? -cmp : cmp;
	};

	int32_t Axis::get_compare()
	{
		return encoder_reverse ? -*compare_reg : *compare_reg;
	};

	bool Axis::internal_direction()
	{
		return direction != generator_reverse;
	};

	void Axis::stop()
	{
		generator = 0;
	};





	void TestDirectionController::init (
		Axis* _axis
	)	
	{
	axis = _axis;
	axis->trigger_callback = do_nothing;
	start_imp = axis->read_encoder();
	proc = switchSched.registry(this, &TestDirectionController::exec);
	};

	void TestDirectionController::exec()
	{
		int32_t TEST_MOVE = 2000;
		int32_t ANSWER_MOVE = TEST_MOVE * (axis->encoder_resolution / axis->generator_resolution);
		int32_t TEST_E = 30;

		stdout.println("PeriodicMoveControllerStart");
		while(1)
		{
			axis->set_speed(100);
			axis->set_direction(Axis::FORWARD);
			axis->set_generator(TEST_MOVE);
			while(axis->generator != 0) schedule_subst();

			msleep(150);
			int32_t moved = axis->read_encoder() - start_imp;

			if 
			(check_range_incl(moved, ANSWER_MOVE - TEST_E, ANSWER_MOVE + TEST_E))
				{
					stdout.print("Test direction: OK!");
					Serial6.print("Test direction: OK!");
				}		
			else if 
			(check_range_incl(moved, -ANSWER_MOVE - TEST_E, -ANSWER_MOVE + TEST_E))
				{
					stdout.print("Test direction: WRONG DIRRECTION!");
					Serial6.print("Test direction: WRONG DIRRECTION!");		
				}
			else if
			(check_range_incl(moved, -TEST_E, TEST_E))
				{
					stdout.print("Test direction: DONT MOVE!");
					Serial6.print("Test direction: DONT MOVE!");
				}
			else 
				{
					stdout.print("Test direction: UNDECLARED ERROR!");
					Serial6.print("Test direction: UNDECLARED ERROR!");
				};
			remove();
		};	
	};

	extern MoveController* current_controller;
	void TestDirectionController::remove()
	{
		stdout.println("TestDirectionControllerRemove");
		axis->stop();
		current_controller = 0;
		if (proc == current_schedee()) exit_subst();
		else switchSched.schedee_set_zombie(proc);
	};





	void HomePositionController::init (
		Axis* _axis
	)	
	{
	axis = _axis;
	axis->stop();
	axis->trigger_callback = do_nothing;
	proc = switchSched.registry(this, &HomePositionController::exec);
	};

	extern int32_t home_position_enc_x;
	extern int32_t home_position_gen_x;
	void HomePositionController::exec()
	{
		msleep(300);
		start_enc = axis->read_encoder();
		start_gen = axis->read_generator();
		stdout.println("HomePositionControllerStart");
		axis->set_speed(1000);
		//axis->set_direction(Axis::FORWARD);
		
		int32_t error_gen = home_position_gen_x - start_gen;	
		int32_t error_enc = home_position_enc_x - start_enc;	
		if (error_enc > 0) 
			{
				axis->set_direction(Axis::FORWARD);
				axis->set_generator((abs(error_enc) + axis->multiplier / 2) / axis->multiplier);
				//axis->set_generator(abs(error));
			}
		else
			{
				axis->set_direction(Axis::BACKWARD);
				axis->set_generator((abs(error_enc) + axis->multiplier / 2) / axis->multiplier);
				//axis->set_generator(abs(error));
			};

		//msleep_subst(200);
		while(!axis->in_position(start_enc, start_gen, 200)) 
		{
			//if (!axis->check_planed_position(start_enc, start_gen, 40000))
			//{debug_print("AllBad"); remove();};
			msleep(10);
		};
		
		remove();	
	};

	extern MoveController* current_controller;
	void HomePositionController::remove()
	{
		stdout.println("HomePositionControllerRemove");
		axis->stop();
		current_controller = 0;
		if (proc == current_schedee()) exit_subst();
		else switchSched.schedee_set_zombie(proc);
	};

	