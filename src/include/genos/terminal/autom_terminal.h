
#ifndef AUTOM_TERMINAL_H
#define AUTOM_TERMINAL_H
	
	#include <syscontext/syscontext.h>
	#include <genos/wait/waitserv.h>
	#include <genos/terminal/readline.h>
	#include <genos/terminal/command_list.h>
	
	class automTerminal
	{public:
		uint8_t state = 0;	
		char oldchar = '\0';
		bool scanmode = false;
		Readline rl;

		void reset_event();
		void endl_event();
		void exec();
	};	
	
#endif