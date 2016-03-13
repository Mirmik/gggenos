

#ifndef COMMAND_LIST_H
	#define COMMAND_LIST_H

	#include "util/stub.h"	
	#include <genos/datastruct/list.h>
	#include "genos/sigslot/delegate.h"
	#include <string.h>
	#include <genos/io/stream.h>

	typedef void(*executed_t)(int,char**);

	struct argvc_t {
		int argc;
		char * argv[10];
	};	

	static void split_argv(char* c, argvc_t& a)
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
		delegate<void, int, char**> _d;
		const char* _name;
		list_head lst;
		public:
		command_t(const char* name, delegate<void, int, char**> d) : _d(d), _name(name) {};
	};
	
	class command_list{
		
		list_head list;

		public:
		command_list() : list() {};
		
		void add(const char* str, delegate<void, int, char**> d) 
		{
			command_t* cmd = new command_t(str, d); 
			list_add(&cmd->lst, &list);
		};	

		void add(const char* str, void(*f)(int,char**)) 
		{
			delegate<void, int, char**>d = f;
			command_t* cmd = new command_t(str, d); 
			list_add(&cmd->lst, &list);
		};	


		void add(const char* str, void(*f)()) 
		{
			delegate<void, int, char**>d = (void(*)(int,char**)) f;
			command_t* cmd = new command_t(str, d); 
			list_add(&cmd->lst, &list);
		};

		template<typename T>
		void add(const char* str, T* a, void (T::*ptr)(int, char**))
		{
			delegate<void, int, char**>d = delegate<void, int, char**>(a, ptr);
			command_t* cmd = new command_t(str, d); 
			list_add(&cmd->lst, &list);
		};

		template<typename T>
		void add(const char* str, T* a, void (T::*ptr)())
		{
			delegate<void, int, char**>d = delegate<void, int, char**>(a, (void (T::*)(int, char**))ptr);
			command_t* cmd = new command_t(str, d); 
			list_add(&cmd->lst, &list);
		};

		int find(const char* str,  delegate<void, int, char**> &outd) 
		{
			outd = (executed_t) do_nothing;
			command_t* elem;
			list_for_each_entry(elem, &list, lst)
			{
				if (!strcmp(elem->_name, str)) 
				{
					outd = elem->_d;
					return 0;
				};
			};
			return -1;
		};	

		int try_execute(char* c, delegate<void, int, char**> &outd)
		{
			argvc_t a;
			split_argv(c, a);
			if (a.argc == 0) return -1;
			if (find(a.argv[0], outd)) return -2;
			return 0;
		};

		int print_list(stream& strm)
		{
			command_t* elem;
			list_for_each_entry(elem, &list, lst)
			{
				strm.println(elem->_name);
			};
		};

	};
	
extern command_list central_cmdlist;

#endif