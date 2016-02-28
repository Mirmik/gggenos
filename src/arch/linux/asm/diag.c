#include "kernel/diag.h"
#include "stdio.h"
#include "util/stub.h"

int local_write(void* buf, int len) {return fwrite(buf, sizeof(char), len, stdout);};
int local_read(void* buf, int len) {return fread(buf, sizeof(char), len, stdin);};

struct diag_ops stdout_diag = {
	putchar, 
	getchar, 
	local_write, 
	local_read, 
	(diag_init_t) do_nothing}; 
