#include "genos/debug/debug.h"
#include "kernel/diag.h"
#include "hal/arch.h"

#define LITTLE_ENDIAN
#include "utilxx/byte_access.h"

int main()
{
arch_init();
diag_init(); 

byte_accessor u;
u.hi = 0xA;
u.lo = 0xB;

dprhexln(u.num);
dprhexln(u.bit0);
dprhexln(u.bit1);
dprhexln(u.bit2);
dprhexln(u.bit3);
dprhexln(u.bit4);
dprhexln(u.bit5);
dprhexln(u.bit6);
dprhexln(u.bit7);

};