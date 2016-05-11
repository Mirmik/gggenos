#ifndef GENOS_PUBSUB_MESSAGE_TYPES_H
#define GENOS_PUBSUB_MESSAGE_TYPES_H

#include "genos.h"
#include "utilxx/charbuf.h"
#include "genos/pubsub/serialize.h"

namespace genos {
namespace messagecore {

	struct message_header
	{
		uint16_t len;
		uint8_t protocol;
		charbuf<8> topicfunc;
		charbuf<8> machine;

		unsigned int serialize(const char* ptr)
		{
			char* _ptr = (char*)ptr;
			_ptr += genos::serialize(_ptr, len);
			_ptr += genos::serialize(_ptr, protocol);
			_ptr += genos::serialize(_ptr, topicfunc);
			_ptr += genos::serialize(_ptr, machine);
			return _ptr - ptr;
		};

		unsigned int deserialize(const char* ptr)
		{
			char* _ptr = (char*)ptr;
			_ptr += genos::deserialize(_ptr, len);
			_ptr += genos::deserialize(_ptr, protocol);
			_ptr += genos::deserialize(_ptr, topicfunc);
			_ptr += genos::deserialize(_ptr, machine);
			return _ptr - ptr;
		};

		string to_str()
		{
			string ret;
			ret <<  "protocol:" << string(protocol) << "\r\n";
			ret <<  "topicfunc:" << string(topicfunc.to_buf()) << "\r\n";
			ret <<  "machine:" << string(machine.to_buf()) << "\r\n";
			return ret;
		};
	};

	template <typename type>
	struct msg : public message_header 
	{	
		type value;

		string to_str()
		{
			string ret = message_header::to_str();
			ret << "value:\r\n" << ::to_str(value) << "\r\n"; 
			return ret;
		};
	};

	struct parsed_message
	{
		message_header header;
		void* data;		
	};

};
};

#endif