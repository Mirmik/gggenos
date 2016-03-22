

#ifndef UTIL_ASSERT_H_
#define UTIL_ASSERT_H_

#include "util/location.h"
#include "sys/cdefs.h"
#include "compiler.h"

#ifndef NDEBUG

#define assert(condition) \
	  ({__g_assert(condition, #condition, "");})

#define assertf(condition, ...) \
	  ({__g_assert(condition, #condition, __VA_ARGS__);})



struct __assertion_point {
	struct location_func location;
	const char *expression;
};



#define __g_assert(condition, expr_str, ...) \
	do { \
		if (!(likely(condition))) {                                     \
			struct __assertion_point __assertion_point = { \
				__if_not_cplusplus(.location   =) LOCATION_FUNC_INIT,   \
				__if_not_cplusplus(.expression =) expr_str,             \
			};                                                          \
			__assert_handler(__assertion_point);             \
		}                                                               \
	} while(0)


EXTERN_C void _NORETURN __assert_handler(struct __assertion_point);

#else //NDEBUG
#define assert(condition) do { } while ((typeof(condition)) 0)
#define assertf(condition, ...) do { } while ((typeof(condition)) 0)
#endif	

#endif /* UTIL_ASSERT_H_ */
