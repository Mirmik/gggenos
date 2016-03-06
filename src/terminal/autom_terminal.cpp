
	
	#include "genos/terminal/automTerminal.h"
	//#include <command_list/command_list.h>
	#include "string.h"

		struct argvc_t {
			char ** argv;
			int argc;
		};
		
		#define DROP_SPACE(c); for (;(*c!=0) && (*c==' '); c++);
		#define DROP_OP(c); for (;(*c!=0) && (*c!=' '); c++);
		
		void split (char* temp, argvc_t &a)
		{	a.argc=0;
			while((*temp) !=0) {
				DROP_SPACE(temp);
				if (*temp != 0) {
					a.argv[a.argc]=temp;
				a.argc++;}
				DROP_OP(temp);
				if (*temp!=0) {*temp='\0';temp++;
				}
			}
		}
		

		void automTerminal::terminal_str_execute()
		{
			str[cursor]=0;
			cursor=0;
			state=0;
			
			void* f=0;
			char* argv [10];
			argvc_t a;
			
			a.argv=argv;
			split(str,a);
			
			if (a.argc==0) return;
			//if (!strcmp(a.argv[0],"list")) {comlist_info(o,comlist);return;};
			if (!strcmp(a.argv[0],"about")) {_o->write((char*)"Genos KTerminal\n",15);return;};
			if (!strcmp(a.argv[0],"firmware")) {_o->write((char*)"Firmware was compiled at ", 25);
			//_DATE__ << ' ' << __TIME__ << endl;
			return;};
			
			//find_comand(a.argv[0],f,comlist);
			
			if (f==0) {_o->write((char*)"KT:command not found\n",21); return;}
			else {
				reinterpret_cast<void (*)(int, const char* const*)>(f)(a.argc,a.argv);
			};
			
			
		}
		

		void automTerminal::add_command(const char* str, void(*f)(int,char**))
		{
			comlist.add_command((char*)str,f);
		};
		
		void automTerminal::exec() 
		{
			switch(state)
			{
				case 0:
				_o->write((char*)"machine:", 8);
				state++;
				cursor=0;
				break;
				
				case 1:
				while (_i->available()) 
				{
					char c = static_cast<char>(_i->read()); 
					if ((oldchar=='\n' && c=='\r') || (oldchar=='\r' && c=='\n')) return;
					switch(c)
					{
						case '\n':
						case '\r': _o->write('\n'); terminal_str_execute(); break;
						default: str[cursor++]=c; _o->write(c);							
					};
					
					
					oldchar=c;
				};
	//			wait_autom(i.i);
				break;
			};
		};
				
	