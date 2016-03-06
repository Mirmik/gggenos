
#ifndef AUTOM_TERMINAL_H
	#define AUTOM_TERMINAL_H
	
	#include "kernel/diag.h"
	#include "genos/terminal/command_list.h"
	#include "genos/terminal/readline.h"

	#include "genos/iostream/writer.h"
	#include "genos/iostream/reader.h"

	class automTerminal
	{
	private:

		Writer<char>* _o;
		Reader<char>* _i;

		int state;
		command_list comlist;
		
		char str[40];
		int cursor;
		char oldchar;
		
	public:
		
		automTerminal( Writer<char>& o, Reader<char>& i ) 
		: _o(&o), _i(&i) {};
		
		void exec();
		
		void add_command(const char* str, void(*f)(int,char**));	

		void terminal_str_execute();
	};
		
#endif