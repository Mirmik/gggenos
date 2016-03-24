#include "genos/terminal/command_list.h"

command_list central_cmdlist;


void sysexec(const char* src)
	{
		delegate<void,int,char**> d;
		char str[40];
		argvc_t a;
		
		strcpy(str, src);
		split_argv(str, a);	

		if (!central_cmdlist.find(a.argv[0], d))
		{
			d(a.argc, a.argv);
		};

	};
