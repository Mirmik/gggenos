#ifndef UTIL_ASCII_CODES
#define UTIL_ASCII_CODES

#define hexascii_to_byte(c) (c <= '9' ? c - '0' : c - 'A' + 10)
#define byte_to_hexascii(n) (n < 10 ? '0' + n : 'A' - 10 + n)

#endif
