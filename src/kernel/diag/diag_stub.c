

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

void diag_setup(struct diag_ops* ndiag){};

int diag_putc(int c){};
int diag_getc(){};
int diag_write(void* buf, int len){};
int diag_read(void* buf, int len){};

int diag_init(){};

int diag_write_stub(void* buf, int len){};
int diag_read_stub(void* buf, int len){};