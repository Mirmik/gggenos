#include "stdlib.h"
#include "hal/arch.h"
#include "hal/gpio.h"
#include "genos/debug/debug.h" 
#include "genos/time/sysclock.h"
#include "genos/kernel/waitserver.h"
#include "genos/io/Serial.h"
#include "genos/schedproc/automSched.h"
#include "genos/io/SerToSer.h"
#include "genos/terminal/numcmd_list.h"
#include "genos/schedproc/switchScheduler.h"
#include "genos/terminal/autom_terminal.h"
#include "genos/terminal/command_list.h"
#include "genos/io/SerToSer.h"
#include "util/ascii_codes.h"
#include "configure.h"
#include "axis.h"
#include "genos/debug/delay.h"
//#include "kernel/mutex.h"

int32_t home_position_enc_x = 0;
int32_t home_position_gen_x = 0;

MoveController* volatile current_controller = 0;

uint32_t acceleration = 150;
uint8_t pulse_direction = 1;


syscontext syscntxt;

numcmd_list Ncmd;
switchScheduler switchSched;
automTerminal automTerm;
context schedule_context;

Serial_t<512,512> Serial2;
Serial_t<512,512> Serial6;
AdapterSerial ASerial2;
AdapterSerial ASerial6;
SerToSer S2S;

PeriodicMoveController periodic_move_x;
TestDirectionController test_direction_x;
HomePositionController home_position_controller_x;


void remove_controller(MoveController* ctrl)
{
	debug_print("force_remove_controller\r\n");
	ctrl->remove();
};

void emergency_stop()
{
	arch_atomic();
	axis_x.stop();
	debug_print("EMERGENCY_STOP");
};

void setup();
void loop();

int main(){
	setup();
	while(1) loop();
};

class Blinker
{
int state = 0;
TimWaiter t;
public:
	void exec()
	{
		while(1)
		{
		gpio_port_tgl_pin(GPIOD,13);
		msleep_subst(100);
		gpio_port_tgl_pin(GPIOD,12);
		msleep_subst(100);
		};
	};
} blinker;

void print_encoder()
{
	stdout.println(axis_x.read_encoder());
};

void print_encoder2()
{
	//stdout.println((int32_t)TIM2->CNT);
	stdout.print("stub");
};

extern uint32_t impulse_count;

void com_x_impulse(int32_t impulse)
{
	if (impulse > 0)
	{
		axis_x.set_direction(Axis::FORWARD);
		axis_x.set_generator(abs(impulse));
	}
	else
	{
		axis_x.set_direction(Axis::BACKWARD);
		axis_x.set_generator(abs(impulse));
	}
}

void pulse_x_impulse(int n, char** c)
{
	if (n!=2) {stdout.println("NumpErr");};	
	int32_t impulse = atoll(c[1]);
	com_x_impulse(impulse);
};

void com_x_speed(uint32_t speed)
{
	axis_x.set_speed(speed);
};

void pulse_x_speed(int n, char** c)
{
	if (n==1) {stdout.println((int64_t)axis_x.get_speed()); return;};
	if (n!=2) {stdout.println("NumpErr");};	
	com_x_speed(atol(c[1]));
};

//void pulse_x_direction_set(int n, char** c)
//{
//	if (n!=2) {stdout.println("NumpErr");};	
//	if ((strcmp(c[1], "0")) && (strcmp(c[1], "1")))  {stdout.println("dirwrong");return;};
//	if (atoi(c[1]) == 1) axis_x.set_direction(Axis::FORWARD);
//	else if (atoi(c[1]) == 0) axis_x.set_direction(Axis::BACKWARD);
//	else debug_print("WrongDir");
//};

void timer4info()
{
	debug_print("CR1:");debug_printhex_uint32(TIM4->CR1);dln;
	debug_print("CR2:");debug_printhex_uint32(TIM4->CR2);dln;
	debug_print("SR:");debug_printhex_uint32(TIM4->SR);dln;
	debug_print("DIER:");debug_printhex_uint32(TIM4->DIER);dln;
	debug_print("CCMR1:");debug_printhex_uint32(TIM4->CCMR1);dln;
	debug_print("CCMR2:");debug_printhex_uint32(TIM4->CCMR2);dln;
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

#include "genos/decoration.h"
void* operator new(size_t, void* ptr) {return ptr;};
void global_constructor_stub()
{
	new(&blinker) Blinker;
	new(&axis_x) Axis;
	new(&switchSched) switchScheduler;
	new(&automTerm) automTerminal;
	new(&waitserver) WaitServer;
	new(&Serial2) Serial_t<512,512>;
	new(&ASerial2) AdapterSerial;
	new(&Serial6) Serial_t<512,512>;
	new(&ASerial6) AdapterSerial;
	new(&syscntxt) syscontext;
	new(&Ncmd) numcmd_list;
	new(&central_cmdlist) command_list;
	new(&periodic_move_x) PeriodicMoveController;
	new(&test_direction_x) TestDirectionController;
	new(&home_position_controller_x) HomePositionController;
	new(&S2S) SerToSer;
	machine_name = "input";
};

void stop()
{
	if (current_controller) 
		remove_controller(current_controller);
	else axis_x.generator = 0;
};

void nac_manual_pulse()
{
	one_pulse_generator_start();
};

void set_controller()
{
	stdout.print("periodic controller setup");
	if (current_controller) remove_controller(current_controller);
	
	periodic_move_x.init(
		&axis_x,
		one_pulse_generator_start
	);
	periodic_move_x.set_task(10000000, 100000, 100000, 10, 1000);

	current_controller = &periodic_move_x;
}

void set_home_gen_controller()
{
	dprln("periodic controller setup");
	if (current_controller) remove_controller(current_controller);
	home_position_controller_x.init(&axis_x);
	current_controller = &home_position_controller_x;
}

void set_test_controller()
{
	dprln("test controller setup");
	if (current_controller) remove_controller(current_controller);
	test_direction_x.init(&axis_x);
	current_controller = &test_direction_x;
}

void ctrstp()
{
	if (current_controller) 
		remove_controller(current_controller);
}

void set_home_position()
{
	home_position_enc_x = axis_x.read_encoder();
	home_position_gen_x = axis_x.read_generator();
}

uint32_t impulse_com(uint32_t imp)
{
	com_x_impulse(imp);
	return 0;
};

uint32_t speed_com(uint32_t speed)
{
	com_x_speed(speed);
	return 0;
};

uint32_t testlink_com(uint32_t)
{
	return 0xABCD;
};

uint32_t stop_com(uint32_t str)
{
	stop();
	return 0;
};

uint32_t testctr_com(uint32_t str)
{
	set_test_controller();
	return 0;
};

uint32_t homectr_com(uint32_t str)
{
	set_home_gen_controller();
	return 0;	
};

uint32_t scanctr_com(uint32_t str)
{
	set_controller();
	return 0;
};



void program()
{


	/*	charbuf<14> sim;
		sim[0] = 1;
		sim[13]  = 3;
		memcpy(&sim[1], "0E", 2);
		memcpy(&sim[3], "00000000", 8);
		sim[11] = sim[12] = 0;
		uint8_t cs = sim.controlsum();
		charbuf<2> ch_buf = uint8_to_hexcode(cs);
		memcpy(&sim[11], ch_buf.to_buf(), 2);

		Serial6.simulation_input(sim.c_str(),14);
		msleep_subst(5000);

*/
	charbuf<14> mes, mest;
	mes = S2S.mbuild(0xDD, "00000000");

	//if(S2S.send(&mes) != 0) Serial2.print("ErrorSend");
	//else Serial2.print(mes.c_str());

	while(true)
		{
			mest = mes;
			S2S.send(&mest);
			//msleep_subst(2000);
			processor_delay_ms(2000);
		};

	exit_subst();
};




void setup(){
	global_constructor_stub();
	arch_init();
	project_configure();
	
	current_syscontext(&syscntxt);
	stdout.direct(&Serial2);
	stdin.direct(&Serial2);

	ASerial2.usart = USART2;
	Serial2.init(&ASerial2);

	ASerial6.usart = USART6;
	Serial6.init(&ASerial6);

	usart2_interrupt_enable();
	usart6_interrupt_enable();

	axis_x.init(&TIM5->CNT, &TIM4->ARR, &TIM5->CCR3, 
		8400000, 50000, 5000);
	axis_x.enable_compare = tim5_compare_enable;
	axis_x.disable_compare = tim5_compare_disable;


	current_context_set(&schedule_context);
	central_cmdlist.add("encoder", print_encoder);
	central_cmdlist.add("encoder2", print_encoder2);
	central_cmdlist.add("nac", nac_manual_pulse);
	central_cmdlist.add("impulse", pulse_x_impulse);
	central_cmdlist.add("speed", pulse_x_speed);
	central_cmdlist.add("timer4", timer4info);
	central_cmdlist.add("timer5", timer5info);
	central_cmdlist.add("timer10", timer10info);
	central_cmdlist.add("stop", stop);
	central_cmdlist.add("testdirection", set_test_controller);
	central_cmdlist.add("controller", set_controller);
	central_cmdlist.add("ctrstp", ctrstp);
	central_cmdlist.add("set_home", set_home_position);
	central_cmdlist.add("go_home", set_home_gen_controller);

	debug_print("System Initialize.\r\n");

	Ncmd.add(1, stop_com);
	Ncmd.add(3, impulse_com);
	Ncmd.add(4, speed_com);
	Ncmd.add(12, testctr_com);
	Ncmd.add(13, homectr_com);
	Ncmd.add(14, scanctr_com);
	Ncmd.add(64, testlink_com);
	
	axis_x.set_direction(Axis::FORWARD);
	
	axis_x.set_speed(300);
	axis_x.generator_reverse = true;
	axis_x.encoder_reverse = true;

	switchSched.registry(&blinker, &Blinker::exec);

	S2S.strm.direct(&Serial6);
	S2S.ncmd = &Ncmd;

	switchSched.registry(&S2S, &SerToSer::exec);
	switchSched.registry(&automTerm, &automTerminal::exec);
	switchSched.registry(program);

	delay(2000);
};


void loop()
{
//	waitserver.check();
//	switchSched.schedule();

processor_delay_ms(500);
//dpr("Allgood");

//dpr("Serialggg");
Serial2.print("Serialfff");
};













//INTERRUPT

#include "genos/debug/debug.h"
#include "asm/Serial.h"
#include "hal/gpio.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "misc.h"

extern "C" void TIM5_IRQHandler();
void TIM5_IRQHandler()
{
  if (TIM_GetITStatus(TIM5, TIM_IT_CC3) != RESET)
    {
      axis_x.trigger_callback();
      TIM_ClearITPendingBit(TIM5, TIM_IT_CC3);
      return;
    };
    
    debug_panic("TIMER5INTERRUPT");
  };


void state0()
{
  gpio_port_set_mask(GPIOD, GPIO_Pin_2 | GPIO_Pin_6);
  gpio_port_clr_mask(GPIOD, GPIO_Pin_0 | GPIO_Pin_4);
};

void state1()
{
  gpio_port_set_mask(GPIOD, GPIO_Pin_0 | GPIO_Pin_6);
  gpio_port_clr_mask(GPIOD, GPIO_Pin_2 | GPIO_Pin_4);
};

void state2()
{
  
  gpio_port_set_mask(GPIOD, GPIO_Pin_0 | GPIO_Pin_4);
  gpio_port_clr_mask(GPIOD, GPIO_Pin_2 | GPIO_Pin_6);
};

void state3()
{
  gpio_port_set_mask(GPIOD, GPIO_Pin_2 | GPIO_Pin_4);
  gpio_port_clr_mask(GPIOD, GPIO_Pin_0 | GPIO_Pin_6);
};


extern "C" void TIM4_IRQHandler();
void TIM4_IRQHandler()
{
//	dpr("here");
  if(TIM4->SR & 0x1) //Прерывание по переполнению
    {
 //   	debug_printdec_uint32(axis_x.generator);
      if (axis_x.generator <= 0) { gpio_port_clr_pin(GPIOD,15); goto _exit;};
      gpio_port_set_pin(GPIOD,15);

      if (axis_x.direction != axis_x.generator_reverse)
      {
		axis_x.pstate = (++axis_x.pstate) & 0x3; 
		++axis_x.count_generated;
      }
      else
      {
      	axis_x.pstate = (--axis_x.pstate) & 0x3; 
      	--axis_x.count_generated;
      }
      
      switch(axis_x.pstate)
      {
        case 0: state0(); break;
        case 1: state1(); break;
        case 2: state2(); break;
        case 3: state3(); break;
      };
      axis_x.generator--;
      
      _exit:
      TIM4->SR &= ~0x1;    
      return; 
    };

  dln;
  debug_printhex_uint32(TIM4->CR1);dln;
  debug_printhex_uint32(TIM4->SR);dln;
  debug_panic("TIM4INTERRUPT");
};




#include "asm/Serial.h"
extern AdapterSerial ASerial2;
extern "C" void USART2_IRQHandler();
void USART2_IRQHandler()
{
  //Действия по опустошению регистра передачи.
  if (USART_GetITStatus(USART2, USART_IT_TXE) == SET)
    {
    	//dpr("interr");
        ASerial2.sended();
        return;
    };

if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
    {
        ASerial2.recv();
        return;
    };


  debug_panic("USART_DEBUG_PANIC2");
};


extern AdapterSerial ASerial6;
extern "C" void USART6_IRQHandler();
void USART6_IRQHandler()
{
  //Действия по опустошению регистра передачи.
  if (USART_GetITStatus(USART6, USART_IT_TXE) == SET)
    {
//        debug_panic("TX");
        ASerial6.sended();
        return;
    };

if (USART_GetITStatus(USART6, USART_IT_RXNE) == SET)
    {
//       debug_panic("RX");
        ASerial6.recv();
        return;
    };


  debug_panic("USART_DEBUG_PANIC2");
};
