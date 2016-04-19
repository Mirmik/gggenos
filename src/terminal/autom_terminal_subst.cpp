#include "genos/terminal/autom_terminal.h"
#include "genos/decoration.h"
#include "genos/defs.h"
#include "stdlib.h"
#include "util/vt102/vt102.h"

void automTerminal::reset_event() 
{
	stdout.println("automTerminal reset_event");
	rl.init();
	state = 0;
};


void automTerminal::endl_event() 
{
	argvc_t a;
	state = 1;
	delegate<void,int,char**> d;
	bool success = false;	
	char* str = rl.get_line();

	if (!strcmp(str, "")) goto _quiet_exit;

	if (!strcmp(str, "about"))
	{
		print_about(&stdout);
		success = true;
		goto _exit;
	};

	if (!strcmp(str, "banner"))
	{
		print_banner(&stdout);
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
		central_cmdlist.print_list(stdout);
		stdout.println("Terminal inline commands:");
		stdout.println("about");
		stdout.println("banner");
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

	a = split_argv(str);	

	if (!central_cmdlist.find(a.argv[0], d))
	{
		d(a.argc, a.argv);
		success = true;
		goto _exit;
	};

	_exit:
	if (!success) {
		stdout.print("Wrong command: ");
		stdout.println(str);
	};
	_quiet_exit:
	rl.init();
};

void  automTerminal::exec() 
{

while(1)

{
	switch(state)
	{
		case 0:
	//dprhex((uint32_t)&stdout);
			print_banner(&stdout);
			print_about(&stdout);
			state=1;
	//dpr("here");
			break;

		case 1:
			stdout.putc('#');
			stdout.print(machine_name.c_str());
			stdout.putc(':');
			state=2;
//	dpr("here");
			break;
				
		case 2:
			while (stdin.available()) 
			{
				char c = static_cast<char>(stdin.getc()); 
			
				if (scanmode) 
				{
					stdout.print("F");
					stdout.printhexln((uint8_t)c);
					goto _exit;
				};

				if ((oldchar=='\n' && c=='\r') || (oldchar=='\r' && c=='\n')) goto _unwait_exit;
				switch(c)
				{
					case '\b': 
						if (rl.backspace()) 
						{ 
							stdout.print(VT102_LEFT);
							stdout.print(VT102_DEL);
						};
						break;
					case '\r': 
					case '\n': 
						stdout.print("\r\n"); endl_event(); goto _unwait_exit;
					default: 
						stdout.putc(c); 
						if (rl.putc(c) < 0) {
							stdout.print("\n\rReadline Error.");
							reset_event(); goto _unwait_exit;
						}; 
					break;							
				};
						
				oldchar=c;
			};
				
			_exit:
			wait_subst(&stdin);

			_unwait_exit:
			break;				
	};

};

};