#ifndef GENOS_VT102_LIB
#define GENOS_VT102_LIB

#include "syscontext/syscontext.h"
#include "genos/terminal/keycom.h"


 /* define the Key codes */
#define KEY_NUL 0 /**< ^@ Null character */
#define KEY_SOH 1 /**< ^A Start of heading, = console interrupt */
#define KEY_STX 2 /**< ^B Start of text, maintenance mode on HP console */
#define KEY_ETX 3 /**< ^C End of text */
#define KEY_EOT 4 /**< ^D End of transmission, not the same as ETB */
#define KEY_ENQ 5 /**< ^E Enquiry, goes with ACK; old HP flow control */
#define KEY_ACK 6 /**< ^F Acknowledge, clears ENQ logon hand */
#define KEY_BEL 7 /**< ^G Bell, rings the bell... */
#define KEY_BS  8 /**< ^H Backspace, works on HP terminals/computers */
#define KEY_HT  9 /**< ^I Horizontal tab, move to next tab stop */
#define KEY_LF  10  /**< ^J Line Feed */
#define KEY_VT  11  /**< ^K Vertical tab */
#define KEY_FF  12  /**< ^L Form Feed, page eject */
#define KEY_CR  13  /**< ^M Carriage Return*/
#define KEY_SO  14  /**< ^N Shift Out, alternate character set */
#define KEY_SI  15  /**< ^O Shift In, resume defaultn character set */
#define KEY_DLE 16  /**< ^P Data link escape */
#define KEY_DC1 17  /**< ^Q XON, with XOFF to pause listings; "okay to send". */
#define KEY_DC2 18  /**< ^R Device control 2, block-mode flow control */
#define KEY_DC3 19  /**< ^S XOFF, with XON is TERM=18 flow control */
#define KEY_DC4 20  /**< ^T Device control 4 */
#define KEY_NAK 21  /**< ^U Negative acknowledge */
#define KEY_SYN 22  /**< ^V Synchronous idle */
#define KEY_ETB 23  /**< ^W End transmission block, not the same as EOT */
#define KEY_CAN 24  /**< ^X Cancel line, MPE echoes !!! */
#define KEY_EM  25  /**< ^Y End of medium, Control-Y interrupt */
#define KEY_SUB 26  /**< ^Z Substitute */
#define KEY_ESC 27  /**< ^[ Escape, next character is not echoed */
#define KEY_FS  28  /**< ^\ File separator */
#define KEY_GS  29  /**< ^] Group separator */
#define KEY_RS  30  /**< ^^ Record separator, block-mode terminator */
#define KEY_US  31  /**< ^_ Unit separator */

#define KEY_DEL 127 /**< Delete (not a real control character...) */

#define IS_CONTROL_CHAR(x) ((x)<=31)

#define VT102_LEFT "\033[D"
#define VT102_DEL "\033[P"


#define IS_ESCAPE_END_SYMBOL(c) \
	((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c == '~'))

constexpr uint8_t VT102_CMD_CHAR = 0;
constexpr uint8_t VT102_CMD_NONE = 1;
constexpr uint8_t VT102_CMD_UP = 2;
constexpr uint8_t VT102_CMD_DOWN = 3;
constexpr uint8_t VT102_CMD_LEFT = 4;
constexpr uint8_t VT102_CMD_RIGHT = 5;
constexpr uint8_t VT102_CMD_CONTROL = 6;

constexpr uint8_t VT102_MODE_NORMAL = 0;
constexpr uint8_t VT102_MODE_ESCAPE = 1;
constexpr uint8_t VT102_MODE_ESCAPE0 = 2;
constexpr uint8_t VT102_MODE_ESCAPE1 = 3;

struct vt102_cmd
{
	uint8_t type = 0;
	uint8_t data0 = 0;
	uint8_t data1 = 0;
	uint8_t data2 = 0;

	vt102_cmd(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
	: type(a), data0(b), data1(b), data2(c) {};
};

class vt102_analyzer
{
public:
	char memory[10];
	char* mcursor;

	uint8_t mode = VT102_MODE_NORMAL;
	uint8_t state = 0;

	vt102_cmd operator()(char c)
	{
		switch(mode)
		{
			case VT102_MODE_NORMAL:
				if (IS_CONTROL_CHAR(c))
				{
					return vt102_cmd(VT102_CMD_CONTROL, c, 0, 0);
				}
				else if (c == '\033')
				{
					mode = VT102_MODE_ESCAPE;
					return vt102_cmd(VT102_CMD_NONE, 0, 0, 0); 					
				}
				else
				{
					return vt102_cmd(VT102_CMD_CHAR, c, 0, 0);
				};
			case VT102_MODE_ESCAPE:
				if (c == '[')
				{
					state = 0;
					mode = VT102_MODE_ESCAPE0;
					return vt102_cmd(VT102_CMD_NONE, 0, 0, 0);
				}
				else
				{
					state = 0;
					mode = VT102_MODE_ESCAPE1;
					return vt102_cmd(VT102_CMD_NONE, 0, 0, 0);
				};
			case VT102_MODE_ESCAPE0:


			case VT102_MODE_ESCAPE1:
				debug_panic("VT102_MODE_ESCAPE1");

			default:
				debug_panic("DefaultVT102analyzer");
		};
	};

	vt102_analyzer() : mcursor(memory) {};
};













/*
class VT102_output : public KeyCom
{
public:
	int putc(char c) {out->putc(c);};
	int cursor_up(uint8_t n);
	int cursor_down(uint8_t n);
	int cursor_right(uint8_t n);
	int cursor_left(uint8_t n);

	int cursor_up();
	int cursor_down();
	int cursor_right();
	int cursor_left();

	int cursor_position(uint8_t l, uint8_t c);
	int cursor_home();

	int backspace();
	int delchar(uint8_t n);


	stream* out;
};


class VT102_input : public stream
{
	static constexpr int NORMALMODE = 0; 
	static constexpr int ESCAPEMODE = 1;
	static constexpr int MEMORY_SIZE = 16;

public:
	char memory[MEMORY_SIZE];
	char* mcursor;

	uint8_t mode;
	uint8_t escape_state;

	VT102_input(KeyCom * _out);
	int putc(uint8_t c);
	

	void wrong_escape();
	void begin_escape_mode();
	void end_escape_mode();	

	int escape_mode_handler(char c);
	int normal_mode_handler(char c);

	
	int up (int);
	int down (int);
	int left (int);
	int right (int);


	KeyCom* out;
};*/

#endif