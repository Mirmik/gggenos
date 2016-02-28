#ifndef GENOS_EVIDENT_TABLE
	#define GENOS_EVIDENT_TABLE
	
	#include "gstd/string.h"
	
	gstd::static_revstring<21> to_string_void(void*);
	gstd::static_revstring<21> to_string_int16(void*);
	gstd::static_revstring<21> to_string_ptr(void*);
	
	struct evident_record
	{
	const char* type;
	gstd::static_revstring<21>(*to_string)(void*);
	
	evident_record(const char* _type, gstd::static_revstring<21>(*_to_string)(void*)) : type(_type), to_string(_to_string){};
	};
	
	
	struct evident_table_t
	{
	evident_record rec[25] = {
		{"unknow", &to_string_void},
		{"int8", &to_string_void},
		{"int16", &to_string_int16},
		{"int32", &to_string_void},
		{"int64", &to_string_void},
		{"uint8", &to_string_void},
		{"uint16", &to_string_void},
		{"uint32", &to_string_void},
		{"uint64", &to_string_void},
		{"char", &to_string_void},
		{"char*", &to_string_ptr},
		{"const char*", &to_string_void},
		{"process_base*", &to_string_ptr},
		{"scheduler_base*", &to_string_ptr},
		{"rtdelegate*", &to_string_void},
		{"cdelegate*", &to_string_void},
		{"thread*", &to_string_void},
		{"static_string*", &to_string_void},
		{"static_revstring*", &to_string_void},
		{"static_string", &to_string_void},
		{"static_revstring", &to_string_void},
		{"unsigned char*", &to_string_ptr},
		{"list_head", &to_string_void},
		{"list_head*", &to_string_ptr},
		{"void*", &to_string_ptr}
		};
	
	
	};
	extern evident_table_t evident_table;
	
	
	
	
	
	
	
	
	
#endif