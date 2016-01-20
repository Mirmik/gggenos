

//#include <elf.h>
//#include "libc.h"

int __libc_start_main(
	int (*main)(), int argc, char **argv,
	int (*init)(int, char **, char **), void (*fini)(void),
	void (*ldso_fini)(void))
{
	main();
	while(1);
	return 0;
}
