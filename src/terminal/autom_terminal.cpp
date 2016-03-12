
#include "genos/terminal/autom_terminal.h"
#include "genos/decoration.h"
#include "genos/defs.h"
#include "stdlib.h"

void automTerminal::reset_event() 
{
	stdout.println("automTerminal reset_event");
	rl.init();
	state = 0;
};


void automTerminal::endl_event() 
{
	state = 1;
	bool success = false;	
	char* str = rl.get_line();

	if (!strcmp(str, "")) goto _exit;

	if (!strcmp(str, "about"))
	{
		print_about(&stdout);
		success = true;
		goto _exit;
	};


	if (!strcmp(str, "scanmode"))
	{
		scanmode = true;
		stdout.println("ScanMode active");
		success = true;
		goto _exit;	
	};

	if (!strcmp(str, "help"))
	{
		stdout.println("CommandServer commands:");
		stdout.println("TODO");
		stdout.println("Terminal inline commands:");
		stdout.println("about");
		stdout.println("help");
		success = true;
		goto _exit;
	};

	if (!strcmp(str, "terminal_reset"))
	{
		reset_event();
		success = true;
		goto _exit;
	};

	argvc_t a;
	split_argv(str, a);	

	executed_t f;
	if (!central_cmdlist.find(a.argv[0], f))
	{
		f(a.argc, a.argv);
		success = true;
		goto _exit;
	};

	_exit:
	rl.init();
	if (!success) {
		stdout.print("Wrong command: ");
		stdout.println(str);
	};
};

void  automTerminal::exec() 
{
	switch(state)
	{
		case 0:
		print_banner(&stdout);
		print_about(&stdout);
		state=1;
		break;

		case 1:
		stdout.putc('#');
		stdout.print(machine_name);
		stdout.putc(':');
		state=2;
		break;
				
		case 2:
		while (stdin.available()) 
		{
			char c = static_cast<char>(stdin.getc()); 
			
			if (scanmode) 
			{
				stdout.print("F");
				stdout.printhexln(c);
				goto _exit;
			};

			/*if (
				!(
					(c >= 'a' && c<='z') ||
					(c >= 'A' && c<='Z') ||
					(c >= '0' && c<='9') ||
					(c == '\n') || (c == '\r')
				 )
				) return;*/

			if ((oldchar=='\n' && c=='\r') || (oldchar=='\r' && c=='\n')) return;
			switch(c)
			{
				case '\b': 
					rl.backspace(); stdout.putc('\b'); break;
				case '\r': 
				case '\n': 
					stdout.print("\r\n"); endl_event(); return;
				default: 
					stdout.putc(c); 
					if (rl.putc(c) < 0) {
						stdout.print("Readline Error.");
						reset_event(); return;
					}; 
				break;							
			};
				
			//_exit:		
			oldchar=c;
		};
			
		_exit:
		wait_autom(&stdin);
		break;				
	};
};