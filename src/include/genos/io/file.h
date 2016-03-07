#ifndef GENOS_FILE_IO
#define GENOS_FILE_IO

using putc_t  = int(*)(char);
using getc_t  = int(*)();
using write_t = int(*)(char*, int n);
using read_t  = int(*)(char*, int n);

using begin_t  = int(*)();
using close_t  = int(*)();

class file_ops 
{

};

#endif