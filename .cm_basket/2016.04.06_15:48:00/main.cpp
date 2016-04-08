#include "hal/arch.h"
#include "genos/debug/debug.h" 
#include "genos/time/sysclock.h"
#include "genos/debug/delay.h"
#include "hal/gpio.h"

#include "stm32f4x7_eth.h"
#include "stm32f4x7_eth_bsp.h"

#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_rcc.h"

#include "configure.h"

#include "genos/kernel/waitserver.h"
#include "asm/Serial.h"
#include "genos/terminal/command_list.h"
#include "syscontext/syscontext.h"

#include "genos/stack/static_stack.h"
#include "kernel/context.h"

#include "genos/schedproc/switchScheduler.h"
#include "genos/terminal/autom_terminal.h"

#include "genos/decoration.h"
#include "netconf.h"
#include "httpd.h"

volatile uint32_t LocalTime = 0;
context schedule_context;


void global_schedule();

void emergency_stop()
{
	usart2_interrupt_disable();
//	usart6_interrupt_disable();
	
	debug_print("EMERGENCY_STOP");
};


context cntxt1, cntxt2, cntxt3; 
static_stack<500> stck2, stck3;

HardwareSerial Serial2;
syscontext syscntxt;
switchScheduler switchSched;
automTerminal automTerm;

void* operator new(size_t, void* ptr) {return ptr;};
void global_constructor_stub()
{
	new(&waitserver) WaitServer;
	new(&Serial2) HardwareSerial;
	new(&syscntxt) syscontext;
	new(&central_cmdlist) command_list;
	new(&switchSched) switchScheduler;
	new(&automTerm) automTerminal;
};


void task1()
{
while(1)
	{
	gpio_port_tgl_pin(GPIOD, 13);
	msleep_subst(200);
	//debug_print("all good\r\n");
	};
};

void server()
{
volatile uint32_t m;
  	
//     while(1)
 // {
  	m = millis();
  	//debug_printdec_uint32(m);dln;
	  /* check if any packet received */
	   if (ETH_CheckFrameReceived())
	   {
//	   	debug_print(">");
	  /* process received ethernet packet */
	     LwIP_Pkt_Handle();
	   }
	  /* handle periodic timers for LwIP */
	   //LwIP_Periodic_Handle(m);
//  context_switch(current_context_get(), &schedule_context);
 // };
};


void task2()
{
while(1)
	{
	gpio_port_tgl_pin(GPIOD, 15);
	msleep_subst(1000);
	USART6->DR = 'H';
	};
};

void setup(){
	global_constructor_stub();
	arch_init();
	project_configure();

	current_context_set(&schedule_context);

	current_syscontext(&syscntxt);
	(current_syscontext()->__stdout).direct(&Serial2);
	(current_syscontext()->__stdin).direct(&Serial2);

	Serial2.usart = USART2;
	gpio_port_set_pin(GPIOD, 12);


	uint16_t value = ETH_BSP_Config();

	debug_printhex_uint8(value);

	LwIP_Init();

    httpd_init();

    //delay(10);
    //debug_printhex_uint8(millis());
	//debug_print("lwip_inited");

	machine_name = "eth_winner";

	switchSched.registry(task2);
	switchSched.registry(task1);
//	switchSched.registry(server);
	switchSched.registry(&automTerm, &automTerminal::exec);

	usart2_interrupt_enable();
	usart2_rx_interrupt_enable();

	USART6->DR = 'H';
};


int main()
{
	setup();
	global_schedule();
};

extern "C" void EXTI15_10_IRQHandler(void); 
void EXTI15_10_IRQHandler(void)
{
	debug_print("irq");
  if(EXTI_GetITStatus(ETH_LINK_EXTI_LINE) != RESET)
  {
    Eth_Link_ITHandler(DP83848_PHY_ADDRESS);
    /* Clear interrupt pending bit */
    EXTI_ClearITPendingBit(ETH_LINK_EXTI_LINE);
  }
}

extern "C" void ETH_IRQHandler()
{
	debug_panic("ETH_IRQ");
};

void global_schedule()
{
while(1)
	{
		waitserver.check();
		switchSched.schedule();
		server();
	};
};