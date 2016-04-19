#include "communication_func.h"
#include "genos/schedproc/automSched.h"

	extern automScheduler automSched;
	Communication communication;

	constexpr uint32_t COMMUNICATION_MANUAL_PULSE = 40;
	constexpr uint32_t COMMUNICATION_ABSOLUTE_POSITION_GET = 50;
	constexpr uint32_t COMMUNICATION_EXIT = 0xFFFF;

	void Communication::exec()
	{
		switch(state)
		{
			case COMMUNICATION_MANUAL_PULSE:
			confmes4(task.message, "8B", "00", "0000");
			task.message_len = 14;
			rsDriver.active(&task);
			wait_autom(&task.flag);
			state = COMMUNICATION_EXIT;
			break;


			case COMMUNICATION_ABSOLUTE_POSITION_GET:
			confmes(task.message, "02", "91");
			task.message_len = 10;
			rsDriver.active(&task);
			wait_autom(&task.flag);
			state = COMMUNICATION_ABSOLUTE_POSITION_GET + 1;
			break;

			case COMMUNICATION_ABSOLUTE_POSITION_GET + 1:
			ret = uint32_protocol_parse(task.answer + 3);
			state = COMMUNICATION_EXIT;
			break;

			case COMMUNICATION_EXIT:
			exit_autom();			
			break;
		};
	};

	void Communication::registry()
	{
		dlist_move_tail(&task.list, &rsDriver.list);
		//task.callback = delegate<void, void*>(&SerialHD6, &HardwareSerialHD::print_answer);
	};


void pulsemode_set()
{
	global_mode = PULSE; 
	communication.state = COMMUNICATION_MANUAL_PULSE;
	wait_child(automSched.registry(&communication, &Communication::exec));
};

void abspos_ret()
{
	communication.state = COMMUNICATION_ABSOLUTE_POSITION_GET;
	wait_child(automSched.registry(&communication, &Communication::exec));
};


int32_t get_ret()
{
	return communication.ret;
};