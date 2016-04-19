#ifndef GENOS_GSTD_H
#define GENOS_GSTD_H

#include "sys/cdefs.h"

__BEGIN_DECLS

 char *g_itoa( int num, char *buf, unsigned short int base );
 char *g_utoa( unsigned int num, char *buf, unsigned short int base );
 char *g_ltoa( long int num, char *buf, unsigned short int base );
 char *g_ultoa( unsigned long int num, char *buf, unsigned short int base );

__END_DECLS

#endif