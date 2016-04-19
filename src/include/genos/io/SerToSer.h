#ifndef GENOS_SERTOSER_H
#define GENOS_SERTOSER_H

#include "genos.h"
#include "hal/arch.h"
#include "genos/container/dlist.h"
#include "genos/io/stream.h"
#include "genos/kernel/waitserver.h"
#include "util/ascii_codes.h"
#include "utilxx/controlsum.h"
#include "genos/terminal/readline.h"
#include "genos/terminal/numcmd_list.h"

class SerToSer
{
public:
	direct_stream strm;
	numcmd_list* ncmd;
	Readline<40> rl;
	int mode = 0;

public:

	struct message
	{
		dlist_head lst;
		string mes;
		int addrfunc;
		int id;
		string ans;
		uint8_t flag;
	};

	dlist<SerToSer::message, &SerToSer::message::lst> active_list;
	dlist<SerToSer::message, &SerToSer::message::lst> passive_list;

	dlist<SerToSer::message, &SerToSer::message::lst> input_list;
	
	SerToSer() : strm(), active_list(), passive_list() {};

	void sendmessage(message& msg)
	{
		sreg_t save;
		arch_atomic_temp(save);
		active_list.push_back(msg);
		arch_deatomic_temp(save);
	};

	string mbuild(string mes, int addrfunc, int id)
	{
		charbuf<2> addrfunc_char = uint_to_hexcode2(addrfunc);
		charbuf<4> id_char = uint_to_hexcode4(id);
		charbuf<2> len_char = uint_to_hexcode2(mes.length());

		string ret;
		ret.reserve(128);
		ret << (char)1
		<< id_char 
		<< addrfunc_char 
		<< len_char
		<< (char)2
		<< mes
		<< (char)3;
		charbuf<2> controlsum_char = uint_to_hexcode2(controlsum(ret) & 0xFF);
		ret << controlsum_char;		
		return ret;
	};

	void send_answer(string mes, uint16_t id)
	{
		charbuf<4> id_char = uint_to_hexcode4(id);
		charbuf<2> len_char = uint_to_hexcode2(mes.length());
		string str;
		str.reserve(128);
		str << (char)2
		<< id_char
		<< len_char
		<< mes
		<< (char)3;
		charbuf<2> controlsum_char = uint_to_hexcode2(controlsum(str) & 0xFF);
		str << controlsum_char;
		strm.print(str.c_str());	
	};

	void analize_received_message(string str, uint16_t id, uint8_t addr)
	{
		//SerToSer::message* msg = new SerToSer::message();

		//msg->mes = str;
		//msg->addrfunc = addr;
		//msg->id = id;
		
		delegate<string,string> dlg;
		int findsts = ncmd->find(addr,dlg);

		if (findsts == -1) return;

		string ret = dlg(str);
		send_answer(ret, id);
	};

	void parse(char* str)
	{
		if (*str != 1 && *str != 2) wrong_receive(2);
		charptr id_char		(str + 1, 4);
		charptr addr_char	(str + 5, 2);
		charptr len_char	(str + 7, 2);
		uint8_t len = hexcode_to_uint8(len_char.to_buf());
		charptr mes_char(str + 10, len);	
		string res(mes_char);
		uint16_t id = hexcode_to_uint16(id_char.to_buf());
		uint8_t addr = hexcode_to_uint8(addr_char.to_buf());	
		analize_received_message(res, id, addr);
	}

	void wrong_receive(int code)
	{
		strm.print("XXXX"); strm.print((int8_t)code);
	};

	void receive()
	{
		parse(rl.get_line());
		rl.init();
		mode = 0;
	};

	void exec()
	{
		while(1)
		{
			wait_subst(&strm);
			_start:
			while (strm.available())
			{
				int c = strm.getc();
				if (c < 0) {wrong_receive(3); goto _start;};
				rl.putc(c);
				if (mode!=0 || c==3) mode++;
				if (mode == 3) receive();
			};
		};
	};	  
};

#endif