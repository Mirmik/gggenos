#ifndef GENOS_DECORATION
#define GENOS_DECORATION

#include "genos/io/stream.h"

#define GENOS_VERSION "0.8.1"	

static void print_banner(stream* strm)
{
	strm->println("  __             __   __ ");
	strm->println(" /              /  | /   ");
	strm->println("( __  ___  ___ (   |(___ ");
	strm->println("|   )|___)|   )|   )    )");
	strm->println("|__/ |__  |  / |__/  __/ ");
};

static void print_about(stream* strm)
{
	strm->println("automTerminal");
	strm->print("Operation System GenOS. version: ");
	strm->println(GENOS_VERSION);
};

extern const char* machine_name;

#endif