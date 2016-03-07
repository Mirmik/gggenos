#include "genos/io/stream.h"

inline int stream::printhex_4(uint8_t u)
{
	return u < 0x0A ? putc(u + '0') : putc(u + 'A' - 0x0A) ;
};

int stream::printhex(uint8_t u)
{
	int ret = 0;
	ret += printhex_4((u >> 4) & 0x0F);
	ret += printhex_4( u 	   & 0x0F);
	return ret;
};

int stream::printhex(uint16_t u)
{
	int ret = 0;
	uint8_t* p = reinterpret_cast<uint8_t*>(&u + 1);
	ret += printhex(*--p);
	ret += printhex(*--p);
	return ret;
};

int stream::printhex(uint32_t u)
{
	int ret = 0;
	uint8_t* p = reinterpret_cast<uint8_t*>(&u + 1);
	ret += printhex(*--p);
	ret += printhex(*--p);
	ret += printhex(*--p);
	ret += printhex(*--p);
	return ret;
};

int stream::printhex(uint64_t u)
{
	int ret = 0;
	uint8_t* p = reinterpret_cast<uint8_t*>(&u + 1);
	ret += printhex(*--p);
	ret += printhex(*--p);
	ret += printhex(*--p);
	ret += printhex(*--p);
	ret += printhex(*--p);
	ret += printhex(*--p);
	ret += printhex(*--p);
	ret += printhex(*--p);
	return ret;
};

int stream::printhex(int8_t i) 
{ return printhex(static_cast<uint8_t>(i)); };

int stream::printhex(int16_t i) 
{ return printhex(static_cast<uint16_t>(i)); };

int stream::printhex(int32_t i) 
{ return printhex(static_cast<uint32_t>(i)); };

int stream::printhex(int64_t i) 
{ return printhex(static_cast<uint64_t>(i)); };

#include "genos/gstl/utility.h"
#include "genos/debug/debug_print.h"
int stream::print(int8_t num, uint8_t base)
{
	int ret = 0;
	uint8_t rest;
	char str[9];
	char* ptr = str + 8;
	*ptr = '\0';

	if (num < 0) {ret += putc('-'); num = -num;};

	while(num) 
	{
		rest = num % base;
		*--ptr = rest < 0xA ? rest + '0' : rest + 'A' - 0xA;
		num /= base; 
	};

	ret += print(ptr);
	return ret;
};

int stream::print(int16_t num, uint8_t base)
{
	int ret = 0;
	uint8_t rest;
	char str[17];
	char* ptr = str + 16;
	*ptr = '\0';

	if (num < 0) {ret += putc('-'); num = -num;};

	while(num) 
	{
		rest = num % base;
		*--ptr = rest < 0xA ? rest + '0' : rest + 'A' - 0xA;
		num /= base; 
	};

	ret += print(ptr);
	return ret;
};


int stream::print(int32_t num, uint8_t base)
{
	int ret = 0;
	uint8_t rest;
	char str[33];
	char* ptr = str + 32;
	*ptr = '\0';

	if (num < 0) {ret += putc('-'); num = -num;};

	while(num) 
	{
		rest = num % base;
		*--ptr = rest < 0xA ? rest + '0' : rest + 'A' - 0xA;
		num /= base; 
	};

	ret += print(ptr);
	return ret;
};


int stream::print(int64_t num, uint8_t base)
{
	int ret = 0;
	uint8_t rest;
	char str[65];
	char* ptr = str + 64;
	*ptr = '\0';

	if (num < 0) {ret += putc('-'); num = -num;};

	while(num) 
	{
		rest = num % base;
		*--ptr = rest < 0xA ? rest + '0' : rest + 'A' - 0xA;
		num /= base; 
	};

	ret += print(ptr);
	return ret;
};


int stream::print(uint64_t num, uint8_t base)
{
	int ret = 0;
	uint8_t rest;
	char str[65];
	char* ptr = str + 64;
	*ptr = '\0';

	while(num) 
	{
		rest = num % base;
		*--ptr = rest < 0xA ? rest + '0' : rest + 'A' - 0xA;
		num /= base; 
	};

	ret += print(ptr);
	return ret;
};



void stream::printdump(void* address, size_t size)
	{
		uint8_t* dump = (uint8_t*) address; 
		uint32_t addr_tmp = (size_t) address;
		uint32_t i = 0;
		uint32_t mark = 0;
		
		for (i = 0; i < size; i++)
		{
			
			if (mark == 0)
			{
				printhex(addr_tmp);
				print(": ");
			}
			
			printhex(dump[i]);
			
			if ( mark == 7 )
			{
				print("|");
				mark++;
			}
			else if ( mark == 15 )
			{
				print(STREAM_ENDL);
				mark = 0;
				
				addr_tmp += 0x10;
			}
			else
			{
				print(" ");
				mark++;
			}
		} 
		print(STREAM_ENDL);
	}

















