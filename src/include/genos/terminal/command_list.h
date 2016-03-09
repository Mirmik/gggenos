

#ifndef COMMAND_LIST_H
	#define COMMAND_LIST_H

	#include "util/stub.h"	
	#include <genos/datastruct/list.h>
	#include <string.h>

	typedef void(*executed_t)(int,char**);
	
	struct argvc_t {
		char ** argv;
		int argc;
	};	

	static void split_argv(char* c, argvc_t a)
	{
		int state = 0;
		char* ptr = c;
		a.argc=0;

		while(*ptr != 0)
		{
			switch(state)
			{
				case 0: 
					switch(*ptr)
					{
						case ' ': 
							break;
						default: 
							state = 1; 
							a.argv[a.argc] = ptr; 
							a.argc++; 
							break;
					}
					break;
				case 1:
					switch(*ptr)
					{ 
						case ' ': 
							state = 0;
							*ptr = '\0';
							break;
						default:
							break;
					};
					break;
			};
		++ptr;
		};
	};

	class command_t{
		public:
		void(*_func)(int,char**);
		const char* _name;
		list_head lst;
		public:
		command_t(const char* name, void(*func)(int,char**)) : _func(func), _name(name) {};
	};
	
	class command_list{
		
		list_head list;

		public:
		command_list() : list() {};
		
		void add(const char* str, void(*f)(int,char**)) 
		{
			command_t* cmd = new command_t(str, f); 
			list_add(&cmd->lst, &list);
		};	

		int find(const char* str, void(*&out_f)(int,char**)) 
		{
			out_f = (executed_t) do_nothing;
			command_t* elem;
			list_for_each_entry(elem, &list, lst)
			{
				if (!strcmp(elem->_name, str)) 
				{
					out_f = elem->_func;
					return 0;
				};
			};
			return -1;
		};	

	};
	
extern command_list central_cmdlist;

#endif