# options.m4 serial 1 file starts here
dnl# This was originally in the emacs configure.ac file.   -*- Autoconf -*-

dnl# OPTION_DEFAULT_OFF([NAME],[HELP-STRING])
dnl# Create a new --with option that defaults to being disabled.
dnl# NAME is the base name of the option.  The shell variable with_NAME
dnl#   will be set to either the user's value (if the option is
dnl#   specified; 'yes' for a plain --with-NAME) or to 'no' (if the
dnl#   option is not specified).  Note that the shell variable name is
dnl#   constructed as autoconf does, by replacing non-alphanumeric
dnl#   characters with "_".
dnl# HELP-STRING is the help text for the option.
AC_DEFUN([OPTION_DEFAULT_OFF],[dnl
  AC_ARG_WITH([$1],[AS_HELP_STRING([--with-$1],[$2])],[],[dnl
    m4_bpatsubst([with_$1],[[^0-9a-z]],[_])=no])dnl
])dnl

dnl# OPTION_DEFAULT_ON(NAME, HELP-STRING)
dnl# Create a new --with option that defaults to ${with_features}.
dnl# NAME is the base name of the option.  The shell variable with_NAME
dnl#   will be set either to 'no' (for a plain --without-NAME) or to
dnl#   'yes' (if the option is not specified).  Note that the shell
dnl#   variable name is constructed as autoconf does, by replacing
dnl#   non-alphanumeric characters with "_".
dnl# HELP-STRING is the help text for the option.
AC_DEFUN([OPTION_DEFAULT_ON],[dnl
  AC_ARG_WITH([$1],[AS_HELP_STRING([--without-$1],[$2])],[],[dnl
   m4_bpatsubst([with_$1],[[^0-9a-z]],[_])=${with_features}])dnl
])dnl

# options.m4 ends here
