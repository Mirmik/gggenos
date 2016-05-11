#ifndef NETPARSER_H
#define NETPARSER_H

class netparser
{
	Readline<64> rl;
	uint16_t len;
	int mode;

public:
	netparser() : mode(0) {};

	void putc(char c)
	{
		switch(mode)
		{
			case 0:
				rl.init();
			case 1:
				rl.putc(c);
				if (rl.length() == 2) 
				{
					mode = 2;
					len = rl[0] << 8 + rl[1];
				};
				break;
			case 2:
				rl.putc(c);
				if (rl.length() == len) 
				{
					receive_handler();
					mode = 0;
				};
				break;
		};
		return;
	};

};

#endif