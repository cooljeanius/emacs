# headers_extra.m4 serial 1 file starts here
dnl# This was originally in the emacs configure.ac file.   -*- Autoconf -*-

dnl# Header checks that might be called elsewhere:
AC_DEFUN([AC_REQUIRE_VARIOUS_HEADER_CHECKS],[
  dnl# first just some normal autoconf ones:
  AC_REQUIRE([AC_HEADER_STAT])dnl
  AC_REQUIRE([AC_HEADER_STDC])dnl
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
  m4_ifdef([gl_SCHED_H],[
    AC_REQUIRE([gl_SCHED_H])dnl
  ])dnl#
  m4_ifdef([gl_WARN_ON_USE_PREPARE],[
    m4_ifdef([gl_SIGNAL_H],[
      AC_REQUIRE([gl_SIGNAL_H])dnl
    ])dnl
  ])dnl# end fourth block that requires warn-on-use
  m4_ifdef([gl_CHECK_SOCKET_HEADERS],[
    AC_REQUIRE([gl_CHECK_SOCKET_HEADERS])dnl
  ])dnl
  m4_ifdef([gl_WARN_ON_USE_PREPARE],[
    m4_ifdef([gl_SPAWN_H],[
      AC_REQUIRE([gl_SPAWN_H])dnl
    ])dnl
  ])dnl# end fifth block that requires warn-on-use
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
  ])dnl# end sixth block that requires warn-on-use
  m4_ifdef([gl_HEADER_TIME_H],[
    AC_REQUIRE([gl_HEADER_TIME_H])dnl
  ])dnl
  m4_ifdef([gl_WARN_ON_USE_PREPARE],[
    m4_ifdef([gl_UNISTD_H],[
      AC_REQUIRE([gl_UNISTD_H])dnl
    ])dnl
  ])dnl# end seventh and final block that requires warn-on-use
])dnl# end macro definition

# headers_extra.m4 ends here
