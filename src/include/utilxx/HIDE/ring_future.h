#ifndef _GENOS_UTILXX_RING
#define _GENOS_UTILXX_RING



template <typename type>
class ring_basic
{
	type* buffer;
	uint16_t size;
	
	inline bool need_fixup(const type* n)
	{
		return n >= size;
	};

	inline size_t next(size_t n)
	{ 
		++n;
		return need_fixup() ? n : 0; 
	};

	inline size_t next(size_t n, size_t bias)
	{ 
		n += bias;
		while(need_fixup(n)) n-=size;
		return n; 
	};

	//void allocate()
	//{
	//	buffer = (type*) malloc(sizeof(type)*size);
	//};
};

template <typename type>
class ring : public ring_basic<type>
{	
public:
	type* head;
	type* tail;

	void putc(type c)
	{ 
		*head = c;
		head = next(head);
	};

	type getc()
	{ 
		type c = *tail;
		tail = next(tail);
		return c;
	};

	bool is_full() 
	{ 
		return next(head) == tail; 
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
		size + (tail - head) / sizeof(type) - 1 : 
		(tail - head) - 1;
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