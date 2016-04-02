#include "hal/arch.h"
#include "genos/debug/debug.h" 
#include "genos/time/sysclock.h"
#include "genos/debug/delay.h"
#include "hal/gpio.h"
#include "genos/kernel/waitserver.h"
#include "asm/Serial.h"
#include "genos/schedproc/automSched.h"
#include "stm32f4xx_flash.h"
#include "util/ascii_codes.h"

#include "configure.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "asm/SerialHD.h"

#include "mitsuprotocol.h"
#include "movecontroller.h"
#include "communication_func.h"
#include "stdlib.h"

#include "genos/schedproc/automSched.h"
#include "genos/terminal/autom_terminal.h"
#include "genos/terminal/command_list.h"


uint32_t communication_absolute_position;
uint32_t communication_revolution_position;

uint32_t acceleration = 150;
uint8_t pulse_direction = 1;

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


void print_encoder()
{
	stdout.println((int32_t)TIM5->CNT);
};




class ManualRS
{
public:
SerialHDDriver::Task task;

int state;
char mes[20];
int mes_len;
public:

	void exec()
	{
			switch (state)
			{
				case 0:
				rsDriver.active(&task);
				wait_autom(&task.flag);
				state = 1;	
				break;

				case 1:
				//debug_print("here");
				//debug_printhex_uint32(task.flag);
				debug_print(task.answer);dln;
				exit_autom();
				break;
			};
	};


	void sesconf(int i, char** c)
	{
		if (i!=3) {stdout.println("Err:NumP error");return;};
		if (strlen(c[1])!=2) {stdout.println("Err:Len[1]!=2");return;};
		if (strlen(c[2])!=2) {stdout.println("Err:Len[2]!=2");return;};
		
		confmes(task.message, c[1], c[2]);

		task.message_len = 10;
		stdout.println(task.message);
		state = 0;
		wait_child(automSched.registry(this, &ManualRS::exec));
	};

	void sesconf8(int i, char** c)
	{
		if (i!=4) {stdout.println("Err:NumP error");return;};
		if (strlen(c[1])!=2) {stdout.println("Err:Len[1]!=2");return;};
		if (strlen(c[2])!=2) {stdout.println("Err:Len[2]!=2");return;};
		if (strlen(c[3])!=8) {stdout.println("Err:Len[3]!=8");return;};
		
		confmes8(task.message, c[1], c[2], c[3]);

		task.message_len = 18;
		stdout.println(task.message);
		state = 0;
		wait_child(automSched.registry(this, &ManualRS::exec));
	};

	void sesconf4(int i, char** c)
	{
		if (i!=4) {stdout.println("Err:NumP error");return;};
		if (strlen(c[1])!=2) {stdout.println("Err:Len[1]!=2");return;};
		if (strlen(c[2])!=2) {stdout.println("Err:Len[2]!=2");return;};
		if (strlen(c[3])!=4) {stdout.println("Err:Len[3]!=4");return;};
		
		confmes4(task.message, c[1], c[2], c[3]);
		task.message_len = 14;
		stdout.println(task.message);
		state = 0;
		wait_child(automSched.registry(this, &ManualRS::exec));
	};

	void registry()
	{
		dlist_move_tail(&task.list, &rsDriver.list);
		//task.callback = delegate<void, void*>(&SerialHD6, &HardwareSerialHD::print_answer);
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
			assert (speed <= MAX_SPEED);
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
	TIM5->CNT = 0;
	//sysexec("pos 100000 1");	
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
	if (speed > MAX_SPEED) {speed = MAX_SPEED; debug_print("speed decrease ");
	debug_printdec_uint32(MAX_SPEED); dln;};
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

	if (speed > MAX_SPEED) {speed = MAX_SPEED; debug_print("speed decrease ");
	debug_printdec_uint32(MAX_SPEED); dln;};

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

extern uint32_t impulse_count;
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

	if (speed_rev_min > MAX_SPEED) {speed_rev_min = MAX_SPEED; debug_print("speed decrease ");
	debug_printdec_uint32(MAX_SPEED); dln;};
	

	speed_rev_sec = speed_rev_min / 60;
	pps = speed_rev_sec * ENCODER_RESOLUTION;

	int res = TIM4_TPS / pps * 4 * 2 * 311 / 200;
	debug_printdec_uint32(res);

	TIM4->ARR = res;
};


void pulse_dirrection_set(int n, char** c)
{
	if (n!=2) {stdout.println("NumpErr");};	
	if ((strcmp(c[1], "0")) && (strcmp(c[1], "1")))  {stdout.println("dirrwrong");return;};

	if (atoi(c[1]) == 1) pulse_direction = -1;
	if (atoi(c[1]) == 0) pulse_direction = 1;
};

uint32_t uint32_protocol_parse(char* ans)
{
	uint32_t res;
	uint8_t* ptr = (uint8_t*) &res;

	char* ans0 = ans;
	char* ans1 = ans + 2;
	char* ans2 = ans + 4;
	char* ans3 = ans + 6;

	*ptr++ = (hexascii_to_byte(*ans3) << 4) + hexascii_to_byte(*(ans3 + 1));
	*ptr++ = (hexascii_to_byte(*ans2) << 4) + hexascii_to_byte(*(ans2 + 1));
	*ptr++ = (hexascii_to_byte(*ans1) << 4) + hexascii_to_byte(*(ans1 + 1));
	*ptr++ = (hexascii_to_byte(*ans0) << 4) + hexascii_to_byte(*(ans0 + 1));

	return res;
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
				confmes(task.message, "02", "91");
				task.message_len = 10;
				wait_autom(&task.flag);
				rsDriver.active(&task);
				state = 2;
			break;


			case 2:
				communication_absolute_position = uint32_protocol_parse(task.answer + 3);
				confmes(task.message, "01", "8B");
				task.message_len = 10;
				wait_autom(&task.flag);
				rsDriver.active(&task);
				state = 3;
			break;
			
			case 3:
				communication_revolution_position = uint32_protocol_parse(task.answer + 3 + 4);
				state = 1;
			break;
		};
	};


} updater;


void updater_start()
{
	automSched.registry(&updater, &Updater::exec);
};

void timer4info()
{
	debug_print("CR1:");debug_printhex_uint32(TIM4->CR1);dln;
	debug_print("CR2:");debug_printhex_uint32(TIM4->CR2);dln;
	debug_print("SR:");debug_printhex_uint32(TIM4->SR);dln;
	debug_print("DIER:");debug_printhex_uint32(TIM4->DIER);dln;
};


void timer5info()
{
	debug_print("CR1:");debug_printhex_uint32(TIM5->CR1);dln;
	debug_print("CR2:");debug_printhex_uint32(TIM5->CR2);dln;
	debug_print("SR:");debug_printhex_uint32(TIM5->SR);dln;
	debug_print("DIER:");debug_printhex_uint32(TIM5->DIER);dln;
debug_print("CCMR1:");debug_printhex_uint32(TIM5->CCMR1);dln;
debug_print("CCMR2:");debug_printhex_uint32(TIM5->CCMR2);dln;

};


void print_communication_abs_pos()
{
	stdout.printhex(communication_absolute_position);
	stdout.println();
};


void print_communication_rev_pos()
{
	stdout.printhex(communication_revolution_position);
	stdout.println();
};

void timer10info()
{
	debug_print("CR1:");debug_printhex_uint32(TIM10->CR1);dln;
	debug_print("CR2:");debug_printhex_uint32(TIM10->CR2);dln;
	debug_print("SR:");debug_printhex_uint32(TIM10->SR);dln;
	debug_print("DIER:");debug_printhex_uint32(TIM10->DIER);dln;
debug_print("CCMR1:");debug_printhex_uint32(TIM10->CCMR1);dln;
debug_print("CCMR2:");debug_printhex_uint32(TIM10->CCMR2);dln;
debug_print("CNT:");debug_printhex_uint32(TIM10->CNT);dln;

};

void rs485maxerror()
{
	stdout.print((uint64_t)rsDriver.max_error);	
};


void nac_manual_pulse()
{
	TIM_Cmd(TIM10, ENABLE);	
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
	new(&rsManual) ManualRS;
	new(&updater) Updater;
	new(&jog_operation) JOG_Operation;
	new(&position_operation) Position_Operation;
	new(&communication) Communication;
	new(&position_controller) PositionController;
};

void position_controller_set()
{
	automSched.registry(&position_controller, &PositionController::exec);
};


void test_flash_write(int n, char** c)
{
	
};

void test_flash_read(int n, char** c)
{
	
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

	central_cmdlist.add("rs", &rsManual, &ManualRS::sesconf);
	central_cmdlist.add("rs8", &rsManual, &ManualRS::sesconf8);
	central_cmdlist.add("rs4", &rsManual, &ManualRS::sesconf4);

	central_cmdlist.add("encoder", print_encoder);
	central_cmdlist.add("jog", reg_jog);
	central_cmdlist.add("jogdir", reg_jog_direction);

	central_cmdlist.add("nac", nac_manual_pulse);

	
	central_cmdlist.add("pos", reg_pos_com);
	central_cmdlist.add("encpos", reg_pos_enc);
	central_cmdlist.add("mode", print_mode);
	
	central_cmdlist.add("posspeed", pos_speed);
	central_cmdlist.add("posacc", pos_acc);
	
	central_cmdlist.add("impulse", impulse);
	central_cmdlist.add("freq", freq);
	central_cmdlist.add("speed", pulse_speed);
	central_cmdlist.add("direction", pulse_dirrection_set);
	
	central_cmdlist.add("mpulse", pulsemode_set);
	

	central_cmdlist.add("timer4", timer4info);
	central_cmdlist.add("timer5", timer5info);
	central_cmdlist.add("timer10", timer10info);
				

	central_cmdlist.add("pseudohome", pseudohome);
	central_cmdlist.add("abs_pos", print_communication_abs_pos);
	central_cmdlist.add("rev_pos", print_communication_rev_pos);
	central_cmdlist.add("posctr", position_controller_set);




//	central_cmdlist.add("pulsemode", reg_pulsemode);
	
	central_cmdlist.add("rs485maxerror", rs485maxerror);
//	central_cmdlist.add("restart", restart_jog);
	jog_operation.registry();
	position_operation.registry();
	rsManual.registry();
	communication.registry();

//	central_cmdlist.add("stop", stop);
	central_cmdlist.add("updater", updater_start);
	central_cmdlist.add("testwrite", test_flash_write);
	central_cmdlist.add("testread", test_flash_read);


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

