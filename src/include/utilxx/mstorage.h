#ifndef GENOS_MEMORY_STORAGE_H
#define GENOS_MEMORY_STORAGE_H

#include "stdlib.h"

namespace genos {

class mstore
{
	void* buffer;
	uint16_t len;

public:
	mstore() { init(); };

	~mstore() 
	{
		if (buffer) free(buffer);
	}

	template <typename type>
	void store(type& value)
	{
		changeBuffer(sizeof(type));
		*(type*)buffer = value;
		len = sizeof(type);
	};

	template <typename type>
	type& ref()
	{
		return *reinterpret_cast<type*>(buffer);
	}

	template <typename type>
	type* ptr()
	{
		return reinterpret_cast<type*>(buffer);
	}

	void* voidptr()
	{
		return buffer;
	}

	void reserve(size_t sz)
	{
		changeBuffer(sz);
	};

	void changeBuffer(size_t sz)
	{
		if (buffer) free(buffer);
		buffer = malloc(sz);
	};

	void invalidate()
	{
		if (buffer) free(buffer);
		init();
	};

	void init()
	{
		buffer = nullptr;
	};
};

};

#endif