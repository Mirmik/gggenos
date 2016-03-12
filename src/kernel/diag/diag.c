

#include "kernel/diag.h"
#include "assert.h"
#include "stddef.h"
#include "util/stub.h"

struct diag_ops diag_stub = {
	(diag_putc_t) do_nothing,
	(diag_getc_t) error_stub,
	(diag_write_t) do_nothing,
	(diag_read_t) error_stub,
	//(diag_available_t) do_nothing,
	//(diag_flush_t) do_nothing,
	(diag_init_t) do_nothing
};

struct diag_ops* current_diag = &diag_stub;;

void diag_setup(struct diag_ops* ndiag){current_diag = ndiag;};

int diag_putc(int c){
	return current_diag->putc(c);
};

int diag_getc(){
	//assert(current_diag->getc != do_nothing);
	return current_diag->getc();
};

int diag_write(void* buf, int len){
	return current_diag->write(buf, len);
};

int diag_read(void* buf, int len){
	//assert(current_diag->read != do_nothing);
	return current_diag->read(buf, len);
};

//int diag_available(){
//	assert(current_diag->available != do_nothing);
//	return current_diag->available();
//};

//int diag_flush(){
//	assert(current_diag->flush != do_nothing);
//	return current_diag->flush();
//};

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