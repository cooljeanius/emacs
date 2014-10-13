# timezone_structs.m4 serial 1 file starts here
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

dnl# struct checks that might be called elsewhere (just timezone ones):
AC_DEFUN([AC_REQUIRE_TIMEZONE_STRUCTS],[
  AC_REQUIRE([AC_STRUCT_TIMEZONE])dnl# also does the "_TM" one
  dnl# however, since we are require-ing, we can do both:
  AC_REQUIRE([AC_STRUCT_TM])dnl
  dnl# and also this:
  AC_REQUIRE([AC_STRUCT_TIMEZONE_DAYLIGHT])dnl
])dnl

# timezone_structs.m4 ends here
