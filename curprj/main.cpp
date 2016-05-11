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
#include "genos/pubsub/msgtypes.h"

#include "genos/terminal/mserver.h"
#include "utilxx/mstorage.h"

#define MESSAGE_LEN 16

LinuxFileStream drv;

genos::uScheduler uSched;

MitsubishiCommunicator mcommunicator;

genos::MServer mserv;

using namespace genos::messagecore;

void loop();
void setup();

int main()
{	setup();
	while(1) loop();
};

void message_parse(char* mes, unsigned int len)
{
	genos::mstore* ms = new genos::mstore;
	ms->reserve(len);
	memcpy(ms->voidptr(), mes, len);
	mserv.recv(ms);
};

void recvall(int dsc, char* mes, unsigned need)
{
    int cur = 0;
    int k = 0;

	while(1)
    {
    	k = recv(dsc, mes + cur, need, 0);
    	cur += k;
    	if (cur == need) break;
    };
};

void* request_handler(void* _client)
{
    TcpServer::Client* client = reinterpret_cast<TcpServer::Client*>(_client);
    int dsc = client->socket;
    char mes[MESSAGE_LEN];
    
    uint16_t len;
    while(1)
    {
   		recvall(dsc, mes, 2);
   		len = ((uint16_t)mes[0] << 8) + (uint16_t)mes[1];
   		recvall(dsc, mes, len);
   		message_parse(mes, len);
   	};
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