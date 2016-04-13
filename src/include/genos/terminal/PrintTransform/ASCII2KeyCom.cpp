

#include "genos/terminal/PrintTransform/ASCII2KeyCom.h"


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

#define IS_ESCAPE_END_SYMBOL(c) \
	((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c == '~'))

// esc seq internal codes
#define _ESC_BRACKET  1
#define _ESC_HOME     2
#define _ESC_END      3

void ASCII2KeyCom::wrong_escape()
{
	debug_print("WrongEscape!!!");
	mode = NORMALMODE;
};


void ASCII2KeyCom::begin_escape_mode()
{
	mode = ESCAPEMODE;
	mcursor = memory;
	escape_state = 0;
};

void ASCII2KeyCom::end_escape_mode()
{
	debug_print("CorrectEscape!!!");
	mode = NORMALMODE;
};

int ASCII2KeyCom::normal_mode_handler(char c)
{
	switch(c){				
		case KEY_ESC:
			begin_escape_mode();
			break;
		
			//-----------------------------------------------------
//			case KEY_NAK: // ^U
					//while (cursor > 0) {
					//rl->backspace ();
				
				//terminal_print_line (0, this->cursor);
//			break;
			//-----------------------------------------------------
//			case KEY_VT:  // ^K
				//this->print ("\033[K");
				//this->cmdlen = this->cursor;
//			break;
			//-----------------------------------------------------
//			case KEY_ENQ: // ^E
				//terminal_move_cursor (this->cmdlen-this->cursor);
				//this->cursor = this->cmdlen;
//			break;
			//-----------------------------------------------------
//			case KEY_SOH: // ^A
				//terminal_reset_cursor ();
				//this->cursor = 0;
//			break;
			//-----------------------------------------------------
//			case KEY_ACK: // ^F
			//if (this->cursor < this->cmdlen) {
				//terminal_move_cursor (1);
				//this->cursor++;
			//}
//			break;
			//-----------------------------------------------------
//			case KEY_STX: // ^B
			//if (this->cursor) {
				//terminal_move_cursor (-1);
				//this->cursor--;
			
//			break;
			//-----------------------------------------------------		
		
//			case KEY_DLE: //^P
//			hist_search (_HIST_UP);
//			break;
			//-----------------------------------------------------
//			case KEY_SO: //^N
//			hist_search (_HIST_DOWN);
//			break;
			//-----------------------------------------------------
			//case KEY_DEL: // Backspace
			//case KEY_BS: // ^U
			//dpr("del");
			//out->putc('\b');
			//break;

//			case KEY_ETX:
			//if (this->sigint != NULL)
			//	this->sigint();
//			break;

		
		
		
		default:
			out->putc((char)c);
			break;	
	}
};

// handling escape sequences
int ASCII2KeyCom::escape_mode_handler(char c)
{
	if (mcursor - memory > MEMORY_SIZE) wrong_escape();
	*mcursor++ = c;
	if (IS_ESCAPE_END_SYMBOL(c)) end_escape_mode();
};

/*
int ASCII2KeyCom::escape_process (char ch)
{
	if (ch == '[') {
		escape_seq = _ESC_BRACKET;
		return 0;
	} else if (escape_seq == _ESC_BRACKET) {
		if (ch == 'A') {
			out->up(1);
			return 1;
		} else if (ch == 'B') {
			out->down(1);
			return 1;
		} else if (ch == 'C') {
			out->right(1);
			return 1;
		} else if (ch == 'D') {
			out->left(1);	
			return 1;
		} else if (ch == '7') {
			escape_seq = _ESC_HOME;
			return 0;
		} else if (ch == '8') {
			escape_seq = _ESC_END;
			return 0;
		} else if (ch == '3') {
			out->del(0);
			return 0;
} 
	} else if (ch == '~') {
		if (escape_seq == _ESC_HOME) {
			//terminal_reset_cursor ();
			//cursor = 0;
			return 1;
		} else if (escape_seq == _ESC_END) {
			//terminal_move_cursor (cmdlen-cursor);
			//cursor = cmdlen;
			return 1;
		}
	}
}*/




int ASCII2KeyCom::putc(uint8_t c)
{
	switch (mode)
	{
		case NORMALMODE: normal_mode_handler(c); break;
		case ESCAPEMODE: escape_mode_handler(c); break;
	};	
	return 1;	
}

ASCII2KeyCom::ASCII2KeyCom(KeyCom * _out) : out(_out) {};
