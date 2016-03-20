#ifndef MITSUPROTOCOL
#define MITSUPROTOCOL

#include "genos/kernel/waitserver.h"
#include "syscontext/syscontext.h"
#include "asm/Serial_half_duplex.h"

extern Serial_HD_simple SerialHD1;

int32_t parse_int32_bias7(char*);


struct driver_info32_element
{
public:
	volatile int32_t data;
	int32_t (* const parse)(char*);
	const char command[2];
	const char datano[2];
public:
	void update_start(char stantion_no);
	int update_end();
	void wait_operation();
};

class MR_4J_A
{
public:
	char stantion_num;
	
	driver_info32_element ABS = 
	{
		0,
		&parse_int32_bias7,
		{'0','1'},
		{'8','C'}
	};

	driver_info32_element ERR = 
	{
		0,
		&parse_int32_bias7,
		{'0','1'},
		{'8','2'}
	};

	driver_info32_element CYC = 
	{
		0,
		&parse_int32_bias7,
		{'0','1'},
		{'8','B'}
	};

	driver_info32_element pPIN0 = 
	{
		0,
		&parse_int32_bias7,
		{'1','2'},
		{'0','0'}
	};
	
	driver_info32_element pPIN1 = 
	{
		0,
		&parse_int32_bias7,
		{'1','2'},
		{'0','1'}
	};
	
	driver_info32_element pPIN2 = 
	{
		0,
		&parse_int32_bias7,
		{'1','2'},
		{'0','2'}
	};

	MR_4J_A(char s) : stantion_num(s) {};	
};

class Updater
{
public:
int state = 0;
char mes[20];
	void exec();
};




void checksum_declare(char* message_buf, char term );

/*void message_generate(
	char* message_buf;
	int st_number;
	uint8_t command_f;
	uint8_t command_s;
	uint8_t dataNo_f;
	uint8_t dataNo_s;
	char* data;
	int data_len;
	);*/

class Driver
{
public:
int state = 0;
uint8_t rs485flag = 0;
char mes[20];
//char* mes = "x201x01x00";
	
	void exec();
	void reinit() {state = 0;};

} ;

extern Driver driver;
extern Updater updater;

extern MR_4J_A servo0;


#endif