/* conf_post.h --- configure.ac includes this via AH_BOTTOM

Copyright (C) 1988, 1993-1994, 1999-2002, 2004-2014 Free Software
Foundation, Inc.

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

/* Commentary:

   Rather than writing this code directly in AH_BOTTOM, we include it
   via this file.  This is so that it does not get processed by
   autoheader.  E.g., any undefs here would otherwise be commented out.
*/

#ifndef EMACS_CONF_POST_H
#define EMACS_CONF_POST_H 1

/* Code: */

/* Include any platform specific configuration file.  */
#ifdef config_opsysfile
# include config_opsysfile
#endif /* config_opsysfile */

#include <stdbool.h>

/* The pre-C99 <stdbool.h> emulation does NOT work for bool bitfields.
   Nor does compiling Objective-C with standard GCC.  */
#if (__STDC_VERSION__ < 199901) || (defined(NS_IMPL_GNUSTEP) && NS_IMPL_GNUSTEP)
typedef unsigned int bool_bf;
#else
typedef bool bool_bf;
#endif /* pre-c99 || NS_IMPL_GNUSTEP */

#ifndef WINDOWSNT
/* On AIX 3 this must be included before any other include file.  */
# include <alloca.h>
# if ! HAVE_ALLOCA
#  error "alloca not available on this machine"
# endif /* !HAVE_ALLOCA */
#endif /* !WINDOWSNT */

/* When not using Clang, assume its attributes and features are absent.  */
#ifndef __has_attribute
# if defined(_STDBOOL_H_) && defined(__bool_true_false_are_defined)
/* "false" does not work as a valid preprocessor expression when it is a
 * macro whose expansion contains a cast: */
#  define __has_attribute(a) 0
# else
#  define __has_attribute(a) false
# endif /* _STDBOOL_H_ */
#endif /* !__has_attribute */
#ifndef __has_feature
# if defined(_STDBOOL_H_) && defined(__bool_true_false_are_defined)
/* "false" does not work as a valid preprocessor expression when it is a
 * macro whose expansion contains a cast: */
#  define __has_feature(a) 0
# else
#  define __has_feature(a) false
# endif /* _STDBOOL_H_ */
#endif /* !__has_feature */

/* True if addresses are being sanitized: */
#ifndef ADDRESS_SANITIZER
# if defined __SANITIZE_ADDRESS__ || __has_feature(address_sanitizer)
#  define ADDRESS_SANITIZER true
# else
#  if defined(_STDBOOL_H_) && defined(__bool_true_false_are_defined)
/* "false" does not work as a valid preprocessor expression when it is a
 * macro whose expansion contains a cast: */
#   define ADDRESS_SANITIZER 0
#  else
#   define ADDRESS_SANITIZER false
#  endif /* _STDBOOL_H_ && __bool_true_false_are_defined */
# endif /* ASAN */
#endif /* !ADDRESS_SANITIZER */

#ifdef DARWIN_OS
/* in case "config.h" failed to define this: */
# ifndef MAC_OSX
#  if (defined(__APPLE__) && defined(__MACH__)) || \
      defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__)
#   define MAC_OSX 10
#  endif /* (__APPLE__ && __MACH__) || __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ */
# endif /* !MAC_OSX */
# ifdef emacs
#  define malloc unexec_malloc
#  define realloc unexec_realloc
#  define free unexec_free
# endif /* emacs */
/* The following solves the problem that Emacs hangs when evaluating
   (make-comint "test0" "/nodir/nofile" nil "") when /nodir/nofile
   does not exist.  Also, setsid is not allowed in the vfork child's
   context as of Darwin 9/Mac OS X 10.5.  */
# undef HAVE_WORKING_VFORK
# define vfork fork
#endif  /* DARWIN_OS */

/* We have to go this route, rather than the old hpux9 approach of
   renaming the functions via macros.  The system's stdlib.h has fully
   prototyped declarations, which yields a conflicting definition of
   srand48; it tries to redeclare what was once srandom to be srand48.
   So we go with HAVE_LRAND48 being defined.  */
#ifdef HPUX
# undef srandom
# undef random
/* We try to avoid checking for random and rint on hpux in
 * configure.ac, but some other configure test might check for them as
 * a dependency, so to be safe we also undefine them here. */
# undef HAVE_RANDOM
# undef HAVE_RINT
#endif  /* HPUX */

#ifdef IRIX6_5
# ifdef emacs
char *_getpty();
# endif /* emacs */

#endif /* IRIX6_5 */

#ifdef MSDOS
# ifndef __DJGPP__
You lose; /* Emacs for DOS must be compiled with DJGPP */
# endif /* !__DJGPP__ */
# define _NAIVE_DOS_REGS

/* Start of gnulib-related stuff  */

/* lib/ftoastr.c wants strtold, but DJGPP only has _strtold.  DJGPP >
   2.03 has it, but it also has _strtold as a stub that jumps to
   strtold, so use _strtold in all versions.  */
# define strtold _strtold

# if (__DJGPP__ > 2) || (__DJGPP_MINOR__ > 3)
#  define HAVE_LSTAT 1
# else
#  define lstat stat
# endif /* DJGPP version check */
/* The "portable" definition of _GL_INLINE on config.h does not work
   with DJGPP GCC 3.4.4: it causes unresolved externals in sysdep.c,
   although lib/execinfo.h is included and the inline functions there
   are visible.  */
# if (__GNUC__ < 4)
#  define _GL_EXECINFO_INLINE inline
# endif /* old gcc version */
/* End of gnulib-related stuff.  */

# define emacs_raise(sig) msdos_fatal_signal (sig)

/* Define one of these for easier conditionals.  */
# ifdef HAVE_X_WINDOWS
/* We need a little extra space, see ../../lisp/loadup.el and the
   commentary below, in the non-X branch.  The 140KB number was
   measured on GNU/Linux and on MS-Windows.  */
#  define SYSTEM_PURESIZE_EXTRA (-170000+140000)
# else
/* We need a little extra space, see ../../lisp/loadup.el.
   As of 20091024, DOS-specific files use up 62KB of pure space.  But
   overall, we end up wasting 130KB of pure space, because
   BASE_PURESIZE starts at 1.47MB, while we need only 1.3MB (including
   non-DOS specific files and load history; the latter is about 55K,
   but depends on the depth of the top-level Emacs directory in the
   directory tree).  Given the unknown policy of different DPMI
   hosts regarding loading of untouched pages, I'm not going to risk
   enlarging Emacs footprint by another 100+ KBytes.  */
#  define SYSTEM_PURESIZE_EXTRA (-170000+65000)
# endif /* HAVE_X_WINDOWS */
#endif  /* MSDOS */

/* If we are using the Carbon API on Mac OS X, define a few more
 * variables as well.  */
#ifdef HAVE_CARBON
# ifndef HAVE_WINDOW_SYSTEM
#  define HAVE_WINDOW_SYSTEM
# endif /* !HAVE_WINDOW_SYSTEM */
# ifndef HAVE_MOUSE
#  define HAVE_MOUSE
# endif /* !HAVE_MOUSE */
#endif /* HAVE_CARBON */

/* Mac OS X / GNUstep need a bit more pure memory.  Of the existing knobs,
   SYSTEM_PURESIZE_EXTRA seems like the least likely to cause problems.  */
#ifdef HAVE_NS
# if defined NS_IMPL_GNUSTEP
#  define SYSTEM_PURESIZE_EXTRA 30000
# elif defined DARWIN_OS
#  define SYSTEM_PURESIZE_EXTRA 200000
# endif /* NS_IMPL_GNUSTEP || DARWIN_OS */
#endif /* HAVE_NS */

#if defined HAVE_NTGUI && !defined DebPrint
# ifdef EMACSDEBUG
extern void _DebPrint (const char *fmt, ...);
#  define DebPrint(stuff) _DebPrint stuff
# else
#  define DebPrint(stuff)
# endif /* EMACSDEBUG */
#endif /* HAVE_NTGUI && !DebPrint */

#if defined CYGWIN && defined HAVE_NTGUI
# define NTGUI_UNICODE /* Cygwin runs only on UNICODE-supporting systems */
# define _WIN32_WINNT 0x500 /* Win2k */
#endif /* CYGWIN && HAVE_NTGUI */

#ifdef emacs /* Do NOT do this for lib-src.  */
/* Tell regex.c to use a type compatible with Emacs.  */
# define RE_TRANSLATE_TYPE Lisp_Object
# define RE_TRANSLATE(TBL, C) char_table_translate(TBL, (int)(C))
# define RE_TRANSLATE_P(TBL) (!EQ(TBL, make_number((EMACS_INT)0)))
#endif /* emacs */

#ifdef HAVE_STRING_H
# include <string.h>
#endif /* HAVE_STRING_H */
#include <stdlib.h>

#if (__GNUC__ >= 3)  /* On GCC 3.0 we might get a warning.  */
# define NO_INLINE __attribute__((noinline))
#else
# define NO_INLINE
#endif /* gcc 3+ */

#if ((defined(__clang__) && __clang__)					\
     ? __has_attribute (externally_visible)				\
     : (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 1)))
# define EXTERNALLY_VISIBLE __attribute__((externally_visible))
#else
# define EXTERNALLY_VISIBLE
#endif /* __clang__ || gcc 4.1+ */

/* Added in gcc 7: */
#ifndef ATTRIBUTE_FALLTHROUGH
# if (__GNUC__ >= 7)
#  define ATTRIBUTE_FALLTHROUGH __attribute__((fallthrough))
# else
#  define ATTRIBUTE_FALLTHROUGH /* FALLTHRU */
# endif /* gcc 7+ */
#endif /* !ATTRIBUTE_FALLTHROUGH */

#if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 7)
# define ATTRIBUTE_FORMAT(spec) __attribute__ ((__format__ spec))
#else
# define ATTRIBUTE_FORMAT(spec) /* empty */
#endif /* gcc 2.7+ */

#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 4)
# define ATTRIBUTE_FORMAT_PRINTF(formatstring_parameter, first_argument) \
   ATTRIBUTE_FORMAT ((__gnu_printf__, formatstring_parameter, first_argument))
#else
# define ATTRIBUTE_FORMAT_PRINTF(formatstring_parameter, first_argument) \
   ATTRIBUTE_FORMAT ((__printf__, formatstring_parameter, first_argument))
#endif /* gcc 4.4+ */

/* Attribute `nonnull' was valid as of gcc 3.3: */
#ifndef ATTRIBUTE_NONNULL
# if __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 3)
#  define ATTRIBUTE_NONNULL(m) __attribute__((__nonnull__(m)))
# else
#  define ATTRIBUTE_NONNULL(m)
# endif /* GNUC >= 3.3 */
#endif /* ATTRIBUTE_NONNULL */

#ifndef ATTRIBUTE_USED
# if __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4)
#  define ATTRIBUTE_USED __attribute__((__used__))
# else
#  define ATTRIBUTE_USED
# endif /* GNUC >= 3.3 */
#endif /* ATTRIBUTE_USED */

#define ATTRIBUTE_CONST _GL_ATTRIBUTE_CONST

/* Work around GCC bug 59600: when a function is inlined, the inlined
   code may have its addresses sanitized even if the function has the
   no_sanitize_address attribute.  This bug is present in GCC 4.8.2
   and clang 3.3, the latest releases as of December 2013, and the
   only platforms known to support address sanitization.  When the bug
   is fixed the #if can be updated accordingly.  */
#if defined(ADDRESS_SANITIZER) && ADDRESS_SANITIZER
# define ADDRESS_SANITIZER_WORKAROUND NO_INLINE
#else
# define ADDRESS_SANITIZER_WORKAROUND
#endif /* ADDRESS_SANITIZER */

/* Attribute of functions whose code should not have addresses
   sanitized.  */
#if (__has_attribute (no_sanitize_address) \
     || 4 < __GNUC__ + (8 <= __GNUC_MINOR__))
# define ATTRIBUTE_NO_SANITIZE_ADDRESS \
    __attribute__ ((no_sanitize_address)) ADDRESS_SANITIZER_WORKAROUND
#elif __has_attribute (no_address_safety_analysis)
# define ATTRIBUTE_NO_SANITIZE_ADDRESS \
    __attribute__ ((no_address_safety_analysis)) ADDRESS_SANITIZER_WORKAROUND
#else
# define ATTRIBUTE_NO_SANITIZE_ADDRESS
#endif /* misc. */

/* Some versions of GNU/Linux define noinline in their headers.  */
#ifdef noinline
# undef noinline
#endif /* noinline */

/* Use Gnulib's extern-inline module for extern inline functions.
   An include file foo.h should prepend FOO_INLINE to function
   definitions, with the following overall pattern:

      [#include any other .h files first.]
      ...
      INLINE_HEADER_BEGIN
      ...
      INLINE int
      incr (int i)
      {
        return i + 1;
      }
      ...
      INLINE_HEADER_END

   For every executable, exactly one file that includes the header
   should do this:

      #define INLINE EXTERN_INLINE

   before including config.h or any other .h file.
   Other .c files should not define INLINE.

   C99 compilers compile functions like 'incr' as C99-style extern
   inline functions.  Pre-C99 GCCs do something similar with
   GNU-specific keywords.  Pre-C99 non-GCC compilers use static
   functions, which bloats the code but is good enough.  */

#ifndef INLINE
# define INLINE _GL_INLINE
#endif /* !INLINE */
#define EXTERN_INLINE _GL_EXTERN_INLINE
#define INLINE_HEADER_BEGIN _GL_INLINE_HEADER_BEGIN
#define INLINE_HEADER_END _GL_INLINE_HEADER_END

/* To use the struct hack with N elements, declare the struct like this:
     struct s { ...; t name[FLEXIBLE_ARRAY_MEMBER]; };
   and allocate (offsetof (struct s, name) + N * sizeof (t)) bytes.  */
#ifndef FLEXIBLE_ARRAY_MEMBER
# if 199901 <= __STDC_VERSION__
#  define FLEXIBLE_ARRAY_MEMBER
# elif (defined(__GNUC__) && __GNUC__) && !defined __STRICT_ANSI__
#  define FLEXIBLE_ARRAY_MEMBER 0
# else
#  define FLEXIBLE_ARRAY_MEMBER 1
# endif /* pre-c99 || GNU, non-ANSI C || neither */
#endif /* !FLEXIBLE_ARRAY_MEMBER */

/* Use this to suppress gcc's `...may be used before initialized' warnings. */
#ifdef lint
/* Use CODE only if lint checking is in effect.  */
# define IF_LINT(Code) Code
#else
# define IF_LINT(Code) /* empty */
#endif /* lint */

/* poison some unwanted functions: */
#if (defined(__GNUC__) && defined(__GNUC_MINOR__) && (__GNUC__ >= 3)) && \
    !defined(NO_POISON) && !defined(FLEX_SCANNER) && !defined(_IN_GNULIB)
/* Include this first to avoid errors: */
# if defined(HAVE_STDIO_H) || defined(STDC_HEADERS) || defined(__STDC__)
#  include <stdio.h>
# endif /* HAVE_STDIO_H || STDC_HEADERS || __STDC__ */
# ifdef sprintf
#  undef sprintf
# endif /* sprintf */
# ifdef vsprintf
#  undef vsprintf
# endif /* vsprintf */
# ifdef strdup
#  undef strdup
# endif /* strdup */
# ifdef strndup
#  undef strndup
# endif /* strndup */
/* I also avoid these ones in the gdb sources: */
# pragma GCC poison strdup sprintf
/* for similar reasons, such as gnulib also providing replacements: */
# pragma GCC poison strndup memdup vsprintf vasprintf
/* also consider poisoining for similar reasons: asprintf atexit exit */
# if defined(HAVE_STRLCPY) && defined(PREFER_BSDISMS)
#  pragma GCC poison strcpy
# endif /* HAVE_STRLCPY && PREFER_BSDISMS */
# if defined(HAVE_STRLCAT) && defined(PREFER_BSDISMS)
#  pragma GCC poison strcat
# endif /* HAVE_STRLCAT && PREFER_BSDISMS */
# if defined(HAVE_FGETS)
#  pragma GCC poison gets
# endif /* HAVE_FGETS */
# if defined(HAVE_STRTOK_R) && defined(_REENTRANT)
#  pragma GCC poison strtok
# endif /* HAVE_STRTOK_R && _REENTRANT */
#endif /* gcc3+ && !NO_POISON && !FLEX_SCANNER && !_IN_GNULIB */

#endif /* !EMACS_CONF_POST_H */

/* conf_post.h ends here */
