#ifndef COMMUNICATION_FUNCTIONS
#define COMMUNICATION_FUNCTIONS

#include "asm/SerialHD.h"
#include "movecontroller.h"
#include "mitsuprotocol.h"

class Communication
{
public:
SerialHDDriver::Task task;
int state = 0;
int32_t ret;
char mes[64];
int meslen;
//uint16_t speed = 255;
//bool direction = false;
	void registry();
	void exec();
};

extern SerialHDDriver rsDriver;
extern Communication communication;

void pulsemode_set();
void abspos_ret();

int32_t get_ret();

#endif