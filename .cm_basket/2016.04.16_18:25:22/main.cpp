#include "genos/debug/debug.h"
#include "kernel/diag.h"
#include "hal/arch.h"


#include "stdlib.h"

#include "util/ring.h"

char buffer[8];
ring r(buffer, 8);

int main()
{
arch_init();
diag_init(); 

dprln(r.is_empty());
dprln(r.is_full());

r.putc('a');
r.putc('b');
r.putc('c');
r.putc('d');
r.putc('e');
r.putc('f');
r.putc('g');
r.putc('g');
r.putc('k');
dprln(r.data_size());
dprln(r.is_empty());
dprln(r.is_full());
};