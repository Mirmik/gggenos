#ifndef GENOS_SCHEDEE_BASE
#define GENOS_SCHEDEE_BASE
	
#include "genos/sigslot/delegate.h"
#include "util/bits.h"
#include "util/stub.h"

	namespace SchedeeStatus
	{
		constexpr uint8_t RUN = 0x01;
		constexpr uint8_t WAIT = 0x02;
		constexpr uint8_t ZOMBIE = 0x03;
		constexpr uint8_t STATEMASK = 0x0F;
		constexpr uint8_t EXECUTED = 0x10;
		constexpr uint8_t AUTOMATICCREATED = 0x80;
	};

	struct schedee_state
	{
		uint8_t status;

		uint8_t statemask()
		{
			return bits_mask(status, SchedeeStatus::STATEMASK);
		};

		void run()
		{
			bits_mask_assign(status, SchedeeStatus::RUN, SchedeeStatus::STATEMASK);
		};

		void wait()
		{
			bits_mask_assign(status, SchedeeStatus::WAIT, SchedeeStatus::STATEMASK);			
		};

		void zombie()
		{
			bits_mask_assign(status, SchedeeStatus::ZOMBIE, SchedeeStatus::STATEMASK);			
		};

		bool is_run()
		{
			return statemask() == SchedeeStatus::RUN;
		};

		bool is_wait()
		{
			return statemask() == SchedeeStatus::WAIT;	
		};

		bool is_zombie()
		{
			return statemask() == SchedeeStatus::ZOMBIE;	
		};

		void execute()
		{
			bits_set(status, SchedeeStatus::EXECUTED);
		};

		void unexecute()
		{
			bits_clr(status, SchedeeStatus::EXECUTED);
		};

		bool is_execute()
		{
			return bits_mask(status, SchedeeStatus::EXECUTED);
		};

		void autocreate()
		{
			bits_set(status, SchedeeStatus::AUTOMATICCREATED);
		};

		void unautocreate()
		{
			bits_clr(status, SchedeeStatus::AUTOMATICCREATED);
		};

		bool is_autocreate()
		{
			return bits_mask(status, SchedeeStatus::AUTOMATICCREATED);
		};
	};

	//Базовый класс единицы планирования.
	struct schedee 
	{
		schedee_state state;
		schedee* parent;
	};
	
#endif