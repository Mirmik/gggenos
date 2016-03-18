#include "hal/arch.h"
#include "kernel/diag.h"
#include "genos/debug/debug.h"
#include "asm/Serial.h"
#include "asm/Serial_half_duplex.h"
#include "hal/gpio.h"
#include "cpu/timers.h"
#include "genos/time/sysclock.h"
#include "genos/sigslot/delegate.h"

#include "genos/kernel/waitserver.h"
#include "genos/schedproc/automSched.h"
#include "genos/terminal/autom_terminal.h"
#include "util/ring.h"
#include "avr/io.h"
#include "cpu/memory_info.h"

#include "mitsuprotocol.h"
#include "configuration.h"
#include "stdlib.h"

uint8_t trap_flag;
uint8_t trap_counter;
int32_t trap[4];
 
HardwareSerial Serial0;
Serial_HD_simple SerialHD1;



void print_data()
{

	stdout.print(servo0.ERR.data); 
	stdout.putc('\t');
	stdout.print(servo0.ABS.data); 
	stdout.putc('\t');
	stdout.print(servo0.CYC.data); 
	stdout.println();

};


void emergency_stop()
{
	debug_print("EMERGENCY_STOP\n");
};

class Blinker{
	int state = 0;
	int period = 100;
public:
	void input(int i, char** c)
	{
		if (i!=2) stdout.println("Need 1 param");
		period = atoi(c[1]);
	};

	void blinker()
	{
		switch(state)
		{
		case 0:
			central_cmdlist.add("blinker", this, &Blinker::input);
			state = 1;
			break;
		case 1:
			gpio_change(13);
			msleep_autom(period);
			break;
		};
	};
} blink;

automScheduler automSched;
automTerminal automTerm;
syscontext syscntxt;	

void setup();
void loop();

int main(){

	setup();
	arch_deatomic();
	while(1) loop();
};

void usart_info()
{
	stdout.printhexln(UCSR0A);
	stdout.printhexln(UCSR0B);
	stdout.printhexln(UCSR0C);
	stdout.printhexln(UCSR1A);
	stdout.printhexln(UCSR1B);
	stdout.printhexln(UCSR1C);
};

uint8_t flag_usart1;

void print_millis()
{
	stdout.println(millis());
};

void start(int i, char** c)
{
	trap_counter = 0;
	trap_flag = 1;
};

void end(int i, char** c)
{
	stdout.println(trap[0]);
	stdout.println(trap[2]);
	stdout.println(trap[1]);
	stdout.println(trap[3]);
};

volatile int64_t supertick;

class PinUpdater
{
int state;
//char mes[20];
//int mes_len;

public:

	void exec0()
	{
		switch(state)
		{
		case 0:
			servo0.pPIN0.update_start(servo0.stantion_num);
			servo0.pPIN0.wait_operation();
			state=1;
			break;
		case 1:
		//stdout.println(servo0.CYC.data);
			servo0.pPIN0.update_end();
			stdout.printhex(servo0.pPIN0.data);
			stdout.println();
			exit_autom();
			break;
		};

	};

	void conf0(int i, char** c)
	{
		automSched.registry(this, &PinUpdater::exec0);
		state = 0;
	};

	void exec01()
	{
		switch(state)
		{
		case 0:
			servo0.pPIN0.update_start(servo0.stantion_num);
			servo0.pPIN0.wait_operation();
			state=1;
			break;
		case 1:
		//stdout.println(servo0.CYC.data);
			servo0.pPIN0.update_end();
			state =2;
			break;
		case 2:

			stdout.printhex(servo0.pPIN0.data);
			stdout.println();
			exit_autom();
			break;
		};
	};

	void conf01(int i, char** c)
	{
		automSched.registry(this, &PinUpdater::exec0);
		state = 0;
	};

	void exec1()
	{
		switch(state)
		{
		case 0:
			servo0.pPIN1.update_start(servo0.stantion_num);
			servo0.pPIN1.wait_operation();
			state=1;
			break;
		case 1:
		//stdout.println(servo0.CYC.data);
			servo0.pPIN1.update_end();
			stdout.printhex(servo0.pPIN1.data);
			stdout.println();
			exit_autom();
			break;
		};

	};

	void conf1(int i, char** c)
	{
		automSched.registry(this, &PinUpdater::exec1);
		state = 0;
	};

	void exec2()
	{
		switch(state)
		{
		case 0:
			servo0.pPIN2.update_start(servo0.stantion_num);
			//stdout.println(SerialHD1.message);
			servo0.pPIN2.wait_operation();
			state=1;
			break;
		case 1:
		//stdout.println(servo0.CYC.data);
			servo0.pPIN2.update_end();
			stdout.printhex(servo0.pPIN2.data);
			stdout.println();
			exit_autom();
			break;
		};

	};

	void conf2(int i, char** c)
	{
		automSched.registry(this, &PinUpdater::exec2);
		state = 0;
	};
	
} pinUpdater;


class ManualRS
{
int state;
char mes[20];
int mes_len;
public:

	void exec()
	{
		switch(state)
		{
			case 0:
				//stdout.println("here");
				SerialHD1.configure_session(mes , mes_len, 3);
				SerialHD1.start_session();
				wait_autom(&SerialHD1.flag);
				state = 1;
				break;
			case 1:
				stdout.println(SerialHD1.answer);
				stdout.print("ans_len:");
				stdout.println(SerialHD1.answer_len);
				stdout.printdumpstr(SerialHD1.answer, SerialHD1.answer_len);				
				stdout.println();
				stdout.print("flag:");
				stdout.println(SerialHD1.flag);
				exit_autom();
				break;

		};

	};

	void sesconf(int i, char** c)
	{
		if (i!=3) {stdout.println("Err:NumP error");return;};
		if (strlen(c[1])!=2) {stdout.println("Err:Len[1]!=2");return;};
		if (strlen(c[2])!=2) {stdout.println("Err:Len[2]!=2");return;};
		
		state = 0;

		mes[0] = 1;
		mes[1] = '2';
		strcpy(&mes[2], c[1]);
		mes[4] = 2;
		strcpy(&mes[5], c[2]);
		mes[7] = 3;
		mes[10] = 0;
		checksum_declare(mes, 3);
		
		mes_len = 10;
		automSched.registry(this, &ManualRS::exec);

		stdout.println(mes);
	};

	void sesconf8(int i, char** c)
	{
		if (i!=4) {stdout.println("Err:NumP error");return;};
		if (strlen(c[1])!=2) {stdout.println("Err:Len[1]!=2");return;};
		if (strlen(c[2])!=2) {stdout.println("Err:Len[2]!=2");return;};
		if (strlen(c[3])!=8) {stdout.println("Err:Len[3]!=8");return;};
		
		state = 0;

		mes[0] = 1;
		mes[1] = '2';
		strcpy(&mes[2], c[1]);
		mes[4] = 2;
		strcpy(&mes[5], c[2]);
		strcpy(&mes[7], c[3]);
		mes[15] = 3;
		mes[18] = 0;
		checksum_declare(mes, 3);
		
		mes_len = 18;
		automSched.registry(this, &ManualRS::exec);

		stdout.println(mes);
	};

	void sesconf4(int i, char** c)
	{
		if (i!=4) {stdout.println("Err:NumP error");return;};
		if (strlen(c[1])!=2) {stdout.println("Err:Len[1]!=2");return;};
		if (strlen(c[2])!=2) {stdout.println("Err:Len[2]!=2");return;};
		if (strlen(c[3])!=4) {stdout.println("Err:Len[3]!=4");return;};
		
		state = 0;

		mes[0] = 1;
		mes[1] = '2';
		strcpy(&mes[2], c[1]);
		mes[4] = 2;
		strcpy(&mes[5], c[2]);
		strcpy(&mes[7], c[3]);
		mes[11] = 3;
		mes[14] = 0;
		checksum_declare(mes, 3);
		
		mes_len = 18;
		automSched.registry(this, &ManualRS::exec);

		stdout.println(mes);
	};

} rsManual;


void setup(){
	arch_init();
	diag_init();

	debug_delay(20000);
	load_configuration();

// 	timer0_source_set(timerSourceTaktRising);
	//timer0_wgm_set(timerWgmNormal);

	//usart_drop(&usart0);
	usart_baudrate_set(&usart0, conf.usart0_speed);
	usart_parity_set(&usart0, conf.usart0_parity);
	usart_stopbits_set(&usart0, conf.usart0_stopbits);
	usart_databits_set(&usart0, conf.usart0_databits);

//	usart_baudrate_set(&usart0, conf.usart1_speed);
//	usart_parity_set(&usart0, conf.usart1_parity);
//	usart_stopbits_set(&usart0, conf.usart1_stopbits);
//	usart_databits_set(&usart0, conf.usart1_databits);

	usart_baudrate_set(&usart1, 115200);
	usart_parity_set(&usart1, usartParityEven);
	usart_stopbits_set(&usart1, usartOneStopBit);
	usart_databits_set(&usart1, usartEightDataBit);

	usart_udr_empty_isr_disable(&usart0);
	usart_rx_isr_enable(&usart0);
	
	usart_rx_enable(&usart0);
	usart_tx_enable(&usart0);

	Serial0.connect(&usart0);
	SerialHD1.connect(&usart1, 6);

	//usart_udr_empty_isr_enable(&usart1);

	current_syscontext(&syscntxt);
	stdout.direct(&Serial0);
	stdin.direct(&Serial0);

	central_cmdlist.add("timer", timer_info);
	central_cmdlist.add("start", start);
	central_cmdlist.add("end", end);
	central_cmdlist.add("print_conf", print_conf);
	central_cmdlist.add("print_writed_conf", print_writed_conf);
	central_cmdlist.add("write_conf", write_conf);
	central_cmdlist.add("memory_info", memory_info);
	central_cmdlist.add("usart_info", usart_info);
	central_cmdlist.add("millis", print_millis);
	central_cmdlist.add("data", print_data);
	central_cmdlist.add("rs", &rsManual, &ManualRS::sesconf);
	central_cmdlist.add("rs8", &rsManual, &ManualRS::sesconf8);
	central_cmdlist.add("rs4", &rsManual, &ManualRS::sesconf4);

	central_cmdlist.add("pin0", &pinUpdater, &PinUpdater::conf0);
	central_cmdlist.add("pin1", &pinUpdater, &PinUpdater::conf1);
	central_cmdlist.add("pin2", &pinUpdater, &PinUpdater::conf2);

	central_cmdlist.add("st1", &pinUpdater, &PinUpdater::conf01);

	gpio_mode_out(10);

	gpio_mode_out(13);
	gpio_hi(13);

	automSched.registry(&blink, &Blinker::blinker);
	automSched.registry(&automTerm, &automTerminal::exec);
//	automSched.registry(&updater, &Updater::exec);
	//automSched.registry(print_data);
	debug_delay(20000);
	
	arch_deatomic();
};

void loop(){
	automSched.schedule();
	waitserver.check();
};

