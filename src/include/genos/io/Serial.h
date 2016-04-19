#ifndef GENOS_SERIAL_H
#define GENOS_SERIAL_H

#include "genos.h"
#include <inttypes.h>
#include "asm/Serial.h"
#include "genos/io/stream.h"
#include "util/ring.h"

//class AdapterSerial; 
template<uint16_t SERIAL_RX_BUFFER, uint16_t SERIAL_TX_BUFFER>
class Serial_t : public stream
{
  public:
    char rx_buffer[SERIAL_RX_BUFFER];
    char tx_buffer[SERIAL_TX_BUFFER];
    ring rx_ring;
    ring tx_ring;
    AdapterSerial* adapter;

  public:
    Serial_t() 
    : 
    rx_ring(rx_buffer, SERIAL_RX_BUFFER), 
    tx_ring(tx_buffer, SERIAL_TX_BUFFER)
    { };
    
    int putc(char c) {
        sreg_t save;
        arch_atomic_temp(save);
        if (tx_ring.is_empty() && adapter->tx_ready()) 
        {
            adapter->putc(c);
            arch_deatomic_temp(save);
            return 1;
        }
        if (tx_ring.is_full()) debug_panic("SerialTXBufferFull"); 
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
        int ret = rx_ring.data_size();
        arch_deatomic_temp(save);
        return ret;
    };

    void tx_handler()
    {
        if (tx_ring.is_empty()) adapter->tx_interrupt_disable();
        else adapter->putc(tx_ring.getc());
    };

    void rx_handler()
    {
        if (!adapter->correct_receive()) {char drop = adapter->getc(); return;};
        char c = adapter->getc();
        if (rx_ring.is_full()) {debug_panic("SerialRXBufferFull");};
        rx_ring.putc(c);
    };

    void simulation_input(const char* str, int len)
    {
        sreg_t save;
        arch_atomic_temp(save);
        for(int i = 0; i < len; i++)
        {
            if (rx_ring.is_full()) 
                {debug_panic("SerialRXBufferFull");};
            rx_ring.putc(*(str + i));
        };
        arch_deatomic_temp(save); 
    };

    void init(AdapterSerial* adpt)
    {
        adapter = adpt;
        adapter->recv = delegate_mtd(this, &Serial_t::rx_handler);
        adapter->sended = delegate_mtd(this, &Serial_t::tx_handler);
        adapter->rx_interrupt_enable();
    };

    string to_info()
    {
        string str;
        str.reserve(128);
        str << "TODO";
        return str;
    };

    string to_str()
    {
        string str;
        str << "<Serial>";
        return str;
    };
};
#endif
