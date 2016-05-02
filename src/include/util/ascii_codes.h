#ifndef UTIL_ASCII_CODES
#define UTIL_ASCII_CODES

#include "utilxx/charbuf.h"
#include "utilxx/byte_access.h"

static inline uint8_t hexascii_to_halfbyte(char c) 
{
	return (uint8_t)(c <= '9' ? c - '0' : c - 'A' + 10);
};

static inline uint8_t hexascii2_to_byte(char hi, char lo) 
{
	uint8_accessor ret;
	ret.h = hexascii_to_halfbyte(hi);
	ret.l = hexascii_to_halfbyte(lo);
	return ret.num;
};

static inline char halfbyte_to_hexascii(uint8_t n) 
{
	return n < 10 ? '0' + n : 'A' - 10 + n;
};

#define get_halfbyte_number(u,n) (u>>(4*(n)) & 0xF)

static inline charbuf<2> uint8_to_hexcode(uint8_t src)
{
	uint8_accessor acs; acs.num = src;
	charbuf<2> buf;
	buf[1] = halfbyte_to_hexascii(acs.l); 
	buf[0] = halfbyte_to_hexascii(acs.h); 
	return buf;
};

static inline charbuf<4> uint16_to_hexcode(uint16_t src)
{
	uint16_accessor acs; acs.num = src;
	charbuf<4> buf;
	buf[3] = halfbyte_to_hexascii(acs.lo_l); 
	buf[2] = halfbyte_to_hexascii(acs.lo_h); 
	buf[1] = halfbyte_to_hexascii(acs.hi_l); 
	buf[0] = halfbyte_to_hexascii(acs.hi_h); 
	return buf;
};

static inline charbuf<8> uint32_to_hexcode(uint32_t src)
{
	uint32_accessor acs; acs.num = src;
	charbuf<8> buf;
	buf[7] = halfbyte_to_hexascii(acs.llo_l); 
	buf[6] = halfbyte_to_hexascii(acs.llo_h); 
	buf[5] = halfbyte_to_hexascii(acs.lhi_l); 
	buf[4] = halfbyte_to_hexascii(acs.lhi_h); 
	buf[3] = halfbyte_to_hexascii(acs.hlo_l); 
	buf[2] = halfbyte_to_hexascii(acs.hlo_h); 
	buf[1] = halfbyte_to_hexascii(acs.hhi_l); 
	buf[0] = halfbyte_to_hexascii(acs.hhi_h); 
	return buf;
};

static inline uint8_t hexcode_to_uint8(const char* str)
{
	uint8_t ret = hexascii2_to_byte(*str,*(str+1));
	return ret;
};

static inline uint16_t hexcode_to_uint16(const char* str)
{
	uint16_accessor s;
	s.hi = hexascii2_to_byte(*(str+0),*(str+1));
	s.lo = hexascii2_to_byte(*(str+2),*(str+3));
	return s.num;
};

static inline uint32_t hexcode_to_uint32(const char* str)
{
	uint32_accessor s;
	s.hhi = hexascii2_to_byte(*(str+0),*(str+1));
	s.hlo = hexascii2_to_byte(*(str+2),*(str+3));
	s.lhi = hexascii2_to_byte(*(str+4),*(str+5));
	s.llo = hexascii2_to_byte(*(str+6),*(str+7));
	return s.num;
};

static inline int32_t hexcode_to_int32(const char* str)
{
	return static_cast<int32_t>(hexcode_to_uint32(str));
};

#endif
