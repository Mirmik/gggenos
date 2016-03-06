#ifndef GENOS_DIAG_H
#define GENOS_DIAG_H

//Diag является отладочным интерфейсом ввода-вывода для таких систем, как debug_print, printk.
//Указанные системы используют функции, смонтированные в cdiag (current diag)
//Diag не регламинтирует, используется ли он для вывода в виде потока байт, 
//консольного форматированного вывода,
//или вывода в файл. То есть cdiag является просто точкой монтирования функций ввода-вывода,
//используемых отладочным вводом-выводом.

#include "sys/cdefs.h"

__BEGIN_DECLS

struct diag_ops 
{
	int (*putc)(int c);
	int (*getc)();
	int (*write)(void* buf, int n);
	int (*read)(void* buf, int n);
//	int (*available)();
//	int (*flush)();
	int (*init)();
};

typedef int(*diag_putc_t)(int);
typedef int(*diag_getc_t)();
typedef int(*diag_write_t)(void*, int);
typedef int(*diag_read_t)(void*, int);;
//typedef int(*diag_available_t)();;
//typedef int(*diag_flush_t)();
typedef int(*diag_init_t)();

extern struct diag_ops* current_diag;

void diag_setup(struct diag_ops* ndiag);
int diag_putc(int c);
int diag_getc();
int diag_write(void* buf, int len);
int diag_read(void* buf, int len);
//int diag_available();
//int diag_flush();
int diag_init();

int diag_write_stub(void* buf, int len);
int diag_read_stub(void* buf, int len);

__END_DECLS

#endif