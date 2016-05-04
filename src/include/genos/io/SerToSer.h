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
#include "genos/gstl/utility.h"
#include "kernel/mutex.h"
#include "genos/time/sysclock.h"
#include "genos/kernel/time.h"

extern Serial_t<512,512> Serial6;
class SerToSer
{
	constexpr static uint32_t BREAK_END_TIMEOUT = 10;
public:
	mutex mtx;

	static constexpr uint8_t TIMEOUT = 0x0F;
	static constexpr uint8_t OK = 0x00;

	TimWaiter timsendWD;

	delegate<void, void*> watchDogDlg;
	direct_stream strm;
	numcmd_list* ncmd;
	Readline<20> rl;
	charbuf<14> answer;
	volatile uint8_t flag;
	volatile uint8_t status;
	int mode = 0;

	string last_message;
	
public:

	SerToSer() : strm(), mtx()
	{
		watchDogDlg = delegate_mtd(this,&SerToSer::watchDogFunc);
	};

	void watchDogFunc(void* ptr)
	{
		//strm.print("TimeOut");
		status = TIMEOUT;
		flag = 1;
	};

	charbuf<14> mbuild(int addrfunc, charbuf<8> mes)
	{
		charbuf<14> ret;
		charbuf<2> addrfunc_char = uint8_to_hexcode(addrfunc);
		ret[0] = 1;
		ret[13]  = 3;
		memcpy(&ret[1], addrfunc_char.to_buf(), 2);
		memcpy(&ret[3], mes.to_buf(), 8);
		ret[11] = ret[12] = 0;
		uint8_t cs = ret.controlsum();
		charbuf<2> ch_buf = uint8_to_hexcode(cs);
		memcpy(&ret[11], ch_buf.to_buf(), 2); 
		return ret;
	};

	charbuf<14> mbuild_answer(int addrfunc, charbuf<8> mes)
	{
		charbuf<14> ret;
		charbuf<2> addrfunc_char = uint8_to_hexcode(addrfunc);
		ret[0] = 2;
		ret[13]  = 3;
		memcpy(&ret[1], addrfunc_char.to_buf(), 2);
		memcpy(&ret[3], mes.to_buf(), 8);
		ret[11] = ret[12] = 0;
		uint8_t cs = ret.controlsum();
		charbuf<2> ch_buf = uint8_to_hexcode(cs);
		memcpy(&ret[11], ch_buf.to_buf(), 2); 
		return ret;
	};

	uint8_t send(charbuf<14>* msg)
	{
		mtx.enter();
		flag = 0;
		last_message = msg->c_str();
		strm.print(last_message.c_str());
		waitserver.delegate_on_simple_timer(watchDogDlg, 
			(void*)0, &timsendWD, BREAK_END_TIMEOUT);

		charbuf<14> sim;
		sim[0] = 2;
		sim[13]  = 3;
		memcpy(&sim[1], "FF", 2);
		memcpy(&sim[3], "00000000", 8);
		sim[11] = sim[12] = 0;
		uint8_t cs = sim.controlsum();
		charbuf<2> ch_buf = uint8_to_hexcode(cs);
		memcpy(&sim[11], ch_buf.to_buf(), 2);

		Serial6.simulation_input(sim.c_str(),14);


		wait((uint8_t*)&flag);
		mtx.leave();
		*msg = answer;
		return status;
	};


	void send_answer(charbuf<14>* msg)
	{
		mtx.enter();
		flag = 0;
		last_message = msg->c_str();
		strm.print(last_message.c_str());
		mtx.leave();
	};

	uint8_t send(int addrfunc, uint32_t num)
	{
		auto msg = mbuild(addrfunc, uint32_to_hexcode(num));
		return send(&msg);
	};

	void send_answer(uint32_t mes)
	{	
		auto msg = mbuild_answer(255, uint32_to_hexcode(mes));
		send_answer(&msg);
	};

	uint8_t checksum(char* str)
	{
		uint8_t sum;
		uint8_t csum;
		for (int i = 0; i < 11; i++)
			{
				sum += *(str + i);
			};
		sum += *(str + 13);
		csum = hexcode_to_uint8(str + 11);
		if (sum != csum) return -1;
		return 0;
	};

	void parse_answer(char* str)
	{
		memcpy(answer.to_buf(), str, 14);
		status = 0;
		//Serial6.print("XXXXXXXXX");
  		waitserver.unwait((TimWaiter*)&timsendWD);
		flag = 1;
		rl.init();
		mode = 0;
	};

	void parse_message(char* str)
	{
		uint8_t code = hexcode_to_uint8(str + 1);
		delegate<uint32_t,uint32_t> dlg;
		int f = ncmd->find(code,dlg);
		string ret(str + 3, 8);
		send_answer(dlg(hexcode_to_uint32(ret.c_str())));
		rl.init();
		mode = 0;
	}

	void parse(char* str)
	{
		if (checksum(str)) {wrong_frame(); return;};
		if (*str != 1 && *str != 2) {wrong_receive(7); return;};
		if (*str == 2) parse_answer(str);
		if (*str == 1) parse_message(str);
	};

	void wrong_frame()
	{
		strm.print("\001XX\003");	
		rl.init();
		mode = 0;	
	};

	void wrong_receive(int code)
	{
		rl.init();
		mode = 0;
	};

	void receive()
	{
		parse(rl.get_line());
		rl.init();
		mode = 0;
	};

	void resend_impl()
	{
		strm.print(last_message.c_str());
	}

	void resend()
	{
		char* str = rl.get_line();
		if (*(str + 0) == '\001' &&
			*(str + 1) == 'X' &&
			*(str + 2) == 'X' &&
			*(str + 3) == '\003') resend_impl();
		rl.init();
		mode = 0;
	};

	uint32_t last_receive;
	void exec()
	{
		while(1)
		{
			wait(&strm);

			if (rl.length() == 0) last_receive = millis();
			if (millis() - last_receive > 20) wrong_receive(6);

			_start:
			while (strm.available())
			{
				int c = strm.getc();
				if (c == 1) rl.init();
				rl.putc(c);
				if (c < 0) {wrong_receive(3); goto _start;};
					if (rl.length() > 14) {wrong_receive(4); goto _start;}; 
					if (c == 3 && rl.length() == 4) 
						{resend();goto _start;};
					if (c == 3 && rl.length() == 14) 
						{receive();goto _start;};
					if (c == 3 && rl.length() != 14) 
						{wrong_receive(5);goto _start;};
			};
		};
	};	  
};

#endif