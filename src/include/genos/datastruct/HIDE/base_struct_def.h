#ifndef GENOS_BASE_STRUCT_DEF_H
	#define GENOS_BASE_STRUCT_DEF_H



# define offsetof(type, member) \
	((size_t) &((type *) 0x0)->member)



#define container_of(ptr, type, member)                   \
                ({(type *) ((char *)(ptr)-offsetof(type,member));})\
                



#endif
