
#include "asm/Serial.h"
#include "util/bits.h"
#include "util/ring.h"
#include "defines/decltypeof.h"
#include "avr/io.h"

HardwareSerial::HardwareSerial() : rx_head(0), rx_tail(0), tx_head(0), tx_tail(0)
{};


int HardwareSerial::putc(char c)
{
  if (ring_empty(tx_head, tx_tail) && usart_udr_is_empty(usart)) 
  {
    *usart->udr = c;
    bits_bit_set(*usart->ucsr_a, TXC0);
    return 1;
  }
  while (ring_full(tx_head, tx_tail, SERIAL_TX_BUFFER_SIZE)) 
  {
    if (bits_bit_is_clr(SREG, SREG_I)) {
      // Interrupts are disabled, so we'll have to poll the data
      // register empty flag ourselves. If it is set, pretend an
      // interrupt has happened and call the handler to free up
      // space for us.
      if(bits_bit_is_clr(*usart->ucsr_a, UDRE0))
	udre_irq((void*)this);
  };};
  ring_putc(tx_head, tx_tail, tx_buffer, SERIAL_TX_BUFFER_SIZE, c);
  usart_udr_empty_isr_enable(usart);
};

int HardwareSerial::getc(void)
{
  if (ring_empty(rx_head, rx_tail)) return -1;
  return ring_getc(rx_head, rx_tail, rx_buffer, SERIAL_RX_BUFFER_SIZE);
};

int HardwareSerial::available(void)
{
	return ring_available_to_getc(rx_head, rx_tail, SERIAL_RX_BUFFER_SIZE);
};


void rx_irq(void* data)
{
  	HardwareSerial* serial = reinterpret_cast<HardwareSerial*>(data);
  	usart_regs* usart = serial->usart;

	if (bits_bit_is_clr(*usart->ucsr_a, UPE0)) 
	{
    	// No Parity error, read byte and store it in the buffer if there is
    	// room
    	unsigned char c = *usart->udr;
    	if (ring_full(serial->rx_head, serial->rx_tail, SERIAL_RX_BUFFER_SIZE)) return; 
      ring_putc(serial->rx_head, serial->rx_tail, serial->rx_buffer, SERIAL_RX_BUFFER_SIZE, c);
} 
  	else 
  	{
      debug_print("Parrity_error");
    	// Parity error, read byte but discard it
    	*usart->udr;
  	};
};


void udre_irq(void* data)
{
  HardwareSerial* serial = reinterpret_cast<HardwareSerial*>(data);
  usart_regs* usart = serial->usart;
  
  *usart->udr = ring_getc(serial->tx_head, serial->tx_tail, serial->tx_buffer, SERIAL_TX_BUFFER_SIZE);

  bits_bit_set(*usart->ucsr_a, TXC0);

  if (ring_empty(serial->tx_head, serial->tx_tail)) 
  {
    // Buffer empty, so disable interrupts
    usart_udr_empty_isr_disable(usart);
  }
};