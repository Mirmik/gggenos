#ifndef GENOS_DECORATION
#define GENOS_DECORATION

#include "genos/io/stream.h"
#include "utilxx/string.h" 

#define GENOS_VERSION "0.8.1"	

void print_banner(stream* strm);
void print_about(stream* strm);

extern string machine_name;

#endif