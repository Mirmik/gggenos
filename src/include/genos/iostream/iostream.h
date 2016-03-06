#ifndef GENOS_IOSTREAM
	#define GENOS_IOSTREAM
	
	#include <genos/iostream/num_put.h>
	//#include <genos/platform/iostream.h>
	//#include <math.h>
	
	namespace genos {		
		
		template<typename T, typename traits = char_traits<T>>
		class basic_istream : public ios<T, traits>	{public:
			Reader<T>* i;
		
			void set(Reader<T>& bi) {i=&bi;};
			
			basic_istream() : i(nullptr) {};
			basic_istream(Reader<T>& _i) : i(&_i) {};
			
			size_t read(){return i->read();};
			size_t available(){return i->available();};
			
			};
		
		
		
		template<typename T, typename traits = char_traits<T>>
		class basic_ostream : public ios<T, traits>	{
			
			static constexpr uint8_t ARRAY_SIZE = 70; 
			
			//Объект, реализующий запись в "устройство"
			Writer<T>* o;
			public:
			void set(Writer<T>& bo) {o=&bo;};
			
			public:
			basic_ostream() : o(nullptr) {};
			basic_ostream(Writer<T>& _o) : o(&_o) {};
			
			public:
			
			//Вывод чисел.
			template<typename Int> 			size_t _print_number(Int n);
			template<typename T_float> 		size_t _print_float(T_float n);
			size_t print(bool b);
			
			//Вывод строки с учетом механизма width.
			size_t print_field(char* c, size_t len);
			
			//Базовый вывод.
			size_t write(T c)			{return o->write(c);};
			size_t write(T* c, size_t n){return o->write(c, n);};
			void   flush()				{o->flush();};
			
			//Адаптации базового вывода.
			size_t print(T c)			{return print_field(&c,1);};
			size_t print(T* c)			{return print_field(c,char_traits<T>().length(c));};
			
			//Перенаправление входа на шаблонные функции.
			size_t print(uint8_t n){return _print_number(n);};
			size_t print(uint16_t n){return _print_number(n);};
			size_t print(uint32_t n){return _print_number(n);};
			size_t print(uint64_t n){return _print_number(n);};
			size_t print(int8_t n){return _print_number(n);};
			size_t print(int16_t n){return _print_number(n);};
			size_t print(int32_t n){return _print_number(n);};
			size_t print(int64_t n){return _print_number(n);};
			size_t print(float n){return _print_float(n);};
			size_t print(double n){return _print_float(n);};
			size_t print(void* n){return _print_number((size_t)n);};
			
			//Вывод дампа
			void print_dump(void* _address, size_t _count);				
			
			
			//Адаптация оператора <<.
			basic_ostream& operator<<(T c)					{o->write(c); return *this;};
			basic_ostream& operator<<(T* c)					{print((char*)c);return *this;};
			
			basic_ostream& operator<<(uint64_t i)			{print(i);return *this;};
			basic_ostream& operator<<(uint32_t i)			{print(i);return *this;};
			basic_ostream& operator<<(uint16_t i)			{print(i);return *this;};
			basic_ostream& operator<<(uint8_t i)			{print(i);return *this;};
			basic_ostream& operator<<(int64_t i)			{print(i);return *this;};
			basic_ostream& operator<<(int32_t i)			{print(i);return *this;};
			basic_ostream& operator<<(int16_t i)			{print(i);return *this;};
			basic_ostream& operator<<(int8_t i)				{print(i);return *this;};
			basic_ostream& operator<<(bool b)				{print(b);return *this;};
			basic_ostream& operator<<(float i)				{print(i);return *this;};
			basic_ostream& operator<<(double i)				{print(i);return *this;};
			basic_ostream& operator<<(void* i)				{print(i);return *this;};
			
			
			class ostr_func{
				public:
				virtual void operator()(basic_ostream&)=0;
			};
			
			basic_ostream& operator<<(ostr_func&& f)		{f(*this); return *this;};		
			basic_ostream& operator<<(basic_ostream& (*pf)(basic_ostream&)){return pf(*this);};	
			
			
		};
		
		
		
		/////Манипуляторы потоков.
		typedef basic_istream<char> istream;
		typedef basic_ostream<char> ostream;
		
		class setw : public ostream::ostr_func{
			public:
			void operator()(ostream& os){os.width(w);};
			setw(int _n): w(_n) {};
			int w;
		};
		
		class setprecision : public ostream::ostr_func{
			public:
			void operator()(ostream& os){os.precision(w);};
			setprecision(int _n): w(_n) {};
			int w;
		};
		
		class setbase : public ostream::ostr_func{
			public:
			void operator()(ostream& os){os.base(w);};
			setbase(int _n): w(_n) {};
			int w;
		};
		
		class dump : public ostream::ostr_func{
			public:
			void operator()(ostream& os){os.print_dump(_address,_count);};
			dump(void* address, size_t count): _address(address), _count(count) {};
			void* _address;
			size_t _count;
		};
		
		ostream& right 			(ostream& os);
		ostream& left 			(ostream& os);
		ostream& endl 			(ostream& os);
		ostream& hex 			(ostream& os);
		ostream& oct 			(ostream& os);
		ostream& dec 			(ostream& os);
		ostream& bin			(ostream& os);
		ostream& usebase 		(ostream& os);
		ostream& showpos 		(ostream& os);
		ostream& showbase 		(ostream& os);
		ostream& noshowpos 		(ostream& os);
		ostream& noshowbase 	(ostream& os);
		ostream& noboolalpha 	(ostream& os);
		ostream& reset 			(ostream& os);	
		ostream& alldigits 		(ostream& os);
		ostream& noalldigits 	(ostream& os);	
		ostream& flush 	(ostream& os);		
		
		
		
		
		
		
		
		/////Реализация методов	
		template<typename T, typename Traits>
		size_t basic_ostream<T,Traits>::print_field(char* c, size_t len)
		{			
			size_t count=0;
			int len_fill = (this->_width) - len;
			if (len_fill <= 0) count += write(c, len);
			else 
			{
				if(this->_flags & ios_base::left)
				{
					count+= write(c, len);
					for(;count < (this->_width);) count+=write(this->_fill);
				}
				
				if(this->_flags & ios_base::right)
				{
					for(;count < (this->_width - len);) count+=write(this->_fill);
					count += write(c, len);
				}	
			};
			return count;
		};
		
		template<typename T, typename Traits>
		template<typename Int> 
		size_t basic_ostream<T,Traits>::_print_number(Int n){
			char c[ARRAY_SIZE];
			char* end_buf = c + ARRAY_SIZE;
			char* ptr = __write_integer_backward(end_buf, this->flags(), this->base(), n);
			return print_field(ptr, end_buf - ptr);
		};
		
		template<typename T, typename Traits>
		template<typename T_float>
		size_t basic_ostream<T,Traits>::_print_float(T_float n){
			char c[ARRAY_SIZE];
			char* end_buf = c + ARRAY_SIZE;
			if (isnan(n)) return print_field("nan",3);
			if (isinf(n)) return print_field("inf",3);
			char* ptr = __write_float_backward(end_buf, this->flags(), this->precision(), 10, n);
			return print_field(ptr, end_buf - ptr);
		};
		
		template<typename T, typename Traits>
		size_t basic_ostream<T,Traits>::print(bool b){
			if (this->_flags & ios_base::boolalpha)
			if (b!=0) return print_field("true",4); else return print_field("false",5);
			else
			return _print_number((int)b);
		};
		
		
		
		template<typename T, typename Traits>
		void basic_ostream<T, Traits>::print_dump(void* _address, size_t _count)
		{
			uint8_t* dump = (uint8_t*) _address; 
			uint32_t addr_tmp = (size_t) _address;
			uint32_t i = 0;
			uint32_t mark = 0;
			
			*this << hex << noshowbase << alldigits;
			for (i = 0; i < _count; i++)
			{
				
				if (mark == 0)
				{
					print(addr_tmp);
					write(": ",2);
				}
				
				//Костыль, из-за ошибок в форматировании нуля
				if (dump[i]==0) print("00"); else 
				print((uint8_t)dump[i]);
				
				if ( mark == 7 )
				{
					write('|');
					mark++;
				}
				else if ( mark == 15 )
				{
					write('\n');
					mark = 0;
					
					addr_tmp += 0x10;
				}
				else
				{
					write(' ');
					mark++;
				}
			} 
			write('\n');
			reset(*this);
		};
		
		
		
		
	};
#endif				