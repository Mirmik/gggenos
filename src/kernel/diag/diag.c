

#include "kernel/diag.h"
#include "assert.h"
#include "stddef.h"
#include "util/stub.h"

struct diag_ops diag_stub = {
	do_nothing,
	do_nothing,
	do_nothing,
	do_nothing,
	do_nothing
};

struct diag_ops* current_diag = &diag_stub;;

void diag_setup(struct diag_ops* ndiag){current_diag = ndiag;};

int diag_putc(int c){
	return current_diag->putc(c);
};

int diag_getc(){
	return current_diag->getc();
};


int diag_write(void* buf, int len){
	return current_diag->write(buf, len);
};


int diag_read(void* buf, int len){
	return current_diag->read(buf, len);
};

int diag_init(){
	return current_diag->init();
};

int diag_write_stub(void* buf, int len)
{
	char *ptr = buf;
	for (;len;len--) diag_putc((int)*ptr++);
};


int diag_read_stub(void* buf, int len)
{
	char *ptr = buf;
	for (;len;len--) *ptr++ = diag_getc();
};