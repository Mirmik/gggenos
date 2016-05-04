#include "genos.h"
#include "utilxx/charbuf.h"

class MitsubishiCommunicator
{
public:

	LinuxFileStream mrs;

	void Open(const char* str)
	{
		mrs.open_out(str);
		mrs.open_in(str);
	};

	void Send(const char* str, uint8_t len)
	{
		mrs.write(str, len);
	};

	void exec()
	{

		dprln("kekeke");
		exit_subst();
	};

};