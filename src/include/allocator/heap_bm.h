/**
 * @file
 * @brief Boundary markers memory allocation algorithm
 *
 * @date 04.02.14
 * @author Alexander Kalmuk
 */

#ifndef MEM_HEAP_BM_H_
#define MEM_HEAP_BM_H_

#include <sys/types.h>
#include "sys/cdefs.h"

__BEGIN_DECLS
extern void bm_init(void *segment, size_t size);
extern void *bm_memalign(void *segment, size_t boundary, size_t size);
extern void bm_free(void *segment, void *ptr);
__END_DECLS

#endif /* MEM_HEAP_BM_H_ */
