#include "util/vt102/vt102.h"

int VT102_output::cursor_up(uint8_t n)
{
	out->print("\033[");
	out->print(n);
	out->print("A");
	return n;
};

int VT102_output::cursor_down(uint8_t n)
{
	out->print("\033[");
	out->print(n);
	out->print("B");
	return n;
};

int VT102_output::cursor_right(uint8_t n)
{
	out->print("\033[");
	out->print(n);
	out->print("C");
	return n;
};

int VT102_output::cursor_left(uint8_t n)
{
	out->print("\033[");
	out->print(n);
	out->print("D");
	return n;
};

int VT102_output::cursor_up()
{
	out->print("\033[A");
	return 1;
};

int VT102_output::cursor_down()
{
	out->print("\033[B");
	return 1;
};

int VT102_output::cursor_right()
{
	out->print("\033[C");
	return 1;
};

int VT102_output::cursor_left()
{
	out->print("\033[D");
	return 1;	
};

int VT102_output::cursor_position(uint8_t l, uint8_t c)
{
	out->print("\033[");
	out->print(l);
	out->print(";");
	out->print(c);
	out->print("H");
	return 1;
};

int VT102_output::cursor_home()
{
	out->print("\033[");
	out->print("H");
	return 1;
};

//int VT102_output::backspace(uint8_t n)
//{

//};

int VT102_output::backspace()
{
	out->putc('\010');
	delchar(1);
	return 1;
};

int VT102_output::delchar(uint8_t n)
{
	out->print("\033[");
	out->print(n);
	out->print("P");	
	return n;
};














