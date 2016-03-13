#include "genos/io/stream.h"
#include "genos/decoration.h"

const char* machine_name = "input"; 

void print_banner(stream* strm)
{
	strm->println(" ______            _ _       _       _             ");
	strm->println("(_____ \\          | (_)     | |     (_)            ");
	strm->println(" _____) ) ____  _ | |_  ___ | |      _ ____   ____ ");
	strm->println("(_____ ( / _  |/ || | |/ _ \\| |     | |  _ \\ / _  )");
	strm->println("      | ( ( | ( (_| | | |_| | |_____| | | | ( (/ / ");
	strm->println("      |_|\\_||_|\\____|_|\\___/|_______)_|_| |_|\\____)");
};

void print_about(stream* strm)
{
	strm->print("Operation System GenOS. version: ");
	strm->println(GENOS_VERSION);
};
