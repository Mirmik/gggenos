#ifndef GENOS_IOS_H
	#define GENOS_IOS_H
	
	#include <genos/iostream/writer.h>
	#include <genos/iostream/reader.h>
	#include <genos/iostream/char_traits.h>
	
	#undef HEX
	#undef OCT
	#undef DEC
	
	
	namespace genos {
		
		//constexpr uint8_t HEX = 16;
		//constexpr uint8_t OCT = 8; 
		//constexpr uint8_t DEC = 10; 
		
		// флаги форматирования
		
		
		
		//template <typename T, typename Traits = elem_traits<T>>
		class ios_base{
			public:
			
			typedef uint32_t fmtflags;
			
			static constexpr fmtflags 		nullflags	= (fmtflags) 0;
			static constexpr fmtflags		boolalpha   = (fmtflags) 1 << 0;
			static constexpr fmtflags      	dec         = (fmtflags) 1 << 1;
			static constexpr fmtflags     	fixed       = (fmtflags) 1 << 2;
			static constexpr fmtflags     	hex         = (fmtflags) 1 << 3;
			static constexpr fmtflags     	internal    = (fmtflags) 1 << 4;
			static constexpr fmtflags   	left        = (fmtflags) 1 << 5;
			static constexpr fmtflags       oct         = (fmtflags) 1 << 6;
			static constexpr fmtflags       right       = (fmtflags) 1 << 7;
			static constexpr fmtflags       scientific  = (fmtflags) 1 << 8;
			static constexpr fmtflags       showbase    = (fmtflags) 1 << 9;
			static constexpr fmtflags       showpoint   = (fmtflags) 1 << 10;
			static constexpr fmtflags       showpos     = (fmtflags) 1 << 11;
			static constexpr fmtflags       skipws      = (fmtflags) 1 << 12;
			static constexpr fmtflags       unitbuf     = (fmtflags) 1 << 13;
			static constexpr fmtflags       uppercase   = (fmtflags) 1 << 14;
			static constexpr fmtflags		usebase		= (fmtflags) 1 << 15;
			static constexpr fmtflags       alldigits   = (fmtflags) 1 << 16;
			static constexpr fmtflags       bin   		= (fmtflags) 1 << 17;
			// oncesetw    = 1 << 15,
			static constexpr fmtflags       adjustfield = left | right;
			static constexpr fmtflags       basefield   = dec | oct | hex | usebase | bin;
			//basefield   = dump | numb,
			//floatfield  = scientific | fixed
		
        // Состояние потока.
		enum iostate
		{
			goodbit = 0,
			badbit  = 1 << 0,
			eofbit  = 1 << 1,
			failbit = 1 << 2
			};
			// Направление поиска для файловых потоков.
			enum seekdir
			{
				beg = 0,
				cur = 1,
				end = 2
			};
			// Режим открытия файлов
			enum openmode
			{
				app    = 1 << 0,
				ate    = 1 << 1,
				binary = 1 << 2,
				in     = 1 << 3,
				out    = 1 << 4,
				trunc  = 1 << 5
			};
			
			
			fmtflags flags ( ) const ;
			fmtflags flags ( fmtflags fmtfl );
			void reset_flags ( void );
			void setf ( fmtflags f );
			void clrf ( fmtflags f );
			bool getf ( fmtflags f );
			void setf ( fmtflags fmtfl, fmtflags mask );
			
			
			ios_base(const ios_base&);
			ios_base& operator=(const ios_base&);
			public:
			ios_base();
			
			//typedef bitset<16> fmtflags;
			// Манипулирование флагами форматирования
			// Ширина поля.
			inline size_t width ( ) const {return _width;};
			inline size_t width ( size_t width ) {size_t tmp = _width; _width = width; return tmp;};
			// Точность.
			inline uint8_t precision ( ) const {return _prec;};
			inline uint8_t precision ( uint8_t prec ) {uint8_t tmp = _base; _prec = prec; return tmp;};
			//База системы счисления.
			inline uint8_t base ( ) const {return _base;};
			inline uint8_t base ( uint8_t base ) {uint8_t tmp = _base; _base = base; return tmp;};
			
			
			public:
			fmtflags _flags;
			size_t _width;
			size_t _prec;
			size_t _base;
			
		};
		
		
		//DECLARE_ENUM_OPERATIONS(ios_base::fmtflags);
		
		
		
		template <typename T, typename traits = char_traits<T>>
		class ios : public ios_base 
		{
			public:
			ios() : _fill(' '){};
			T _fill;
			
			T fill(T c){_fill=c;};
			
			
			
			
			
		};
		
	};
	
#endif