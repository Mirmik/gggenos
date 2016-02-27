/*
 * @file
 *
 * @date Jan 2, 2013
 * @author: Anton Bondarev
 */

#ifndef SYS_CDEFS_H_
#define SYS_CDEFS_H_

#ifdef __cplusplus
 #define __BEGIN_NAMESPACE_STD namespace std {
 #define __END_NAMESPACE_STD }
#else
#define __BEGIN_NAMESPACE_STD 
#define __END_NAMESPACE_STD 
#endif

/* __BEGIN_DECLS should be used at the beginning of your declarations,
   so that C++ compilers don't mangle their names.  Use __END_DECLS at
   the end of C declarations. */
#undef __BEGIN_DECLS
#undef __END_DECLS
#ifdef __cplusplus
# define __BEGIN_DECLS extern "C" {
# define __END_DECLS }
#else
# define __BEGIN_DECLS /* empty */
# define __END_DECLS /* empty */
#endif

#undef EXTERN_C
#ifdef __cplusplus
# define EXTERN_C extern "C"
#else
# define EXTERN_C extern
#endif

/*
#undef EMPTY_STRUCT_BODY
#ifdef __cplusplus
# define EMPTY_STRUCT_BODY int __placeholder[0];
#else
# define EMPTY_STRUCT_BODY
#endif
*/

#ifdef __cplusplus
#define __if_cplusplus(...) __VA_ARGS__
#define __if_not_cplusplus(...)
#define __if_C_if_CXX(c, cxx) cxx
#else
#define __if_cplusplus(...)
#define __if_not_cplusplus(...) __VA_ARGS__
#define __if_C_if_CXX(c, cxx) c 
#endif


#endif /* SYS_CDEFS_H_ */
