
#include "mitsuprotocol.h"
#include "stdlib.h"
#include "string.h"
#include "util/ascii_codes.h"
#include "util/assert.h"


void hilohilo_byte_to_symbol(uint32_t num, char* symb)
{
	uint8_t lll = num & 0x0000000F;
	uint8_t llh = (num & 0x000000F0) >> 4; 
	uint8_t lhl = (num & 0x00000F00) >> 8;
	uint8_t lhh = (num & 0x0000F000) >> 12; 
	uint8_t hll = (num & 0x000F0000) >> 16;
	uint8_t hlh = (num & 0x00F00000) >> 20; 
	uint8_t hhl = (num & 0x0F000000) >> 24;
	uint8_t hhh = (num & 0xF0000000) >> 28; 
	
	*(symb + 7) = byte_to_hexascii(lll);
	*(symb + 6) = byte_to_hexascii(llh);
	*(symb + 5) = byte_to_hexascii(lhl);
	*(symb + 4) = byte_to_hexascii(lhh);
	*(symb + 3) = byte_to_hexascii(hll);
	*(symb + 2) = byte_to_hexascii(hlh);
	*(symb + 1) = byte_to_hexascii(hhl);
	*(symb)     = byte_to_hexascii(hhh);
};


void hilo_byte_to_symbol(uint16_t num, char* symb)
{
	uint8_t ll = num & 0x000F;
	uint8_t lh = (num & 0x00F0) >> 4; 
	uint8_t hl = (num & 0x0F00) >> 8;
	uint8_t hh = (num & 0xF000) >> 12; 
	*(symb + 3) = byte_to_hexascii(ll);
	*(symb + 2) = byte_to_hexascii(lh);
	*(symb + 1) = byte_to_hexascii(hl);
	*(symb)     = byte_to_hexascii(hh);
};


void lo_byte_to_symbol(uint8_t num, char* symb)
{
	uint8_t l = num % 0x10;
	uint8_t h = num >> 4; 
	*(symb + 1) = byte_to_hexascii(l);
	*(symb)     = byte_to_hexascii(h);
};

void checksum_declare(char* message_buf, char term)
{
	char* ptr = message_buf;
	int sum = 0;
	assert(*ptr == 1);
	do {
		ptr++;
		sum += *ptr;
	} while(*ptr != 3);
	lo_byte_to_symbol(sum, ++ptr);
};



void confmes(char* mes, const char* com, const char* datano)
{
		mes[0] = 1;
		mes[1] = '2';
		mes[2] = com[0];
		mes[3] = com[1];
		mes[4] = 2;
		mes[5] = datano[0];
		mes[6] = datano[1];
		mes[7] = 3;
		mes[10] = 0;
		checksum_declare(mes, 3);
};


void confmes8(char* mes, const char* com, const char* datano, const char* data)
{
		mes[0] = 1;
		mes[1] = '2';
		mes[2] = com[0];
		mes[3] = com[1];
		mes[4] = 2;
		mes[5] = datano[0];
		mes[6] = datano[1];
		memcpy(&mes[7], data, 8);
		mes[15] = 3;
		mes[18] = 0;
		checksum_declare(mes, 3);
};

void confmes4(char* mes, const char* com, const char* datano, const char* data)
{
		mes[0] = 1;
		mes[1] = '2';
		mes[2] = com[0];
		mes[3] = com[1];
		mes[4] = 2;
		mes[5] = datano[0];
		mes[6] = datano[1];
		memcpy(&mes[7], data, 4);
		mes[11] = 3;
		mes[14] = 0;
		checksum_declare(mes, 3);
};

uint32_t uint32_protocol_parse(char* ans)
{
	uint32_t res;
	uint8_t* ptr = (uint8_t*) &res;

	char* ans0 = ans;
	char* ans1 = ans + 2;
	char* ans2 = ans + 4;
	char* ans3 = ans + 6;

	*ptr++ = (hexascii_to_byte(*ans3) << 4) + hexascii_to_byte(*(ans3 + 1));
	*ptr++ = (hexascii_to_byte(*ans2) << 4) + hexascii_to_byte(*(ans2 + 1));
	*ptr++ = (hexascii_to_byte(*ans1) << 4) + hexascii_to_byte(*(ans1 + 1));
	*ptr++ = (hexascii_to_byte(*ans0) << 4) + hexascii_to_byte(*(ans0 + 1));

	return res;
};