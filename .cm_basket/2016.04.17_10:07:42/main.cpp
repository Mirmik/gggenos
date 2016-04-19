#include "genos/debug/debug.h"
#include "kernel/diag.h"
#include "hal/arch.h"

#include "stdlib.h"
#include "util/stub.h"
#include "util/future/uroboros.h"


int main()
{
arch_init();
diag_init(); 
char buffer[10];
uroboros u(buffer, 10);

u.putc('1');
u.putc('2');
u.putc('3');
u.putc('4');
u.putc('5');
u.putc('6');
u.putc('7');
u.putc('8');
u.putc('9');
u.putc('A');
u.putc('B');
u.putc('C');

char buf[12];

dpr(u.to_info());
u.read(buf,20);
dprln(buf);
dpr_dump_ascii(&u, 10);

};