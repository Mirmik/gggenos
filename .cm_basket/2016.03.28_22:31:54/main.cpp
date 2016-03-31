#include "hal/arch.h"
#include "genos/debug/debug.h" 
#include "genos/time/sysclock.h"
#include "genos/debug/delay.h"
#include "hal/gpio.h"
#include "genos/kernel/waitserver.h"
#include "asm/Serial.h"
#include "genos/schedproc/automSched.h"

#include "configure.h"
#include "stm32f4xx_rcc.h"
#include "asm/SerialHD.h"
#include "util/ascii_codes.h"
#include "stdlib.h"

#include "genos/schedproc/automSched.h"
#include "genos/terminal/autom_terminal.h"
#include "genos/terminal/command_list.h"

#define SPEED_DECREASE 1000
#define ENCODER_RESOLUTION 50000
#define PULSE_RESOLUTION 50000

uint32_t acceleration = 150;
uint8_t pulse_direction = 1;

enum GlobalMode
{
	JOG = 0,
	POSITIONING = 1,
	PULSE = 2
};

GlobalMode global_mode = PULSE;


automScheduler automSched;
automTerminal automTerm;

void emergency_stop()
{
	usart2_interrupt_disable();
	usart6_interrupt_disable();
	
	debug_print("EMERGENCY_STOP");
};

void stop()
{

	debug_panic("emergstp");
};


void print_mode()
{
	debug_printdec_uint32(global_mode);dln;
};


void setup();
void loop();

int main(){
	setup();
	while(1) loop();
};


class A
{
int state = 0;
TimWaiter t;

public:
	void exec()
	{
		switch(state)
		{
			case 0:
				gpio_port_tgl_pin(GPIOD,13);
				state = 1;
//				debug_printdec_uint32(millis());dln;
				msleep_autom_bias(&t, 100);
				break;
			case 1:
				gpio_port_tgl_pin(GPIOD,12);
				msleep_autom_bias(&t, 100);
				state = 0;
				break;
		};
	};
};

A a;

HardwareSerial Serial2;
HardwareSerialHD SerialHD6;
SerialHDDriver rsDriver;

syscontext syscntxt;

void hilohilo_byte_to_symbol(uint32_t num, char* symb)
{
	uint8_t lll = num & 0x0000000F;
	uint8_t llh = (num & 0x000000F0) >> 4; 
	uint8_t lhl = (num & 0x00000F00) >> 8;
	uint8_t lhh = (num & 0x0000F000) >> 12; 
	uint8_t hll = (num & 0x000F0000) >> 16;
	uint8_t hlh = (num & 0x00F00000) >> 20; 
	uint8_t hhl = (num & 0x0F000000) >> 24;
	uint8_t hhh = (num & 0xF0000000) >> 28; 
	
	*(symb + 7) = byte_to_hexascii(lll);
	*(symb + 6) = byte_to_hexascii(llh);
	*(symb + 5) = byte_to_hexascii(lhl);
	*(symb + 4) = byte_to_hexascii(lhh);
	*(symb + 3) = byte_to_hexascii(hll);
	*(symb + 2) = byte_to_hexascii(hlh);
	*(symb + 1) = byte_to_hexascii(hhl);
	*(symb)     = byte_to_hexascii(hhh);
};


void hilo_byte_to_symbol(uint16_t num, char* symb)
{
	uint8_t ll = num & 0x000F;
	uint8_t lh = (num & 0x00F0) >> 4; 
	uint8_t hl = (num & 0x0F00) >> 8;
	uint8_t hh = (num & 0xF000) >> 12; 
	*(symb + 3) = byte_to_hexascii(ll);
	*(symb + 2) = byte_to_hexascii(lh);
	*(symb + 1) = byte_to_hexascii(hl);
	*(symb)     = byte_to_hexascii(hh);
};


void lo_byte_to_symbol(uint8_t num, char* symb)
{
	uint8_t l = num % 0x10;
	uint8_t h = num >> 4; 
	*(symb + 1) = byte_to_hexascii(l);
	*(symb)     = byte_to_hexascii(h);
};

void checksum_declare(char* message_buf, char term)
{
	char* ptr = message_buf;
	int sum = 0;
	assert(*ptr == 1);
	do {
		ptr++;
		sum += *ptr;
	} while(*ptr != 3);
	lo_byte_to_symbol(sum, ++ptr);
};

void print_encoder()
{
	stdout.println((int32_t)TIM5->CNT);
};


void confmes(char* mes, const char* com, const char* datano)
{
		mes[0] = 1;
		mes[1] = '2';
		mes[2] = com[0];
		mes[3] = com[1];
		mes[4] = 2;
		mes[5] = datano[0];
		mes[6] = datano[1];
		mes[7] = 3;
		mes[10] = 0;
		checksum_declare(mes, 3);
};


void confmes8(char* mes, const char* com, const char* datano, const char* data)
{
		mes[0] = 1;
		mes[1] = '2';
		mes[2] = com[0];
		mes[3] = com[1];
		mes[4] = 2;
		mes[5] = datano[0];
		mes[6] = datano[1];
		memcpy(&mes[7], data, 8);
		mes[15] = 3;
		mes[18] = 0;
		checksum_declare(mes, 3);
};

void confmes4(char* mes, const char* com, const char* datano, const char* data)
{
		mes[0] = 1;
		mes[1] = '2';
		mes[2] = com[0];
		mes[3] = com[1];
		mes[4] = 2;
		mes[5] = datano[0];
		mes[6] = datano[1];
		memcpy(&mes[7], data, 4);
		mes[11] = 3;
		mes[14] = 0;
		checksum_declare(mes, 3);
};


class ManualRS
{
int state;
char mes[20];
int mes_len;
public:

	void exec()
	{
		SerialHD6.configure_session(mes , mes_len, 3);
		SerialHD6.callback = delegate_mtd(this, &ManualRS::callback);
		SerialHD6.start_session();
		wait_autom(&SerialHD6.flag);
	};

	void callback(void*)
	{
		stdout.write(SerialHD6.answer, SerialHD6.answer_len); 
		stdout.println();
	};

	void sesconf(int i, char** c)
	{
		if (i!=3) {stdout.println("Err:NumP error");return;};
		if (strlen(c[1])!=2) {stdout.println("Err:Len[1]!=2");return;};
		if (strlen(c[2])!=2) {stdout.println("Err:Len[2]!=2");return;};
		
		confmes(mes, c[1], c[2]);

		mes_len = 10;
		stdout.println(mes);
		exec();
	};

	void sesconf8(int i, char** c)
	{
		if (i!=4) {stdout.println("Err:NumP error");return;};
		if (strlen(c[1])!=2) {stdout.println("Err:Len[1]!=2");return;};
		if (strlen(c[2])!=2) {stdout.println("Err:Len[2]!=2");return;};
		if (strlen(c[3])!=8) {stdout.println("Err:Len[3]!=8");return;};
		
		confmes8(mes, c[1], c[2], c[3]);

		mes_len = 18;
		stdout.println(mes);
		exec();
	};

	void sesconf4(int i, char** c)
	{
		if (i!=4) {stdout.println("Err:NumP error");return;};
		if (strlen(c[1])!=2) {stdout.println("Err:Len[1]!=2");return;};
		if (strlen(c[2])!=2) {stdout.println("Err:Len[2]!=2");return;};
		if (strlen(c[3])!=4) {stdout.println("Err:Len[3]!=4");return;};
		
		confmes4(mes, c[1], c[2], c[3]);
		mes_len = 14;
		stdout.println(mes);
		exec();
	};

} rsManual;

class JOG_Operation
{
public:
SerialHDDriver::Task task;
	
int state;
char mes[50];
int meslen;
uint16_t speed = 255;
bool direction = false;

public:

	void registry()
	{
		dlist_move_tail(&task.list, &rsDriver.list);
		//task.callback = delegate<void, void*>(&SerialHD6, &HardwareSerialHD::print_answer);
	};

	void exec()
	{
		char temp[10];
		switch(state)
		{

			case -1:
			//stdout.print("step one:");

			confmes4(task.message, "8B", "00", "0000");
			task.message_len = 14;
			rsDriver.active(&task);
			wait_autom(&task.flag);
			state = 0;
			break;


			case 0:
			//stdout.print("step one:");

			confmes4(task.message, "8B", "00", "0001");
			task.message_len = 14;
			rsDriver.active(&task);
			wait_autom(&task.flag);
			state = 1;
			break;

			case 1:
			//stdout.println(task.answer);
			//stdout.print("step two:");
			//while(1);
			assert (speed <= SPEED_DECREASE);
			hilo_byte_to_symbol(speed, temp);	
			confmes4(task.message, "A0", "10", temp);
			task.message_len = 14;
			rsDriver.active(&task);
			wait_autom(&task.flag);
			state = 2;
			break;

			case 2:
			hilohilo_byte_to_symbol(acceleration, temp);
			confmes8(task.message, "A0", "11", temp);
			task.message_len = 18;
			rsDriver.active(&task);
			wait_autom(&task.flag);
			state = 3;
			break;


			case 3:
			//stdout.println(task.answer);
			//stdout.print("step four:");
			if (direction == true)
			confmes8(task.message, "92", "00", "00000807");
			else 
			confmes8(task.message, "92", "00", "00001007"); 
			task.message_len = 18;
			rsDriver.active(&task);
			wait_autom(&task.flag);
			state = 4;
			break;

			case 4:
			//stdout.println(task.answer);
			exit_autom();
			break;
		};
	};

} jog_operation;






class Position_Operation
{
public:
SerialHDDriver::Task task;
	
int state;
char mes[50];
int meslen;
uint32_t pos = 255;
uint32_t speed = 255;
bool direction = false;

enum DIRMODE {FORWARD, BACKWARD} dir;
enum COMMODE {ENCPULSE, COMPULSE} mode;

public:

	void registry()
	{
		dlist_move_tail(&task.list, &rsDriver.list);
		//task.callback = delegate<void, void*>(&SerialHD6, &HardwareSerialHD::print_answer);
	};

	void exec()
	{
		char temp[20];
		switch(state)
		{

			case -1:
			stdout.println("step0");
			confmes4(task.message, "8B", "00", "0000");
			task.message_len = 14;
			rsDriver.active(&task);
			wait_autom(&task.flag);
			state = 0;
			break;


			case 0:
			stdout.println("step0");
			confmes4(task.message, "8B", "00", "0002");
			task.message_len = 14;
			rsDriver.active(&task);
			wait_autom(&task.flag);
			state = 1;
			break;


			case 1:
			stdout.println("step1");
			hilo_byte_to_symbol(speed, temp);
			confmes4(task.message, "A0", "10", temp);
			task.message_len = 14;
			rsDriver.active(&task);
			wait_autom(&task.flag);
			state = 2;
			break;


			case 2:
			stdout.println("step2");	
			//stdout.print("step three");
			hilohilo_byte_to_symbol(acceleration, temp);
			confmes8(task.message, "A0", "11", temp);
			task.message_len = 18;
			rsDriver.active(&task);
			wait_autom(&task.flag);
			state = 3;
			break;


			case 3:
			stdout.println("step3");
			//stdout.print("step three");
			hilohilo_byte_to_symbol(pos, temp);
			confmes8(task.message, "A0", "20", temp);
			debug_write(temp,8);
			task.message_len = 18;
			rsDriver.active(&task);
			wait_autom(&task.flag);
			state = 4;
			break;


			case 4:
			stdout.println("step5");
			strcpy(temp, "0000");
			if (mode == ENCPULSE) temp[1] = '1';
			else temp[1] = '0';
			if (dir == FORWARD) temp[3] = '1';
			else temp[3] = '0';
			confmes4(task.message, "A0", "21", temp);
			task.message_len = 14;
			rsDriver.active(&task);
			wait_autom(&task.flag);
			state = 5;
			break;

			case 5:
			stdout.println("step4");
			//stdout.print("step four");
			//if (direction == true)
			//confmes8(task.message, "92", "00", "00000807");
			//else 
			confmes8(task.message, "92", "00", "00000007"); 
			task.message_len = 18;
			rsDriver.active(&task);
			wait_autom(&task.flag);
			state = 6;
			break;

			case 6:
			stdout.println("step6");
			confmes4(task.message, "A0", "40", "1EA5");
			task.message_len = 14;
			rsDriver.active(&task);
			wait_autom(&task.flag);
			state = 7;
			break;

			case 7:
			exit_autom();
			break;
		};
	};

} position_operation;



void pseudohome()
{

	sysexec("pos 100000 1");	
};


void reg_pos_enc(int n, char** c){
	if (n != 3) {stdout.println("Err:NumP error");return;};
	//if (n == 1) jog_operation.speed = 200;
	//stdout.print(c[1]);
	position_operation.pos = atoll(c[1]);

	if (!strcmp(c[2], "0")) position_operation.dir = Position_Operation::FORWARD;
	else if (!strcmp(c[2], "1")) position_operation.dir = Position_Operation::BACKWARD;
	else {stdout.println("Err: wrong [2]");return;};

	char temp[20];
	hilohilo_byte_to_symbol(position_operation.pos, temp);

	if (global_mode != POSITIONING)
	{
		global_mode = POSITIONING; 
		position_operation.state = -1;
	}
	else
	{
		position_operation.state = -1;
	};
	position_operation.mode = Position_Operation::ENCPULSE;	
	automSched.registry(&position_operation, &Position_Operation::exec);
};


void reg_pos_com(int n, char** c){
	if (n != 3) {stdout.println("Err:NumP error");return;};
	//if (n == 1) jog_operation.speed = 200;
	//stdout.print(c[1]);
	position_operation.pos = atoll(c[1]);

	if (!strcmp(c[2], "0")) position_operation.dir = Position_Operation::FORWARD;
	else if (!strcmp(c[2], "1")) position_operation.dir = Position_Operation::BACKWARD;
	else {stdout.println("Err: wrong [2].");return;};

	char temp[20];
	hilohilo_byte_to_symbol(position_operation.pos, temp);

	if (global_mode != POSITIONING)
	{
		global_mode = POSITIONING; 
		position_operation.state = -1;
	}
	else
	{
		position_operation.state = -1;
	};
	position_operation.mode = Position_Operation::COMPULSE;
	automSched.registry(&position_operation, &Position_Operation::exec);
};


void pos_speed(int n, char** c){
	if (n != 2) {stdout.println("Err:NumP error");return;};

	uint32_t speed = atol(c[1]);
	if (speed > SPEED_DECREASE) {speed = SPEED_DECREASE; debug_print("speed decrease ");
	debug_printdec_uint32(SPEED_DECREASE); dln;};
	position_operation.speed = speed;	
};


void pos_acc(int n, char** c){
	if (n != 2) {stdout.println("Err:NumP error");return;};
	acceleration = atol(c[1]);	
};

void reg_jog(int n, char** c){
	if (n != 2) {stdout.println("Err:NumP error");return;};
	//if (n == 1) jog_operation.speed = 200;

	uint32_t speed = atoi(c[1]);

	if (speed > SPEED_DECREASE) {speed = SPEED_DECREASE; debug_print("speed decrease ");
	debug_printdec_uint32(SPEED_DECREASE); dln;};

	jog_operation.speed = speed;

	if (global_mode != JOG)
	{
		debug_print("set jog\r\n");
		global_mode = JOG; 
		jog_operation.state = -1;
	}
	else
	{
		debug_print("not set jog\r\n");
		jog_operation.state = 1;
	};
	automSched.registry(&jog_operation, &JOG_Operation::exec);
};

void reg_jog_direction(int n, char** c){
	if ((n != 2 && n!=1)) {stdout.println("Err:NumP error");return;};
	
	if (n==1) {stdout.print((uint64_t)jog_operation.direction); return;};

	if ((strcmp(c[1], "0")) && (strcmp(c[1], "1")))  {stdout.println("dirrwrong");return;};

	
	//return;
	jog_operation.direction = atoi(c[1]);
	if (global_mode == JOG)
	{
		jog_operation.state = 1;
		automSched.registry(&jog_operation, &JOG_Operation::exec);
	}
	else
	{
		return;
	};
	
};

#include "stdlib.h"
void restart_jog(int n, char** c){
//	if (n!=2) {stdout.println("Err:NumP error");return;};
	
//	jog_operation.state = 1;
//	jog_operation.speed = atoi(c[1]);
};

extern int32_t impulse_count;
void impulse(int n, char** c)
{
	if (n!=2) {stdout.println("NumpErr");};	
	impulse_count = atol(c[1]);
};

void freq(int n, char** c)
{
	if (n!=2) {stdout.println("NumpErr");};	
	assert( atol(c[1]) >200);
	TIM4->ARR = atol(c[1]);
};


#define TIM4_TPS 8400000
void pulse_speed(int n, char** c)
{
	float speed_rev_min;
	float speed_rev_sec;
	float pps;

	if (n!=2) {stdout.println("NumpErr");};	
	speed_rev_min = atol(c[1]);
	speed_rev_sec = speed_rev_min / 60;
	pps = speed_rev_sec * ENCODER_RESOLUTION;

	int res = TIM4_TPS / pps * 4 * 2 * 311 / 200;
	debug_printdec_uint32(res);

	TIM4->ARR = res;
};


void test_task()
{
char mes[20];
	char* ptr = mes; 
	*ptr++ = 1;
	*ptr++ = '2';
	*ptr++ = '0';
	*ptr++ = '1';
	*ptr++ = 2;
	*ptr++ = '0';
	*ptr++ = '0';
	*ptr++ = 3;
	checksum_declare(mes, 3);
//	stdout.write(mes, 10);
	//debug_print(mes);
	SerialHD6.configure_session(mes,10,3);
	SerialHD6.start_session();

	wait_autom(&SerialHD6.flag);
	//delay(100);
};

class Updater
{
int state;
public:
	SerialHDDriver::Task task;
	void exec()
	{
		char mes[40];
		switch(state)
		{
			case 0:
				dlist_move_tail(&task.list, &rsDriver.list);
				//task.callback = delegate<void, void*>(&SerialHD6, &HardwareSerialHD::print_answer);
				state = 1;
			break;

			case 1:
				confmes(task.message, "01", "00");
				task.message_len = 10;
				wait_autom(&task.flag);
				rsDriver.active(&task);
				state = 1;
			break;
		};
	};


} updater;


void updater_start()
{
	automSched.registry(&updater, &Updater::exec);
};

void rs485maxerror()
{
	stdout.print((uint64_t)rsDriver.max_error);	
};

void* operator new(size_t, void* ptr) {return ptr;};
void global_constructor_stub()
{
	new(&a) A;
	new(&automSched) automScheduler;
	new(&automTerm) automTerminal;
	new(&waitserver) WaitServer;
	new(&Serial2) HardwareSerial;
	new(&SerialHD6) HardwareSerialHD;
	new(&syscntxt) syscontext;
	new(&central_cmdlist) command_list;
	new(&rsDriver) SerialHDDriver;	
	new(&updater) Updater;
	new(&jog_operation) JOG_Operation;
	new(&position_operation) Position_Operation;
};


void setup(){
	global_constructor_stub();
	arch_init();
	project_configure();
	
	current_syscontext(&syscntxt);
	stdout.direct(&Serial2);
	stdin.direct(&Serial2);

	Serial2.usart = USART2;

	//debug_print("JJJORKEY!!!");

	SerialHD6.usart = USART6;
	SerialHD6.changedir_port = GPIOC;
	SerialHD6.changedir_pin = GPIO_Pin_8;
	  gpio_port_set_mask(GPIOC, GPIO_Pin_8);

	//debug_print("setup");
	usart2_interrupt_enable();
	usart6_interrupt_enable();
	usart2_rx_interrupt_enable();

	global_mode = PULSE;

	central_cmdlist.add("testrs", test_task);	
	central_cmdlist.add("rs", &rsManual, &ManualRS::sesconf);
	central_cmdlist.add("rs8", &rsManual, &ManualRS::sesconf8);
	central_cmdlist.add("rs4", &rsManual, &ManualRS::sesconf4);

	central_cmdlist.add("encoder", print_encoder);
	central_cmdlist.add("jog", reg_jog);
	central_cmdlist.add("jogdir", reg_jog_direction);
	
	central_cmdlist.add("pos", reg_pos_com);
	central_cmdlist.add("encpos", reg_pos_enc);
	central_cmdlist.add("mode", print_mode);
	
	central_cmdlist.add("posspeed", pos_speed);
	central_cmdlist.add("posacc", pos_acc);
	
	central_cmdlist.add("impulse", impulse);
	central_cmdlist.add("freq", freq);
	central_cmdlist.add("speed", pulse_speed);
		

	central_cmdlist.add("pseudohome", pseudohome);

//	central_cmdlist.add("pulsemode", reg_pulsemode);
	
	central_cmdlist.add("rs485maxerror", rs485maxerror);
//	central_cmdlist.add("restart", restart_jog);
	jog_operation.registry();
	position_operation.registry();

//	central_cmdlist.add("stop", stop);
	central_cmdlist.add("updater", updater_start);


	automSched.registry(&a, &A::exec);
	automSched.registry(&automTerm, &automTerminal::exec);
	automSched.registry(&rsDriver, &SerialHDDriver::exec);
};


void loop()
{
	automSched.schedule();
	waitserver.check();
//	debug_printdec_int32(impulse_count);dln;
/*	ptr = mes;
	*ptr++ = 1;
	*ptr++ = '2';
	*ptr++ = '0';
	*ptr++ = '1';
	*ptr++ = 2;
	*ptr++ = '0';
	*ptr++ = '0';
	*ptr++ = 3;
	checksum_declare(mes, 3);
	//stdout.write(mes, 10);
	//SerialHD6.configure_session(mes,10,3);
	//SerialHD6.start_session();
debug_write(mes, 10);
delay(200);
//SerialHD6.break_end((void*)0);	*/
};

