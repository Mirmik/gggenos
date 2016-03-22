#ifndef GENOS_DEBUG_DELAY_H
#define GENOS_DEBUG_DELAY_H

#include "sys/cdefs.h"
#include "inttypes.h"

__BEGIN_DECLS

void processor_delay_ms(uint32_t);
void processor_delay_us(uint32_t);

__END_DECLS 

#endif