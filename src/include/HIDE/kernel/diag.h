#ifndef GENOS_DIAG_H
#define GENOS_DIAG_H

//Diag является отладочным интерфейсом ввода-вывода для таких систем, как debug_print, printk.
//Указанные системы используют функции, смонтированные в cdiag (current diag)
//Diag не регламинтирует, используется ли он для вывода в виде потока байт, 
//консольного форматированного вывода,
//или вывода в файл. То есть cdiag является просто точкой монтирования функций ввода-вывода,
//используемых отладочным вводом-выводом.

#include "assert.h"

struct diag_ops 
{
	void (*putc)(char c);
	char (*getc)();
	
	int (*write)(char* buf, int n);
	int (*read)	(char* buf, int n);
	
	void (*init)();
};


extern struct diag_ops* current_diag;


static void diag_setup(struct diag_ops* ndiag){current_diag = ndiag;};

static void diag_putc(char c){
	assert(current_diag);
	assert(current_diag->putc);
	current_diag->putc(c);
};

static char diag_getc(){
	assert(current_diag);
	assert(current_diag->getc);
	return current_diag->getc();
};

static char diag_init(){
	assert(current_diag);
	assert(current_diag->init);
	current_diag->init();
};
#endif