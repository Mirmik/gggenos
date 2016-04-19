#ifndef _CONFIGURE
#define _CONFIGURE

void project_configure();

void usart2_interrupt_enable();
void usart2_interrupt_disable();
void usart2_rx_interrupt_enable();

void usart6_interrupt_enable();
void usart6_interrupt_disable();
//void usart6_rx_interrupt_enable();

void tim5_compare_enable();
void tim5_compare_disable();

void one_pulse_generator_start();
#endif /*_CONFIGURE*/