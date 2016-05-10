#ifndef GENOS_SERIALIZED_H
#define GENOS_SERIALIZED_H

#include "stdint.h"

static inline unsigned int serialize_object(void* buf, const void* obj, size_t size)
{
	char* _obj = (char*) obj;
	char* _buf = (char*) buf;
	char* end = _obj + size;
	while(_obj != end)
	{
		*_buf++ = *_obj++;
	};
	return size;
};

static inline unsigned int deserialize_object(const void* buf, void* obj, size_t size)
{
	char* _obj = (char*) obj;
	char* _buf = (char*) buf;
	char* end = _obj + size;
	while(_obj != end)
	{
		*_obj++ = *_buf++;
	};
	return size;
};

static inline unsigned int serialize(void* buf, const uint8_t& obj)
{
	return serialize_object( buf, &obj, 1 );
};

static inline unsigned int serialize(void* buf, const uint16_t& obj)
{
	return serialize_object( buf, &obj, 2 );
};

static inline unsigned int serialize(void* buf, const uint32_t& obj)
{
	return serialize_object( buf, &obj, 4 );
};

static inline unsigned int serialize(void* buf, const uint64_t& obj)
{
	return serialize_object( buf, &obj, 8 );
};

static inline unsigned int serialize(void* buf, const char* ptr, uint16_t sz)
{
	uint16_t res = 0;
	res += serialize_object( buf, &sz, 2 );
	res += serialize_object( buf + 2, ptr, sz );
	return res;
};

/*
static inline unsigned int serialize(void* buf, const string& str)
{
	uint16_t res = 0;
	uint16_t len = (uint16_t)str.length();
	res += serialize_object( buf, &len, 2 );
	res += serialize_object( buf + 2, str.c_str(), len );
	return res;
};*/

static inline unsigned int deserialize(void* buf, uint8_t& ref)
{
	return deserialize_object(buf, &ref, 1);
};

static inline unsigned int deserialize(void* buf, uint16_t& ref)
{
	return deserialize_object(buf, &ref, 2);
};

static inline unsigned int deserialize(void* buf, uint32_t& ref)
{
	return deserialize_object(buf, &ref, 4);
};

static inline unsigned int deserialize(void* buf, uint64_t& ref)
{
	return deserialize_object(buf, &ref, 8);
};

/*
static inline unsigned int deserialize(void* buf, string& str)
{
	uint16_t res = 0;
	uint16_t len;
	res += deserialize_object( buf, &len, 2 );
	str = string(charptr(((char*)buf) + 2,len));
	res += len;
	return res;
};*/
#endif