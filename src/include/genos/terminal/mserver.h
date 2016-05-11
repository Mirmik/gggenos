#ifndef GENOS_MSERVER_H
#define GENOS_MSERVER_H

#include "genos/container/dlist.h"
#include "utilxx/mstorage.h"
#include "genos/gstl/algorithm.h"
#include "genos/sigslot/sigslot.h"

namespace genos {

class MServer
{
public:
	class record
	{
	public:
		dlist_head lst;
		sigslot<void, void*> ss;
		int topic;
	};

	dlist<record,&record::lst> reclist;

	void recv(mstore* ms)
	{
		dprln("here");
		delete ms;
	};

	void add_handler(int topic, delegate<void, void*> dlg)
	{
		auto ret = gstd::find_if(reclist.begin(), reclist.end(), [=](genos::MServer::record& rec)
		{
			return rec.topic == topic;
		});
		
		if (reclist.end() == ret) dprln("fasdfasdf");		
	};
};

};
#endif