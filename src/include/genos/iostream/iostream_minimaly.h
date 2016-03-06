#ifndef GENOS_MINIMUM_IOSTREAM
	#define GENOS_MINIMUM_IOSTREAM
	
	#include <genos/debug/debug_print.h>
	#include <gstd/string.h>
	
	
	namespace genos {	
		
		class ostream_minimaly 	{
			
			static constexpr uint8_t UnsignedLongLex = 40; 
			
			Writer<char>* o;
			
			public:
			ostream_minimaly() : o(0) {};
			ostream_minimaly(Writer<char>& _o) : o(&_o) {};
			
			public:
			
			size_t write(char c){return o->write(c);};
			size_t write(char* c, size_t n){return o->write(c, n);};
			
			size_t print(char c){return write(c);};
			size_t print(char* c){return write(c ,gstd::strlen(c));};
			
			
			size_t print(short n){};
			size_t print(unsigned short n){};
			size_t print(int n){};
			size_t print(unsigned int n){};
			size_t print(long n){};
			size_t print(unsigned long n){};
			size_t print(long long n){};
			size_t print(unsigned long long n){};
			
			size_t print(bool b){if (b) return print ("true"); else return print("false");};
				
			
				
			
			ostream_minimaly& operator<<(char c){o->write(c); return *this;};
			ostream_minimaly& operator<<(char* c){print((char*)c);return *this;};
			//basic_ostream& operator<<(const T c){print(c);return *this;};
			//basic_ostream& operator<<(const T*& c) const{print(c);return *this;};
			ostream_minimaly& operator<<(unsigned long long i){print(i);return *this;};
			ostream_minimaly& operator<<(unsigned long i){print(i);return *this;};
			ostream_minimaly& operator<<(unsigned int i){print(i);return *this;};
			ostream_minimaly& operator<<(unsigned short i){print(i);return *this;};
			ostream_minimaly& operator<<(uint8_t i){print(i);return *this;};
			ostream_minimaly& operator<<(long long i){print(i);return *this;};
			ostream_minimaly& operator<<(long i){print(i);return *this;};
			ostream_minimaly& operator<<(int i){print(i);return *this;};
			ostream_minimaly& operator<<(short i){print(i);return *this;};
			//ostream_minimaly& operator<<(ostream_minimaly& (*pf)(ostream_minimaly&)){return pf(*this);};
			ostream_minimaly& operator<<(bool b){print(b); return *this;};
			
		};
	
		ostream_minimaly& endl (ostream_minimaly& os);
		
		
	};
#endif				