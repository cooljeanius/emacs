# headers_extra.m4 serial 2 file starts here
dnl# This was originally in the emacs configure.ac file.   -*- Autoconf -*-

dnl# Header checks that might be called elsewhere:
AC_DEFUN([AC_REQUIRE_VARIOUS_HEADER_CHECKS],[
  dnl# first just some normal autoconf ones:
  AC_REQUIRE([AC_HEADER_STAT])dnl
  dnl# deprecated in 2.70:
  m4_ifdef([AC_HEADER_STDC],[
    AC_REQUIRE([AC_HEADER_STDC])dnl
  ])dnl
  dnl# now gnulib ones, in alphabetical-ish order:
  m4_ifdef([gl_WARN_ON_USE_PREPARE],[
    m4_ifdef([gl_DIRENT_H],[
      AC_REQUIRE([gl_DIRENT_H])dnl
    ])dnl
  ])dnl# end first block that requires warn-on-use
  m4_ifdef([gl_HEADER_ERRNO_H],[
    AC_REQUIRE([gl_HEADER_ERRNO_H])dnl
  ])dnl
  m4_ifdef([gl_EXECINFO_H],[
    AC_REQUIRE([gl_EXECINFO_H])dnl
  ])dnl
  m4_ifdef([gl_EXTERN_INLINE],[
    AC_REQUIRE([gl_EXTERN_INLINE])dnl
  ])dnl
  m4_ifdef([gl_WARN_ON_USE_PREPARE],[
    m4_ifdef([gl_FCNTL_H],[
      AC_REQUIRE([gl_FCNTL_H])dnl
    ])dnl
  ])dnl# end second block that requires warn-on-use
  m4_ifdef([gl_FCNTL_O_FLAGS],[
    AC_REQUIRE([gl_FCNTL_O_FLAGS])dnl
  ])dnl
  m4_ifdef([gl_FEATURES_H],[
    AC_REQUIRE([gl_FEATURES_H])dnl
  ])dnl
  m4_ifdef([gl_AC_HEADER_INTTYPES_H],[
    AC_REQUIRE([gl_AC_HEADER_INTTYPES_H])dnl
  ])dnl
  m4_ifdef([gl_WARN_ON_USE_PREPARE],[
    m4_ifdef([gl_INTTYPES_H],[
      AC_REQUIRE([gl_INTTYPES_H])dnl
    ])dnl
  ])dnl# end third block that requires warn-on-use
  m4_ifdef([gl_LIMITS_H],[
    AC_REQUIRE([gl_LIMITS_H])dnl
  ])dnl
  m4_ifdef([gl_WARN_ON_USE_PREPARE],[
    m4_ifdef([gl_PTY_H],[
      AC_REQUIRE([gl_PTY_H])dnl
    ])dnl
  ])dnl# end fourth block that requires warn-on-use
  m4_ifdef([gl_SCHED_H],[
    AC_REQUIRE([gl_SCHED_H])dnl
  ])dnl#
  m4_ifdef([gl_WARN_ON_USE_PREPARE],[
    m4_ifdef([gl_SIGNAL_H],[
      AC_REQUIRE([gl_SIGNAL_H])dnl
    ])dnl
  ])dnl# end fifth block that requires warn-on-use
  m4_ifdef([gl_CHECK_SOCKET_HEADERS],[
    AC_REQUIRE([gl_CHECK_SOCKET_HEADERS])dnl
  ])dnl
  m4_ifdef([gl_WARN_ON_USE_PREPARE],[
    m4_ifdef([gl_SPAWN_H],[
      AC_REQUIRE([gl_SPAWN_H])dnl
    ])dnl
  ])dnl# end sixth block that requires warn-on-use
  m4_ifdef([gl_STDALIGN_H],[
    AC_REQUIRE([gl_STDALIGN_H])dnl
  ])dnl
  m4_ifdef([gl_STDBOOL_H],[
    AC_REQUIRE([gl_STDBOOL_H])dnl
  ],[
    m4_ifdef([AM_STDBOOL_H],[
      AC_REQUIRE([AM_STDBOOL_H])dnl
    ],[
      AC_REQUIRE([AC_HEADER_STDBOOL])dnl# also "_CHECK"s it
      dnl# however, since we are require-ing, we can do both:
      AC_REQUIRE([AC_CHECK_HEADER_STDBOOL])dnl
    ])dnl
  ])dnl# end stdbool variants
  m4_ifdef([gl_AC_HEADER_STDINT_H],[
    AC_REQUIRE([gl_AC_HEADER_STDINT_H])dnl
  ])dnl
  m4_ifdef([gl_STDINT_H],[
    AC_REQUIRE([gl_STDINT_H])dnl
  ])dnl
  m4_ifdef([gl_WARN_ON_USE_PREPARE],[
    m4_ifdef([gl_STDIO_H],[
      AC_REQUIRE([gl_STDIO_H])dnl
    ])dnl
    m4_ifdef([gl_STDLIB_H],[
      AC_REQUIRE([gl_STDLIB_H])dnl
    ])dnl
    m4_ifdef([gl_HEADER_STRING_H],[
      AC_REQUIRE([gl_HEADER_STRING_H])dnl
    ])dnl
    m4_ifdef([gl_HEADER_SYS_RESOURCE],[
      AC_REQUIRE([gl_HEADER_SYS_RESOURCE])dnl
    ])dnl
    m4_ifdef([gl_HEADER_SYS_SELECT],[
      AC_REQUIRE([gl_HEADER_SYS_SELECT])dnl
    ])dnl
    m4_ifdef([gl_HEADER_SYS_SOCKET],[
      AC_REQUIRE([gl_HEADER_SYS_SOCKET])dnl
    ])dnl
    m4_ifdef([gl_HEADER_SYS_STAT_H],[
      AC_REQUIRE([gl_HEADER_SYS_STAT_H])dnl
    ])dnl
    m4_ifdef([gl_HEADER_SYS_TIME_H],[
      AC_REQUIRE([gl_HEADER_SYS_TIME_H])dnl
    ])dnl
  ])dnl# end seventh block that requires warn-on-use
  m4_ifdef([gl_HEADER_TIME_H],[
    AC_REQUIRE([gl_HEADER_TIME_H])dnl
  ])dnl
  m4_ifdef([gl_WARN_ON_USE_PREPARE],[
    m4_ifdef([gl_UNISTD_H],[
      AC_REQUIRE([gl_UNISTD_H])dnl
    ])dnl
  ])dnl# end eighth and final block that requires warn-on-use
])dnl# end macro definition

dnl# AC_HEADER_STDC
dnl# Taken from autoconf 2.69, to un-obsolete
dnl# --------------
AC_DEFUN([AC_HEADER_STDC],[
AC_REQUIRE([AC_PROG_EGREP])dnl
AC_CACHE_CHECK([for ANSI C header files],[ac_cv_header_stdc],
[AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <float.h>
]])],
		    [ac_cv_header_stdc=yes],
		    [ac_cv_header_stdc=no])dnl

if test "x${ac_cv_header_stdc}" = "xyes"; then
  # SunOS 4.x string.h does not declare mem*, contrary to ANSI.
  AC_EGREP_HEADER([memchr],[string.h],[],[ac_cv_header_stdc=no])
fi

if test "x${ac_cv_header_stdc}" = "xyes"; then
  # ISC 2.0.2 stdlib.h does not declare free, contrary to ANSI.
  AC_EGREP_HEADER([free],[stdlib.h],[],[ac_cv_header_stdc=no])
fi

if test "x${ac_cv_header_stdc}" = "xyes"; then
  # /bin/cc in Irix-4.0.5 gets non-ANSI ctype macros unless using -ansi.
  AC_RUN_IFELSE([AC_LANG_SOURCE(
[[#include <ctype.h>
#include <stdlib.h>
#if ((' ' & 0x0FF) == 0x020)
# define ISLOWER(c) ('a' <= (c) && (c) <= 'z')
# define TOUPPER(c) (ISLOWER(c) ? 'A' + ((c) - 'a') : (c))
#else
# define ISLOWER(c) \
		   (('a' <= (c) && (c) <= 'i') \
		     || ('j' <= (c) && (c) <= 'r') \
		     || ('s' <= (c) && (c) <= 'z'))
# define TOUPPER(c) (ISLOWER(c) ? ((c) | 0x40) : (c))
#endif

#define XOR(e, f) (((e) && !(f)) || (!(e) && (f)))
int
main(void)
{
  int i;
  for (i = 0; i < 256; i++)
    if (XOR(islower(i), ISLOWER(i))
	|| (toupper(i) != TOUPPER(i)))
      return 2;
  return 0;
}]])],[],[ac_cv_header_stdc=no],[:])
fi])
if test "x${ac_cv_header_stdc}" = "xyes"; then
  AC_DEFINE([STDC_HEADERS],[1],
	    [Define to 1 if you have the ANSI C header files.])
fi
])dnl# end definition of AC_HEADER_STDC

# headers_extra.m4 ends here
