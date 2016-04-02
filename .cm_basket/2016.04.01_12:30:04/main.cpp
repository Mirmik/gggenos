#include "hal/arch.h"

void emergency_stop()
{
};

void setup();
void loop();


int main(){
	setup();
	while(1) loop();
};

//#include "stdlib.h"
void setup()
{
	arch_init();
};


void loop()
{
};











