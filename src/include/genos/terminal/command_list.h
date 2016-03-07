

#ifndef COMMAND_LIST_H
	#define COMMAND_LIST_H
	
	#include <genos/datastruct/list.h>
	#include <string.h>
	
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

		int find(const char* str, void(*&f)(int,char**)) 
		{
			command_t* elem;
			list_for_each_entry(elem, &list, lst)
			{
				if (!strcmp(elem->_name, str)) 
				{
					f = elem->_func;
					return 0;
				};
			};
			return -1;
		};	

	};
	
extern command_list central_cmdlist;

#endif