#ifndef MAIN_CONFIGURATION_H
#define MAIN_CONFIGURATION_H

#include "cpu/usart_control_struct.h" 

struct configuration_t
{
	uint16_t magic;
	uint32_t usart0_speed;
	usartParityMode usart0_parity;
	usartStopBitsMode usart0_stopbits;
	usartDataBitsMode usart0_databits;
	uint32_t usart1_speed;
	usartParityMode usart1_parity;
	usartStopBitsMode usart1_stopbits;
	usartDataBitsMode usart1_databits;
};

extern configuration_t default_conf;
extern configuration_t conf;

void load_configuration();
void update_configuration();
void print_conf();
void print_writed_conf();
void write_conf();

#endif