#include "genos/time/sysclock.h"
#include "avr/interrupt.h"
#include "genos/debug/debug.h"
#include "asm/Serial.h"

int interrupt_checker(){ return 1; };

extern "C" void TIMER0_OVF_vect() __attribute__((signal, used, externally_visible));
void TIMER0_OVF_vect()
{
	sysclock_tick();
};

extern "C" void USART0_RX_vect() __attribute__((signal, used, externally_visible));
void USART0_RX_vect()
{
	char c;
	if (ASerial0.correct_receive()) ASerial0.recv(ASerial0.getc());
	else ASerial0.getc();
};

extern "C" void USART0_UDRE_vect() __attribute__((signal, used, externally_visible));
void USART0_UDRE_vect()
{
	ASerial0.sended();
};