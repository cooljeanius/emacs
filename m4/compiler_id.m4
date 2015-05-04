# compiler_id.m4 serial 2 file starts here
dnl# This was originally in the emacs configure.ac file.   -*- Autoconf -*-

dnl# these two are now defun-ed macros in preparation for moving to their
dnl# own macro file:
AC_DEFUN([EMACS_COMPILER_ID_CLANG],[
# clang is unduly picky about some things.
AC_CACHE_CHECK([whether the compiler is actually clang],[emacs_cv_clang],
  [AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
	  #ifndef __clang__
	    #error "not clang"
	  #endif /* !__clang__ */
        ]])],
     [emacs_cv_clang=yes],
     [emacs_cv_clang=no])])dnl
])dnl# end macro for clang

AC_DEFUN([EMACS_COMPILER_ID_APPLE],[
# some flags are Apple-specific:
AC_CACHE_CHECK([if the compiler is an Apple compiler],[emacs_cv_apple],
  [AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
/* not sure if the first '||' should actually be an '&&' here: */
#if !defined(__APPLE__) || !defined(__APPLE_CC__) || !(__APPLE_CC__ > 1) 
  #error "not Apple"
#endif /* !__APPLE__ || !__APPLE_CC__ || !(__APPLE_CC__ > 1) */
        ]])],
     [emacs_cv_apple=yes],
     [emacs_cv_apple=no])])dnl
])dnl# end macro for Apple

AC_DEFUN([EMACS_CLANG_STATIC_ANALYSIS],[
AC_REQUIRE([EMACS_COMPILER_ID_CLANG])dnl
AC_ARG_VAR([CLANG_ANALYZER],[Path to the clang static analyzer])dnl
  ## change the message based on what we already know:
if test "x${emacs_cv_clang}" = "xyes"; then
  emacs_clang_check_message="if we can also use clang for static analysis"
else
  emacs_clang_check_message="for the clang static analyzer"
fi
AC_CACHE_CHECK([${emacs_clang_check_message}],[ac_cv_path_CLANG_ANALYZER],
  [AC_PATH_PROGS_FEATURE_CHECK([CLANG_ANALYZER],
    [clang clang++ clang-mp-3.5 clang-mp-3.4 clang-mp-3.3 clang-mp-3.2],
    [[${ac_path_CLANG_ANALYZER} --analyze /dev/null > /dev/null 2>&1 && \
      ac_cv_path_CLANG_ANALYZER=${ac_path_CLANG_ANALYZER}
      ac_path_CLANG_ANALYZER_found=:]],
    [AC_MSG_WARN([we will not be able to do static analysis with clang])],
    [${PATH}])dnl# end program check
  ])dnl# end cache check
  ## (need this extra line here)
AC_SUBST([CLANG_ANALYZER],[${ac_cv_path_CLANG_ANALYZER}])dnl
])dnl

dnl#TODO: see if there are any remaining checks that currently check the
dnl# value of ${host_vendor} for apple that I can move to use the above
dnl# conftest instead.

# compiler_id.m4 ends here
