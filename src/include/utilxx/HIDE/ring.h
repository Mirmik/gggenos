#ifndef GENOS_UTILXX_RING
#define GENOS_UTILXX_RING

template<typename type>
class ring
{
private:

	inline bool need_fixup(type* ptr)
	{
		return ptr >= buffer + size;
	};

	inline type* next(type* ptr)
	{ 
		++ptr; 
		return need_fixup(ptr) ? buffer : ptr; 
	};

public:
	type* buffer;
	uint16_t size;
	type* head;
	type* tail;

	void putc(type c)
	{ 
		*head++ = c;
		if (head >= buffer + size) head = buffer;
	};

	type getc()
	{ 
		type c = *tail++;
		if (tail >= buffer + size) tail = buffer;
		return c;
	};

	bool is_full() 
	{ 
		return next(head) == tail ? 1 : 0; 
	};
	
	bool is_empty() 
	{ 
		return head == tail;
	};

	int data_size() 
	{ 
		return (head >= tail) ? 
		head - tail : 
		size + head - tail; 
	};

	int room_size()
	{
		return (head >= tail) ? 
		size - 1 + ( tail - head ) : 
		( tail - head ) - 1;
	};

	void init(void* _buffer, uint16_t _size)
	{ 
		buffer = (type*)_buffer;
		size = _size;
		head = buffer;
		tail = buffer;
	};
};


#endif