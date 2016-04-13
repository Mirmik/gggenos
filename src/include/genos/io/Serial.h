

#ifndef GENOS_SERIAL_H
#define GENOS_SERIAL_H

#include <inttypes.h>
#include "asm/Serial.h"
#include "genos/io/stream.h"
#include "utilxx/ring.h"
#include "stdlib.h"

//class AdapterSerial; 
template<uint16_t SERIAL_RX_BUFFER, uint16_t SERIAL_TX_BUFFER>
class Serial_t : public stream
{
  private:
    ring<char> rx_ring;
    ring<char> tx_ring;
    AdapterSerial* adapter;

  public:
    Serial_t() 
    {  
        void* rx_buffer = malloc(SERIAL_RX_BUFFER);
        void* tx_buffer = malloc(SERIAL_TX_BUFFER);
        rx_ring.init(rx_buffer, SERIAL_RX_BUFFER);
        tx_ring.init(tx_buffer, SERIAL_TX_BUFFER);
    };
    
    int putc(char c) {
        sreg_t save;
        arch_atomic_temp(save);
        if (tx_ring.empty() && adapter->tx_ready()) 
        {
            adapter->putc(c);
            arch_deatomic_temp(save);
            return 1;
        }
        if (tx_ring.full()) debug_panic("SerialTXBufferFull"); 
        tx_ring.putc(c);
        adapter->tx_interrupt_enable();
        arch_deatomic_temp(save);
    };

    int getc()
    {
        sreg_t save;
        arch_atomic_temp(save);
        int ret = rx_ring.getc();
        arch_deatomic_temp(save);
        return ret;
    };
    
    int available() 
    {
        sreg_t save;
        arch_atomic_temp(save);
        int ret = rx_ring.available_to_getc();
        arch_deatomic_temp(save);
        return ret;
    };

    void tx_handler()
    {
        if (tx_ring.empty()) adapter->tx_interrupt_disable();
        else adapter->putc(tx_ring.getc());
    };

    void rx_handler(char c)
    {
        if (rx_ring.full()) {debug_panic("SerialRXBufferFull");};
        rx_ring.putc(c);
    };

    void init(AdapterSerial* adpt)
    {
        adapter = adpt;
        adapter->recv = delegate<void,char>(this, &Serial_t::rx_handler);
        adapter->sended = delegate<void>(this, &Serial_t::tx_handler);
        adapter->rx_interrupt_enable();
    };

};
#endif
