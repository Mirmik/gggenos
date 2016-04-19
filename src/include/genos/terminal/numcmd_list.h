#ifndef NUMCMD_LIST
#define NUMCMD_LIST

#include "genos/container/dlist.h"
#include "genos/sigslot/delegate.h"
#include "utilxx/string.h"
#include "genos/gstl/algorithm.h"

struct numcmd_t
{
	dlist_head lst;
	delegate<string,string> dlg;
	uint8_t num;
	numcmd_t(uint8_t _num, delegate<string,string>d): num(_num), dlg(d) {}
};

class numcmd_list
{
public:
	dlist<numcmd_t, &numcmd_t::lst> list;

	void add(uint8_t num, delegate<string,string> d) 
	{
		numcmd_t* cmd = new numcmd_t(num, d); 
		list.push_back(*cmd);
	};	

	int find(uint8_t _num, delegate<string, string>& dlg)
	{
		auto ret = gstd::find_if(list.begin(),list.end(),
		[_num](numcmd_t& ncmd)
		{
			if (ncmd.num == _num) return 1;
			else return 0;
		});
		if (ret == list.end()) return -1;
		dlg = ret->dlg;
		return 1; 				
	};

	numcmd_list():list() {};
};

#endif
