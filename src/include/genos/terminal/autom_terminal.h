
#ifndef AUTOM_TERMINAL_H
#define AUTOM_TERMINAL_H
	
	#include <genos/iostream/reader.h>
	#include <genos/iostream/writer.h>
	#include <genos/wait/waitserv.h>
	
	class automTerminal
	{public:
		Reader<char>* i;
		Writer<char>* o;
		uint8_t state;	
		char oldchar;
		
		automTerminal(Writer<char>& bo, Reader<char>& bi) : o(bo) ,  i(bi), state(0), oldchar('\0') {};
		
		void set(Writer<char>& bo, Reader<char>& bi)
		{
			o.set(bo);  i.set(bi);
		};
		
		void exec() 
		{
			switch(state)
			{
				case 0:
				o->print("machine:");;
				state++;
				break;
				
				case 1:
				while (i.available()) 
				{
					char c = static_cast<char>(i.read()); 
					if ((oldchar=='\n' && c=='\r') || (oldchar=='\r' && c=='\n')) return;
					switch(c)
					{
						case '\n':
						case '\r': o->write('\n'); break;
						default: str[cursor++]=c; o->write(c);							
					};
					
					
					oldchar=c;
				};
				wait_autom(i.i);
				break;
				
				
				
				
				
				
				
			};
			
			
			
			
		};
		
		
		
		
		
		
		
		
		
		
		
		
		
	};	
	
#endif