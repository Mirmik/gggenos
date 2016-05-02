#ifndef MITSU_PROTOCOL_H
#define MITSU_PROTOCOL_H

#include "inttypes.h"


void hilohilo_byte_to_symbol(uint32_t num, char* symb);

void hilo_byte_to_symbol(uint16_t num, char* symb);

void lo_byte_to_symbol(uint8_t num, char* symb);

void checksum_declare(char* message_buf, char term);

void confmes(char* mes, const char* com, const char* datano);

void confmes8(char* mes, const char* com, const char* datano, const char* data);

void confmes4(char* mes, const char* com, const char* datano, const char* data);

uint32_t uint32_protocol_parse(char* ans);

#endif