#include "genos/debug/debug.h"
#include "kernel/diag.h"
#include "hal/arch.h"

//#include "genos/sigslot/delegate.h"

#include "utilxx/ring.h"

#include "stdlib.h"

int main()
{
arch_init();
diag_init(); 

ring_t ring;

void* buf = malloc(100);

ring.init(buf, 100);

ring.putc('a');
ring.putc('b');
ring.putc('z');
ring.putc('a');
ring.putc('c');
ring.putc('c');

debug_putchar(ring.getc());
debug_putchar(ring.getc());
debug_putchar(ring.getc());
debug_putchar(ring.getc());
debug_putchar(ring.getc());
dln;
debug_printhex_uint32((uint32_t)ring.available_to_getc());dln;
debug_putchar(ring.getc());
debug_putchar(ring.getc());
debug_putchar(ring.getc());
debug_putchar(ring.getc());
debug_putchar(ring.getc());
dln;
debug_printhex_uint32((uint32_t)ring.head);dln;
debug_printhex_uint32((uint32_t)ring.tail);dln;

dprhex("Kekeke");dln;

dprhexln(0.2);


dprln(27);

};