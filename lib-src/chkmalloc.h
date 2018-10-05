/* chkmalloc.h
 * Include this file after, or in place of, <stdlib.h> or anything else
 * that might contain a declaration of malloc() and family.
 */

#ifndef CHKMALLOC_H
#define CHKMALLOC_H 1

#ifndef GCC_VERSION
# define GCC_VERSION (__GNUC__ * 1000 + __GNUC_MINOR__)
#endif /* GCC_VERSION */

#if (GCC_VERSION < 2007)
# define __attribute__(x)
#endif /* gcc pre-2.7 */

/* Attribute __malloc__ on functions was valid as of gcc 2.96. */
#ifndef ATTRIBUTE_MALLOC
# if (GCC_VERSION >= 2096)
#  define ATTRIBUTE_MALLOC __attribute__((__malloc__))
# else
#  define ATTRIBUTE_MALLOC
# endif /* GNUC >= 2.96 */
#endif /* ATTRIBUTE_MALLOC */

#if defined(__STDC__) || defined(PROTOTYPES) || defined(__PROTOTYPES)
# ifdef HAVE_STDLIB_H
#  include <stdlib.h>
# else
/* Put this instead of <stdlib.h> if your system does not have one: */
#  ifndef _STDLIB_H_
#   define _STDLIB_H_ 1
#  endif /* !_STDLIB_H_ */
extern void *calloc(int, int);
extern void *malloc(int) ATTRIBUTE_MALLOC;
extern void *realloc(void *, int);
extern void free(void *ptr);
# endif /* HAVE_STDLIB_H */
extern void *trace_malloc(char const *, int, size_t);
extern void *trace_calloc(char const *, int, size_t, size_t);
extern void *trace_realloc(char const *, int, void *, size_t);
extern void trace_free(char const *, int, void *);
#else
extern char *trace_malloc();
extern char *trace_calloc();
extern char *trace_realloc();
extern void trace_free();
#endif /* __STDC__ || PROTOTYPES || __PROTOTYPES */

#ifdef REDEFINE_MALLOC
# ifdef _MALLOC_INTERNAL	/* for use with gnu malloc and -DHACKMALLOC */
#  define malloc maLLoc
#  define calloc caLLoc
#  define realloc reaLLoc
#  define free frEE
# else
#  define malloc(n)     trace_malloc(__FILE__, __LINE__, n)
#  define calloc(m, n)  trace_calloc(__FILE__, __LINE__, m, n)
#  define realloc(p, n) trace_realloc(__FILE__, __LINE__, p, n)
#  define free(p)       trace_free(__FILE__, __LINE__, p)
# endif /* _MALLOC_INTERNAL */
#endif /* REDEFINE_MALLOC */

#endif /* CHKMALLOC_H */

/* EOF */
