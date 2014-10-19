# grep.m4 serial 1 file starts here
dnl# This was originally in the emacs configure.ac file.   -*- Autoconf -*-

dnl# I often put something like this in my configure files, but in this
dnl# version I have drawn inspiration from the analogous libtool macro:
dnl# If we do NOT have a new enough Autoconf to choose the best grep
dnl# available, then choose the one first in the user's PATH:
AC_DEFUN([AC_REQUIRE_GREPS],[
  AC_REQUIRE([AC_PROG_GREP])dnl
  AC_REQUIRE([AC_PROG_EGREP])dnl
  AC_REQUIRE([AC_PROG_FGREP])dnl
  test -z "${GREP}" && export GREP=grep
  dnl# Non-bleeding-edge autoconf does NOT subst GREP, so do it here too:
  AC_SUBST([GREP])dnl
])dnl

AC_DEFUN([AC_EGREP_PREPROC_SANITY],[
  dnl# grep is often used with cpp; so throw in a sanity check for them
  dnl# here as a control group to compare failures against:
  AC_REQUIRE([AC_PROG_CPP])dnl
  AC_CACHE_CHECK([sanity of egrep used with cpp],[ac_cv_egrep_cpp_sanity],[
    AC_EGREP_CPP([sanity],[sanity],
                 [ac_cv_egrep_cpp_sanity=yes],
                 [ac_cv_egrep_cpp_sanity=no])dnl
  ])dnl# this next blank line is necessary:

  dnl# ...and do likewise with the brother of the previous macro:
  AC_CACHE_CHECK([sanity of egrep for checking headers],
                 [ac_cv_egrep_header_sanity],[
    AC_EGREP_HEADER([printf],[stdio.h],
                    [ac_cv_egrep_header_sanity=yes],
                    [ac_cv_egrep_header_sanity=no])dnl
  ])dnl

  if test "x${ac_cv_egrep_cpp_sanity}" = "xyes" && test "x${ac_cv_egrep_header_sanity}" = "xyes"; then
    export EGREP_PREPROC_SANE=true
  fi
])dnl

# grep.m4 ends here
