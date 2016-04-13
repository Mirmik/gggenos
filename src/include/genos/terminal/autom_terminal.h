
#ifndef AUTOM_TERMINAL_H
#define AUTOM_TERMINAL_H
	
	#include <syscontext/syscontext.h>
	#include <genos/kernel/waitserver.h>
	#include <genos/terminal/readline.h>
	#include <genos/terminal/command_list.h>
	
	class automTerminal
	{public:

		static constexpr int NEEDWAIT_YES = 1;
		static constexpr int NEEDWAIT_NO = 0;

		uint8_t state = 0;	
		char oldchar = '\0';
		bool scanmode = false;
		Readline rl;

		//int alghoritm();
		void reset_event();
		void endl_event();
		void exec();

		automTerminal() : state(0), oldchar('\0'), scanmode(false), rl() {};
	};	
	
#endif