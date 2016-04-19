#include "genos/terminal/command_list.h"

command_list central_cmdlist;


void sysexec(const char* src)
	{
		delegate<void,int,char**> d;
		char str[40];
		
		
		strcpy(str, src);
		argvc_t a = split_argv(str);	

		if (!central_cmdlist.find(a.argv[0], d))
		{
			d(a.argc, a.argv);
		};

	};
