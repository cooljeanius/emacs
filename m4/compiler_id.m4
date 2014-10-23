# compiler_id.m4 serial 1 file starts here
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

dnl#TODO: see if there are any remaining checks that currently check the
dnl# value of ${host_vendor} for apple that I can move to use the above
dnl# conftest instead.

# compiler_id.m4 ends here
