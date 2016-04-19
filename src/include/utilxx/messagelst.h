#ifndef GENOS_STRING_LIST_H
#define GENOS_STRING_LIST_H

#include "genos/container/dlist.h"
#include "utilxx/string.h"
#include "genos/gstl/algorithm.h"


class messagelst
{
public:
	dlist<message, &message::lst> list;

	void add(int address, const string& str)
	{
		auto lmes = new message;
		lmes->mes = str;
		lmes->address = address;
		list.push_back(*lmes);
	};

	//string peek()
	//{
	//	return (*list.begin()).data;
	//};

	string to_str()
	{
		string str;
		int i = 0;
		str.reserve(128);
		gstd::for_each(list.begin(),list.end(), [i,&str](message& lstr) mutable
			{
				i++;
				str << i << ":" << lstr.mes << "\r\n"; 
			});
		return str;
	};
};

#endif