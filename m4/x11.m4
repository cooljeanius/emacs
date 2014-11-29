# x11.m4 serial 1                       -*- Autoconf -*-
dnl# The x11 macros taken from libs.m4 from autoconf, to override with
dnl# better versions.
dnl# Originally copyright (C) 1992-1996, 1998-2006, 2008-2012 Free Software
dnl# Foundation, Inc.

dnl# ---------------------- #
dnl# Checks for X windows:  #
dnl# ---------------------- #

dnl# _AC_PATH_X_XMKMF
dnl# ----------------
dnl# Internal subroutine of _AC_PATH_X.
dnl# Set ac_x_includes and/or ac_x_libraries.
m4_define([_AC_PATH_X_XMKMF],
[AC_ARG_VAR([XMKMF],
            [Path to xmkmf, Makefile generator for X Window System])dnl
AC_REQUIRE([AC_PROG_CC])dnl
AC_REQUIRE([AC_PROG_CPP])dnl
AC_REQUIRE([AC_PROG_SED])dnl

rm -f -r conftest.dir
if mkdir conftest.dir; then
  cd conftest.dir
  cat >Imakefile <<'_ACEOF'
incroot:
	@echo incroot='${INCROOT}'
usrlibdir:
	@echo usrlibdir='${USRLIBDIR}'
libdir:
	@echo libdir='${LIBDIR}'
_ACEOF
  if (export CC; ${XMKMF-xmkmf}) >/dev/null 2>/dev/null && test -f Makefile; then
    # GNU make sometimes prints "make[1]: Entering ...", which would
    # confuse us:
    for ac_var in incroot usrlibdir libdir; do
      eval "ac_im_${ac_var}=\`\${MAKE-make} ${ac_var} 2>/dev/null | sed -n 's/^${ac_var}=//p'\`"
    done
    # Open Windows xmkmf reportedly sets LIBDIR instead of USRLIBDIR.
    for ac_extension in a so sl dylib la dll; do
      if test ! -f "${ac_im_usrlibdir}/libX11.${ac_extension}" &&
	 test -f "${ac_im_libdir}/libX11.${ac_extension}"; then
	ac_im_usrlibdir="${ac_im_libdir}"; break
      fi
    done
    # Screen out bogus values from the imake configuration.  They are
    # bogus both because they are the default anyway, and because
    # using them would break gcc on systems where it needs fixed includes.
    case ${ac_im_incroot} in
	/usr/include) ac_x_includes="" ;;
	*) test -f "${ac_im_incroot}/X11/Xos.h" && ac_x_includes="${ac_im_incroot}";;
    esac
    case ${ac_im_usrlibdir} in
	/usr/lib | /usr/lib64 | /lib | /lib64) ;;
	*) test -d "${ac_im_usrlibdir}" && ac_x_libraries="${ac_im_usrlibdir}" ;;
    esac
  fi
  cd ..
  rm -f -r conftest.dir
else
  # creating conftest.dir failed, so just check for the program:
  AC_PATH_PROG([XMKMF],[xmkmf])
fi
])dnl# _AC_PATH_X_XMKMF


dnl# _AC_PATH_X_DIRECT
dnl# -----------------
dnl# Internal subroutine of _AC_PATH_X.
dnl# Set ac_x_includes and/or ac_x_libraries.
m4_define([_AC_PATH_X_DIRECT],
[# Standard set of common directories for X headers.
# Check X11 before X11Rn because it is often a symlink to the current release.
ac_x_header_dirs='
/usr/X11/include
/usr/X11R7/include
/usr/X11R6/include
/usr/X11R5/include
/usr/X11R4/include

/usr/include/X11
/usr/include/X11R7
/usr/include/X11R6
/usr/include/X11R5
/usr/include/X11R4

/usr/local/X11/include
/usr/local/X11R7/include
/usr/local/X11R6/include
/usr/local/X11R5/include
/usr/local/X11R4/include

/usr/local/include/X11
/usr/local/include/X11R7
/usr/local/include/X11R6
/usr/local/include/X11R5
/usr/local/include/X11R4

/usr/X386/include
/usr/x386/include
/usr/XFree86/include/X11

/usr/include
/usr/local/include
/usr/unsupported/include
/usr/athena/include
/usr/local/x11r5/include
/usr/lpp/Xamples/include

/usr/openwin/include
/usr/openwin/share/include

/opt/X11/include'

AC_REQUIRE([AC_PROG_CPP])dnl

if test "x${ac_x_includes}" = "xno"; then
  # Guess where to find include files, by looking for Xlib.h.
  # First, try using that file with no special directory specified.
  AC_PREPROC_IFELSE([AC_LANG_SOURCE([@%:@include <X11/Xlib.h>])],
[# We can compile using X headers with no special include directory.
ac_x_includes=""],
[for ac_dir in ${ac_x_header_dirs}; do
  if test -r "${ac_dir}/X11/Xlib.h"; then
    ac_x_includes="${ac_dir}"
    break
  fi
done])
fi # end case where ${ac_x_includes} = no

AC_REQUIRE([AC_PROG_SED])dnl

if test "x${ac_x_libraries}" = "xno"; then
  # Check for the libraries.
  # See if we find them without any special options.
  # Do NOT add to ${LIBS} permanently.
  ac_save_LIBS="${LIBS}"
  LIBS="-lX11 ${LIBS}"
  AC_LINK_IFELSE([AC_LANG_PROGRAM([@%:@include <X11/Xlib.h>],
				  [XrmInitialize ()])],
		 [LIBS="${ac_save_LIBS}"
# We can link X programs with no special library path.
ac_x_libraries=""],
		 [LIBS="${ac_save_LIBS}"
for ac_dir in `AS_ECHO(["${ac_x_includes} ${ac_x_header_dirs}"]) | sed s/include/lib/g`
do
  # Do NOT even attempt the hair of trying to link an X program!
  for ac_extension in a so sl dylib la dll; do
    if test -r "${ac_dir}/libX11.${ac_extension}"; then
      ac_x_libraries="${ac_dir}"
      break 2
    fi
  done
done])
fi # end case where ${ac_x_libraries} = no
])# _AC_PATH_X_DIRECT


dnl# _AC_PATH_X
dnl# ----------
dnl# Compute ac_cv_have_x.
AC_DEFUN([_AC_PATH_X],
[AC_CACHE_VAL([ac_cv_have_x],
[# One or both of the vars are not set, and there is no cached value.
ac_x_includes=no ac_x_libraries=no
_AC_PATH_X_XMKMF
_AC_PATH_X_DIRECT
case ${ac_x_includes},${ac_x_libraries} in #(
  no,* | *,no | *\'*)
    # Did NOT find X, or a directory has "'" in its name.
    ac_cv_have_x="have_x=no";; #(
  *)
    # Record where we found X for the cache.
    ac_cv_have_x="have_x=yes\
	ac_x_includes='${ac_x_includes}'\
	ac_x_libraries='${ac_x_libraries}'"
esac])dnl
])dnl# _AC_PATH_X


dnl# AC_PATH_X
dnl# ---------
dnl# If we find X, set shell vars x_includes and x_libraries to the
dnl# paths, otherwise set no_x=yes.
dnl# Uses ac_ vars as temps to allow command line to override cache/checks.
dnl# --without-x overrides everything else, but does not touch the cache.
AN_HEADER([X11/Xlib.h],[AC_PATH_X])dnl
AC_DEFUN([AC_PATH_X],
[dnl# Document the X abnormal options inherited from history.
m4_divert_once([HELP_BEGIN],[
X features:
  --x-includes=DIR    X include files are in DIR
  --x-libraries=DIR   X library files are in DIR])dnl
AC_MSG_CHECKING([for X])dnl

AC_ARG_WITH([x],[AS_HELP_STRING([--with-x],[use the X Window System])])
# ${have_x} is `yes', `no', `disabled', or empty when we do not yet know.
if test "x${with_x}" = "xno"; then
  # The user explicitly disabled X.
  have_x=disabled
else
  case ${x_includes},${x_libraries} in #(
    *\'*)
      AC_MSG_ERROR([cannot use X directory names containing a literal single-quote character])
      ;; #(
    *,NONE | NONE,*)
      _AC_PATH_X
      ;; #(
    *)
      have_x=yes
      ;;
  esac
  eval "${ac_cv_have_x}"
fi # case where ${with_x} != no

AC_SUBST([with_x])dnl

# this was originally left uninitialized:
export no_x=yes
if test "x${have_x}" != "xyes"; then
  AC_MSG_RESULT([${have_x}])
  no_x=yes
else
  # now that no_x is initialized unconditionally, be sure to
  # change its value away from "yes" in this conditon:
  export no_x=no
  # If each of the values was on the command line, it overrides each guess.
  test "x${x_includes}" = "xNONE" && x_includes="${ac_x_includes}"
  test "x${x_libraries}" = "xNONE" && x_libraries="${ac_x_libraries}"
  # Update the cache value to reflect the command line values:
  ac_cv_have_x="have_x=yes\
	ac_x_includes='${x_includes}'\
	ac_x_libraries='${x_libraries}'"
  AC_MSG_RESULT([libraries ${x_libraries}, headers ${x_includes}])
fi
AC_SUBST([have_x])dnl
AC_SUBST([no_x])dnl
])dnl# AC_PATH_X



dnl# AC_PATH_XTRA
dnl# ------------
dnl# Find additional X libraries, magic flags, etc.
AC_DEFUN([AC_PATH_XTRA],
[AC_REQUIRE([AC_PATH_X])dnl
if test "x${no_x}" = "xyes"; then
  # Not all programs may use this symbol, but it does not hurt to define it.
  AC_DEFINE([X_DISPLAY_MISSING],[1],
	[Define to 1 if the X Window System is missing or not being used.])
  X_CFLAGS=""; X_PRE_LIBS=""; X_LIBS=""; X_EXTRA_LIBS="";
else
  if test -n "${x_includes}"; then
    X_CFLAGS="${X_CFLAGS} -I${x_includes}"
  fi

  # It would also be nice to do this for all -L options, not just this one.
  if test -n "${x_libraries}"; then
    X_LIBS="${X_LIBS} -L${x_libraries}"
    # For Solaris; some versions of Sun CC require a space after -R and
    # others require no space.  Words are not sufficient . . . .
    AC_MSG_CHECKING([whether -R must be followed by a space])
    ac_xsave_LIBS="${LIBS}"; LIBS="${LIBS} -R${x_libraries}"
    ac_xsave_[]_AC_LANG_ABBREV[]_werror_flag=$ac_[]_AC_LANG_ABBREV[]_werror_flag
    ac_[]_AC_LANG_ABBREV[]_werror_flag=yes
    AC_LINK_IFELSE([AC_LANG_PROGRAM([[]],[[]])],
      [AC_MSG_RESULT([no])
       X_LIBS="${X_LIBS} -R${x_libraries}"],
      [LIBS="${ac_xsave_LIBS} -R ${x_libraries}"
       AC_LINK_IFELSE([AC_LANG_PROGRAM([[]],[[]])],
	 [AC_MSG_RESULT([yes])
	  X_LIBS="${X_LIBS} -R ${x_libraries}"],
	 [AC_MSG_RESULT([neither works])])])
    ac_[]_AC_LANG_ABBREV[]_werror_flag=$ac_xsave_[]_AC_LANG_ABBREV[]_werror_flag
    LIBS="${ac_xsave_LIBS}"
  fi

  # Check for system-dependent libraries X programs must link with.
  # Do this before checking for the system-independent R6 libraries
  # (-lICE), since we may need -lsocket or whatever for X linking.

  if test "x${ISC}" = "xyes"; then
    X_EXTRA_LIBS="${X_EXTRA_LIBS} -lnsl_s -linet"
  else
    # Martyn Johnson says this is needed for Ultrix, if the X
    # libraries were built with DECnet support.  And Karl Berry says
    # the Alpha needs dnet_stub (dnet does not exist).
    ac_xsave_LIBS="${LIBS}"; LIBS="${LIBS} ${X_LIBS} -lX11"
    AC_LINK_IFELSE([AC_LANG_CALL([],[XOpenDisplay])],
		   [],
    [AC_CHECK_LIB([dnet],[dnet_ntoa],
       [X_EXTRA_LIBS="${X_EXTRA_LIBS} -ldnet"])
    if test "x${ac_cv_lib_dnet_dnet_ntoa}" = "xno"; then
      AC_CHECK_LIB([dnet_stub],[dnet_ntoa],
	[X_EXTRA_LIBS="${X_EXTRA_LIBS} -ldnet_stub"])
    fi])
    LIBS="${ac_xsave_LIBS}"

    # msh@cis.ufl.edu says -lnsl (and -lsocket) are needed for his 386/AT,
    # to get the SysV transport functions.
    # Chad R. Larson says the Pyramis MIS-ES running DC/OSx (SVR4)
    # needs -lnsl.
    # The nsl library prevents programs from opening the X display
    # on Irix 5.2, according to T.E. Dickey.
    # The functions gethostbyname, getservbyname, and inet_addr are
    # in -lbsd on LynxOS 3.0.1/i386, according to Lars Hecking.
    AC_CHECK_FUNC([gethostbyname])
    if test "x${ac_cv_func_gethostbyname}" = "xno"; then
      AC_CHECK_LIB([nsl],[gethostbyname],
        [X_EXTRA_LIBS="${X_EXTRA_LIBS} -lnsl"])
      if test "x${ac_cv_lib_nsl_gethostbyname}" = "xno"; then
	AC_CHECK_LIB([bsd],[gethostbyname],
          [X_EXTRA_LIBS="${X_EXTRA_LIBS} -lbsd"])
      fi
    fi

    # lieder@skyler.mavd.honeywell.com says without -lsocket,
    # socket/setsockopt and other routines are undefined under SCO ODT
    # 2.0.  But -lsocket is broken on IRIX 5.2 (and is not necessary
    # on later versions), says Simon Leinen: it contains gethostby*
    # variants that do NOT use the name server (or something).  -lsocket
    # must be given before -lnsl if both are needed.  We assume that
    # if connect needs -lnsl, so does gethostbyname.
    AC_CHECK_FUNC([connect])
    if test "x${ac_cv_func_connect}" = "xno"; then
      AC_CHECK_LIB([socket],[connect],
        [X_EXTRA_LIBS="-lsocket ${X_EXTRA_LIBS}"],[],[${X_EXTRA_LIBS}])
    fi

    # Guillermo Gomez says -lposix is necessary on A/UX.
    AC_CHECK_FUNC([remove])
    if test "x${ac_cv_func_remove}" = "xno"; then
      AC_CHECK_LIB([posix],[remove],
        [X_EXTRA_LIBS="${X_EXTRA_LIBS} -lposix"])
    fi

    # BSDI BSD/OS 2.1 needs -lipc for XOpenDisplay.
    AC_CHECK_FUNC([shmat])
    if test "x${ac_cv_func_shmat}" = "xno"; then
      AC_CHECK_LIB([ipc],[shmat],[X_EXTRA_LIBS="${X_EXTRA_LIBS} -lipc"])
    fi
  fi

  # Check for libraries that X11R6 Xt/Xaw programs need.
  ac_save_LDFLAGS="${LDFLAGS}"
  test -n "${x_libraries}" && LDFLAGS="${LDFLAGS} -L${x_libraries}"
  # SM needs ICE to (dynamically) link under SunOS 4.x (so we have to
  # check for ICE first), but we must link in the order -lSM -lICE or
  # we get undefined symbols.  So assume we have SM if we have ICE.
  # These have to be linked with before -lX11, unlike the other
  # libraries we check for below, so use a different variable.
  # John Interrante, Karl Berry
  AC_CHECK_LIB([ICE],[IceConnectionNumber],
    [X_PRE_LIBS="${X_PRE_LIBS} -lSM -lICE"],[],[${X_EXTRA_LIBS}])
  LDFLAGS="${ac_save_LDFLAGS}"

  # ...anything else?
fi

AC_SUBST([X_CFLAGS])dnl
AC_SUBST([X_PRE_LIBS])dnl
AC_SUBST([X_LIBS])dnl
AC_SUBST([X_EXTRA_LIBS])dnl
])dnl# AC_PATH_XTRA
