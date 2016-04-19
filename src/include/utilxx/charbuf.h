#ifndef GENOS_CHARBUF_H
#define GENOS_CHARBUF_H

template<unsigned int size>
class charbuf
{
public:
	char data[size + 1];
	char& operator[](int i)
	{
		return data[i];
	};

	char* to_buf()
	{
		return data;
	};

	int length()
	{
		return size;
	};

	charbuf() { data[size] = 0; };
};

class charptr
{
public:
	char* data;
	int size;

	char& operator[](int i)
	{
		return *(data + i);
	};

	char* to_buf()
	{
		return data;
	};

	int length()
	{
		return size;
	};

	charptr(char* d, int sz) : data(d), size(sz) {};
};


#endif