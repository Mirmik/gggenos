#ifndef GENOS_TO_STRING_H
#define GENOS_TO_STRING_H

#include "utilxx/string.h"

template<typename type>
genos::string to_str(const type& value)
{
	return value.to_to_str();
};

genos::string to_str(const int value);

genos::string to_str(const short int value);

genos::string to_str(const long int value);

//string to_str(const long long int value)
//{
//	return string(value);
//};

genos::string to_str(const unsigned int value);

genos::string to_str(const unsigned short int value);

genos::string to_str(const unsigned long int value);

//string to_str(const unsigned long long int value)
//{
//	return string(value);
//};

#endif