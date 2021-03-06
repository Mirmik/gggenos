#include "genos.h"
#include "genos/io/stream.h"
#include "genos/decoration.h"

string machine_name = "input"; 

void print_banner(stream* strm)
{
	strm->println();
	strm->println("  __             __   __ ");
	strm->println(" /              /  | /   ");
	strm->println("( __  ___  ___ (   |(___ ");
	strm->println("|   )|___)|   )|   )    )");
	strm->println("|__/ |__  |  / |__/  __/ ");
};

void print_about(stream* strm)
{
	strm->print("Operation System GenOS. version: ");
	strm->println(GENOS_VERSION);
};
