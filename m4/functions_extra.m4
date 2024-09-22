# functions_extra.m4 serial 3 file starts here
dnl# This was originally in the emacs configure.ac file.   -*- Autoconf -*-

dnl# checks for functions that might be called elsewhere:
AC_DEFUN([AC_REQUIRE_VARIOUS_FUNC_CHECKS],[dnl
  ## on to actual modules:
  m4_ifdef([gl_FUNC_CLOSEDIR],[dnl
    AC_REQUIRE([gl_FUNC_CLOSEDIR])dnl
  ],[dnl
    AC_REQUIRE([AC_FUNC_CLOSEDIR_VOID])dnl
    ## keep things like autoscan (and my OCD) happy:
    if test "x${ac_cv_func_closedir_void}" = "x" && test "x${ac_cv_func_closedir}" = "x"; then
      test -z "${ac_cv_func_closedir_void}" && test -z "${ac_cv_func_closedir}"
      AC_CHECK_FUNCS([closedir])
    fi  ##end check for closedir function check cache values
  ])dnl
  dnl# the normal autoconf error_at_line check makes libobjs, which is bad
  dnl# here, so use the gnulib version of it instead, if available:
  m4_ifdef([gl_ERROR],[dnl
    AC_REQUIRE([gl_ERROR])dnl
  ],[dnl
    AC_REQUIRE([AC_FUNC_ERROR_AT_LINE])dnl
  ])dnl
  m4_ifdef([gl_FUNC_FCHDIR],[dnl
    AC_REQUIRE([gl_FUNC_FCHDIR])dnl
    gl_UNISTD_MODULE_INDICATOR([fchdir])dnl
  ])dnl
  AC_MSG_NOTICE([note that require-ing may lead to checks being done out of order])
  dnl# the gnulib check for getgroups will also call the autoconf check
  dnl# for it, but without requiring it, so prefer the gnulib one to try
  dnl# to make sure it only happens once:
  m4_ifdef([gl_FUNC_GETGROUPS],[ ## have gnulib macro for getgroups
    AC_MSG_NOTICE([using (used?) the gnulib check for getgroups])
    AC_REQUIRE([AC_TYPE_UID_T])dnl# (note that m4/override.m4 may have overridden this)
    m4_ifdef([AC_TYPE_GETGROUPS],[dnl
      AC_REQUIRE([AC_TYPE_GETGROUPS])dnl
    ])dnl
    AC_REQUIRE([gl_FUNC_GETGROUPS])dnl
    m4_ifdef([gl_CONDITIONAL],[dnl
      gl_CONDITIONAL([GL_COND_OBJ_GETGROUPS],
                     [test ${HAVE_GETGROUPS} = 0 || \
                      test ${REPLACE_GETGROUPS} = 1])
    ])dnl
    gl_UNISTD_MODULE_INDICATOR([getgroups])dnl
  ],[ ## fall back to regular autoconf macro for getgroups
    AC_MSG_NOTICE([using (used?) the standard autoconf check for getgroups])
    AC_REQUIRE([AC_FUNC_GETGROUPS])dnl
  ])dnl
  dnl# if we have the gnulib check for getloadavg, use that check for it,
  dnl# instead of the standard autoconf one that it says is "obsolescent":
  m4_ifdef([gl_GETLOADAVG],[dnl
    AC_REQUIRE([gl_GETLOADAVG])dnl
  ],[dnl
    AC_REQUIRE([AC_FUNC_GETLOADAVG])dnl
  ])dnl
  AC_REQUIRE([AC_FUNC_GETMNTENT])dnl
  AC_REQUIRE([AC_FUNC_GETPGRP])dnl
  m4_ifdef([gl_FUNC_GETPROGNAME],[dnl
    AC_REQUIRE([gl_FUNC_GETPROGNAME])dnl
  ])dnl
  m4_ifdef([gl_WARN_ON_USE_PREPARE],[dnl
    m4_ifdef([gl_FUNC_IOCTL],[dnl
      AC_REQUIRE([gl_FUNC_IOCTL])dnl
    ])dnl
  ])dnl
  m4_ifdef([gl_FUNC_LDEXP],[dnl
    AC_REQUIRE([gl_FUNC_LDEXP])dnl
  ])dnl
  m4_ifdef([gl_FUNC_ISNANL],[dnl
    AC_REQUIRE([gl_FUNC_ISNANL])dnl
    m4_ifdef([gl_FUNC_LDEXPL],[dnl
      AC_REQUIRE([gl_FUNC_LDEXPL])dnl
    ])dnl
  ])dnl
  m4_ifdef([gl_FUNC_LSEEK],[dnl
    AC_REQUIRE([gl_FUNC_LSEEK])dnl
  ],[dnl
    AC_CHECK_FUNCS([lseek])dnl
  ])dnl
  m4_ifdef([gl_FUNC_LSTAT],[dnl
    AC_REQUIRE([gl_FUNC_LSTAT])dnl# also checks the next one:
    AC_REQUIRE([gl_FUNC_LSTAT_FOLLOWS_SLASHED_SYMLINK])dnl
    dnl# (since we are require-ing, we can do both)
  ],[dnl
    AC_REQUIRE([AC_FUNC_LSTAT])dnl# also checks the next one:
    AC_REQUIRE([AC_FUNC_LSTAT_FOLLOWS_SLASHED_SYMLINK])dnl
    dnl# (since we are require-ing, we can do both)
  ])dnl
  m4_ifdef([gl_FUNC_MBRTOWC],[
    AC_REQUIRE([gl_FUNC_MBRTOWC])dnl
  ],[dnl
    AC_REQUIRE([AC_FUNC_MBRTOWC])dnl
  ])dnl
  m4_ifdef([gl_FUNC_MEMCHR],[ ## have gnulib macro for memchr
    AC_REQUIRE([gl_FUNC_MEMCHR])dnl
    ## in case the function itself never actually got checked:
    if test "x${ac_cv_func_memchr}" = "x"; then
      test -z "${ac_cv_func_memchr}" || unset ac_cv_func_memchr
      AC_CHECK_FUNCS([memchr])
    fi  ##end check for memchr function check cache value
  ],[dnl
    AC_CHECK_FUNCS([memchr])dnl
  ])dnl
  m4_ifdef([gl_FUNC_MKDIR],[
    AC_REQUIRE([gl_FUNC_MKDIR])dnl
  ],[dnl
    AC_CHECK_FUNCS([mkdir])dnl
  ])dnl
  m4_ifdef([gl_FUNC_MKTIME_INTERNAL],[ ## begin gnulib mktime section
    ## case where we have the internal one:
    AC_REQUIRE([gl_FUNC_MKTIME_INTERNAL])dnl
    ## same as below:
    if test "x${gl_cv_func_working_mktime}" = "xno"; then
      AC_DEFINE([BROKEN_MKTIME],[1],
                [Define to 1 if the mktime function is broken.])
    elif test "x${gl_cv_func_working_mktime}" = "x" && test "x${gl_cv_func_mktime}" = "x"; then
      test -z "${gl_cv_func_working_mktime}" && test -z "${gl_cv_func_mktime}"
      AC_FUNC_MKTIME
      if test "x${ac_cv_func_working_mktime}" = "xno"; then
        AC_DEFINE([BROKEN_MKTIME],[1],
                  [Define to 1 if the mktime function is broken.])
      elif test "x${ac_cv_func_working_mktime}" = "x" && test "x${ac_cv_func_mktime}" = "x"; then
        test -z "${ac_cv_func_working_mktime}" && test -z "${ac_cv_func_mktime}"
        AC_CHECK_FUNCS([mktime])
      fi  ##end check for autoconf mktime cache values
    fi  ##end check for gnulib mktime cache values
  ],[dnl
    dnl# the internal mktime gnulib macro would have called this normally,
    dnl# but just in case we have this and not the other:
    m4_ifdef([gl_FUNC_MKTIME],[dnl
      ## gnulib mktime checks:
      AC_REQUIRE([gl_FUNC_MKTIME])dnl
      ## pretty much the same as below, but with "gl" instead of "ac" at
      ## first, and then only the "ac" version if necessary:
      if test "x${gl_cv_func_working_mktime}" = "xno"; then
        AC_DEFINE([BROKEN_MKTIME],[1],
                  [Define to 1 if the mktime function is broken.])
      elif test "x${gl_cv_func_working_mktime}" = "x" && test "x${gl_cv_func_mktime}" = "x"; then
        test -z "${gl_cv_func_working_mktime}" && test -z "${gl_cv_func_mktime}"
        AC_FUNC_MKTIME
        if test "x${ac_cv_func_working_mktime}" = "xno"; then
          AC_DEFINE([BROKEN_MKTIME],[1],
                    [Define to 1 if the mktime function is broken.])
        elif test "x${ac_cv_func_working_mktime}" = "x" && test "x${ac_cv_func_mktime}" = "x"; then
          test -z "${ac_cv_func_working_mktime}" && test -z "${ac_cv_func_mktime}"
          AC_CHECK_FUNCS([mktime])
        fi  ##end check for autoconf mktime cache values
      fi  ##end check for gnulib mktime cache values
    ],[dnl
      ## fall back to autoconf's default mktime checks:
      AC_REQUIRE([AC_FUNC_MKTIME])dnl
      ## moved here from the main configure.ac file:
      if test "x${ac_cv_func_working_mktime}" = "xno"; then
        AC_DEFINE([BROKEN_MKTIME],[1],
                  [Define to 1 if the mktime function is broken.])
      elif test "x${ac_cv_func_working_mktime}" = "x" && test "x${ac_cv_func_mktime}" = "x"; then
        test -z "${ac_cv_func_working_mktime}" && test -z "${ac_cv_func_mktime}"
        AC_CHECK_FUNCS([mktime])
      fi  ##end check for autoconf mktime cache values
    ])dnl
  ])dnl
  AC_REQUIRE([gl_FUNC_OBSTACK])dnl
  AC_REQUIRE([AC_FUNC_OBSTACK])dnl
  dnl# now for other gnulib function checks:
  m4_ifdef([gl_WARN_ON_USE_PREPARE],[dnl
    m4_ifdef([gl_HEADER_SYS_TIME_H],[dnl
      m4_ifdef([gl_FUNC_GETTIMEOFDAY],[dnl
        AC_REQUIRE([gl_FUNC_GETTIMEOFDAY])dnl
      ])dnl
    ])dnl
    m4_ifdef([gl_FUNC_OPENPTY],[dnl
      AC_REQUIRE([gl_FUNC_OPENPTY])dnl
    ])dnl
  ])dnl# end replication of problematic macro call stack
  m4_ifdef([gl_FUNC_PERROR],[dnl
    AC_REQUIRE([gl_FUNC_PERROR])dnl
  ])dnl
  m4_ifdef([gl_FUNC_POSIX_SPAWN_FILE_ACTIONS_ADDCLOSE],[dnl
    AC_REQUIRE([gl_FUNC_POSIX_SPAWN_FILE_ACTIONS_ADDCLOSE])dnl
  ])dnl
  m4_ifdef([gl_FUNC_POSIX_SPAWN_FILE_ACTIONS_ADDDUP2],[dnl
    AC_REQUIRE([gl_FUNC_POSIX_SPAWN_FILE_ACTIONS_ADDDUP2])dnl
  ])dnl
  m4_ifdef([gl_FUNC_POSIX_SPAWN_FILE_ACTIONS_ADDOPEN],[dnl
    AC_REQUIRE([gl_FUNC_POSIX_SPAWN_FILE_ACTIONS_ADDOPEN])dnl
  ])dnl
  m4_ifdef([gl_FUNC_RAWMEMCHR],[dnl
    AC_REQUIRE([gl_FUNC_RAWMEMCHR])dnl
  ])dnl
  m4_ifdef([gl_FUNC_READ],[dnl
    AC_REQUIRE([gl_FUNC_READ])dnl
  ])dnl
  dnl# we override this, but still need to subst something from the
  dnl# overriding version:
  m4_ifdef([gl_FUNC_SELECT],[dnl
    AC_REQUIRE([gl_FUNC_SELECT])dnl
  ])dnl
  m4_ifdef([gl_FUNC_STRCHRNUL],[dnl
    AC_REQUIRE([gl_FUNC_STRCHRNUL])dnl
  ])dnl
  m4_ifdef([gl_FUNC_STRERROR_R],[dnl
    AC_REQUIRE([gl_FUNC_STRERROR_R])dnl
  ],[dnl
    AC_REQUIRE([AC_FUNC_STRERROR_R])dnl
  ])dnl
  m4_ifdef([gl_FUNC_GNU_STRFTIME],[dnl
    AC_REQUIRE([gl_FUNC_GNU_STRFTIME])dnl
  ],[
    m4_ifdef([gl_FUNC_STRFTIME],[dnl
      AC_REQUIRE([gl_FUNC_STRFTIME])dnl
    ],[dnl
      AC_REQUIRE([AC_FUNC_STRFTIME])dnl
    ])dnl
  ])dnl
  m4_ifdef([gl_FUNC_STRNDUP],[dnl
    AC_REQUIRE([gl_FUNC_STRNDUP])dnl
  ],[
    if test "x${ac_cv_func_strndup}" = "x"; then
      test -z "${ac_cv_func_strndup}" || unset ac_cv_func_strndup
      AC_CHECK_FUNCS([strndup])
    fi
  ])dnl
  m4_ifdef([gl_FUNC_STRNLEN],[dnl
    AC_REQUIRE([gl_FUNC_STRNLEN])dnl
  ],[dnl
    AC_REQUIRE([AC_FUNC_STRNLEN])dnl
  ])dnl
  ## XXX
  m4_ifdef([gl_FUNC_STRSTR],[ ## have gnulib macro for strstr
    AC_REQUIRE([gl_FUNC_STRSTR])dnl
    ## in case the function itself never actually got checked:
    if test "x${ac_cv_func_strstr}" = "x"; then
      test -z "${ac_cv_func_strstr}" || unset ac_cv_func_strstr
      AC_CHECK_FUNCS([strstr])
    fi  ##end check for strstr function check cache value
  ],[dnl
    dnl# the regular strstr gnulib macro would have called this normally,
    dnl# but just in case we have this and not the other:
    m4_ifdef([gl_FUNC_STRSTR_SIMPLE],[dnl
      AC_REQUIRE([gl_FUNC_STRSTR_SIMPLE])dnl
      ## in case the function itself never actually got checked:
      if test "x${ac_cv_func_strstr}" = "x"; then
        test -z "${ac_cv_func_strstr}" || unset ac_cv_func_strstr
        AC_CHECK_FUNCS([strstr])
      fi  ##end check for strstr function check cache value
    ],[
      AC_CHECK_FUNCS([strstr])dnl
    ])dnl
  ])dnl
  m4_ifdef([gl_FUNC_STRTOD],[dnl
    AC_REQUIRE([gl_FUNC_STRTOD])dnl
  ],[
    AC_REQUIRE([AC_FUNC_STRTOD])dnl
  ])dnl
  m4_ifdef([gl_FUNC_STRTOLD],[dnl
    AC_REQUIRE([gl_FUNC_STRTOLD])dnl
  ])dnl
  m4_ifdef([gl_FUNC_UNAME],[dnl
    AC_REQUIRE([gl_FUNC_UNAME])dnl
  ],[
    if test "x${ac_cv_func_uname}" = "x"; then
      test -z "${ac_cv_func_uname}" || unset ac_cv_func_uname
      AC_CHECK_FUNCS([uname])
    fi
  ])dnl
  m4_ifdef([gl_FUNC_WAITPID],[dnl
    AC_REQUIRE([gl_FUNC_WAITPID])dnl
  ])dnl
  m4_ifdef([gl_FUNC_WRITE],[dnl
    AC_REQUIRE([gl_FUNC_WRITE])dnl
  ])dnl
])dnl# end macro definition

AC_DEFUN([AC_REQUIRE_FUNC_FSEEKO],[dnl
  AC_REQUIRE([AC_FUNC_FSEEKO])dnl
])dnl

dnl# dummy macro, only exists for searchability:
AC_DEFUN([gl_FUNC_UMBRELLA],[dnl
  AC_REQUIRE([AC_REQUIRE_VARIOUS_FUNC_CHECKS])dnl
])dnl# end macro definition

# functions_extra.m4 ends here
