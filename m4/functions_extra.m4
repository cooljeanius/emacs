# functions_extra.m4 serial 1 file starts here
dnl# This was originally in the emacs configure.ac file.   -*- Autoconf -*-

dnl# checks for functions that might be called elsewhere:
AC_DEFUN([AC_REQUIRE_VARIOUS_FUNC_CHECKS],[
  dnl# the normal autoconf error_at_line check makes libobjs, which is bad
  dnl# here, so use the gnulib version of it instead, if available:
  m4_ifdef([gl_ERROR],[
    AC_REQUIRE([gl_ERROR])dnl
  ],[
    AC_REQUIRE([AC_FUNC_ERROR_AT_LINE])dnl
  ])dnl
  dnl# the gnulib check for getgroups will also call the autoconf check
  dnl# for it, but without requiring it, so prefer the gnulib one to try
  dnl# to make sure it only happens once:
  m4_ifdef([gl_FUNC_GETGROUPS],[
    AC_REQUIRE([gl_FUNC_GETGROUPS])dnl
  ],[
    AC_REQUIRE([AC_FUNC_GETGROUPS])dnl
  ])dnl
  dnl# if we have the gnulib check for getloadavg, use that check for it,
  dnl# instead of the standard autoconf one that it says is "obsolescent":
  m4_ifdef([gl_GETLOADAVG],[
    AC_REQUIRE([gl_GETLOADAVG])dnl
  ],[
    AC_REQUIRE([AC_FUNC_GETLOADAVG])dnl
  ])dnl
  AC_REQUIRE([AC_FUNC_GETPGRP])dnl
  AC_REQUIRE([AC_FUNC_OBSTACK])dnl
  dnl# now for other gnulib function checks:
  m4_ifdef([gl_WARN_ON_USE_PREPARE],[
    m4_ifdef([gl_HEADER_SYS_TIME_H],[
      m4_ifdef([gl_FUNC_GETTIMEOFDAY],[
        AC_REQUIRE([gl_FUNC_GETTIMEOFDAY])dnl
      ])dnl
    ])dnl
  ])dnl# end replication of problematic macro call stack
  m4_ifdef([gl_FUNC_POSIX_SPAWN_FILE_ACTIONS_ADDCLOSE],[
    AC_REQUIRE([gl_FUNC_POSIX_SPAWN_FILE_ACTIONS_ADDCLOSE])dnl
  ])dnl
  m4_ifdef([gl_FUNC_POSIX_SPAWN_FILE_ACTIONS_ADDDUP2],[
    AC_REQUIRE([gl_FUNC_POSIX_SPAWN_FILE_ACTIONS_ADDDUP2])dnl
  ])dnl
  m4_ifdef([gl_FUNC_POSIX_SPAWN_FILE_ACTIONS_ADDOPEN],[
    AC_REQUIRE([gl_FUNC_POSIX_SPAWN_FILE_ACTIONS_ADDOPEN])dnl
  ])dnl
  m4_ifdef([gl_FUNC_RAWMEMCHR],[
    AC_REQUIRE([gl_FUNC_RAWMEMCHR])dnl
  ])dnl
  m4_ifdef([gl_FUNC_STRCHRNUL],[
    AC_REQUIRE([gl_FUNC_STRCHRNUL])dnl
  ])dnl
  m4_ifdef([gl_FUNC_GNU_STRFTIME],[
    AC_REQUIRE([gl_FUNC_GNU_STRFTIME])dnl
  ],[
    m4_ifdef([gl_FUNC_STRFTIME],[
      AC_REQUIRE([gl_FUNC_STRFTIME])dnl
    ])dnl
  ])dnl
])dnl

# functions_extra.m4 ends here
