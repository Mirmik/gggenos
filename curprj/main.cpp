#include "genos/debug/debug.h"
#include "kernel/diag.h"
#include "hal/arch.h"

#include "asm/Serial.h"
#include "genos/schedproc/ucontextScheduler.h"
#include "genos/kernel/waitserver.h"
#include "genos/kernel/time.h"
#include "socketlib/tcp.h"

#include "genos/terminal/readline.h"

#include "genos/pubsub/serialize.h"

#include "mrs.h"

#define MESSAGE_LEN 16

LinuxFileStream drv;

uScheduler uSched;

MitsubishiCommunicator mcommunicator;

void loop();
void setup();

int main()
{	setup();
	while(1) loop();
};

struct Message
{
	uint32_t a;
	uint32_t b;
	uint32_t c;
	uint32_t d;	
};


void message_parse(char* mes)
{
	Message msg;
	mes += deserialize(mes, msg.a);
	mes += deserialize(mes, msg.b);
	mes += deserialize(mes, msg.c);
	mes += deserialize(mes, msg.d);
	
	dprln(msg.a);
	dprln(msg.b);
	dprln(msg.c);
	dprln(msg.d);
};

void* request_handler(void* _client)
{
    TcpServer::Client* client = reinterpret_cast<TcpServer::Client*>(_client);
    int dsc = client->socket;

    Readline<20> rl;

    char c;
    char mes[MESSAGE_LEN];
    char buffer[MESSAGE_LEN];
    int k;

    init:
    rl.init();

    start:
	k = read(dsc,&c,1);
	if (k == 0) goto start;
	rl.putc(c);

	if (rl.length() == MESSAGE_LEN)
		{
			memcpy(mes, rl.get_line(), MESSAGE_LEN);
			message_parse(mes);			
			goto init;
		};

	goto start;
};

void setup_command_server()
{
	TcpServer serv;
    serv.begin();

    //dprln("here");
    serv.bind(9666);
    //dprln("here");
    serv.listen(10);
    //dprln("here");
    serv.cycling_accept(request_handler);
};






void setup()
{

	arch_init();	
	diag_init();

	uSched.init();

	setup_command_server();

	mcommunicator.Open("/dev/ttyS3");
	mcommunicator.Send("\001201\00200\003F8",10);

//delay(100);

//	while(mcommunicator.mrs.available()) debug_putchar(mcommunicator.mrs.getc());
	//mcommunicator.mrs.close_out();
	//mcommunicator.mrs.close_in();

	//uSched.registry(task);
	uSched.registry(delegate_mtd(&mcommunicator, &MitsubishiCommunicator::exec));
};

void loop()
{
	uSched.schedule();
	waitserver.check();
};