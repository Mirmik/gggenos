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

#include "genos/schedproc/automSched.h"
#include "genos/terminal/autom_terminal.h"
#include "genos/terminal/command_list.h"


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
syscontext syscntxt;


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
	stdout.println((uint64_t)TIM5->CNT);
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
int state;
char mes[50];
int meslen;
uint16_t speed = 255;

public:
	void exec()
	{
		char temp[10];
		switch(state)
		{
			case 0:
			stdout.print("step one");
			confmes4(mes, "8B", "00", "0001");
			meslen = 14;
			SerialHD6.configure_session(mes , meslen, 3);
			SerialHD6.callback = delegate_mtd(&rsManual, &ManualRS::callback);
			delay(20);
			stdout.write(mes,meslen);
			SerialHD6.start_session();
			wait_autom(&SerialHD6.flag);
			state = 1;
			break;

			case 1:
			stdout.print("step two");
			hilo_byte_to_symbol(speed, temp);	
			confmes4(mes, "A0", "10", temp);
			//stdout.write(mes, 18);
			//while(1);
			meslen = 14;
			SerialHD6.configure_session(mes , meslen, 3);
			SerialHD6.callback = delegate_mtd(&rsManual, &ManualRS::callback);
			delay(20);
			stdout.write(mes,meslen);
			SerialHD6.start_session();
			wait_autom(&SerialHD6.flag);
			state = 2;
			break;

			case 2:
			stdout.print("step three");
			confmes8(mes, "A0", "11", "00000400");
			meslen = 18;
			SerialHD6.configure_session(mes , meslen, 3);
			SerialHD6.callback = delegate_mtd(&rsManual, &ManualRS::callback);
			stdout.write(mes,meslen);
			delay(20);
			SerialHD6.start_session();
			wait_autom(&SerialHD6.flag);
			state = 3;
			break;


			case 3:
			//stdout.print("step four");
			confmes8(mes, "92", "00", "00000807");
			meslen = 18;
			//stdout.write(mes,meslen);
			SerialHD6.configure_session(mes , meslen, 3);
			//SerialHD6.callback = delegate_mtd(&rsManual, &ManualRS::callback);
			SerialHD6.drop_callback();
			SerialHD6.start_session();
			wait_autom(&SerialHD6.flag);
			state = 4;
			break;

			case 4:
			//while(1);
			msleep_autom(20);
			state = 3;
			break;
		};
	};

} jog_operation;



void reg_jog(){
	jog_operation.state = 0;
	automSched.registry(&jog_operation, &JOG_Operation::exec);
};

#include "stdlib.h"
void restart_jog(int n, char** c){
	if (n!=2) {stdout.println("Err:NumP error");return;};
	
	jog_operation.state = 1;
	jog_operation.speed = atoi(c[1]);
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
	new(&jog_operation) JOG_Operation;
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

	
	central_cmdlist.add("testrs", test_task);	
	central_cmdlist.add("rs", &rsManual, &ManualRS::sesconf);
	central_cmdlist.add("rs8", &rsManual, &ManualRS::sesconf8);
	central_cmdlist.add("rs4", &rsManual, &ManualRS::sesconf4);

	central_cmdlist.add("encoder", print_encoder);
	central_cmdlist.add("start", reg_jog);
	central_cmdlist.add("restart", restart_jog);

	central_cmdlist.add("stop", stop);

	automSched.registry(&a, &A::exec);
	automSched.registry(&automTerm, &automTerminal::exec);
};
void loop()
{
	automSched.schedule();
	waitserver.check();

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

