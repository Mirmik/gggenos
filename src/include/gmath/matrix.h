#ifndef GENOS_MATRIX_H
#define GENOS_MATRIX_H

template <typename type>
class Matrix
{
public:
	type* buffer;
	unsigned int rows;
	unsigned int columns;
	unsigned int & N = columns;
	unsigned int & M = rows;		  

	void* operator new (size_t sz, type* ptr)
	{
		return ptr;
	};

	void overall_destructor()
	{
		type* end = buffer + columns * rows;
		for(type* ptr = buffer; ptr != end; ++ptr)
			destructor(ptr);
	};

	void destructor(type* ptr)
	{
		ptr -> type::~type();
	};

	Matrix()
	{ 
		dprln("cnstr");
		init(); 
	};

	~Matrix()
	{
		dprln("destr");
		if (buffer) free(buffer); 
	};

	Matrix( const Matrix& m )
	{
		dprln("cnstr &");
		init();
		copy(m.buffer, m.columns, m.rows);
	};

	Matrix( Matrix&& m )
	{
		dprln("cnstr &&");
		init();
		move(m);
	};	

	void copy( type* ptr, unsigned int new_columns, unsigned int new_rows )
	{
		if (buffer) { overall_destructor(); free (buffer); };
		buffer = (type*) malloc(new_rows * new_columns * sizeof(type));
		for(int i = 0; i < new_rows * new_columns; ++i)
			new (buffer + i) type(*(ptr + i));		
		rows = new_rows;
		columns = new_columns;
	};

	void move( Matrix& m )
	{
		if (buffer) {overall_destructor(); free(buffer);}
		buffer = m.buffer;
		rows = m.rows;
		columns = m.columns;
		m.buffer = nullptr;
	};

	void create(unsigned int _columns, unsigned int _rows)
	{
		if (buffer) { overall_destructor(); free (buffer); };
		buffer = (type*) malloc(_columns * _rows * sizeof(type));
		columns = _columns;
		rows = _rows;
		for (int i = 0; i < rows * columns; ++i)
		{
			new (buffer + i) type();	
		};
	};

	void init()
	{
		buffer = nullptr;
		rows = 0;
		columns = 0;
	};

	type & ref (unsigned int n, unsigned int m)
	{
		return *(buffer + m * N + n);
	};

	string to_str()
	{
		string str;
		str.reserve(128);
		for(int i = 0; i < columns; ++i)
		{
			for(int j = 0; j < rows; ++j)
			{
				str << ref(i,j) << " ";
			};
			str << "\r\n";
		};
		return str;
	};
};

template<typename type>
Matrix<type> operator + (Matrix<type>&& lhs, const Matrix<type>& rhs)
{
	dprln("m + && &");
	assert(lhs.buffer);
	assert(rhs.buffer);
	assert(lhs.rows == rhs.rows && rhs.columns == lhs.columns);
	for(int i = 0; i < lhs.rows * lhs.columns; i++)
		{
			*(lhs.buffer + i) += *(rhs.buffer + i);
		};
	return gstd::move(lhs);
};

template<typename type>
Matrix<type> operator + (const Matrix<type>& lhs, Matrix<type>&& rhs)
{
	dprln("m + & &&");
	assert(lhs.buffer);
	assert(rhs.buffer);
	assert(lhs.rows == rhs.rows && rhs.columns == lhs.columns);
	for(int i = 0; i < lhs.rows * lhs.columns; i++)
		{
			*(rhs.buffer + i) += *(lhs.buffer + i);
		};
	return gstd::move(rhs);
};

template<typename type>
Matrix<type> operator + (Matrix<type>&& lhs, Matrix<type>&& rhs)
{
	dprln("m + && &&");
	assert(lhs.buffer);
	assert(rhs.buffer);
	assert(lhs.rows == rhs.rows && rhs.columns == lhs.columns);
	for(int i = 0; i < lhs.rows * lhs.columns; i++)
		{
			*(rhs.buffer + i) += *(lhs.buffer + i);
		};
	return gstd::move(rhs);
};

template<typename type>
Matrix<type> operator + (const Matrix<type>& lhs, const Matrix<type>& rhs)
{
	dprln("m + & &");
	assert(lhs.buffer);
	assert(rhs.buffer);
	assert(lhs.rows == rhs.rows && rhs.columns == lhs.columns);

	Matrix<type> mtrx;
	mtrx.create(lhs.columns, lhs.rows);
	for(int i = 0; i < lhs.rows * lhs.columns; i++)
		{
			*(mtrx.buffer + i) = *(lhs.buffer + i) + *(rhs.buffer + i);
		};
	return mtrx;
};
#endif