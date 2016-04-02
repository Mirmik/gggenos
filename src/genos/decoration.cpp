#include "genos/io/stream.h"
#include "genos/decoration.h"

const char* machine_name = "input"; 

void print_banner(stream* strm)
{
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
