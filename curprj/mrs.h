#include "genos.h"
#include "utilxx/charbuf.h"

class MitsubishiCommunicator
{
public:

	LinuxFileStream mrs;

	void Open(const char* str)
	{
		mrs.open_out(str);
		mrs.open_in(str);
	};

	void Send(const char* str, uint8_t len)
	{
		mrs.write(str, len);
		//mrs.print(str);
	};

	void exec()
	{
		while(1)
		{

			//dprln((bool)mrs.i);
			//dprln((bool)mrs.o);
			//while(1)			
			//dprln(mrs.available());



			//while(1);
			//delay(10);
			if (mrs.available()) 
				{debug_putchar(mrs.getc());
			dpr("\r\n");};
			//delay(1000);
			yield();
		};
	};
};