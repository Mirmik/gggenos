#ifndef GENOS_STREAM_IO
#define GENOS_STREAM_IO

#define STREAM_ENDL "\r\n"

using putc_t  = int(*)(const char);
using getc_t  = int(*)();
using write_t = int(*)(const char*, int);
using read_t  = int(*)(char*, int);
using available_t  = int(*)();

using open_t = int(*)(const char*);
using close_t = int(*)();

#include "string.h"
#include "util/stub.h"

class stream
{
public:
	virtual int putc(const char c) {error_stub();};
	virtual int getc() {error_stub();};
	virtual int available() {error_stub();};

	virtual int write(const char* str, int n) 
	{
		int ret = 0;
		while(n--) {
			ret += putc(*str++);
		};
		return ret;
	};

	virtual int read(char* str, int n)
	{
		int ret = 0;
		while(n--) {
			int c = getc(); if (c < 0) return ret;
			*str++ = static_cast<char>(c);
		};
		return ret; 	
	};
	
	int print(char c) { return putc(c); }
	int print(const char* str) { return write(str, strlen(str)); };
	
	int print(int8_t i, uint8_t base = 10);
	int print(int16_t i, uint8_t base = 10);
	int print(int32_t i, uint8_t base = 10);
	int print(int64_t i, uint8_t base = 10);

	int print(uint8_t i) { print("IOP");
		return print(i, 10);};
	//int print(uint8_t i, uint8_t base = 10);
	//int print(uint16_t i, uint8_t base = 10);
	//int print(uint32_t i, uint8_t base = 10);
	int print(uint64_t i, uint8_t base = 10);

	int printhex(int8_t i);
	int printhex(int16_t i);
	int printhex(int32_t i);
	int printhex(int64_t i);

	int printhex(uint8_t i);
	int printhex(uint16_t i);
	int printhex(uint32_t i);
	int printhex(uint64_t i);

	int print(float f);
	int print(double d);

	template<typename T>
	int println(T var) 
	{
		int ret = 0;
		ret += print(var);
		ret += print(STREAM_ENDL);
		return ret;
	};

	template<typename T>
	int println(T var, uint8_t base) 
	{
		int ret = 0;
		ret += print(var, base);
		ret += print(STREAM_ENDL);
		return ret;
	};

	int println()
	{
		return print(STREAM_ENDL);
	};

	template<typename T>
	int printhexln(T var) 
	{
		int ret = 0;
		ret += printhex(var);
		ret += print(STREAM_ENDL);
		return ret;
	};


	int printptr(void* p)
	{return printhex(reinterpret_cast<size_t>(p));};

	void printdump(void* address, size_t size);

private:
	int printhex_4(uint8_t);
};

static int sys_open(stream*& _strm, const char*) {};
static int sys_close(stream*& _strm) {};

////////////

class direct_stream : public stream
{
private:
	stream* _strm;
public:
	int putc(char c) {return _strm->putc(c);};
	int getc() 		 {return _strm->getc();};
	int write(char* c, int n) {return _strm->write(c, n);};
	int read(char* c, int n) {return _strm->read(c, n);};
	int available() {return _strm->available();};

	int open(const char* c) {return sys_open(_strm, c);};
	int close() {return sys_close(_strm);};
	stream* direct() {return _strm;};
	void direct(stream* _s) {_strm = _s;};
};  

#endif