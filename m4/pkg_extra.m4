# pkg_extra.m4 serial 1 file starts here
dnl# This was originally in the emacs configure.ac file.   -*- Autoconf -*-
dnl# Before that, this function was definition taken from Gnome 2.0
dnl# PKG_CHECK_MODULES([GSTUFF],[gtk+-2.0 >= 1.3 glib = 1.3.4],[action-if],
dnl#                   [action-not])
dnl# defines GSTUFF_LIBS, GSTUFF_CFLAGS, see pkg-config man page
dnl# also defines GSTUFF_PKG_ERRORS on error
dnl# (why not just use the official pkg-config macros from pkg.m4?)
AC_DEFUN([PKG_CHECK_MODULES],[
  succeeded=no

  if test "x${PKG_CONFIG}" = "xno"; then
     ifelse([$4],[],[AC_MSG_ERROR([
      *** The pkg-config script could not be found. Make sure it is in your path, or give the full name of pkg-config with the PKG_CONFIG environment variable or --with-pkg-config-prog.  Or see http://www.freedesktop.org/software/pkgconfig to get pkg-config.])],[$4])
  else
     dnl# generally I try to avoid doing this, but...
     m4_pattern_allow([PKG_CONFIG_MIN_VERSION])
     dnl# m4-quote this; it is NOT its own macro:
     [
     export PKG_CONFIG_MIN_VERSION="0.9.0"
     ]
     dnl# end m4-quoting
     if "${PKG_CONFIG}" --atleast-pkgconfig-version ${PKG_CONFIG_MIN_VERSION}; then
        AC_MSG_CHECKING([for $2])dnl

        if "${PKG_CONFIG}" --exists "$2" 2>&AS_MESSAGE_LOG_FD &&
	   $1_CFLAGS=`"$PKG_CONFIG" --cflags "$2" 2>&AS_MESSAGE_LOG_FD` &&
	   $1_LIBS=`"${PKG_CONFIG}" --libs "$2" 2>&AS_MESSAGE_LOG_FD`; then
	    edit_cflags="
	      s,///*,/,g
	      s/^/ /
	      s/ -I/ ${isystem}/g
	      s/^ //
	    "
	    $1_CFLAGS=`AS_ECHO(["$$1_CFLAGS"]) | sed -e "${edit_cflags}"`
	    $1_LIBS=`AS_ECHO(["$$1_LIBS"]) | sed -e 's,///*,/,g'`
            AC_MSG_RESULT([yes CFLAGS='$$1_CFLAGS' LIBS='$$1_LIBS'])
            succeeded=yes
        else
            AC_MSG_RESULT([no])
            $1_CFLAGS=""
            $1_LIBS=""
            ## If we have a custom action on failure, do NOT print errors,
            ## but do set a variable so people can do so. Do it in a
            ## subshell to capture any diagnostics in invoking pkg-config.
	    $1_PKG_ERRORS=`("${PKG_CONFIG}" --print-errors "$2") 2>&1`
	    ifelse([$4],[],[echo "$$1_PKG_ERRORS"],[])
        fi

        AC_SUBST([$1_CFLAGS])
        AC_SUBST([$1_LIBS])
     else
        echo "*** Your version of pkg-config is too old. You need version ${PKG_CONFIG_MIN_VERSION} or newer."
        echo "*** See http://www.freedesktop.org/software/pkgconfig"
     fi
  fi

  if test "x${succeeded}" = "xyes"; then
     ifelse([$3],[],[:],[$3])
  else
     ifelse([$4],[],[AC_MSG_ERROR([Library requirements ($2) not met; consider adjusting the PKG_CONFIG_PATH environment variable if your libraries are in a nonstandard prefix so pkg-config can find them.])], [$4])
  fi
])dnl

# pkg_extra.m4 used to end here

dnl# New version:
dnl# EMACS_CHECK_MODULES([GSTUFF],[gtk+-2.0 >= 1.3 glib = 1.3.4])
dnl# acts like PKG_CHECK_MODULES([GSTUFF],[gtk+-2.0 >= 1.3 glib = 1.3.4],
dnl# [HAVE_GSTUFF=yes],[HAVE_GSTUFF=no]) -- see pkg-config man page --
dnl# except that it postprocesses CFLAGS as needed for
dnl# --enable-gcc-warnings.
dnl# EMACS_CHECK_MODULES accepts optional 3rd and 4th arguments that
dnl# can take the place of the default HAVE_GSTUFF=yes and HAVE_GSTUFF=no
dnl# actions.
AC_DEFUN([EMACS_CHECK_MODULES],
  [AC_REQUIRE([AC_PROG_SED])
   dnl# make sure edit_cflags is non-empty:
   test -n "${edit_cflags}" ##FIXME: do something based on this
   dnl# now just pass through to the underlying macro:
   PKG_CHECK_MODULES([$1],[$2],
     [$1_CFLAGS=`AS_ECHO(["$$1_CFLAGS"]) | sed -e "${edit_cflags}"`
      m4_default([$3],[HAVE_$1=yes])],
     [m4_default([$4],[HAVE_$1=no])])])dnl

# pkg_extra.m4 actually ends here
