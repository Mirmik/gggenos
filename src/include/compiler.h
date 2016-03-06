/**
 * @file
 * @brief
 *
 * @author  Anton Kozlov
 * @date    11.07.2015
 */

#ifndef COMPILER_H_
#define COMPILER_H_

#ifdef __GNUC__

#define _NORETURN __attribute__ ((noreturn))
#define _PRINTF_FORMAT(format_i, arg_start_i) \
	__attribute__ ((format (printf, format_i, arg_start_i)))

#else /* __GNUC__ */

#define _NORETURN
#define _PRINTF_FORMAT(format_i, arg_start_i)

#endif /* __GNUC__ */

#if (!defined(__STDC__) || !defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L) && \
	(!defined(__GNUC_GNU_INLINE__) || !__GNUC_GNU_INLINE__) && \
	(!defined(__GNUC_STDC_INLINE__) || !__GNUC_STDC_INLINE__)
#define inline /* to nothind */
#endif

#define likely(x)	__builtin_expect(!!(x), 1)
#define unlikely(x)	__builtin_expect(!!(x), 0)




#ifdef __cplusplus
 #define __BEGIN_NAMESPACE_STD namespace std {
 #define __END_NAMESPACE_STD }
#else
#define __BEGIN_NAMESPACE_STD 
#define __END_NAMESPACE_STD 
#endif

#undef EXTERN_C
#ifdef __cplusplus
# define EXTERN_C extern "C"
#else
# define EXTERN_C extern
#endif

#ifdef __cplusplus
#define __if_cplusplus(...) __VA_ARGS__
#define __if_not_cplusplus(...)
#define __if_C_if_CXX(c, cxx) cxx
#else
#define __if_cplusplus(...)
#define __if_not_cplusplus(...) __VA_ARGS__
#define __if_C_if_CXX(c, cxx) c 
#endif



#endif /* COMPILER_H_ */
