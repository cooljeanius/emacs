# timezone_structs.m4 serial 2 file starts here
dnl# This was originally in the emacs configure.ac file.   -*- Autoconf -*-
dnl# Well, the latter part, at least.
dnl# First, though:

dnl# from autoconf 2.13 acspecific.m4 by way of the file/libmagic
dnl# acinclude.m4, with changes to check for daylight:

AC_DEFUN([AC_STRUCT_TIMEZONE_DAYLIGHT],
[AC_PREREQ([2.13])dnl
AC_REQUIRE([AC_STRUCT_TM])dnl
AC_CACHE_CHECK([for tm_zone in struct tm],[ac_cv_struct_tm_zone],
[AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
#include <sys/types.h>
#include <${ac_cv_struct_tm}>
]],[[
struct tm tm; tm.tm_zone;
]])],
  [ac_cv_struct_tm_zone=yes],[ac_cv_struct_tm_zone=no])
])
if test "x${ac_cv_struct_tm_zone}" = "xyes"; then
  AC_DEFINE([HAVE_TM_ZONE],[1],[HAVE_TM_ZONE])
fi

# On SGI, apparently tzname is a #define, but that is ok,
# AC_CHECK_DECL will consider it declared and we will not give our
# own extern.
AC_CHECK_HEADERS_ONCE([time.h])dnl
AC_CHECK_DECLS([tzname],[],[],[#include <time.h>])dnl
AC_CACHE_CHECK([for tzname],[ac_cv_var_tzname],
[AC_LINK_IFELSE([AC_LANG_SOURCE([[
#include <time.h>
#if !HAVE_DECL_TZNAME
extern char *tzname[];
#endif /* !HAVE_DECL_TZNAME */
]],[[
return tzname[0][0];
]])],
  [ac_cv_var_tzname=yes],[ac_cv_var_tzname=no])
])
if test "x${ac_cv_var_tzname}" = "xyes"; then
  AC_DEFINE([HAVE_TZNAME],[1],[HAVE_TZNAME])
fi

AC_CACHE_CHECK([for tm_isdst in struct tm],[ac_cv_struct_tm_isdst],
[AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
#include <sys/types.h>
#include <${ac_cv_struct_tm}>
]],[[
struct tm tm; tm.tm_isdst;
]])],
  [ac_cv_struct_tm_isdst=yes],[ac_cv_struct_tm_isdst=no])
])
if test "x${ac_cv_struct_tm_isdst}" = "xyes"; then
  AC_DEFINE([HAVE_TM_ISDST],[1],[HAVE_TM_ISDST])
fi

AC_CHECK_DECLS([daylight],[],[],[#include <time.h>])dnl
AC_CACHE_CHECK([for daylight],[ac_cv_var_daylight],
[AC_LINK_IFELSE([AC_LANG_SOURCE([[
#include <time.h>
#if !HAVE_DECL_DAYLIGHT
extern int daylight;
#endif /* !HAVE_DECL_DAYLIGHT */
]],[[
atoi(daylight);
]])],
  [ac_cv_var_daylight=yes],[ac_cv_var_daylight=no])
])
  if test "x${ac_cv_var_daylight}" = "xyes"; then
    AC_DEFINE([HAVE_DAYLIGHT],[1],[HAVE_DAYLIGHT])
  fi
])dnl

dnl# Some systems have <utime.h>, but do NOT declare the struct anyplace.
AC_DEFUN([EMACS_STRUCT_UTIMBUF],[
  AC_CACHE_CHECK([for struct utimbuf],[emacs_cv_struct_utimbuf],[
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#ifdef TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# ifdef HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  ifdef HAVE_TIME_H
#   include <time.h>
#  else
#   if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#    warning "The conftest for struct utimbuf needs a time-related header."
#   endif /* __GNUC__ && !__STRICT_ANSI__ */
#  endif /* HAVE_TIME_H */
# endif /* HAVE_SYS_TIME_H */
#endif /* TIME_WITH_SYS_TIME */
#ifdef HAVE_UTIME_H
# include <utime.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "The conftest for struct utimbuf wants to include <utime.h>."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_UTIME_H */
  ]],[[static struct utimbuf x; x.actime = x.modtime;]])],
    [emacs_cv_struct_utimbuf=yes],[emacs_cv_struct_utimbuf=no])])
  if test "x${emacs_cv_struct_utimbuf}" = "xyes"; then
    AC_DEFINE([HAVE_STRUCT_UTIMBUF],[1],
              [Define to 1 if `struct utimbuf' is declared by <utime.h>.])
  fi
])dnl

AC_DEFUN([EMACS_STRUCT_TIMEVAL],[
  AC_CACHE_CHECK([for struct timeval],[emacs_cv_struct_timeval],[
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#ifdef TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# ifdef HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  ifdef HAVE_TIME_H
#   include <time.h>
#  else
#   if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#    warning "The conftest for struct timeval needs a time-related header."
#   endif /* __GNUC__ && !__STRICT_ANSI__ */
#  endif /* HAVE_TIME_H */
# endif /* HAVE_SYS_TIME_H */
#endif /* TIME_WITH_SYS_TIME */
  ]],[[static struct timeval x; x.tv_sec = x.tv_usec;]])],
    [emacs_cv_struct_timeval=yes],[emacs_cv_struct_timeval=no])])
  HAVE_TIMEVAL="${emacs_cv_struct_timeval}"
  if test "x${emacs_cv_struct_timeval}" = "xyes"; then
    AC_DEFINE([HAVE_TIMEVAL],[1],
              [Define to 1 if <sys/time.h> declares `struct timeval'.])
  fi
])dnl

dnl# struct checks that might be called elsewhere (just time(zone) ones):
AC_DEFUN([AC_REQUIRE_TIMEZONE_STRUCTS],[
  AC_REQUIRE([AC_STRUCT_TIMEZONE])dnl# also does the "_TM" one
  dnl# however, since we are require-ing, we can do both:
  AC_REQUIRE([AC_STRUCT_TM])dnl
  dnl# and also this:
  AC_REQUIRE([AC_STRUCT_TIMEZONE_DAYLIGHT])dnl
  dnl# and some old, unrelated ones:
  AC_REQUIRE([EMACS_STRUCT_UTIMBUF])dnl
  AC_REQUIRE([EMACS_STRUCT_TIMEVAL])dnl
])dnl

# timezone_structs.m4 ends here
