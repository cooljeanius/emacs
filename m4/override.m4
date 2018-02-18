# override.m4 serial 2 file starts here
dnl# This was originally in the emacs configure.ac file.   -*- Autoconf -*-
dnl########################################################################
dnl# shell-level comments with just a number on the line are a substitute
dnl# for whitespace lines, and help make the generated configure script
dnl# more readable (for debugging purposes).
# 0
dnl# Do NOT bother to test for C89:
AC_DEFUN([_AC_PROG_CC_C89],[$2])dnl
# 1
dnl# Avoid gnulib's tests for -lcrypto, so that there is no static
dnl# dependency on it:
AC_DEFUN([gl_CRYPTO_CHECK],[
  dnl# still need to subst this though:
  test -z "${LIB_CRYPTO}"
  AC_SUBST([LIB_CRYPTO])dnl
])dnl
# 2
dnl# We DO actually need HAVE_WORKING_O_NOFOLLOW now, so stop overriding it
:
# 3
dnl# Avoid gnulib's threadlib module, as we do threads our own way:
AC_DEFUN([gl_THREADLIB],[:])dnl
# 4
dnl# Avoid gnulib's select module, for whatever reason, and replace its
dnl# macro with a shorter version:
AC_DEFUN([gl_FUNC_SELECT],[
  test -z "${REPLACE_SELECT}" && REPLACE_SELECT=0
  AC_REQUIRE([AC_CANONICAL_HOST])dnl# for cross-compiles
  if test "x${ac_cv_header_winsock2_h}" = "xyes"; then
    REPLACE_SELECT=1
  else
    dnl# "_ONCE" causes test to jump out of the conditional and be done at
    dnl# wrong location, so instead check the cache value:
    if test "x${ac_cv_header_sys_select_h}" = "x"; then
      test -z "${ac_cv_header_sys_select_h}"
      AC_CHECK_HEADERS([sys/select.h])
    fi
    dnl# On Interix 3.5, select(0, NULL, NULL, NULL, timeout) fails with
    dnl# error EFAULT.
    dnl# Skip the actual check though:
    gl_cv_func_select_supports0="maybe"
    case "${host_os}" in
                # Guess no on Interix:
      interix*) gl_cv_func_select_supports0="guessing no" ;;
                # Guess yes otherwise:
      *)        gl_cv_func_select_supports0="guessing yes" ;;
    esac
    case "${gl_cv_func_select_supports0}" in
      *yes) ;;
      *no)  REPLACE_SELECT=1 ;;
    esac

    dnl# On FreeBSD 8.2, select() sometimes fails to reject bad fds.
    dnl# Skip actually checking for this though.
    gl_cv_func_select_detects_ebadf="maybe"
    case "${host_os}" in
                 # Guess yes on glibc systems:
     *-gnu*)     gl_cv_func_select_detects_ebadf="guessing yes" ;;
                 # Assume no on FreeBSD:
     *-freebsd*) gl_cv_func_select_detects_ebadf="assuming no" ;;
                 # If we do NOT know, then assume the worst:
     *)          gl_cv_func_select_detects_ebadf="guessing no" ;;
    esac
    case ${gl_cv_func_select_detects_ebadf} in
      *yes) ;;
      *no)  REPLACE_SELECT=1 ;;
    esac
  fi

  dnl# Determine the needed libraries:
  LIB_SELECT=""
  if test "x${LIBSOCKET}" != "x"; then
    test -n "${LIBSOCKET}" && export LIB_SELECT="${LIBSOCKET}"
  fi
  if test ${REPLACE_SELECT} = 1; then
    case "${host_os}" in
      mingw*)
        dnl# On the MSVC platform, the function MsgWaitForMultipleObjects
        dnl# (used by select) requires linking with -luser32.
        dnl# On mingw, it is implicit.
        AC_CHECK_LIB([user32],[MsgWaitForMultipleObjects],
                     [LIB_SELECT="${LIB_SELECT} -luser32"])
        ;;
    esac
  fi
  AC_SUBST([LIB_SELECT])dnl
])dnl# end macro for select function
# 5
dnl########################################################################
dnl# override other gnulib modules we avoid, just in case:
# 6
dnl# from close.m4:
AC_DEFUN([gl_FUNC_CLOSE],[
  if test ${HAVE_MSVC_INVALID_PARAMETER_HANDLER} = 1; then
    REPLACE_CLOSE=1
  fi
  dnl# ifdef-ed, so ok to keep:
  m4_ifdef([gl_PREREQ_SYS_H_WINSOCK2],[
    gl_PREREQ_SYS_H_WINSOCK2
    if test ${UNISTD_H_HAVE_WINSOCK2_H} = 1; then
      dnl# Even if the 'socket' module is not used here, another part
      dnl# of the application may use it and pass file descriptors
      dnl# that refer to sockets to the close() function.
      dnl# So enable the support for sockets:
      REPLACE_CLOSE=1
    fi
  ])dnl
  dnl# Not actually replacing close() for supporting the gnulib-defined
  dnl# fchdir() function, but keep fchdir's bookkeeping up-to-date:
  m4_ifdef([gl_FUNC_FCHDIR],[
    if test ${REPLACE_CLOSE} = 0; then
      m4_ifdef([gl_TEST_FCHDIR],[
        gl_TEST_FCHDIR
        if test ${HAVE_FCHDIR} = 0; then
          REPLACE_CLOSE=1
        fi
      ])dnl# end inner ifdef
      ## (else do nothing)
    fi
  ])dnl# end outer ifdef
])dnl
# 7
dnl# from dup.m4:
AC_DEFUN([gl_FUNC_DUP],[
  if test ${HAVE_MSVC_INVALID_PARAMETER_HANDLER} = 1; then
    REPLACE_DUP=1
  fi
  dnl# Not actually replacing dup() for supporting the gnulib-defined
  dnl# fchdir() function, but keep fchdir's bookkeeping up-to-date:
  m4_ifdef([gl_FUNC_FCHDIR],[
    m4_ifdef([gl_TEST_FCHDIR],[
      gl_TEST_FCHDIR
      if test ${HAVE_FCHDIR} = 0; then
        REPLACE_DUP=1
      fi
    ])dnl# end inner ifdef
  ])dnl# end outer ifdef
])dnl
# 8
dnl# from fchdir.m4:
AC_DEFUN([gl_FUNC_FCHDIR],[
  AC_REQUIRE([AC_CANONICAL_HOST])dnl# for cross-compiles
  AC_CHECK_DECLS_ONCE([fchdir])dnl
  ## ok to keep:
  if test "x${ac_cv_have_decl_fchdir}" = "xno"; then
    HAVE_DECL_FCHDIR=0
  fi

  m4_ifdef([gl_TEST_FCHDIR],[
    AC_REQUIRE([gl_TEST_FCHDIR])dnl
  ])dnl
  ## ok to keep here:
  if test ${HAVE_FCHDIR} = 0; then
    m4_ifdef([gl_PREREQ_FCHDIR],[
      gl_PREREQ_FCHDIR
    ])dnl
    AC_DEFINE([REPLACE_FCHDIR],[1],
      [Define to 1 if the gnulib fchdir() replacement would be used.])dnl
    dnl# Normally we would also have to replace anything that can
    dnl# manipulate a directory fd, to keep our bookkeeping up-to-date.
    dnl# Skip actually checking though.
    gl_cv_func_open_directory_works="maybe"
    case "${host_os}" in
              # Guess yes on glibc systems:
      *-gnu*) gl_cv_func_open_directory_works="guessing yes" ;;
              # If we do NOT know, then assume the worst:
      *)      gl_cv_func_open_directory_works="guessing no" ;;
    esac
    case "${gl_cv_func_open_directory_works}" in
      *yes) ;;
      *no)
        AC_DEFINE([REPLACE_OPEN_DIRECTORY],[1],[Define to 1 if open()
should work around the inability to open a directory.])
        ;;
    esac
  fi
])dnl
# 9
dnl# likewise:
AC_DEFUN([gl_TEST_FCHDIR],[
  AC_CHECK_FUNCS_ONCE([fchdir])dnl
  ## ok to keep in:
  if test "x${ac_cv_func_fchdir}" = "xno"; then
    HAVE_FCHDIR=0
  fi
])dnl
# 10
dnl# from localcharset.m4:
AC_DEFUN([gl_LOCALCHARSET],[
  m4_ifdef([AM_LANGINFO_CODESET],[
    AC_REQUIRE([AM_LANGINFO_CODESET])dnl
  ])dnl
  AC_CHECK_DECLS_ONCE([getc_unlocked])dnl
])dnl
# 11
dnl# from lock.m4:
AC_DEFUN([gl_LOCK],[
  m4_ifdef([gl_THREADLIB],[
    AC_REQUIRE([gl_THREADLIB])dnl
  ])dnl
  m4_ifdef([gl_PREREQ_LOCK],[
    AC_REQUIRE([gl_PREREQ_LOCK])dnl
  ])dnl
])dnl
# 12
dnl# (malloc.m4 is complicated, skip overriding it for now)
# 13
dnl# from memchr-obsolete.m4:
AC_DEFUN([gl_FUNC_MEMCHR_OBSOLETE],[:])dnl
# 14
dnl# from msvc-nothrow.m4:
AC_DEFUN([gl_MSVC_NOTHROW],[
  m4_ifdef([gl_MSVC_INVAL],[
    AC_REQUIRE([gl_MSVC_INVAL])dnl
  ])dnl
])dnl
# 15
dnl# from open.m4:
AC_DEFUN([gl_FUNC_OPEN],[
  AC_REQUIRE([AC_CANONICAL_HOST])dnl# for cross-compiles
  ## just assume based on host:
  case "${host_os}" in
    mingw* | pw*)
      REPLACE_OPEN=1
      ;;
    *)
      dnl# open("foo/") should not create a file when the file name has a
      dnl# trailing slash.  FreeBSD only has the problem on symlinks.
      dnl# Skip actually checking though.
      AC_CHECK_FUNCS_ONCE([lstat])dnl
      ## assume:
      gl_cv_func_open_slash="maybe"
      case "${host_os}" in
        freebsd* | aix* | hpux* | solaris2.[0-9] | solaris2.[0-9].*)
          gl_cv_func_open_slash="guessing no" ;;
        *)
          gl_cv_func_open_slash="guessing yes" ;;
      esac
      case "${gl_cv_func_open_slash}" in
        *no)
          AC_DEFINE([OPEN_TRAILING_SLASH_BUG],[1],
            [Define to 1 if open() fails to recognize a trailing slash.])
          REPLACE_OPEN=1
          ;;
      esac
      ;;
  esac

  dnl# Not actually replacing open() for supporting the gnulib-defined
  dnl# fchdir() function, but keep fchdir's bookkeeping up-to-date:
  m4_ifdef([gl_FUNC_FCHDIR],[
    if test ${REPLACE_OPEN} = 0; then
      m4_ifdef([gl_TEST_FCHDIR],[
        gl_TEST_FCHDIR
        if test $HAVE_FCHDIR = 0; then
          REPLACE_OPEN=1
        fi
      ])dnl
      ## (else do nothing)
    fi
  ])dnl
  dnl# Act like we replace open() for supporting the gnulib-defined
  dnl# O_NONBLOCK flag:
  m4_ifdef([gl_NONBLOCKING_IO],[
    if test ${REPLACE_OPEN} = 0; then
      gl_NONBLOCKING_IO
      if test "x${gl_cv_have_open_O_NONBLOCK}" != "xyes"; then
        REPLACE_OPEN=1
      fi
    fi
  ])dnl
])dnl
# 16
dnl# likewise:
AC_DEFUN([gl_PREREQ_OPEN],[:])dnl
# 17
dnl# from opendir.m4:
AC_DEFUN([gl_FUNC_OPENDIR],[
  AC_CHECK_FUNCS([opendir])dnl
  ## ok to keep:
  if test "x${ac_cv_func_opendir}" = "xno"; then
    HAVE_OPENDIR=0
  fi
  dnl# Not actually replacing opendir() for supporting the gnulib-defined
  dnl# fchdir() function here, but keep fchdir's bookkeeping up-to-date:
  m4_ifdef([gl_FUNC_FCHDIR],[
    m4_ifdef([gl_TEST_FCHDIR],[
      gl_TEST_FCHDIR
      if test ${HAVE_FCHDIR} = 0; then
        if test ${HAVE_OPENDIR} = 1; then
          REPLACE_OPENDIR=1
        fi
      fi
    ])dnl# end inner ifdef
  ])dnl# end outer ifdef
])dnl
# 18
dnl# from raise.m4:
AC_DEFUN([gl_FUNC_RAISE],[
  AC_CHECK_FUNCS_ONCE([raise])dnl
  ## not actually going to be replacing, but check like we are:
  if test "x${ac_cv_func_raise}" = "xno"; then
    HAVE_RAISE=0
  else
    if test ${HAVE_MSVC_INVALID_PARAMETER_HANDLER} = 1; then
      REPLACE_RAISE=1
    fi
    dnl# these are ifdef-ed, so it is okay to leave them:
    m4_ifdef([gl_SIGNALBLOCKING],[
      gl_SIGNALBLOCKING
      if test ${HAVE_POSIX_SIGNALBLOCKING} = 0; then
        m4_ifdef([gl_SIGNAL_SIGPIPE],[
          gl_SIGNAL_SIGPIPE
          if test "x${gl_cv_header_signal_h_SIGPIPE}" != "xyes"; then
            REPLACE_RAISE=1
          fi
        ],[:])dnl# end check for signal sigpipe macro
        ## (else do nothing)
      fi
    ])dnl# end check for blocking signal macro
    ## (end "else" half of condition)
  fi
])dnl
# 19
dnl# from save-cwd.m4:
AC_DEFUN([gl_SAVE_CWD],[
  AC_CHECK_FUNCS_ONCE([fchdir])dnl
])dnl
# 20
dnl# from signalblocking.m4:
AC_DEFUN([gl_SIGNALBLOCKING],[
  if test "x${gl_cv_type_sigset_t}" = "xyes"; then
    AC_CHECK_FUNC([sigprocmask],[gl_cv_func_sigprocmask=1])
  fi
  if test -z "${gl_cv_func_sigprocmask}"; then
    HAVE_POSIX_SIGNALBLOCKING=0
  fi
])dnl
# 21
dnl# from strdup.m4:
AC_DEFUN([gl_FUNC_STRDUP],[
  AC_CHECK_FUNCS_ONCE([strdup])dnl
  AC_CHECK_DECLS_ONCE([strdup])dnl
  ## might still need this:
  if test "x${ac_cv_have_decl_strdup}" = "xno"; then
    HAVE_DECL_STRDUP=0
  fi
])dnl
# 22
dnl# likewise:
AC_DEFUN([gl_FUNC_STRDUP_POSIX],[
  AC_CHECK_FUNCS_ONCE([strdup])dnl
  ## not actually replacing, but whatever:
  if test "x${ac_cv_func_strdup}" = "xyes"; then
    if test "x${gl_cv_func_malloc_posix}" = "xno"; then
      REPLACE_STRDUP=1
    fi
  fi
  AC_CHECK_DECLS_ONCE([strdup])dnl
  ## might be duplicated, but whatever:
  if test "x${ac_cv_have_decl_strdup}" = "xno"; then
    HAVE_DECL_STRDUP=0
  fi
])dnl
# 23
dnl# from threadlib.m4:
AC_DEFUN([gl_THREADLIB_EARLY],[
  m4_ifdef([gl_THREADLIB_EARLY_BODY],[
    AC_REQUIRE([gl_THREADLIB_EARLY_BODY])dnl
  ])dnl# end ifdef
])dnl
# 24
dnl# likewise:
AC_DEFUN([gl_THREADLIB_EARLY_BODY],[:])dnl
# 25
dnl# likewise:
AC_DEFUN([gl_THREADLIB_BODY],[
  m4_ifdef([gl_THREADLIB_EARLY_BODY],[
    AC_REQUIRE([gl_THREADLIB_EARLY_BODY])dnl
  ])dnl# end ifdef
  ## make sure these variables are all set:
  gl_threads_api=none
  LIBTHREAD=""
  LTLIBTHREAD=""
  LIBMULTITHREAD=""
  LTLIBMULTITHREAD=""
  dnl# ...and subst-ed:
  AC_SUBST([LIBTHREAD])dnl
  AC_SUBST([LTLIBTHREAD])dnl
  AC_SUBST([LIBMULTITHREAD])dnl
  AC_SUBST([LTLIBMULTITHREAD])dnl
])dnl
# 26
dnl# (main threadlib macro is already overridden above)
# 27
dnl# likewise (this one can stay un-modified):
AC_DEFUN([gl_DISABLE_THREADS],[
  m4_divert_text([INIT_PREPARE],[gl_use_threads_default=no])dnl
])dnl
# 28
dnl# from tls.m4:
AC_DEFUN([gl_TLS],[
  m4_ifdef([gl_THREADLIB],[
    AC_REQUIRE([gl_THREADLIB])dnl
  ])dnl
])dnl
# 29
dnl# from vasnprintf.m4:
AC_DEFUN([gl_FUNC_VASNPRINTF],[
  AC_CHECK_FUNCS_ONCE([vasnprintf])dnl
  dnl# not actually replacing it, but act like we are:
  if test "x${ac_cv_func_vasnprintf}" = "xno"; then
    dnl# this should remain defined even when overridden here:
    m4_ifdef([gl_REPLACE_VASNPRINTF],[
      gl_REPLACE_VASNPRINTF
    ])dnl
    ## or whatever...
  fi
])dnl
# 30
dnl# likewise:
AC_DEFUN([gl_REPLACE_VASNPRINTF],[
  AC_CHECK_FUNCS_ONCE([vasnprintf])dnl
  dnl# still might need this define:
  if test "x${ac_cv_func_vasnprintf}" = "xyes"; then
    AC_DEFINE([REPLACE_VASNPRINTF],[1],
      [Define to 1 if vasnprintf exists but would be overridden by gnulib])
  fi
])dnl
# 31
dnl# from vasnprintf-posix.m4:
AC_DEFUN([gl_FUNC_VASNPRINTF_POSIX],[
  gl_cv_func_vasnprintf_posix=no
  AC_CHECK_FUNCS_ONCE([vasnprintf])dnl
])dnl
# 32
dnl# end gnulib section
dnl########################################################################
# 33
dnl# Suppress obsolescent Autoconf test for size_t; Emacs assumes C89 or
dnl# better:
AC_DEFUN([AC_TYPE_SIZE_T],[:])dnl
# 34
dnl# Likewise for obsolescent test for uid_t, gid_t; Emacs assumes them:
AC_DEFUN([AC_TYPE_UID_T],[:])dnl
# 35
dnl# Obsolete in Autoconf 2.70; copied here from 2.69 to un-obsolete it:
AC_DEFUN([AC_HEADER_TIME],
[AC_CACHE_CHECK([whether time.h and sys/time.h may both be included],
  [ac_cv_header_time],
[AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
]],
[[if ((struct tm *)0)
return 0;]])],
		   [ac_cv_header_time=yes],
		   [ac_cv_header_time=no])])
if test "x${ac_cv_header_time}" = "xyes"; then
  AC_DEFINE([TIME_WITH_SYS_TIME],[1],
	    [Define to 1 if you can safely include both <sys/time.h>
	     and <time.h>.])
fi
])dnl# end check for time header
# 36
dnl# Obsolete in Autoconf 2.70; copied here from 2.69 to un-obsolete it:
AC_DEFUN([AC_PROG_CC_STDC],
[ AC_REQUIRE([AC_PROG_CC])dnl
  AS_CASE([$ac_cv_prog_cc_stdc],
    [no], [ac_cv_prog_cc_c99=no; ac_cv_prog_cc_c89=no],
	  [_AC_PROG_CC_C99([ac_cv_prog_cc_stdc=$ac_cv_prog_cc_c99],
	     [_AC_PROG_CC_C89([ac_cv_prog_cc_stdc=$ac_cv_prog_cc_c89],
			      [ac_cv_prog_cc_stdc=no])])])
  AC_MSG_CHECKING([for $CC option to accept ISO Standard C])
  AC_CACHE_VAL([ac_cv_prog_cc_stdc], [])
  AS_CASE([$ac_cv_prog_cc_stdc],
    [no], [AC_MSG_RESULT([unsupported])],
    [''], [AC_MSG_RESULT([none needed])],
	  [AC_MSG_RESULT([$ac_cv_prog_cc_stdc])])
])dnl# end check for standard C compiler
# 37
#### override.m4 ends here
