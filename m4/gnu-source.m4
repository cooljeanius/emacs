# gnu-source.m4 serial 1  -*- Autoconf -*-
# split off from either extensions.m4 or specific.m4

# Copyright (C) 2003, 2006-2014 Free Software Foundation, Inc.
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.


# AC_GNU_SOURCE
# --------------
AC_DEFUN([AC_GNU_SOURCE],
[AH_VERBATIM([_GNU_SOURCE],
[/* Enable GNU extensions on systems that have them.  */
#ifndef _GNU_SOURCE
# undef _GNU_SOURCE
#endif /* !_GNU_SOURCE */])dnl
AC_BEFORE([$0],[AC_COMPILE_IFELSE])dnl
AC_BEFORE([$0],[AC_RUN_IFELSE])dnl
AC_DEFINE([_GNU_SOURCE])
])dnl
