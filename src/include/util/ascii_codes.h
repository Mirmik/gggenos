#ifndef UTIL_ASCII_CODES
#define UTIL_ASCII_CODES

#include "utilxx/charbuf.h"

static inline uint8_t hexascii_to_halfbyte(char c) 
{
	return (uint8_t)(c <= '9' ? c - '0' : c - 'A' + 10);
};

static inline uint8_t hexascii2_to_byte(char hi, char lo) 
{
	uint8_t uhi = hexascii_to_halfbyte(hi);
	uint8_t ulo = hexascii_to_halfbyte(lo);
	return (uhi << 4) + ulo;
};

static inline char halfbyte_to_hexascii(uint8_t n) 
{
	return n < 10 ? '0' + n : 'A' - 10 + n;
};

#define get_halfbyte_number(u,n) (u>>(4*(n)) & 0xF)

static inline charbuf<2> uint_to_hexcode2(uint8_t src)
{
	charbuf<2> buf;
	buf[1] = halfbyte_to_hexascii(src & 0xF); src >>= 4;
	buf[0] = halfbyte_to_hexascii(src & 0xF); //src >>= 4;
	return buf;
};

static inline charbuf<4> uint_to_hexcode4(uint16_t src)
{
	charbuf<4> buf;
	buf[3] = halfbyte_to_hexascii(src & 0xF); src >>= 4;
	buf[2] = halfbyte_to_hexascii(src & 0xF); src >>= 4;
	buf[1] = halfbyte_to_hexascii(src & 0xF); src >>= 4;
	buf[0] = halfbyte_to_hexascii(src & 0xF); //src >>= 4;
	return buf;
};

static inline charbuf<8> uint_to_hexcode8(uint32_t src)
{
	charbuf<8> buf;
	buf[7] = halfbyte_to_hexascii(src & 0xF); src >>= 4;
	buf[6] = halfbyte_to_hexascii(src & 0xF); src >>= 4;
	buf[5] = halfbyte_to_hexascii(src & 0xF); src >>= 4;
	buf[4] = halfbyte_to_hexascii(src & 0xF); src >>= 4;
	buf[3] = halfbyte_to_hexascii(src & 0xF); src >>= 4;
	buf[2] = halfbyte_to_hexascii(src & 0xF); src >>= 4;
	buf[1] = halfbyte_to_hexascii(src & 0xF); src >>= 4;
	buf[0] = halfbyte_to_hexascii(src & 0xF); //src >>= 4;
	return buf;
};

static inline uint8_t hexcode_to_uint8(const char* str)
{
	uint8_t ret = hexascii2_to_byte(*str,*(str+1));
	return ret;
};

static inline uint16_t hexcode_to_uint16(const char* str)
{
	union 
	{
		uint16_t ret;
		struct 
		{
			uint8_t lo;
			uint8_t hi;		
		}s;
	};
	s.hi = hexascii2_to_byte(*(str+0),*(str+1));
	s.lo = hexascii2_to_byte(*(str+2),*(str+3));
	return ret;
};

static inline uint32_t hexcode_to_uint32(const char* str)
{
	union 
	{
		uint32_t ret;
		struct 
		{
			uint8_t llo;
			uint8_t lhi;
			uint8_t hlo;
			uint8_t hhi;		
		}s;
	};
	s.hhi = hexascii2_to_byte(*(str+0),*(str+1));
	s.hlo = hexascii2_to_byte(*(str+2),*(str+3));
	s.lhi = hexascii2_to_byte(*(str+4),*(str+5));
	s.llo = hexascii2_to_byte(*(str+6),*(str+7));
	return ret;
};

static inline int32_t hexcode_to_int32(const char* str)
{
	return static_cast<int32_t>(hexcode_to_uint32(str));
};

#endif
