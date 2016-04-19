#ifndef GENOS_LINUX_STREAM
#define GENOS_LINUX_STREAM

#include "genos/io/stream.h"
#include <iostream>

class LinuxStream : public stream
{
	int putc(char c)
	{
		std::cout << c;
	};

	int getc()
	{
		char c;
		std::cin >> c;
		return c;
	};
};

#endif