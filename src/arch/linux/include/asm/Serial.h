#ifndef GENOS_LINUX_STREAM
#define GENOS_LINUX_STREAM

#include "genos/io/stream.h"
#include <iostream>
#include <fstream>

class LinuxStdStream : public stream
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

class LinuxFileStream : public stream
{
	std::ifstream i;
	std::ofstream o;
public:
	int putc(char c)
	{
		o << c << std::flush;
	};

	int getc()
	{
		char c;
		i >> c;
		return c;
	};

	int available()
	{
		i.rdbuf()->in_avail();
	};

	int open_in(const char* str)
	{
		i.open(str);
	};

	int open_out(const char* str)
	{
		o.open(str);
	};

	int close_in()
	{
		i.close();
	};

	int close_out()
	{
		o.close();
	};
};

#endif