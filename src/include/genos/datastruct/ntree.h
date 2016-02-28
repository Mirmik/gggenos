
#ifndef GENOS_NTREE
	#define GENOS_NTREE
	
	
	#include "genos/base_struct/list.h"
	#include "genos/base_struct/bits.h"
	
	static constexpr uint8_t  NODE_DIRECTORY = 0x80;
	
	struct ntree : public list_head
	{
		uint8_t trait;
	};
	
	
	struct ntreedir : public ntree
	{
		ntreedir* parent;
		list_head nodes;
		
		ntreedir() : parent(nullptr) {trait = NODE_DIRECTORY;};
	};
	
	#define ntree_entry(ptr, type, member) \
	container_of(ptr, type, member)
	
	static inline void ntree_add_node(ntree* _new, ntreedir* dir)
	{
		list_add(_new, &dir->nodes);
	};
	
	static inline void ntree_add_dir(ntreedir* _new, ntreedir* dir)
	{
			ntree_add_node(_new, dir);
			_new->parent = dir;
	};
	
	static inline bool ntree_is_dir(ntree* lst)
	{
		return bits_mask(lst->trait, NODE_DIRECTORY);
		};
#endif