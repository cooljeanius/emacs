# _Generic.m4 serial 1                                      -*- Autoconf -*-
# Taken from autoconf 2.71's c.m4

# AC_C__GENERIC
# -------------
# Define HAVE_C__GENERIC if _Generic works, a la C11.
AN_IDENTIFIER([_Generic], [AC_C__GENERIC])
AC_DEFUN([AC_C__GENERIC],
[AC_CACHE_CHECK([for _Generic], ac_cv_c__Generic,
[AC_COMPILE_IFELSE(
   [AC_LANG_SOURCE(
      [[int
	 main (int argc, char **argv)
	 {
	   int a = _Generic (argc, int: argc = 1);
	   int *b = &_Generic (argc, default: argc);
	   char ***c = _Generic (argv, int: argc, default: argv ? &argv : 0);
	   _Generic (1 ? 0 : b, int: a, default: b) = &argc;
	   _Generic (a = 1, default: a) = 3;
	   return a + !b + !c;
	 }
      ]])],
   [ac_cv_c__Generic=yes],
   [ac_cv_c__Generic=no])])
if test $ac_cv_c__Generic = yes; then
  AC_DEFINE([HAVE_C__GENERIC], 1,
	    [Define to 1 if C11-style _Generic works.])
fi
])# AC_C__GENERIC
