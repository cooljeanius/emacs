/* lisp.h: Fundamental definitions for GNU Emacs Lisp interpreter.

Copyright (C) 1985-1987, 1993-1995, 1997-2014 Free Software Foundation,
Inc.

This file is part of GNU Emacs.

GNU Emacs is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

GNU Emacs is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.  */

#ifndef EMACS_LISP_H
#define EMACS_LISP_H

#include <setjmp.h>
#if (defined(HAVE_STDALIGN_H) && HAVE_STDALIGN_H) || \
    (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L))
# include <stdalign.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__) && defined(lint)
#  warning "lisp.h expects <stdalign.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ && lint */
#endif /* HAVE_STDALIGN_H || c11 or newer */
#include <stdarg.h>
#include <stddef.h>
#include <float.h>
#include <inttypes.h>
#include <limits.h>

#include <intprops.h>
/* hack to silence '-Wundef': */
#ifndef _MSC_VER
# ifndef FAKED__MSC_VER
#  define FAKED__MSC_VER 1
# endif /* !FAKED__MSC_VER */
# define _MSC_VER 0
#endif /* !_MSC_VER */
#include <verify.h>
/* pop it off again: */
#if defined(FAKED__MSC_VER) && defined(_MSC_VER)
# undef _MSC_VER
#endif /* FAKED__MSC_VER && _MSC_VER */

INLINE_HEADER_BEGIN

/* this file contains way too many of these for now: */
#if defined(__GNUC__) && defined(__GNUC_MINOR__) && \
    ((__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6)))
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wredundant-decls"
#endif /* gcc 4.6+ */

/* Declare the prototype for a general external function: */
#ifndef P_
# if defined(PROTOTYPES) || defined(__PROTOTYPES) || defined(WINDOWSNT)
#  define P_(proto) proto
# else
#  define P_(proto) ()
# endif /* PROTOTYPES || __PROTOTYPES || WINDOWSNT */
#endif /* !P_ */

#ifdef WE_NEED_TO_TEMPORARILY_HUNT_A_BUG
/* Define this temporarily to hunt a bug.  If defined, the size of
 * strings is redundantly recorded in sdata structures so that it can
 * be compared to the sizes recorded in Lisp strings.  */
# ifndef GC_CHECK_STRING_BYTES
#  define GC_CHECK_STRING_BYTES 1
# endif /* !GC_CHECK_STRING_BYTES */

/* Define this to check for short string overrun: */
# ifndef GC_CHECK_STRING_OVERRUN
#  define GC_CHECK_STRING_OVERRUN 1
# endif /* !GC_CHECK_STRING_OVERRUN */

/* Define this to check the string free list: */
# ifndef GC_CHECK_STRING_FREE_LIST
#  define GC_CHECK_STRING_FREE_LIST 1
# endif /* !GC_CHECK_STRING_FREE_LIST */

/* Define this to check for malloc buffer overrun: */
# ifndef XMALLOC_OVERRUN_CHECK
#  define XMALLOC_OVERRUN_CHECK 1
# endif /* !XMALLOC_OVERRUN_CHECK */

/* Define this to check for errors in cons list: */
# ifndef GC_CHECK_CONS_LIST
#  define GC_CHECK_CONS_LIST 1
# endif /* !GC_CHECK_CONS_LIST */
#endif /* WE_NEED_TO_TEMPORARILY_HUNT_A_BUG */

#ifdef GC_CHECK_CONS_LIST
/* from alloc.c: */
# define CHECK_CONS_LIST() check_cons_list()
#else
# define CHECK_CONS_LIST() 0
#endif /* GC_CHECK_CONS_LIST */


/* Define a TYPE constant ID as an externally visible name.  Use like this:

      DEFINE_GDB_SYMBOL_BEGIN (TYPE, ID)
      #define ID something
      DEFINE_GDB_SYMBOL_END (ID)

   This hack is for the benefit of compilers that do not make macro
   definitions visible to the debugger.  It is used for symbols that
   .gdbinit needs, symbols whose values may not fit in 'int' (where an
   enum would suffice).  */
#if defined MAIN_PROGRAM
# define DEFINE_GDB_SYMBOL_BEGIN(type, id) type const id EXTERNALLY_VISIBLE
# define DEFINE_GDB_SYMBOL_END(id) = id;
#else
# define DEFINE_GDB_SYMBOL_BEGIN(type, id)
# define DEFINE_GDB_SYMBOL_END(val)
#endif /* MAIN_PROGRAM */

/* The ubiquitous max and min macros.  */
#undef min
#undef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

#ifndef SIZE_T_MAX
# ifdef SIZE_MAX
#  define SIZE_T_MAX SIZE_MAX
# else
#  ifdef ULONG_MAX
#   define SIZE_T_MAX ULONG_MAX	/* max value for a size_t */
#  else
#   ifdef UINT_MAX
#    define SIZE_T_MAX UINT_MAX
#   endif /* UINT_MAX */
#  endif /* ULONG_MAX */
# endif /* SIZE_MAX */
#endif /* !SIZE_T_MAX */

#ifndef BUF_LEN_MAX_FOR_SNPRINTF
# define BUF_LEN_MAX_FOR_SNPRINTF \
           (min((((SIZE_T_MAX - 1UL) / 2UL) - 1UL), ((size_t)INT_MAX - 1UL)))
#endif /* BUF_LEN_MAX_FOR_SNPRINTF */

/* EMACS_INT - signed integer wide enough to hold an Emacs value
   EMACS_INT_MAX - maximum value of EMACS_INT; can be used in #if
   pI - printf length modifier for EMACS_INT
   EMACS_UINT - unsigned variant of EMACS_INT */
#ifndef EMACS_INT_MAX
# if INTPTR_MAX <= 0
#  error "INTPTR_MAX misconfigured"
# elif INTPTR_MAX <= INT_MAX && !defined WIDE_EMACS_INT
typedef int EMACS_INT;
typedef unsigned int EMACS_UINT;
#  define EMACS_INT_MAX INT_MAX
#  define pI ""
# elif INTPTR_MAX <= LONG_MAX && !defined WIDE_EMACS_INT
typedef long int EMACS_INT;
typedef unsigned long EMACS_UINT;
#  define EMACS_INT_MAX LONG_MAX
#  define pI "l"
# elif INTPTR_MAX <= LLONG_MAX
typedef long long int EMACS_INT;
typedef unsigned long long int EMACS_UINT;
#  define EMACS_INT_MAX LLONG_MAX
#  define pI "ll"
# else
#  error "INTPTR_MAX too large"
# endif
#endif /* !EMACS_INT_MAX */

/* Number of bits to put in each character in the internal representation
   of bool vectors.  This should not vary across implementations.  */
enum {  BOOL_VECTOR_BITS_PER_CHAR =
#define BOOL_VECTOR_BITS_PER_CHAR 8
        BOOL_VECTOR_BITS_PER_CHAR
};

/* An unsigned integer type representing a fixed-length bit sequence,
   suitable for words in a Lisp bool vector.  Normally it is size_t
   for speed, but it is unsigned char on weird platforms.  */
#if BOOL_VECTOR_BITS_PER_CHAR == CHAR_BIT
typedef size_t bits_word;
# define BITS_WORD_MAX SIZE_MAX
enum { BITS_PER_BITS_WORD = CHAR_BIT * sizeof (bits_word) };
#else
typedef unsigned char bits_word;
# define BITS_WORD_MAX ((1u << BOOL_VECTOR_BITS_PER_CHAR) - 1)
enum { BITS_PER_BITS_WORD = BOOL_VECTOR_BITS_PER_CHAR };
#endif
verify (BITS_WORD_MAX >> (BITS_PER_BITS_WORD - 1) == 1);

/* Number of bits in some machine integer types.  */
enum
  {
    BITS_PER_CHAR      = CHAR_BIT,
    BITS_PER_SHORT     = CHAR_BIT * sizeof (short),
    BITS_PER_INT       = CHAR_BIT * sizeof (int),
    BITS_PER_LONG      = CHAR_BIT * sizeof (long int),
    BITS_PER_EMACS_INT = CHAR_BIT * sizeof (EMACS_INT)
  };

/* printmax_t and uprintmax_t are types for printing large integers.
   These are the widest integers that are supported for printing.
   pMd etc. are conversions for printing them.
   On C99 hosts, there is no problem, as even the widest integers work.
   Fall back on EMACS_INT on pre-C99 hosts.  */
#ifdef PRIdMAX
typedef intmax_t printmax_t;
typedef uintmax_t uprintmax_t;
# define pMd PRIdMAX
# define pMu PRIuMAX
#else
typedef EMACS_INT printmax_t;
typedef EMACS_UINT uprintmax_t;
# define pMd pI"d"
# define pMu pI"u"
#endif /* PRIdMAX */

/* Use pD to format ptrdiff_t values, which suffice for indexes into
   buffers and strings.  Emacs never allocates objects larger than
   PTRDIFF_MAX bytes, as they cause problems with pointer subtraction.
   In C99, pD can always be "t"; configure it here for the sake of
   pre-C99 libraries such as glibc 2.0 and Solaris 8.  */
/* Since sometimes these macros can be used to print unsigned values,
 * add corresponding typedefs, since there is no standard uptrdiff_t,
 * and size_t is not always equivalent.  */
#if PTRDIFF_MAX == INT_MAX
# define pD ""
typedef unsigned int uptrdiff_t;
#elif PTRDIFF_MAX == LONG_MAX
# define pD "l"
typedef unsigned long uptrdiff_t;
#elif PTRDIFF_MAX == LLONG_MAX
# define pD "ll"
typedef unsigned long long uptrdiff_t;
#else
# define pD "t"
typedef size_t uptrdiff_t;
#endif

/* Extra internal type checking?  */

/* Define Emacs versions of <assert.h>'s 'assert (COND)' and <verify.h>'s
   'assume (COND)'.  COND should be free of side effects, as it may or
   may not be evaluated.

   'eassert (COND)' checks COND at runtime if ENABLE_CHECKING is
   defined and suppress_checking is false, and does nothing otherwise.
   Emacs dies if COND is checked and is false.  The suppress_checking
   variable is initialized to 0 in alloc.c.  Set it to 1 using a
   debugger to temporarily disable aborting on detected internal
   inconsistencies or error conditions.

   In some cases, a good compiler may be able to optimize away the
   eassert macro even if ENABLE_CHECKING is true, e.g., if XSTRING (x)
   uses eassert to test STRINGP (x), but a particular use of XSTRING
   is invoked only after testing that STRINGP (x) is true, making the
   test redundant.

   eassume is like eassert except that it also causes the compiler to
   assume that COND is true afterwards, regardless of whether runtime
   checking is enabled.  This can improve performance in some cases,
   though it can degrade performance in others.  It's often suboptimal
   for COND to call external functions or access volatile storage.  */

#ifndef ENABLE_CHECKING
# define eassert(cond) ((void) (false && (cond))) /* Check COND compiles.  */
# define eassume(cond) assume (cond)
# ifndef CHECK
/* Produce same side effects and result, but do NOT complain: */
#  define CHECK(check,msg) ((check),0)
# endif /* !CHECK */
#else /* do ENABLE_CHECKING: */

extern _Noreturn void die (const char *, const char *, int);

extern bool suppress_checking EXTERNALLY_VISIBLE;

# ifndef CHECK
#  define CHECK(check,msg) (((check) || suppress_checking		\
			     ? (void) 0                                 \
			     : die ((msg), __FILE__, __LINE__)),	\
			    0)
# endif /* !CHECK */

# define eassert(cond)						\
   (suppress_checking || (cond) 				\
    ? (void) 0							\
    : die (# cond, __FILE__, __LINE__))
# define eassume(cond)						\
   (suppress_checking						\
    ? assume (cond)						\
    : (cond)							\
    ? (void) 0							\
    : die (# cond, __FILE__, __LINE__))
#endif /* !ENABLE_CHECKING || do enable it */

/* Used for making sure that Emacs is compilable in all
 * configurations: */
#if defined(USE_LISP_UNION_TYPE) && defined(NO_UNION_TYPE)
# undef NO_UNION_TYPE
#endif /* USE_LISP_UNION_TYPE && NO_UNION_TYPE */

/* Use the configure flag --enable-check-lisp-object-type to make
   Lisp_Object use a struct type instead of the default int.  The flag
   causes CHECK_LISP_OBJECT_TYPE to be defined.  */

/***** Select the tagging scheme.  *****/
/* The following option controls the tagging scheme:
   - USE_LSB_TAG means that we can assume the least 3 bits of pointers are
     always 0, and we can thus use them to hold tag bits, without
     restricting our addressing space.

   If ! USE_LSB_TAG, then use the top 3 bits for tagging, thus
   restricting our possible address range.

   USE_LSB_TAG not only requires the least 3 bits of pointers returned by
   malloc to be 0 but also needs to be able to impose a mult-of-8 alignment
   on the few static Lisp_Objects used: all the defsubr as well
   as the two special buffers buffer_defaults and buffer_local_symbols.  */

/* First, try and define DECL_ALIGN(type,var) which declares a static
 * variable VAR of type TYPE with the added requirement that it be
 * TYPEBITS-aligned. */
#ifndef NO_DECL_ALIGN
# ifndef DECL_ALIGN
/* What compiler directive should we use for non-gcc compilers?  -stef  */
#  if defined(__GNUC__)
#   define DECL_ALIGN(type, var) \
     type __attribute__ ((__aligned__ (1 << GCTYPEBITS))) var
#  endif /* __GNUC__ */
# endif /* !DECL_ALIGN */
#endif /* !NO_DECL_ALIGN */

enum Lisp_Bits
  {
    /* Number of bits in a Lisp_Object tag.  This can be used in #if,
       and for GDB's sake also as a regular symbol.  */
    GCTYPEBITS =
#define GCTYPEBITS 3
	GCTYPEBITS,

    /* 2**GCTYPEBITS.  This must be a macro that expands to a literal
       integer constant, for MSVC.  */
#define GCALIGNMENT 8

    /* Number of bits in a Lisp_Object value, not counting the tag.  */
    VALBITS = BITS_PER_EMACS_INT - GCTYPEBITS,

    /* Number of bits in a Lisp fixnum tag.  */
    INTTYPEBITS = GCTYPEBITS - 1,

    /* Number of bits in a Lisp fixnum value, not counting the tag.  */
    FIXNUM_BITS = VALBITS + 1
  };

#if GCALIGNMENT != 1 << GCTYPEBITS
# error "GCALIGNMENT and GCTYPEBITS are inconsistent"
#endif

/* The maximum value that can be stored in a EMACS_INT, assuming all
   bits other than the type bits contribute to a nonnegative signed value.
   This can be used in #if, e.g., '#if VAL_MAX < UINTPTR_MAX' below.  */
#define VAL_MAX (EMACS_INT_MAX >> (GCTYPEBITS - 1))

/* Unless otherwise specified, use USE_LSB_TAG on systems where:  */
#ifndef USE_LSB_TAG
/* 1.  We know malloc returns a multiple of 8.  */
# if (defined GNU_MALLOC || defined DOUG_LEA_MALLOC || defined __GLIBC__ \
      || defined DARWIN_OS || defined __sun)
/* 2.  We can specify multiple-of-8 alignment on static variables.  */
#  ifdef alignas
/* 3.  Pointers-as-ints exceed VAL_MAX.
   On hosts where pointers-as-ints do not exceed VAL_MAX, USE_LSB_TAG is:
    a. unnecessary, because the top bits of an EMACS_INT are unused, and
    b. slower, because it typically requires extra masking.
   So, default USE_LSB_TAG to true only on hosts where it might be useful.  */
#   if VAL_MAX < UINTPTR_MAX
#    define USE_LSB_TAG true
#   endif /* VAL_MAX < UINTPTR_MAX */
#  endif /* alignas */
# endif /* malloc returns a multiple of 8 */
#endif /* !USE_LSB_TAG */
#ifdef USE_LSB_TAG
# undef USE_LSB_TAG
enum enum_USE_LSB_TAG { USE_LSB_TAG = true };
# if defined(_STDBOOL_H_) && defined(__bool_true_false_are_defined)
/* "true" does not work as a valid preprocessor expression when it is a
 * macro whose expansion contains a cast: */
#  define USE_LSB_TAG 1
# else
#  define USE_LSB_TAG true
# endif /* _STDBOOL_H_ && __bool_true_false_are_defined */
#else
enum enum_USE_LSB_TAG { USE_LSB_TAG = false };
# define USE_LSB_TAG false
#endif /* USE_LSB_TAG */

#ifndef alignas
# define alignas(alignment) /* empty */
# if USE_LSB_TAG
#  error "USE_LSB_TAG requires alignas"
# endif /* USE_LSB_TAG */
#endif /* !alignas */

/* If we cannot use 8-byte alignment, make DECL_ALIGN a no-op: */
#ifndef DECL_ALIGN
# if defined(USE_LSB_TAG) && !defined(alignas)
#  error "USE_LSB_TAG used without defining DECL_ALIGN or alignas"
# endif /* USE_LSB_TAG && !alignas */
# define DECL_ALIGN(type, var) type var
#endif /* !DECL_ALIGN */


/* Some operations are so commonly executed that they are implemented
   as macros, not functions, because otherwise runtime performance would
   suffer too much when compiling with GCC without optimization.
   There's no need to inline everything, just the operations that
   would otherwise cause a serious performance problem.

   For each such operation OP, define a macro lisp_h_OP that contains
   the operation's implementation.  That way, OP can be implemented
   via a macro definition like this:

     #define OP(x) lisp_h_OP (x)

   and/or via a function definition like this:

     LISP_MACRO_DEFUN (OP, Lisp_Object, (Lisp_Object x), (x))

   which macro-expands to this:

     Lisp_Object (OP) (Lisp_Object x) { return lisp_h_OP (x); }

   without worrying about the implementations diverging, since
   lisp_h_OP defines the actual implementation.  The lisp_h_OP macros
   are intended to be private to this include file, and should not be
   used elsewhere.

   FIXME: Remove the lisp_h_OP macros, and define just the inline OP
   functions, once most developers have access to GCC 4.8 or later and
   can use "gcc -Og" to debug.  Maybe in the year 2016.  See
   Bug#11935.

   Commentary for these macros can be found near their corresponding
   functions, below.  */

#if defined(CHECK_LISP_OBJECT_TYPE) && CHECK_LISP_OBJECT_TYPE
# define lisp_h_XLI(o) ((o).i)
# define lisp_h_XIL(i) ((Lisp_Object){ i })
#else
# define lisp_h_XLI(o) (o)
# define lisp_h_XIL(i) (i)
#endif /* CHECK_LISP_OBJECT_TYPE */
#define lisp_h_CHECK_LIST_CONS(x, y) CHECK_TYPE(CONSP(x), Qlistp, y)
#define lisp_h_CHECK_NUMBER(x) CHECK_TYPE(INTEGERP(x), Qintegerp, x)
#define lisp_h_CHECK_SYMBOL(x) CHECK_TYPE(SYMBOLP(x), Qsymbolp, x)
#define lisp_h_CHECK_TYPE(ok, Qxxxp, x) \
   ((ok) ? (void)0 : (void)wrong_type_argument(Qxxxp, x))
#define lisp_h_CONSP(x) (XTYPE(x) == Lisp_Cons)
#define lisp_h_EQ(x, y) (XLI(x) == XLI(y))
#define lisp_h_FLOATP(x) (XTYPE(x) == Lisp_Float)
#define lisp_h_INTEGERP(x) ((XTYPE(x) & (enum Lisp_Type)(~Lisp_Int1)) == 0)
#define lisp_h_MARKERP(x) (MISCP(x) && XMISCTYPE(x) == Lisp_Misc_Marker)
#define lisp_h_MISCP(x) (XTYPE(x) == Lisp_Misc)
#define lisp_h_NILP(x) EQ(x, Qnil)
#define lisp_h_SET_SYMBOL_VAL(sym, v) \
   (eassert((sym)->redirect == SYMBOL_PLAINVAL), (sym)->val.value = (v))
#define lisp_h_SYMBOL_CONSTANT_P(sym) (XSYMBOL(sym)->constant)
#define lisp_h_SYMBOL_VAL(sym) \
   (eassert((sym)->redirect == SYMBOL_PLAINVAL), (sym)->val.value)
#define lisp_h_SYMBOLP(x) (XTYPE(x) == Lisp_Symbol)
#define lisp_h_VECTORLIKEP(x) (XTYPE(x) == Lisp_Vectorlike)
#define lisp_h_XCAR(c) (eassert(XCONS(c) != NULL), XCONS(c)->car)
#define lisp_h_XCDR(c) (eassert(XCONS(c) != NULL), XCONS(c)->u.cdr)
#define lisp_h_XCONS(a) \
   (eassert(CONSP(a)), (struct Lisp_Cons *)XUNTAG(a, Lisp_Cons))
#define lisp_h_XHASH(a) XUINT(a)
#define lisp_h_XPNTR(a) \
   ((void *)(intptr_t)((XLI(a) & VALMASK) | DATA_SEG_BITS))
#define lisp_h_XSYMBOL(a) \
   (eassert(SYMBOLP(a)), (struct Lisp_Symbol *)XUNTAG(a, Lisp_Symbol))
#ifndef GC_CHECK_CONS_LIST
# define lisp_h_check_cons_list() ((void) 0)
#endif /* !GC_CHECK_CONS_LIST */
#if USE_LSB_TAG
# define lisp_h_make_number(n) XIL((EMACS_INT)(n) << INTTYPEBITS)
# define lisp_h_XFASTINT(a) XINT(a)
# define lisp_h_XINT(a) (XLI(a) >> INTTYPEBITS)
# define lisp_h_XTYPE(a) ((enum Lisp_Type)(XLI(a) & ~VALMASK))
# define lisp_h_XUNTAG(a, type) ((void *)(XLI(a) - (type)))
#endif /* USE_LSB_TAG */

/* When compiling via gcc -O0, define the key operations as macros, as
   Emacs is too slow otherwise.  To disable this optimization, compile
   with -DINLINING=false.  */
#if (defined __NO_INLINE__ \
     && ! defined __OPTIMIZE__ && ! defined __OPTIMIZE_SIZE__ \
     && ! (defined INLINING && ! INLINING))
# define XLI(o) lisp_h_XLI(o)
# define XIL(i) lisp_h_XIL(i)
# define CHECK_LIST_CONS(x, y) lisp_h_CHECK_LIST_CONS(x, y)
# define CHECK_NUMBER(x) lisp_h_CHECK_NUMBER(x)
# define CHECK_SYMBOL(x) lisp_h_CHECK_SYMBOL(x)
# define CHECK_TYPE(ok, Qxxxp, x) lisp_h_CHECK_TYPE(ok, Qxxxp, x)
# define CONSP(x) lisp_h_CONSP(x)
# define EQ(x, y) lisp_h_EQ(x, y)
# define FLOATP(x) lisp_h_FLOATP(x)
# define INTEGERP(x) lisp_h_INTEGERP(x)
# define MARKERP(x) lisp_h_MARKERP(x)
# define MISCP(x) lisp_h_MISCP(x)
# define NILP(x) lisp_h_NILP(x)
# define SET_SYMBOL_VAL(sym, v) lisp_h_SET_SYMBOL_VAL(sym, v)
# define SYMBOL_CONSTANT_P(sym) lisp_h_SYMBOL_CONSTANT_P(sym)
# define SYMBOL_VAL(sym) lisp_h_SYMBOL_VAL(sym)
# define SYMBOLP(x) lisp_h_SYMBOLP(x)
# define VECTORLIKEP(x) lisp_h_VECTORLIKEP(x)
# define XCAR(c) lisp_h_XCAR(c)
# define XCDR(c) lisp_h_XCDR(c)
# define XCONS(a) lisp_h_XCONS(a)
# define XHASH(a) lisp_h_XHASH(a)
# define XPNTR(a) lisp_h_XPNTR(a)
# define XSYMBOL(a) lisp_h_XSYMBOL(a)
# ifndef GC_CHECK_CONS_LIST
#  define check_cons_list() lisp_h_check_cons_list()
# endif /* !GC_CHECK_CONS_LIST */
# if USE_LSB_TAG
#  define make_number(n) lisp_h_make_number(n)
#  define XFASTINT(a) lisp_h_XFASTINT(a)
#  define XINT(a) lisp_h_XINT(a)
#  define XTYPE(a) lisp_h_XTYPE(a)
#  define XUNTAG(a, type) lisp_h_XUNTAG(a, type)
# endif /* USE_LSB_TAG */
#endif /* check for inlining */

#if !defined(GCC_VERSION) && defined(__GNUC__) && defined(__GNUC_MINOR__)
# define GCC_VERSION (__GNUC__ * 1000 + __GNUC_MINOR__)
#endif /* !GCC_VERSION && __GNUC__ && __GNUC_MINOR__ */

/* We use __extension__ in some places to suppress -pedantic warnings
 * about GCC extensions.  This feature did NOT work properly before
 * gcc 2.8.  */
#if !defined(__extension__) && defined(GCC_VERSION)
# if (GCC_VERSION < 2008)
#  define __extension__
# endif /* gcc pre-2.8 */
#endif /* !__extension__ && GCC_VERSION */

#if defined(GCC_VERSION) && (GCC_VERSION < 2007)
# define __attribute__(x)
#endif /* gcc pre-2.7 */

/* Attribute __malloc__ on functions was valid as of gcc 2.96. */
#ifndef ATTRIBUTE_MALLOC
# if defined(GCC_VERSION) && (GCC_VERSION >= 2096)
#  define ATTRIBUTE_MALLOC __attribute__((__malloc__))
# else
#  define ATTRIBUTE_MALLOC /* (nothing) */
# endif /* GNUC >= 2.96 */
#endif /* ATTRIBUTE_MALLOC */

/* Define NAME as a lisp.h inline function that returns TYPE and has
   arguments declared as ARGDECLS and passed as ARGS.  ARGDECLS and
   ARGS should be parenthesized.  Implement the function by calling
   lisp_h_NAME ARGS.  */
#define LISP_MACRO_DEFUN(name, type, argdecls, args) \
  INLINE type (name) argdecls { return __extension__ (type)lisp_h_##name args; }

/* like LISP_MACRO_DEFUN, except NAME returns void.  */
#define LISP_MACRO_DEFUN_VOID(name, argdecls, args) \
  INLINE void (name) argdecls { lisp_h_##name args; }


/* Define the fundamental Lisp data structures.  */

/* This is the set of Lisp data types.  If you want to define a new
   data type, read the comments after Lisp_Fwd_Type definition
   below.  */

/* Lisp integers use 2 tags, to give them one extra bit, thus
   extending their range from, e.g., -2^28..2^28-1 to -2^29..2^29-1.  */
#define INTMASK (EMACS_INT_MAX >> (INTTYPEBITS - 1))
#define case_Lisp_Int case Lisp_Int0: case Lisp_Int1

/* back-up re-definition of __extension__ has been moved above */

/* Idea stolen from GDB.  Pedantic GCC complains about enum bitfields,
   MSVC does NOT support them, and xlc and Oracle Studio c99 complain
   vociferously about them.  */
#if (defined __STRICT_ANSI__ || defined _MSC_VER || defined __IBMC__ \
     || (defined __SUNPRO_C && __STDC__))
# define ENUM_BF(TYPE) unsigned int
#else
# define ENUM_BF(TYPE) __extension__ enum TYPE
#endif /* pedantry */

#ifndef SYMBOL_VALUE
# define SYMBOL_VALUE(sym)			\
    (XSYMBOL(sym)->indirect_variable		\
     ? XSYMBOL(indirect_variable(sym))->value	\
     : XSYMBOL(sym)->value)
#endif /* !SYMBOL_VALUE */

enum Lisp_Type
  {
    /* Integer.  XINT (obj) is the integer value.  */
    Lisp_Int0 = 0,
    Lisp_Int1 = USE_LSB_TAG ? 1 << INTTYPEBITS : 1,

    /* Symbol.  XSYMBOL (object) points to a struct Lisp_Symbol.  */
    Lisp_Symbol = 2,

    /* Miscellaneous.  XMISC (object) points to a union Lisp_Misc,
       whose first member indicates the subtype.  */
    Lisp_Misc = 3,

    /* String.  XSTRING (object) points to a struct Lisp_String.
       The length of the string, and its contents, are stored therein.  */
    Lisp_String = USE_LSB_TAG ? 1 : 1 << INTTYPEBITS,

    /* Vector of Lisp objects, or something resembling it.
       XVECTOR (object) points to a struct Lisp_Vector, which contains
       the size and contents.  The size field also contains the type
       information, if it's not a real vector object.  */
    Lisp_Vectorlike = 5,

    /* Cons.  XCONS (object) points to a struct Lisp_Cons.  */
    Lisp_Cons = 6,

    Lisp_Float = 7
  };

/* This is the set of data types that share a common structure.
   The first member of the structure is a type code from this set.
   The enum values are arbitrary, but we'll use large numbers to make it
   more likely that we'll spot the error if a random word in memory is
   mistakenly interpreted as a Lisp_Misc.  */
enum Lisp_Misc_Type
  {
    Lisp_Misc_Free = 0x5eab,
    Lisp_Misc_Marker,
    Lisp_Misc_Overlay,
    Lisp_Misc_Save_Value,
    /* Currently floats are not a misc type,
       but let's define this in case we want to change that.  */
    Lisp_Misc_Float,
    /* This is not a type code.  It is for range checking.  */
    Lisp_Misc_Limit
  };

/* These are the types of forwarding objects used in the value slot
   of symbols for special built-in variables whose value is stored in
   C variables.  */
enum Lisp_Fwd_Type
  {
    Lisp_Fwd_Int,	 /* Fwd to a C `int' variable.  */
    Lisp_Fwd_Bool,	 /* Fwd to a C boolean var.  */
    Lisp_Fwd_Obj,	 /* Fwd to a C Lisp_Object variable.  */
    Lisp_Fwd_Buffer_Obj, /* Fwd to a Lisp_Object field of buffers.  */
    Lisp_Fwd_Kboard_Obj	 /* Fwd to a Lisp_Object field of kboards.  */
  };

/* If you want to define a new Lisp data type, here are some
   instructions.  See the thread at
   http://lists.gnu.org/archive/html/emacs-devel/2012-10/msg00561.html
   for more info.

   First, there are already a couple of Lisp types that can be used if
   your new type does not need to be exposed to Lisp programs nor
   displayed to users.  These are Lisp_Save_Value, a Lisp_Misc
   subtype; and PVEC_OTHER, a kind of vectorlike object.  The former
   is suitable for temporarily stashing away pointers and integers in
   a Lisp object.  The latter is useful for vector-like Lisp objects
   that need to be used as part of other objects, but which are never
   shown to users or Lisp code (search for PVEC_OTHER in xterm.c for
   an example).

   These two types do NOT look pretty when printed, so they are
   unsuitable for Lisp objects that can be exposed to users.

   To define a new data type, add one more Lisp_Misc subtype or one
   more pseudovector subtype.  Pseudovectors are more suitable for
   objects with several slots that need to support fast random access,
   while Lisp_Misc types are for everything else.  A pseudovector object
   provides one or more slots for Lisp objects, followed by struct
   members that are accessible only from C.  A Lisp_Misc object is a
   wrapper for a C struct that can contain anything you like.

   Explicit freeing is discouraged for Lisp objects in general.  But if
   you really need to exploit this, use Lisp_Misc (check free_misc in
   alloc.c to see why).  There is no way to free a vectorlike object.

   To add a new pseudovector type, extend the pvec_type enumeration;
   to add a new Lisp_Misc, extend the Lisp_Misc_Type enumeration.

   For a Lisp_Misc, you will also need to add your entry to union
   Lisp_Misc (but make sure the first word has the same structure as
   the others, starting with a 16-bit member of the Lisp_Misc_Type
   enumeration and a 1-bit GC markbit) and make sure the overall size
   of the union is not increased by your addition.

   For a new pseudovector, it is highly desirable to limit the size
   of your data type by VBLOCK_BYTES_MAX bytes (defined in alloc.c).
   Otherwise you will need to change sweep_vectors (also in alloc.c).

   Then you will need to add switch branches in print.c (in
   print_object, to print your object, and possibly also in
   print_preprocess) and to alloc.c, to mark your object (in
   mark_object) and to free it (in gc_sweep).  The latter is also the
   right place to call any code specific to your data type that needs
   to run when the object is recycled -- e.g., free any additional
   resources allocated for it that are not Lisp objects.  You can even
   make a pointer to the function that frees the resources a slot in
   your object -- this way, the same object could be used to represent
   several disparate C structures.  */

#ifdef CHECK_LISP_OBJECT_TYPE
/* Caveat: adding '__attribute__((aligned))' is an attempt to hunt a bug;
 * I am not sure if it is correct or not... */
typedef struct { EMACS_INT i; } Lisp_Object __attribute__((aligned(__alignof__(EMACS_INT))));
# define LISP_INITIALLY_ZERO { 0 }
# undef CHECK_LISP_OBJECT_TYPE
enum CHECK_LISP_OBJECT_TYPE { CHECK_LISP_OBJECT_TYPE = true };
#else /* CHECK_LISP_OBJECT_TYPE */
/* If a struct type is not wanted, define Lisp_Object as just a number: */
typedef EMACS_INT Lisp_Object;
# define LISP_INITIALLY_ZERO 0
enum CHECK_LISP_OBJECT_TYPE { CHECK_LISP_OBJECT_TYPE = false };
#endif /* CHECK_LISP_OBJECT_TYPE */

#ifdef __GNUC__
static __inline__ Lisp_Object LISP_MAKE_RVALUE(Lisp_Object o)
{
  return o;
}
#else
/* This is NOT quite right - it keeps the argument as an lvalue.
 * Making it const via casting would help avoid code actually
 * modifying the location in question, but the casting could cover
 * other type-related bugs.  */
# define LISP_MAKE_RVALUE(o) (o)
#endif /* __GNUC__ */

/* Convert a Lisp_Object to the corresponding EMACS_INT and vice versa.
   At the machine level, these operations are no-ops.  */
LISP_MACRO_DEFUN (XLI, EMACS_INT, (Lisp_Object o), (o))
LISP_MACRO_DEFUN (XIL, Lisp_Object, (EMACS_INT i), (i))

/* In the size word of a vector, this bit means the vector has been marked.  */

DEFINE_GDB_SYMBOL_BEGIN (ptrdiff_t, ARRAY_MARK_FLAG)
#define ARRAY_MARK_FLAG PTRDIFF_MIN
DEFINE_GDB_SYMBOL_END (ARRAY_MARK_FLAG)

/* In the size word of a struct Lisp_Vector, this bit means it's really
   some other vector-like object.  */
DEFINE_GDB_SYMBOL_BEGIN (ptrdiff_t, PSEUDOVECTOR_FLAG)
#define PSEUDOVECTOR_FLAG (PTRDIFF_MAX - PTRDIFF_MAX / 2)
DEFINE_GDB_SYMBOL_END (PSEUDOVECTOR_FLAG)

/* In a pseudovector, the size field actually contains a word with one
   PSEUDOVECTOR_FLAG bit set, and one of the following values extracted
   with PVEC_TYPE_MASK to indicate the actual type.  */
enum pvec_type
{
  PVEC_NORMAL_VECTOR,
  PVEC_FREE,
  PVEC_PROCESS,
  PVEC_FRAME,
  PVEC_WINDOW,
  PVEC_BOOL_VECTOR,
  PVEC_BUFFER,
  PVEC_HASH_TABLE,
  PVEC_TERMINAL,
  PVEC_WINDOW_CONFIGURATION,
  PVEC_SUBR,
  PVEC_OTHER,
  /* These should be last, check internal_equal to see why.  */
  PVEC_COMPILED,
  PVEC_CHAR_TABLE,
  PVEC_SUB_CHAR_TABLE,
  PVEC_FONT /* Should be last because it's used for range checking.  */
};

/* DATA_SEG_BITS forces extra bits to be or'd in with any pointers
   which were stored in a Lisp_Object.  */
#ifndef DATA_SEG_BITS
# define DATA_SEG_BITS 0
#endif /* !DATA_SEG_BITS */
enum { gdb_DATA_SEG_BITS = DATA_SEG_BITS };
#undef DATA_SEG_BITS

enum More_Lisp_Bits
  {
    DATA_SEG_BITS = gdb_DATA_SEG_BITS,

    /* For convenience, we also store the number of elements in these bits.
       Note that this size is not necessarily the memory-footprint size, but
       only the number of Lisp_Object fields (that need to be traced by GC).
       The distinction is used, e.g., by Lisp_Process, which places extra
       non-Lisp_Object fields at the end of the structure.  */
    PSEUDOVECTOR_SIZE_BITS = 12,
    PSEUDOVECTOR_SIZE_MASK = (1 << PSEUDOVECTOR_SIZE_BITS) - 1,

    /* To calculate the memory footprint of the pseudovector, it's useful
       to store the size of non-Lisp area in word_size units here.  */
    PSEUDOVECTOR_REST_BITS = 12,
    PSEUDOVECTOR_REST_MASK = (((1 << PSEUDOVECTOR_REST_BITS) - 1)
			      << PSEUDOVECTOR_SIZE_BITS),

    /* Used to extract pseudovector subtype information.  */
    PSEUDOVECTOR_AREA_BITS = PSEUDOVECTOR_SIZE_BITS + PSEUDOVECTOR_REST_BITS,
    PVEC_TYPE_MASK = 0x3f << PSEUDOVECTOR_AREA_BITS
  };

/* These functions extract various sorts of values from a Lisp_Object.
   For example, if tem is a Lisp_Object whose type is Lisp_Cons,
   XCONS (tem) is the struct Lisp_Cons * pointing to the memory for
   that cons.  */

DEFINE_GDB_SYMBOL_BEGIN (EMACS_INT, VALMASK)
#define VALMASK (USE_LSB_TAG ? - (1 << GCTYPEBITS) : VAL_MAX)
DEFINE_GDB_SYMBOL_END (VALMASK)

/* Largest and smallest representable fixnum values.  These are the C
   values.  They are macros for use in static initializers.  */
#define MOST_POSITIVE_FIXNUM (EMACS_INT_MAX >> INTTYPEBITS)
#define MOST_NEGATIVE_FIXNUM (-1 - MOST_POSITIVE_FIXNUM)

/* Extract the pointer hidden within A.  */
LISP_MACRO_DEFUN (XPNTR, void *, (Lisp_Object a), (a))

#if USE_LSB_TAG

# ifdef NO_UNION_TYPE
#  ifndef TYPEMASK
#   define TYPEMASK ((((EMACS_INT) 1) << GCTYPEBITS) - 1)
#  endif /* !TYPEMASK */
# endif /* NO_UNION_TYPE */
LISP_MACRO_DEFUN (make_number, Lisp_Object, (EMACS_INT n), (n))
LISP_MACRO_DEFUN (XINT, EMACS_INT, (Lisp_Object a), (a))
LISP_MACRO_DEFUN (XFASTINT, EMACS_INT, (Lisp_Object a), (a))
LISP_MACRO_DEFUN (XTYPE, enum Lisp_Type, (Lisp_Object a), (a))
LISP_MACRO_DEFUN (XUNTAG, void *, (Lisp_Object a, int type), (a, type))

#else /* ! USE_LSB_TAG */

/* Although compiled only if ! USE_LSB_TAG, the following functions
   also work when USE_LSB_TAG; this is to aid future maintenance when
   the lisp_h_* macros are eventually removed.  */

/* Make a Lisp integer representing the value of the low order
   bits of N.  */
INLINE Lisp_Object
make_number (EMACS_INT n)
{
  return XIL (USE_LSB_TAG ? n << INTTYPEBITS : n & INTMASK);
}

/* Extract A's value as a signed integer.  */
INLINE EMACS_INT
XINT (Lisp_Object a)
{
  EMACS_INT i = XLI (a);
  return (USE_LSB_TAG ? i : i << INTTYPEBITS) >> INTTYPEBITS;
}

/* Like XINT (A), but may be faster.  A must be nonnegative.
   If ! USE_LSB_TAG, this takes advantage of the fact that Lisp
   integers have zero-bits in their tags.  */
INLINE EMACS_INT
XFASTINT (Lisp_Object a)
{
  EMACS_INT n = USE_LSB_TAG ? XINT (a) : XLI (a);
  eassert (0 <= n);
  return n;
}

/* Extract A's type: */
INLINE enum Lisp_Type
XTYPE(Lisp_Object a)
{
  EMACS_UINT i = XLI(a);
  return (enum Lisp_Type)(USE_LSB_TAG ? (i & ~VALMASK) : (i >> VALBITS));
}

/* Extract A's pointer value, assuming A's type is TYPE.  */
INLINE void *
XUNTAG (Lisp_Object a, int type)
{
  if (USE_LSB_TAG)
    {
      intptr_t i = XLI (a) - type;
      return (void *) i;
    }
  return XPNTR (a);
}

#endif /* ! USE_LSB_TAG */

/* During garbage collection, XGCTYPE must be used for extracting types
 * so that the mark bit is ignored.  XMARKBIT accesses the markbit.
 * Markbits are used only in particular slots of particular structure types.
 * Other markbits are always zero.
 * Outside of garbage collection, all mark bits are always zero.  */

#ifndef XGCTYPE
/* The distinction does not exist now that the MARKBIT has been eliminated: */
# define XGCTYPE(a) XTYPE(a)
#endif /* !XGCTYPE */

#ifndef XPNTR
# if defined(HAVE_SHM) && !defined(pure_size)
/* In this representation, data is found in two widely separated segments: */
extern size_t pure_size;
# endif /* HAVE_SHM && !pure_size */
#endif /* no XPNTR */

/* Extract A's value as an unsigned integer: */
INLINE EMACS_UINT XUINT(Lisp_Object a)
{
  EMACS_UINT i = (EMACS_UINT)XLI(a);
  return (USE_LSB_TAG ? (i >> INTTYPEBITS) : (i & INTMASK));
}

/* Return A's (Lisp-integer sized) hash.  Happens to be like XUINT
   right now, but XUINT should only be applied to objects we know are
   integers.  */
LISP_MACRO_DEFUN(XHASH, EMACS_INT, (Lisp_Object a), (a))

/* Like make_number (N), but may be faster.
 * N must be in nonnegative range: */
INLINE Lisp_Object make_natnum(EMACS_INT n)
{
  eassert((0 <= n) && (n <= MOST_POSITIVE_FIXNUM));
  return (USE_LSB_TAG ? make_number(n) : XIL(n));
}

/* Return true if X and Y are the same object.  */
LISP_MACRO_DEFUN (EQ, bool, (Lisp_Object x, Lisp_Object y), (x, y))

/* Value is true if I doesn't fit into a Lisp fixnum.  It is
   written this way so that it also works if I is of unsigned
   type or if I is a NaN.  */

#define FIXNUM_OVERFLOW_P(i) \
  (! ((0 <= (i) || MOST_NEGATIVE_FIXNUM <= (i)) && (i) <= MOST_POSITIVE_FIXNUM))

INLINE ptrdiff_t
clip_to_bounds (ptrdiff_t lower, EMACS_INT num, ptrdiff_t upper)
{
  return ((num < lower) ? lower : ((num <= upper) ? num : upper));
}

/* Forward declarations.  */

/* Defined in this file.  */
union Lisp_Fwd;
INLINE bool BOOL_VECTOR_P (Lisp_Object);
INLINE bool BUFFER_OBJFWDP (union Lisp_Fwd *);
INLINE bool BUFFERP (Lisp_Object);
INLINE bool CHAR_TABLE_P (Lisp_Object);
INLINE Lisp_Object CHAR_TABLE_REF_ASCII (Lisp_Object, ptrdiff_t);
INLINE bool (CONSP) (Lisp_Object);
INLINE bool (FLOATP) (Lisp_Object);
INLINE bool functionp (Lisp_Object);
INLINE bool (INTEGERP) (Lisp_Object);
INLINE bool (MARKERP) (Lisp_Object);
INLINE bool (MISCP) (Lisp_Object);
INLINE bool (NILP) (Lisp_Object);
INLINE bool OVERLAYP (Lisp_Object);
INLINE bool PROCESSP (Lisp_Object);
INLINE bool PSEUDOVECTORP (Lisp_Object, int);
INLINE bool SAVE_VALUEP (Lisp_Object);
INLINE void set_sub_char_table_contents (Lisp_Object, ptrdiff_t,
					      Lisp_Object);
INLINE bool STRINGP (Lisp_Object);
INLINE bool SUB_CHAR_TABLE_P (Lisp_Object);
INLINE bool SUBRP (Lisp_Object);
INLINE bool (SYMBOLP) (Lisp_Object);
INLINE bool (VECTORLIKEP) (Lisp_Object);
INLINE bool WINDOWP (Lisp_Object);
INLINE struct Lisp_Save_Value *XSAVE_VALUE (Lisp_Object);

/* Defined in chartab.c.  */
extern Lisp_Object char_table_ref (Lisp_Object, int);
extern void char_table_set (Lisp_Object, int, Lisp_Object);
extern int char_table_translate (Lisp_Object, int);

/* Defined in data.c.  */
extern Lisp_Object Qarrayp, Qbufferp, Qbuffer_or_string_p, Qchar_table_p;
extern Lisp_Object Qconsp, Qfloatp, Qintegerp, Qlambda, Qlistp, Qmarkerp, Qnil;
extern Lisp_Object Qnumberp, Qstringp, Qsymbolp, Qt, Qvectorp;
extern Lisp_Object Qbool_vector_p;
extern Lisp_Object Qvector_or_char_table_p, Qwholenump;
extern Lisp_Object Qwindow;
extern Lisp_Object Ffboundp (Lisp_Object);
extern _Noreturn Lisp_Object wrong_type_argument (Lisp_Object, Lisp_Object);

/* Defined in emacs.c.  */
extern bool initialized;

/* Defined in eval.c.  */
extern Lisp_Object Qautoload;

/* Defined in floatfns.c.  */
extern double extract_float (Lisp_Object);

/* Defined in process.c.  */
extern Lisp_Object Qprocessp;

/* Defined in window.c.  */
extern Lisp_Object Qwindowp;

/* Defined in xdisp.c.  */
extern Lisp_Object Qimage;


/* Extract a value or address from a Lisp_Object.  */

LISP_MACRO_DEFUN (XCONS, struct Lisp_Cons *, (Lisp_Object a), (a))

INLINE struct Lisp_Vector *
XVECTOR (Lisp_Object a)
{
  eassert (VECTORLIKEP (a));
  return XUNTAG (a, Lisp_Vectorlike);
}

INLINE struct Lisp_String *
XSTRING (Lisp_Object a)
{
  eassert (STRINGP (a));
  return XUNTAG (a, Lisp_String);
}

LISP_MACRO_DEFUN (XSYMBOL, struct Lisp_Symbol *, (Lisp_Object a), (a))

INLINE struct Lisp_Float *
XFLOAT (Lisp_Object a)
{
  eassert (FLOATP (a));
  return XUNTAG (a, Lisp_Float);
}

/* Pseudovector types: */
INLINE struct Lisp_Process *
XPROCESS (Lisp_Object a)
{
  eassert (PROCESSP (a));
  return XUNTAG (a, Lisp_Vectorlike);
}

INLINE struct window *
XWINDOW (Lisp_Object a)
{
  eassert (WINDOWP (a));
  return XUNTAG (a, Lisp_Vectorlike);
}

INLINE struct terminal *
XTERMINAL (Lisp_Object a)
{
  return XUNTAG (a, Lisp_Vectorlike);
}

INLINE struct Lisp_Subr *
XSUBR (Lisp_Object a)
{
  eassert (SUBRP (a));
  return XUNTAG (a, Lisp_Vectorlike);
}

INLINE struct buffer *
XBUFFER (Lisp_Object a)
{
  eassert (BUFFERP (a));
  return XUNTAG (a, Lisp_Vectorlike);
}

INLINE struct Lisp_Char_Table *
XCHAR_TABLE (Lisp_Object a)
{
  eassert (CHAR_TABLE_P (a));
  return XUNTAG (a, Lisp_Vectorlike);
}

INLINE struct Lisp_Sub_Char_Table *
XSUB_CHAR_TABLE (Lisp_Object a)
{
  eassert (SUB_CHAR_TABLE_P (a));
  return XUNTAG (a, Lisp_Vectorlike);
}

INLINE struct Lisp_Bool_Vector *
XBOOL_VECTOR (Lisp_Object a)
{
  eassert (BOOL_VECTOR_P (a));
  return XUNTAG (a, Lisp_Vectorlike);
}

/* Construct a Lisp_Object from a value or address: */
INLINE Lisp_Object make_lisp_ptr(void *ptr, enum Lisp_Type type)
{
  EMACS_UINT utype = type;
  EMACS_UINT typebits = (USE_LSB_TAG ? type : (utype << VALBITS));
  Lisp_Object a =
    __extension__ (Lisp_Object)XIL((EMACS_INT)(typebits | (uintptr_t)ptr));
  eassert((XTYPE(a) == type) && (XUNTAG(a, (int)type) == ptr));
  return a;
}

INLINE Lisp_Object
make_lisp_proc(struct Lisp_Process *p)
{
  return make_lisp_ptr(p, Lisp_Vectorlike);
}

#define XSETINT(a, b) ((a) = make_number(b))
#define XSETFASTINT(a, b) ((a) = make_natnum((EMACS_INT)(b)))
#define XSETCONS(a, b) ((a) = make_lisp_ptr(b, Lisp_Cons))
#define XSETVECTOR(a, b) ((a) = make_lisp_ptr(b, Lisp_Vectorlike))
#define XSETSTRING(a, b) ((a) = make_lisp_ptr(b, Lisp_String))
#define XSETSYMBOL(a, b) ((a) = make_lisp_ptr(b, Lisp_Symbol))
#define XSETFLOAT(a, b) ((a) = make_lisp_ptr(b, Lisp_Float))
#define XSETMISC(a, b) ((a) = make_lisp_ptr(b, Lisp_Misc))

/* Pseudovector types: */
#define XSETPVECTYPE(v, code)						\
  ((v)->header.size |= PSEUDOVECTOR_FLAG | ((code) << PSEUDOVECTOR_AREA_BITS))
#define XSETPVECTYPESIZE(v, code, lispsize, restsize)		\
  ((v)->header.size = (PSEUDOVECTOR_FLAG			\
		       | ((code) << PSEUDOVECTOR_AREA_BITS)	\
		       | ((restsize) << PSEUDOVECTOR_SIZE_BITS) \
		       | (lispsize)))

/* The cast to struct vectorlike_header * avoids aliasing issues.  */
#define XSETPSEUDOVECTOR(a, b, code) \
  XSETTYPED_PSEUDOVECTOR (a, b,					\
			  (((struct vectorlike_header *)	\
			    XUNTAG (a, Lisp_Vectorlike))	\
			   ->size),				\
			  code)
#define XSETTYPED_PSEUDOVECTOR(a, b, size, code)			\
  (XSETVECTOR (a, b),							\
   eassert ((size & (PSEUDOVECTOR_FLAG | PVEC_TYPE_MASK))		\
	    == (PSEUDOVECTOR_FLAG | (code << PSEUDOVECTOR_AREA_BITS))))

#define XSETWINDOW_CONFIGURATION(a, b) \
  (XSETPSEUDOVECTOR (a, b, PVEC_WINDOW_CONFIGURATION))
#define XSETPROCESS(a, b) (XSETPSEUDOVECTOR (a, b, PVEC_PROCESS))
#define XSETWINDOW(a, b) (XSETPSEUDOVECTOR (a, b, PVEC_WINDOW))
#define XSETTERMINAL(a, b) (XSETPSEUDOVECTOR (a, b, PVEC_TERMINAL))
#define XSETSUBR(a, b) (XSETPSEUDOVECTOR (a, b, PVEC_SUBR))
#define XSETCOMPILED(a, b) (XSETPSEUDOVECTOR (a, b, PVEC_COMPILED))
#define XSETBUFFER(a, b) (XSETPSEUDOVECTOR (a, b, PVEC_BUFFER))
#define XSETCHAR_TABLE(a, b) (XSETPSEUDOVECTOR (a, b, PVEC_CHAR_TABLE))
#define XSETBOOL_VECTOR(a, b) (XSETPSEUDOVECTOR (a, b, PVEC_BOOL_VECTOR))
#define XSETSUB_CHAR_TABLE(a, b) (XSETPSEUDOVECTOR (a, b, PVEC_SUB_CHAR_TABLE))

/* Type checking: */
LISP_MACRO_DEFUN_VOID (CHECK_TYPE, (int ok, Lisp_Object Qxxxp, Lisp_Object x),
		       (ok, Qxxxp, x))

/* Deprecated and will be removed soon: */
#ifndef INTERNAL_FIELD
# define INTERNAL_FIELD(field) field ## _
#endif /* !INTERNAL_FIELD */

/* See the macros in intervals.h: */
typedef struct interval *INTERVAL;

/* Complain if object is not string or buffer type: */
#if !defined(CHECK_STRING_OR_BUFFER) && !defined(TMEM) && !defined(PLIST_ELT_P)
# define CHECK_STRING_OR_BUFFER(x) \
   CHECK_TYPE(STRINGP(x) || BUFFERP(x), Qbuffer_or_string_p, x)
#endif /* !CHECK_STRING_OR_BUFFER && !TMEM && !PLIST_ELT_P */

/* In a cons, the markbit of the car is the gc mark bit: */
struct Lisp_Cons
  {
    /* Car of this cons cell.  */
    Lisp_Object car;

    union
    {
      /* Cdr of this cons cell.  */
      Lisp_Object cdr;

      /* Used to chain conses on a free list.  */
      struct Lisp_Cons *chain;
    } u;
  };

/* Take the car or cdr of something known to be a cons cell.  */
/* The _addr functions and _AS_LVALUE macros should NOT be used outside
 * of the minimal set of code that has to know what a cons cell looks like.
 * Other code not part of the basic lisp implementation should assume that
 * the car and cdr fields are not accessible (as lvalues).
 * (What if we want to switch to a copying collector someday? Cached cons
 * cell field addresses may be invalidated at arbitrary points.)  */
#ifndef XCAR_AS_LVALUE
# define XCAR_AS_LVALUE(c) (XCONS((c))->car)
#endif /* !XCAR_AS_LVALUE */
#ifndef XCDR_AS_LVALUE
# define XCDR_AS_LVALUE(c) (XCONS((c))->u.cdr)
#endif /* !XCDR_AS_LVALUE */
INLINE Lisp_Object *xcar_addr(Lisp_Object c)
{
  return &XCONS(c)->car;
}
INLINE Lisp_Object *xcdr_addr(Lisp_Object c)
{
  return &XCONS(c)->u.cdr;
}

/* Use these from normal code: */
LISP_MACRO_DEFUN(XCAR, Lisp_Object, (Lisp_Object c), (c))
LISP_MACRO_DEFUN(XCDR, Lisp_Object, (Lisp_Object c), (c))

/* Use these to set the fields of a cons cell.

   Note that both arguments may refer to the same object, so 'n'
   should not be read after 'c' is first modified.  */
INLINE void
XSETCAR (Lisp_Object c, Lisp_Object n)
{
  *xcar_addr (c) = n;
}
INLINE void
XSETCDR (Lisp_Object c, Lisp_Object n)
{
  *xcdr_addr (c) = n;
}

/* Take the car or cdr of something whose type is not known.  */
INLINE Lisp_Object
CAR (Lisp_Object c)
{
  return (CONSP (c) ? XCAR (c)
	  : NILP (c) ? Qnil
	  : wrong_type_argument (Qlistp, c));
}
INLINE Lisp_Object
CDR (Lisp_Object c)
{
  return (CONSP (c) ? XCDR (c)
	  : NILP (c) ? Qnil
	  : wrong_type_argument (Qlistp, c));
}

/* Take the car or cdr of something whose type is not known.  */
INLINE Lisp_Object
CAR_SAFE (Lisp_Object c)
{
  return CONSP (c) ? XCAR (c) : Qnil;
}
INLINE Lisp_Object
CDR_SAFE (Lisp_Object c)
{
  return CONSP (c) ? XCDR (c) : Qnil;
}

/* Get text properties: */
#ifndef STRING_INTERVALS
# define STRING_INTERVALS(STR)  (XSTRING(STR)->intervals + 0)
#endif /* !STRING_INTERVALS */

/* Set text properties: */
#ifndef STRING_SET_INTERVALS
# define STRING_SET_INTERVALS(STR, INT) (XSTRING(STR)->intervals = (INT))
#endif /* !STRING_SET_INTERVALS */

/* In a string or vector, the sign bit of the `size' is the gc mark bit.  */

struct Lisp_String
  {
    ptrdiff_t size;
    ptrdiff_t size_byte;
    INTERVAL intervals;		/* Text properties in this string.  */
    unsigned char *data;
  };

#ifndef OFFSETOF
# ifdef offsetof
#  define OFFSETOF(type,field) offsetof(type,field)
# else
#  define OFFSETOF(type,field) \
     ((int)((char*)&((type*)0)->field - (char*)0))
# endif /* offsetof */
#endif /* !OFFSETOF */

/* True if STR is a multibyte string: */
INLINE bool
STRING_MULTIBYTE(Lisp_Object str)
{
  struct Lisp_String *lstr = XSTRING(str);
  eassert(lstr != NULL);
  return (0 <= lstr->size_byte);
}

/* An upper bound on the number of bytes in a Lisp string, not
   counting the terminating null.  This a tight enough bound to
   prevent integer overflow errors that would otherwise occur during
   string size calculations.  A string cannot contain more bytes than
   a fixnum can represent, nor can it be so long that C pointer
   arithmetic stops working on the string plus its terminating null.
   Although the actual size limit (see STRING_BYTES_MAX in alloc.c)
   may be a bit smaller than STRING_BYTES_BOUND, calculating it here
   would expose alloc.c internal details that we'd rather keep
   private.

   This is a macro for use in static initializers.  The cast to
   ptrdiff_t ensures that the macro is signed.  */
#define STRING_BYTES_BOUND  \
  ((ptrdiff_t) min (MOST_POSITIVE_FIXNUM, min (SIZE_MAX, PTRDIFF_MAX) - 1))

/* Mark STR as a unibyte string.  */
#define STRING_SET_UNIBYTE(STR)				\
  do {							\
    if (EQ (STR, empty_multibyte_string))		\
      (STR) = empty_unibyte_string;			\
    else						\
      XSTRING (STR)->size_byte = -1;			\
  } while (false)

/* Mark STR as a multibyte string.  Assure that STR contains only
   ASCII characters in advance.  */
#define STRING_SET_MULTIBYTE(STR)			\
  do {							\
    if (EQ(STR, empty_unibyte_string))			\
      (STR) = empty_multibyte_string;			\
    else						\
      XSTRING(STR)->size_byte = XSTRING(STR)->size;	\
  } while (false)

/* Convenience functions for dealing with Lisp strings: */
INLINE unsigned char *SDATA(Lisp_Object string)
{
  struct Lisp_String *lstr = XSTRING(string);
  eassert(lstr != NULL);
  /* Avoid "differ in sign" warnings: */
  return (unsigned char *)lstr->data;
}
INLINE char *SSDATA(Lisp_Object string)
{
  /* Avoid "differ in sign" warnings: */
  return (char *)SDATA(string);
}
INLINE unsigned char SREF(Lisp_Object string, ptrdiff_t arr_index)
{
  return SDATA(string)[arr_index];
}
INLINE void SSET(Lisp_Object string, ptrdiff_t array_index,
                 unsigned char newc)
{
  SDATA(string)[array_index] = newc;
}
INLINE ptrdiff_t SCHARS(Lisp_Object string)
{
  struct Lisp_String *lstr = XSTRING(string);
  eassert(lstr != NULL);
  return lstr->size;
}

#ifdef GC_CHECK_STRING_BYTES
extern ptrdiff_t string_bytes(struct Lisp_String *);
#endif /* GC_CHECK_STRING_BYTES */
INLINE ptrdiff_t STRING_BYTES(struct Lisp_String *s)
{
#ifdef GC_CHECK_STRING_BYTES
  return string_bytes(s);
#else
  return ((s->size_byte < 0) ? s->size : s->size_byte);
#endif /* GC_CHECK_STRING_BYTES */
}

INLINE ptrdiff_t SBYTES(Lisp_Object string)
{
  return STRING_BYTES(XSTRING (string));
}
INLINE void STRING_SET_CHARS(Lisp_Object string, ptrdiff_t newsize)
{
  XSTRING(string)->size = newsize;
}
INLINE void
STRING_COPYIN (Lisp_Object string, ptrdiff_t strindex, char const *newstr,
	       ptrdiff_t count)
{
  memcpy((SDATA(string) + strindex), newstr, (size_t)count);
}

/* Header of vector-like objects.  This documents the layout constraints on
   vectors and pseudovectors (objects of PVEC_xxx subtype).  It also prevents
   compilers from being fooled by Emacs's type punning: XSETPSEUDOVECTOR
   and PSEUDOVECTORP cast their pointers to struct vectorlike_header *,
   because when two such pointers potentially alias, a compiler won't
   incorrectly reorder loads and stores to their size fields.  See
   Bug#8546.  */
struct vectorlike_header
  {
    /* The only field contains various pieces of information:
       - The MSB (ARRAY_MARK_FLAG) holds the gcmarkbit.
       - The next bit (PSEUDOVECTOR_FLAG) indicates whether this is a plain
         vector (0) or a pseudovector (1).
       - If PSEUDOVECTOR_FLAG is 0, the rest holds the size (number
         of slots) of the vector.
       - If PSEUDOVECTOR_FLAG is 1, the rest is subdivided into three fields:
	 - a) pseudovector subtype held in PVEC_TYPE_MASK field;
	 - b) number of Lisp_Objects slots at the beginning of the object
	   held in PSEUDOVECTOR_SIZE_MASK field.  These objects are always
	   traced by the GC;
	 - c) size of the rest fields held in PSEUDOVECTOR_REST_MASK and
	   measured in word_size units.  Rest fields may also include
	   Lisp_Objects, but these objects usually needs some special treatment
	   during GC.
	 There are some exceptions.  For PVEC_FREE, b) is always zero.  For
	 PVEC_BOOL_VECTOR and PVEC_SUBR, both b) and c) are always zero.
	 Current layout limits the pseudovectors to 63 PVEC_xxx subtypes,
	 4095 Lisp_Objects in GC-ed area and 4095 word-sized other slots.  */
    ptrdiff_t size;
  };

/* A regular vector is just a header plus an array of Lisp_Objects: */
struct Lisp_Vector
  {
#ifdef _USE_OLD_LISP_DATA_STRUCTURES
    EMACS_INT size; /* added back from old versions of emacs */
#endif /* _USE_OLD_LISP_DATA_STRUCTURES */
    struct vectorlike_header header;
    Lisp_Object contents[FLEXIBLE_ARRAY_MEMBER];
  };

#if defined(HAVE_STDALIGN_H) && !defined(BUILDING_FROM_XCODE) && \
    !(defined(__clang_major__) && defined(__clang_minor__) && \
      (((__clang_major__ < 3) && defined(__clang_analyzer__)) || \
       ((__clang_major__ == 3) && (__clang_minor__ <= 1))))
/* C11 prohibits alignof (struct Lisp_Vector), so compute it manually: */
enum
  {
    ALIGNOF_STRUCT_LISP_VECTOR
      = alignof (union { struct vectorlike_header a; Lisp_Object b; })
  };
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__) && defined(lint) && \
     !defined(ALIGNOF_STRUCT_LISP_VECTOR)
#  warning "ALIGNOF_STRUCT_LISP_VECTOR will be unavailable."
# endif /* __GNUC__ && !__STRICT_ANSI__ && lint && !ALIGNOF_STRUCT_LISP_VECTOR */
#endif /* HAVE_STDALIGN_H && !BUILDING_FROM_XCODE && !(clang pre-3.2) */

/* A boolvector is a kind of vectorlike, with contents like a string: */
struct Lisp_Bool_Vector
  {
    /* HEADER.SIZE is the vector's size field.  It does NOT have the real
     * size, just the subtype information: */
    struct vectorlike_header header;
    /* This is the size in bits.  */
    EMACS_INT size;
    /* The actual bits, packed into bytes.
       Zeros fill out the last word if needed.
       The bits are in little-endian order in the bytes, and
       the bytes are in little-endian order in the words.  */
    bits_word data[FLEXIBLE_ARRAY_MEMBER];
  };

INLINE EMACS_INT
bool_vector_size (Lisp_Object a)
{
  EMACS_INT size = XBOOL_VECTOR (a)->size;
  eassume (0 <= size);
  return size;
}

INLINE bits_word *
bool_vector_data (Lisp_Object a)
{
  return XBOOL_VECTOR (a)->data;
}

INLINE unsigned char *
bool_vector_uchar_data (Lisp_Object a)
{
  return (unsigned char *) bool_vector_data (a);
}

/* The number of data words and bytes in a bool vector with SIZE bits.  */

INLINE EMACS_INT
bool_vector_words (EMACS_INT size)
{
  eassume (0 <= size && size <= EMACS_INT_MAX - (BITS_PER_BITS_WORD - 1));
  return (size + BITS_PER_BITS_WORD - 1) / BITS_PER_BITS_WORD;
}

INLINE EMACS_INT
bool_vector_bytes (EMACS_INT size)
{
  eassume (0 <= size && size <= EMACS_INT_MAX - (BITS_PER_BITS_WORD - 1));
  return (size + BOOL_VECTOR_BITS_PER_CHAR - 1) / BOOL_VECTOR_BITS_PER_CHAR;
}

/* True if A's Ith bit is set.  */

INLINE bool
bool_vector_bitref (Lisp_Object a, EMACS_INT i)
{
  eassume (0 <= i && i < bool_vector_size (a));
  return !! (bool_vector_uchar_data (a)[i / BOOL_VECTOR_BITS_PER_CHAR]
	     & (1 << (i % BOOL_VECTOR_BITS_PER_CHAR)));
}

INLINE Lisp_Object
bool_vector_ref (Lisp_Object a, EMACS_INT i)
{
  return bool_vector_bitref (a, i) ? Qt : Qnil;
}

/* Set A's Ith bit to B: */
INLINE void
bool_vector_set(Lisp_Object a, EMACS_INT i, bool b)
{
  unsigned char *addr;

  eassume((0 <= i) && (i < bool_vector_size(a)));
  addr = &bool_vector_uchar_data (a)[i / BOOL_VECTOR_BITS_PER_CHAR];

  if (b)
    *addr = ((unsigned char)
             (*addr | ((unsigned char)
                       (1U << (i % BOOL_VECTOR_BITS_PER_CHAR)))));
  else
    *addr &= (unsigned char)(~(1U << (i % BOOL_VECTOR_BITS_PER_CHAR)));
}

/* Some handy constants for calculating sizes
   and offsets, mostly of vectorlike objects.   */

enum
  {
    header_size = offsetof(struct Lisp_Vector, contents),
    bool_header_size = offsetof(struct Lisp_Bool_Vector, data),
    word_size = sizeof(Lisp_Object)
  };

/* Conveniences for dealing with Lisp arrays.  */

INLINE Lisp_Object
AREF(Lisp_Object array, ptrdiff_t idx)
{
  return XVECTOR(array)->contents[idx];
}

INLINE Lisp_Object *
aref_addr(Lisp_Object array, ptrdiff_t idx)
{
  return &XVECTOR(array)->contents[idx];
}

INLINE ptrdiff_t
ASIZE(Lisp_Object array)
{
  struct Lisp_Vector *lvec = XVECTOR(array);
  eassert(lvec != NULL);
  return lvec->header.size;
}

INLINE void
ASET(Lisp_Object array, ptrdiff_t idx, Lisp_Object val)
{
  eassert((0 <= idx) && (idx < ASIZE(array)));
  XVECTOR(array)->contents[idx] = val;
}

INLINE void
gc_aset(Lisp_Object array, ptrdiff_t idx, Lisp_Object val)
{
  /* Like ASET, but also can be used in the garbage collector:
     sweep_weak_table calls set_hash_key etc. while the table is marked.  */
  eassert((0 <= idx) && (idx < (ASIZE(array) & ~ARRAY_MARK_FLAG)));
  XVECTOR(array)->contents[idx] = val;
}

/* If a struct is made to look like a vector, this macro returns the length
   of the shortest vector that would hold that struct.  */

#define VECSIZE(type)						\
  ((sizeof(type) - header_size + word_size - 1) / word_size)

/* Like VECSIZE, but used when the pseudo-vector has non-Lisp_Object fields
   at the end and we need to compute the number of Lisp_Object fields (the
   ones that the GC needs to trace).  */

#define PSEUDOVECSIZE(type, nonlispfield)			\
  ((offsetof(type, nonlispfield) - header_size) / word_size)

/* Compute A OP B, using the unsigned comparison operator OP.  A and B
   should be integer expressions.  This is not the same as
   mathematical comparison; for example, UNSIGNED_CMP (0, <, -1)
   returns true.  For efficiency, prefer plain unsigned comparison if A
   and B's sizes both fit (after integer promotion).  */
#define UNSIGNED_CMP(a, op, b)						\
  ((max(sizeof((a) + 0), sizeof((b) + 0)) <= sizeof(unsigned int))	\
   ? ((a) + (unsigned int)0U) op((b) + (unsigned int)0U)		\
   : ((a) + (uintmax_t)0UL) op((b) + (uintmax_t)0UL))

/* True iff C is an ASCII character: */
#define ASCII_CHAR_P(c) UNSIGNED_CMP(c, <, 0x80)

/* A char-table is a kind of vectorlike, with contents are like a
   vector but with a few other slots.  For some purposes, it makes
   sense to handle a char-table with type struct Lisp_Vector.  An
   element of a char table can be any Lisp objects, but if it is a sub
   char-table, we treat it a table that contains information of a
   specific range of characters.  A sub char-table has the same
   structure as a vector.  A sub char table appears only in an element
   of a char-table, and there's no way to access it directly from
   Emacs Lisp program.  */

enum CHARTAB_SIZE_BITS
  {
    CHARTAB_SIZE_BITS_0 = 6,
    CHARTAB_SIZE_BITS_1 = 4,
    CHARTAB_SIZE_BITS_2 = 5,
    CHARTAB_SIZE_BITS_3 = 7
  };

extern const int chartab_size[4];

struct Lisp_Char_Table
  {
    /* HEADER.SIZE is the vector's size field, which also holds the
       pseudovector type information.  It holds the size, too.
       The size counts the defalt, parent, purpose, ascii,
       contents, and extras slots.  */
    struct vectorlike_header header;

    /* This holds a default value,
       which is used whenever the value for a specific character is nil.  */
    Lisp_Object defalt;

    /* This points to another char table, which we inherit from when the
       value for a specific character is nil.  The `defalt' slot takes
       precedence over this.  */
    Lisp_Object parent;

    /* This is a symbol which says what kind of use this char-table is
       meant for.  */
    Lisp_Object purpose;

    /* The bottom sub char-table for characters of the range 0..127.  It
       is nil if none of ASCII character has a specific value.  */
    Lisp_Object ascii;

    Lisp_Object contents[(1 << CHARTAB_SIZE_BITS_0)];

    /* These hold additional data.  It is a vector.  */
    Lisp_Object extras[FLEXIBLE_ARRAY_MEMBER];
  };

struct Lisp_Sub_Char_Table
  {
    /* HEADER.SIZE is the vector's size field, which also holds the
       pseudovector type information.  It holds the size, too.  */
    struct vectorlike_header header;

    /* Depth of this sub char-table.  It should be 1, 2, or 3.  A sub
       char-table of depth 1 contains 16 elements, and each element
       covers 4096 (128*32) characters.  A sub char-table of depth 2
       contains 32 elements, and each element covers 128 characters.  A
       sub char-table of depth 3 contains 128 elements, and each element
       is for one character.  */
    Lisp_Object depth;

    /* Minimum character covered by the sub char-table.  */
    Lisp_Object min_char;

    /* Use set_sub_char_table_contents to set this.  */
    Lisp_Object contents[FLEXIBLE_ARRAY_MEMBER];
  };

INLINE Lisp_Object
CHAR_TABLE_REF_ASCII (Lisp_Object ct, ptrdiff_t idx)
{
  struct Lisp_Char_Table *tbl = NULL;
  Lisp_Object val;
  do {
      tbl = (tbl ? XCHAR_TABLE(tbl->parent) : XCHAR_TABLE(ct));
      val = (! SUB_CHAR_TABLE_P(tbl->ascii) ? tbl->ascii
	     : XSUB_CHAR_TABLE(tbl->ascii)->contents[idx]);
      if (NILP(val))
	val = tbl->defalt;
  } while (NILP(val) && ! NILP(tbl->parent));

  return val;
}

/* Almost equivalent to Faref (CT, IDX) with optimization for ASCII
   characters.  Do not check validity of CT.  */
INLINE Lisp_Object CHAR_TABLE_REF(Lisp_Object ct, int idx)
{
  return (ASCII_CHAR_P((unsigned)idx)
	  ? CHAR_TABLE_REF_ASCII(ct, (ptrdiff_t)idx)
	  : char_table_ref(ct, idx));
}

/* Equivalent to Faset (CT, IDX, VAL) with optimization for ASCII and
   8-bit European characters.  Do not check validity of CT.  */
INLINE void CHAR_TABLE_SET(Lisp_Object ct, int idx, Lisp_Object val)
{
  if (ASCII_CHAR_P((unsigned int)idx)
      && SUB_CHAR_TABLE_P(XCHAR_TABLE(ct)->ascii))
    set_sub_char_table_contents(XCHAR_TABLE(ct)->ascii, (ptrdiff_t)idx,
                                val);
  else
    char_table_set(ct, idx, val);
}

/* This structure describes a built-in function.
   It is generated by the DEFUN macro only.
   defsubr makes it into a Lisp object.  */
struct Lisp_Subr
  {
    struct vectorlike_header header;
    union {
      Lisp_Object (*a0) (void);
      Lisp_Object (*a1) (Lisp_Object);
      Lisp_Object (*a2) (Lisp_Object, Lisp_Object);
      Lisp_Object (*a3) (Lisp_Object, Lisp_Object, Lisp_Object);
      Lisp_Object (*a4) (Lisp_Object, Lisp_Object, Lisp_Object, Lisp_Object);
      Lisp_Object (*a5) (Lisp_Object, Lisp_Object, Lisp_Object, Lisp_Object, Lisp_Object);
      Lisp_Object (*a6) (Lisp_Object, Lisp_Object, Lisp_Object, Lisp_Object, Lisp_Object, Lisp_Object);
      Lisp_Object (*a7) (Lisp_Object, Lisp_Object, Lisp_Object, Lisp_Object, Lisp_Object, Lisp_Object, Lisp_Object);
      Lisp_Object (*a8) (Lisp_Object, Lisp_Object, Lisp_Object, Lisp_Object, Lisp_Object, Lisp_Object, Lisp_Object, Lisp_Object);
      Lisp_Object (*aUNEVALLED) (Lisp_Object args);
      Lisp_Object (*aMANY) (ptrdiff_t, Lisp_Object *);
    } function;
    short min_args, max_args;
    const char *symbol_name;
    const char *intspec;
    const char *doc;
  };

/* This is the number of slots that every char table must have.  This
   counts the ordinary slots and the top, defalt, parent, and purpose
   slots.  */
enum CHAR_TABLE_STANDARD_SLOTS
  {
    CHAR_TABLE_STANDARD_SLOTS = PSEUDOVECSIZE(struct Lisp_Char_Table, extras)
  };

/* Return the number of "extra" slots in the char table CT: */
INLINE int
CHAR_TABLE_EXTRA_SLOTS(struct Lisp_Char_Table *ct)
{
  return (int)((ct->header.size & PSEUDOVECTOR_SIZE_MASK)
               - CHAR_TABLE_STANDARD_SLOTS);
}


/***********************************************************************
			       Symbols
 ***********************************************************************/

/* Interned state of a symbol.  */

enum symbol_interned
{
  SYMBOL_UNINTERNED = 0,
  SYMBOL_INTERNED = 1,
  SYMBOL_INTERNED_IN_INITIAL_OBARRAY = 2
};

enum symbol_redirect
{
  SYMBOL_PLAINVAL  = 4,
  SYMBOL_VARALIAS  = 1,
  SYMBOL_LOCALIZED = 2,
  SYMBOL_FORWARDED = 3
};

/* In a symbol, the markbit of the plist is used as the gc mark bit: */
struct Lisp_Symbol
{
  bool_bf gcmarkbit : 1;

#ifdef _USE_OLD_LISP_DATA_STRUCTURES
  /* Non-zero means symbol serves as a variable alias.  The symbol
   * holding the real value is found in the value slot.  */
  unsigned indirect_variable : 1;
#endif /* _USE_OLD_LISP_DATA_STRUCTURES */

  /* Indicates where the value can be found:
     0 : it's a plain var, the value is in the `value' field.
     1 : it's a varalias, the value is really in the `alias' symbol.
     2 : it's a localized var, the value is in the `blv' object.
     3 : it's a forwarding variable, the value is in `forward'.  */
  ENUM_BF (symbol_redirect) redirect : 3;

  /* Non-zero means symbol is constant, i.e. changing its value
     should signal an error.  If the value is 3, then the var
     can be changed, but only by `defconst'.  */
  unsigned constant : 2;

  /* Interned state of the symbol.  This is an enumerator from
     enum symbol_interned.  */
  unsigned interned : 2;

  /* True means that this variable has been explicitly declared
     special (with `defvar' etc), and shouldn't be lexically bound.  */
  bool_bf declared_special : 1;

  /* The symbol's name, as a Lisp string.  */
  Lisp_Object name;

#ifdef _USE_OLD_LISP_DATA_STRUCTURES
  /* The name "xname" was used to intentionally break code referring to
   * the old field "name" of type pointer to struct Lisp_String, but that
   * has been added back... */
  Lisp_Object xname;

  /* Value of the symbol or Qunbound if unbound.  If this symbol is a
   * defvaralias, `value' contains the symbol for which it is an
   * alias.  Use the SYMBOL_VALUE and SET_SYMBOL_VALUE macros to get
   * and set a symbol's value, to take defvaralias into account.  */
  Lisp_Object value;
#endif /* _USE_OLD_LISP_DATA_STRUCTURES */

  /* Value of the symbol or Qunbound if unbound.  Which alternative of the
     union is used depends on the `redirect' field above.  */
  union {
    Lisp_Object value;
    struct Lisp_Symbol *alias;
    struct Lisp_Buffer_Local_Value *blv;
    union Lisp_Fwd *fwd;
  } val;

  /* Function value of the symbol or Qnil if not fboundp.  */
  Lisp_Object function;

  /* The symbol's property list.  */
  Lisp_Object plist;

  /* Next symbol in obarray bucket, if the symbol is interned.  */
  struct Lisp_Symbol *next;
};

/* Value is name of symbol.  */

LISP_MACRO_DEFUN (SYMBOL_VAL, Lisp_Object, (struct Lisp_Symbol *sym), (sym))

INLINE struct Lisp_Symbol *
SYMBOL_ALIAS (struct Lisp_Symbol *sym)
{
  eassert (sym->redirect == SYMBOL_VARALIAS);
  return sym->val.alias;
}
INLINE struct Lisp_Buffer_Local_Value *
SYMBOL_BLV (struct Lisp_Symbol *sym)
{
  eassert (sym->redirect == SYMBOL_LOCALIZED);
  return sym->val.blv;
}
INLINE union Lisp_Fwd *
SYMBOL_FWD (struct Lisp_Symbol *sym)
{
  eassert (sym->redirect == SYMBOL_FORWARDED);
  return sym->val.fwd;
}

LISP_MACRO_DEFUN_VOID (SET_SYMBOL_VAL,
		       (struct Lisp_Symbol *sym, Lisp_Object v), (sym, v))

INLINE void
SET_SYMBOL_ALIAS (struct Lisp_Symbol *sym, struct Lisp_Symbol *v)
{
  eassert (sym->redirect == SYMBOL_VARALIAS);
  sym->val.alias = v;
}
INLINE void
SET_SYMBOL_BLV (struct Lisp_Symbol *sym, struct Lisp_Buffer_Local_Value *v)
{
  eassert (sym->redirect == SYMBOL_LOCALIZED);
  sym->val.blv = v;
}
INLINE void
SET_SYMBOL_FWD (struct Lisp_Symbol *sym, union Lisp_Fwd *v)
{
  eassert (sym->redirect == SYMBOL_FORWARDED);
  sym->val.fwd = v;
}

INLINE Lisp_Object
SYMBOL_NAME(Lisp_Object sym)
{
  struct Lisp_Symbol *lsym = XSYMBOL(sym);
  eassert(lsym != NULL);
  return lsym->name;
}

/* Value is true if SYM is an interned symbol.  */

INLINE bool
SYMBOL_INTERNED_P (Lisp_Object sym)
{
  return XSYMBOL (sym)->interned != SYMBOL_UNINTERNED;
}

/* Value is true if SYM is interned in initial_obarray.  */

INLINE bool
SYMBOL_INTERNED_IN_INITIAL_OBARRAY_P (Lisp_Object sym)
{
  return XSYMBOL (sym)->interned == SYMBOL_INTERNED_IN_INITIAL_OBARRAY;
}

/* Value is non-zero if symbol is considered a constant, i.e. its
   value cannot be changed (there is an exception for keyword symbols,
   whose value can be set to the keyword symbol itself).  */

LISP_MACRO_DEFUN (SYMBOL_CONSTANT_P, int, (Lisp_Object sym), (sym))

#define DEFSYM(sym, name)						\
  do { (sym) = intern_c_string ((name)); staticpro (&(sym)); } while (false)


/***********************************************************************
			     Hash Tables
 ***********************************************************************/

/* The structure of a Lisp hash table.  */

struct hash_table_test
{
  /* Name of the function used to compare keys.  */
  Lisp_Object name;

  /* User-supplied hash function, or nil.  */
  Lisp_Object user_hash_function;

  /* User-supplied key comparison function, or nil.  */
  Lisp_Object user_cmp_function;

  /* C function to compare two keys.  */
  bool (*cmpfn) (struct hash_table_test *t, Lisp_Object, Lisp_Object);

  /* C function to compute hash code.  */
  EMACS_UINT (*hashfn) (struct hash_table_test *t, Lisp_Object);
};

struct Lisp_Hash_Table
{
  /* This is for Lisp; the hash table code does not refer to it.  */
  struct vectorlike_header header;

  /* Nil if table is non-weak.  Otherwise a symbol describing the
     weakness of the table.  */
  Lisp_Object weak;

  /* When the table is resized, and this is an integer, compute the
     new size by adding this to the old size.  If a float, compute the
     new size by multiplying the old size with this factor.  */
  Lisp_Object rehash_size;

  /* Resize hash table when number of entries/ table size is >= this
     ratio, a float.  */
  Lisp_Object rehash_threshold;

  /* Vector of hash codes.. If hash[I] is nil, this means that that
     entry I is unused.  */
  Lisp_Object hash;

  /* Vector used to chain entries.  If entry I is free, next[I] is the
     entry number of the next free item.  If entry I is non-free,
     next[I] is the index of the next entry in the collision chain.  */
  Lisp_Object next;

  /* Index of first free entry in free list.  */
  Lisp_Object next_free;

  /* Bucket vector.  A non-nil entry is the index of the first item in
     a collision chain.  This vector's size can be larger than the
     hash table size to reduce collisions.  */
  Lisp_Object index;

  /* Only the fields above are traced normally by the GC.  The ones below
     `count' are special and are either ignored by the GC or traced in
     a special way (e.g. because of weakness).  */

  /* Number of key/value entries in the table.  */
  ptrdiff_t count;

  /* Vector of keys and values.  The key of item I is found at index
     2 * I, the value is found at index 2 * I + 1.
     This is gc_marked specially if the table is weak.  */
  Lisp_Object key_and_value;

  /* The comparison and hash functions.  */
  struct hash_table_test test;

  /* Next weak hash table if this is a weak hash table.  The head
     of the list is in weak_hash_tables.  */
  struct Lisp_Hash_Table *next_weak;
};


INLINE struct Lisp_Hash_Table *
XHASH_TABLE (Lisp_Object a)
{
  return XUNTAG (a, Lisp_Vectorlike);
}

#define XSET_HASH_TABLE(VAR, PTR) \
     (XSETPSEUDOVECTOR (VAR, PTR, PVEC_HASH_TABLE))

INLINE bool
HASH_TABLE_P (Lisp_Object a)
{
  return PSEUDOVECTORP (a, PVEC_HASH_TABLE);
}

/* Value is the key part of entry IDX in hash table H.  */
INLINE Lisp_Object
HASH_KEY (struct Lisp_Hash_Table *h, ptrdiff_t idx)
{
  return AREF (h->key_and_value, 2 * idx);
}

/* Value is the value part of entry IDX in hash table H.  */
INLINE Lisp_Object
HASH_VALUE (struct Lisp_Hash_Table *h, ptrdiff_t idx)
{
  return AREF (h->key_and_value, 2 * idx + 1);
}

/* Value is the index of the next entry following the one at IDX
   in hash table H.  */
INLINE Lisp_Object
HASH_NEXT (struct Lisp_Hash_Table *h, ptrdiff_t idx)
{
  return AREF (h->next, idx);
}

/* Value is the hash code computed for entry IDX in hash table H.  */
INLINE Lisp_Object
HASH_HASH (struct Lisp_Hash_Table *h, ptrdiff_t idx)
{
  return AREF (h->hash, idx);
}

/* Value is the index of the element in hash table H that is the
   start of the collision list at index IDX in the index vector of H.  */
INLINE Lisp_Object
HASH_INDEX (struct Lisp_Hash_Table *h, ptrdiff_t idx)
{
  return AREF (h->index, idx);
}

/* Value is the size of hash table H.  */
INLINE ptrdiff_t
HASH_TABLE_SIZE (struct Lisp_Hash_Table *h)
{
  return ASIZE (h->next);
}

/* Default size for hash tables if not specified.  */

enum DEFAULT_HASH_SIZE { DEFAULT_HASH_SIZE = 65 };

/* Default threshold specifying when to resize a hash table.  The
   value gives the ratio of current entries in the hash table and the
   size of the hash table.  */

static double const DEFAULT_REHASH_THRESHOLD ATTRIBUTE_USED = 0.8;

/* Default factor by which to increase the size of a hash table: */
static double const DEFAULT_REHASH_SIZE ATTRIBUTE_USED = 1.5;

/* Combine two integers X and Y for hashing.  The result might not fit
   into a Lisp integer.  */

INLINE EMACS_UINT
sxhash_combine (EMACS_UINT x, EMACS_UINT y)
{
  return (x << 4) + (x >> (BITS_PER_EMACS_INT - 4)) + y;
}

/* Hash X, returning a value that fits into a fixnum.  */

INLINE EMACS_UINT
SXHASH_REDUCE (EMACS_UINT x)
{
  return (x ^ x >> (BITS_PER_EMACS_INT - FIXNUM_BITS)) & INTMASK;
}

/* These structures are used for various misc types.  */

struct Lisp_Misc_Any		/* Supertype of all Misc types.  */
{
  ENUM_BF (Lisp_Misc_Type) type : 16;		/* = Lisp_Misc_??? */
  bool_bf gcmarkbit : 1;
  unsigned spacer : 15;
};

struct Lisp_Marker
{
  ENUM_BF (Lisp_Misc_Type) type : 16;		/* = Lisp_Misc_Marker */
  bool_bf gcmarkbit : 1;
  unsigned spacer : 13;
  /* This flag is temporarily used in the functions
     decode/encode_coding_object to record that the marker position
     must be adjusted after the conversion.  */
  bool_bf need_adjustment : 1;
  /* True means normal insertion at the marker's position
     leaves the marker after the inserted text.  */
  bool_bf insertion_type : 1;
  /* This is the buffer that the marker points into, or 0 if it points nowhere.
     Note: a chain of markers can contain markers pointing into different
     buffers (the chain is per buffer_text rather than per buffer, so it's
     shared between indirect buffers).  */
  /* This is used for (other than NULL-checking):
     - Fmarker_buffer
     - Fset_marker: check eq(oldbuf, newbuf) to avoid unchain+rechain.
     - unchain_marker: to find the list from which to unchain.
     - Fkill_buffer: to only unchain the markers of current indirect buffer.
     */
  struct buffer *buffer;

  /* The remaining fields are meaningless in a marker that
     does not point anywhere.  */

  /* For markers that point somewhere,
     this is used to chain of all the markers in a given buffer.  */
  /* We could remove it and use an array in buffer_text instead.
     That would also allow to preserve it ordered.  */
  struct Lisp_Marker *next;
  /* This is the char position where the marker points.  */
  ptrdiff_t charpos;
  /* This is the byte position.
     It's mostly used as a charpos<->bytepos cache (i.e. it's not directly
     used to implement the functionality of markers, but rather to (ab)use
     markers as a cache for char<->byte mappings).  */
  ptrdiff_t bytepos;
};

/* START and END are markers in the overlay's buffer, and
   PLIST is the overlay's property list.  */
struct Lisp_Overlay
/* An overlay's real data content is:
   - plist
   - buffer (really there are two buffer pointers, one per marker,
     and both points to the same buffer)
   - insertion type of both ends (per-marker fields)
   - start & start byte (of start marker)
   - end & end byte (of end marker)
   - next (singly linked list of overlays)
   - next fields of start and end markers (singly linked list of markers).
   I.e. 9words plus 2 bits, 3words of which are for external linked lists.
*/
  {
    ENUM_BF (Lisp_Misc_Type) type : 16;	/* = Lisp_Misc_Overlay */
    bool_bf gcmarkbit : 1;
    unsigned spacer : 15;
    struct Lisp_Overlay *next;
    Lisp_Object start;
    Lisp_Object end;
    Lisp_Object plist;
  };

/* Types of data which may be saved in a Lisp_Save_Value.  */

enum
  {
    SAVE_UNUSED,
    SAVE_INTEGER,
    SAVE_FUNCPOINTER,
    SAVE_POINTER,
    SAVE_OBJECT
  };

/* Number of bits needed to store one of the above values.  */
enum { SAVE_SLOT_BITS = 3 };

/* Number of slots in a save value where save_type is nonzero.  */
enum { SAVE_VALUE_SLOTS = 4 };

/* Bit-width and values for struct Lisp_Save_Value's save_type member.  */

enum { SAVE_TYPE_BITS = (SAVE_VALUE_SLOTS * SAVE_SLOT_BITS) + 1 };

enum Lisp_Save_Type
  {
    SAVE_TYPE_UNUSED = SAVE_UNUSED,
    SAVE_TYPE_INT_INT = SAVE_INTEGER + (SAVE_INTEGER << SAVE_SLOT_BITS),
    SAVE_TYPE_INT_INT_INT
      = (SAVE_INTEGER + (SAVE_TYPE_INT_INT << SAVE_SLOT_BITS)),
    SAVE_TYPE_OBJ_OBJ = SAVE_OBJECT + (SAVE_OBJECT << SAVE_SLOT_BITS),
    SAVE_TYPE_OBJ_OBJ_OBJ = SAVE_OBJECT + (SAVE_TYPE_OBJ_OBJ << SAVE_SLOT_BITS),
    SAVE_TYPE_OBJ_OBJ_OBJ_OBJ
      = SAVE_OBJECT + (SAVE_TYPE_OBJ_OBJ_OBJ << SAVE_SLOT_BITS),
    SAVE_TYPE_PTR_INT = SAVE_POINTER + (SAVE_INTEGER << SAVE_SLOT_BITS),
    SAVE_TYPE_PTR_OBJ = SAVE_POINTER + (SAVE_OBJECT << SAVE_SLOT_BITS),
    SAVE_TYPE_PTR_PTR = SAVE_POINTER + (SAVE_POINTER << SAVE_SLOT_BITS),
    SAVE_TYPE_FUNCPTR_PTR_OBJ
      = SAVE_FUNCPOINTER + (SAVE_TYPE_PTR_OBJ << SAVE_SLOT_BITS),

    /* This has an extra bit indicating it is raw memory: */
    SAVE_TYPE_MEMORY = SAVE_TYPE_PTR_INT + (1 << (SAVE_TYPE_BITS - 1))
  };

/* Special object used to hold a different values for later use.

   This is mostly used to package C integers and pointers to call
   record_unwind_protect when two or more values need to be saved.
   For example:

   ...
     struct my_data *md = get_my_data ();
     ptrdiff_t mi = get_my_integer ();
     record_unwind_protect (my_unwind, make_save_ptr_int (md, mi));
   ...

   Lisp_Object my_unwind (Lisp_Object arg)
   {
     struct my_data *md = XSAVE_POINTER (arg, 0);
     ptrdiff_t mi = XSAVE_INTEGER (arg, 1);
     ...
   }

   If ENABLE_CHECKING is in effect, XSAVE_xxx macros do type checking of the
   saved objects and raise eassert if type of the saved object doesn't match
   the type which is extracted.  In the example above, XSAVE_INTEGER (arg, 2)
   and XSAVE_OBJECT (arg, 0) are wrong because nothing was saved in slot 2 and
   slot 0 is a pointer.  */

typedef void (*voidfuncptr) (void);

struct Lisp_Save_Value
  {
    ENUM_BF (Lisp_Misc_Type) type : 16;	/* = Lisp_Misc_Save_Value */
    bool_bf gcmarkbit : 1;
    unsigned spacer : 32 - (16 + 1 + SAVE_TYPE_BITS);

    /* V->data may hold up to SAVE_VALUE_SLOTS entries.  The type of
       V's data entries are determined by V->save_type.  E.g., if
       V->save_type == SAVE_TYPE_PTR_OBJ, V->data[0] is a pointer,
       V->data[1] is an integer, and V's other data entries are unused.

       If V->save_type == SAVE_TYPE_MEMORY, V->data[0].pointer is the address of
       a memory area containing V->data[1].integer potential Lisp_Objects.  */
    ENUM_BF (Lisp_Save_Type) save_type : SAVE_TYPE_BITS;
    union {
      void *pointer;
      voidfuncptr funcpointer;
      ptrdiff_t integer;
      Lisp_Object object;
    } data[SAVE_VALUE_SLOTS];
  };

/* Return the type of V's Nth saved value.  */
INLINE int
save_type (struct Lisp_Save_Value *v, int n)
{
  eassert (0 <= n && n < SAVE_VALUE_SLOTS);
  return (v->save_type >> (SAVE_SLOT_BITS * n) & ((1 << SAVE_SLOT_BITS) - 1));
}

/* Get and set the Nth saved pointer.  */

INLINE void *
XSAVE_POINTER (Lisp_Object obj, int n)
{
  eassert (save_type (XSAVE_VALUE (obj), n) == SAVE_POINTER);
  return XSAVE_VALUE (obj)->data[n].pointer;
}
INLINE void
set_save_pointer (Lisp_Object obj, int n, void *val)
{
  eassert (save_type (XSAVE_VALUE (obj), n) == SAVE_POINTER);
  XSAVE_VALUE (obj)->data[n].pointer = val;
}
INLINE voidfuncptr
XSAVE_FUNCPOINTER (Lisp_Object obj, int n)
{
  eassert (save_type (XSAVE_VALUE (obj), n) == SAVE_FUNCPOINTER);
  return XSAVE_VALUE (obj)->data[n].funcpointer;
}

/* Likewise for the saved integer.  */

INLINE ptrdiff_t
XSAVE_INTEGER (Lisp_Object obj, int n)
{
  eassert (save_type (XSAVE_VALUE (obj), n) == SAVE_INTEGER);
  return XSAVE_VALUE (obj)->data[n].integer;
}
INLINE void
set_save_integer (Lisp_Object obj, int n, ptrdiff_t val)
{
  eassert (save_type (XSAVE_VALUE (obj), n) == SAVE_INTEGER);
  XSAVE_VALUE (obj)->data[n].integer = val;
}

/* Extract Nth saved object.  */

INLINE Lisp_Object
XSAVE_OBJECT (Lisp_Object obj, int n)
{
  eassert (save_type (XSAVE_VALUE (obj), n) == SAVE_OBJECT);
  return XSAVE_VALUE (obj)->data[n].object;
}

/* A miscellaneous object, when it's on the free list.  */
struct Lisp_Free
  {
    ENUM_BF (Lisp_Misc_Type) type : 16;	/* = Lisp_Misc_Free */
    bool_bf gcmarkbit : 1;
    unsigned spacer : 15;
    union Lisp_Misc *chain;
  };

/* To get the type field of a union Lisp_Misc, use XMISCTYPE.
   It uses one of these struct subtypes to get the type field.  */

union Lisp_Misc
  {
    struct Lisp_Misc_Any u_any;	   /* Supertype of all Misc types.  */
    struct Lisp_Free u_free;
    struct Lisp_Marker u_marker;
    struct Lisp_Overlay u_overlay;
    struct Lisp_Save_Value u_save_value;
  };

INLINE union Lisp_Misc *
XMISC(Lisp_Object a)
{
  return XUNTAG(a, Lisp_Misc);
}

INLINE struct Lisp_Misc_Any *
XMISCANY(Lisp_Object a)
{
  eassert(MISCP(a));
  return &XMISC(a)->u_any;
}

INLINE enum Lisp_Misc_Type
XMISCTYPE(Lisp_Object a)
{
  return XMISCANY(a)->type;
}

INLINE struct Lisp_Marker *
XMARKER(Lisp_Object a)
{
  eassert(MARKERP(a));
  return &XMISC(a)->u_marker;
}

INLINE struct Lisp_Overlay *
XOVERLAY(Lisp_Object a)
{
  eassert(OVERLAYP(a));
  return &XMISC(a)->u_overlay;
}

INLINE struct Lisp_Save_Value *
XSAVE_VALUE(Lisp_Object a)
{
  eassert(SAVE_VALUEP(a));
  return &XMISC(a)->u_save_value;
}

/* Forwarding pointer to an int variable.
   This is allowed only in the value cell of a symbol,
   and it means that the symbol's value really lives in the
   specified int variable.  */
struct Lisp_Intfwd
  {
    enum Lisp_Fwd_Type type;	/* = Lisp_Fwd_Int */
    EMACS_INT *intvar;
  };

/* Boolean forwarding pointer to an int variable.
   This is like Lisp_Intfwd except that the ostensible
   "value" of the symbol is t if the bool variable is true,
   nil if it is false.  */
struct Lisp_Boolfwd
  {
    enum Lisp_Fwd_Type type;	/* = Lisp_Fwd_Bool */
    bool *boolvar;
  };

/* Forwarding pointer to a Lisp_Object variable.
   This is allowed only in the value cell of a symbol,
   and it means that the symbol's value really lives in the
   specified variable.  */
struct Lisp_Objfwd
  {
    enum Lisp_Fwd_Type type;	/* = Lisp_Fwd_Obj */
    Lisp_Object *objvar;
  };

/* Like Lisp_Objfwd except that value lives in a slot in the
   current buffer.  Value is byte index of slot within buffer.  */
struct Lisp_Buffer_Objfwd
  {
    enum Lisp_Fwd_Type type;	/* = Lisp_Fwd_Buffer_Obj */
    int offset;
    /* One of Qnil, Qintegerp, Qsymbolp, Qstringp, Qfloatp or Qnumberp.  */
    Lisp_Object predicate;
  };

/* struct Lisp_Buffer_Local_Value is used in a symbol value cell when
   the symbol has buffer-local or frame-local bindings.  (Exception:
   some buffer-local variables are built-in, with their values stored
   in the buffer structure itself.  They are handled differently,
   using struct Lisp_Buffer_Objfwd.)

   The `realvalue' slot holds the variable's current value, or a
   forwarding pointer to where that value is kept.  This value is the
   one that corresponds to the loaded binding.  To read or set the
   variable, you must first make sure the right binding is loaded;
   then you can access the value in (or through) `realvalue'.

   `buffer' and `frame' are the buffer and frame for which the loaded
   binding was found.  If those have changed, to make sure the right
   binding is loaded it is necessary to find which binding goes with
   the current buffer and selected frame, then load it.  To load it,
   first unload the previous binding, then copy the value of the new
   binding into `realvalue' (or through it).  Also update
   LOADED-BINDING to point to the newly loaded binding.

   `local_if_set' indicates that merely setting the variable creates a
   local binding for the current buffer.  Otherwise the latter, setting
   the variable does not do that; only make-local-variable does that.  */

struct Lisp_Buffer_Local_Value
  {
    /* True means that merely setting the variable creates a local
       binding for the current buffer.  */
    bool_bf local_if_set : 1;
    /* True means this variable can have frame-local bindings, otherwise, it is
       can have buffer-local bindings.  The two cannot be combined.  */
    bool_bf frame_local : 1;
    /* True means that the binding now loaded was found.
       Presumably equivalent to (defcell!=valcell).  */
    bool_bf found : 1;
    /* If non-NULL, a forwarding to the C var where it should also be set.  */
    union Lisp_Fwd *fwd;	/* Should never be (Buffer|Kboard)_Objfwd.  */
    /* The buffer or frame for which the loaded binding was found.  */
    Lisp_Object where;
    /* A cons cell that holds the default value.  It has the form
       (SYMBOL . DEFAULT-VALUE).  */
    Lisp_Object defcell;
    /* The cons cell from `where's parameter alist.
       It always has the form (SYMBOL . VALUE)
       Note that if `forward' is non-nil, VALUE may be out of date.
       Also if the currently loaded binding is the default binding, then
       this is `eq'ual to defcell.  */
    Lisp_Object valcell;
  };

/* Like Lisp_Objfwd except that value lives in a slot in the
   current kboard.  */
struct Lisp_Kboard_Objfwd
  {
    enum Lisp_Fwd_Type type;	/* = Lisp_Fwd_Kboard_Obj */
    int offset;
  };

union Lisp_Fwd
  {
    struct Lisp_Intfwd u_intfwd;
    struct Lisp_Boolfwd u_boolfwd;
    struct Lisp_Objfwd u_objfwd;
    struct Lisp_Buffer_Objfwd u_buffer_objfwd;
    struct Lisp_Kboard_Objfwd u_kboard_objfwd;
  };

INLINE enum Lisp_Fwd_Type
XFWDTYPE (union Lisp_Fwd *a)
{
  return a->u_intfwd.type;
}

INLINE struct Lisp_Buffer_Objfwd *
XBUFFER_OBJFWD (union Lisp_Fwd *a)
{
  eassert (BUFFER_OBJFWDP (a));
  return &a->u_buffer_objfwd;
}

/* Lisp floating point type.  */
struct Lisp_Float
  {
    union
    {
      double data;
      struct Lisp_Float *chain;
    } u;
  };

INLINE double
XFLOAT_DATA(Lisp_Object f)
{
  struct Lisp_Float *lf = XFLOAT(f);
  eassert(lf != NULL);
  return lf->u.data;
}

/* Most hosts nowadays use IEEE floating point, so they use IEC 60559
   representations, have infinities and NaNs, and do not trap on
   exceptions.  Define IEEE_FLOATING_POINT if this host is one of the
   typical ones.  The C11 macro __STDC_IEC_559__ is close to what is
   wanted here, but is not quite right because Emacs does not require
   all the features of C11 Annex F (and does not require C11 at all,
   for that matter).  */
enum
  {
    IEEE_FLOATING_POINT
      = (FLT_RADIX == 2 && FLT_MANT_DIG == 24
	 && FLT_MIN_EXP == -125 && FLT_MAX_EXP == 128)
  };

/* A character, declared with the following typedef, is a member
   of some character set associated with the current buffer.  */
#ifndef _UCHAR_T  /* Protect against something in ctab.h on AIX.  */
# define _UCHAR_T
typedef unsigned char UCHAR;
#endif /* !_UCHAR_T */

/* Meanings of slots in a Lisp_Compiled:  */

enum Lisp_Compiled
  {
    COMPILED_ARGLIST = 0,
    COMPILED_BYTECODE = 1,
    COMPILED_CONSTANTS = 2,
    COMPILED_STACK_DEPTH = 3,
    COMPILED_DOC_STRING = 4,
    COMPILED_INTERACTIVE = 5
  };

/* Flag bits in a character.  These also get used in termhooks.h.
   Richard Stallman <rms@gnu.ai.mit.edu> thinks that MULE
   (MUlti-Lingual Emacs) might need 22 bits for the character value
   itself, so we probably shouldn't use any bits lower than 0x0400000.  */
enum char_bits
  {
    CHAR_ALT = 0x0400000,
    CHAR_SUPER = 0x0800000,
    CHAR_HYPER = 0x1000000,
    CHAR_SHIFT = 0x2000000,
    CHAR_CTL = 0x4000000,
    CHAR_META = 0x8000000,

    CHAR_MODIFIER_MASK =
      CHAR_ALT | CHAR_SUPER | CHAR_HYPER | CHAR_SHIFT | CHAR_CTL | CHAR_META,

    /* Actually, the current Emacs uses 22 bits for the character value
       itself.  */
    CHARACTERBITS = 22
  };

/* Data type checking.  */

LISP_MACRO_DEFUN (NILP, bool, (Lisp_Object x), (x))

INLINE bool
NUMBERP (Lisp_Object x)
{
  return INTEGERP (x) || FLOATP (x);
}
INLINE bool
NATNUMP (Lisp_Object x)
{
  return INTEGERP (x) && 0 <= XINT (x);
}

INLINE bool
RANGED_INTEGERP (intmax_t lo, Lisp_Object x, intmax_t hi)
{
  return INTEGERP (x) && lo <= XINT (x) && XINT (x) <= hi;
}

#define TYPE_RANGED_INTEGERP(type, x) \
  (INTEGERP (x)			      \
   && (TYPE_SIGNED (type) ? TYPE_MINIMUM (type) <= XINT (x) : 0 <= XINT (x)) \
   && XINT (x) <= TYPE_MAXIMUM (type))

LISP_MACRO_DEFUN (CONSP, bool, (Lisp_Object x), (x))
LISP_MACRO_DEFUN (FLOATP, bool, (Lisp_Object x), (x))
LISP_MACRO_DEFUN (MISCP, bool, (Lisp_Object x), (x))
LISP_MACRO_DEFUN (SYMBOLP, bool, (Lisp_Object x), (x))
LISP_MACRO_DEFUN (INTEGERP, bool, (Lisp_Object x), (x))
LISP_MACRO_DEFUN (VECTORLIKEP, bool, (Lisp_Object x), (x))
LISP_MACRO_DEFUN (MARKERP, bool, (Lisp_Object x), (x))

INLINE bool
STRINGP (Lisp_Object x)
{
  return XTYPE (x) == Lisp_String;
}
INLINE bool
VECTORP (Lisp_Object x)
{
  return VECTORLIKEP (x) && ! (ASIZE (x) & PSEUDOVECTOR_FLAG);
}
INLINE bool
OVERLAYP (Lisp_Object x)
{
  return MISCP (x) && XMISCTYPE (x) == Lisp_Misc_Overlay;
}
INLINE bool
SAVE_VALUEP (Lisp_Object x)
{
  return MISCP (x) && XMISCTYPE (x) == Lisp_Misc_Save_Value;
}

INLINE bool
AUTOLOADP (Lisp_Object x)
{
  return CONSP (x) && EQ (Qautoload, XCAR (x));
}

INLINE bool
BUFFER_OBJFWDP (union Lisp_Fwd *a)
{
  return XFWDTYPE (a) == Lisp_Fwd_Buffer_Obj;
}

INLINE bool
PSEUDOVECTOR_TYPEP(struct vectorlike_header *a, int code)
{
  eassert(a != NULL);
  return ((a->size & (PSEUDOVECTOR_FLAG | PVEC_TYPE_MASK))
	  == (PSEUDOVECTOR_FLAG | (code << PSEUDOVECTOR_AREA_BITS)));
}

/* True if A is a pseudovector whose code is CODE.  */
INLINE bool
PSEUDOVECTORP (Lisp_Object a, int code)
{
  if (! VECTORLIKEP (a))
    return false;
  else
    {
      /* Converting to struct vectorlike_header * avoids aliasing issues.  */
      struct vectorlike_header *h = XUNTAG (a, Lisp_Vectorlike);
      return PSEUDOVECTOR_TYPEP (h, code);
    }
}


/* Test for specific pseudovector types.  */

INLINE bool
WINDOW_CONFIGURATIONP (Lisp_Object a)
{
  return PSEUDOVECTORP (a, PVEC_WINDOW_CONFIGURATION);
}

INLINE bool
PROCESSP (Lisp_Object a)
{
  return PSEUDOVECTORP (a, PVEC_PROCESS);
}

INLINE bool
WINDOWP (Lisp_Object a)
{
  return PSEUDOVECTORP (a, PVEC_WINDOW);
}

INLINE bool
TERMINALP (Lisp_Object a)
{
  return PSEUDOVECTORP (a, PVEC_TERMINAL);
}

INLINE bool
SUBRP (Lisp_Object a)
{
  return PSEUDOVECTORP (a, PVEC_SUBR);
}

INLINE bool
COMPILEDP (Lisp_Object a)
{
  return PSEUDOVECTORP (a, PVEC_COMPILED);
}

INLINE bool
BUFFERP (Lisp_Object a)
{
  return PSEUDOVECTORP (a, PVEC_BUFFER);
}

INLINE bool
CHAR_TABLE_P (Lisp_Object a)
{
  return PSEUDOVECTORP (a, PVEC_CHAR_TABLE);
}

INLINE bool
SUB_CHAR_TABLE_P (Lisp_Object a)
{
  return PSEUDOVECTORP (a, PVEC_SUB_CHAR_TABLE);
}

INLINE bool
BOOL_VECTOR_P (Lisp_Object a)
{
  return PSEUDOVECTORP (a, PVEC_BOOL_VECTOR);
}

INLINE bool
FRAMEP (Lisp_Object a)
{
  return PSEUDOVECTORP (a, PVEC_FRAME);
}

/* Test for image (image . spec)  */
INLINE bool
IMAGEP (Lisp_Object x)
{
  return CONSP (x) && EQ (XCAR (x), Qimage);
}

/* Array types.  */
INLINE bool
ARRAYP (Lisp_Object x)
{
  return VECTORP (x) || STRINGP (x) || CHAR_TABLE_P (x) || BOOL_VECTOR_P (x);
}

INLINE void
CHECK_LIST (Lisp_Object x)
{
  CHECK_TYPE (CONSP (x) || NILP (x), Qlistp, x);
}

LISP_MACRO_DEFUN_VOID (CHECK_LIST_CONS, (Lisp_Object x, Lisp_Object y), (x, y))
LISP_MACRO_DEFUN_VOID (CHECK_SYMBOL, (Lisp_Object x), (x))
LISP_MACRO_DEFUN_VOID (CHECK_NUMBER, (Lisp_Object x), (x))

INLINE void
CHECK_STRING (Lisp_Object x)
{
  CHECK_TYPE (STRINGP (x), Qstringp, x);
}
INLINE void
CHECK_STRING_CAR (Lisp_Object x)
{
  CHECK_TYPE (STRINGP (XCAR (x)), Qstringp, XCAR (x));
}
INLINE void
CHECK_CONS (Lisp_Object x)
{
  CHECK_TYPE (CONSP (x), Qconsp, x);
}
INLINE void
CHECK_VECTOR (Lisp_Object x)
{
  CHECK_TYPE (VECTORP (x), Qvectorp, x);
}
INLINE void
CHECK_BOOL_VECTOR (Lisp_Object x)
{
  CHECK_TYPE (BOOL_VECTOR_P (x), Qbool_vector_p, x);
}
INLINE void
CHECK_VECTOR_OR_STRING (Lisp_Object x)
{
  CHECK_TYPE (VECTORP (x) || STRINGP (x), Qarrayp, x);
}
INLINE void
CHECK_ARRAY (Lisp_Object x, Lisp_Object Qxxxp)
{
  CHECK_TYPE (ARRAYP (x), Qxxxp, x);
}
INLINE void
CHECK_BUFFER (Lisp_Object x)
{
  CHECK_TYPE (BUFFERP (x), Qbufferp, x);
}
INLINE void
CHECK_WINDOW (Lisp_Object x)
{
  CHECK_TYPE (WINDOWP (x), Qwindowp, x);
}
INLINE void
CHECK_PROCESS (Lisp_Object x)
{
  CHECK_TYPE (PROCESSP (x), Qprocessp, x);
}
INLINE void
CHECK_NATNUM (Lisp_Object x)
{
  CHECK_TYPE (NATNUMP (x), Qwholenump, x);
}

#define CHECK_RANGED_INTEGER(x, lo, hi)					\
  do {									\
    CHECK_NUMBER(x);							\
    if (!(((lo) <= XINT(x)) && (XINT(x) <= (hi))))  			\
      args_out_of_range_3						\
	(x,								\
	 make_number(((lo) < 0) && ((lo) < MOST_NEGATIVE_FIXNUM)	\
		     ? MOST_NEGATIVE_FIXNUM				\
		     : (lo)),						\
	 make_number(min(hi, MOST_POSITIVE_FIXNUM)));			\
  } while (false)
#define CHECK_TYPE_RANGED_INTEGER(type, x) \
  do {									\
    if (TYPE_SIGNED(type))						\
      CHECK_RANGED_INTEGER(x, TYPE_MINIMUM(type), TYPE_MAXIMUM(type));  \
    else								\
      CHECK_RANGED_INTEGER(x, 0, TYPE_MAXIMUM(type));			\
  } while (false)

#define CHECK_NUMBER_COERCE_MARKER(x)					\
  do {									\
    if (MARKERP((x)))							\
      XSETFASTINT(x, marker_position(x));				\
    else								\
      CHECK_TYPE(INTEGERP(x), Qinteger_or_marker_p, x); 		\
  } while (false)

INLINE double
XFLOATINT(Lisp_Object n)
{
  return extract_float(n);
}

INLINE void
CHECK_NUMBER_OR_FLOAT(Lisp_Object x)
{
  CHECK_TYPE(FLOATP(x) || INTEGERP(x), Qnumberp, x);
}

#define CHECK_NUMBER_OR_FLOAT_COERCE_MARKER(x)				\
  do {									\
    if (MARKERP(x))							\
      XSETFASTINT(x, marker_position(x));				\
    else								\
      CHECK_TYPE(INTEGERP(x) || FLOATP(x), Qnumber_or_marker_p, x);	\
  } while (false)

/* Since we can't assign directly to the CAR or CDR fields of a cons
   cell, use these when checking that those fields contain numbers.  */
INLINE void
CHECK_NUMBER_CAR(Lisp_Object x)
{
  Lisp_Object tmp = XCAR(x);
  CHECK_NUMBER(tmp);
  XSETCAR(x, tmp);
}

INLINE void
CHECK_NUMBER_CDR(Lisp_Object x)
{
  Lisp_Object tmp = XCDR(x);
  CHECK_NUMBER(tmp);
  XSETCDR(x, tmp);
}

/* Define a built-in function for calling from Lisp.
 `lname' should be the name to give the function in Lisp,
    as a null-terminated C string.
 `fnname' should be the name of the function in C.
    By convention, it starts with F.
 `sname' should be the name for the C constant structure
    that records information on this function for internal use.
    By convention, it should be the same as `fnname' but with S instead of F.
    It's too bad that C macros can't compute this from `fnname'.
 `minargs' should be a number, the minimum number of arguments allowed.
 `maxargs' should be a number, the maximum number of arguments allowed,
    or else MANY or UNEVALLED.
    MANY means pass a vector of evaluated arguments,
	 in the form of an integer number-of-arguments
	 followed by the address of a vector of Lisp_Objects
	 which contains the argument values.
    UNEVALLED means pass the list of unevaluated arguments
 `intspec' says how interactive arguments are to be fetched.
    If the string starts with a `(', `intspec' is evaluated and the resulting
    list is the list of arguments.
    If it's a string that doesn't start with `(', the value should follow
    the one of the doc string for `interactive'.
    A null string means call interactively with no arguments.
 `doc' is documentation for the user.  */

/* This version of DEFUN declares a function prototype with the right
   arguments, so we can catch errors with maxargs at compile-time.  */
#ifdef _MSC_VER
#define DEFUN(lname, fnname, sname, minargs, maxargs, intspec, doc)	\
   Lisp_Object fnname DEFUN_ARGS_ ## maxargs ;				\
   static struct Lisp_Subr alignas (GCALIGNMENT) sname =		\
   { { (PVEC_SUBR << PSEUDOVECTOR_AREA_BITS)				\
       | (sizeof (struct Lisp_Subr) / sizeof (EMACS_INT)) },		\
      { (Lisp_Object (__cdecl *)(void))fnname },                        \
       minargs, maxargs, lname, intspec, 0};				\
   Lisp_Object fnname
#else  /* not _MSC_VER */
# if __STDC_VERSION__ < 199901
#  define DEFUN_FUNCTION_INIT(fnname, maxargs) (Lisp_Object (*) (void)) fnname
# else
#  define DEFUN_FUNCTION_INIT(fnname, maxargs) .a ## maxargs = fnname
# endif
#define DEFUN(lname, fnname, sname, minargs, maxargs, intspec, doc)	\
   Lisp_Object fnname DEFUN_ARGS_ ## maxargs ;				\
   static struct Lisp_Subr alignas (GCALIGNMENT) sname =		\
     { { PVEC_SUBR << PSEUDOVECTOR_AREA_BITS },				\
       { DEFUN_FUNCTION_INIT (fnname, maxargs) },			\
       minargs, maxargs, lname, intspec, 0};				\
   Lisp_Object fnname
#endif

/* Note that the weird token-substitution semantics of ANSI C makes
   this work for MANY and UNEVALLED.  */
#define DEFUN_ARGS_MANY		(ptrdiff_t, Lisp_Object *)
#define DEFUN_ARGS_UNEVALLED	(Lisp_Object)
#define DEFUN_ARGS_0	(void)
#define DEFUN_ARGS_1	(Lisp_Object)
#define DEFUN_ARGS_2	(Lisp_Object, Lisp_Object)
#define DEFUN_ARGS_3	(Lisp_Object, Lisp_Object, Lisp_Object)
#define DEFUN_ARGS_4	(Lisp_Object, Lisp_Object, Lisp_Object, Lisp_Object)
#define DEFUN_ARGS_5	(Lisp_Object, Lisp_Object, Lisp_Object, Lisp_Object, \
			 Lisp_Object)
#define DEFUN_ARGS_6	(Lisp_Object, Lisp_Object, Lisp_Object, Lisp_Object, \
			 Lisp_Object, Lisp_Object)
#define DEFUN_ARGS_7	(Lisp_Object, Lisp_Object, Lisp_Object, Lisp_Object, \
			 Lisp_Object, Lisp_Object, Lisp_Object)
#define DEFUN_ARGS_8	(Lisp_Object, Lisp_Object, Lisp_Object, Lisp_Object, \
			 Lisp_Object, Lisp_Object, Lisp_Object, Lisp_Object)

/* True if OBJ is a Lisp function.  */
INLINE bool
FUNCTIONP (Lisp_Object obj)
{
  return functionp (obj);
}

/* defsubr (Sname);
   is how we define the symbol for function `name' at start-up time.  */
extern void defsubr (struct Lisp_Subr *);

enum maxargs
  {
    MANY = -2,
    UNEVALLED = -1
  };

extern void defvar_lisp (struct Lisp_Objfwd *, const char *, Lisp_Object *);
extern void defvar_lisp_nopro (struct Lisp_Objfwd *, const char *, Lisp_Object *);
extern void defvar_bool (struct Lisp_Boolfwd *, const char *, bool *);
extern void defvar_int (struct Lisp_Intfwd *, const char *, EMACS_INT *);
#ifndef PROTOS_IN_EMACS_BUFFER_C
# ifdef KEEP_EMACS_BUFFER_C_STUFF_LOCAL
static
# else
extern
# endif /* KEEP_EMACS_BUFFER_C_STUFF_LOCAL */
void defvar_per_buffer (const char *, Lisp_Object *, Lisp_Object, char *);
#else
# if defined(KEEP_EMACS_BUFFER_C_STUFF_LOCAL) && defined(lint)
#  undef KEEP_EMACS_BUFFER_C_STUFF_LOCAL
# endif /* KEEP_EMACS_BUFFER_C_STUFF_LOCAL && lint */
#endif /* !PROTOS_IN_EMACS_BUFFER_C */
extern void defvar_kboard (struct Lisp_Kboard_Objfwd *, const char *, int);

/* Macros we use to define forwarded Lisp variables.
   These are used in the syms_of_FILENAME functions.

   An ordinary (not in buffer_defaults, per-buffer, or per-keyboard)
   lisp variable is actually a field in `struct emacs_globals'.  The
   field's name begins with "f_", which is a convention enforced by
   these macros.  Each such global has a corresponding #define in
   globals.h; the plain name should be used in the code.

   E.g., the global "cons_cells_consed" is declared as "int
   f_cons_cells_consed" in globals.h, but there is a define:

      #define cons_cells_consed globals.f_cons_cells_consed

   All C code uses the `cons_cells_consed' name.  This is all done
   this way to support indirection for multi-threaded Emacs.  */

#define DEFVAR_LISP(lname, vname, doc)		\
  do {						\
    static struct Lisp_Objfwd o_fwd;		\
    defvar_lisp (&o_fwd, lname, &globals.f_ ## vname);		\
  } while (false)
#define DEFVAR_LISP_NOPRO(lname, vname, doc)	\
  do {						\
    static struct Lisp_Objfwd o_fwd;		\
    defvar_lisp_nopro (&o_fwd, lname, &globals.f_ ## vname);	\
  } while (false)
#define DEFVAR_BOOL(lname, vname, doc)		\
  do {						\
    static struct Lisp_Boolfwd b_fwd;		\
    defvar_bool (&b_fwd, lname, &globals.f_ ## vname);		\
  } while (false)
#define DEFVAR_INT(lname, vname, doc)		\
  do {						\
    static struct Lisp_Intfwd i_fwd;		\
    defvar_int (&i_fwd, lname, &globals.f_ ## vname);		\
  } while (false)

/* TYPE is nil for a general Lisp variable.
 * An integer specifies a type; then only Lisp values
 * with that type code are allowed (except that nil is allowed too).
 * LNAME is the Lisp-level variable name.
 * VNAME is the name of the buffer slot.
 * DOC is a dummy where you write the doc string as a comment.  */
#ifndef DEFVAR_PER_BUFFER
# define DEFVAR_PER_BUFFER(lname, vname, type, doc)  \
  defvar_per_buffer(lname, vname, type, 0)
#endif /* !DEFVAR_PER_BUFFER */

#define DEFVAR_BUFFER_DEFAULTS(lname, vname, doc)		\
  do {								\
    static struct Lisp_Objfwd o_fwd;				\
    defvar_lisp_nopro (&o_fwd, lname, &BVAR (&buffer_defaults, vname));	\
  } while (false)

#define DEFVAR_KBOARD(lname, vname, doc)			\
  do {								\
    static struct Lisp_Kboard_Objfwd ko_fwd;			\
    defvar_kboard (&ko_fwd, lname, offsetof (KBOARD, vname ## _)); \
  } while (false)

/* Save and restore the instruction and environment pointers,
   without affecting the signal mask.  */

#ifdef HAVE__SETJMP
typedef jmp_buf sys_jmp_buf;
# define sys_setjmp(j) _setjmp (j)
# define sys_longjmp(j, v) _longjmp (j, v)
#elif defined HAVE_SIGSETJMP
typedef sigjmp_buf sys_jmp_buf;
# define sys_setjmp(j) sigsetjmp (j, 0)
# define sys_longjmp(j, v) siglongjmp (j, v)
#else
/* A platform that uses neither _longjmp nor siglongjmp; assume
   longjmp does not affect the sigmask.  */
typedef jmp_buf sys_jmp_buf;
# define sys_setjmp(j) setjmp (j)
# define sys_longjmp(j, v) longjmp (j, v)
#endif


/* Elisp uses several stacks:
   - the C stack.
   - the bytecode stack: used internally by the bytecode interpreter.
     Allocated from the C stack.
   - The specpdl stack: keeps track of active unwind-protect and
     dynamic-let-bindings.  Allocated from the `specpdl' array, a manually
     managed stack.
   - The handler stack: keeps track of active catch tags and condition-case
     handlers.  Allocated in a manually managed stack implemented by a
     doubly-linked list allocated via xmalloc and never freed.  */

/* Structure for recording Lisp call stack for backtrace purposes.  */

/* The special binding stack holds the outer values of variables while
   they are bound by a function application or a let form, stores the
   code to be executed for unwind-protect forms.

   NOTE: The specbinding union is defined here, because SPECPDL_INDEX is
   used all over the place, needs to be fast, and needs to know the size of
   union specbinding.  But only eval.c should access it.  */

enum specbind_tag {
  SPECPDL_UNWIND,		/* An unwind_protect function on Lisp_Object.  */
  SPECPDL_UNWIND_PTR,		/* Likewise, on void *.  */
  SPECPDL_UNWIND_INT,		/* Likewise, on int.  */
  SPECPDL_UNWIND_VOID,		/* Likewise, with no arg.  */
  SPECPDL_BACKTRACE,		/* An element of the backtrace.  */
  SPECPDL_LET,			/* A plain and simple dynamic let-binding.  */
  /* Tags greater than SPECPDL_LET must be "subkinds" of LET.  */
  SPECPDL_LET_LOCAL,		/* A buffer-local let-binding.  */
  SPECPDL_LET_DEFAULT		/* A global binding for a localized var.  */
};

union specbinding
  {
    ENUM_BF (specbind_tag) kind : CHAR_BIT;
    struct {
      ENUM_BF (specbind_tag) kind : CHAR_BIT;
      void (*func) (Lisp_Object);
      Lisp_Object arg;
    } unwind;
    struct {
      ENUM_BF (specbind_tag) kind : CHAR_BIT;
      void (*func) (void *);
      void *arg;
    } unwind_ptr;
    struct {
      ENUM_BF (specbind_tag) kind : CHAR_BIT;
      void (*func) (int);
      int arg;
    } unwind_int;
    struct {
      ENUM_BF (specbind_tag) kind : CHAR_BIT;
      void (*func) (void);
    } unwind_void;
    struct {
      ENUM_BF (specbind_tag) kind : CHAR_BIT;
      /* `where' is not used in the case of SPECPDL_LET.  */
      Lisp_Object symbol, old_value, where;
    } let;
    struct {
      ENUM_BF (specbind_tag) kind : CHAR_BIT;
      bool_bf debug_on_exit : 1;
      Lisp_Object function;
      Lisp_Object *args;
      ptrdiff_t nargs;
    } bt;
  };

extern union specbinding *specpdl;
extern union specbinding *specpdl_ptr;
extern ptrdiff_t specpdl_size;

INLINE ptrdiff_t
SPECPDL_INDEX (void)
{
  return specpdl_ptr - specpdl;
}

/* This structure helps implement the `catch/throw' and `condition-case/signal'
   control structures.  A struct handler contains all the information needed to
   restore the state of the interpreter after a non-local jump.

   handler structures are chained together in a doubly linked list; the `next'
   member points to the next outer catchtag and the `nextfree' member points in
   the other direction to the next inner element (which is typically the next
   free element since we mostly use it on the deepest handler).

   A call like (throw TAG VAL) searches for a catchtag whose `tag_or_ch'
   member is TAG, and then unbinds to it.  The `val' member is used to
   hold VAL while the stack is unwound; `val' is returned as the value
   of the catch form.

   All the other members are concerned with restoring the interpreter
   state.

   Members are volatile if their values need to survive _longjmp when
   a 'struct handler' is a local variable.  */

enum handlertype { CATCHER, CONDITION_CASE };

struct handler
{
  enum handlertype type;
  Lisp_Object tag_or_ch;
  Lisp_Object val;
  struct handler *next;
  struct handler *nextfree;

  /* The bytecode interpreter can have several handlers active at the same
     time, so when we longjmp to one of them, it needs to know which handler
     this was and what was the corresponding internal state.  This is stored
     here, and when we longjmp we make sure that handlerlist points to the
     proper handler.  */
  Lisp_Object *bytecode_top;
  int bytecode_dest;

  /* Most global vars are reset to their value via the specpdl mechanism,
     but a few others are handled by storing their value here.  */
#if 1 /* GC_MARK_STACK == GC_MAKE_GCPROS_NOOPS, but defined later.  */
  struct gcpro *gcpro;
#endif /* 1 */
  sys_jmp_buf jmp;
  EMACS_INT lisp_eval_depth;
  ptrdiff_t pdlcount;
  int poll_suppress_count;
  int interrupt_input_blocked;
  struct byte_stack *byte_stack;
};

/* Fill in the components of c, and put it on the list.  */
#define PUSH_HANDLER(c, tag_ch_val, handlertype)	\
  if (handlerlist->nextfree)				\
    (c) = handlerlist->nextfree;			\
  else							\
    {							\
      (c) = xmalloc (sizeof (struct handler));		\
      (c)->nextfree = NULL;				\
      handlerlist->nextfree = (c);			\
    }							\
  (c)->type = (handlertype);				\
  (c)->tag_or_ch = (tag_ch_val);			\
  (c)->val = Qnil;					\
  (c)->next = handlerlist;				\
  (c)->lisp_eval_depth = lisp_eval_depth;		\
  (c)->pdlcount = SPECPDL_INDEX ();			\
  (c)->poll_suppress_count = poll_suppress_count;	\
  (c)->interrupt_input_blocked = interrupt_input_blocked;\
  (c)->gcpro = gcprolist;				\
  (c)->byte_stack = byte_stack_list;			\
  handlerlist = (c);


extern Lisp_Object memory_signal_data;

/* An address near the bottom of the stack.
   Tells GC how to save a copy of the stack.  */
extern char *stack_bottom;

/* Check quit-flag and quit if it is non-nil.
   Typing C-g does not directly cause a quit; it only sets Vquit_flag.
   So the program needs to do QUIT at times when it is safe to quit.
   Every loop that might run for a long time or might not exit
   ought to do QUIT at least once, at a safe place.
   Unless that is impossible, of course.
   But it is very desirable to avoid creating loops where QUIT is impossible.

   Exception: if you set immediate_quit to true,
   then the handler that responds to the C-g does the quit itself.
   This is a good thing to do around a loop that has no side effects
   and (in particular) cannot call arbitrary Lisp code.

   If quit-flag is set to `kill-emacs' the SIGINT handler has received
   a request to exit Emacs when it is safe to do.  */

extern void process_pending_signals (void);
extern bool volatile pending_signals;

extern void process_quit_flag (void);
#define QUIT						\
  do {							\
    if (!NILP (Vquit_flag) && NILP (Vinhibit_quit))	\
      process_quit_flag ();				\
    else if (pending_signals)				\
      process_pending_signals ();			\
  } while (false)


/* True if ought to quit now: */
#define QUITP (!NILP (Vquit_flag) && NILP (Vinhibit_quit))

extern Lisp_Object Vascii_downcase_table;
extern Lisp_Object Vascii_canon_table;

/* Variables used locally in the following case handling macros: */
extern int case_temp1;
extern Lisp_Object case_temp2;

/* Current buffer's map from characters to lower-case characters: */
#define DOWNCASE_TABLE current_buffer->INTERNAL_FIELD(downcase_table)

/* Current buffer's map from characters to upper-case characters: */
#define UPCASE_TABLE current_buffer->INTERNAL_FIELD(upcase_table)

/* Downcase a character, or make no change if that cannot be done: */
#ifndef DOWNCASE
# define DOWNCASE(CH)						\
   ((case_temp1 = (CH),						\
     case_temp2 = CHAR_TABLE_REF(DOWNCASE_TABLE, case_temp1),	\
     NATNUMP(case_temp2))					\
    ? XFASTINT(case_temp2) : case_temp1)
#endif /* !DOWNCASE */

/* 1 if CH is upper case: */
#ifndef UPPERCASEP
# define UPPERCASEP(CH) (DOWNCASE(CH) != (CH))
#endif /* !UPPERCASEP */

/* 1 if CH is neither upper nor lower case: */
#ifndef NOCASEP
# define NOCASEP(CH) (UPCASE1(CH) == (CH))
#endif /* !NOCASEP */

/* 1 if CH is lower case: */
#ifndef LOWERCASEP
# define LOWERCASEP(CH) (!UPPERCASEP (CH) && !NOCASEP(CH))
#endif /* !LOWERCASEP */

/* Upcase a character, or make no change if that cannot be done: */
#ifndef UPCASE
# define UPCASE(CH) (!UPPERCASEP(CH) ? UPCASE1(CH) : (CH))
#endif /* !UPCASE */

/* Upcase a character known to be not upper case: */
#ifndef UPCASE1
# define UPCASE1(CH)						\
   ((case_temp1 = (CH),						\
     case_temp2 = CHAR_TABLE_REF(UPCASE_TABLE, case_temp1),	\
     NATNUMP(case_temp2))					\
    ? XFASTINT(case_temp2) : case_temp1)
#endif /* !UPCASE1 */

/* Structure for recording stack slots that need marking.  */

/* This is a chain of structures, each of which points at a Lisp_Object
   variable whose value should be marked in garbage collection.
   Normally every link of the chain is an automatic variable of a function,
   and its `val' points to some argument or local variable of the function.
   On exit to the function, the chain is set back to the value it had on entry.
   This way, no link remains in the chain when the stack frame containing the
   link disappears.

   Every function that can call Feval must protect in this fashion all
   Lisp_Object variables whose contents will be used again.  */

extern struct gcpro *gcprolist;

struct gcpro
{
  struct gcpro *next;

  /* Address of first protected variable.  */
  volatile Lisp_Object *var;

  /* Number of consecutive protected variables.  */
  ptrdiff_t nvars;

#ifdef DEBUG_GCPRO
  int level;
#endif /* DEBUG_GCPRO */
};

/* Values of GC_MARK_STACK during compilation:

   0	Use GCPRO as before
   1	Do the real thing, make GCPROs and UNGCPRO no-ops.
   2    Mark the stack, and check that everything GCPRO'd is
	marked.
   3	Mark using GCPRO's, mark stack last, and count how many
	dead objects are kept alive.

   Formerly, method 0 was used.  Currently, method 1 is used unless
   otherwise specified by hand when building, e.g.,
   "make CPPFLAGS='-DGC_MARK_STACK=GC_USE_GCPROS_AS_BEFORE'".
   Methods 2 and 3 are present mainly to debug the transition from 0 to 1.  */

#define GC_USE_GCPROS_AS_BEFORE		0
#define GC_MAKE_GCPROS_NOOPS		1
#define GC_MARK_STACK_CHECK_GCPROS	2
#define GC_USE_GCPROS_CHECK_ZOMBIES	3

#ifndef GC_MARK_STACK
# define GC_MARK_STACK GC_MAKE_GCPROS_NOOPS
#endif /* !GC_MARK_STACK */

/* Whether we do the stack marking manually.  */
#define BYTE_MARK_STACK !(GC_MARK_STACK == GC_MAKE_GCPROS_NOOPS		\
			  || GC_MARK_STACK == GC_MARK_STACK_CHECK_GCPROS)


#if GC_MARK_STACK == GC_MAKE_GCPROS_NOOPS

/* Do something silly with gcproN vars just so gcc shuts up.  */
/* You get warnings from MIPSPro...  */

#define GCPRO1(varname) ((void) gcpro1)
#define GCPRO2(varname1, varname2) ((void) gcpro2, (void) gcpro1)
#define GCPRO3(varname1, varname2, varname3) \
  ((void) gcpro3, (void) gcpro2, (void) gcpro1)
#define GCPRO4(varname1, varname2, varname3, varname4) \
  ((void) gcpro4, (void) gcpro3, (void) gcpro2, (void) gcpro1)
#define GCPRO5(varname1, varname2, varname3, varname4, varname5) \
  ((void) gcpro5, (void) gcpro4, (void) gcpro3, (void) gcpro2, (void) gcpro1)
#define GCPRO6(varname1, varname2, varname3, varname4, varname5, varname6) \
  ((void) gcpro6, (void) gcpro5, (void) gcpro4, (void) gcpro3, (void) gcpro2, \
   (void) gcpro1)
#define GCPRO7(a, b, c, d, e, f, g) (GCPRO6 (a, b, c, d, e, f), (void) gcpro7)
#define UNGCPRO ((void) 0)

#else /* GC_MARK_STACK != GC_MAKE_GCPROS_NOOPS */

#ifndef DEBUG_GCPRO

#define GCPRO1(varname) \
 {gcpro1.next = gcprolist; gcpro1.var = &varname; gcpro1.nvars = 1; \
  gcprolist = &gcpro1; }

#define GCPRO2(varname1, varname2) \
 {gcpro1.next = gcprolist; gcpro1.var = &varname1; gcpro1.nvars = 1; \
  gcpro2.next = &gcpro1; gcpro2.var = &varname2; gcpro2.nvars = 1; \
  gcprolist = &gcpro2; }

#define GCPRO3(varname1, varname2, varname3) \
 {gcpro1.next = gcprolist; gcpro1.var = &varname1; gcpro1.nvars = 1; \
  gcpro2.next = &gcpro1; gcpro2.var = &varname2; gcpro2.nvars = 1; \
  gcpro3.next = &gcpro2; gcpro3.var = &varname3; gcpro3.nvars = 1; \
  gcprolist = &gcpro3; }

#define GCPRO4(varname1, varname2, varname3, varname4) \
 {gcpro1.next = gcprolist; gcpro1.var = &varname1; gcpro1.nvars = 1; \
  gcpro2.next = &gcpro1; gcpro2.var = &varname2; gcpro2.nvars = 1; \
  gcpro3.next = &gcpro2; gcpro3.var = &varname3; gcpro3.nvars = 1; \
  gcpro4.next = &gcpro3; gcpro4.var = &varname4; gcpro4.nvars = 1; \
  gcprolist = &gcpro4; }

#define GCPRO5(varname1, varname2, varname3, varname4, varname5) \
 {gcpro1.next = gcprolist; gcpro1.var = &varname1; gcpro1.nvars = 1; \
  gcpro2.next = &gcpro1; gcpro2.var = &varname2; gcpro2.nvars = 1; \
  gcpro3.next = &gcpro2; gcpro3.var = &varname3; gcpro3.nvars = 1; \
  gcpro4.next = &gcpro3; gcpro4.var = &varname4; gcpro4.nvars = 1; \
  gcpro5.next = &gcpro4; gcpro5.var = &varname5; gcpro5.nvars = 1; \
  gcprolist = &gcpro5; }

#define GCPRO6(varname1, varname2, varname3, varname4, varname5, varname6) \
 {gcpro1.next = gcprolist; gcpro1.var = &varname1; gcpro1.nvars = 1; \
  gcpro2.next = &gcpro1; gcpro2.var = &varname2; gcpro2.nvars = 1; \
  gcpro3.next = &gcpro2; gcpro3.var = &varname3; gcpro3.nvars = 1; \
  gcpro4.next = &gcpro3; gcpro4.var = &varname4; gcpro4.nvars = 1; \
  gcpro5.next = &gcpro4; gcpro5.var = &varname5; gcpro5.nvars = 1; \
  gcpro6.next = &gcpro5; gcpro6.var = &varname6; gcpro6.nvars = 1; \
  gcprolist = &gcpro6; }

#define GCPRO7(a, b, c, d, e, f, g)				\
 {gcpro1.next = gcprolist; gcpro1.var = &(a); gcpro1.nvars = 1;	\
  gcpro2.next = &gcpro1; gcpro2.var = &(b); gcpro2.nvars = 1;	\
  gcpro3.next = &gcpro2; gcpro3.var = &(c); gcpro3.nvars = 1;	\
  gcpro4.next = &gcpro3; gcpro4.var = &(d); gcpro4.nvars = 1;	\
  gcpro5.next = &gcpro4; gcpro5.var = &(e); gcpro5.nvars = 1;	\
  gcpro6.next = &gcpro5; gcpro6.var = &(f); gcpro6.nvars = 1;	\
  gcpro7.next = &gcpro6; gcpro7.var = &(g); gcpro7.nvars = 1;	\
  gcprolist = &gcpro7; }

#define UNGCPRO (gcprolist = gcpro1.next)

#else

extern int gcpro_level;

#define GCPRO1(varname) \
 {gcpro1.next = gcprolist; gcpro1.var = &varname; gcpro1.nvars = 1; \
  gcpro1.level = gcpro_level++; \
  gcprolist = &gcpro1; }

#define GCPRO2(varname1, varname2) \
 {gcpro1.next = gcprolist; gcpro1.var = &varname1; gcpro1.nvars = 1; \
  gcpro1.level = gcpro_level; \
  gcpro2.next = &gcpro1; gcpro2.var = &varname2; gcpro2.nvars = 1; \
  gcpro2.level = gcpro_level++; \
  gcprolist = &gcpro2; }

#define GCPRO3(varname1, varname2, varname3) \
 {gcpro1.next = gcprolist; gcpro1.var = &varname1; gcpro1.nvars = 1; \
  gcpro1.level = gcpro_level; \
  gcpro2.next = &gcpro1; gcpro2.var = &varname2; gcpro2.nvars = 1; \
  gcpro3.next = &gcpro2; gcpro3.var = &varname3; gcpro3.nvars = 1; \
  gcpro3.level = gcpro_level++; \
  gcprolist = &gcpro3; }

#define GCPRO4(varname1, varname2, varname3, varname4) \
 {gcpro1.next = gcprolist; gcpro1.var = &varname1; gcpro1.nvars = 1; \
  gcpro1.level = gcpro_level; \
  gcpro2.next = &gcpro1; gcpro2.var = &varname2; gcpro2.nvars = 1; \
  gcpro3.next = &gcpro2; gcpro3.var = &varname3; gcpro3.nvars = 1; \
  gcpro4.next = &gcpro3; gcpro4.var = &varname4; gcpro4.nvars = 1; \
  gcpro4.level = gcpro_level++; \
  gcprolist = &gcpro4; }

#define GCPRO5(varname1, varname2, varname3, varname4, varname5) \
 {gcpro1.next = gcprolist; gcpro1.var = &varname1; gcpro1.nvars = 1; \
  gcpro1.level = gcpro_level; \
  gcpro2.next = &gcpro1; gcpro2.var = &varname2; gcpro2.nvars = 1; \
  gcpro3.next = &gcpro2; gcpro3.var = &varname3; gcpro3.nvars = 1; \
  gcpro4.next = &gcpro3; gcpro4.var = &varname4; gcpro4.nvars = 1; \
  gcpro5.next = &gcpro4; gcpro5.var = &varname5; gcpro5.nvars = 1; \
  gcpro5.level = gcpro_level++; \
  gcprolist = &gcpro5; }

#define GCPRO6(varname1, varname2, varname3, varname4, varname5, varname6) \
 {gcpro1.next = gcprolist; gcpro1.var = &varname1; gcpro1.nvars = 1; \
  gcpro1.level = gcpro_level; \
  gcpro2.next = &gcpro1; gcpro2.var = &varname2; gcpro2.nvars = 1; \
  gcpro3.next = &gcpro2; gcpro3.var = &varname3; gcpro3.nvars = 1; \
  gcpro4.next = &gcpro3; gcpro4.var = &varname4; gcpro4.nvars = 1; \
  gcpro5.next = &gcpro4; gcpro5.var = &varname5; gcpro5.nvars = 1; \
  gcpro6.next = &gcpro5; gcpro6.var = &varname6; gcpro6.nvars = 1; \
  gcpro6.level = gcpro_level++; \
  gcprolist = &gcpro6; }

#define GCPRO7(a, b, c, d, e, f, g)					\
 {gcpro1.next = gcprolist; gcpro1.var = &(a); gcpro1.nvars = 1;		\
  gcpro1.level = gcpro_level;						\
  gcpro2.next = &gcpro1; gcpro2.var = &(b); gcpro2.nvars = 1;		\
  gcpro3.next = &gcpro2; gcpro3.var = &(c); gcpro3.nvars = 1;		\
  gcpro4.next = &gcpro3; gcpro4.var = &(d); gcpro4.nvars = 1;		\
  gcpro5.next = &gcpro4; gcpro5.var = &(e); gcpro5.nvars = 1;		\
  gcpro6.next = &gcpro5; gcpro6.var = &(f); gcpro6.nvars = 1;		\
  gcpro7.next = &gcpro6; gcpro7.var = &(g); gcpro7.nvars = 1;		\
  gcpro7.level = gcpro_level++;						\
  gcprolist = &gcpro7; }

#define UNGCPRO					\
  (--gcpro_level != gcpro1.level		\
   ? emacs_abort ()				\
   : (void) (gcprolist = gcpro1.next))

#endif /* DEBUG_GCPRO */
#endif /* GC_MARK_STACK != GC_MAKE_GCPROS_NOOPS */


/* Evaluate expr, UNGCPRO, and then return the value of expr.  */
#define RETURN_UNGCPRO(expr)			\
  do						\
    {						\
      Lisp_Object ret_ungc_val;			\
      ret_ungc_val = (expr);			\
      UNGCPRO;					\
      return ret_ungc_val;			\
    }						\
  while (false)

/* Call staticpro (&var) to protect static variable `var'.  */

void staticpro (Lisp_Object *);

/* Declare a Lisp-callable function.  The MAXARGS parameter has the same
   meaning as in the DEFUN macro, and is used to construct a prototype.  */
#if (!defined(__STDC__) && !defined(PROTOTYPES) && !defined(__PROTOTYPES)) \
    || defined(USE_NONANSI_DEFUN)
# define EXFUN(fnname, maxargs) \
   extern Lisp_Object fnname()
#else
/* We can use the same trick as in the DEFUN macro to generate the
   appropriate prototype.  */
# define EXFUN(fnname, maxargs) \
   extern Lisp_Object fnname DEFUN_ARGS_ ## maxargs
#endif /* (!__STDC__ && !PROTOTYPES && !__PROTOTYPES) || USE_NONANSI_DEFUN */

#include "globals.h"

/* Forward declarations for prototypes: */
struct window;
struct frame;

/* Copy COUNT Lisp_Objects from ARGS to contents of V starting from OFFSET: */
INLINE void
vcopy(Lisp_Object v, ptrdiff_t offset, Lisp_Object *args, ptrdiff_t count)
{
  eassert((0 <= offset) && (0 <= count) && ((offset + count) <= ASIZE(v)));
  memcpy((XVECTOR(v)->contents + offset), args, ((size_t)count * sizeof(*args)));
}

/* Functions to modify hash tables: */
INLINE void
set_hash_key_slot(struct Lisp_Hash_Table *h, ptrdiff_t idx, Lisp_Object val)
{
  gc_aset(h->key_and_value, (2 * idx), val);
}

INLINE void
set_hash_value_slot(struct Lisp_Hash_Table *h, ptrdiff_t idx, Lisp_Object val)
{
  gc_aset(h->key_and_value, (2 * idx + 1), val);
}

/* Use these functions to set Lisp_Object
   or pointer slots of struct Lisp_Symbol.  */
INLINE void
set_symbol_function(Lisp_Object sym, Lisp_Object function)
{
  XSYMBOL(sym)->function = function;
}

INLINE void
set_symbol_plist(Lisp_Object sym, Lisp_Object plist)
{
  XSYMBOL(sym)->plist = plist;
}

INLINE void
set_symbol_next(Lisp_Object sym, struct Lisp_Symbol *next)
{
  XSYMBOL(sym)->next = next;
}

/* Buffer-local (also frame-local) variable access functions: */
INLINE int
blv_found(struct Lisp_Buffer_Local_Value *blv)
{
  eassert (blv->found == !EQ(blv->defcell, blv->valcell));
  return blv->found;
}

/* Set overlay's property list: */
INLINE void
set_overlay_plist(Lisp_Object overlay, Lisp_Object plist)
{
  XOVERLAY(overlay)->plist = plist;
}

/* Get text properties of S: */
INLINE INTERVAL
string_intervals(Lisp_Object s)
{
  return XSTRING(s)->intervals;
}

/* Set text properties of S to I: */
INLINE void
set_string_intervals(Lisp_Object s, INTERVAL i)
{
  XSTRING(s)->intervals = i;
}

/* Set a Lisp slot in TABLE to VAL.  Most code should use this instead
 * of setting slots directly: */
INLINE void
set_char_table_defalt(Lisp_Object table, Lisp_Object val)
{
  XCHAR_TABLE (table)->defalt = val;
}
INLINE void
set_char_table_purpose(Lisp_Object table, Lisp_Object val)
{
  XCHAR_TABLE(table)->purpose = val;
}

/* Set different slots in (sub)character tables: */
INLINE void
set_char_table_extras(Lisp_Object table, ptrdiff_t idx, Lisp_Object val)
{
  eassert((0 <= idx) && (idx < CHAR_TABLE_EXTRA_SLOTS(XCHAR_TABLE(table))));
  XCHAR_TABLE(table)->extras[idx] = val;
}

INLINE void
set_char_table_contents(Lisp_Object table, ptrdiff_t idx, Lisp_Object val)
{
  eassert((0 <= idx) && (idx < (1 << CHARTAB_SIZE_BITS_0)));
  XCHAR_TABLE(table)->contents[idx] = val;
}

INLINE void
set_sub_char_table_contents(Lisp_Object table, ptrdiff_t idx, Lisp_Object val)
{
  XSUB_CHAR_TABLE(table)->contents[idx] = val;
}

/* Defined in data.c.  */
extern Lisp_Object Qnil, Qt, Qquote, Qlambda, Qunbound;
extern Lisp_Object Qerror_conditions, Qerror_message, Qtop_level;
extern Lisp_Object Qerror, Qquit, Qargs_out_of_range;
extern Lisp_Object Qvoid_variable, Qvoid_function;
extern Lisp_Object Qinvalid_read_syntax;
extern Lisp_Object Qinvalid_function, Qwrong_number_of_arguments, Qno_catch;
extern Lisp_Object Quser_error, Qend_of_file, Qarith_error, Qmark_inactive;
extern Lisp_Object Qbeginning_of_buffer, Qend_of_buffer, Qbuffer_read_only;
extern Lisp_Object Qtext_read_only;
extern Lisp_Object Qinteractive_form;
extern Lisp_Object Qcircular_list;
extern Lisp_Object Qintegerp, Qwholenump, Qsymbolp, Qlistp, Qconsp;
extern Lisp_Object Qstringp, Qarrayp, Qsequencep, Qbufferp;
extern Lisp_Object Qchar_or_string_p, Qmarkerp, Qinteger_or_marker_p, Qvectorp;
extern Lisp_Object Qbuffer_or_string_p;
extern Lisp_Object Qfboundp;
extern Lisp_Object Qchar_table_p, Qvector_or_char_table_p;

extern Lisp_Object Qcdr;

extern Lisp_Object Qrange_error, Qoverflow_error;

extern Lisp_Object Qfloatp;
extern Lisp_Object Qnumberp, Qnumber_or_marker_p;

extern Lisp_Object Qbuffer, Qinteger, Qsymbol;

extern Lisp_Object Qfont_spec, Qfont_entity, Qfont_object;

EXFUN (Fbyteorder, 0) ATTRIBUTE_CONST;

/* Defined in data.c.  */
extern Lisp_Object indirect_function (Lisp_Object);
extern Lisp_Object find_symbol_value (Lisp_Object);
enum Arith_Comparison {
  ARITH_EQUAL,
  ARITH_NOTEQUAL,
  ARITH_LESS,
  ARITH_GRTR,
  ARITH_LESS_OR_EQUAL,
  ARITH_GRTR_OR_EQUAL
};
extern Lisp_Object arithcompare (Lisp_Object num1, Lisp_Object num2,
                                 enum Arith_Comparison comparison);

/* Convert the integer I to an Emacs representation, either the integer
   itself, or a cons of two or three integers, or if all else fails a float.
   I should not have side effects.  */
#define INTEGER_TO_CONS(i)					    \
  (! FIXNUM_OVERFLOW_P (i)					    \
   ? make_number (i)						    \
   : ! ((FIXNUM_OVERFLOW_P (INTMAX_MIN >> 16)			    \
	 || FIXNUM_OVERFLOW_P (UINTMAX_MAX >> 16))		    \
	&& FIXNUM_OVERFLOW_P ((i) >> 16))			    \
   ? Fcons (make_number ((i) >> 16), make_number ((i) & 0xffff))    \
   : ! ((FIXNUM_OVERFLOW_P (INTMAX_MIN >> 16 >> 24)		    \
	 || FIXNUM_OVERFLOW_P (UINTMAX_MAX >> 16 >> 24))	    \
	&& FIXNUM_OVERFLOW_P ((i) >> 16 >> 24))			    \
   ? Fcons (make_number ((i) >> 16 >> 24),			    \
	    Fcons (make_number ((i) >> 16 & 0xffffff),		    \
		   make_number ((i) & 0xffff)))			    \
   : make_float (i))

/* Convert the Emacs representation CONS back to an integer of type
   TYPE, storing the result the variable VAR.  Signal an error if CONS
   is not a valid representation or is out of range for TYPE.  */
#define CONS_TO_INTEGER(cons, type, var)				\
 (TYPE_SIGNED (type)							\
  ? ((var) = cons_to_signed (cons, TYPE_MINIMUM (type), TYPE_MAXIMUM (type))) \
  : ((var) = cons_to_unsigned (cons, TYPE_MAXIMUM (type))))
extern intmax_t cons_to_signed (Lisp_Object, intmax_t, intmax_t);
extern uintmax_t cons_to_unsigned (Lisp_Object, uintmax_t);

#if defined(_USE_OLD_LISP_DATA_STRUCTURES) && defined(P_)
extern Lisp_Object indirect_variable P_((Lisp_Object));
#else
extern struct Lisp_Symbol *indirect_variable(struct Lisp_Symbol *);
#endif /* _USE_OLD_LISP_DATA_STRUCTURES && P_ */
extern Lisp_Object long_to_cons (unsigned long);
extern unsigned long cons_to_long (Lisp_Object);
extern _Noreturn void args_out_of_range (Lisp_Object, Lisp_Object);
extern _Noreturn void args_out_of_range_3 (Lisp_Object, Lisp_Object,
					   Lisp_Object);
extern _Noreturn Lisp_Object wrong_type_argument (Lisp_Object, Lisp_Object);
extern Lisp_Object do_symval_forwarding (union Lisp_Fwd *);
extern void set_internal (Lisp_Object, Lisp_Object, Lisp_Object, bool);
extern void syms_of_data (void);
extern void swap_in_global_binding (struct Lisp_Symbol *);

/* Defined in cmds.c */
extern void syms_of_cmds (void);
extern void keys_of_cmds (void);

/* Defined in coding.c.  */
extern Lisp_Object Qcharset;
extern Lisp_Object detect_coding_system (const unsigned char *, ptrdiff_t,
                                         ptrdiff_t, bool, bool, Lisp_Object);
extern void init_coding (void);
extern void init_coding_once (void);
extern void syms_of_coding (void);

/* Defined in character.c.  */
EXFUN (Fmax_char, 0) ATTRIBUTE_CONST;
extern ptrdiff_t chars_in_text (const unsigned char *, ptrdiff_t);
extern ptrdiff_t multibyte_chars_in_text (const unsigned char *, ptrdiff_t);
extern int multibyte_char_to_unibyte (int) ATTRIBUTE_CONST;
extern int multibyte_char_to_unibyte_safe (int) ATTRIBUTE_CONST;
extern void syms_of_character (void);

/* Defined in charset.c.  */
extern void init_charset (void);
extern void init_charset_once (void);
extern void syms_of_charset (void);
/* Structure forward declarations.  */
struct charset;

/* Defined in composite.c.  */
extern void syms_of_composite (void);

/* Defined in syntax.c.  */
extern void init_syntax_once (void);
extern void syms_of_syntax (void);

/* Defined in fns.c.  */
extern Lisp_Object QCrehash_size, QCrehash_threshold;
enum { NEXT_ALMOST_PRIME_LIMIT = 11 };
EXFUN (Fidentity, 1) ATTRIBUTE_CONST;
extern EMACS_INT next_almost_prime (EMACS_INT) ATTRIBUTE_CONST;
extern Lisp_Object larger_vector (Lisp_Object, ptrdiff_t, ptrdiff_t);
extern void sweep_weak_hash_tables (void);
extern Lisp_Object Qcursor_in_echo_area;
extern Lisp_Object Qstring_lessp;
extern Lisp_Object QCsize, QCtest, QCweakness, Qequal, Qeq;
EMACS_UINT hash_string (char const *, ptrdiff_t);
EMACS_UINT sxhash (Lisp_Object, int);
Lisp_Object make_hash_table (struct hash_table_test, Lisp_Object, Lisp_Object,
                             Lisp_Object, Lisp_Object);
Lisp_Object make_hash_table_old_way (Lisp_Object, Lisp_Object, Lisp_Object,
                                     Lisp_Object, Lisp_Object, Lisp_Object,
                                     Lisp_Object);
ptrdiff_t hash_lookup (struct Lisp_Hash_Table *, Lisp_Object, EMACS_UINT *);
ptrdiff_t hash_put (struct Lisp_Hash_Table *, Lisp_Object, Lisp_Object,
		    EMACS_UINT);
extern struct hash_table_test hashtest_eql, hashtest_equal;

extern Lisp_Object substring_both (Lisp_Object, ptrdiff_t, ptrdiff_t,
				   ptrdiff_t, ptrdiff_t);
extern Lisp_Object merge (Lisp_Object, Lisp_Object, Lisp_Object);
extern Lisp_Object do_yes_or_no_p (Lisp_Object);
extern Lisp_Object concat2 (Lisp_Object, Lisp_Object);
extern Lisp_Object concat3 (Lisp_Object, Lisp_Object, Lisp_Object);
extern Lisp_Object nconc2 (Lisp_Object, Lisp_Object);
extern Lisp_Object assq_no_quit (Lisp_Object, Lisp_Object);
extern Lisp_Object assoc_no_quit (Lisp_Object, Lisp_Object);
extern void clear_string_char_byte_cache (void);
extern ptrdiff_t string_char_to_byte (Lisp_Object, ptrdiff_t);
extern ptrdiff_t string_byte_to_char (Lisp_Object, ptrdiff_t);
extern Lisp_Object string_to_multibyte (Lisp_Object);
extern Lisp_Object string_make_unibyte (Lisp_Object);
extern void syms_of_fns (void);

/* Defined in floatfns.c.  */
extern double extract_float (Lisp_Object);
extern void syms_of_floatfns (void);
extern Lisp_Object fmod_float (Lisp_Object x, Lisp_Object y);

/* Defined in fringe.c.  */
extern void syms_of_fringe (void);
extern void init_fringe (void);
#ifdef HAVE_WINDOW_SYSTEM
extern void mark_fringe_data (void);
extern void init_fringe_once (void);
#endif /* HAVE_WINDOW_SYSTEM */

/* Defined in image.c.  */
extern Lisp_Object QCascent, QCmargin, QCrelief;
extern Lisp_Object QCconversion;
extern int x_bitmap_mask (struct frame *, ptrdiff_t);
extern void reset_image_types (void);
extern void syms_of_image (void);

/* Defined in insdel.c.  */
extern Lisp_Object Qinhibit_modification_hooks;
extern void move_gap_both (ptrdiff_t, ptrdiff_t);
extern _Noreturn void buffer_overflow (void);
extern void make_gap (ptrdiff_t);
extern void make_gap_1 (struct buffer *, ptrdiff_t);
extern ptrdiff_t copy_text (const unsigned char *, unsigned char *,
			    ptrdiff_t, bool, bool);
extern int count_combining_before (const unsigned char *,
				   ptrdiff_t, ptrdiff_t, ptrdiff_t);
extern int count_combining_after (const unsigned char *,
				  ptrdiff_t, ptrdiff_t, ptrdiff_t);
extern void insert (const char *, ptrdiff_t);
extern void insert_and_inherit (const char *, ptrdiff_t);
extern void insert_1_both (const char *, ptrdiff_t, ptrdiff_t,
			   bool, bool, bool);
extern void insert_from_gap (ptrdiff_t, ptrdiff_t, bool text_at_gap_tail);
extern void insert_from_string (Lisp_Object, ptrdiff_t, ptrdiff_t,
				ptrdiff_t, ptrdiff_t, bool);
extern void insert_from_buffer (struct buffer *, ptrdiff_t, ptrdiff_t, bool);
extern void insert_char (int);
extern void insert_string (const char *);
extern void insert_before_markers (const char *, ptrdiff_t);
extern void insert_before_markers_and_inherit (const char *, ptrdiff_t);
extern void insert_from_string_before_markers (Lisp_Object, ptrdiff_t,
					       ptrdiff_t, ptrdiff_t,
					       ptrdiff_t, bool);
extern void del_range (ptrdiff_t, ptrdiff_t);
extern Lisp_Object del_range_1 (ptrdiff_t, ptrdiff_t, bool, bool);
extern void del_range_byte (ptrdiff_t, ptrdiff_t, bool);
extern void del_range_both (ptrdiff_t, ptrdiff_t, ptrdiff_t, ptrdiff_t, bool);
extern Lisp_Object del_range_2 (ptrdiff_t, ptrdiff_t,
				ptrdiff_t, ptrdiff_t, bool);
extern void modify_text (ptrdiff_t, ptrdiff_t);
extern void prepare_to_modify_buffer (ptrdiff_t, ptrdiff_t, ptrdiff_t *);
extern void prepare_to_modify_buffer_1 (ptrdiff_t, ptrdiff_t, ptrdiff_t *);
extern void invalidate_buffer_caches (struct buffer *, ptrdiff_t, ptrdiff_t);
extern void signal_after_change (ptrdiff_t, ptrdiff_t, ptrdiff_t);
extern void adjust_after_insert (ptrdiff_t, ptrdiff_t, ptrdiff_t,
				 ptrdiff_t, ptrdiff_t);
extern void adjust_markers_for_delete (ptrdiff_t, ptrdiff_t,
				       ptrdiff_t, ptrdiff_t);
extern void replace_range (ptrdiff_t, ptrdiff_t, Lisp_Object, bool, bool, bool);
extern void replace_range_2 (ptrdiff_t, ptrdiff_t, ptrdiff_t, ptrdiff_t,
			     const char *, ptrdiff_t, ptrdiff_t, bool);
extern void syms_of_insdel (void);

/* Defined in dispnew.c.  */
#if (defined PROFILING \
     && (defined __FreeBSD__ || defined GNU_LINUX || defined __MINGW32__))
_Noreturn void __executable_start (void);
#endif
extern Lisp_Object Vwindow_system;
extern Lisp_Object sit_for (Lisp_Object, bool, int);
extern void init_display (void);
extern void syms_of_display (void);

/* Defined in xdisp.c.  */
extern Lisp_Object Qinhibit_point_motion_hooks;
extern Lisp_Object Qinhibit_redisplay, Qdisplay;
extern Lisp_Object Qmenu_bar_update_hook;
extern Lisp_Object Qwindow_scroll_functions;
extern Lisp_Object Qoverriding_local_map, Qoverriding_terminal_local_map;
extern Lisp_Object Qimage, Qtext, Qboth, Qboth_horiz, Qtext_image_horiz;
extern Lisp_Object Qspace, Qcenter, QCalign_to;
extern Lisp_Object Qbar, Qhbar, Qbox, Qhollow;
extern Lisp_Object Qleft_margin, Qright_margin;
extern Lisp_Object QCdata, QCfile;
extern Lisp_Object QCmap;
#ifdef HAVE_CARBON
extern Lisp_Object Qrect;
#endif /* HAVE_CARBON */
extern Lisp_Object Qrisky_local_variable;
extern bool noninteractive_need_newline;
extern Lisp_Object echo_area_buffer[2];
extern void add_to_log (const char *, Lisp_Object, Lisp_Object);
extern void check_message_stack (void);
extern void setup_echo_area_for_printing (int);
extern bool push_message (void);
extern void pop_message_unwind (void);
extern Lisp_Object restore_message_unwind (Lisp_Object);
extern void restore_message (void);
extern Lisp_Object current_message (void);
extern void clear_message (bool, bool);
extern void message (const char *, ...) ATTRIBUTE_FORMAT_PRINTF (1, 2);
extern void message1 (const char *);
extern void message1_nolog (const char *);
extern void message3 (Lisp_Object);
extern void message3_nolog (Lisp_Object);
extern void message_dolog (const char *, ptrdiff_t, bool, bool);
extern void message_with_string (const char *, Lisp_Object, int);
extern void message_log_maybe_newline (void);
extern void update_echo_area (void);
extern void truncate_echo_area (ptrdiff_t);
extern void redisplay (void);
extern void redisplay_preserve_echo_area (int);

void set_frame_cursor_types (struct frame *, Lisp_Object);
extern void syms_of_xdisp (void);
extern void init_xdisp (void);
extern Lisp_Object safe_eval (Lisp_Object);
extern int pos_visible_p (struct window *, ptrdiff_t, int *,
                          int *, int *, int *, int *, int *);

/* Defined in xsettings.c.  */
extern void syms_of_xsettings (void);

/* Defined in vm-limit.c.  */
extern void memory_warnings (void *, void (*warnfun) (const char *));

/* Defined in alloc.c.  */
extern void check_pure_size (void);
extern void free_misc (Lisp_Object);
extern void allocate_string_data (struct Lisp_String *, EMACS_INT, EMACS_INT);
extern void malloc_warning (const char *);
extern _Noreturn void memory_full (size_t);
extern _Noreturn void buffer_memory_full (ptrdiff_t);
extern bool survives_gc_p (Lisp_Object);
extern void mark_object (Lisp_Object);
#if defined REL_ALLOC && !defined SYSTEM_MALLOC
extern void refill_memory_reserve (void);
#endif /* REL_ALLOC && !SYSTEM_MALLOC */
extern const char *pending_malloc_warning;
extern Lisp_Object zero_vector;
extern Lisp_Object *stack_base;
extern EMACS_INT consing_since_gc;
extern EMACS_INT gc_relative_threshold;
extern EMACS_INT memory_full_cons_threshold;
extern Lisp_Object list1(Lisp_Object);
extern Lisp_Object list2(Lisp_Object, Lisp_Object);
extern Lisp_Object list3(Lisp_Object, Lisp_Object, Lisp_Object);
extern Lisp_Object list4(Lisp_Object, Lisp_Object, Lisp_Object, Lisp_Object);
extern Lisp_Object list5(Lisp_Object, Lisp_Object, Lisp_Object, Lisp_Object,
			 Lisp_Object);
enum constype { CONSTYPE_HEAP, CONSTYPE_PURE };
extern Lisp_Object listn(enum constype, ptrdiff_t, Lisp_Object, ...);
extern Lisp_Object make_save_value(void *, int);

/* Build a frequently used 2/3/4-integer lists.  */

INLINE Lisp_Object
list2i (EMACS_INT x, EMACS_INT y)
{
  return list2 (make_number (x), make_number (y));
}

INLINE Lisp_Object
list3i (EMACS_INT x, EMACS_INT y, EMACS_INT w)
{
  return list3 (make_number (x), make_number (y), make_number (w));
}

INLINE Lisp_Object
list4i (EMACS_INT x, EMACS_INT y, EMACS_INT w, EMACS_INT h)
{
  return list4 (make_number (x), make_number (y),
		make_number (w), make_number (h));
}

extern Lisp_Object make_uninit_bool_vector (EMACS_INT);
extern Lisp_Object bool_vector_fill (Lisp_Object, Lisp_Object);
extern _Noreturn void string_overflow (void);
extern Lisp_Object make_string (const char *, ptrdiff_t);
extern Lisp_Object make_formatted_string (char *, const char *, ...)
  ATTRIBUTE_FORMAT_PRINTF (2, 3);
extern Lisp_Object make_unibyte_string (const char *, ptrdiff_t);

/* Make unibyte string from C string when the length is unknown: */
INLINE Lisp_Object
build_unibyte_string(const char *str)
{
  return make_unibyte_string(str, (ptrdiff_t)strlen((str != NULL)
                                                    ? str : ""));
}

extern Lisp_Object make_multibyte_string (const char *, ptrdiff_t, ptrdiff_t);
extern Lisp_Object make_event_array (ptrdiff_t, Lisp_Object *);
extern Lisp_Object make_uninit_string (EMACS_INT);
extern Lisp_Object make_uninit_multibyte_string (EMACS_INT, EMACS_INT);
extern Lisp_Object make_string_from_bytes (const char *, ptrdiff_t, ptrdiff_t);
extern Lisp_Object make_specified_string (const char *,
					  ptrdiff_t, ptrdiff_t, bool);
extern Lisp_Object make_pure_string (const char *, ptrdiff_t, ptrdiff_t, bool);
extern Lisp_Object make_pure_c_string (const char *, ptrdiff_t);

/* Make a string allocated in pure space, use STR as string data: */
INLINE Lisp_Object build_pure_c_string(const char *str)
{
  return make_pure_c_string(str, (ptrdiff_t)strlen(str));
}

/* Make a string from the data at STR, treating it as multibyte if the
 * data warrants: */
INLINE Lisp_Object build_string(const char *str)
{
  return make_string(str, (ptrdiff_t)strlen(str));
}

extern Lisp_Object pure_cons (Lisp_Object, Lisp_Object);
extern void make_byte_code (struct Lisp_Vector *);
extern Lisp_Object Qautomatic_gc;
extern Lisp_Object Qchar_table_extra_slots;
extern struct Lisp_Vector *allocate_vector (EMACS_INT);

/* Make an uninitialized vector for SIZE objects.  NOTE: you must
   be sure that GC cannot happen until the vector is completely
   initialized.  E.g. the following code is likely to crash:

   v = make_uninit_vector (3);
   ASET (v, 0, obj0);
   ASET (v, 1, Ffunction_can_gc ());
   ASET (v, 2, obj1);  */

INLINE Lisp_Object
make_uninit_vector (ptrdiff_t size)
{
  Lisp_Object v;
  struct Lisp_Vector *p;

  p = allocate_vector (size);
  XSETVECTOR (v, p);
  return v;
}

extern struct Lisp_Vector *allocate_pseudovector (int, int, enum pvec_type);
#define ALLOCATE_PSEUDOVECTOR(typ,field,tag)				\
  ((typ*)								\
   allocate_pseudovector						\
       (VECSIZE (typ), PSEUDOVECSIZE (typ, field), tag))
extern struct Lisp_Hash_Table *allocate_hash_table (void);
extern struct window *allocate_window (void);
extern struct frame *allocate_frame (void);
extern struct Lisp_Process *allocate_process (void);
extern struct terminal *allocate_terminal (void);
extern bool gc_in_progress;
extern bool abort_on_gc;
extern Lisp_Object make_float (double);
extern void display_malloc_warning (void);
extern ptrdiff_t inhibit_garbage_collection (void);
extern Lisp_Object make_save_int_int_int (ptrdiff_t, ptrdiff_t, ptrdiff_t);
extern Lisp_Object make_save_obj_obj_obj_obj (Lisp_Object, Lisp_Object,
					      Lisp_Object, Lisp_Object);
extern Lisp_Object make_save_ptr (void *);
extern Lisp_Object make_save_ptr_int (void *, ptrdiff_t);
extern Lisp_Object make_save_ptr_ptr (void *, void *);
extern Lisp_Object make_save_funcptr_ptr_obj (void (*) (void), void *,
					      Lisp_Object);
extern Lisp_Object make_save_memory (Lisp_Object *, ptrdiff_t);
extern void free_save_value (Lisp_Object);
extern Lisp_Object build_overlay (Lisp_Object, Lisp_Object, Lisp_Object);
extern void free_marker (Lisp_Object);
extern void free_cons (struct Lisp_Cons *);
extern void init_alloc_once (void);
extern void init_alloc (void);
extern void syms_of_alloc (void);
extern struct buffer * allocate_buffer (void);
extern int valid_lisp_object_p (Lisp_Object);
#ifdef GC_CHECK_CONS_LIST
extern void check_cons_list (void);
#else
INLINE void (check_cons_list) (void) { lisp_h_check_cons_list (); }
#endif /* GC_CHECK_CONS_LIST */

#ifdef REL_ALLOC
/* Defined in ralloc.c.  */
extern void *r_alloc (void **, size_t);
extern void r_alloc_free (void **);
extern void *r_re_alloc (void **, size_t);
extern void r_alloc_reset_variable (void **, void **);
extern void r_alloc_inhibit_buffer_relocation (int);
#endif /* REL_ALLOC */

/* Defined in chartab.c.  */
extern Lisp_Object copy_char_table (Lisp_Object);
extern Lisp_Object char_table_ref (Lisp_Object, int);
extern Lisp_Object char_table_ref_and_range (Lisp_Object, int,
                                             int *, int *);
extern void char_table_set (Lisp_Object, int, Lisp_Object);
extern void char_table_set_range (Lisp_Object, int, int, Lisp_Object);
extern int char_table_translate (Lisp_Object, int);
extern void map_char_table (void (*) (Lisp_Object, Lisp_Object,
                            Lisp_Object),
                            Lisp_Object, Lisp_Object, Lisp_Object);
extern void map_char_table_for_charset (void (*c_function) (Lisp_Object, Lisp_Object),
					Lisp_Object, Lisp_Object,
					Lisp_Object, struct charset *,
					unsigned, unsigned);
extern Lisp_Object uniprop_table (Lisp_Object);
extern void syms_of_chartab (void);

/* Defined in print.c.  */
extern Lisp_Object Vprin1_to_string_buffer;
extern void debug_print (Lisp_Object) EXTERNALLY_VISIBLE;
extern Lisp_Object Qstandard_output;
extern Lisp_Object Qexternal_debugging_output;
extern void temp_output_buffer_setup (const char *);
extern int print_level;
extern Lisp_Object Qprint_escape_newlines;
extern void write_string (const char *, int);
extern void print_error_message (Lisp_Object, Lisp_Object, const char *,
				 Lisp_Object);
extern Lisp_Object internal_with_output_to_temp_buffer
        (const char *, Lisp_Object (*) (Lisp_Object), Lisp_Object);
enum FLOAT_TO_STRING_BUFSIZE { FLOAT_TO_STRING_BUFSIZE = 350 };
extern int float_to_string (char *, double);
extern void init_print_once (void);
extern void syms_of_print (void);

/* Defined in doprnt.c.  */
extern ptrdiff_t doprnt (char *, ptrdiff_t, const char *, const char *,
			 va_list);
extern ptrdiff_t esprintf (char *, char const *, ...)
  ATTRIBUTE_FORMAT_PRINTF (2, 3);
extern ptrdiff_t exprintf (char **, ptrdiff_t *, char const *, ptrdiff_t,
			   char const *, ...)
  ATTRIBUTE_FORMAT_PRINTF (5, 6);
extern ptrdiff_t evxprintf (char **, ptrdiff_t *, char const *, ptrdiff_t,
			    char const *, va_list)
  ATTRIBUTE_FORMAT_PRINTF (5, 0);

/* Defined in lread.c.  */
#ifdef HAVE_CARBON
extern Lisp_Object Qdata, Qsize;
#endif /* HAVE_CARBON */
extern Lisp_Object Qvariable_documentation, Qstandard_input;
extern Lisp_Object Qbackquote, Qcomma, Qcomma_at, Qcomma_dot, Qfunction;
extern Lisp_Object Qlexical_binding;
extern Lisp_Object check_obarray (Lisp_Object);
extern Lisp_Object intern_1 (const char *, ptrdiff_t);
extern Lisp_Object intern_c_string_1 (const char *, ptrdiff_t);
extern Lisp_Object oblookup (Lisp_Object, const char *, ptrdiff_t, ptrdiff_t);
INLINE void
LOADHIST_ATTACH (Lisp_Object x)
{
  if (initialized)
    Vcurrent_load_list = Fcons (x, Vcurrent_load_list);
}
extern int openp (Lisp_Object, Lisp_Object, Lisp_Object,
                  Lisp_Object *, Lisp_Object, bool);
extern Lisp_Object string_to_number (char const *, int, bool);
extern void map_obarray (Lisp_Object, void (*) (Lisp_Object, Lisp_Object),
                         Lisp_Object);
extern void dir_warning (const char *, Lisp_Object);
extern void init_obarray (void);
extern void init_lread (void);
extern void syms_of_lread (void);

INLINE Lisp_Object intern(const char *str)
{
  return intern_1(str, (ptrdiff_t)strlen(str));
}

INLINE Lisp_Object intern_c_string(const char *str)
{
  return intern_c_string_1(str, (ptrdiff_t)strlen(str));
}

/* Defined in eval.c.  */
extern Lisp_Object Qautoload, Qexit, Qinteractive, Qcommandp, Qmacro;
extern Lisp_Object Qinhibit_quit, Qinternal_interpreter_environment, Qclosure;
extern Lisp_Object Qand_rest;
extern Lisp_Object Vautoload_queue;
extern Lisp_Object Vsignaling_function;
extern Lisp_Object inhibit_lisp_code;
extern struct handler *handlerlist;

/* To run a normal hook, use the appropriate function from the list below.
   The calling convention:

   if (!NILP (Vrun_hooks))
     call1 (Vrun_hooks, Qmy_funny_hook);

   should no longer be used.  */
extern Lisp_Object Vrun_hooks;
extern void run_hook_with_args_2 (Lisp_Object, Lisp_Object, Lisp_Object);
extern Lisp_Object run_hook_with_args (ptrdiff_t nargs, Lisp_Object *args,
				       Lisp_Object (*funcall)
				       (ptrdiff_t nargs, Lisp_Object *args));
extern _Noreturn void xsignal (Lisp_Object, Lisp_Object);
extern _Noreturn void xsignal0 (Lisp_Object);
extern _Noreturn void xsignal1 (Lisp_Object, Lisp_Object);
extern _Noreturn void xsignal2 (Lisp_Object, Lisp_Object, Lisp_Object);
extern _Noreturn void xsignal3 (Lisp_Object, Lisp_Object, Lisp_Object,
				Lisp_Object);
extern _Noreturn void signal_error (const char *, Lisp_Object);
extern Lisp_Object eval_sub (Lisp_Object form);
extern Lisp_Object apply1 (Lisp_Object, Lisp_Object);
extern Lisp_Object call0 (Lisp_Object);
extern Lisp_Object call1 (Lisp_Object, Lisp_Object);
extern Lisp_Object call2 (Lisp_Object, Lisp_Object, Lisp_Object);
extern Lisp_Object call3 (Lisp_Object, Lisp_Object, Lisp_Object, Lisp_Object);
extern Lisp_Object call4 (Lisp_Object, Lisp_Object, Lisp_Object, Lisp_Object, Lisp_Object);
extern Lisp_Object call5 (Lisp_Object, Lisp_Object, Lisp_Object, Lisp_Object, Lisp_Object, Lisp_Object);
extern Lisp_Object call6 (Lisp_Object, Lisp_Object, Lisp_Object, Lisp_Object, Lisp_Object, Lisp_Object, Lisp_Object);
extern Lisp_Object call7 (Lisp_Object, Lisp_Object, Lisp_Object, Lisp_Object, Lisp_Object, Lisp_Object, Lisp_Object, Lisp_Object);
extern Lisp_Object internal_catch (Lisp_Object, Lisp_Object (*) (Lisp_Object), Lisp_Object);
extern Lisp_Object internal_lisp_condition_case (Lisp_Object, Lisp_Object, Lisp_Object);
extern Lisp_Object internal_condition_case (Lisp_Object (*) (void), Lisp_Object, Lisp_Object (*) (Lisp_Object));
extern Lisp_Object internal_condition_case_1 (Lisp_Object (*) (Lisp_Object), Lisp_Object, Lisp_Object, Lisp_Object (*) (Lisp_Object));
extern Lisp_Object internal_condition_case_2 (Lisp_Object (*) (Lisp_Object, Lisp_Object), Lisp_Object, Lisp_Object, Lisp_Object, Lisp_Object (*) (Lisp_Object));
extern Lisp_Object internal_condition_case_n
    (Lisp_Object (*) (ptrdiff_t, Lisp_Object *), ptrdiff_t, Lisp_Object *,
     Lisp_Object, Lisp_Object (*) (Lisp_Object, ptrdiff_t, Lisp_Object *));
extern void specbind (Lisp_Object, Lisp_Object);
extern void record_unwind_protect (void (*) (Lisp_Object), Lisp_Object);
extern void record_unwind_protect_ptr (void (*) (void *), void *);
extern void record_unwind_protect_int (void (*) (int), int);
extern void record_unwind_protect_void (void (*) (void));
extern void record_unwind_protect_nothing (void);
extern void clear_unwind_protect (ptrdiff_t);
extern void set_unwind_protect (ptrdiff_t, void (*) (Lisp_Object), Lisp_Object);
extern void set_unwind_protect_ptr (ptrdiff_t, void (*) (void *), void *);
extern Lisp_Object unbind_to (ptrdiff_t, Lisp_Object);
extern _Noreturn void error (const char *, ...) ATTRIBUTE_FORMAT_PRINTF (1, 2);
extern _Noreturn void verror (const char *, va_list)
  ATTRIBUTE_FORMAT_PRINTF (1, 0);
extern void un_autoload (Lisp_Object);
extern Lisp_Object call_debugger (Lisp_Object arg);
extern void init_eval_once (void);
extern Lisp_Object safe_call (ptrdiff_t, Lisp_Object, ...);
extern Lisp_Object safe_call1 (Lisp_Object, Lisp_Object);
extern Lisp_Object safe_call2 (Lisp_Object, Lisp_Object, Lisp_Object);
extern void init_eval (void);
extern void syms_of_eval (void);
extern void unwind_body (Lisp_Object);
extern void record_in_backtrace (Lisp_Object function,
				 Lisp_Object *args, ptrdiff_t nargs);
extern void mark_specpdl (void);
extern void get_backtrace (Lisp_Object array);
Lisp_Object backtrace_top_function (void);
extern bool let_shadows_buffer_binding_p (struct Lisp_Symbol *symbol);
extern bool let_shadows_global_binding_p (Lisp_Object symbol);


/* Defined in editfns.c.  */
extern Lisp_Object Qfield;
extern void insert1 (Lisp_Object);
extern Lisp_Object format2 (const char *, Lisp_Object, Lisp_Object);
extern Lisp_Object save_excursion_save (void);
extern Lisp_Object save_restriction_save (void);
extern void save_excursion_restore (Lisp_Object);
extern void save_restriction_restore (Lisp_Object);
extern _Noreturn void time_overflow (void);
extern Lisp_Object make_buffer_string (ptrdiff_t, ptrdiff_t, bool);
extern Lisp_Object make_buffer_string_both (ptrdiff_t, ptrdiff_t, ptrdiff_t,
					    ptrdiff_t, bool);
extern void init_editfns (void);
extern void syms_of_editfns (void);
extern void set_time_zone_rule (const char *);

/* Defined in buffer.c.  */
extern bool mouse_face_overlay_overlaps (Lisp_Object);
extern _Noreturn void nsberror (Lisp_Object);
extern void adjust_overlays_for_insert (ptrdiff_t, ptrdiff_t);
extern void adjust_overlays_for_delete (ptrdiff_t, ptrdiff_t);
extern void fix_start_end_in_overlays (ptrdiff_t, ptrdiff_t);
extern void report_overlay_modification (Lisp_Object, Lisp_Object, bool,
                                         Lisp_Object, Lisp_Object, Lisp_Object);
extern bool overlay_touches_p (ptrdiff_t);
extern Lisp_Object other_buffer_safely (Lisp_Object);
extern Lisp_Object get_truename_buffer (Lisp_Object);
extern void init_buffer_once (void);
extern void init_buffer (void);
extern void syms_of_buffer (void);
extern void keys_of_buffer (void);

/* Defined in marker.c.  */

extern ptrdiff_t marker_position (Lisp_Object);
extern ptrdiff_t marker_byte_position (Lisp_Object);
extern void clear_charpos_cache (struct buffer *);
extern ptrdiff_t buf_charpos_to_bytepos (struct buffer *, ptrdiff_t);
extern ptrdiff_t buf_bytepos_to_charpos (struct buffer *, ptrdiff_t);
extern void unchain_marker (struct Lisp_Marker *marker);
extern Lisp_Object set_marker_restricted (Lisp_Object, Lisp_Object, Lisp_Object);
extern Lisp_Object set_marker_both (Lisp_Object, Lisp_Object, ptrdiff_t, ptrdiff_t);
extern Lisp_Object set_marker_restricted_both (Lisp_Object, Lisp_Object,
                                               ptrdiff_t, ptrdiff_t);
extern Lisp_Object build_marker (struct buffer *, ptrdiff_t, ptrdiff_t);
extern void syms_of_marker (void);

/* Defined in fileio.c.  */

extern Lisp_Object Qfile_error;
extern Lisp_Object Qfile_notify_error;
extern Lisp_Object Qfile_exists_p;
extern Lisp_Object Qfile_directory_p;
extern Lisp_Object Qinsert_file_contents;
extern Lisp_Object Qfile_name_history;
extern Lisp_Object expand_and_dir_to_file (Lisp_Object, Lisp_Object);
extern Lisp_Object write_region (Lisp_Object, Lisp_Object, Lisp_Object,
				 Lisp_Object, Lisp_Object, Lisp_Object,
				 Lisp_Object, int);
EXFUN (Fread_file_name, 6);     /* Not a normal DEFUN.  */
extern void close_file_unwind (int);
extern void fclose_unwind (void *);
extern void restore_point_unwind (Lisp_Object);
extern _Noreturn void report_file_errno (const char *, Lisp_Object, int);
extern _Noreturn void report_file_error (const char *, Lisp_Object);
extern bool internal_delete_file (Lisp_Object);
extern Lisp_Object emacs_readlinkat (int, const char *);
extern bool file_directory_p (const char *);
extern bool file_accessible_directory_p (const char *);
extern void init_fileio (void);
extern void syms_of_fileio (void);
extern Lisp_Object make_temp_name (Lisp_Object, bool);
extern Lisp_Object Qdelete_file;

#if !defined(syms_of_abbrev) || defined(_USE_OLD_LISP_DATA_STRUCTURES)
/* Defined in abbrev.c */
extern void syms_of_abbrev (void);
#endif /* !syms_of_abbrev || _USE_OLD_LISP_DATA_STRUCTURES */

/* Defined in search.c.  */
extern void shrink_regexp_cache (void);
extern void restore_search_regs (void);
extern void record_unwind_save_match_data (void);
struct re_registers;
extern struct re_pattern_buffer *compile_pattern (Lisp_Object,
						  struct re_registers *,
						  Lisp_Object, bool, bool);
extern ptrdiff_t fast_string_match (Lisp_Object, Lisp_Object);
extern ptrdiff_t fast_c_string_match_ignore_case (Lisp_Object, const char *,
						  ptrdiff_t);
extern ptrdiff_t fast_string_match_ignore_case (Lisp_Object, Lisp_Object);
extern ptrdiff_t fast_looking_at (Lisp_Object, ptrdiff_t, ptrdiff_t,
                                  ptrdiff_t, ptrdiff_t, Lisp_Object);
extern ptrdiff_t find_newline (ptrdiff_t, ptrdiff_t, ptrdiff_t, ptrdiff_t,
			       ptrdiff_t, ptrdiff_t *, ptrdiff_t *, bool);
extern ptrdiff_t scan_newline (ptrdiff_t, ptrdiff_t, ptrdiff_t, ptrdiff_t,
			       ptrdiff_t, bool);
extern ptrdiff_t find_newline_no_quit (ptrdiff_t, ptrdiff_t,
				       ptrdiff_t, ptrdiff_t *);
extern ptrdiff_t find_before_next_newline (ptrdiff_t, ptrdiff_t,
					   ptrdiff_t, ptrdiff_t *);
extern void syms_of_search (void);
extern void clear_regexp_cache (void);

/* Defined in minibuf.c.  */

extern Lisp_Object Qcompletion_ignore_case;
extern Lisp_Object Vminibuffer_list;
extern Lisp_Object last_minibuf_string;
extern Lisp_Object get_minibuffer (EMACS_INT);
extern void init_minibuf_once (void);
extern void syms_of_minibuf (void);

/* Defined in callint.c.  */

extern Lisp_Object Qminus, Qplus;
extern Lisp_Object Qwhen;
extern Lisp_Object Qmouse_leave_buffer_hook;
extern void syms_of_callint (void);

/* Defined in casefiddle.c.  */

extern Lisp_Object Qidentity;
extern void syms_of_casefiddle (void);
extern void keys_of_casefiddle (void);

/* Defined in casetab.c.  */

extern void init_casetab_once (void);
extern void syms_of_casetab (void);

/* Defined in keyboard.c.  */

extern Lisp_Object echo_message_buffer;
extern struct kboard *echo_kboard;
extern void cancel_echoing (void);
extern Lisp_Object Qdisabled, QCfilter;
extern Lisp_Object Qup, Qdown, Qbottom;
extern Lisp_Object Qtop;
extern Lisp_Object last_undo_boundary;
extern bool input_pending;
extern Lisp_Object menu_bar_items (Lisp_Object);
extern Lisp_Object tool_bar_items (Lisp_Object, int *);
extern void discard_mouse_events (void);
#ifdef USABLE_SIGIO
void handle_input_available_signal (int);
#endif /* USABLE_SIGIO */
extern Lisp_Object pending_funcalls;
extern bool detect_input_pending (void);
extern bool detect_input_pending_ignore_squeezables (void);
extern bool detect_input_pending_run_timers (bool);
extern void safe_run_hooks (Lisp_Object);
extern void cmd_error_internal (Lisp_Object, const char *);
extern Lisp_Object command_loop_1 (void);
extern Lisp_Object read_menu_command (void);
extern Lisp_Object recursive_edit_1 (void);
extern void record_auto_save (void);
extern void force_auto_save_soon (void);
extern void init_keyboard (void);
extern void syms_of_keyboard (void);
extern void keys_of_keyboard (void);

/* Defined in indent.c.  */
extern ptrdiff_t current_column (void);
extern void invalidate_current_column (void);
extern bool indented_beyond_p (ptrdiff_t, ptrdiff_t, EMACS_INT);
extern void syms_of_indent (void);

/* Defined in frame.c.  */
extern Lisp_Object Qonly, Qnone;
extern Lisp_Object Qvisible;
extern void set_frame_param (struct frame *, Lisp_Object, Lisp_Object);
extern void store_frame_param (struct frame *, Lisp_Object, Lisp_Object);
extern void store_in_alist (Lisp_Object *, Lisp_Object, Lisp_Object);
extern Lisp_Object do_switch_frame (Lisp_Object, int, int, Lisp_Object);
#if defined(HAVE_CARBON) || (defined(HAVE_NS) && HAVE_NS) || (defined(HAVE_NTGUI) && HAVE_NTGUI)
extern Lisp_Object get_frame_param (struct frame *, Lisp_Object);
#endif /* HAVE_CARBON || HAVE_NS || HAVE_NTGUI */
extern void frames_discard_buffer (Lisp_Object);
extern void syms_of_frame (void);

/* Defined in emacs.c.  */
extern char **initial_argv;
extern int initial_argc;
#if defined(HAVE_X_WINDOWS) || defined(HAVE_NS)
extern bool display_arg;
#endif /* HAVE_X_WINDOWS || HAVE_NS */
extern Lisp_Object decode_env_path (const char *, const char *, bool);
extern Lisp_Object empty_unibyte_string, empty_multibyte_string;
extern Lisp_Object Qfile_name_handler_alist;
extern _Noreturn void terminate_due_to_signal (int, int);
extern Lisp_Object Qkill_emacs;
#ifdef WINDOWSNT
extern Lisp_Object Vlibrary_cache;
#endif /* WINDOWSNT */
#if HAVE_SETLOCALE
void fixup_locale (void);
void synchronize_system_messages_locale (void);
void synchronize_system_time_locale (void);
#else
INLINE void fixup_locale (void) {}
INLINE void synchronize_system_messages_locale (void) {}
INLINE void synchronize_system_time_locale (void) {}
#endif
extern void shut_down_emacs (int, Lisp_Object);

/* True means don't do interactive redisplay and don't change tty modes.  */
extern bool noninteractive;

/* True means remove site-lisp directories from load-path.  */
extern bool no_site_lisp;

/* Pipe used to send exit notification to the daemon parent at
   startup.  */
extern int daemon_pipe[2];
#define IS_DAEMON (daemon_pipe[1] != 0)

/* True if handling a fatal error already.  */
extern bool fatal_error_in_progress;

/* True means don't do use window-system-specific display code.  */
extern bool inhibit_window_system;
/* True means that a filter or a sentinel is running.  */
extern bool running_asynch_code;

/* Defined in process.c.  */
extern Lisp_Object QCtype, Qlocal;
extern Lisp_Object Qprocessp;
extern void kill_buffer_processes (Lisp_Object);
extern bool wait_reading_process_output (intmax_t, int, int, bool,
					 Lisp_Object,
					 struct Lisp_Process *,
					 int);
/* Max value for the first argument of wait_reading_process_output.  */
#if __GNUC__ == 3 || (__GNUC__ == 4 && __GNUC_MINOR__ <= 5)
/* Work around a bug in GCC 3.4.2, known to be fixed in GCC 4.6.3.
   The bug merely causes a bogus warning, but the warning is annoying.  */
# define WAIT_READING_MAX min (TYPE_MAXIMUM (time_t), INTMAX_MAX)
#else
# define WAIT_READING_MAX INTMAX_MAX
#endif
extern void add_keyboard_wait_descriptor (int);
extern void delete_keyboard_wait_descriptor (int);
#ifdef HAVE_GPM
extern void add_gpm_wait_descriptor (int);
extern void delete_gpm_wait_descriptor (int);
#endif /* HAVE_GPM */
extern void init_process_emacs (void);
extern void syms_of_process (void);
extern void setup_process_coding_systems (Lisp_Object);

/* Defined in callproc.c.  */
#ifndef DOS_NT
 _Noreturn
#endif /* !DOS_NT */
extern int child_setup (int, int, int, char **, bool, Lisp_Object);
extern void init_callproc_1 (void);
extern void init_callproc (void);
extern void set_initial_environment (void);
extern void syms_of_callproc (void);

/* Defined in doc.c.  */
extern Lisp_Object Qfunction_documentation;
extern Lisp_Object read_doc_string (Lisp_Object);
extern Lisp_Object get_doc_string (Lisp_Object, bool, bool);
extern void syms_of_doc (void);
extern int read_bytecode_char (bool);

/* Defined in bytecode.c.  */
extern void syms_of_bytecode (void);
extern struct byte_stack *byte_stack_list;
#if BYTE_MARK_STACK
extern void mark_byte_stack (void);
#endif /* BYTE_MARK_STACK */
extern void unmark_byte_stack (void);
extern Lisp_Object exec_byte_code (Lisp_Object, Lisp_Object, Lisp_Object,
				   Lisp_Object, ptrdiff_t, Lisp_Object *);

/* Defined in macros.c.  */
extern void init_macros (void);
extern void syms_of_macros (void);

/* Defined in undo.c.  */
extern Lisp_Object Qapply;
extern Lisp_Object Qinhibit_read_only;
extern void truncate_undo_list (struct buffer *);
extern void record_marker_adjustment (Lisp_Object, ptrdiff_t);
extern void record_insert (ptrdiff_t, ptrdiff_t);
extern void record_delete (ptrdiff_t, Lisp_Object);
extern void record_first_change (void);
extern void record_change (ptrdiff_t, ptrdiff_t);
extern void record_property_change (ptrdiff_t, ptrdiff_t,
				    Lisp_Object, Lisp_Object,
                                    Lisp_Object);
extern void syms_of_undo (void);
/* Defined in textprop.c.  */
extern Lisp_Object Qfont, Qmouse_face;
extern Lisp_Object Qinsert_in_front_hooks, Qinsert_behind_hooks;
extern Lisp_Object Qfront_sticky, Qrear_nonsticky;
extern Lisp_Object Qminibuffer_prompt;

extern void report_interval_modification (Lisp_Object, Lisp_Object);

/* Defined in menu.c.  */
extern void syms_of_menu (void);

/* Defined in xmenu.c.  */
extern void syms_of_xmenu (void);

/* Defined in termchar.h.  */
struct tty_display_info;

/* Defined in termhooks.h.  */
struct terminal;

/* Defined in sysdep.c.  */
#ifndef HAVE_GET_CURRENT_DIR_NAME
extern char *get_current_dir_name (void);
#endif /* !HAVE_GET_CURRENT_DIR_NAME */
extern void stuff_char (char c);
extern void init_foreground_group (void);
extern void init_sigio (int);
extern void sys_subshell (void);
extern void sys_suspend (void);
extern void discard_tty_input (void);
extern void block_tty_out_signal (void);
extern void unblock_tty_out_signal (void);
extern void init_sys_modes (struct tty_display_info *);
extern void reset_sys_modes (struct tty_display_info *);
extern void init_all_sys_modes (void);
extern void reset_all_sys_modes (void);
extern void child_setup_tty (int);
extern void setup_pty (int);
extern int set_window_size (int, int, int);
extern EMACS_INT get_random (void);
extern void seed_random (void *, ptrdiff_t);
extern void init_random (void);
extern void emacs_backtrace (int);
extern _Noreturn void emacs_abort (void) NO_INLINE;
#ifndef emacs_open
extern int emacs_open(const char *, int, int);
#endif /* !emacs_open */
extern int emacs_pipe(int[2]);
#ifndef emacs_close
extern int emacs_close(int);
#endif /* !emacs_close */
extern ptrdiff_t emacs_read (int, void *, ptrdiff_t);
extern ptrdiff_t emacs_write (int, void const *, ptrdiff_t);
extern ptrdiff_t emacs_write_sig (int, void const *, ptrdiff_t);
extern void emacs_perror (char const *);

extern void unlock_all_files (void);
extern void lock_file (Lisp_Object);
extern void unlock_file (Lisp_Object);
extern void unlock_buffer (struct buffer *);
extern void syms_of_filelock (void);

/* Defined in sound.c.  */
extern void syms_of_sound (void);

/* Defined in category.c.  */
extern void init_category_once (void);
extern Lisp_Object char_category_set (int);
extern void syms_of_category (void);

/* Defined in ccl.c.  */
extern void syms_of_ccl (void);

/* Defined in dired.c.  */
extern void syms_of_dired (void);
extern Lisp_Object directory_files_internal (Lisp_Object, Lisp_Object,
                                             Lisp_Object, Lisp_Object,
                                             bool, Lisp_Object);

/* Defined in term.c.  */
extern int *char_ins_del_vector;
extern void syms_of_term (void);
extern _Noreturn void fatal (const char *msgid, ...)
  ATTRIBUTE_FORMAT_PRINTF (1, 2);

/* Defined in terminal.c.  */
extern void syms_of_terminal (void);

/* Defined in font.c.  */
extern void syms_of_font (void);
extern void init_font (void);

#ifdef HAVE_WINDOW_SYSTEM
/* Defined in fontset.c.  */
extern void syms_of_fontset (void);

/* Defined in xfns.c, w32fns.c, or macfns.c.  */
extern Lisp_Object Qfont_param;
#endif /* HAVE_WINDOW_SYSTEM */

/* Defined in gfilenotify.c */
#ifdef HAVE_GFILENOTIFY
extern void globals_of_gfilenotify (void);
extern void syms_of_gfilenotify (void);
#endif /* HAVE_GFILENOTIFY */

/* Defined in inotify.c */
#ifdef HAVE_INOTIFY
extern void syms_of_inotify (void);
#endif /* HAVE_INOTIFY */

#ifdef HAVE_W32NOTIFY
/* Defined on w32notify.c.  */
extern void syms_of_w32notify (void);
#endif /* HAVE_W32NOTIFY */

/* Defined in xfaces.c.  */
extern Lisp_Object Qdefault, Qtool_bar, Qfringe;
extern Lisp_Object Qheader_line, Qscroll_bar, Qcursor;
extern Lisp_Object Qmode_line_inactive;
extern Lisp_Object Qface;
extern Lisp_Object Qnormal;
extern Lisp_Object QCfamily, QCweight, QCslant;
extern Lisp_Object QCheight, QCname, QCwidth, QCforeground, QCbackground;
extern Lisp_Object Qextra_light, Qlight, Qsemi_light, Qsemi_bold;
extern Lisp_Object Qbold, Qextra_bold, Qultra_bold;
extern Lisp_Object Qoblique, Qitalic;
extern Lisp_Object Vface_alternative_font_family_alist;
extern Lisp_Object Vface_alternative_font_registry_alist;
extern void syms_of_xfaces (void);

#ifdef HAVE_X_WINDOWS
/* Defined in xfns.c.  */
extern void syms_of_xfns (void);

/* Defined in xsmfns.c.  */
extern void syms_of_xsmfns (void);

/* Defined in xselect.c.  */
extern void syms_of_xselect (void);

/* Defined in xterm.c.  */
extern void syms_of_xterm (void);
#endif /* HAVE_X_WINDOWS */

#ifdef HAVE_WINDOW_SYSTEM
/* Defined in xterm.c, nsterm.m, w32term.c.  */
extern char *x_get_keysym_name (int);
#endif /* HAVE_WINDOW_SYSTEM */

#ifdef HAVE_CARBON
/* Defined in macfns.c */
extern void syms_of_macfns (void);

/* Defined in macselect.c */
extern void syms_of_macselect (void);

/* Defined in macterm.c */
extern void syms_of_macterm (void);

/* Defined in macmenu.c */
extern void syms_of_macmenu (void);

/* Defined in mac.c */
extern const char *mac_etc_directory;
extern const char *mac_exec_path;
extern const char *mac_load_path;
extern void syms_of_mac (void);
extern void init_mac_osx_environment (void);
#endif /* HAVE_CARBON */

#ifdef HAVE_LIBXML2
/* Defined in xml.c.  */
extern void syms_of_xml (void);
extern void xml_cleanup_parser (void);
#endif /* HAVE_LIBXML2 */

#ifdef HAVE_ZLIB
/* Defined in decompress.c.  */
extern void syms_of_decompress (void);
#endif /* HAVE_ZLIB */

#ifdef HAVE_DBUS
/* Defined in dbusbind.c.  */
void syms_of_dbusbind (void);
#endif /* HAVE_DBUS */


/* Defined in profiler.c.  */
extern bool profiler_memory_running;
extern void malloc_probe (size_t);
extern void syms_of_profiler (void);


#ifdef DOS_NT
/* Defined in msdos.c, w32.c.  */
extern char *emacs_root_dir (void);
#endif /* DOS_NT */

/* True means Emacs has already been initialized.
   Used during startup to detect startup of dumped Emacs.  */
extern bool initialized;

/* True means ^G can quit instantly.  */
extern bool immediate_quit;

extern void *xmalloc (size_t) ATTRIBUTE_MALLOC;
extern void *xzalloc (size_t);
extern void *xrealloc (void *, size_t);
extern void xfree (void *);
extern void *xnmalloc (ptrdiff_t, ptrdiff_t) ATTRIBUTE_MALLOC;
extern void *xnrealloc (void *, ptrdiff_t, ptrdiff_t);
extern void *xpalloc (void *, ptrdiff_t *, ptrdiff_t, ptrdiff_t, ptrdiff_t);

extern char *xstrdup (const char *);
extern char *xlispstrdup (Lisp_Object);
extern void dupstring (char **, char const *);
extern void xputenv (const char *);

extern char *egetenv (const char *);

/* Copy Lisp string to temporary (allocated on stack) C string.  */

#define xlispstrdupa(string)			\
  memcpy (alloca (SBYTES (string) + 1),		\
	  SSDATA (string), SBYTES (string) + 1)

/* Set up the name of the machine we're running on.  */
extern void init_system_name (void);

/* Return the absolute value of X.  X should be a signed integer
   expression without side effects, and X's absolute value should not
   exceed the maximum for its promoted type.  This is called 'eabs'
   because 'abs' is reserved by the C standard.  */
#define eabs(x)         ((x) < 0 ? -(x) : (x))

/* Return a fixnum or float, depending on whether VAL fits in a Lisp
   fixnum.  */

#define make_fixnum_or_float(val) \
   (FIXNUM_OVERFLOW_P (val) ? make_float (val) : make_number (val))

/* SAFE_ALLOCA normally allocates memory on the stack, but if size is
   larger than MAX_ALLOCA, use xmalloc to avoid overflowing the stack.  */

enum MAX_ALLOCA { MAX_ALLOCA = 16 * 1024 };

extern void *record_xmalloc (size_t);

/* Take some diagnostic macros from glibc/include/libc-diag.h: */

/* Push diagnostic state.  */
#define DIAG_PUSH_NEEDS_COMMENT _Pragma ("GCC diagnostic push")

/* Pop diagnostic state.  */
#define DIAG_POP_NEEDS_COMMENT _Pragma ("GCC diagnostic pop")

#define _DIAG_STR1(s) #s
#define _DIAG_STR(s) _DIAG_STR1(s)

/* Ignore the diagnostic OPTION.  VERSION is the most recent GCC
 * version for which the diagnostic has been confirmed to appear in
 * the absence of the pragma (in the form MAJOR.MINOR for GCC 4.x,
 * just MAJOR for GCC 5 and later).  Uses of this pragma should be
 * reviewed when the GCC version given is no longer supported for
 * building emacs; the version number should always be on the same
 * source line as the macro name, so such uses can be found with grep.
 * Uses should come with a comment giving more details of the
 * diagnostic, and an architecture on which it is seen if possibly
 * optimization-related and not in architecture-specific code.  This
 * macro should only be used if the diagnostic seems hard to fix (for
 * example, optimization-related false positives).  */
#define DIAG_IGNORE_NEEDS_COMMENT(version, option)     \
  _Pragma (_DIAG_STR (GCC diagnostic ignored option))

/* FIXME: get SAFE_ALLOCA to push and pop -Walloca-larger-than */
#define USE_SAFE_ALLOCA			\
  ptrdiff_t sa_count = SPECPDL_INDEX (); bool sa_must_free = false

/* SAFE_ALLOCA allocates a simple buffer.  */

#define SAFE_ALLOCA(size) (((size) < MAX_ALLOCA)\
			   ? alloca((size_t)(size))	\
			   : (sa_must_free = true, record_xmalloc(size)))

/* SAFE_NALLOCA sets BUF to a newly allocated array of MULTIPLIER *
   NITEMS items, each of the same type as *BUF.  MULTIPLIER must
   positive.  The code is tuned for MULTIPLIER being a constant.  */

#define SAFE_NALLOCA(buf, multiplier, nitems)			 \
  do {								 \
    if ((nitems) <= (MAX_ALLOCA / sizeof(*(buf)) / (multiplier)))\
      (buf) = alloca(sizeof(*(buf)) * (multiplier) * (size_t)(nitems));\
    else							 \
      {								 \
	(buf) = xnmalloc((ptrdiff_t)nitems, (sizeof(*(buf)) * (multiplier))); \
	sa_must_free = true;					 \
	record_unwind_protect_ptr(xfree, buf);			 \
      }								 \
  } while (false)

/* SAFE_FREE frees xmalloced memory and enables GC as needed.  */

#define SAFE_FREE()			\
  do {					\
    if (sa_must_free) {			\
      sa_must_free = false;		\
      unbind_to (sa_count, Qnil);	\
    }					\
  } while (false)

/* SAFE_ALLOCA_LISP allocates an array of Lisp_Objects.  */

#define SAFE_ALLOCA_LISP(buf, nelt)			       \
  do {							       \
    if ((nelt) < MAX_ALLOCA / word_size)		       \
      (buf) = alloca ((nelt) * word_size);		       \
    else if ((nelt) < min (PTRDIFF_MAX, SIZE_MAX) / word_size) \
      {							       \
	Lisp_Object arg_;				       \
	(buf) = xmalloc ((nelt) * word_size);		       \
	arg_ = make_save_memory (buf, nelt);		       \
	sa_must_free = true;				       \
	record_unwind_protect (free_save_value, arg_);	       \
      }							       \
    else						       \
      memory_full (SIZE_MAX);				       \
  } while (false)

/* Loop over all tails of a list, checking for cycles.
   FIXME: Make tortoise and n internal declarations.
   FIXME: Unroll the loop body so we don't need `n'.  */
#define FOR_EACH_TAIL(hare, list, tortoise, n)	\
  for ((tortoise) = (hare) = (list), (n) = true;		\
       CONSP (hare);						\
       (hare = XCDR (hare), (n) = !(n),				\
   	((n)							\
   	 ? (EQ (hare, tortoise)					\
	    ? xsignal1 (Qcircular_list, list)			\
	    : (void) 0)						\
	 /* Move tortoise before the next iteration, in case */ \
	 /* the next iteration does an Fsetcdr.  */		\
   	 : (void) ((tortoise) = XCDR (tortoise)))))

/* Do a `for' loop over alist values.  */

#define FOR_EACH_ALIST_VALUE(head_var, list_var, value_var)		\
  for ((list_var) = (head_var);						\
       (CONSP (list_var) && ((value_var) = XCDR (XCAR (list_var)), true)); \
       (list_var) = XCDR (list_var))

/* Check whether it is time for GC, and run it if so: */
INLINE void
maybe_gc (void)
{
  if ((consing_since_gc > gc_cons_threshold
       && consing_since_gc > gc_relative_threshold)
      || (!NILP (Vmemory_full)
	  && consing_since_gc > memory_full_cons_threshold))
    Fgarbage_collect ();
}

INLINE bool
functionp (Lisp_Object object)
{
  if (SYMBOLP (object) && !NILP (Ffboundp (object)))
    {
      object = Findirect_function (object, Qt);

      if (CONSP (object) && EQ (XCAR (object), Qautoload))
	{
	  /* Autoloaded symbols are functions, except if they load
	     macros or keymaps.  */
	  int i;
	  for (i = 0; i < 4 && CONSP (object); i++)
	    object = XCDR (object);

	  return ! (CONSP (object) && !NILP (XCAR (object)));
	}
    }

  if (SUBRP (object))
    return XSUBR (object)->max_args != UNEVALLED;
  else if (COMPILEDP (object))
    return true;
  else if (CONSP (object))
    {
      Lisp_Object car = XCAR (object);
      return EQ (car, Qlambda) || EQ (car, Qclosure);
    }
  else
    return false;
}

/* keep condition the same as when we push away the redundancy warning: */
#if defined(__GNUC__) && defined(__GNUC_MINOR__) && \
    ((__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6)))
# pragma GCC diagnostic pop
#endif /* gcc 4.6+ */

INLINE_HEADER_END

#endif /* EMACS_LISP_H */

/* arch-tag: 9b2ed020-70eb-47ac-94ee-e1c2a5107d5e
   (do not change this comment) */
