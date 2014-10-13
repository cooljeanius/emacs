dnl#                                               -*- Autoconf -*-
dnl#  Autoconf script for GNU Emacs
dnl# To rebuild the `configure' script from this, execute the command
dnl# 	autoconf
dnl# in the directory containing this script.
dnl# If you changed any AC_DEFINES, also run autoheader.
dnl#
dnl# Copyright (C) 1994-1996, 1999-2014 Free Software Foundation, Inc.
dnl#
dnl#  This file is part of GNU Emacs.
dnl#
dnl#  GNU Emacs is free software: you can redistribute it and/or modify
dnl#  it under the terms of the GNU General Public License as published by
dnl#  the Free Software Foundation, either version 3 of the License, or
dnl#  (at your option) any later version.
dnl#
dnl#  GNU Emacs is distributed in the hope that it will be useful,
dnl#  but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl#  GNU General Public License for more details.
dnl#
dnl#  You should have received a copy of the GNU General Public License
dnl#  along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.

AC_PREREQ([2.69])
AC_INIT([emacs],[24.3.50],[bug-gnu-emacs@gnu.org])dnl

dnl# We get MINGW64 with MSYS2:
if test "x${MSYSTEM}" = "xMINGW32" -o "x${MSYSTEM}" = "xMINGW64"
then
  AC_MSG_NOTICE([sourcing ${srcdir}/nt/mingw-cfg.site])
  . ${srcdir}/nt/mingw-cfg.site

  case ${srcdir} in
    /* | ?:*)
      # srcdir is an absolute path.  In this case, force the format
      # "/c/foo/bar", to simplify later conversions to native Windows
      # format ("c:/foo/bar")
      srcdir=`cd "${srcdir}" && pwd -W`
      srcdir="/${srcdir:0:1}${srcdir:2}"
      ;;
  esac
fi

dnl# Set emacs_config_options to the options of 'configure', quoted for
dnl# the shell, and then quoted again for a C string. Separate options
dnl# with spaces. Add some environment variables, if they were passed via
dnl# the environment rather than on the command-line.
emacs_config_options=""
optsep=""
dnl# This is the documented way to record the args passed to configure,
dnl# rather than ${ac_configure_args}.
for opt in ${1+"$@"} CFLAGS CPPFLAGS LDFLAGS; do
  case ${opt} in
    -n | --no-create | --no-recursion)
      continue ;;
    CFLAGS | CPPFLAGS | LDFLAGS)
      eval 'test "${'$opt'+set}" = set' || continue
      case " $*" in
	*" ${opt}="*) continue ;;
      esac
      eval opt=${opt}=\$$opt ;;
  esac

  emacs_shell_specials=$IFS\''"#$&()*;<>?@<:@\\`{|~'
  case ${opt} in
    *[["${emacs_shell_specials}"]]*)
      case ${opt} in
	*\'*)
	  emacs_quote_apostrophes="s/'/'\\\\''/g"
	  opt=`AS_ECHO(["${opt}"]) | sed "${emacs_quote_apostrophes}"` ;;
      esac
      opt="'${opt}'"
      case ${opt} in
	*[['"\\']]*)
	  emacs_quote_for_c='s/[["\\]]/\\&/g; $!s/$/\\n\\/'
	  opt=`AS_ECHO(["${opt}"]) | sed "${emacs_quote_for_c}"` ;;
      esac ;;
  esac
  AS_VAR_APPEND([emacs_config_options],["${optsep}${opt}"])
  optsep=' '
done

AC_CONFIG_HEADERS([src/config.h:src/config.in])
AC_CONFIG_SRCDIR([src/lisp.h])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
dnl# "lib" is where gnulib lives, so it makes sense to put libobjs there
dnl# with their sources, but "src" is where the libobjs are actually needed
dnl# (and maybe in "lib-src", too), so I am not sure if either of those
dnl# would be better here:
AC_CONFIG_LIBOBJ_DIR([lib])
if test "x${LIBOBJDIR}" = "x"; then
  # The trailing slashes at the end of each of these paths are necessary:
  if test "x${ac_abs_top_srcdir}" != "x"; then
    test -z "${LIBOBJDIR}" && test -n "${ac_abs_top_srcdir}" && test -d "${ac_abs_top_srcdir}/lib/" && export LIBOBJDIR="${ac_abs_top_srcdir}/lib/"
  elif test "x${ac_top_srcdir}" != "x"; then
    test -z "${LIBOBJDIR}" && test -n "${ac_top_srcdir}" && test -d "${ac_top_srcdir}/lib/" && export LIBOBJDIR="${ac_top_srcdir}/lib/"
  elif test "x${srcdir}" = "x."; then
    # the check for srcdir is not to actually use it, but to verify that
    # `pwd` will work:
    test -z "${LIBOBJDIR}" && test -n "`pwd`" && test -d "`pwd`/lib/" && export LIBOBJDIR="`pwd`/lib/"
  else
    test -z "${LIBOBJDIR}"
    AC_MSG_WARN([cannot figure out how to export a decent value for LIBOBJDIR])
  fi
fi
AC_SUBST([ac_abs_top_srcdir])dnl
AC_SUBST([ac_top_srcdir])dnl
AC_SUBST([LIBOBJDIR])dnl

dnl# do similarly with SRCROOT:
if test "x${SRCROOT}" = "x"; then
  if test "x${ac_abs_top_srcdir}" != "x"; then
    test -z "${SRCROOT}" && test -n "${ac_abs_top_srcdir}" && export SRCROOT="${ac_abs_top_srcdir}"
  elif test "x${ac_top_srcdir}" != "x"; then
    test -z "${SRCROOT}" && test -n "${ac_top_srcdir}" && export LIBOBJDIR="${SRCROOT}"
  elif test "x${srcdir}" = "x."; then
    # the check for srcdir is not to actually use it, but to verify that
    # `pwd` will work:
    test -z "${SRCROOT}" && test -n "`pwd`" && export SRCROOT="`pwd`"
  else
    test -z "${SRCROOT}"
    AC_MSG_WARN([cannot figure out how to export a good value for SRCROOT])
  fi
fi
AC_SUBST([SRCROOT])dnl

xcsdkdir=""
AC_CHECK_PROGS([XCRUN],[xcrun])
if test -n "${XCRUN}"; then
  if test -z "${MAKE}"; then
    dnl# Call the variable MAKE_PROG, not MAKE, to avoid confusion with
    dnl# SET_MAKE and with the usual MAKE variable that 'make' itself uses.
    AC_CHECK_PROG([MAKE_PROG],[make],[yes])
    if test -z "${MAKE_PROG}"; then
      MAKE="${XCRUN} MAKE"
      export MAKE
      xcsdkdir=`${XCRUN} --show-sdk-path 2>/dev/null`
      AC_MSG_NOTICE([xcsdkdir is "${xcsdkdir}"])
      AC_SUBST([xcsdkdir])
    fi
  fi
else
  if test -z "${MAKE}"; then
    dnl# since we do NOT have xcrun in this case, we can check more names
    dnl# for 'make' than xcrun knows about:
    AC_CHECK_PROGS([MAKE_PROG],[make gmake gnumake remake])
  fi
fi

dnl# Version is fairly arbitrary, older versions might work too:
AC_MSG_CHECKING([automake things])
AC_MSG_RESULT([ ])
AM_INIT_AUTOMAKE([1.11 gnits tar-pax dist-bzip2 dist-xz])
AM_MAINTAINER_MODE

dnl# Support for --program-prefix, --program-suffix and
dnl# --program-transform-name options
dnl# (I thought that this would already have been called by automake?)
AC_ARG_PROGRAM

dnl# It is important that variables on the RHS not be expanded here,
dnl# hence the single quotes.  This is per the GNU coding standards, see
dnl# (autoconf) Installation Directory Variables
dnl# See also epaths.h below.
lispdir='${datadir}/emacs/${version}/lisp'
standardlisppath='${lispdir}'
locallisppath='${datadir}/emacs/${version}/site-lisp:'\
'${datadir}/emacs/site-lisp'
lisppath='${locallisppath}:${standardlisppath}'
etcdir='${datadir}/emacs/${version}/etc'
archlibdir='${libexecdir}/emacs/${version}/${configuration}'
etcdocdir='${datadir}/emacs/${version}/etc'
gamedir='${localstatedir}/games/emacs'

dnl# Special option to disable the most of other options.
AC_ARG_WITH([all],
[AS_HELP_STRING([--without-all],
		[omit almost all features and build
		small executable with minimal dependencies])],
  [with_features="${withval}"],
  [with_features=yes])dnl

dnl# the definitions of the option checking macros have been moved to:
m4_include([m4/options.m4])dnl

OPTION_DEFAULT_ON([pop],[do NOT support POP mail retrieval with movemail])
if test "x${with_pop}" = "xyes"; then
   AC_DEFINE([MAIL_USE_POP])
fi
AH_TEMPLATE([MAIL_USE_POP],[Define to support POP mail retrieval.])dnl

OPTION_DEFAULT_OFF([kerberos],[support Kerberos-authenticated POP])
if test "x${with_kerberos}" != "xno"; then
   AC_DEFINE([KERBEROS])
fi
AH_TEMPLATE([KERBEROS],
	[Define to support Kerberos-authenticated POP mail retrieval.])dnl

OPTION_DEFAULT_OFF([kerberos5],
                   [support Kerberos version 5 authenticated POP])
if test "x${with_kerberos5}" != "xno"; then
  if test "x${with_kerberos}" = "xno"; then
    with_kerberos=yes
    AC_DEFINE([KERBEROS])
  fi
  AC_DEFINE([KERBEROS5],[1],
            [Define to use Kerberos 5 instead of Kerberos 4.])
fi

OPTION_DEFAULT_OFF([hesiod],[support Hesiod to get the POP server host])
dnl# FIXME hesiod support may not be present, so it seems like an error
dnl# to define, or at least use, this unconditionally.
if test "x${with_hesiod}" != "xno"; then
  AC_DEFINE([HESIOD],[1],
    [Define to 1 to support using a Hesiod database to find a POP server])
fi

OPTION_DEFAULT_OFF([mmdf],[support MMDF mailboxes])
if test "x${with_mmdf}" != "xno"; then
   AC_DEFINE([MAIL_USE_MMDF],[1],
             [Define to 1 to support MMDF mailboxes in movemail.])
fi

OPTION_DEFAULT_OFF([mail-unlink],
                   [unlink, rather than empty, mail spool after reading])
if test "x${with_mail_unlink}" != "xno"; then
   AC_DEFINE([MAIL_UNLINK_SPOOL],[1],
         [Define to 1 to unlink (vs. empty) the mail spool after reading])
fi

AC_ARG_WITH([mailhost],[AS_HELP_STRING([--with-mailhost=HOSTNAME],
    [string giving default POP mail host])],
    [AC_DEFINE_UNQUOTED([MAILHOST],["${withval}"],
                        [String giving fallback POP mail host.])
])dnl

AC_ARG_WITH([sound],[AS_HELP_STRING([--with-sound=VALUE],
  [compile with sound support (VALUE one of: yes, alsa, oss, bsd-ossaudio,
no; default yes).  Only for GNU/Linux, FreeBSD, NetBSD, MinGW.])],
  [ case "${withval}" in
      yes|no|alsa|oss|bsd-ossaudio) val="${withval}" ;;
      *) AC_MSG_ERROR([`--with-sound=${withval}' is invalid;
this option's value should be `yes', `no', `alsa', `oss', or `bsd-ossaudio'.])
      ;;
    esac
    with_sound="${val}"
  ],[with_sound="${with_features}"])dnl

dnl# FIXME: currently it is not the last.
dnl# This should be the last --with option, because --with-x is
dnl# added later on when we find the file name of X, and it is best to
dnl# keep them together visually.
AC_ARG_WITH([x-toolkit],[AS_HELP_STRING([--with-x-toolkit=KIT],
 [use KIT as an X toolkit for the emacs GUI
  (KIT may be one of: yes or gtk, gtk2, gtk3, lucid or athena, motif, no)
  (this option may also have seemingly unrelated side-effects)])],
[	  case "${withval}" in
	    y | ye | yes )	val=gtk ;;
	    n | no )		val=no  ;;
	    l | lu | luc | luci | lucid )	val=lucid ;;
	    a | at | ath | athe | athen | athena )	val=athena ;;
	    m | mo | mot | moti | motif )	val=motif ;;
	    g | gt | gtk  )	val=gtk ;;
	    gtk2  )	val=gtk2 ;;
	    gtk3  )	val=gtk3 ;;
	    * )
AC_MSG_ERROR([`--with-x-toolkit=${withval}' is invalid;
this option's value should be `yes', `no', `lucid', `athena', `motif',
`gtk', `gtk2' or `gtk3'.  `yes' and `gtk' are synonyms.
`athena' and `lucid' are synonyms.])
	    ;;
	  esac
	  with_x_toolkit="${val}"
])dnl

OPTION_DEFAULT_OFF([wide-int],
                   [prefer wide Emacs integers (typically 62-bit)])
if test "x${with_wide_int}" = "xyes"; then
  AC_DEFINE([WIDE_EMACS_INT],[1],
            [Use long long for EMACS_INT if available.])
fi

dnl# _ON results in a '--without' option in the --help output, so
dnl# the help text should refer to "do NOT compile", etc.
OPTION_DEFAULT_ON([xpm],[do NOT compile with XPM image support])
OPTION_DEFAULT_ON([jpeg],[do NOT compile with JPEG image support])
OPTION_DEFAULT_ON([tiff],[do NOT compile with TIFF image support])
OPTION_DEFAULT_ON([gif],[do NOT compile with GIF image support])
OPTION_DEFAULT_ON([png],[do NOT compile with PNG image support])
OPTION_DEFAULT_ON([rsvg],[do NOT compile with SVG image support])
OPTION_DEFAULT_ON([xml2],[do NOT compile with XML parsing support])
OPTION_DEFAULT_ON([imagemagick],
                  [do NOT compile with ImageMagick image support])dnl

OPTION_DEFAULT_ON([xft],[do NOT use XFT for anti aliased fonts])
OPTION_DEFAULT_ON([libotf],[do NOT use libotf for OpenType font support])
OPTION_DEFAULT_ON([m17n-flt],[do NOT use m17n-flt for text shaping])dnl

OPTION_DEFAULT_ON([toolkit-scroll-bars],
                  [do NOT use Motif or Xaw3d scroll bars])
OPTION_DEFAULT_ON([xaw3d],[do NOT use Xaw3d])dnl
OPTION_DEFAULT_ON([xim],[do NOT use X11 XIM])dnl

OPTION_DEFAULT_OFF([ns],[use NeXTstep (Cocoa or GNUstep) windowing system])
OPTION_DEFAULT_OFF([w32],[use native MS Windows GUI in a Cygwin build])dnl

OPTION_DEFAULT_ON([gpm],
              [do NOT use -lgpm for mouse support on a GNU/Linux console])
OPTION_DEFAULT_ON([dbus],[do NOT compile with D-Bus support])
OPTION_DEFAULT_ON([gconf],[do NOT compile with GConf support])
OPTION_DEFAULT_ON([gsettings],[do NOT compile with GSettings support])
OPTION_DEFAULT_ON([selinux],[do NOT compile with SELinux support])
OPTION_DEFAULT_ON([gnutls],[do NOT use -lgnutls for SSL/TLS support])
OPTION_DEFAULT_ON([zlib],
                  [do NOT compile with zlib decompression support])dnl

AC_ARG_WITH([file-notification],
            [AS_HELP_STRING([--with-file-notification=LIB],
 [use a file notification library (LIB one of: yes, gfile, inotify, w32, no)])],
 [ case "${withval}" in
    y | ye | yes )	val=yes ;;
    n | no )		val=no  ;;
    g | gf | gfi | gfil | gfile )	val=gfile ;;
    i | in | ino | inot | inoti | inotif | inotify )	val=inotify ;;
    w | w3 | w32 )	val=w32 ;;
    * ) AC_MSG_ERROR([`--with-file-notification=${withval}' is invalid;
this option's value should be `yes', `no', `gfile', `inotify' or `w32'.
`yes' is a synonym for `w32' on MS-Windows, for `no' on Nextstep/darwin,
otherwise for the first of `gfile' or `inotify' that is usable.])
    ;;
   esac
   with_file_notification="${val}"
 ],[with_file_notification="${with_features}"])dnl

## For the times when you want to build Emacs but do NOT have
## a suitable makeinfo, and can live without the manuals.
dnl# http://lists.gnu.org/archive/html/emacs-devel/2008-04/msg01844.html
OPTION_DEFAULT_ON([makeinfo],
                  [do NOT require makeinfo for building manuals])dnl

## Makefile.in needs the cache file name.
AC_SUBST([cache_file])dnl

## This is an option because I do not know if all info/man support
## compressed files, nor how to test if they do so.
OPTION_DEFAULT_ON([compress-install],
  [do NOT compress some files (.el, .info, etc.) when installing.  Equivalent to:
make GZIP_PROG="" install])dnl

AC_ARG_WITH([pkg-config-prog],dnl
[AS_HELP_STRING([--with-pkg-config-prog=FILENAME],
                [file name of pkg-config for finding GTK and librsvg])])
if test "X${with_pkg_config_prog}" != "X"; then
   if test "X${with_pkg_config_prog}" != "Xyes"; then
      PKG_CONFIG="${with_pkg_config_prog}"
   fi
fi

AC_ARG_WITH([gameuser],dnl
[AS_HELP_STRING([--with-gameuser=USER],
                [user for shared game score files])])
test "X${with_gameuser}" != "X" && test "X${with_gameuser}" != "Xyes" \
  && gameuser="${with_gameuser}"
test "X${gameuser}" = "X" && gameuser=games

AC_ARG_WITH([gnustep-conf],dnl
[AS_HELP_STRING([--with-gnustep-conf=FILENAME],
   [name of GNUstep.conf; default ${GNUSTEP_CONFIG_FILE}, or /etc/GNUstep/GNUstep.conf])])
test "X${with_gnustep_conf}" != "X" && test "X${with_gnustep_conf}" != "Xyes" && \
  GNUSTEP_CONFIG_FILE="${with_gnustep_conf}"
test "X${GNUSTEP_CONFIG_FILE}" = "X" && \
     GNUSTEP_CONFIG_FILE=/etc/GNUstep/GNUstep.conf

AC_ARG_ENABLE([ns-self-contained],
[AS_HELP_STRING([--disable-ns-self-contained],
                [disable self contained build under NeXTstep/darwin])],
   [EN_NS_SELF_CONTAINED="${enableval}"],
   [EN_NS_SELF_CONTAINED=yes])dnl

AC_ARG_ENABLE([locallisppath],
[AS_HELP_STRING([--enable-locallisppath=PATH],
                [directories Emacs should search for lisp files specific
		 to this site])],[
if test "x${enableval}" = "xno"; then
  locallisppath=""
elif test "x${enableval}" != "xyes"; then
  locallisppath="${enableval}"
else
  m4_ifdef([AM_PATH_LISPDIR],[
    AM_PATH_LISPDIR
  ],[
    test ! -z "${lispdir}" && test -d "${lispdir}"
  ])
  if test "x${locallisppath}" = "x"; then
    test -z "${locallisppath}" && test -n "${lispdir}" && locallisppath="${lispdir}"
  fi
fi
])dnl

AC_ARG_ENABLE([checking],
[AS_HELP_STRING([--enable-checking@<:@=LIST@:>@],
		[enable expensive run-time checks.  By supplying
		 a comma-separated list of checks in LIST,
		 enable only specific categories of checks.
		 Categories are: all,yes,no.
		 Flags are: stringbytes, stringoverrun, stringfreelist,
		 xmallocoverrun, conslist, glyphs])],
[ac_checking_flags="${enableval}"],[])
IFS="${IFS= 	}"; ac_save_IFS="${IFS}"; IFS="${IFS},"
for check in ${ac_checking_flags}
do
	case ${check} in
	# these set all the flags to specific states:
	yes)		ac_enable_checking=1 ;;
	no)		ac_enable_checking="" ;
			ac_gc_check_stringbytes="" ;
	                ac_gc_check_string_overrun="" ;
	                ac_gc_check_string_free_list="" ;
	                ac_xmalloc_overrun="" ;
	                ac_gc_check_cons_list="" ;
			ac_glyphs_debug="" ;;
	all)		ac_enable_checking=1 ;
			ac_gc_check_stringbytes=1 ;
	                ac_gc_check_string_overrun=1 ;
	                ac_gc_check_string_free_list=1 ;
	                ac_xmalloc_overrun=1 ;
	                ac_gc_check_cons_list=1 ;
			ac_glyphs_debug=1 ;;
	# these enable particular checks:
	stringbytes) 	ac_gc_check_stringbytes=1 ;;
	stringoverrun) 	ac_gc_check_string_overrun=1 ;;
	stringfreelist) ac_gc_check_string_free_list=1 ;;
	xmallocoverrun)	ac_xmalloc_overrun=1 ;;
	conslist)	ac_gc_check_cons_list=1 ;;
	glyphs)		ac_glyphs_debug=1 ;;
	*)	AC_MSG_ERROR([unknown check category ${check}]) ;;
	esac
        # only print this message after we have had a chance to error out:
        AC_MSG_NOTICE([enabled runtime checks of category ${check}])
done
IFS="${ac_save_IFS}"

if test "x${ac_enable_checking}" != "x"; then
  AC_DEFINE([ENABLE_CHECKING],[1],
[Define to 1 if expensive run-time data type and consistency checks are enabled.])
fi
if test "x${ac_gc_check_stringbytes}" != "x"; then
  AC_DEFINE([GC_CHECK_STRING_BYTES],[1],
[Define this temporarily to hunt a bug.  If defined, the size of
   strings is redundantly recorded in sdata structures so that it can
   be compared to the sizes recorded in Lisp strings.])
fi
if test "x${ac_gc_check_string_overrun}" != "x"; then
  AC_DEFINE([GC_CHECK_STRING_OVERRUN],[1],
[Define this to check for short string overrun.])
fi
if test "x${ac_gc_check_string_free_list}" != "x"; then
  AC_DEFINE([GC_CHECK_STRING_FREE_LIST],[1],
[Define this to check the string free list.])
fi
if test "x${ac_xmalloc_overrun}" != "x"; then
  AC_DEFINE([XMALLOC_OVERRUN_CHECK],[1],
[Define this to check for malloc buffer overrun.])
fi
if test "x$ac_gc_check_cons_list" != "x"; then
  AC_DEFINE([GC_CHECK_CONS_LIST],[1],
[Define this to check for errors in cons list.])
fi
if test "x${ac_glyphs_debug}" != "x"; then
  AC_DEFINE([GLYPH_DEBUG],[1],
[Define this to enable glyphs debugging code.])
fi

AC_ARG_ENABLE([check-lisp-object-type],
[AS_HELP_STRING([--enable-check-lisp-object-type],
                [enable compile time checks for the Lisp_Object data type.
		This is useful for development for catching certain types
		of bugs.])],
[if test "x${enableval}" != "xno"; then
   AC_DEFINE([CHECK_LISP_OBJECT_TYPE],[1],
   [Define this to enable compile time checks for the Lisp_Object data type.])
fi
])dnl

dnl# The name of this option is unfortunate. It predates, and has no
dnl# relation to, the "sampling-based elisp profiler" added in 24.3.
dnl# Actually, it stops it working.
dnl# http://lists.gnu.org/archive/html/emacs-devel/2012-11/msg00393.html
AC_ARG_ENABLE([profiling],
[AS_HELP_STRING([--enable-profiling],
		[build emacs with low-level, gprof profiling support.
                 Mainly useful for debugging Emacs itself. May not work on
                 all platforms. Stops profiler.el working.])],
[ac_enable_profiling="${enableval}"],[])
if test "x${ac_enable_profiling}" != "x"; then
   PROFILING_CFLAGS="-DPROFILING=1 -pg"
else
   PROFILING_CFLAGS=""
fi
AC_SUBST([PROFILING_CFLAGS])dnl

AC_ARG_ENABLE([autodepend],
[AS_HELP_STRING([--enable-autodepend],
		[automatically generate dependencies to .h-files.
 		 Requires GNU Make and Gcc. Enabled if GNU Make and Gcc is
 		 found])],
[ac_enable_autodepend="${enableval}"],[ac_enable_autodepend=yes])dnl

AC_ARG_ENABLE([gtk-deprecation-warnings],
[AS_HELP_STRING([--enable-gtk-deprecation-warnings],
		[Show Gtk+/Gdk deprecation warnings for Gtk+ >= 3.0])],
[ac_enable_gtk_deprecation_warnings="${enableval}"],[])dnl

### Check for grep before using it:
dnl# The definition of the macro for it has been moved to:
m4_include([m4/grep.m4])dnl
AC_REQUIRE_GREPS

### Canonicalize the configuration name.

AC_MSG_CHECKING([canonical system])
AC_MSG_RESULT([ ])dnl

AC_CANONICAL_HOST
canonical="${host}"
configuration=${host_alias-${build_alias-$host}}

if test -z "${HOST_ARCHITECTURE}" -a -n "${host_cpu}"; then
  export HOST_ARCHITECTURE="${host_cpu}"
  AC_SUBST([HOST_ARCHITECTURE])
fi

if test -z "${RC_ARCHS}"; then
  if test -n "${HOST_ARCHITECTURE}"; then
    export RC_ARCHS="${HOST_ARCHITECTURE}"
  fi
  if test -n "${TARGET_ARCHITECTURE}" -a "x${HOST_ARCHITECTURE}" != "x${TARGET_ARCHITECTURE}"; then
    export RC_ARCHS="${RC_ARCHS} ${TARGET_ARCHITECTURE}"
  fi
fi
AC_SUBST([RC_ARCHS])dnl

dnl# This used to use changequote, but, apart from `changequote is evil'
dnl# per the autoconf manual, we can speed up autoconf somewhat by quoting
dnl# the great gob of text. Thus it is not processed for possible
dnl# expansion.
dnl# Just make sure the brackets remain balanced.
dnl#
dnl# Since Emacs cannot find matching pairs of quotes, boundaries are
dnl# indicated by comments.
dnl# this batch of m4-quotation (block 1) begins:
[

### If you add support for a new configuration, add code to this
### switch statement to recognize your configuration name and select
### the appropriate opsys.

### As far as handling version numbers on operating systems is
### concerned, make sure things will fail in a fixable way.  If
### /etc/MACHINES does NOT say anything about version numbers, be
### prepared to handle anything reasonably.  If version numbers
### matter, be sure /etc/MACHINES says something about it.

opsys='' unported=no
case "${canonical}" in

  ## GNU/Linux and similar ports:
  *-*-linux* )
    opsys=gnu-linux
  ;;

  ## FreeBSD ports:
  *-*-freebsd* )
    opsys=freebsd
  ;;

  ## DragonFly ports:
  *-*-dragonfly* )
    opsys=dragonfly
  ;;

  ## FreeBSD kernel + glibc based userland:
  *-*-kfreebsd*gnu* )
    opsys=gnu-kfreebsd
  ;;

  ## NetBSD ports:
  *-*-netbsd* )
    opsys=netbsd
  ;;

  ## OpenBSD ports:
  *-*-openbsd* )
    opsys=openbsd
  ;;

  ## Apple Darwin / Mac OS X:
  *-apple-darwin* )
    case "${canonical}" in
      i[3456]86-* )  ;;
      powerpc-* )    ;;
      x86_64-* )     ;;
      * )            unported=yes ;;
    esac
    opsys=darwin
    ## Use Fink packages if available.
    ## FIXME: find a better way to do this: http://debbugs.gnu.org/11507
    ## How about ony doing it if a Fink-specific environment variable is
    ## present? Like this:
      if test "x${DBUS_FINK_SESSION_BUS_SOCKET}" != "x"; then
        ## force check for Fink in its default prefix, because a Fink in a
        ## non-default prefix may be mis-configured:
        if test -x /sw/bin/fink; then
          echo "Found installation of Fink in its default prefix; checking to see if we should use packages from it..."
          FINK_ARCH=`/sw/bin/fink --version --quiet | grep Distribution | cut -d\, -f3 | tr -d [:blank:]`
          CANONICAL_ARCH=`echo ${canonical} | cut -d\- -f1`
          if test "x${FINK_ARCH}" = "x${CANONICAL_ARCH}"; then
            if test -d /sw/include && test -d /sw/lib; then
              echo "configure: deciding to use Fink packages"
              GCC_TEST_OPTIONS="-I/sw/include -L/sw/lib"
              NON_GCC_TEST_OPTIONS="${GCC_TEST_OPTIONS}"
            fi
          else
            echo "Fink's arch (${FINK_ARCH}) does not match the arch we are configuring for (${CANONICAL_ARCH}), so not using packages from it"
          fi
        else
          echo "A Fink-specific environment variable is set, but Fink is not installed in its default prefix (i.e. '/sw'), so not using packages from it."
        fi
      else
        ## (TODO: put checks for other package managers here?)
        ## (e.g.: MacPorts, Homebrew, pkgsrc, others?)
        echo "Fink not found, and checks for other package managers are currently unimplemented..."
      fi
  ;;

  ## Cygwin ports:
  *-*-cygwin )
    opsys=cygwin
  ;;

  ## HP 9000 series 700 and 800, running HP/UX:
  hppa*-hp-hpux10.2* )
    opsys=hpux10-20
  ;;
  hppa*-hp-hpux1[1-9]* )
    opsys=hpux11
    CFLAGS="-D_INCLUDE__STDC_A1_SOURCE ${CFLAGS}"
  ;;

  ## IBM machines:
  rs6000-ibm-aix4.[23]* )
    opsys=aix4-2
  ;;
  powerpc-ibm-aix4.[23]*  )
    opsys=aix4-2
  ;;
  rs6000-ibm-aix[56]* )
    opsys=aix4-2
  ;;
  powerpc-ibm-aix[56]*  )
    opsys=aix4-2
  ;;

  ## Silicon Graphics machines:
  ## Iris 4D:
  mips-sgi-irix6.5 )
    opsys=irix6-5
    # Without defining _LANGUAGE_C, things get masked out in the headers
    # so that, for instance, grepping for `free' in stdlib.h fails and
    # AC_HEADER_STD_C fails.   (MIPSPro 7.2.1.2m compilers, Irix 6.5.3m).
    NON_GCC_TEST_OPTIONS="-D_LANGUAGE_C"
  ;;

  ## Suns:
  *-sun-solaris* \
    | i[3456]86-*-solaris2* | i[3456]86-*-sunos5* \
    | x86_64-*-solaris2*    | x86_64-*-sunos5*)
    case "${canonical}" in
      i[3456]86-*-* )   ;;
      amd64-*-*|x86_64-*-*) ;;
      sparc* )		;;
      * )		unported=yes ;;
    esac
    case "${canonical}" in
      *-sunos5.[1-9][0-9]* | *-solaris2.[1-9][0-9]* )
		opsys=sol2-10
		emacs_check_sunpro_c=yes
		;;
      *-sunos5.6* | *-solaris2.6* )
		opsys=sol2-6
		RANLIB="ar -ts"
		;;
      *-sunos5.[7-9]* | *-solaris2.[7-9]* )
		opsys=sol2-6
		emacs_check_sunpro_c=yes
		;;
    esac
    ## Watch out for a compiler that we know will not work.
    case "${canonical}" in
     *-solaris* | *-sunos5* )
		if test "x${CC}" = "x/usr/ucb/cc"; then
		  ## /usr/ucb/cc does NOT work;
		  ## we should find some other compiler that does work.
		  unset CC
		fi
		;;
      *) ;;
    esac
  ;;

  ## Intel 386 machines where we do NOT care about the manufacturer.
  i[3456]86-*-* )
    case "${canonical}" in
      *-darwin* )               opsys=darwin ;;
      *-mingw32 )
		opsys=mingw32
		# MinGW overrides and adds some system headers in nt/inc.
		GCC_TEST_OPTIONS="-I ${srcdir}/nt/inc"
		;;
      *-sysv4.2uw* )	  	opsys=unixware ;;
      *-sysv5uw* )	  	opsys=unixware ;;
      *-sysv5OpenUNIX* )  	opsys=unixware ;;
      ## Otherwise, we will fall through to the generic opsys code at the
      ## bottom.
    esac
  ;;

  # MinGW64:
  x86_64-*-* )
    case "${canonical}" in
      *-mingw32 )
		opsys=mingw32
		# MinGW overrides and adds some system headers in nt/inc.
		GCC_TEST_OPTIONS="-I ${srcdir}/nt/inc"
		;;
      ## Otherwise, we will fall through to the generic opsys code at the
      ## bottom.
    esac
  ;;

  * )
    unported=yes
  ;;
esac

### If the code above did NOT choose an operating system, just choose
### an operating system based on the configuration name.  You really
### only want to use this when you have no idea what the right
### operating system is; if you know what operating systems a machine
### runs, it is cleaner to make it explicit in the case statement
### above.
if test x"${opsys}" = x""; then
  case "${canonical}" in
    *-gnu* )				opsys=gnu ;;
    * )
      unported=yes
    ;;
  esac
fi

]
dnl# this batch of m4-quotation (block 1) ends, moving to also use similar
dnl# blocks in other places...

if test "x${unported}" = "xyes"; then
  AC_MSG_ERROR([Emacs does not support `${canonical}' systems.
If you think it should, please send a report to bug-gnu-emacs@gnu.org.
Check `etc/MACHINES' for recognized configuration names.])
fi
dnl# end check for ported-ness

#### Choose a compiler.
AC_MSG_CHECKING([compiler])
AC_MSG_RESULT([ ])dnl

AC_LANG([C])dnl

dnl# Sets GCC=yes if using gcc.
AC_PROG_CC([gcc cc cl clang "${XCRUN} gcc" "${XCRUN} clang"])
if test -n "${XCRUN}"; then
  AC_CHECK_PROGS([AR],[ar "${XCRUN} ar"])
  test -n "${AR}" && export AR
else
  test -z "${AR}"
fi

AM_PROG_CC_C_O
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_CXXCPP
AC_PROG_GCC_TRADITIONAL
AC_PROG_OBJC
AC_PROG_OBJCPP
dnl# This next one is new; I put it in m4/grep.m4, which should already be
dnl# included above:
AC_EGREP_PREPROC_SANITY

dnl# as above with the giant gob of m4-quotation (this is block 2):
[
if test "x${GCC}" = "xyes"; then
  test "x${GCC_TEST_OPTIONS}" != "x" && CC="${CC} ${GCC_TEST_OPTIONS}"
else
  test "x${NON_GCC_TEST_OPTIONS}" != "x" && CC="${CC} ${NON_GCC_TEST_OPTIONS}"
fi
]
dnl# end smaller m4-quotations (block 2)

dnl# This is used in lib/Makefile.am to use nt/gnulib.mk, the
dnl# alternative to lib/gnulib.mk, so as to avoid generating header files
dnl# that clash with MinGW.
AM_CONDITIONAL([BUILDING_FOR_WINDOWSNT],[test "x${opsys}" = "xmingw32"])dnl

AC_MSG_NOTICE([done with initial compiler checks, now initializing gnulib])
# Avoid gnulib's tests for -lcrypto, so that there is no static
# dependency on it:
AC_DEFUN([gl_CRYPTO_CHECK],[:])
# Avoid gnulib's tests for HAVE_WORKING_O_NOATIME and
# HAVE_WORKING_O_NOFOLLOW, as we do NOT use them:
AC_DEFUN([gl_FCNTL_O_FLAGS],[:])
# Avoid gnulib's threadlib module, as we do threads our own way:
AC_DEFUN([gl_THREADLIB],[:])dnl

# Initialize gnulib right after choosing the compiler.
dnl# Amongst other things, this sets AR and ARFLAGS.
gl_EARLY
AC_MSG_NOTICE([done initializing gnulib, back to more compiler checks])dnl

if test "x${ac_test_CFLAGS}" != "xset"; then
  # It is helpful to have C macros available to GDB, so prefer -g3 to -g
  # if -g3 works and the user does not specify CFLAGS.
  # This test must follow gl_EARLY; otherwise AC_LINK_IFELSE complains.
  case ${CFLAGS} in
    '-g')
      emacs_g3_CFLAGS='-g3';;
    '-g -O2')
      emacs_g3_CFLAGS='-g3 -O2';;
    *)
      emacs_g3_CFLAGS='';;
  esac
  if test -n "${emacs_g3_CFLAGS}"; then
    emacs_save_CFLAGS="${CFLAGS}"
    CFLAGS="${emacs_g3_CFLAGS}"
    AC_CACHE_CHECK([whether ${CC} accepts ${emacs_g3_CFLAGS}],
      [emacs_cv_prog_cc_g3],
      [AC_LINK_IFELSE([AC_LANG_PROGRAM([[]],[[]])],
	 [emacs_cv_prog_cc_g3=yes],
	 [emacs_cv_prog_cc_g3=no])])
    if test "x${emacs_cv_prog_cc_g3}" != "xyes"; then
      CFLAGS="${emacs_save_CFLAGS}"
    fi
    if test "x${opsys}" = "xmingw32"; then
      CFLAGS="${CFLAGS} -gdwarf-2"
    fi
  fi

  case ${CFLAGS} in
    *-O*) ;;
    *)
      # No optimization flag was inferred for this non-GCC compiler.
      # Try -O. This is needed for xlc on AIX; see Bug#14258.
      emacs_save_CFLAGS="${CFLAGS}"
      test -z "${CFLAGS}" || CFLAGS="${CFLAGS} "
      CFLAGS="${CFLAGS}-O"
      AC_CACHE_CHECK([whether ${CC} accepts -O],
        [emacs_cv_prog_cc_o],
	[AC_LINK_IFELSE([AC_LANG_PROGRAM([[]],[[]])],
	   [emacs_cv_prog_cc_o=yes],
	   [emacs_cv_prog_cc_o=no])])
      if test "x${emacs_cv_prog_cc_o}" != "xyes"; then
	CFLAGS="${emacs_save_CFLAGS}"
      fi ;;
  esac
else
  AC_MSG_NOTICE([CFLAGS already set; skipping attempt to add more])
fi

AC_ARG_ENABLE([gcc-warnings],
  [AS_HELP_STRING([--enable-gcc-warnings],
                  [turn on lots of GCC warnings/errors. This is meant for
		   developers, and may generate false alarms when used
		   with older or non-GNU development tools.])],
  [case ${enableval} in
     yes|no) ;;
     *) AC_MSG_ERROR([bad value ${enableval} for gcc-warnings option]) ;;
   esac
   gl_gcc_warnings="${enableval}"],
  [gl_gcc_warnings=no])dnl

AC_ARG_ENABLE([link-time-optimization],
[AS_HELP_STRING([--enable-link-time-optimization],
                [build emacs with link-time optimization.
                 This is supported only for GCC since 4.5.0.])],[
if test "x${enableval}" != "xno"; then
   AC_MSG_CHECKING([whether link-time optimization is supported])
   ac_lto_supported=no
   if test "x${GCC}" = "xyes"; then
      AC_PATH_PROG([GETCONF_PROG],[getconf])
      CPUS=`getconf _NPROCESSORS_ONLN 2>/dev/null`
      if test "x${CPUS}" != "x"; then
      	 LTO="-flto=${CPUS}"
      else
	 LTO="-flto"
      fi
      old_CFLAGS="${CFLAGS}"
      CFLAGS="${CFLAGS} ${LTO}"
      AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]],[[]])],
      	[ac_lto_supported=yes],[ac_lto_supported=no])
      CFLAGS="${old_CFLAGS}"
   fi
   AC_MSG_RESULT([${ac_lto_supported}])
   if test "x${ac_lto_supported}" = "xyes"; then
      CFLAGS="${CFLAGS} ${LTO}"
   fi
else
   AC_MSG_NOTICE([skipping checks for link-time optimization])
fi
])dnl

dnl# definition of macro for checking version of gcc has been moved to:
m4_include([m4/gcc-version.m4])dnl

# clang is unduly picky about some things.
AC_CACHE_CHECK([whether the compiler is actually clang],[emacs_cv_clang],
  [AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
	  #ifndef __clang__
	    #error "not clang"
	  #endif /* !__clang__ */
        ]])],
     [emacs_cv_clang=yes],
     [emacs_cv_clang=no])])dnl

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
dnl#TODO: move checks that currently check the value of ${host_vendor} for
dnl# apple to use the above conftest instead.

# need this to properly check clang warning flags:
PRE_WARNING_TESTING_CFLAGS="${CFLAGS}"
if test "x${emacs_cv_clang}" = "xyes" || test "x${host_vendor}" = "xapple"; then
  gl_UNKNOWN_WARNINGS_ARE_ERRORS
  CFLAGS="${CFLAGS} ${gl_unknown_warnings_are_errors}"
fi

# When compiling with GCC, prefer -isystem to -I when including system
# include files, to avoid generating useless diagnostics for the files.
if test "x${gl_gcc_warnings}" != "xyes"; then
  isystem='-I'
  if test "x${emacs_cv_clang}" = "xyes"; then
     # Turn off some warnings if supported.
     gl_WARN_ADD([-Wno-switch])
     gl_WARN_ADD([-Wno-tautological-constant-out-of-range-compare])
     gl_WARN_ADD([-Wno-pointer-sign])
  elif test "x${host_vendor}" = "xapple"; then
     gl_WARN_ADD([-Wno-pointer-sign])
  fi
else
  isystem='-isystem '

  # This, ${nw}, is the list of warnings we disable.
  nw=""

  case ${with_x_toolkit} in
    lucid | athena | motif)
       # Old toolkits mishandle 'const'.
       AC_C_CONST
       nw="${nw} -Wwrite-strings"
       ;;
    no)
       AC_MSG_NOTICE([not using -Werror without an X toolkit])
       nw="${nw} -Werror"
       ;;
    *)
       AC_MSG_WARN([X toolkit unspecified, assuming safe to add -Werror])
       gl_WARN_ADD([-Werror],[WERROR_CFLAGS])
       ;;
  esac
  AC_SUBST([WERROR_CFLAGS])dnl

  dnl# m4-quoted block number 3 begins
  [
  nw="${nw} -W"                                    # redundant with -Wextra
  nw="${nw} -Wsystem-headers"  # Do NOT let system headers trigger warnings
  nw="${nw} -Woverlength-strings"                # Not a problem these days
  nw="${nw} -Wlogical-op"                 # any use of fwrite provokes this
  nw="${nw} -Wformat-nonliteral"                         # we do this a lot
  nw="${nw} -Wvla"                                  # warnings in gettext.h
  nw="${nw} -Wnested-externs"          # use of XARGMATCH/verify_function__
  nw="${nw} -Wswitch-default"                   # Too many warnings for now
  nw="${nw} -Winline"                               # OK to ignore 'inline'
  nw="${nw} -Wjump-misses-init"       # We sometimes safely jump over init.
  nw="${nw} -Wstrict-overflow"  # OK to assume signed overflow is undefined
  nw="${nw} -Wsync-nand"       # irrelevant here, and provokes ObjC warning
  nw="${nw} -Wunsafe-loop-optimizations"  # OK to stop unsafe optimizations
  nw="${nw} -Wbad-function-cast"    # These casts are no worse than others.
  nw="${nw} -Wignored-qualifiers"                        # invalid for ObjC
  nw="${nw} -Wopenmp-simd"                               # invalid for ObjC

  # Emacs does NOT care about shadowing; see
  # <http://lists.gnu.org/archive/html/emacs-diffs/2011-11/msg00265.html>.
  nw="${nw} -Wshadow"

  # Emacs's use of alloca inhibits protecting the stack.
  nw="${nw} -Wstack-protector"

  # The following line should be removable at some point.
  nw="${nw} -Wsuggest-attribute=pure"

  # This part is merely for shortening the command line,
  # since -Wno-FOO needs to be added below regardless.
  nw="${nw} -Wmissing-field-initializers"
  nw="${nw} -Wsign-compare"
  nw="${nw} -Wswitch"
  nw="${nw} -Wtype-limits"
  nw="${nw} -Wunused-parameter"
  # (-Wformat-nonliteral is already added to nw above)
  ]
  dnl# m4-quoted block number 3 ends

  if test "x${emacs_cv_clang}" = "xyes"; then
    # Prints way too many warnings for now:
    nw="${nw} -Wcast-align"
    # Same "shorten-the-command-line" reasoning as above:
    nw="${nw} -Wformat-extra-args"
    nw="${nw} -Wtautological-constant-out-of-range-compare"
    nw="${nw} -Wunused-command-line-argument"
    nw="${nw} -Wunused-value"
  else
    if test "x${WERROR_CFLAGS}" = "x" && test "x${OBJC}" = "x"; then
      dnl# m4/manywarnings.m4 is unaware of this one:
      gl_WARN_ADD([-Wcast-qual])
    fi
  fi

  case ${host_vendor} in 
    apple*)
      ## these ones provoke warnings saying they are only valid in
      ## C++/ObjC++ when using Apple's gcc:
      if test "x${GCC}" = "xyes" && test "x${CC}" != "x${CXX}"; then
        nw="${nw} -Wabi"
        nw="${nw} -Wdeprecated"
      fi
      ## for some reason Apple's gcc prints way more prototype warnings
      ## and such than normal; let us see if this is will fix it:
      if test -n "${emacs_g3_CFLAGS}" || test "x${emacs_cv_prog_cc_g3}" = "xyes"; then
        nw="${nw} -Wmissing-prototypes"
        nw="${nw} -Wmissing-declarations"
      fi
      ;;
  esac

  gl_MANYWARN_ALL_GCC([ws])
  gl_MANYWARN_COMPLEMENT([ws],[${ws}],[${nw}])
  for w in ${ws}; do
    gl_WARN_ADD([${w}])
  done
  gl_WARN_ADD([-Wno-missing-field-initializers]) # We need this one
  gl_WARN_ADD([-Wno-sign-compare])     # Too many warnings for now
  gl_WARN_ADD([-Wno-type-limits])      # Too many warnings for now
  gl_WARN_ADD([-Wno-switch])           # Too many warnings for now
  gl_WARN_ADD([-Wno-unused-parameter]) # Too many warnings for now
  gl_WARN_ADD([-Wno-format-nonliteral])dnl

  # In spite of excluding -Wlogical-op above, it is enabled, as of
  # gcc 4.5.0 20090517.
  gl_WARN_ADD([-Wno-logical-op])dnl

  # More things that clang is unduly picky about.
  if test "x${emacs_cv_clang}" = "xyes"; then
    gl_WARN_ADD([-Wno-format-extra-args])
    gl_WARN_ADD([-Wno-tautological-constant-out-of-range-compare])
    gl_WARN_ADD([-Wno-unused-command-line-argument])
    gl_WARN_ADD([-Wno-unused-value])
  fi

  dnl# these flags are all newly added, so prevent them from screwing up
  dnl# the build:
  if test "x${WERROR_CFLAGS}" = "x"; then
    dnl# some Apple-specific warnings (for both apple-gcc and clang) that
    dnl# m4/manywarnings.m4 is unaware of:
    case ${host_vendor} in 
      apple*)
        gl_WARN_ADD([-Wextra-tokens])
        gl_WARN_ADD([-Wfour-char-constants])
        gl_WARN_ADD([-Wnewline-eof])
        dnl# These next ones are linker warnings.
        dnl# Need to make sure that this variable is defined first though,
        dnl# to prevent aclocal/autoconf/autom4te/gm4 from choking on the
        dnl# comma when it is used in arguments to a macro:
        if test "x${acl_cv_wl}" = "x"; then
          test -z "${acl_cv_wl}" && export acl_cv_wl='-Wl,'
        fi
        AC_SUBST([ac_cv_wl])
        dnl#FIXME: get them to actually be used properly:
        WARN_LDFLAGS=""
        gl_WARN_ADD([${acl_cv_wl}-warn_compact_unwind],[WARN_LDFLAGS])
        gl_WARN_ADD([${acl_cv_wl}-warn_weak_exports],[WARN_LDFLAGS])
        gl_WARN_ADD([${acl_cv_wl}-warn_stabs],[WARN_LDFLAGS])
        gl_WARN_ADD([${acl_cv_wl}-warn_commons],[WARN_LDFLAGS])
        AC_SUBST([WARN_LDFLAGS])
        ;;
    esac

    dnl# some other ones from the gcc4.2 manual that m4/manywarnings.m4
    dnl# leaves out:
    if test "x${I_AM_GOOD_AT_SILENCING_WARNINGS}" = "xyes"; then
      test -n "${I_AM_GOOD_AT_SILENCING_WARNINGS}"
      gl_WARN_ADD([-Waggregate-return])
    else
      test -z "${I_AM_GOOD_AT_SILENCING_WARNINGS}"
      nw="${nw} -Waggregate-return"
    fi
    if test "x${PRINT_CONVERSION_WARNINGS}" = "xyes"; then
      test -n "${PRINT_CONVERSION_WARNINGS}"
      gl_WARN_ADD([-Wconversion])
    else
      test -z "${PRINT_CONVERSION_WARNINGS}"
      nw="${nw} -Wconversion"
    fi
    gl_WARN_ADD([-Wdeclaration-after-statement])
    gl_WARN_ADD([-Wfloat-equal])
    gl_WARN_ADD([-Wmissing-noreturn])
    if test "x${I_AM_GOOD_AT_SILENCING_WARNINGS}" = "xyes"; then
      test -n "${I_AM_GOOD_AT_SILENCING_WARNINGS}"
      gl_WARN_ADD([-Wpadded])
      gl_WARN_ADD([-Wredundant-decls])
    else
      test -z "${I_AM_GOOD_AT_SILENCING_WARNINGS}"
      nw="${nw} -Wpadded"
      nw="${nw} -Wredundant-decls"
    fi
    dnl#FIXME: this will always end up in the first case, unless "${nw}" is
    dnl# entirely empty:
    if test -n "`echo ${nw} | grep Wswitch`"; then
      nw="${nw} -Wswitch-enum"
    else
      gl_WARN_ADD([-Wswitch-enum])
    fi
    if test "x${emacs_cv_clang}" = "xyes" || test "x${host_vendor}" = "xapple"; then
      nw="${nw} -Wunreachable-code"
    else
      gl_WARN_ADD([-Wunreachable-code])
    fi

    dnl# some other ones from the gcc4.9 manual that m4/manywarnings.m4
    dnl# leaves out:
    if test "x${CXX}" != "x" && test "x${CXX}" = "x${CC}"; then
      gl_WARN_ADD([-Wc++-compat])
    fi
    if test "x${PRINT_CONVERSION_WARNINGS}" = "xyes"; then
      dnl# leave this environment variable undocumented:
      test -n "${PRINT_CONVERSION_WARNINGS}"
      gl_WARN_ADD([-Wfloat-conversion])
      gl_WARN_ADD([-Wtraditional-conversion])
      gl_WARN_ADD([-Wsign-conversion])
    else
      test -z "${PRINT_CONVERSION_WARNINGS}"
      nw="${nw} -Wfloat-conversion"
      nw="${nw} -Wtraditional-conversion"
      nw="${nw} -Wsign-conversion"
    fi
    if test "x${I_AM_GOOD_AT_SILENCING_WARNINGS}" = "xyes"; then
      test -n "${I_AM_GOOD_AT_SILENCING_WARNINGS}"
      gl_WARN_ADD([-Wunsuffixed-float-constants])
    else
      test -z "${I_AM_GOOD_AT_SILENCING_WARNINGS}"
      nw="${nw} -Wunsuffixed-float-constants"
    fi

    dnl# to be added in a future version of gcc:
    gl_WARN_ADD([-Wbool-compare])
    gl_WARN_ADD([-Wc90-c99-compat])
    gl_WARN_ADD([-Wc99-c11-compat])
    gl_WARN_ADD([-Wlogical-not-parentheses])
    gl_WARN_ADD([-Wsizeof-array-argument])
    gl_WARN_ADD([-Wswitch-bool])
  fi

  AC_DEFINE([lint],[1],[Define to 1 if the compiler is checking for lint.])
  AH_VERBATIM([FORTIFY_SOURCE],
  [/* Enable compile-time and run-time bounds-checking, and some warnings,
    * without upsetting glibc 2.15+. */
   #if !defined _FORTIFY_SOURCE && defined __OPTIMIZE__ && __OPTIMIZE__
   # define _FORTIFY_SOURCE 2
   #endif /* !_FORTIFY_SOURCE && __OPTIMIZE__ */
  ])
  AC_DEFINE([GNULIB_PORTCHECK],[1],
            [Define to 1 to enable some gnulib portability checks])dnl

  # We use a slightly smaller set of warning options for lib/.
  # Remove the following and save the result in GNULIB_WARN_CFLAGS.
  nw=""
  nw="${nw} -Wunused-macros"

  dnl# For some reason m4/manywarnings.m4 misses this one:
  gl_WARN_ADD([-Wundef],[GNULIB_WARN_CFLAGS])dnl

  gl_MANYWARN_COMPLEMENT([GNULIB_WARN_CFLAGS],[${WARN_CFLAGS}],[${nw}])
  AC_SUBST([GNULIB_WARN_CFLAGS])dnl
  # end warning checks
fi

CFLAGS="${PRE_WARNING_TESTING_CFLAGS}"

dnl########################################################################
dnl# Some other nice autoconf tests.
AC_MSG_CHECKING([other programs])
AC_MSG_RESULT([ ])
dnl# These are ifdef-ed out, since gl_EARLY and/or Autoconf should have
dnl# already done them.
m4_ifdef([gl_00GNULIB],[:],[
AC_PROG_INSTALL
AC_PROG_MKDIR_P
if test "x${RANLIB}" = "x"; then
  test -z "${RANLIB}"
  AC_PROG_RANLIB
fi
])dnl
dnl# this is ifdef-ed the other way (i.e. "in" instead of "out"):
m4_ifdef([AM_PROG_AR],[
AM_PROG_AR
],[:])dnl

dnl# not sure where else to put these:
AC_PATH_PROG([FILE_MAGIC_BIN],[file])dnl
AC_PATH_PROG([RELPATH_BIN],[relpath])dnl

dnl#TODO: might as well just add the entirety of libtool here while we are
dnl# at it (this macro is based on it):
EMACS_REQUIRED_DARWIN_CHECKS

dnl########################################################################
dnl# Sadly, AC_PROG_LN_S is too restrictive. It also tests whether links
dnl# can be made to directories. This is not relevant for our usage, and
dnl# excludes some cases that work fine for us. E.g. MS Windows or files
dnl# hosted on AFS, both examples where simple links work, but links to
dnl# directories fail. We use a cut-down version instead.
dnl# Keep autoscan happy with a dummy condition though:
if test "x" = "Y0"; then
  test -x `which autoscan`
  AC_PROG_LN_S
  unset ac_cv_prog_ln_s
fi

AC_MSG_CHECKING([whether ln -s works for files in the same directory])
rm -f conf$$ conf$$.file

LN_S_FILEONLY='cp -p'

if (echo >conf$$.file) 2>/dev/null; then
  if ln -s conf$$.file conf$$ 2>/dev/null; then
    LN_S_FILEONLY='ln -s'
  elif ln conf$$.file conf$$ 2>/dev/null; then
    LN_S_FILEONLY=ln
  fi
fi

rm -f conf$$ conf$$.file

if test "x${LN_S_FILEONLY}" = "xln -s"; then
   AC_MSG_RESULT([yes])
else
   AC_MSG_RESULT([no, using ${LN_S_FILEONLY}])
fi

AC_SUBST([LN_S_FILEONLY])dnl

dnl########################################################################
dnl# AC_PROG_LN_S sets LN_S to 'cp -pR' for MinGW, on the premise that 'ln'
dnl# does NOT support links to directories, as in "ln file dir". But that
dnl# use is non-portable, and OTOH MinGW wants to use hard links for Emacs
dnl# executables at "make install" time. See:
dnl# http://lists.gnu.org/archive/html/emacs-devel/2013-04/msg00475.html
dnl# for more details.
if test "x${opsys}" = "xmingw32"; then
  LN_S="ln"
fi

AC_PATH_PROG([INSTALL_INFO],[install-info],[:],
  [${PATH}${PATH_SEPARATOR}/usr/sbin${PATH_SEPARATOR}/sbin])
dnl# Do NOT use GZIP, which is used by gzip for additional parameters.
AC_PATH_PROG([GZIP_PROG],[gzip])dnl

test "x${with_compress_install}" != "xyes" && test -n "${GZIP_PROG}" && \
   GZIP_PROG=" # ${GZIP_PROG} # (disabled by configure --without-compress-install)"

if test "x${opsys}" = "xgnu-linux"; then
  AC_PATH_PROG([PAXCTL],[paxctl],[],
    [${PATH}${PATH_SEPARATOR}/sbin${PATH_SEPARATOR}/usr/sbin])
  if test "X${PAXCTL}" != "X"; then
    AC_MSG_CHECKING([whether binaries have a PT_PAX_FLAGS header])
    AC_LINK_IFELSE([AC_LANG_PROGRAM([[]],[[]])],
      [if ${PAXCTL} -v conftest${EXEEXT} >/dev/null 2>&1; then
         AC_MSG_RESULT([yes])
       else
         AC_MSG_RESULT([no]);
         PAXCTL="";
       fi])
  fi

  if test "${SETFATTR+set}" != "set"; then
    AC_CACHE_CHECK([for setfattr],
      [emacs_cv_prog_setfattr],
      [touch conftest.tmp
       if (setfattr -n user.pax.flags conftest.tmp) >/dev/null 2>&1; then
	 emacs_cv_prog_setfattr=yes
       else
	 emacs_cv_prog_setfattr=no
       fi])
    if test "x${emacs_cv_prog_setfattr}" = "xyes"; then
      SETFATTR=setfattr
    else
      SETFATTR=""
    fi
    rm -f conftest.tmp
    AC_SUBST([SETFATTR])
  fi
fi

## Need makeinfo >= 4.7 (?) to build the manuals.
AC_PATH_PROG([MAKEINFO],[makeinfo],[no])
dnl# By this stage, configure has already checked for egrep and set EGREP,
dnl# or exited with an error if no egrep was found.
if test "x${MAKEINFO}" != "xno"; then
  case `
    ${MAKEINFO} --version 2> /dev/null |
    ${EGREP} 'texinfo[[^0-9]]*([[1-4]][[0-9]]+|[[5-9]]|4\.[[7-9]]|4\.[[1-6]][[0-9]]+)'
  ` in
    '') MAKEINFO=no;;
  esac
fi

## Makeinfo is unusual.  For a released Emacs, the manuals are
## pre-built, and not deleted by the normal clean rules.  makeinfo is
## therefore in the category of "special tools" not normally required,
## which configure does not have to check for (e.g. autoconf itself).
## In a repository checkout on the other hand, the manuals are not
## included. So makeinfo is a requirement to build from the repository,
## and configure should test for it as it does for any other build
## requirement. We use the presence of ${srcdir}/info/emacs to distinguish
## a release, with pre-built manuals, from a repository checkout.
HAVE_MAKEINFO=yes

if test "x${MAKEINFO}" = "xno"; then
  MAKEINFO=makeinfo
  if test "x${with_makeinfo}" = "xno"; then
    HAVE_MAKEINFO=no
  elif test ! -e "${srcdir}/info/emacs" && test ! -e "${srcdir}/info/emacs.info"; then
    AC_MSG_ERROR([You do not seem to have makeinfo >= 4.7, and your
source tree does not seem to have pre-built manuals in the `info' directory.
Either install a suitable version of makeinfo, or re-run configure
with the `--without-makeinfo' option to build without the manuals.])
  fi
fi
AC_SUBST([HAVE_MAKEINFO])dnl

dnl# Just so that there is only a single place we need to edit.
INFO_EXT=.info
INFO_OPTS=--no-split
AC_SUBST([INFO_EXT])dnl
AC_SUBST([INFO_OPTS])dnl

if test "x${opsys}" = "xmingw32"; then
   DOCMISC_W32="efaq-w32"
else
   DOCMISC_W32=""
fi
AC_SUBST([DOCMISC_W32])dnl

dnl# Add our options to ac_link now in an m4-quoted block (number 4),
dnl# after our options are set up:
[
if test "x${GCC}" = "xyes"; then
  test "x${GCC_LINK_TEST_OPTIONS}" != "x" && \
    ac_link="${ac_link} ${GCC_LINK_TEST_OPTIONS}"
else
  test "x${NON_GCC_LINK_TEST_OPTIONS}" != "x" && \
    ac_link="${ac_link} ${NON_GCC_LINK_TEST_OPTIONS}"
fi
]
dnl# fourth m4-quoted block ends

dnl# We need -znocombreloc if we are using a relatively recent GNU ld.
dnl# If we can link with the flag, it should NOT do any harm anyhow.
dnl# (Do NOT use `-z nocombreloc' as -z takes no arg on Irix.)
dnl# Treat GCC specially since it just gives a non-fatal
dnl# `unrecognized option' if not built to support GNU ld.

dnl# For a long time, -znocombreloc was added to LDFLAGS rather than
dnl# LD_SWITCH_SYSTEM_TEMACS.  That is:
dnl# * inappropriate, as LDFLAGS is a user option but this is essential.
dnl#   E.g. "make LDFLAGS=... all" could run into problems,
dnl#   http://bugs.debian.org/684788
dnl# * unnecessary, since temacs is the only thing that actually needs it.
dnl#   Indeed this is where it was originally, prior to:
dnl#   http://lists.gnu.org/archive/html/emacs-pretest-bug/2004-03/msg00170.html
late_LDFLAGS="${LDFLAGS}"
if test "x${GCC}" = "xyes"; then
  LDFLAGS_NOCOMBRELOC="-Wl,-znocombreloc"
else
  LDFLAGS_NOCOMBRELOC="-znocombreloc"
fi

LDFLAGS="${LDFLAGS} ${LDFLAGS_NOCOMBRELOC}"

AC_MSG_CHECKING([for -znocombreloc ld flag])
AC_LINK_IFELSE([AC_LANG_PROGRAM([[]],[[]])],
  [AC_MSG_RESULT([yes])],
   LDFLAGS_NOCOMBRELOC=""
  [AC_MSG_RESULT([no])])dnl

LDFLAGS="${late_LDFLAGS}"

AC_CACHE_CHECK([whether addresses are sanitized],
  [emacs_cv_sanitize_address],
  [AC_COMPILE_IFELSE(
     [AC_LANG_PROGRAM(
	[[#ifndef __has_feature
	  # define __has_feature(f) 0
	  #endif /* !__has_feature */
	  #if defined __SANITIZE_ADDRESS__ || __has_feature (address_sanitizer)
	  #else
	   #error "Addresses are not sanitized."
	  #endif /* __SANITIZE_ADDRESS__ */
	]])],
     [emacs_cv_sanitize_address=yes],
     [emacs_cv_sanitize_address=no])])dnl

dnl# The function dump-emacs will not be defined and temacs will do
dnl# (load "loadup") automatically unless told otherwise.
test "x${CANNOT_DUMP}" = "x" && export CANNOT_DUMP="no"
case "${opsys}" in
  your-opsys-here) CANNOT_DUMP=yes ;;
esac

if test "x${CANNOT_DUMP}" = "xyes"; then
  AC_DEFINE([CANNOT_DUMP],[1],
            [Define if Emacs cannot be dumped on your system.])
elif test "x${emacs_cv_sanitize_address}" = "xyes"; then
  AC_MSG_WARN([[Addresses are sanitized; suggest CANNOT_DUMP=yes]])
fi

AC_SUBST([CANNOT_DUMP])dnl

dnl# m4-quotation block 5 begins:
[
UNEXEC_OBJ=unexelf.o
case "${opsys}" in
  # MSDOS uses unexcoff.o
  aix4-2)
   UNEXEC_OBJ=unexaix.o
   ;;
  cygwin)
   UNEXEC_OBJ=unexcw.o
   ;;
  darwin)
   UNEXEC_OBJ=unexmacosx.o
   ;;
  hpux10-20 | hpux11)
   UNEXEC_OBJ=unexhp9k800.o
   ;;
  mingw32)
   UNEXEC_OBJ=unexw32.o
   ;;
  sol2-10)
   # Use the Solaris dldump() function, called from unexsol.c, to dump
   # emacs, instead of the generic ELF dump code found in unexelf.c.
   # The resulting binary has a complete symbol table, and is better
   # for debugging and other observability tools (debuggers, pstack, etc).
   #
   # If you encounter a problem using dldump(), please consider sending
   # a message to the OpenSolaris tools-linking mailing list:
   #      http://mail.opensolaris.org/mailman/listinfo/tools-linking
   #
   # It is likely that dldump() works with older Solaris too, but this has
   # not been tested, so for now this change is for Solaris 10 or newer.
   UNEXEC_OBJ=unexsol.o
   ;;
esac

LD_SWITCH_SYSTEM=""
case "${opsys}" in
  freebsd|dragonfly)
   ## Let `ld' find image libs and similar things in /usr/local/lib.
   ## The system compiler, GCC, has apparently been modified to not
   ## look there, contrary to what a stock GCC would do.
### It is not our place to do this.  See bug#10313#17.
      if test -d /usr/local/lib && test -z "${LD_SWITCH_SYSTEM}" && test -n "${BE_PRESUMPTUOUS}"; then
         LD_SWITCH_SYSTEM='-L/usr/local/lib'
      fi
   ;;

  gnu-linux)
   ## cpp test was "ifdef __mips__", but presumably this is equivalent...
   case ${host_cpu} in
     mips*)
      LD_SWITCH_SYSTEM="-G 0"
      ;;
   esac
   ;;

  netbsd)
### It is not our place to do this.  See bug#10313#17.
      if test -d /usr/pkg/lib && test -d /usr/local/lib && test -z "${LD_SWITCH_SYSTEM}" && test -n "${BE_PRESUMPTUOUS}"; then
         LD_SWITCH_SYSTEM="-Wl,-rpath,/usr/pkg/lib -L/usr/pkg/lib -Wl,-rpath,/usr/local/lib -L/usr/local/lib"
      fi
   ;;

  openbsd)
   ## Han Boetes <han@boetes.org> says this is necessary,
   ## otherwise Emacs dumps core on elf systems.
   LD_SWITCH_SYSTEM="-Z"
   ;;
esac
]
dnl# m4-quotation block 5 ends

AC_SUBST([LD_SWITCH_SYSTEM])dnl

dnl# m4-quotation block 6 begins:
[
ac_link="${ac_link} ${LD_SWITCH_SYSTEM}"

## This setting of LD_SWITCH_SYSTEM references LD_SWITCH_X_SITE_RPATH,
## which has not been defined yet. When this was handled with cpp,
## it was expanded to null when configure sourced the s/*.h file.
## Thus LD_SWITCH_SYSTEM had different values in configure and the
## Makefiles.
## FIXME: it would be cleaner to put this in LD_SWITCH_SYSTEM_TEMACS
## (or somesuch), but because it is supposed to go at the _front_
## of LD_SWITCH_SYSTEM, we cannot do that in exactly the same way.
## Compare with the gnu-linux case below, which added to the end
## of LD_SWITCH_SYSTEM, and so can instead go at the front of
## LD_SWITCH_SYSTEM_TEMACS.
case "${opsys}" in
  netbsd|openbsd)
   LD_SWITCH_SYSTEM="\$(LD_SWITCH_X_SITE_RPATH) ${LD_SWITCH_SYSTEM}" ;;
esac
]
dnl# m4-quotation block 6 ends

dnl# system-specific flags for C compilation:
C_SWITCH_MACHINE=""
case ${canonical} in
 alpha*)
  AC_CHECK_DECL([__ELF__])
  if test "x${ac_cv_have_decl___ELF__}" = "xyes"; then
    ## With ELF, make sure that all common symbols get allocated to in the
    ## data section.  Otherwise, the dump of temacs may miss variables in
    ## the shared library that have been initialized.  For example, with
    ## GNU libc, __malloc_initialized would normally be resolved to the
    ## shared library's .bss section, which is fatal.
    if test "x${GCC}" = "xyes"; then
      C_SWITCH_MACHINE="-fno-common"
    else
      AC_MSG_ERROR([Non-GCC compilers are not supported.])
    fi
  else
      dnl# This was the unexalpha.c case.  Removed in 24.1, 2010-07-24,
      dnl# albeit under the mistaken assumption that said file
      dnl# was no longer used.
      AC_MSG_ERROR([Non-ELF systems are not supported since Emacs 24.1.])
  fi
  ;;
esac
AC_SUBST([C_SWITCH_MACHINE])dnl

AC_SUBST([UNEXEC_OBJ])dnl

C_SWITCH_SYSTEM=""
## Some programs in src produce warnings saying certain subprograms
## are too complex and need a MAXMEM value greater than 2000 for
## additional optimization.  --nils@exp-math.uni-essen.de
test "x${opsys}" = "xaix4.2" && test "x${GCC}" != "xyes" && \
  export C_SWITCH_SYSTEM="-ma -qmaxmem=4000"
if test "x${opsys}" = "xmingw32"; then
  case "${canonical}" in
    x86_64-*-mingw32) C_SWITCH_SYSTEM="-mtune=generic" ;;
    *) C_SWITCH_SYSTEM="-mtune=pentium4" ;;
  esac
fi
## gnu-linux might need -D_BSD_SOURCE on old libc5 systems.
## It is redundant in glibc2, since we define _GNU_SOURCE.
AC_SUBST([C_SWITCH_SYSTEM])dnl

dnl# system-specific libraries to use (m4-quotation block 7 begins):
[
LIBS_SYSTEM=""
case "${opsys}" in
  ## IBM's X11R5 uses -lIM and -liconv in AIX 3.2.2.
  aix4-2) LIBS_SYSTEM="-lrts -lIM -liconv" ;;

  freebsd|dragonfly) LIBS_SYSTEM="-lutil" ;;

  hpux*) LIBS_SYSTEM="-l:libdld.sl" ;;

  sol2*) LIBS_SYSTEM="-lsocket -lnsl" ;;

  ## Motif needs -lgen.
  unixware) LIBS_SYSTEM="-lsocket -lnsl -lelf -lgen" ;;

  darwin)
    if test -e /usr/lib/libintl.dylib || test -e "${prefix}/lib/libintl.dylib"; then
      LIBS_SYSTEM="-lintl"
    elif test -e /usr/lib/libSystem.dylib; then
      LIBS_SYSTEM="-lSystem"
    fi
    ;;
esac
]
dnl# m4-quotation block 7 ends

AC_SUBST([LIBS_SYSTEM])dnl

dnl# m4-quotation block 8 begins:
[
### Make sure subsequent tests use flags consistent with the build flags.

if test x"${OVERRIDE_CPPFLAGS}" != x""; then
  CPPFLAGS="${OVERRIDE_CPPFLAGS}"
else
  CPPFLAGS="${C_SWITCH_SYSTEM} ${C_SWITCH_MACHINE} ${CPPFLAGS}"
fi
]
dnl# m4-quotation block 8 ends

# Suppress obsolescent Autoconf test for size_t; Emacs assumes C89 or
# better.
AC_DEFUN([AC_TYPE_SIZE_T],[:])dnl
# Likewise for obsolescent test for uid_t, gid_t; Emacs assumes them.
AC_DEFUN([AC_TYPE_UID_T],[:])dnl

dnl# other type tests that autoscan wants:
AC_MSG_CHECKING([types])
AC_MSG_RESULT([ ])
AC_TYPE_INT32_T
AC_TYPE_OFF_T
AC_TYPE_SIGNAL
AC_TYPE_SSIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_CHECK_TYPES([ptrdiff_t])dnl

AC_MSG_CHECKING([for flag to link with libm])
dnl# libm checks:
LIB_MATH='-lm'
dnl# Current possibilities handled by sed (aix4-2 -> aix,
dnl# gnu-linux -> gnu/linux, etc.):
dnl# gnu, gnu/linux, gnu/kfreebsd, aix, cygwin, darwin, hpux, irix.
dnl# And special cases: berkeley-unix, usg-unix-v, ms-dos, windows-nt.
dnl# m4-quotation block 9 begins:
[
SYSTEM_TYPE=`echo ${opsys} | sed -e 's/[[0-9]].*//' -e 's|-|/|'`

case ${opsys} in
  cygwin )
    LIB_MATH=""
    ;;
  darwin )
    ## Adding -lm confuses the dynamic linker, so omit it.
    LIB_MATH=""
    ;;
  freebsd | dragonfly )
    SYSTEM_TYPE=berkeley-unix
    ;;
  gnu-linux | gnu-kfreebsd )
    ;;
  hpux10-20 | hpux11 )
    ;;
  mingw32 )
    LIB_MATH=""
    SYSTEM_TYPE=windows-nt
    ;;
  ## NB this may be adjusted below.
  netbsd | openbsd )
    SYSTEM_TYPE=berkeley-unix
    ;;
  sol2* | unixware )
    SYSTEM_TYPE=usg-unix-v
    ;;
esac
]
dnl# m4-quotation block 9 ends

AC_SUBST([LIB_MATH])dnl
AC_DEFINE_UNQUOTED([SYSTEM_TYPE],["${SYSTEM_TYPE}"],
  [The type of system you are compiling for; sets `system-type'.])dnl

if test "x${LIB_MATH}" = "x"; then
  test -z "${LIB_MATH}"
  AC_MSG_RESULT([none needed])
else
  test ! -z "${LIB_MATH}" || test -n "${LIB_MATH}"
  AC_MSG_RESULT([${LIB_MATH}])
fi

dnl# pkg-config-checked packages:
AC_MSG_CHECKING([packages])
AC_MSG_RESULT([ ])
pre_PKG_CONFIG_CFLAGS="${CFLAGS}"
pre_PKG_CONFIG_LIBS="${LIBS}"
AC_SUBST([pre_PKG_CONFIG_CFLAGS])dnl
AC_SUBST([pre_PKG_CONFIG_LIBS])dnl

AC_PATH_PROG([PKG_CONFIG],[pkg-config],[no])dnl

dnl# The definition of the macro for checking pkgs has moved to:
m4_include([m4/pkg_extra.m4])dnl

AC_MSG_CHECKING([for sound support])
AC_MSG_RESULT([ ])
HAVE_SOUND=no
if test "x${with_sound}" != "xno"; then
  # Sound support for GNU/Linux, the free BSDs, and MinGW.
  AC_CHECK_HEADERS([machine/soundcard.h sys/soundcard.h soundcard.h],
    [have_sound_header=yes],[],[
    #ifdef __MINGW32__
    # define WIN32_LEAN_AND_MEAN
    # include <windows.h>
    #endif /* __MINGW32__ */
    ])
  test "${with_sound}" = "oss" && test "${have_sound_header}" != "yes" && \
    AC_MSG_ERROR([OSS sound support requested but not found.])dnl

  if test "x${with_sound}" = "xbsd-ossaudio" || test "x${with_sound}" = "xyes"; then
    # Emulation library used on NetBSD.
    AC_CHECK_LIB([ossaudio],[_oss_ioctl],
                 [LIBSOUND="-lossaudio"],[LIBSOUND=""])
    test "x${with_sound}" = "xbsd-ossaudio" && test -z "${LIBSOUND}" && \
      AC_MSG_ERROR([bsd-ossaudio sound support requested but not found.])
    dnl# FIXME? If we found ossaudio should we set with_sound=bsd-ossaudio?
    dnl# Traditionally we go on to check for alsa. Does that make sense?
  fi
  AC_SUBST([LIBSOUND])dnl

  if test "x${with_sound}" = "xalsa" || test "${with_sound}" = "yes"; then
    ALSA_REQUIRED=1.0.0
    ALSA_MODULES="alsa >= ${ALSA_REQUIRED}"
    PKG_CHECK_MODULES([ALSA],[${ALSA_MODULES}],
                      [HAVE_ALSA=yes],[HAVE_ALSA=no])
    if test "x${HAVE_ALSA}" = "xyes"; then
      SAVE_CFLAGS="${CFLAGS}"
      SAVE_LIBS="${LIBS}"
      CFLAGS="${ALSA_CFLAGS} ${CFLAGS}"
      LIBS="${ALSA_LIBS} ${LIBS}"
      AC_CHECK_HEADERS([asoundlib.h])
      AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <asoundlib.h>]], [[snd_lib_error_set_handler (0);]])],
                      [emacs_alsa_normal=yes],
  	            [emacs_alsa_normal=no])
      if test "x${emacs_alsa_normal}" != "xyes"; then
        AC_CHECK_HEADERS([alsa/asoundlib.h])
        AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <alsa/asoundlib.h>]],
                       [[snd_lib_error_set_handler (0);]])],
                       [emacs_alsa_subdir=yes],
  	             [emacs_alsa_subdir=no])
        if test "x${emacs_alsa_subdir}" != "xyes"; then
          AC_MSG_ERROR([pkg-config found alsa, but it does not compile.  See config.log for error messages.])
        fi
        ALSA_CFLAGS="${ALSA_CFLAGS} -DALSA_SUBDIR_INCLUDE"
      fi

      CFLAGS="${SAVE_CFLAGS}"
      LIBS="${SAVE_LIBS}"
      LIBSOUND="${LIBSOUND} ${ALSA_LIBS}"
      CFLAGS_SOUND="${CFLAGS_SOUND} ${ALSA_CFLAGS}"
      AC_DEFINE([HAVE_ALSA],[1],[Define to 1 if ALSA is available.])
    elif test "x${with_sound}" = "xalsa"; then
      AC_MSG_ERROR([ALSA sound support requested but not found.])
    fi
  fi                            dnl# with_sound = alsa|yes

  dnl# Define HAVE_SOUND if we have sound support. We know it works and
  dnl# compiles only on the specified platforms. For others, it
  dnl# probably does NOT make sense to try.
  dnl# FIXME: So surely we should bypass this whole section if not using
  dnl# one of these platforms?
  if test "x${have_sound_header}" = "xyes" || test "x${HAVE_ALSA}" = "xyes"; then
     case "${opsys}" in
       dnl# defined __FreeBSD__ || defined __NetBSD__ || defined __linux__
       dnl# Adjust the --with-sound help text if you change this.
       gnu-linux|freebsd|netbsd|mingw32)
         AC_DEFINE([HAVE_SOUND],[1],
                   [Define to 1 if you have sound support.])
         HAVE_SOUND=yes
         ;;
     esac
  fi

  AC_SUBST([CFLAGS_SOUND])dnl
  # end of sound support checks
fi

dnl# checks for header files:
AC_MSG_CHECKING([headers])
AC_MSG_RESULT([ ])
dnl# "_ONCE" causes test to be done at wrong location, unfortunately:
AC_CHECK_HEADERS([ \
  sys/systeminfo.h \
  coff.h pty.h \
  sys/resource.h \
  sys/utsname.h pwd.h utmp.h util.h])
AC_CHECK_HEADERS([OS.h arpa/inet.h fcntl.h float.h langinfo.h libintl.h \
                  limits.h locale.h malloc.h nl_types.h stddef.h \
                  sys/file.h sys/ioctl.h sys/vfs.h \
                  termio.h termios.h wctype.h])
dnl# Keep autoscan happy with a dummy condition:
if test "x" = "Y0"; then
  test -x `which autoscan`
  AC_CHECK_HEADERS([stdio_ext.h sys/param.h sys/time.h utime.h wchar.h])
fi

AC_CHECK_HEADERS([sys/personality.h])
AC_MSG_CHECKING([if personality LINUX32 can be set])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <sys/personality.h>]], [[personality(PER_LINUX32)]])],
               [emacs_cv_personality_linux32=yes],
	       [emacs_cv_personality_linux32=no])
AC_MSG_RESULT([${emacs_cv_personality_linux32}])dnl

if test "x${emacs_cv_personality_linux32}" = "xyes"; then
  AC_DEFINE([HAVE_PERSONALITY_LINUX32],[1],
            [Define to 1 if personality LINUX32 can be set.])
fi

dnl# On Solaris 8 there is a compilation warning for term.h because
dnl# it does NOT define `bool'.
AC_CHECK_HEADERS([term.h],[],[],[-])
dnl# ones with specific macros for them:
AC_HEADER_DIRENT
AC_HEADER_MAJOR
AC_HEADER_RESOLV
dnl# definition of this next macro has been moved to the included file:
m4_include([m4/headers_extra.m4])dnl
AC_REQUIRE_VARIOUS_HEADER_CHECKS
AC_HEADER_TIME
dnl# not sure why these decls go in the headers section, but whatever:
AC_CHECK_DECLS([sys_siglist],[],[],[[#include <signal.h>
			           ]])
if test "x${ac_cv_have_decl_sys_siglist}" != "xyes"; then
  # For Tru64, at least:
  AC_CHECK_DECLS([__sys_siglist],[],[],[[#include <signal.h>
			               ]])
elif test "x${ac_cv_have_decl_sys_siglist}" = "x"; then
  AC_CHECK_DECLS([sys_siglist])
fi
AC_CHECK_DECLS([MD5_DIGEST_SIZE])
dnl# well this one at least is here so we can know if we need to check
dnl# for mach headers:
AC_CHECK_DECLS([__MACH__])
if test "x${ac_cv_have_decl___MACH__}" = "xyes" || test -d /usr/include/mach-o; then
  ## this is referred to as "documentation" by unexmacosx.c:
  AC_CHECK_HEADERS([mach-o/loader.h])
fi

dnl# ...anyways:
AC_HEADER_SYS_WAIT

dnl# adding "_ONCE" causes tests to be done at wrong location, so just call
dnl# the macros regularly:
AC_CHECK_HEADERS([sys/socket.h])
AC_CHECK_HEADERS([net/if.h],[],[],[AC_INCLUDES_DEFAULT
#if HAVE_SYS_SOCKET_H
# include <sys/socket.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "this conftest expects <sys/socket.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_SYS_SOCKET_H */
])
AC_CHECK_HEADERS([ifaddrs.h],[],[],[AC_INCLUDES_DEFAULT
#if HAVE_SYS_SOCKET_H
# include <sys/socket.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "this conftest expects <sys/socket.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_SYS_SOCKET_H */
])
AC_CHECK_HEADERS([net/if_dl.h],[],[],[AC_INCLUDES_DEFAULT
#if HAVE_SYS_SOCKET_H
# include <sys/socket.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "this conftest expects <sys/socket.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_SYS_SOCKET_H */
])dnl

dnl# checks for structure members:
AC_MSG_CHECKING([structure members])
AC_MSG_RESULT([ ])
AC_CHECK_MEMBERS([struct stat.st_rdev])
AC_CHECK_MEMBERS([struct ifreq.ifr_flags, struct ifreq.ifr_hwaddr,
		  struct ifreq.ifr_netmask, struct ifreq.ifr_broadaddr,
		  struct ifreq.ifr_addr,
		  struct ifreq.ifr_addr.sa_len],[],[],
		 [AC_INCLUDES_DEFAULT
#if HAVE_SYS_SOCKET_H
# include <sys/socket.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "this conftest expects <sys/socket.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_SYS_SOCKET_H */
#if HAVE_NET_IF_H
# include <net/if.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "this conftest expects <net/if.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_NET_IF_H */
])dnl
dnl# definition of macro for checking timezone structs has been moved to:
m4_include([m4/timezone_structs.m4])dnl
AC_REQUIRE_TIMEZONE_STRUCTS

dnl# not really sure where else to put these:
AC_C_VOLATILE

dnl# Check for endianness.
dnl# AC_C_BIGENDIAN is done by gnulib, so ifdef it out:
m4_ifdef([gl_00GNULIB],[:],[AC_C_BIGENDIAN])dnl

dnl# check for Make feature:
AC_MSG_CHECKING([features of make])
AC_MSG_RESULT([ ])
dnl# AC_PROG_MAKE_SET is done by Automake, so do it conditionally:
if test "x${ac_cv_prog_make_make_set}" = "x"; then
  test -z "${ac_cv_prog_make_make_set}"
  AC_PROG_MAKE_SET
fi

DEPFLAGS=""
MKDEPDIR="echo 'skipping making depdir.'"
deps_frag=deps.mk
dnl# check for GNU Make if we have GCC and autodepend is on.
if test "x${GCC}" = "xyes" && test "x${ac_enable_autodepend}" = "xyes"; then
   AC_MSG_CHECKING([whether we are using GNU Make])
   HAVE_GNU_MAKE=no
   testval=`${MAKE-make} --version 2>/dev/null | grep 'GNU Make'`
   if test "x${testval}" != "x"; then
      HAVE_GNU_MAKE=yes
   else
      ac_enable_autodepend=no
   fi
   AC_MSG_RESULT([${HAVE_GNU_MAKE}])
   if test "x${HAVE_GNU_MAKE}" = "xyes"; then
      AC_MSG_CHECKING([whether gcc understands -MMD -MF])
      SAVE_CFLAGS="${CFLAGS}"
      CFLAGS="${CFLAGS} -MMD -MF deps.d -MP"
      AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]],[[]])],[],
                        [ac_enable_autodepend=no])
      CFLAGS="${SAVE_CFLAGS}"
      test -f deps.d || ac_enable_autodepend=no
      rm -rf deps.d
      AC_MSG_RESULT([${ac_enable_autodepend}])
   fi
   if test "x${ac_enable_autodepend}" = "xyes"; then
      DEPFLAGS='-MMD -MF ${DEPDIR}/$*.d -MP'
      ## MKDIR_P is documented (see AC_PROG_MKDIR_P) to be parallel-safe.
      MKDEPDIR='${MKDIR_P} ${DEPDIR}'
      deps_frag=autodeps.mk
   fi
else
   AC_MSG_NOTICE([skipping tests that are only relevant for autodepend])
fi
dnl# actually always check for GNU make:
AX_CHECK_GNU_MAKE([])dnl

dnl# now set and subst some stuff:
deps_frag="${srcdir}/src/${deps_frag}"
AC_SUBST([MKDEPDIR])dnl
AC_SUBST([DEPFLAGS])dnl
AC_SUBST_FILE([deps_frag])dnl

lisp_frag="${srcdir}/src/lisp.mk"
AC_SUBST_FILE([lisp_frag])dnl

dnl# checks for operating system services:
AC_SYS_LONG_FILE_NAMES

#### Choose a window system.

AC_MSG_CHECKING([windowing system])
AC_MSG_RESULT([ ])dnl

## We leave window_system equal to none if
## we end up building without one. Any new window system should
## set window_system to an appropriate value and add objects to
## window-system-specific substs.

window_system=none
# this is originally uninitialized:
export no_x=yes
AC_PATH_X
# the macro never sets this if we do actually have X, so do that for it:
if test "x${have_x}" = "xyes"; then
  export no_x=no
fi
AC_PATH_XTRA
if test "x${no_x}" != "xyes"; then
  window_system=x11
  AC_MSG_NOTICE([using x11 for the window system])
fi
AC_SUBST([no_x])dnl

dnl# m4-quotation block 10 begins:
[
LD_SWITCH_X_SITE_RPATH=""
if test "x${x_libraries}" != "xNONE"; then
  if test -n "${x_libraries}"; then
    LD_SWITCH_X_SITE=-L`echo ${x_libraries} | sed -e "s/:/ -L/g"`
    LD_SWITCH_X_SITE_RPATH=-Wl,-rpath,`echo ${x_libraries} | sed -e "s/:/ -Wl,-rpath,/g"`
  fi
  x_default_search_path=""
  x_search_path="${x_libraries}"
  if test -z "${x_search_path}"; then
    x_search_path=/usr/lib
  fi
  for x_library in `echo ${x_search_path}: | \
		    sed -e "s/:/ /g" -e p -e "s:/lib[[^ /]]* :/share :g"`; do
    x_search_path="\
${x_library}/X11/%L/%T/%N%C%S:\
${x_library}/X11/%l/%T/%N%C%S:\
${x_library}/X11/%T/%N%C%S:\
${x_library}/X11/%L/%T/%N%S:\
${x_library}/X11/%l/%T/%N%S:\
${x_library}/X11/%T/%N%S"
    if test x"${x_default_search_path}" = x""; then
      x_default_search_path="${x_search_path}"
    else
      x_default_search_path="${x_search_path}:${x_default_search_path}"
    fi
  done
fi
]
dnl# m4-quotation block 10 ends

AC_SUBST([LD_SWITCH_X_SITE_RPATH])dnl

dnl# m4-quotation block 11 begins:
[
if test "x${x_includes}" != "xNONE" && test -n "${x_includes}"; then
  C_SWITCH_X_SITE="${isystem}"`echo ${x_includes} | sed -e "s/:/ ${isystem}/g"`
fi

if test x"${x_includes}" = x""; then
  bitmapdir=/usr/include/X11/bitmaps
else
  # accumulate include directories that have X11 bitmap subdirectories:
  bmd_acc="dummyval"
  for bmd in `echo ${x_includes} | sed -e "s/:/ /g"`; do
    if test -d "${bmd}/X11/bitmaps"; then
      bmd_acc="${bmd_acc}:${bmd}/X11/bitmaps"
    fi
    if test -d "${bmd}/bitmaps"; then
      bmd_acc="${bmd_acc}:${bmd}/bitmaps"
    fi
  done
  if test "x${bmd_acc}" != "xdummyval"; then
    bitmapdir=`echo ${bmd_acc} | sed -e "s/^dummyval://"`
  fi
fi

HAVE_NS=no
NS_IMPL_COCOA=no
NS_IMPL_GNUSTEP=no
tmp_CPPFLAGS="${CPPFLAGS}"
tmp_CFLAGS="${CFLAGS}"
CPPFLAGS="${CPPFLAGS} -x objective-c"
CFLAGS="${CFLAGS} -x objective-c"
GNU_OBJC_CFLAGS=""
LIBS_GNUSTEP=""
]
dnl# m4-quotation block 11 ends
if test "x${with_ns}" != "xno"; then
  if test "x${opsys}" = "xdarwin"; then
     NS_IMPL_COCOA=yes
     ns_appdir=`pwd`/nextstep/Emacs.app
     ns_appbindir="${ns_appdir}/Contents/MacOS"
     ns_appresdir="${ns_appdir}/Contents/Resources"
     ns_appsrc=Cocoa/Emacs.base
     dnl# always use exceptions on darwin; no need to test:
     GNU_OBJC_CFLAGS="-fobjc-exceptions"
     dnl# check for blocks support:
     DISPATCH_C_BLOCKS
     GNU_OBJC_CFLAGS="${GNU_OBJC_CFLAGS} ${CBLOCKS_FLAGS}"
     if test "x${have_cblocks}" = "xfalse"; then
       AC_MSG_WARN([the Objective C sources need cblocks, but we do not have them.])
     else
       AC_DEFINE([HAVE_CBLOCKS_LANGUAGE_FEATURE],[1],
                 [Define to 1 if we can use cblocks in our code])
     fi
  elif test -f ${GNUSTEP_CONFIG_FILE}; then
     NS_IMPL_GNUSTEP=yes
     ns_appdir=`pwd`/nextstep/Emacs.app
     ns_appbindir="${ns_appdir}"
     ns_appresdir="${ns_appdir}/Resources"
     ns_appsrc=GNUstep/Emacs.base
     dnl# FIXME: sourcing this several times in subshells seems inefficient
     GNUSTEP_SYSTEM_HEADERS="$(. ${GNUSTEP_CONFIG_FILE}; echo ${GNUSTEP_SYSTEM_HEADERS})"
     GNUSTEP_SYSTEM_LIBRARIES="$(. ${GNUSTEP_CONFIG_FILE}; echo ${GNUSTEP_SYSTEM_LIBRARIES})"
     dnl# I seemed to need these as well with GNUstep-startup 0.25:
     GNUSTEP_LOCAL_HEADERS="$(. ${GNUSTEP_CONFIG_FILE}; echo ${GNUSTEP_LOCAL_HEADERS})"
     GNUSTEP_LOCAL_LIBRARIES="$(. ${GNUSTEP_CONFIG_FILE}; echo ${GNUSTEP_LOCAL_LIBRARIES})"
     test "x${GNUSTEP_LOCAL_HEADERS}" != "x" && \
       GNUSTEP_LOCAL_HEADERS="-I${GNUSTEP_LOCAL_HEADERS}"
     test "x${GNUSTEP_LOCAL_LIBRARIES}" != "x" && \
       GNUSTEP_LOCAL_LIBRARIES="-L${GNUSTEP_LOCAL_LIBRARIES}"
     CPPFLAGS="${CPPFLAGS} -I${GNUSTEP_SYSTEM_HEADERS} ${GNUSTEP_LOCAL_HEADERS}"
     CFLAGS="${CFLAGS} -I${GNUSTEP_SYSTEM_HEADERS} ${GNUSTEP_LOCAL_HEADERS}"
     LDFLAGS="${LDFLAGS} -L${GNUSTEP_SYSTEM_LIBRARIES} ${GNUSTEP_LOCAL_LIBRARIES}"
     LIBS_GNUSTEP="-lgnustep-gui -lgnustep-base -lobjc -lpthread"
     AC_CHECK_HEADERS([GNUstepBase/GSConfig.h])
     dnl# GNUstep defines BASE_NATIVE_OBJC_EXCEPTIONS to 0 or 1.
     dnl# If they had chosen to either define it or not, we could have
     dnl# just used AC_CHECK_DECL here.
     AC_CACHE_CHECK([if GNUstep defines BASE_NATIVE_OBJC_EXCEPTIONS],
       [emacs_cv_objc_exceptions],[
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <GNUstepBase/GSConfig.h>]],
[[#if defined BASE_NATIVE_OBJC_EXCEPTIONS && (BASE_NATIVE_OBJC_EXCEPTIONS > 0)
1;
#else
fail;
#endif /* BASE_NATIVE_OBJC_EXCEPTIONS > 0 */]])],
[emacs_cv_objc_exceptions=yes],[emacs_cv_objc_exceptions=no])
])
     if test "x${emacs_cv_objc_exceptions}" = "xyes"; then
       dnl# _NATIVE_OBJC_EXCEPTIONS is used by the GNUstep headers.
       AC_DEFINE([_NATIVE_OBJC_EXCEPTIONS],[1],
         [Define if GNUstep uses ObjC exceptions.])
       GNU_OBJC_CFLAGS="-fobjc-exceptions"
     fi
  fi

  if test "x${gl_gcc_warnings}" = "xyes" && test "x${WERROR_CFLAGS}" = "x"; then
    AC_LANG_PUSH([Objective C])
    gl_WARN_ADD([-Wselector],[GNU_OBJC_FLAGS])
    gl_WARN_ADD([-Wstrict-selector-match],[GNU_OBJC_FLAGS])
    gl_WARN_ADD([-Wundeclared-selector],[GNU_OBJC_FLAGS])
    AC_LANG_POP
  fi

  dnl# This is only used while we test the NS headers, it gets reset below.
  CFLAGS="${CFLAGS} ${GNU_OBJC_CFLAGS}"

  unset ac_cv_header_appkit_appkit_h
  AC_CHECK_HEADER([AppKit/AppKit.h],
                  [ac_cv_header_appkit_appkit_h=yes
                   HAVE_NS=yes],
  		  [AC_MSG_ERROR([`--with-ns' was specified, but the include
  files are missing or cannot be compiled.])])dnl

  macfont_file=""
  if test "x${NS_IMPL_COCOA}" = "xyes"; then
    dnl# "_ONCE" causes test to jump out of the conditional and be done at
    dnl# wrong location
    if test "x${ac_cv_header_appkit_appkit_h}" = "x"; then
      test -z "${ac_cv_header_appkit_appkit_h}"
      AC_CHECK_HEADERS([AppKit/AppKit.h])
    fi
    AC_MSG_CHECKING([for OSX 10.4 or newer])
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <AppKit/AppKit.h>]],
                                       [[
#ifdef MAC_OS_X_VERSION_MAX_ALLOWED
# if MAC_OS_X_VERSION_MAX_ALLOWED >= 1040
 ; /* OK */
# else
#  error "OSX 10.4 or newer required"
# endif /* 10.4+ */
#endif /* MAC_OS_X_VERSION_MAX_ALLOWED */
		    ]])],
		    [ns_osx_have_104=yes],
		    [ns_osx_have_104=no])
    AC_MSG_RESULT([${ns_osx_have_104}])dnl

    if test "x${ns_osx_have_104}" = "xno"; then
       AC_MSG_ERROR([`OSX 10.4 or newer is required']);
    fi
    AC_MSG_CHECKING([for OSX 10.5 or newer])
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <AppKit/AppKit.h>]],
                                       [[
#ifdef MAC_OS_X_VERSION_MAX_ALLOWED
# if MAC_OS_X_VERSION_MAX_ALLOWED >= 1050
 ; /* OK */
# else
#  error "OSX 10.5 not found"
# endif /* 10.5+ */
#endif /* MAC_OS_X_VERSION_MAX_ALLOWED */
		    ]])],
		    [ns_osx_have_105=yes],
		    [ns_osx_have_105=no])
    AC_MSG_RESULT([${ns_osx_have_105}])
    if test "x${ns_osx_have_105}" = "xyes"; then
      macfont_file="macfont.o"
    fi
  fi
  dnl# "_ONCE" causes test to jump out of the conditional and be done at
  dnl# wrong location:
  AC_CHECK_HEADERS([Foundation/NSObjCRuntime.h])
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <Foundation/NSObjCRuntime.h>]],
                                     [NSInteger i;])],
		    [ns_have_nsinteger=yes],
		    [ns_have_nsinteger=no])
  if test "x${ns_have_nsinteger}" = "xyes"; then
    AC_DEFINE([NS_HAVE_NSINTEGER],[1],
              [Define to 1 if `NSInteger' is defined.])
  fi
else
  if test "x${opsys}" = "xdarwin" && test "x${host_vendor}" = "xapple" && test "x${OBJC}" != "x"; then
    test -n "${OBJC}"
    AC_MSG_WARN(['--with-ns' is recommended for darwin, but it was not specified.])
  fi
fi

# if we never tested these, assume the worst:
if test -z "${HAVE_CBLOCKS_TRUE}" && test -z "${HAVE_CBLOCKS_FALSE}"; then
  HAVE_CBLOCKS_TRUE='#'
  HAVE_CBLOCKS_FALSE=''
fi
if test -z "${HAVE_CXXBLOCKS_TRUE}" && test -z "${HAVE_CXXBLOCKS_FALSE}"; then
  HAVE_CXXBLOCKS_TRUE='#'
  HAVE_CXXBLOCKS_FALSE=''
fi

AC_SUBST([LIBS_GNUSTEP])dnl

INSTALL_ARCH_INDEP_EXTRA=install-etc
ns_self_contained=no
NS_OBJ=""
NS_OBJC_OBJ=""
if test "x${HAVE_NS}" = "xyes"; then
  if test "x${with_toolkit_scroll_bars}" = "xno"; then
    AC_MSG_ERROR([Non-toolkit scroll bars are not implemented for Nextstep.])
  fi

  window_system=nextstep
  # set up packaging dirs
  if test "x${EN_NS_SELF_CONTAINED}" = "xyes"; then
     ns_self_contained=yes
     prefix="${ns_appresdir}"
     exec_prefix="${ns_appbindir}"
     dnl# This one is NOT really used, only archlibdir is.
     libexecdir="\${ns_appbindir}/libexec"
     archlibdir="\${ns_appbindir}/libexec"
     etcdocdir="\${ns_appresdir}/etc"
     etcdir="\${ns_appresdir}/etc"
     dnl# FIXME: maybe set datarootdir instead.
     dnl# That would also get applications, icons, man.
     infodir="\${ns_appresdir}/info"
     mandir="\${ns_appresdir}/man"
     lispdir="\${ns_appresdir}/lisp"
     INSTALL_ARCH_INDEP_EXTRA=""
  fi

  NS_OBJC_OBJ="nsterm.o nsfns.o nsmenu.o nsselect.o nsimage.o nsfont.o ${macfont_file}"
fi
CFLAGS="${tmp_CFLAGS}"
CPPFLAGS="${tmp_CPPFLAGS}"
AC_SUBST([INSTALL_ARCH_INDEP_EXTRA])dnl
AC_SUBST([ns_self_contained])dnl
AC_SUBST([NS_OBJ])dnl
AC_SUBST([NS_OBJC_OBJ])dnl

HAVE_W32=no
W32_OBJ=""
W32_LIBS=""
EMACSRES=""
CLIENTRES=""
CLIENTW=""
W32_RES_LINK=""
EMACS_MANIFEST=""
UPDATE_MANIFEST=""
if test "x${with_w32}" != "xno"; then
  case "${opsys}" in
    cygwin)
      unset ac_cv_header_windows_h
      AC_CHECK_HEADER([windows.h],[HAVE_W32=yes],
             [AC_MSG_ERROR([`--with-w32' was specified, but windows.h
                            cannot be found.])])
    ;;
    mingw32)
    ## Using --with-w32 with MinGW is a no-op, but we allow it.
    ;;
    *)
      AC_MSG_ERROR([Using w32 with an autotools build is only supported for Cygwin and MinGW32.])
    ;;
  esac
fi

if test "x${opsys}" = "xmingw32"; then
  AC_CHECK_HEADERS([usp10.h])
  AC_MSG_CHECKING([whether Windows API headers are recent enough])
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
     #include <windows.h>
     #include <usp10.h>]],
   [[PIMAGE_NT_HEADERS pHeader;
     PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(pHeader)]])],
   [emacs_cv_w32api=yes
    HAVE_W32=yes],
   [emacs_cv_w32api=no])
  AC_MSG_RESULT([${emacs_cv_w32api}])
  if test "x${emacs_cv_w32api}" = "xno"; then
    AC_MSG_ERROR([the Windows API headers are too old to support this build.])
  fi
fi

FIRSTFILE_OBJ=""
NTDIR=""
LIBS_ECLIENT=""
LIB_WSOCK32=""
NTLIB=""
CM_OBJ="cm.o"
XARGS_LIMIT=""
if test "x${HAVE_W32}" = "xyes"; then
  AC_DEFINE([HAVE_NTGUI],[1],[Define to use native MS Windows GUI.])
  AC_CHECK_TOOL([WINDRES],[windres],
                [AC_MSG_ERROR([No resource compiler found.])])
  W32_OBJ="w32fns.o w32menu.o w32reg.o w32font.o w32term.o"
  W32_OBJ="${W32_OBJ} w32xfns.o w32select.o w32uniscribe.o"
  EMACSRES="emacs.res"
  case "${canonical}" in
    x86_64-*-*) EMACS_MANIFEST="emacs-x64.manifest" ;;
    *) EMACS_MANIFEST="emacs-x86.manifest" ;;
  esac
  UPDATE_MANIFEST=update-game-score.exe.manifest
  if test "x${opsys}" = "xcygwin"; then
    W32_LIBS="${W32_LIBS} -lkernel32 -luser32 -lgdi32 -lole32 -lcomdlg32"
    W32_LIBS="${W32_LIBS} -lusp10 -lcomctl32 -lwinspool"
    # Tell the linker that emacs.res is an object (which we compile from
    # the rc file), not a linker script.
    W32_RES_LINK="-Wl,emacs.res"
  else
    W32_OBJ="${W32_OBJ} w32.o w32console.o w32heap.o w32inevt.o w32proc.o"
    W32_LIBS="${W32_LIBS} -lwinmm -lgdi32 -lcomdlg32"
    W32_LIBS="${W32_LIBS} -lmpr -lwinspool -lole32 -lcomctl32 -lusp10"
    W32_RES_LINK="\$(EMACSRES)"
    CLIENTRES="emacsclient.res"
    CLIENTW="emacsclientw\$(EXEEXT)"
    FIRSTFILE_OBJ=firstfile.o
    NTDIR=nt
    CM_OBJ=""
    LIBS_ECLIENT="-lcomctl32"
    LIB_WSOCK32="-lwsock32"
    NTLIB="ntlib.${ac_objext}"
    XARGS_LIMIT="-s 10000"
  fi
fi
AC_SUBST([W32_OBJ])dnl
AC_SUBST([W32_LIBS])dnl
AC_SUBST([EMACSRES])dnl
AC_SUBST([EMACS_MANIFEST])dnl
AC_SUBST([UPDATE_MANIFEST])dnl
AC_SUBST([CLIENTRES])dnl
AC_SUBST([CLIENTW])dnl
AC_SUBST([W32_RES_LINK])dnl
AC_SUBST([FIRSTFILE_OBJ])dnl
AC_SUBST([NTDIR])dnl
AC_SUBST([CM_OBJ])dnl
AC_SUBST([LIBS_ECLIENT])dnl
AC_SUBST([LIB_WSOCK32])dnl
AC_SUBST([NTLIB])dnl
AC_SUBST([XARGS_LIMIT])dnl

if test "x${HAVE_W32}" = "xyes"; then
  window_system=w32
  with_xft=no
fi

## ${window_system} is now set to the window system we will
## ultimately use.

if test "x${window_system}" = "xnone" && test "x${gl_gcc_warnings}" = "xyes"; then
   # Too many warnings for now.
   nw=""
   nw="${nw} -Wsuggest-attribute=const"
   nw="${nw} -Wsuggest-attribute=noreturn"
   gl_MANYWARN_COMPLEMENT([WARN_CFLAGS],[${WARN_CFLAGS}],[${nw}])dnl

   gl_WARN_ADD([-Wno-unused-variable])
   gl_WARN_ADD([-Wno-unused-but-set-variable])
   gl_WARN_ADD([-Wno-unused-but-set-parameter])
fi

term_header=""
HAVE_X_WINDOWS=no
HAVE_X11=no
USE_X_TOOLKIT=none

case "${window_system}" in
  x11 )
    HAVE_X_WINDOWS=yes
    HAVE_X11=yes
    term_header=xterm.h
    case "${with_x_toolkit}" in
      athena | lucid ) USE_X_TOOLKIT=LUCID ;;
      motif ) USE_X_TOOLKIT=MOTIF ;;
      gtk ) with_gtk=yes
            term_header=gtkutil.h
dnl# Do NOT set this for GTK. A lot of tests below assume Xt when
dnl# USE_X_TOOLKIT is set.
            USE_X_TOOLKIT=none ;;
      gtk2 ) with_gtk2=yes
             term_header=gtkutil.h
             USE_X_TOOLKIT=none ;;
      gtk3 ) with_gtk3=yes
             term_header=gtkutil.h
             USE_X_TOOLKIT=none ;;
      no ) USE_X_TOOLKIT=none ;;
dnl# If user did not say whether to use a toolkit, make this decision
dnl# later: use the toolkit if we have gtk, or X11R5 or newer.
      * ) USE_X_TOOLKIT=maybe ;;
    esac
  ;;
  nextstep )
    term_header=nsterm.h
  ;;
  w32 )
    term_header=w32term.h
  ;;
esac

if test "x${window_system}" = "xnone" && test "X${with_x}" != "Xno"; then
   AC_CHECK_PROGS([HAVE_XSERVER],[X X11 Xquartz Xorg],[true],[false])
   if test "x${HAVE_XSERVER}" = "xtrue" ||
      test -n "${DISPLAY}" ||
      test "`echo /usr/lib/libX11.*`" != "/usr/lib/libX11.*"; then
        AC_MSG_ERROR([You seem to be running X, but no X development
libraries were found. You should install the relevant development files
for X and for the toolkit you want, such as Gtk+ or Motif. Also make
sure you have development files for image handling, i.e.
tiff, gif, jpeg, png and xpm.
If you are sure you want Emacs compiled without X window support, pass
  --without-x
to configure.])
   fi
fi

# Does the opsystem file prohibit the use of the GNU malloc?
# Assume not, until told otherwise.
AC_MSG_CHECKING([malloc characteristics (GNU or system)])
AC_MSG_RESULT([ ])
GNU_MALLOC=yes

AC_CACHE_CHECK([whether malloc is Doug Lea style],
  [emacs_cv_var_doug_lea_malloc],
  [emacs_cv_var_doug_lea_malloc=no
   dnl# Hooks do not work with address sanitization.
   if test "x${emacs_cv_sanitize_address}" != "xyes"; then
     AC_LINK_IFELSE(
       [AC_LANG_PROGRAM(
	  [[#include <malloc.h>
	    static void hook (void) {}]],
	  [[malloc_set_state(malloc_get_state());
	    __after_morecore_hook = hook;
	    __malloc_initialize_hook = hook;]])],
       [emacs_cv_var_doug_lea_malloc=yes])])
   fi
doug_lea_malloc="${emacs_cv_var_doug_lea_malloc}"

system_malloc="${emacs_cv_sanitize_address}"
case "${opsys}" in
  ## darwin ld insists on the use of malloc routines in the System
  ## framework.
  darwin|sol2-10) system_malloc=yes ;;
esac

GMALLOC_OBJ=""
if test "${system_malloc}" = "yes"; then
  AC_DEFINE([SYSTEM_MALLOC],[1],
    [Define to 1 to use the system memory allocator, even if it is not
     Doug Lea style.])
  GNU_MALLOC=no
  GNU_MALLOC_reason="
    (The GNU allocators do NOT work with this system configuration.)"
  VMLIMIT_OBJ=""
else
  test "x${doug_lea_malloc}" != "xyes" && export GMALLOC_OBJ=gmalloc.o
  VMLIMIT_OBJ=vm-limit.o

  AC_CHECK_HEADERS([sys/vlimit.h])
  AC_CACHE_CHECK([for data_start],[emacs_cv_data_start],
    [AC_LINK_IFELSE(
       [AC_LANG_PROGRAM(
	 [[extern char data_start[]; char ch;]],
	 [[return data_start < &ch;]])],
       [emacs_cv_data_start=yes],
       [emacs_cv_data_start=no])])
  if test "x${emacs_cv_data_start}" = "xyes"; then
    AC_DEFINE([HAVE_DATA_START],[1],
      [Define to 1 if data_start is the address of the start
       of the main data segment.])
  fi
fi
AC_SUBST([GMALLOC_OBJ])dnl
AC_SUBST([VMLIMIT_OBJ])dnl

if test "x${doug_lea_malloc}" = "xyes"; then
  if test "x${GNU_MALLOC}" = "xyes"; then
    GNU_MALLOC_reason="
      (Using Doug Lea's new malloc from the GNU C Library.)"
  fi
  AC_DEFINE([DOUG_LEA_MALLOC],[1],
    [Define to 1 if the system memory allocator is Doug Lea style,
     with malloc hooks and malloc_set_state.])dnl

  ## Use mmap directly for allocating larger buffers.
  ## FIXME: this comes from src/s/{gnu,gnu-linux}.h:
  ## #ifdef DOUG_LEA_MALLOC; #undef REL_ALLOC; #endif
  ## Does the AC_FUNC_MMAP test below make this check unnecessary?
  case "${opsys}" in
    gnu*) REL_ALLOC=no ;;
  esac
fi

if test x"${REL_ALLOC}" = x""; then
  REL_ALLOC="${GNU_MALLOC}"
fi

use_mmap_for_buffers=no
case "${opsys}" in
  cygwin|freebsd|irix6-5) use_mmap_for_buffers=yes ;;
esac

AC_FUNC_MMAP
if test "x${use_mmap_for_buffers}" = "xyes"; then
  AC_DEFINE([USE_MMAP_FOR_BUFFERS],[1],
            [Define to use mmap to allocate buffer text.])
  REL_ALLOC=no
fi

AC_MSG_CHECKING([other libraries])
AC_MSG_RESULT([ ])
LIBS="${LIBS_SYSTEM} ${LIBS}"

dnl# If found, this adds -ldnet to LIBS, which Autoconf uses for checks.
AC_CHECK_LIB([dnet],[dnet_ntoa])
dnl# This causes -lresolv to get used in subsequent tests,
dnl# which causes failures on some systems such as HPUX 9.
case ${opsys} in
  hpux9*)
    AC_MSG_WARN([skipping check for libresolv on HPUX 9])
    ;;
  *)
    AC_CHECK_LIB([resolv],[gethostbyname])
    ;;
esac

dnl# FIXME: replace main with a func that we actually want from this lib:
AC_CHECK_LIB([Xbsd],[main],
             [LD_SWITCH_X_SITE="${LD_SWITCH_X_SITE} -lXbsd"])dnl

dnl# Check if pthreads is available.
AC_MSG_CHECKING([pthreads characteristics])
AC_MSG_RESULT([ ])
LIB_PTHREAD=""
dnl# "_ONCE" causes test to be done at wrong location
AC_CHECK_HEADERS([pthread.h])
if test "${ac_cv_header_pthread_h}"; then
  dnl# gmalloc.c uses pthread_atfork, which is not available on older-style
  dnl# hosts such as MirBSD10, so test for pthread_atfork instead of merely
  dnl# testing for pthread_self if Emacs uses gmalloc.c.
  if test "x${GMALLOC_OBJ}" = "xgmalloc.o"; then
    emacs_pthread_function=pthread_atfork
  else
    emacs_pthread_function=pthread_self
  fi
  AC_CHECK_LIB([pthread],[${emacs_pthread_function}],[HAVE_PTHREAD=yes])
fi
if test "x${HAVE_PTHREAD}" = "xyes"; then
  case "${canonical}" in
    *-hpux*) ;;
    *) LIB_PTHREAD="-lpthread"
       LIBS="${LIB_PTHREAD} ${LIBS}" ;;
  esac
  AC_DEFINE([HAVE_PTHREAD],[1],
            [Define to 1 if you have pthread (-lpthread).])
fi
AC_SUBST([LIB_PTHREAD])dnl

AC_CHECK_LIB([pthreads],[cma_open])dnl

## Note: when using cpp in s/aix4.2.h, this definition depended on
## HAVE_LIBPTHREADS.  That was not defined earlier in configure when
## the system file was sourced.  Hence the value of LIBS_SYSTEM
## added to LIBS in configure would never contain the pthreads part,
## but the value used in Makefiles might.  FIXME?
##
## -lpthreads seems to be necessary for Xlib in X11R6, and should
## be harmless on older versions of X where it happens to exist.
test "x${opsys}" = "xaix4-2" && \
  test "x${ac_cv_lib_pthreads_cma_open}" = "xyes" && \
  export LIBS_SYSTEM="${LIBS_SYSTEM} -lpthreads"

dnl# Check for need for bigtoc support on IBM AIX:
case ${host_os} in
aix*)
  AC_CACHE_CHECK([for -bbigtoc option],[gdb_cv_bigtoc],[
    case ${GCC} in
    yes) gdb_cv_bigtoc=-Wl,-bbigtoc ;;
    *) gdb_cv_bigtoc=-bbigtoc ;;
    esac

    LDFLAGS="${LDFLAGS}\ ${gdb_cv_bigtoc}"
    AC_LINK_IFELSE([AC_LANG_PROGRAM([[]],[[int i;]])],[],
                   [gdb_cv_bigtoc=""])
  ])
  ;;
esac

# Change CFLAGS and CPPFLAGS temporarily so that C_SWITCH_X_SITE gets
# used for the tests that follow.  We set them back to REAL_CFLAGS and
# REAL_CPPFLAGS later on.

REAL_CFLAGS="${CFLAGS}"
REAL_CPPFLAGS="${CPPFLAGS}"
AC_SUBST([REAL_CFLAGS])dnl
AC_SUBST([REAL_CPPFLAGS])dnl

AC_MSG_CHECKING([X11 features])
AC_MSG_RESULT([ ])
if test "x${HAVE_X11}" = "xyes"; then
  DEFS="${C_SWITCH_X_SITE} ${DEFS}"
  LDFLAGS="${LDFLAGS} ${LD_SWITCH_X_SITE}"
  LIBS="-lX11 ${LIBS}"
  CFLAGS="${C_SWITCH_X_SITE} ${CFLAGS}"
  CPPFLAGS="${C_SWITCH_X_SITE} ${CPPFLAGS}"

  # On Solaris, arrange for LD_RUN_PATH to point to the X libraries for
  # tests. This is handled by LD_SWITCH_X_SITE_RPATH during the real build,
  # but it is more convenient here to set LD_RUN_PATH since this
  # also works on hosts that do NOT understand LD_SWITCH_X_SITE_RPATH.
  if test "x${x_libraries}" != "xNONE" && test -n "${x_libraries}"; then
    LD_RUN_PATH="${x_libraries}${LD_RUN_PATH+:}${LD_RUN_PATH}"
    export LD_RUN_PATH
  fi

  if test "x${opsys}" = "xgnu-linux"; then
    AC_CHECK_FUNCS([XOpenDisplay])
    AC_MSG_CHECKING([whether X on GNU/Linux needs -b to link])
    AC_LINK_IFELSE([AC_LANG_PROGRAM([[]],
     [[XOpenDisplay("foo");]])],
     [xgnu_linux_first_failure=no],
     [xgnu_linux_first_failure=yes])
    if test "x${xgnu_linux_first_failure}" = "xyes"; then
      OLD_LD_SWITCH_X_SITE="${LD_SWITCH_X_SITE}"
      OLD_C_SWITCH_X_SITE="${C_SWITCH_X_SITE}"
      OLD_CPPFLAGS="${CPPFLAGS}"
      OLD_LIBS="${LIBS}"
      LD_SWITCH_X_SITE="${LD_SWITCH_X_SITE} -b i486-linuxaout"
      C_SWITCH_X_SITE="${C_SWITCH_X_SITE} -b i486-linuxaout"
      CPPFLAGS="${CPPFLAGS} -b i486-linuxaout"
      LIBS="${LIBS} -b i486-linuxaout"
      AC_LINK_IFELSE([AC_LANG_PROGRAM([[]],
       [[XOpenDisplay ("foo");]])],
       [xgnu_linux_second_failure=no],
       [xgnu_linux_second_failure=yes])
      if test "x${xgnu_linux_second_failure}" = "xyes"; then
	# If we get the same failure with -b, there is no use adding -b.
	# So take it out. This plays safe.
	LD_SWITCH_X_SITE="${OLD_LD_SWITCH_X_SITE}"
	C_SWITCH_X_SITE="${OLD_C_SWITCH_X_SITE}"
	CPPFLAGS="${OLD_CPPFLAGS}"
	LIBS="${OLD_LIBS}"
        AC_MSG_RESULT([no])
      else
        AC_MSG_RESULT([yes])
      fi
    else
      AC_MSG_RESULT([no])
    fi
  fi

  # Reportedly, some broken Solaris systems have XKBlib.h but are missing
  # header files included from there.
  AC_CHECK_HEADERS([X11/Xlib.h X11/XKBlib.h])
  AC_MSG_CHECKING([for Xkb])
  AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <X11/Xlib.h>
#include <X11/XKBlib.h>]],
	[[XkbDescPtr kb = XkbGetKeyboard(0, XkbAllComponentsMask, XkbUseCoreKbd);]])],
	[emacs_xkb=yes],[emacs_xkb=no])
  AC_MSG_RESULT([${emacs_xkb}])
  if test "x${emacs_xkb}" = "xyes"; then
    AC_DEFINE([HAVE_XKB],[1],[Define to 1 if you have the Xkb extension.])
  fi

  AC_CHECK_FUNCS([XrmSetDatabase XScreenResourceString \
                  XScreenNumberOfScreen])
fi

if test "x${window_system}" = "xx11"; then
  AC_MSG_CHECKING([X11 version 6])
  AC_CACHE_VAL([emacs_cv_x11_version_6],
  [AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <X11/Xlib.h>]],
[[#if XlibSpecificationRelease < 6
fail;
#endif /* XlibSpecificationRelease < 6 */
]])],[emacs_cv_x11_version_6=yes],[emacs_cv_x11_version_6=no])])
  if test "x${emacs_cv_x11_version_6}" = "xyes"; then
    AC_MSG_RESULT([6 or newer])
    AC_DEFINE([HAVE_X11R6],[1],
	      [Define to 1 if you have the X11R6 or newer version of Xlib])
    AC_DEFINE([HAVE_X_I18N],[1],[Define if you have usable i18n support.])
    ## inoue@ainet.or.jp says Solaris has a bug related to X11R6-style
    ## XIM support.
    case "${opsys}" in
      sol2-*) : ;;
      *) AC_DEFINE([HAVE_X11R6_XIM],[1],
           [Define if you have usable X11R6-style XIM support.])
         ;;
    esac
  else
    AC_MSG_RESULT([before 6])
  fi
fi

AC_MSG_CHECKING([graphics packages])
AC_MSG_RESULT([ ])
### Use -lrsvg-2 if available, unless `--with-rsvg=no' is specified.
HAVE_RSVG=no
if test "x${HAVE_X11}" = "xyes" || test "x${HAVE_NS}" = "xyes" || test "x${opsys}" = "xmingw32"; then
  if test "x${with_rsvg}" != "xno"; then
    RSVG_REQUIRED=2.11.0
    RSVG_MODULE="librsvg-2.0 >= ${RSVG_REQUIRED}"

    PKG_CHECK_MODULES([RSVG],[${RSVG_MODULE}],[HAVE_RSVG=yes],[:])
    AC_SUBST([RSVG_CFLAGS])dnl
    AC_SUBST([RSVG_LIBS])dnl

    if test "x${HAVE_RSVG}" = "xyes"; then
      AC_DEFINE([HAVE_RSVG],[1],[Define to 1 if using librsvg.])
      CFLAGS="${CFLAGS} ${RSVG_CFLAGS}"
      # Windows loads librsvg dynamically
      if test "x${opsys}" = "xmingw32"; then
       	RSVG_LIBS=""
      fi
      LIBS="${RSVG_LIBS} ${LIBS}"
    fi
  fi
fi

HAVE_IMAGEMAGICK=no
if test "x${HAVE_X11}" = "xyes" || test "x${HAVE_NS}" = "xyes" || test "x${HAVE_W32}" = "xyes"; then
  if test "x${with_imagemagick}" != "xno"; then
    ## 6.2.8 is the earliest version known to work, but earlier versions
    ## might work - let us know if you find one.
    ## 6.0.7 does not work.  See bug#7955.
    ## 6.8.2 makes Emacs crash; see Bug#13867.
    IMAGEMAGICK_MODULE="Wand >= 6.2.8 Wand != 6.8.2"
    PKG_CHECK_MODULES([IMAGEMAGICK],[${IMAGEMAGICK_MODULE}],
                      [HAVE_IMAGEMAGICK=yes],[:])
    ## imagemagick sometimes has '-fopenmp' in its CFLAGS, but the support
    ## in clang for openmp is incomplete, so remove the flag when using
    ## clang:
    if test "x${emacs_cv_clang}" = "xyes"; then
      IMAGEMAGICK_CFLAGS="`echo ${IMAGEMAGICK_CFLAGS} | sed 's|-fopenmp||g'`"
    fi
    AC_SUBST([IMAGEMAGICK_CFLAGS])dnl
    AC_SUBST([IMAGEMAGICK_LIBS])dnl

    if test "x${HAVE_IMAGEMAGICK}" = "xyes"; then
      AC_DEFINE([HAVE_IMAGEMAGICK],[1],[Define to 1 if using imagemagick.])
      CFLAGS="${CFLAGS} ${IMAGEMAGICK_CFLAGS}"
      LIBS="${IMAGEMAGICK_LIBS} ${LIBS}"
      AC_CHECK_FUNCS([MagickExportImagePixels MagickMergeImageLayers])
    fi
  fi
fi

dnl# section for gtk environment:
AC_MSG_CHECKING([gtk features])
AC_MSG_RESULT([ ])
HAVE_GTK=no
GTK_OBJ=""
gtk_term_header="${term_header}"
check_gtk2=no
gtk3_pkg_errors=""
if test "x${opsys}" != "xmingw32"; then
  if test "x${with_gtk3}" = "xyes" || test "x${with_gtk}" = "xyes" || test "x${USE_X_TOOLKIT}" = "xmaybe"; then
    GLIB_REQUIRED=2.28
    GTK_REQUIRED=3.0
    GTK_MODULES="gtk+-3.0 >= ${GTK_REQUIRED} glib-2.0 >= ${GLIB_REQUIRED}"

    dnl# Checks for libraries:
    PKG_CHECK_MODULES([GTK],[${GTK_MODULES}],
                      [pkg_check_gtk=yes],[pkg_check_gtk=no])
    if test "x${pkg_check_gtk}" = "xno" && test "x${with_gtk3}" = "xyes"; then
       AC_MSG_ERROR([${GTK_PKG_ERRORS}])
    fi
    if test "x${pkg_check_gtk}" = "xyes"; then
       AC_DEFINE([HAVE_GTK3],[1],[Define to 1 if using GTK 3 or later.])
       GTK_OBJ=emacsgtkfixed.o
       gtk_term_header=gtkutil.h
       USE_GTK_TOOLKIT="GTK3"
       if test "x${ac_enable_gtk_deprecation_warnings}" = "x"; then
	 AC_DEFINE([GDK_DISABLE_DEPRECATION_WARNINGS],[1],
	   [Define to 1 to disable GTK+/GDK deprecation warnings.])
	 AC_DEFINE([GLIB_DISABLE_DEPRECATION_WARNINGS],[1],
	   [Define to 1 to disable Glib deprecation warnings.])
       fi
    else
       check_gtk2=yes
       gtk3_pkg_errors="${GTK_PKG_ERRORS} "
    fi
  fi

  if test "x${with_gtk2}" = "xyes" || test "x${check_gtk2}" = "xyes"; then
    GLIB_REQUIRED=2.10
    GTK_REQUIRED=2.10
    GTK_MODULES="gtk+-2.0 >= ${GTK_REQUIRED} glib-2.0 >= ${GLIB_REQUIRED}"

    dnl# Checks for libraries.
    PKG_CHECK_MODULES([GTK],[${GTK_MODULES}],
                      [pkg_check_gtk=yes],[pkg_check_gtk=no])
    if test "x${pkg_check_gtk}" = "xno" &&
       { test "x${with_gtk}" = "xyes" || test "x${with_gtk2}" = "xyes"; }
    then
      AC_MSG_ERROR([${gtk3_pkg_errors}${GTK_PKG_ERRORS}])
    fi
    test "x${pkg_check_gtk}" = "xyes" && export USE_GTK_TOOLKIT="GTK2"
  fi
fi

if test x"${pkg_check_gtk}" = x"yes"; then
  AC_SUBST([GTK_LIBS])dnl

  C_SWITCH_X_SITE="${C_SWITCH_X_SITE} ${GTK_CFLAGS}"
  CFLAGS="${CFLAGS} ${GTK_CFLAGS}"
  CPPFLAGS="${CPPFLAGS} ${GTK_CFLAGS}"
  LIBS="${GTK_LIBS} ${LIBS}"
  dnl# Try to compile a simple GTK program.
  AC_CHECK_HEADERS([gtk/gtk.h glib-object.h])
  AC_MSG_CHECKING([whether GTK compiles])
  GTK_COMPILES=no
  AC_LINK_IFELSE(
    [AC_LANG_PROGRAM(
       [[/* Check the Gtk and Glib APIs.  */
	 #include <gtk/gtk.h>
	 #include <glib-object.h>
	 static void
	 callback(GObject *go, GParamSpec *spec, gpointer user_data)
	 {}
       ]],
       [[
	 GtkSettings *gs = 0;
	 /* Use G_CALLBACK to make sure function pointers can be cast to
	  * void *; strict C prohibits this. Use gtk_main_iteration to
	  * test that the libraries are there.  */
	 if (g_signal_handler_find(G_OBJECT(gs), G_SIGNAL_MATCH_FUNC,
				   0, 0, 0, G_CALLBACK(callback), 0))
	   gtk_main_iteration();
       ]])],
    [GTK_COMPILES=yes])
  AC_MSG_RESULT([${GTK_COMPILES}])
  if test "x${GTK_COMPILES}" != "xyes"; then
    GTK_OBJ=""
    if test "x${USE_X_TOOLKIT}" != "xmaybe"; then
      AC_MSG_ERROR([Gtk+ wanted, but it does not compile, see config.log.  Maybe some x11-devel files missing?])
    fi
  else
    HAVE_GTK=yes
    AC_DEFINE([USE_GTK],[1],[Define to 1 if using GTK.])
    GTK_OBJ="gtkutil.o ${GTK_OBJ}"
    term_header="${gtk_term_header}"
    USE_X_TOOLKIT=none
    if "${PKG_CONFIG}" --atleast-version=2.10 gtk+-2.0; then
      export MY_GTK_VERSION_IS_GOOD_ENOUGH="true"
    else
      AC_MSG_WARN([[Your version of Gtk+ will have problems with
       closing open displays. This is no problem if you just use
       one display, but if you use more than one and close one of them
       Emacs may crash.]])
      sleep 3
    fi
  fi
fi
AC_SUBST([GTK_OBJ])dnl

dnl# more GTK stuff:
if test "x${HAVE_GTK}" = "xyes"; then

  dnl# GTK scrollbars resemble toolkit scrollbars a lot, so to avoid
  dnl# a lot if #ifdef:s, say we have toolkit scrollbars.
  if test "x${with_toolkit_scroll_bars}" != "xno"; then
    with_toolkit_scroll_bars=yes
  fi

  dnl# Check if we have the old file selection dialog declared and
  dnl# in the link library. In 2.x it may be in the library,
  dnl# but not declared if deprecated featured has been selected out.
  dnl# AC_CHECK_DECL checks for a macro, so check GTK_TYPE_FILE_SELECTION.
  HAVE_GTK_FILE_SELECTION=no
  AC_CHECK_DECL([GTK_TYPE_FILE_SELECTION],[HAVE_GTK_FILE_SELECTION=yes],
                   [HAVE_GTK_FILE_SELECTION=no],[AC_INCLUDES_DEFAULT
#include <gtk/gtk.h>])
  if test "x${HAVE_GTK_FILE_SELECTION}" = "xyes"; then
    AC_CHECK_FUNCS([gtk_file_selection_new])
  fi

  dnl# Same as above for gtk_handle_box.
  HAVE_GTK_HANDLE_BOX=no
  AC_CHECK_DECL([GTK_TYPE_HANDLE_BOX],[HAVE_GTK_HANDLE_BOX=yes],
                   [HAVE_GTK_HANDLE_BOX=no],[AC_INCLUDES_DEFAULT
#include <gtk/gtk.h>])
  if test "x${HAVE_GTK_HANDLE_BOX}" = "xyes"; then
    AC_CHECK_FUNCS([gtk_handle_box_new])
  fi

  dnl# Same as above for gtk_tearoff_menu_item.
  HAVE_GTK_TEAROFF_MENU_ITEM=no
  AC_CHECK_DECL([GTK_TYPE_TEAROFF_MENU_ITEM],
                [HAVE_GTK_TEAROFF_MENU_ITEM=yes],
                [HAVE_GTK_TEAROFF_MENU_ITEM=no],[AC_INCLUDES_DEFAULT
#include <gtk/gtk.h>])
  if test "x${HAVE_GTK_TEAROFF_MENU_ITEM}" = "xyes"; then
    AC_CHECK_FUNCS([gtk_tearoff_menu_item_new])
  fi

  dnl# Check for functions introduced in 2.14 and later.
  AC_CHECK_FUNCS([gtk_widget_get_window gtk_widget_set_has_window \
                  gtk_dialog_get_action_area gtk_widget_get_sensitive \
                  gtk_widget_get_mapped gtk_adjustment_get_page_size \
                  gtk_orientable_set_orientation \
		  gtk_window_set_has_resize_grip])dnl

  term_header=gtkutil.h
fi

AC_MSG_CHECKING([settings/configuration packages])
AC_MSG_RESULT([ ])
dnl# D-Bus has been tested under GNU/Linux only. Must be adapted for
dnl# other platforms.
HAVE_DBUS=no
DBUS_OBJ=""
if test "x${with_dbus}" = "xyes"; then
   PKG_CHECK_MODULES([DBUS],[dbus-1 >= 1.0],[HAVE_DBUS=yes],[HAVE_DBUS=no])
   if test "x${HAVE_DBUS}" = "xyes"; then
     LIBS="${LIBS} ${DBUS_LIBS}"
     AC_DEFINE([HAVE_DBUS],[1],[Define to 1 if using D-Bus.])
     dnl# dbus_watch_get_unix_fd has been introduced in D-Bus 1.1.1.
     dnl# dbus_type_is_valid and dbus_validate_* have been introduced in
     dnl# D-Bus 1.5.12.
     AC_CHECK_FUNCS([dbus_watch_get_unix_fd \
		     dbus_type_is_valid \
		     dbus_validate_bus_name \
                     dbus_validate_path \
		     dbus_validate_interface \
		     dbus_validate_member])
     DBUS_OBJ=dbusbind.o
   fi
fi
AC_SUBST([DBUS_OBJ])dnl

dnl# GSettings has been tested under GNU/Linux only.
HAVE_GSETTINGS=no
if test "x${HAVE_X11}" = "xyes" && test "x${with_gsettings}" = "xyes"; then
   PKG_CHECK_MODULES([GSETTINGS],[gio-2.0 >= 2.26],
                     [HAVE_GSETTINGS=yes],[HAVE_GSETTINGS=no])
   if test "x${HAVE_GSETTINGS}" = "xyes"; then
      AC_DEFINE([HAVE_GSETTINGS],[1],[Define to 1 if using GSettings.])
      SETTINGS_CFLAGS="${GSETTINGS_CFLAGS}"
      SETTINGS_LIBS="${GSETTINGS_LIBS}"
   fi
fi

dnl# GConf has been tested under GNU/Linux only.
dnl# The version is really arbitrary, it is about the same age as Gtk+ 2.6.
HAVE_GCONF=no
if test "x${HAVE_X11}" = "xyes" && test "x${with_gconf}" = "xyes"; then
   PKG_CHECK_MODULES([GCONF],[gconf-2.0 >= 2.13],
                     [HAVE_GCONF=yes],[HAVE_GCONF=no])
   if test "x${HAVE_GCONF}" = "xyes"; then
      AC_DEFINE([HAVE_GCONF],[1],[Define to 1 if using GConf.])
      dnl# Newer GConf does NOT link with g_objects, so this is not
      dnl# defined.
      SETTINGS_CFLAGS="${SETTINGS_CFLAGS} ${GCONF_CFLAGS}"
      SETTINGS_LIBS="${SETTINGS_LIBS} ${GCONF_LIBS}"
   fi
fi

if test "x${HAVE_GSETTINGS}" = "xyes" || test "x${HAVE_GCONF}" = "xyes"; then
    PKG_CHECK_MODULES([GOBJECT],[gobject-2.0 >= 2.0],
                      [HAVE_GOBJECT=yes],[HAVE_GOBJECT=no])
    if test "x${HAVE_GOBJECT}" = "xyes"; then
       SETTINGS_CFLAGS="${SETTINGS_CFLAGS} ${GOBJECT_CFLAGS}"
       SETTINGS_LIBS="${SETTINGS_LIBS} ${GOBJECT_LIBS}"
    fi
    SAVE_CFLAGS="${CFLAGS}"
    SAVE_LIBS="${LIBS}"
    CFLAGS="${SETTINGS_CFLAGS} ${CFLAGS}"
    LIBS="${SETTINGS_LIBS} ${LIBS}"
    CFLAGS="${SAVE_CFLAGS}"
    LIBS="${SAVE_LIBS}"
fi
AC_SUBST([SETTINGS_CFLAGS])dnl
AC_SUBST([SETTINGS_LIBS])dnl

dnl# secure ourselves:
AC_MSG_CHECKING([security packages])
AC_MSG_RESULT([ ])
dnl# SELinux is available for GNU/Linux only.
HAVE_LIBSELINUX=no
LIBSELINUX_LIBS=""
if test "x${with_selinux}" = "xyes"; then
   AC_CHECK_LIB([selinux],[lgetfilecon],
                [HAVE_LIBSELINUX=yes],[HAVE_LIBSELINUX=no])
   if test "x${HAVE_LIBSELINUX}" = "xyes"; then
      AC_DEFINE([HAVE_LIBSELINUX],[1],[Define to 1 if using SELinux.])
      LIBSELINUX_LIBS='-lselinux'
   fi
fi
AC_SUBST([LIBSELINUX_LIBS])dnl

HAVE_GNUTLS=no
HAVE_GNUTLS3=no
if test "x${with_gnutls}" = "xyes"; then
  PKG_CHECK_MODULES([LIBGNUTLS],[gnutls >= 3.0.0],
                    [HAVE_GNUTLS3=yes],[HAVE_GNUTLS3=no])
  if test "x${HAVE_GNUTLS3}" = "xyes"; then
    AC_DEFINE([HAVE_GNUTLS3],[1],[Define if using GnuTLS v3.])
    HAVE_GNUTLS="yes"
  else
    PKG_CHECK_MODULES([LIBGNUTLS],[gnutls >= 2.6.6],
                      [HAVE_GNUTLS=yes],[HAVE_GNUTLS=no])
  fi
  if test "x${HAVE_GNUTLS}" = "xyes"; then
    AC_DEFINE([HAVE_GNUTLS],[1],[Define if using GnuTLS.])
  fi

  # Windows loads GnuTLS dynamically:
  if test "x${opsys}" = "xmingw32"; then
    LIBGNUTLS_LIBS=""
  else
    CFLAGS="${CFLAGS} ${LIBGNUTLS_CFLAGS}"
    LIBS="${LIBGNUTLS_LIBS} ${LIBS}"
  fi
fi

AC_SUBST([LIBGNUTLS_LIBS])dnl
AC_SUBST([LIBGNUTLS_CFLAGS])dnl

AC_MSG_CHECKING([notification packages])
AC_MSG_RESULT([ ])
NOTIFY_OBJ=""
NOTIFY_SUMMARY=no

dnl# FIXME?  Do NOT auto-detect on NS, but do allow someone to specify
dnl# a particular library.  This does NOT make much sense?
if test "x${with_ns}" = "xyes" && test "x${with_file_notification}" = "xyes"; then
  with_file_notification=no
fi

dnl# MS Windows native file monitor is available for mingw32 only.
case ${with_file_notification},${opsys} in
  w32,* | yes,mingw32)
    AC_CHECK_HEADER([windows.h])
    if test "x${ac_cv_header_windows_h}" = "xyes"; then
       AC_DEFINE([HAVE_W32NOTIFY],[1],[Define to 1 to use w32notify.])
       NOTIFY_OBJ=w32notify.o
       NOTIFY_SUMMARY="yes (w32)"
    fi ;;
esac

dnl# g_file_monitor exists since glib 2.18.  G_FILE_MONITOR_EVENT_MOVED
dnl# has been added in glib 2.24.  It has been tested under
dnl# GNU/Linux only.  We take precedence over inotify, but this makes
dnl# only sense when glib has been compiled with inotify support.  How
dnl# to check?
case ${with_file_notification},${NOTIFY_OBJ} in
  gfile, | yes,)
    PKG_CHECK_MODULES([GFILENOTIFY],[gio-2.0 >= 2.24],
                      [HAVE_GFILENOTIFY=yes],[HAVE_GFILENOTIFY=no])
    if test "x${HAVE_GFILENOTIFY}" = "xyes"; then
       AC_DEFINE([HAVE_GFILENOTIFY],[1],[Define to 1 if using GFile.])
       NOTIFY_OBJ=gfilenotify.o
       NOTIFY_SUMMARY="yes -lgio (gfile)"
    fi ;;
esac

dnl# inotify is only available on GNU/Linux.
case ${with_file_notification},${NOTIFY_OBJ} in
  inotify, | yes,)
    AC_CHECK_HEADER([sys/inotify.h])
    if test "x${ac_cv_header_sys_inotify_h}" = "xyes"; then
	AC_CHECK_FUNC([inotify_init1])
	if test "x${ac_cv_func_inotify_init1}" = "xyes"; then
	  AC_DEFINE([HAVE_INOTIFY],[1],[Define to 1 to use inotify.])
	  NOTIFY_OBJ=inotify.o
	  NOTIFY_SUMMARY="yes -lglibc (inotify)"
       fi
    fi ;;
esac

case ${with_file_notification},${NOTIFY_OBJ} in
  yes,* | no,* | *,?*) ;;
  *) AC_MSG_ERROR([File notification `$with_file_notification' requested but requirements not found.]) ;;
esac

if test -n "${NOTIFY_OBJ}"; then
   AC_DEFINE([USE_FILE_NOTIFY],[1],
             [Define to 1 if using file notifications.])
fi
AC_SUBST([NOTIFY_OBJ])dnl
AC_SUBST([GFILENOTIFY_CFLAGS])dnl
AC_SUBST([GFILENOTIFY_LIBS])dnl

dnl# Do not put whitespace before the #include statements below.
dnl# Older compilers (e.g. sunos4 cc) choke on it.
AC_MSG_CHECKING([X toolkits])
AC_MSG_RESULT([ ])
HAVE_XAW3D=no
LUCID_LIBW=""
if test x"${USE_X_TOOLKIT}" = x"maybe" || test x"${USE_X_TOOLKIT}" = x"LUCID"; then
  if test "x${with_xaw3d}" != "xno"; then
    AC_CHECK_HEADERS([X11/Xaw3d/Simple.h])
    AC_CACHE_VAL([emacs_cv_xaw3d],
    [AC_LINK_IFELSE([AC_LANG_PROGRAM([[
#include <X11/Intrinsic.h>
#include <X11/Xaw3d/Simple.h>]],
      [[]])],
      [AC_CHECK_LIB([Xaw3d],[XawScrollbarSetThumb],
                    [emacs_cv_xaw3d=yes],[emacs_cv_xaw3d=no])],
      [emacs_cv_xaw3d=no])])
  else
    emacs_cv_xaw3d=no
  fi
  if test "x${emacs_cv_xaw3d}" = "xyes"; then
    AC_MSG_CHECKING([for xaw3d])
    AC_MSG_RESULT([yes; using Lucid toolkit])
    USE_X_TOOLKIT=LUCID
    HAVE_XAW3D=yes
    LUCID_LIBW='-lXaw3d'
    AC_DEFINE([HAVE_XAW3D],[1],
              [Define to 1 if you have the Xaw3d library (-lXaw3d).])
  else
    AC_MSG_CHECKING([for xaw3d])
    AC_MSG_RESULT([no])
    AC_CHECK_HEADERS([X11/Xaw/Simple.h])
    AC_MSG_CHECKING([for libXaw])
    AC_CACHE_VAL([emacs_cv_xaw],
    [AC_LINK_IFELSE([AC_LANG_PROGRAM([[
#include <X11/Intrinsic.h>
#include <X11/Xaw/Simple.h>]],
      [[]])],
      [emacs_cv_xaw=yes],
      [emacs_cv_xaw=no])])
    if test "x${emacs_cv_xaw}" = "xyes"; then
      AC_MSG_RESULT([yes; using Lucid toolkit])
      USE_X_TOOLKIT=LUCID
      LUCID_LIBW='-lXaw'
    elif test x"${USE_X_TOOLKIT}" = x"LUCID"; then
      AC_MSG_ERROR([Lucid toolkit requires X11/Xaw include files])
    else
      AC_MSG_ERROR([No X toolkit could be found.
If you are sure you want Emacs compiled without an X toolkit, pass
  --with-x-toolkit=no
to configure.  Otherwise, install the development libraries for the toolkit
that you want to use (e.g. Gtk+) and re-run configure.])
    fi
  fi
fi

X_TOOLKIT_TYPE="${USE_X_TOOLKIT}"

LIBXTR6=""
if test "x${USE_X_TOOLKIT}" != "xnone"; then
  dnl# "_ONCE" causes test to jump out of the conditional and be done at
  dnl# wrong location (check ac_cv_header_X11_Intrinsic_h)
  AC_CHECK_HEADERS([X11/Intrinsic.h])
  AC_MSG_CHECKING([X11 toolkit version])
  AC_CACHE_VAL([emacs_cv_x11_toolkit_version_6],
  [AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <X11/Intrinsic.h>]],
[[#if XtSpecificationRelease < 6
fail;
#endif /* XtSpecificationRelease < 6 */
]])],[emacs_cv_x11_toolkit_version_6=yes],
     [emacs_cv_x11_toolkit_version_6=no])])
  HAVE_X11XTR6="${emacs_cv_x11_toolkit_version_6}"
  if test "x${emacs_cv_x11_toolkit_version_6}" = "xyes"; then
    AC_MSG_RESULT([6 or newer])
    AC_DEFINE([HAVE_X11XTR6],[1],
	      [Define to 1 if you have the X11R6 or newer version of Xt.])
    LIBXTR6="-lSM -lICE"
    case "${opsys}" in
      ## Use libw.a along with X11R6 Xt.
      unixware) LIBXTR6="${LIBXTR6} -lw" ;;
    esac
  else
    AC_MSG_RESULT([before 6])
  fi

dnl# If using toolkit, check whether libXmu.a exists.
dnl# tranle@intellicorp.com says libXmu.a can need XtMalloc in libXt.a to
dnl# link.
  OLDLIBS="${LIBS}"
  if test "x${HAVE_X11XTR6}" = "xyes"; then
    LIBS="-lXt -lSM -lICE ${LIBS}"
  else
    LIBS="-lXt ${LIBS}"
  fi
  AC_CHECK_LIB([Xmu],[XmuConvertStandardSelection])
  test "x${ac_cv_lib_Xmu_XmuConvertStandardSelection}" = "xno" && export LIBS="${OLDLIBS}"
  dnl# ac_cv_lib_Xmu_XmuConvertStandardSelection is also referenced below.
fi
AC_SUBST([LIBXTR6])dnl

dnl# FIXME: the logic here seems weird, but this is what cpp was doing.
dnl# Why not just test for libxmu in the normal way?
LIBXMU='-lXmu'
case ${opsys} in
  ## These systems do NOT supply Xmu.
  hpux* | aix4-2 )
    test "X${ac_cv_lib_Xmu_XmuConvertStandardSelection}" != "Xyes" && export LIBXMU=""
    ;;
  mingw32 )
    LIBXMU=""
    ;;
esac
AC_SUBST([LIBXMU])dnl

# On Irix 6.5, at least, we need XShapeQueryExtension from -lXext for
# Xaw3D.
if test "x${HAVE_X11}" = "xyes"; then
  if test "x${USE_X_TOOLKIT}" != "xnone"; then
    AC_CHECK_LIB([Xext],[XShapeQueryExtension])
  fi
fi

LIBXP=""
if test "x${USE_X_TOOLKIT}" = "xMOTIF"; then
  # OpenMotif may be installed in such a way on some GNU/Linux systems.
  if test -d /usr/include/openmotif; then
    CPPFLAGS="-I/usr/include/openmotif ${CPPFLAGS}"
    emacs_cv_openmotif=yes
    case "${canonical}" in
      x86_64-*-linux-gnu* | powerpc64-*-linux-gnu* | sparc64-*-linux-gnu*)
      test -d /usr/lib64/openmotif && export LDFLAGS="-L/usr/lib64/openmotif ${LDFLAGS}"
      ;;
      *)
      test -d /usr/lib/openmotif && export LDFLAGS="-L/usr/lib/openmotif ${LDFLAGS}"
    esac
  else
    emacs_cv_openmotif=no
  fi
  AC_CHECK_HEADERS([Xm/Xm.h])
  AC_CACHE_CHECK([for (Open)Motif version 2.1],
                 [emacs_cv_motif_version_2_1],
  [AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <Xm/Xm.h>]],
    [[#if (XmVERSION > 2) || ((XmVERSION == 2) && (XmREVISION >= 1))
int x = 5;
#else
Motif version prior to 2.1.
#endif /* Motif version check */
]])],
    [emacs_cv_motif_version_2_1=yes],[emacs_cv_motif_version_2_1=no])])
  if test "x${emacs_cv_motif_version_2_1}" = "xyes"; then
    AC_CHECK_LIB([Xp],[XpCreateContext],[LIBXP='-lXp'])
    if test "x${emacs_cv_openmotif}" = "xyes"; then
      REAL_CPPFLAGS="-I/usr/include/openmotif ${REAL_CPPFLAGS}"
    fi
  else
    AC_CACHE_CHECK([for LessTif where some systems put it],
                   [emacs_cv_lesstif],
    # We put this in CFLAGS temporarily to precede other -I options
    # that might be in CFLAGS temporarily.
    # We put this in CPPFLAGS where it precedes the other -I options.
    OLD_CPPFLAGS="${CPPFLAGS}"
    OLD_CFLAGS="${CFLAGS}"
    CPPFLAGS="-I/usr/X11R6/LessTif/Motif1.2/include ${CPPFLAGS}"
    CFLAGS="-I/usr/X11R6/LessTif/Motif1.2/include ${CFLAGS}"
    [AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include </usr/X11R6/LessTif/Motif1.2/include/Xm/Xm.h>]],
      [[int x = 5;]])],
      [emacs_cv_lesstif=yes],[emacs_cv_lesstif=no])])
    if test "x${emacs_cv_lesstif}" = "xyes"; then
      # Make sure this -I option remains in CPPFLAGS after it is set
      # back to REAL_CPPFLAGS.
      # There is no need to change REAL_CFLAGS, because REAL_CFLAGS does
      # not have those other -I options anyway. Ultimately, having this
      # directory ultimately in CPPFLAGS will be enough.
      REAL_CPPFLAGS="-I/usr/X11R6/LessTif/Motif1.2/include ${REAL_CPPFLAGS}"
      LDFLAGS="-L/usr/X11R6/LessTif/Motif1.2/lib ${LDFLAGS}"
    else
      CFLAGS="${OLD_CFLAGS}"
      CPPFLAGS="${OLD_CPPFLAGS}"
    fi
  fi
  AC_CHECK_HEADER([Xm/BulletinB.h],[],
    [AC_MSG_ERROR([Motif toolkit requested, but requirements not found.])])
fi

dnl# Use toolkit scroll bars if configured for GTK or X toolkit and either
dnl# using Motif or Xaw3d is available, and unless
dnl# --with-toolkit-scroll-bars=no was specified.

AH_TEMPLATE([USE_TOOLKIT_SCROLL_BARS],
	    [Define to 1 if we should use toolkit scroll bars.])dnl
USE_TOOLKIT_SCROLL_BARS=no
if test "x${with_toolkit_scroll_bars}" != "xno"; then
  if test "x${USE_X_TOOLKIT}" != "xnone"; then
    if test "x${USE_X_TOOLKIT}" = "xMOTIF"; then
      AC_DEFINE([USE_TOOLKIT_SCROLL_BARS])
      HAVE_XAW3D=no
      USE_TOOLKIT_SCROLL_BARS=yes
    elif test "x${HAVE_XAW3D}" = "xyes" || test "x${USE_X_TOOLKIT}" = "xLUCID"; then
      AC_DEFINE([USE_TOOLKIT_SCROLL_BARS])
      USE_TOOLKIT_SCROLL_BARS=yes
    fi
  elif test "x${HAVE_GTK}" = "xyes"; then
    AC_DEFINE([USE_TOOLKIT_SCROLL_BARS])
    USE_TOOLKIT_SCROLL_BARS=yes
  elif test "x${HAVE_NS}" = "xyes"; then
    AC_DEFINE([USE_TOOLKIT_SCROLL_BARS])
    USE_TOOLKIT_SCROLL_BARS=yes
  elif test "x${HAVE_W32}" = "xyes"; then
    AC_DEFINE([USE_TOOLKIT_SCROLL_BARS])
    USE_TOOLKIT_SCROLL_BARS=yes
  fi
fi

dnl# See if XIM is available.
AC_CHECK_HEADERS([X11/Xresource.h])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
	  #include <X11/Xlib.h>
	  #include <X11/Xresource.h>]],
	 [[XIMProc  callback;]])],
	 [HAVE_XIM=yes
	  AC_DEFINE([HAVE_XIM],[1],[Define to 1 if XIM is available])],
	 [HAVE_XIM=no])dnl

dnl# `--with-xim' now controls only the initial value of use_xim at
dnl# runtime.

if test "x${with_xim}" != "xno"; then
  AC_DEFINE([USE_XIM],[1],
	    [Define to 1 if we should use XIM, if it is available.])
fi

if test "x${HAVE_XIM}" != "xno"; then
  late_CFLAGS="${CFLAGS}"
  if test "x${GCC}" = "xyes"; then
    # I thought the flag had just one hyphen in it?
    CFLAGS="${CFLAGS} --pedantic-errors"
  fi
  AC_CHECK_FUNCS([XRegisterIMInstantiateCallback])
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <X11/Xlib.h>
#include <X11/Xresource.h>]],
[[Display *display;
XrmDatabase db;
char *res_name;
char *res_class;
XIMProc  callback;
XPointer *client_data;
#ifndef __GNUC__
/* If we are not using GCC, it is probably not XFree86, and this is
 * probably right, but we cannot use something like --pedantic-errors.  */
extern Bool XRegisterIMInstantiateCallback(Display*, XrmDatabase, char*,
                                           char*, XIMProc, XPointer*);
#endif /* !__GNUC__ */
(void)XRegisterIMInstantiateCallback(display, db, res_name, res_class, callback,
   client_data);]])],
    [emacs_cv_arg6_star=yes])
  AH_TEMPLATE([XRegisterIMInstantiateCallback_arg6],
         [Define to the type of the 6th arg of XRegisterIMInstantiateCallback,
either XPointer or XPointer*.])dnl
  if test "x${emacs_cv_arg6_star}" = "xyes"; then
    AC_DEFINE([XRegisterIMInstantiateCallback_arg6],[XPointer*])
  else
    AC_DEFINE([XRegisterIMInstantiateCallback_arg6],[XPointer])
  fi
  CFLAGS="${late_CFLAGS}"
fi

AC_MSG_CHECKING([font-rendering-backend packages])
AC_MSG_RESULT([ ])
### Start of font-backend (under any platform) section.
# (nothing here yet -- this is a placeholder)
### End of font-backend (under any platform) section.

### Start of font-backend (under X11) section.
if test "x${HAVE_X11}" = "xyes"; then
   PKG_CHECK_MODULES([FONTCONFIG],[fontconfig >= 2.2.0],
                     [HAVE_FC=yes],[HAVE_FC=no])dnl

   ## Use -lXft if available, unless `--with-xft=no'.
   HAVE_XFT=maybe
    if test "x${HAVE_FC}" = "xno" || test "x${with_x}" = "xno"; then
      with_xft="no";
    fi
    if test "x${with_xft}" != "xno"; then

      PKG_CHECK_MODULES([XFT],[xft >= 0.13.0],[],[HAVE_XFT=no])
      ## Because xftfont.c uses XRenderQueryExtension, we also
      ## need to link to -lXrender.
      HAVE_XRENDER=no
      AC_CHECK_LIB([Xrender],[XRenderQueryExtension],[HAVE_XRENDER=yes])
      if test "x${HAVE_XFT}" != "xno" && test "x${HAVE_XRENDER}" != "xno"; then
	OLD_CPPFLAGS="${CPPFLAGS}"
	OLD_CFLAGS="${CFLAGS}"
	OLD_LIBS="${LIBS}"
	CPPFLAGS="${CPPFLAGS} ${XFT_CFLAGS}"
	CFLAGS="${CFLAGS} ${XFT_CFLAGS}"
	XFT_LIBS="-lXrender ${XFT_LIBS}"
	LIBS="${XFT_LIBS} ${LIBS}"
        AC_CHECK_HEADER([X11/X.h])
	AC_CHECK_HEADER([X11/Xft/Xft.h],
	  [AC_CHECK_LIB([Xft],[XftFontOpen],[HAVE_XFT=yes],[],
                        [${XFT_LIBS}])],[],
          [[#include <X11/X.h>]])dnl

	if test "x${HAVE_XFT}" = "xyes"; then
	  AC_DEFINE([HAVE_XFT],[1],
                    [Define to 1 if you have the Xft library.])
	    AC_SUBST([XFT_LIBS])
	  C_SWITCH_X_SITE="${C_SWITCH_X_SITE} ${XFT_CFLAGS}"
	else
	  CPPFLAGS="${OLD_CPPFLAGS}"
	  CFLAGS="${OLD_CFLAGS}"
	  LIBS="${OLD_LIBS}"
	fi                        # "x${HAVE_XFT}" = "xyes"
      fi                          # "x${HAVE_XFT}" != "xno"
    fi                            # "x${with_xft}" != "xno"

    ## We used to allow building with FreeType and without Xft.
    ## However, the ftx font backend driver is not in good shape.
    if test "x${HAVE_XFT}" != "xyes"; then
       dnl# For the "Does Emacs use" message at the end.
       HAVE_XFT=no
       HAVE_FREETYPE=no
    else
       dnl# Strict linkers fail with
       dnl# ftfont.o: undefined reference to symbol 'FT_New_Face'
       dnl# if -lfreetype is not specified.
       dnl# The following is needed to set FREETYPE_LIBS.
       PKG_CHECK_MODULES([FREETYPE],[freetype2],[HAVE_FREETYPE=yes],
			 [HAVE_FREETYPE=no])dnl

       test "x${HAVE_FREETYPE}" = "xno" && AC_MSG_ERROR([libxft requires libfreetype])
    fi

    HAVE_LIBOTF=no
    if test "x${HAVE_FREETYPE}" = "xyes"; then
      AC_DEFINE([HAVE_FREETYPE],[1],
		[Define to 1 if using the freetype/fontconfig libraries.])
      if test "x${with_libotf}" != "xno"; then
	PKG_CHECK_MODULES([LIBOTF],[libotf],[HAVE_LIBOTF=yes],
			  [HAVE_LIBOTF=no])
	if test "x${HAVE_LIBOTF}" = "xyes"; then
	  AC_DEFINE([HAVE_LIBOTF],[1],[Define to 1 if using libotf.])
	  AC_CHECK_LIB([otf],[OTF_get_variation_glyphs],
		       [HAVE_OTF_GET_VARIATION_GLYPHS=yes],
		       [HAVE_OTF_GET_VARIATION_GLYPHS=no])
	  if test "x${HAVE_OTF_GET_VARIATION_GLYPHS}" = "xyes"; then
	    AC_DEFINE([HAVE_OTF_GET_VARIATION_GLYPHS],[1],
		      [Define to 1 if libotf has OTF_get_variation_glyphs.])
	  fi
	fi
      fi
    dnl# FIXME: should there be an error if HAVE_FREETYPE != yes?
    dnl# Does the new font backend require it, or can it work without it?
    fi

    HAVE_M17N_FLT=no
    if test "x${HAVE_LIBOTF}" = "xyes"; then
      if test "x${with_m17n_flt}" != "xno"; then
	PKG_CHECK_MODULES([M17N_FLT],[m17n-flt],
                          [HAVE_M17N_FLT=yes],[HAVE_M17N_FLT=no])
	if test "x${HAVE_M17N_FLT}" = "xyes"; then
	  AC_DEFINE([HAVE_M17N_FLT],[1],
                    [Define to 1 if using libm17n-flt.])
	fi
      fi
    fi
else
    HAVE_XFT=no
    HAVE_FREETYPE=no
    HAVE_LIBOTF=no
    HAVE_M17N_FLT=no
fi

### End of font-backend (under X11) section.

AC_SUBST([FREETYPE_CFLAGS])dnl
AC_SUBST([FREETYPE_LIBS])dnl
AC_SUBST([FONTCONFIG_CFLAGS])dnl
AC_SUBST([FONTCONFIG_LIBS])dnl
AC_SUBST([LIBOTF_CFLAGS])dnl
AC_SUBST([LIBOTF_LIBS])dnl
AC_SUBST([M17N_FLT_CFLAGS])dnl
AC_SUBST([M17N_FLT_LIBS])dnl

AC_MSG_CHECKING([more graphics packages])
AC_MSG_RESULT([ ])
### Use -lXpm if available, unless `--with-xpm=no'.
### mingw32 does NOT use -lXpm, since it loads the library dynamically.
HAVE_XPM=no
LIBXPM=""
if test "x${HAVE_W32}" = "xyes" && test "x${opsys}" = "xcygwin"; then
  if test "x${with_xpm}" != "xno"; then
    SAVE_CPPFLAGS="${CPPFLAGS}"
    SAVE_LDFLAGS="${LDFLAGS}"
    CPPFLAGS="${CPPFLAGS} -I/usr/include/noX"
    LDFLAGS="${LDFLAGS} -L/usr/lib/noX"
    AC_CHECK_HEADER([X11/xpm.h],
      [AC_CHECK_LIB([Xpm],[XpmReadFileToImage],[HAVE_XPM=yes])])
    if test "x${HAVE_XPM}" = "xyes"; then
      AC_MSG_CHECKING([for XpmReturnAllocPixels preprocessor define])
      AC_EGREP_CPP([no_return_alloc_pixels],
      [#include "X11/xpm.h"
#ifndef XpmReturnAllocPixels
no_return_alloc_pixels
#endif /* !XpmReturnAllocPixels */
      ],[HAVE_XPM=no],[HAVE_XPM=yes])dnl

      if test "x${HAVE_XPM}" = "xyes"; then
        REAL_CPPFLAGS="${REAL_CPPFLAGS} -I/usr/include/noX"
	AC_MSG_RESULT([yes])
      else
	AC_MSG_RESULT([no])
        CPPFLAGS="${SAVE_CPPFLAGS}"
        LDFLAGS="${SAVE_LDFLAGS}"
      fi
    fi
  fi

  if test "x${HAVE_XPM}" = "xyes"; then
    AC_DEFINE([HAVE_XPM],[1],
              [Define to 1 if you have the Xpm library (-lXpm).])
    LIBXPM='-lXpm'
  fi
fi

## this seems duplicated...
if test "x${HAVE_X11}" = "xyes"; then
  if test "x${with_xpm}" != "xno"; then
    AC_CHECK_HEADER([X11/xpm.h],
      [AC_CHECK_LIB([Xpm],[XpmReadFileToPixmap],[HAVE_XPM=yes],[],
                    [-lX11])])
    if test "x${HAVE_XPM}" = "xyes"; then
      AC_MSG_CHECKING([for XpmReturnAllocPixels preprocessor define])
      AC_EGREP_CPP([no_return_alloc_pixels],
      [#include "X11/xpm.h"
#ifndef XpmReturnAllocPixels
no_return_alloc_pixels
#endif /* !XpmReturnAllocPixels */
      ],[HAVE_XPM=no],[HAVE_XPM=yes])dnl

      if test "x${HAVE_XPM}" = "xyes"; then
	AC_MSG_RESULT([yes])
      else
	AC_MSG_RESULT([no])
      fi
    fi
  fi

  if test "x${HAVE_XPM}" = "xyes"; then
    AC_DEFINE([HAVE_XPM],[1],
              [Define to 1 if you have the Xpm library (-lXpm).])
    LIBXPM='-lXpm'
  fi
fi

### FIXME: Perhaps regroup to minimize code duplication due to MinGW's
### slightly different requirements wrt image libraries (it does NOT
### use -lXpm because it loads the xpm shared library dynamically at
### run time).
if test "x${opsys}" = "xmingw32"; then
  if test "x${with_xpm}" != "xno"; then
    AC_CHECK_HEADER([X11/xpm.h],[HAVE_XPM=yes],[HAVE_XPM=no],[
#define FOR_MSW 1])
  fi

  if test "x${HAVE_XPM}" = "xyes"; then
    AC_DEFINE([HAVE_XPM],[1],
              [Define to 1 if you have the Xpm library (-lXpm).])
  fi
fi

AC_SUBST([LIBXPM])dnl

### Use -ljpeg if available, unless `--with-jpeg=no'.
### mingw32 does NOT use -ljpeg, since it loads the library dynamically.
HAVE_JPEG=no
LIBJPEG=""
if test "x${opsys}" = "xmingw32"; then
  if test "x${with_jpeg}" != "xno"; then
    dnl# Checking for jpeglib.h can lose because of a redefinition of
    dnl# HAVE_STDLIB_H, so do it conditionally:
    if test "x${ac_cv_header_stdlib_h}" = "x"; then
      test -z "${ac_cv_header_stdlib_h}"
      AC_CHECK_HEADER([jpeglib.h],[HAVE_JPEG=yes],[:])
    fi
    dnl# and also check for this just in case:
    AC_CHECK_HEADER([jerror.h],[HAVE_JPEG=yes],[HAVE_JPEG=no])
  fi
  AH_TEMPLATE([HAVE_JPEG],
              [Define to 1 if you have the jpeg library (-ljpeg).])dnl
  if test "x${HAVE_JPEG}" = "xyes"; then
    AC_DEFINE([HAVE_JPEG])
    AC_EGREP_CPP([version= *(6[2-9]|[7-9][0-9])],
        [#include <jpeglib.h>
	 version=JPEG_LIB_VERSION
        ],[AC_DEFINE([HAVE_JPEG])],
        [AC_MSG_WARN([libjpeg found, but not version 6b or later])
         HAVE_JPEG=no])
  fi
elif test "x${HAVE_X11}" = "xyes" || test "x${HAVE_W32}" = "xyes"; then
  if test "x${with_jpeg}" != "xno"; then
    dnl# Checking for jpeglib.h can lose because of a redefinition of
    dnl# HAVE_STDLIB_H, so do it conditionally:
    if test "x${ac_cv_header_stdlib_h}" = "x"; then
      test -z "${ac_cv_header_stdlib_h}"
      AC_CHECK_HEADER([jpeglib.h],[HAVE_JPEG=yes],[:])
    fi
    dnl# and also check for this just in case:
    AC_CHECK_HEADER([jerror.h],
      [AC_CHECK_LIB([jpeg],[jpeg_destroy_compress],[HAVE_JPEG=yes],
        [AC_CHECK_LIB([jpeg],[main],[HAVE_JPEG=yes],
          [AC_SEARCH_LIBS([jpeg_destroy_compress],
                          [jpeg jpeg-turbo mozjpeg],[HAVE_JPEG=yes])
        ])
      ])
    ])
  fi

  AH_TEMPLATE([HAVE_JPEG],
              [Define to 1 if you have the jpeg library (-ljpeg).])dnl
  if test "x${HAVE_JPEG}" = "xyes"; then
    AC_DEFINE(HAVE_JPEG)
    AC_EGREP_CPP([version= *(6[2-9]|[7-9][0-9])],
	[#include <jpeglib.h>
	 version=JPEG_LIB_VERSION
        ],[AC_DEFINE([HAVE_JPEG])],
	[AC_MSG_WARN([libjpeg found, but not version 6b or later])
	 HAVE_JPEG=no])
  fi
  if test "x${HAVE_JPEG}" = "xyes"; then
    LIBJPEG='-ljpeg'
  fi
fi
AC_SUBST([LIBJPEG])dnl

### Use -lpng if available, unless `--with-png=no'.
### mingw32 does NOT use -lpng, since it loads the library dynamically.
HAVE_PNG=no
LIBPNG=""
if test "x${opsys}" = "xmingw32"; then
  if test "x${with_png}" != "xno"; then
    AC_CHECK_HEADER([png.h],[HAVE_PNG=yes],[HAVE_PNG=no])
  fi
  if test "x${HAVE_PNG}" = "xyes"; then
    AC_DEFINE([HAVE_PNG],[1],
              [Define to 1 if you have the png library (-lpng).])dnl

    AC_CHECK_DECL([png_longjmp],[],
      [AC_DEFINE([PNG_DEPSTRUCT],[],
	 [Define to empty to suppress deprecation warnings when building
	  with --enable-gcc-warnings and with libpng versions before 1.5,
	  which lack png_longjmp.])],
      [[#ifdef HAVE_LIBPNG_PNG_H
	# include <libpng/png.h>
	#else
	# include <png.h>
	#endif /* HAVE_LIBPNG_PNG_H */
      ]])
  fi
elif test "x${HAVE_X11}" = "xyes" || test "x${HAVE_W32}" = "xyes"; then
  if test "x${with_png}" != "xno"; then
    # Debian unstable as of July 2003 has multiple libpngs, and puts png.h
    # in /usr/include/libpng.
    AC_CHECK_HEADERS([png.h libpng/png.h],[break])
    if test "x${ac_cv_header_png_h}" = "xyes" || test "x${ac_cv_header_libpng_png_h}" = "xyes"; then
      AC_CHECK_LIB([png],[png_get_channels],[HAVE_PNG=yes],[
        AC_CHECK_LIB([png],[main],[HAVE_PNG=yes],[],[-lz -lm])
      ],[-lz -lm])
    fi
  fi

  if test "x${HAVE_PNG}" = "xyes"; then
    AC_DEFINE([HAVE_PNG],[1],
              [Define to 1 if you have the png library (-lpng).])
    LIBPNG="-lpng -lz -lm"

    AC_CHECK_DECL([png_longjmp],[],
      [AC_DEFINE([PNG_DEPSTRUCT],[],
	 [Define to empty to suppress deprecation warnings when building
	  with --enable-gcc-warnings and with libpng versions before 1.5,
	  which lack png_longjmp.])],
      [[#ifdef HAVE_LIBPNG_PNG_H
	# include <libpng/png.h>
	#else
	# include <png.h>
	#endif /* HAVE_LIBPNG_PNG_H */
      ]])
  fi
fi
AC_SUBST([LIBPNG])dnl

HAVE_ZLIB=no
LIBZ=""
if test "x${with_zlib}" != "xno"; then
  OLIBS="${LIBS}"
  AC_SEARCH_LIBS([inflateEnd],[z],[HAVE_ZLIB=yes])
  LIBS="${OLIBS}"
  case ${ac_cv_search_inflateEnd} in
    -*) LIBZ="${ac_cv_search_inflateEnd}" ;;
  esac
fi
if test "x${HAVE_ZLIB}" = "xyes"; then
  AC_DEFINE([HAVE_ZLIB],[1],
            [Define to 1 if you have the zlib library (-lz).])
  ### mingw32 does NOT use -lz, since it loads the library dynamically.
  if test "x${opsys}" = "xmingw32"; then
     LIBZ=""
  fi
fi
AC_SUBST([LIBZ])dnl

dnl# tiff section:
### Use -ltiff if available, unless `--with-tiff=no'.
### mingw32 does NOT use -ltiff, since it loads the library dynamically.
HAVE_TIFF=no
LIBTIFF=""
if test "x${opsys}" = "xmingw32"; then
  if test "x${with_tiff}" != "xno"; then
    AC_CHECK_HEADER([tiffio.h],[HAVE_TIFF=yes],[HAVE_TIFF=no])
  fi
  if test "x${HAVE_TIFF}" = "xyes"; then
    AC_DEFINE([HAVE_TIFF],[1],
              [Define to 1 if you have the tiff library (-ltiff).])
  fi
elif test "x${HAVE_X11}" = "xyes" || test "x${HAVE_W32}" = "xyes"; then
  if test "x${with_tiff}" != "xno"; then
    AC_CHECK_HEADER([tiffio.h],
      [tifflibs="-lz -lm"
       # At least one tiff package requires the jpeg library.
       if test "x${HAVE_JPEG}" = "xyes"; then
         tifflibs="-ljpeg ${tifflibs}";
       fi
       AC_CHECK_LIB([tiff],[TIFFGetVersion],[HAVE_TIFF=yes],[
         AC_CHECK_LIB([tiff],[main],[HAVE_TIFF=yes],[],[${tifflibs}])
       ],[${tifflibs}])])
  fi

  if test "x${HAVE_TIFF}" = "xyes"; then
    AC_DEFINE([HAVE_TIFF],[1],
              [Define to 1 if you have the tiff library (-ltiff).])
    dnl# FIXME -lz -lm, as per libpng?
    LIBTIFF='-ltiff'
  fi
fi
AC_SUBST([LIBTIFF])dnl

### Use -lgif or -lungif if available, unless `--with-gif=no'.
### mingw32 does NOT use -lgif/-lungif, since it loads the library
### dynamically.
HAVE_GIF=no
LIBGIF=""
if test "x${opsys}" = "xmingw32"; then
  if test "x${with_gif}" != "xno"; then
    AC_CHECK_HEADER([gif_lib.h],[HAVE_GIF=yes],[HAVE_GIF=no])
  fi
  if test "x${HAVE_GIF}" = "xyes"; then
    AC_DEFINE([HAVE_GIF],[1],
              [Define to 1 if you have a gif (or ungif) library.])
  fi
elif test "x${HAVE_X11}" = "xyes" && test "x${with_gif}" != "xno" \
        || test "x${HAVE_W32}" = "xyes"; then
  AC_CHECK_HEADER([gif_lib.h],
# EGifPutExtensionLast only exists from version libungif-4.1.0b1.
# Earlier versions can crash Emacs, but version 5.0 removes
# EGifPutExtensionLast.
    [AC_CHECK_LIB([gif],[GifMakeMapObject],[HAVE_GIF=yes],
        [AC_CHECK_LIB([gif],[EGifPutExtensionLast],
                      [HAVE_GIF=yes],
            [AC_CHECK_LIB([gif],[main],[HAVE_GIF=yes],[HAVE_GIF=maybe])dnl
        ])dnl
    ])dnl
  ])dnl

  if test "x${HAVE_GIF}" = "xyes"; then
    LIBGIF='-lgif'
  elif test "x${HAVE_GIF}" = "xmaybe"; then
# If gif_lib.h but no libgif, try libungif.
    AC_CHECK_LIB([ungif],[EGifPutExtensionLast],[HAVE_GIF=yes],[
      AC_CHECK_LIB([ungif],[main],[HAVE_GIF=yes],[HAVE_GIF=no])
    ])
    test "x${HAVE_GIF}" = "xyes" && export LIBGIF='-lungif'
  fi

  if test "x${HAVE_GIF}" = "xyes"; then
    AC_DEFINE([HAVE_GIF],[1],
              [Define to 1 if you have a gif (or ungif) library.])
  fi
fi
AC_SUBST([LIBGIF])dnl

dnl# Check for required libraries.
if test "x${HAVE_X11}" = "xyes"; then
  MISSING=""
  WITH_NO=""
  test "x${with_xpm}" != "xno" && test "x${HAVE_XPM}" != "xyes" &&
    MISSING="libXpm" && WITH_NO="--with-xpm=no"
  test "x${with_jpeg}" != "xno" && test "x${HAVE_JPEG}" != "xyes" &&
    MISSING="${MISSING} libjpeg" && WITH_NO="${WITH_NO} --with-jpeg=no"
  test "x${with_png}" != "xno" && test "x${HAVE_PNG}" != "xyes" &&
    MISSING="${MISSING} libpng" && WITH_NO="${WITH_NO} --with-png=no"
  test "x${with_gif}" != "xno" && test "x${HAVE_GIF}" != "xyes" &&
    MISSING="${MISSING} libgif/libungif" && WITH_NO="${WITH_NO} --with-gif=no"
  test "x${with_tiff}" != "xno" && test "x${HAVE_TIFF}" != "xyes" &&
    MISSING="${MISSING} libtiff" && WITH_NO="${WITH_NO} --with-tiff=no"

  if test "X${MISSING}" != "X"; then
    AC_MSG_ERROR([The following required libraries were not found:
    ${MISSING}
Maybe some development libraries/packages are missing?
If you do NOT want to link with them give
    ${WITH_NO}
as options to configure])
  fi
fi

### Use -lgpm if available, unless `--with-gpm=no'.
HAVE_GPM=no
LIBGPM=""
if test "x${with_gpm}" != "xno"; then
  AC_CHECK_HEADER([gpm.h],
    [AC_CHECK_LIB([gpm],[Gpm_Open],[HAVE_GPM=yes])])dnl

  if test "x${HAVE_GPM}" = "xyes"; then
    AC_DEFINE([HAVE_GPM],[1],
              [Define to 1 if you have the gpm library (-lgpm).])
    LIBGPM='-lgpm'
  fi
fi
AC_SUBST([LIBGPM])dnl

AC_MSG_CHECKING([darwin/NeXTstep])
AC_MSG_RESULT([ ])
dnl# Check for malloc/malloc.h on darwin:
case "${opsys}" in
  darwin)
    test -z "${ac_cv_header_malloc_malloc_h}"
    dnl# "_ONCE" causes test to jump out of the conditional and be done at
    dnl# wrong location
    AC_CHECK_HEADERS([malloc/malloc.h])
    ;;
esac

GNUSTEP_CFLAGS=""
### Use NeXTstep API to implement GUI.
if test "x${HAVE_NS}" = "xyes"; then
  AC_DEFINE([HAVE_NS],[1],
            [Define to 1 if you are using the NeXTstep API, either GNUstep or Cocoa on Mac OS X.])
  if test "x${NS_IMPL_COCOA}" = "xyes"; then
    AC_DEFINE([NS_IMPL_COCOA],[1],
              [Define to 1 if you are using NS windowing under MacOS X.])
  fi
  if test "x${NS_IMPL_GNUSTEP}" = "xyes"; then
    AC_DEFINE([NS_IMPL_GNUSTEP],[1],
              [Define to 1 if you are using NS windowing under GNUstep.])
    # See also .m.o rule in Makefile.in */
    # FIXME: are all these flags really needed?  Document here why.  */
    GNUSTEP_CFLAGS="-D_REENTRANT -fPIC -fno-strict-aliasing -I${GNUSTEP_SYSTEM_HEADERS} ${GNUSTEP_LOCAL_HEADERS}"
    ## Extra CFLAGS applied to src/*.m files.
    GNU_OBJC_CFLAGS="${GNU_OBJC_CFLAGS} -fgnu-runtime -Wno-import -fconstant-string-class=NSConstantString -DGNUSTEP_BASE_LIBRARY=1 -DGNU_GUI_LIBRARY=1 -DGNU_RUNTIME=1 -DGSWARN -DGSDIAGNOSE"
  fi
  OTHER_FILES=ns-app
fi

AC_MSG_CHECKING([even more X11 packages])
AC_MSG_RESULT([ ])
### Use session management (-lSM -lICE) if available:
HAVE_X_SM=no
LIBXSM=""
if test "x${HAVE_X11}" = "xyes"; then
  AC_CHECK_HEADER([X11/SM/SMlib.h],
    [AC_CHECK_LIB([SM],[SmcOpenConnection],[HAVE_X_SM=yes],[],[-lICE])])dnl

  if test "x${HAVE_X_SM}" = "xyes"; then
    AC_DEFINE([HAVE_X_SM],[1],
              [Define to 1 if you have the SM library (-lSM).])
    LIBXSM="-lSM -lICE"
    case "${LIBS}" in
      *-lSM*) ;;
      *)      LIBS="${LIBXSM} ${LIBS}" ;;
    esac
  fi
fi
AC_SUBST([LIBXSM])dnl

### Use XRandr (-lXrandr) if available:
HAVE_XRANDR=no
if test "x${HAVE_X11}" = "xyes"; then
  XRANDR_REQUIRED=1.2.2
  XRANDR_MODULES="xrandr >= ${XRANDR_REQUIRED}"
  PKG_CHECK_MODULES([XRANDR],[${XRANDR_MODULES}],
                    [HAVE_XRANDR=yes],[HAVE_XRANDR=no])
  if test "x${HAVE_XRANDR}" = "xno"; then
    # Test old way in case pkg-config does NOT have it (older machines).
    AC_CHECK_HEADER([X11/extensions/Xrandr.h],
      [AC_CHECK_LIB([Xrandr],[XRRGetScreenResources],[HAVE_XRANDR=yes])])
    if test "x${HAVE_XRANDR}" = "xyes"; then
      XRANDR_LIBS='-lXrandr'
      AC_SUBST([XRANDR_LIBS])
    fi
  fi
  if test "x${HAVE_XRANDR}" = "xyes"; then
    SAVE_CFLAGS="${CFLAGS}"
    SAVE_LIBS="${LIBS}"
    CFLAGS="${XRANDR_CFLAGS} ${CFLAGS}"
    LIBS="${XRANDR_LIBS} ${LIBS}"
    AC_CHECK_FUNCS([XRRGetOutputPrimary XRRGetScreenResourcesCurrent])
    CFLAGS="${SAVE_CFLAGS}"
    LIBS="${SAVE_LIBS}"

    AC_DEFINE([HAVE_XRANDR],[1],
              [Define to 1 if you have the XRandr extension.])
  fi
fi

### Use Xinerama (-lXinerama) if available:
HAVE_XINERAMA=no
if test "x${HAVE_X11}" = "xyes"; then
  XINERAMA_REQUIRED=1.0.2
  XINERAMA_MODULES="xinerama >= ${XINERAMA_REQUIRED}"
  PKG_CHECK_MODULES([XINERAMA],[${XINERAMA_MODULES}],[HAVE_XINERAMA=yes],
                    [HAVE_XINERAMA=no])
  if test "x${HAVE_XINERAMA}" = "xno"; then
    # Test old way in case pkg-config does NOT have it (older machines).
    AC_CHECK_HEADER([X11/extensions/Xinerama.h],
      [AC_CHECK_LIB([Xinerama],[XineramaQueryExtension],
                    [HAVE_XINERAMA=yes])])
    if test "x${HAVE_XINERAMA}" = "xyes"; then
      XINERAMA_LIBS='-lXinerama'
      AC_SUBST([XINERAMA_LIBS])
    fi
  fi
  if test "x${HAVE_XINERAMA}" = "xyes"; then
    AC_DEFINE([HAVE_XINERAMA],[1],
              [Define to 1 if you have the Xinerama extension.])
  fi
fi

AC_MSG_CHECKING([various other libraries])
AC_MSG_RESULT([ ])
### Use libxml (-lxml2) if available.
### mingw32 does NOT use -lxml2, since it loads the library dynamically.
HAVE_LIBXML2=no
if test "x${with_xml2}" != "xno"; then
  ### I am not sure what the version number should be, so I just guessed.
  PKG_CHECK_MODULES([LIBXML2],[libxml-2.0 > 2.6.17],
                    [HAVE_LIBXML2=yes],[HAVE_LIBXML2=no])
  # Built-in libxml2 on OS X 10.8+ lacks libxml-2.0.pc.
  if test "x${HAVE_LIBXML2}" != "xyes" -a "x${opsys}" = "xdarwin"; then
    SAVE_CPPFLAGS="${CPPFLAGS}"
    CPPFLAGS="${CPPFLAGS} -I${xcsdkdir}/usr/include/libxml2"
    AC_CHECK_HEADER([libxml/HTMLparser.h],
      [AC_CHECK_DECL([HTML_PARSE_RECOVER],[HAVE_LIBXML2=yes],[],
      		     [#include <libxml/HTMLparser.h>])])
    CPPFLAGS="${SAVE_CPPFLAGS}"
    if test "x${HAVE_LIBXML2}" = "xyes"; then
      LIBXML2_CFLAGS="-I'${xcsdkdir}/usr/include/libxml2'"
      LIBXML2_LIBS="-lxml2"
    fi
  fi
  if test "x${HAVE_LIBXML2}" = "xyes"; then
    if test "x${opsys}" != "xmingw32"; then
      LIBS="${LIBXML2_LIBS} ${LIBS}"
      AC_CHECK_LIB([xml2],[htmlReadMemory],
                   [HAVE_LIBXML2=yes],[HAVE_LIBXML2=no])
    else
      LIBXML2_LIBS=""
    fi
    if test "x${HAVE_LIBXML2}" = "xyes"; then
      AC_DEFINE([HAVE_LIBXML2],[1],
                [Define to 1 if you have the libxml library (-lxml2).])
    else
      LIBXML2_LIBS=""
      LIBXML2_CFLAGS=""
    fi
  fi
fi
AC_SUBST([LIBXML2_LIBS])dnl
AC_SUBST([LIBXML2_CFLAGS])dnl

# If netdb.h does NOT declare h_errno, we must declare it by hand.
# On MinGW, that is provided by nt/inc/sys/socket.h and w32.c.
if test "x${opsys}" = "xmingw32"; then
  emacs_cv_netdb_declares_h_errno=yes
fi
AC_CACHE_CHECK([whether netdb declares h_errno],
	       [emacs_cv_netdb_declares_h_errno],
[AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <netdb.h>]],
  [[return h_errno;]])],
  [emacs_cv_netdb_declares_h_errno=yes],
  [emacs_cv_netdb_declares_h_errno=no])])
if test "x${emacs_cv_netdb_declares_h_errno}" = "xyes"; then
  AC_DEFINE([HAVE_H_ERRNO],[1],[Define to 1 if netdb.h declares h_errno.])
fi

# sqrt and other floating-point functions such as fmod and frexp
# are found in -lm on most systems, but mingw32 does NOT use -lm.
if test "x${opsys}" != "xmingw32"; then
  m4_ifdef([LT_LIB_M],[
    LT_LIB_M
  ],[
#HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
    AC_CHECK_LIB([m],[sqrt])
    AC_CHECK_LIB([m],[fmod])
    AC_CHECK_LIB([m],[frexp])
  ])
fi

AC_MSG_CHECKING([mail libraries])
AC_MSG_RESULT([ ])
# Check for mail-locking functions in a "mail" library. Probably this
# should have the same check as for liblockfile below.
AC_CHECK_LIB([mail],[maillock],[have_mail=yes],[have_mail=no])
if test "x${have_mail}" = "xyes"; then
  LIBS_MAIL='-lmail'
  LIBS="${LIBS_MAIL} ${LIBS}"
  AC_DEFINE([HAVE_LIBMAIL],[1],
            [Define to 1 if you have the `mail' library (-lmail).])
else
  LIBS_MAIL=""
fi
dnl# Debian, at least:
AC_CHECK_LIB([lockfile],[maillock],[have_lockfile=yes],[have_lockfile=no])
if test "x${have_lockfile}" = "xyes"; then
   LIBS_MAIL='-llockfile'
   LIBS="${LIBS_MAIL} ${LIBS}"
   AC_DEFINE([HAVE_LIBLOCKFILE],[1],
             [Define to 1 if you have the `lockfile' library (-llockfile)])
else
# If we have the shared liblockfile, assume we must use it for mail
# locking (e.g. Debian).  If we could NOT link against liblockfile
# (no liblockfile.a installed), ensure that we do NOT need to.
  dnl# This works for files generally, not just executables.
  dnl# Should we look elsewhere for it?  Maybe examine /etc/ld.so.conf?
  AC_CHECK_PROG([liblockfile],[liblockfile.so],[yes],[no],
                [/usr/lib:/lib:/usr/local/lib:${LD_LIBRARY_PATH}])
  if test "x${ac_cv_prog_liblockfile}" = "xyes"; then
    AC_MSG_ERROR([Shared liblockfile found but cannot link against it.
This probably means that movemail could lose mail.
There may be a `development' package to install containing liblockfile.])
  fi
fi
dnl# "_ONCE" causes this test to be done at wrong location, unfortunately:
AC_CHECK_HEADERS([maillock.h])
AC_SUBST([LIBS_MAIL])dnl

dnl# m4-quotation block number 12 begin:
[
## Define MAIL_USE_FLOCK (or LOCKF) if the mailer uses flock (or lockf) to
## interlock access to the mail spool. The alternative is a lock file named
## /usr/spool/mail/${USER}.lock.
mail_lock=no
case "${opsys}" in
  aix4-2) mail_lock="lockf" ;;

  gnu|freebsd|dragonfly|netbsd|openbsd|darwin|irix6-5) mail_lock="flock" ;;

  ## On GNU/Linux systems, both methods are used by various mail programs.
  ## I assume most people are using newer mailers that have heard of flock.
  ## Change this if you need to.
  ## Debian contains a patch which says: ``On Debian/GNU/Linux systems,
  ## configure gets the right answers, and that means *NOT* using flock.
  ## Using flock is guaranteed to be the wrong thing. See Debian Policy
  ## for details.'' and then uses `#ifdef DEBIAN'.  Unfortunately the
  ## Debian maintainer has NOT provided a clean fix for Emacs.
  ## movemail.c will use `maillock' when MAILDIR, HAVE_LIBMAIL and
  ## HAVE_MAILLOCK_H are defined, so the following appears to be the
  ## correct logic.  -- fx
  ## We must check for HAVE_LIBLOCKFILE too, as movemail does.
  ## liblockfile is a Free Software replacement for libmail, used on
  ## Debian systems and elsewhere. -rfr.
  gnu-*)
    mail_lock="flock"
    if test "x${have_mail}" = "xyes" || test "x${have_lockfile}" = "xyes"; then
      test "x${ac_cv_header_maillock_h}" = "xyes" && mail_lock=no
    fi
    ;;

  mingw32)
    mail_lock="none-needed" ;;
esac
]
dnl# m4-quotation block number 12 end

BLESSMAIL_TARGET=""
case "${mail_lock}" in
  flock)
    AC_DEFINE([MAIL_USE_FLOCK],[1],
              [Define to 1 if mailer uses flock to interlock mail spool])
    ;;

  lockf)
    AC_DEFINE([MAIL_USE_LOCKF],[1],
              [Define to 1 if mailer uses lockf to interlock mail spool])
    ;;
  none-needed) ;;
  *) BLESSMAIL_TARGET="need-blessmail" ;;
esac
AC_SUBST([BLESSMAIL_TARGET])dnl

dnl# checks for other various functions:
AC_MSG_CHECKING([functions])
AC_MSG_RESULT([ ])
AC_FUNC_CHOWN
AC_FUNC_CLOSEDIR_VOID
dnl# definition of this next macro has been moved to the included file:
m4_include([m4/functions_extra.m4])dnl
AC_REQUIRE_VARIOUS_FUNC_CHECKS
AC_FUNC_LSTAT dnl# also checks whether it "_FOLLOWS_SLASHED_SYMLINK"
if test "x${emacs_cv_var_doug_lea_malloc}" = "x"; then
  if test "x${system_malloc}" = "x"; then
    if test "x${GNU_MALLOC}" -a "x${GMALLOC_OBJ}" = "x"; then
      test -z "${ac_cv_func_malloc}"
      AC_FUNC_MALLOC
    fi
  fi
fi
AC_FUNC_MEMCMP
AC_FUNC_MKTIME
AC_FUNC_REALLOC
AC_FUNC_SELECT_ARGTYPES
AC_FUNC_SETPGRP
AC_FUNC_SETVBUF_REVERSED
AC_FUNC_STAT
AC_FUNC_STRERROR_R
AC_FUNC_STRFTIME
AC_FUNC_STRTOD
AC_FUNC_UTIME_NULL
AC_CHECK_FUNCS([accept4 gethostname \
getrusage get_current_dir_name \
lrand48 \
select setlocale \
getrlimit setrlimit shutdown getaddrinfo \
strsignal setitimer \
sendto recvfrom getsockname getpeername getifaddrs freeifaddrs \
gai_strerror getline getdelim sync \
getpwent endpwent getgrent endgrent \
touchlock \
cfmakeraw cfsetspeed copysign __executable_start log2])
AC_CHECK_FUNCS([__fpending acl atexit btowc bzero doprnt dup2 fchdir \
                floor ftime ftruncate getcwd getpass inet_ntoa isascii \
                mbrlen memchr memmove memset mkdir munmap nl_langinfo \
                posix_spawnp pow putenv re_comp regcomp rmdir \
                setenv socket sqrt strcasecmp strchr strcspn strdup \
                strerror strpbrk strrchr strstr strtol strtoul \
                uname utime utmpname])
dnl# Keep autoscan happy with a dummy condition:
if test "x" = "Y0"; then
  test -x `which autoscan`
  if test "x${ac_cv_lib_error_at_line}" = "x" || test "x${ac_cv_lib_error_at_line}" = "xno"; then
    test -z "${ac_cv_lib_error_at_line}"
    AC_FUNC_ERROR_AT_LINE
  fi
  AC_CHECK_FUNCS([alarm gethostbyname getpagesize gettimeofday \
                  localtime_r strtoumax tzset])
fi

dnl# No need to check for aligned_alloc and posix_memalign if using
dnl# gmalloc.o, as it supplies them. Do NOT use these functions on
dnl# Darwin though, as they are incompatible with unexmacosx.c.
if test -z "${GMALLOC_OBJ}" && test "x${opsys}" != "xdarwin"; then
  AC_CHECK_FUNCS([aligned_alloc posix_memalign],[break])
fi

## Eric Backus <ericb@lsid.hp.com> says, HP-UX 9.x on HP 700 machines
## has a broken `rint' in some library versions including math library
## version number A.09.05.
## You can fix the math library by installing patch number PHSS_4630.
## But we can fix it more reliably for Emacs by just not using rint.
## We also skip HAVE_RANDOM - see comments in src/conf_post.h.
case ${opsys} in
   hpux*) : ;;
   *) AC_CHECK_FUNCS([random rint]) ;;
esac

dnl# Cannot use AC_CHECK_FUNCS fo this:
AC_CACHE_CHECK([for __builtin_unwind_init],
	       [emacs_cv_func___builtin_unwind_init],
[AC_LINK_IFELSE([AC_LANG_PROGRAM([[]],[[__builtin_unwind_init();]])],
		[emacs_cv_func___builtin_unwind_init=yes],
		[emacs_cv_func___builtin_unwind_init=no])])
if test "x${emacs_cv_func___builtin_unwind_init}" = "xyes"; then
  AC_DEFINE([HAVE___BUILTIN_UNWIND_INIT],[1],
	    [Define to 1 if you have the `__builtin_unwind_init' function])
fi

dnl# "_ONCE" causes test to be done at wrong location
AC_CHECK_HEADERS([sys/un.h])dnl

AC_FUNC_FSEEKO

# UNIX98 PTYs.
AC_CHECK_FUNCS([grantpt])dnl

# PTY-related GNU extensions.
AC_CHECK_FUNCS([getpt posix_openpt])dnl

# Check this now, so that we will NOT find the above functions in ncurses.
# That is because we have not set up to link ncurses in lib-src.
# It is better to believe a function is not available
# than to expect to find it in ncurses.
# Also we need tputs and friends to be able to build at all.
AC_CHECK_FUNCS([tputs])
AC_MSG_CHECKING([for library containing tputs])
# Run a test program that contains a call to tputs, a call that is
# never executed. This tests whether a pre-'main' dynamic linker
# works with the library. It is too much trouble to actually call
# tputs in the test program, due to portability hassles. When
# cross-compiling, assume the test program will run if it links.
AC_DEFUN([tputs_link_source],[
  AC_LANG_SOURCE([[
       extern void tputs(const char *, int, int (*)(int));
       int main(int argc, char **argv)
       {
	  if (argc == 10000) {
	    tputs(argv[0], 0, 0);
          }
	  return 0;
       }]])
])
if test "x${opsys}" = "xmingw32"; then
  msg='none required'
else
  # Maybe curses should be tried earlier?
  # See http://debbugs.gnu.org/cgi/bugreport.cgi?bug=9736#35
  for tputs_library in '' tinfo ncurses terminfo termcap curses System termkey tputs gnu c iberty; do
    OLIBS="${LIBS}"
    if test -z "${tputs_library}"; then
      LIBS_TERMCAP=""
      msg='none required'
    else
      LIBS_TERMCAP=-l${tputs_library}
      msg="${LIBS_TERMCAP}"
      LIBS="${LIBS_TERMCAP} ${LIBS}"
    fi
    AC_RUN_IFELSE([tputs_link_source],[],[msg=no],
      [AC_LINK_IFELSE([tputs_link_source],[],[msg=no])])
    LIBS="${OLIBS}"
    if test "X${msg}" != "Xno"; then
      break
    fi
  done
fi
AC_MSG_RESULT([${msg}])
if test "X${msg}" = "Xno"; then
  AC_MSG_NOTICE([trying some more tputs checks before really erroring out])
  AC_SEARCH_LIBS([tputs],
 [tinfo ncurses terminfo termcap curses System termkey tputs gnu c iberty])
  AC_MSG_ERROR([The required function `tputs' was not found in any library
(at least not when we first checked).
The following libraries were tried (in order):
  libtinfo, libncurses, libterminfo, libtermcap, libcurses, libSystem,
  libtermkey, libtputs, libgnu, libc, libiberty
Please try installing whichever of these libraries is most appropriate
for your system, together with its header files.
For example, a libncurses-dev(el) or similar package.

If a library is found when checking the second way and you still get
this error, check config.log for any relevant differences between
the different conftests.])
fi

## Use termcap instead of terminfo?
## Only true for: freebsd < 40000, ms-w32, msdos, netbsd < 599002500.
TERMINFO=yes
## FIXME?  In the cases below where we unconditionally set
## LIBS_TERMCAP="-lncurses", this overrides LIBS_TERMCAP = -ltinfo,
## if that was found above to have tputs.
## Should we use the gnu* logic everywhere?
case "${opsys}" in
  ## darwin: Prevent crashes when running Emacs in Terminal.app under 10.2.
  ##  The ncurses library has been moved out of the System framework in
  ##  Mac OS X 10.2.  So if configure detects it, set the command-line
  ##  option to use it.
  darwin) LIBS_TERMCAP="-lncurses" ;;

  gnu*) test -z "${LIBS_TERMCAP}" && LIBS_TERMCAP="-lncurses" ;;

  freebsd)
    dnl# "_ONCE" causes test to jump out of the conditional and be done at
    dnl# wrong location
    AC_CHECK_HEADERS([osreldate.h])
    AC_MSG_CHECKING([whether FreeBSD is new enough to use terminfo])
    AC_CACHE_VAL([emacs_cv_freebsd_terminfo],
    [AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <osreldate.h>]],
[[#if __FreeBSD_version < 400000
fail;
#endif /* old FreeBSD */
]])],[emacs_cv_freebsd_terminfo=yes],[emacs_cv_freebsd_terminfo=no])])dnl

    AC_MSG_RESULT([${emacs_cv_freebsd_terminfo}])dnl

    if test "x${emacs_cv_freebsd_terminfo}" = "xyes"; then
      LIBS_TERMCAP="-lncurses"
    else
      TERMINFO=no
      LIBS_TERMCAP="-ltermcap"
    fi
    ;;

  mingw32)
    TERMINFO=no
    LIBS_TERMCAP=""
    ;;

  netbsd)
    if test "x${LIBS_TERMCAP}" != "x-lterminfo"; then
      TERMINFO=no
      LIBS_TERMCAP="-ltermcap"
    fi
    ;;

  openbsd | dragonfly) LIBS_TERMCAP="-lncurses" ;;

  ## hpux: Make sure we get select from libc rather than from libcurses
  ##  because libcurses on HPUX 10.10 has a broken version of select.
  ##  We used to use -lc -lcurses, but this may be cleaner.
  ## FIXME?  But TERMINFO = yes on hpux (it used to be explicitly
  # set that way, now it uses the default). Is this not a contradiction?
  hpux*) LIBS_TERMCAP="-ltermcap" ;;

esac

TERMCAP_OBJ=tparam.o
if test "x${TERMINFO}" = "xyes"; then
  AC_DEFINE([TERMINFO],[1],
            [Define to 1 if you use terminfo instead of termcap.])
  TERMCAP_OBJ=terminfo.o
fi
if test "X${LIBS_TERMCAP}" = "X-lncurses"; then
  AC_DEFINE([USE_NCURSES],[1],[Define to 1 if you use ncurses.])
fi
AC_SUBST([LIBS_TERMCAP])dnl
AC_SUBST([TERMCAP_OBJ])dnl

dnl# network resolving section:
# Do we have res_init, for detecting changes in /etc/resolv.conf?
# On Darwin, res_init appears not to be useful: see bug#562 and
# http://lists.gnu.org/archive/html/emacs-devel/2007-11/msg01467.html
resolv=no

if test "x${opsys}" != "xdarwin"; then
  AC_CHECK_HEADERS([arpa/nameser.h resolv.h])
  AC_CHECK_FUNCS([res_init])
  AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <netinet/in.h>
#include <arpa/nameser.h>
#include <resolv.h>]],
    [[return res_init();]])],
    [have_res_init=yes],[have_res_init=no])
  if test "x${have_res_init}" = "xno"; then
    OLIBS="${LIBS}"
    LIBS="${LIBS} -lresolv"
    AC_MSG_CHECKING([for res_init with -lresolv])
    AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <netinet/in.h>
#include <arpa/nameser.h>
#include <resolv.h>]],
      [[return res_init();]])],
      [have_res_init=yes],[have_res_init=no])
    AC_MSG_RESULT([${have_res_init}])
    if test "x${have_res_init}" = "xyes"; then
      resolv=yes
    fi
    LIBS="${OLIBS}"
  fi

  if test "x${have_res_init}" = "xyes"; then
    AC_DEFINE([HAVE_RES_INIT],[1],[Define to 1 if res_init is available.])
  fi
else
  AC_MSG_NOTICE([skipping check for res_init() on darwin])
fi                              dnl# !darwin

# Do we need the Hesiod library to provide the support routines?
dnl# FIXME? Should we be skipping this on Darwin too? (I would say "yes")
LIBHESIOD=""
if test "x${with_hesiod}" != "xno"; then
  # Do NOT set ${LIBS} here -- see comments above.  FIXME: which comments?
  AC_CHECK_FUNC([res_send],[],[AC_CHECK_FUNC([__res_send],[],
     [AC_CHECK_LIB([resolv],[res_send],[resolv=yes],
		   [AC_CHECK_LIB([resolv],[__res_send],[resolv=yes])])])])
  if test "x${resolv}" = "xyes"; then
    RESOLVLIB='-lresolv'
  else
    RESOLVLIB=""
  fi
  AC_CHECK_FUNC([hes_getmailhost],[],
                [AC_CHECK_LIB([hesiod],[hes_getmailhost],
	                      [hesiod=yes],[:],[${RESOLVLIB}])])dnl

  if test x"${hesiod}" = x"yes"; then
    LIBHESIOD='-lhesiod'
  fi
fi
AC_SUBST([LIBHESIOD])dnl

# Do we need libresolv (due to res_init or Hesiod)?
if test "x${resolv}" = "xyes" && test "x${opsys}" != "xdarwin"; then
  LIBRESOLV='-lresolv'
else
  LIBRESOLV=""
fi
AC_SUBST([LIBRESOLV])dnl

# These tell us which Kerberos-related libraries to use.
COM_ERRLIB=""
CRYPTOLIB=""
KRB5LIB=""
DESLIB=""
KRB4LIB=""

if test "x${with_kerberos}" != "xno"; then
  AC_CHECK_LIB([com_err],[com_err],[have_com_err=yes],[have_com_err=no])
  if test "x${have_com_err}" = "xyes"; then
    COM_ERRLIB='-lcom_err'
    LIBS="${COM_ERRLIB} ${LIBS}"
  fi
  AC_CHECK_LIB([crypto],[mit_des_cbc_encrypt],
               [have_crypto=yes],[have_crypto=no])
  if test "x${have_crypto}" = "xyes"; then
    CRYPTOLIB='-lcrypto'
    LIBS="${CRYPTOLIB} ${LIBS}"
  fi
  AC_CHECK_LIB([k5crypto],[mit_des_cbc_encrypt],
               [have_k5crypto=yes],[have_k5crypto=no])
  if test "x${have_k5crypto}" = "xyes"; then
    CRYPTOLIB='-lk5crypto'
    LIBS="${CRYPTOLIB} ${LIBS}"
  fi
  AC_CHECK_LIB([krb5],[krb5_init_context],[have_krb5=yes],[have_krb5=no])
  if test "x${have_krb5}" = "xyes"; then
    KRB5LIB='-lkrb5'
    LIBS="${KRB5LIB} ${LIBS}"
  fi
  dnl# FIXME: Simplify. Does not match 22 logic, thanks to default_off?
  if test "x${with_kerberos5}" = "xno"; then
    AC_CHECK_LIB([des425],[des_cbc_encrypt],
                 [have_des425=yes],[have_des425=no])
    if test "x${have_des425}" = "xyes"; then
      DESLIB='-ldes425'
      LIBS="${DESLIB} ${LIBS}"
    else
      AC_CHECK_LIB([des],[des_cbc_encrypt],[have_des=yes],[have_des=no])
      if test "x${have_des}" = "xyes"; then
        DESLIB='-ldes'
        LIBS="${DESLIB} ${LIBS}"
      fi
    fi
    AC_CHECK_LIB([krb4],[krb_get_cred],[have_krb4=yes],[have_krb4=no])
    if test "x${have_krb4}" = "xyes"; then
      KRB4LIB='-lkrb4'
      LIBS="${KRB4LIB} ${LIBS}"
    else
      AC_CHECK_LIB([krb],[krb_get_cred],[have_krb=yes],[have_krb=no])
      if test "x${have_krb}" = "xyes"; then
        KRB4LIB='-lkrb'
        LIBS="${KRB4LIB} ${LIBS}"
      fi
    fi
  fi

  if test "x${with_kerberos5}" != "xno"; then
    AC_CHECK_HEADERS([krb5.h],
      [AC_CHECK_MEMBERS([krb5_error.text, krb5_error.e_text],[],[],
		        [#include <krb5.h>])])
  else
    AC_CHECK_HEADERS([krb.h],[],
		     [AC_CHECK_HEADERS([kerberosIV/krb.h],[],
				       [AC_CHECK_HEADERS([kerberos/krb.h])
                                       ])
                     ])
  fi
  AC_CHECK_HEADERS([com_err.h])
fi

AC_SUBST([COM_ERRLIB])dnl
AC_SUBST([CRYPTOLIB])dnl
AC_SUBST([KRB5LIB])dnl
AC_SUBST([DESLIB])dnl
AC_SUBST([KRB4LIB])dnl

AC_CHECK_HEADERS([valgrind/valgrind.h])dnl

dnl# "_ONCE" causes test to be done at wrong location, so instead check the
dnl# cache value:
if test "x${ac_cv_func_tzset}" = "x"; then
  test -z "${ac_cv_func_tzset}"
  AC_CHECK_FUNCS([tzset])
fi
AC_MSG_CHECKING([whether localtime caches TZ])
AC_CACHE_VAL([emacs_cv_localtime_cache],
[if test "x${ac_cv_func_tzset}" = "xyes"; then
AC_RUN_IFELSE([AC_LANG_SOURCE([[#include <time.h>
char TZ_GMT0[] = "TZ=GMT0";
char TZ_PST8[] = "TZ=PST8";
main()
{
  time_t now = time((time_t *)0);
  int hour_GMT0, hour_unset;
  if (putenv(TZ_GMT0) != 0) {
    exit(1);
  }
  hour_GMT0 = localtime(&now)->tm_hour;
  unsetenv("TZ");
  hour_unset = localtime(&now)->tm_hour;
  if (putenv(TZ_PST8) != 0) {
    exit(1);
  }
  if (localtime(&now)->tm_hour == hour_GMT0) {
    exit(1);
  }
  unsetenv("TZ");
  if (localtime(&now)->tm_hour != hour_unset) {
    exit(1);
  }
  exit(0);
}]])],[emacs_cv_localtime_cache=no],[emacs_cv_localtime_cache=yes],
[# If we have tzset, assume the worst when cross-compiling.
emacs_cv_localtime_cache=yes])
else
	# If we lack tzset, report that localtime does not cache TZ,
	# since we cannot invalidate the cache if we do NOT have tzset.
	emacs_cv_localtime_cache=no
fi])dnl
AC_MSG_RESULT([${emacs_cv_localtime_cache}])
if test "x${emacs_cv_localtime_cache}" = "xyes"; then
  AC_DEFINE([LOCALTIME_CACHE],[1],
	    [Define to 1 if localtime caches TZ.])
fi

ok_so_far=yes
unset ac_cv_func_socket
AC_CHECK_FUNC([socket],[],[ok_so_far=no])
if test "x${ok_so_far}" = "xyes"; then
  unset ac_cv_header_netinet_in_h
  AC_CHECK_HEADER([netinet/in.h],[],[ok_so_far=no])
fi
if test "x${ok_so_far}" = "xyes"; then
  unset ac_cv_header_arpa_inet_h
  AC_CHECK_HEADER([arpa/inet.h],[],[ok_so_far=no])
fi
if test "x${ok_so_far}" = "xyes"; then
dnl# Fixme: Not used. Should this be HAVE_SOCKETS?
  AC_DEFINE([HAVE_INET_SOCKETS],[1],
	    [Define to 1 if you have inet sockets.])
fi

dnl# Check for a Solaris 2.4 vfork bug that Autoconf misses (through 2.69).
dnl# This can be removed once we assume Autoconf 2.70.
case ${canonical} in
  *-solaris2.4 | *-solaris2.4.*)
    dnl# Disable the Autoconf-generated vfork test.
    : ${ac_cv_func_vfork_works=no};;
esac

AC_FUNC_FORK

AC_CHECK_FUNCS([snprintf])dnl

dnl# Check this late. It depends on what other libraries (lrsvg, Gtk+ etc)
dnl# Emacs uses.
XGSELOBJ=""
AC_MSG_CHECKING([whether GLib is linked in])
AC_LINK_IFELSE([AC_LANG_PROGRAM(
	[[#include <glib.h>
 	]],
	[[g_print ("Hello world");]])],
     [links_glib=yes],
     [links_glib=no])
AC_MSG_RESULT([${links_glib}])
if test "x${links_glib}" = "xyes"; then
  AC_DEFINE([HAVE_GLIB],[1],[Define to 1 if GLib is linked in.])
  if test "x${HAVE_NS}" = "xno";then
    XGSELOBJ=xgselect.o
  fi
fi
AC_SUBST([XGSELOBJ])dnl

dnl# Adapted from Haible's version.
AC_CACHE_CHECK([for nl_langinfo and CODESET],[emacs_cv_langinfo_codeset],
  [AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <langinfo.h>]],
    [[char* cs = nl_langinfo(CODESET);]])],
    [emacs_cv_langinfo_codeset=yes],
    [emacs_cv_langinfo_codeset=no])
  ])
if test "x${emacs_cv_langinfo_codeset}" = "xyes"; then
  AC_DEFINE([HAVE_LANGINFO_CODESET],[1],
    [Define if you have <langinfo.h> and nl_langinfo(CODESET).])
fi

unset ac_cv_type_mbstate_t
AC_TYPE_MBSTATE_T

AC_CACHE_CHECK([for C restricted array declarations],
               [emacs_cv_c_restrict_arr],
  [AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[void fred (int x[__restrict]);]],
                                      [[]])],
                  [emacs_cv_c_restrict_arr=yes],
                  [emacs_cv_c_restrict_arr=no])])
if test "x${emacs_cv_c_restrict_arr}" = "xyes"; then
  AC_DEFINE([__restrict_arr],[__restrict],
    [Define to compiler's equivalent of C99 restrict keyword in array
     declarations. Define as empty for no equivalent.])
fi

dnl# Fixme: AC_SYS_POSIX_TERMIOS should probably be used, but it is not
dnl# clear how the tty code is related to POSIX and/or other versions of
dnl# termios. The following looks like a useful start:

AC_SYS_POSIX_TERMIOS
if test "x${ac_cv_sys_posix_termios}" = "xyes"; then
   AC_DEFINE([HAVE_TERMIOS],[1],[Define to 1 if you have POSIX-style
                                 functions & macros for terminal control])
   AC_DEFINE([HAVE_TCATTR],[1],
             [Define to 1 if you have tcgetattr and tcsetattr.])
fi

dnl# Turned on June 1996 supposing nobody will mind it.
dnl# MinGW emulates passwd database, so this feature does NOT make sense
dnl# there.
if test "x${opsys}" != "xmingw32"; then
   AC_DEFINE([AMPERSAND_FULL_NAME],[1],
             [Define to use the convention that an ampersand in the
              full name stands for the login id.])
fi

dnl# Every platform that uses configure supports this.
dnl# There is a create-lockfiles option you can
dnl# customize if you do not want the lock files to be written.
dnl# So it is not clear that this #define still needs to exist.
AC_DEFINE([CLASH_DETECTION],[1],
          [Define if you want lock files to be written, so that Emacs can
           tell instantly when you try to modify a file that someone else
           has modified in his/her Emacs.])dnl

dnl# Everybody supports this, except MS.
dnl# Seems like the kind of thing we should be testing for, though.
## Note: PTYs exist but are broken on darwin < 6. Use at your own risk.
if test "x${opsys}" != "xmingw32"; then
  AC_DEFINE([HAVE_PTYS],[1],[Define if the system supports pty devices.])
fi

dnl# Everybody supports this, except MS-DOS.
dnl# Seems like the kind of thing we should be testing for, though.
dnl# Compare with HAVE_INET_SOCKETS (which is unused...) above.
AC_DEFINE([HAVE_SOCKETS],[1],
          [Define if the system supports 4.2-compatible sockets.])dnl

AH_TEMPLATE([INTERNAL_TERMINAL],
            [This is substituted when ${TERM} is "internal".])dnl

AH_TEMPLATE([NULL_DEVICE],
            [Name of the file to open to get a null file, or a data sink.])
if test "x${opsys}" = "xmingw32"; then
  AC_DEFINE([NULL_DEVICE],["NUL:"])
else
  AC_DEFINE([NULL_DEVICE],["/dev/null"])
fi

if test "x${opsys}" = "xmingw32"; then
  SEPCHAR=';'
else
  SEPCHAR=':'
fi
AC_DEFINE_UNQUOTED([SEPCHAR],['${SEPCHAR}'],
                   [Character that separates PATH elements.])
dnl# This is for MinGW, and is used in test/automated/Makefile.in.
dnl# The MSYS Bash has heuristics for replacing ':' with ';' when it
dnl# decides that a command-line argument to be passed to a MinGW program
dnl# is a PATH-style list of directories. But that heuristics plays it
dnl# safe, and only does the replacement when it is _absolutely_ sure it
dnl# sees a colon-separated list of file names; e.g. ":." is left alone,
dnl# which breaks in-tree builds. So we do this manually instead.
dnl# Note that we cannot rely on PATH_SEPARATOR, as that one will always
dnl# be computed as ':' in MSYS Bash.
AC_SUBST([SEPCHAR])dnl

dnl# Everybody supports this, except MS-DOS.
AC_DEFINE([subprocesses],[1],
          [Define to 1 to enable asynchronous subprocesses.])dnl

AC_DEFINE([USER_FULL_NAME],[pw->pw_gecos],
          [How to get a user's full name.])dnl

AC_DEFINE([DIRECTORY_SEP],['/'],
  [Character that separates directories in a file name.])dnl

if test "x${opsys}" = "xmingw32"; then
  AC_DEFINE([IS_DEVICE_SEP(_c_)],[((_c_) == ':')],
    [Returns true if character is a device separator.])dnl

  AC_DEFINE([IS_DIRECTORY_SEP(_c_)],[(((_c_) == '/') || ((_c_) == '\\'))],
    [Returns true if character is a directory separator.])dnl

  AC_DEFINE([IS_ANY_SEP(_c_)],
            [(IS_DIRECTORY_SEP (_c_) || IS_DEVICE_SEP(_c_))],
            [Returns true if character is any form of separator.])
else
  AC_DEFINE([IS_DEVICE_SEP(_c_)],[0],
    [Returns true if character is a device separator.])dnl

  AC_DEFINE([IS_DIRECTORY_SEP(_c_)],[((_c_) == DIRECTORY_SEP)],
    [Returns true if character is a directory separator.])dnl

  AC_DEFINE([IS_ANY_SEP(_c_)],[(IS_DIRECTORY_SEP (_c_))],
    [Returns true if character is any form of separator.])
fi

AH_TEMPLATE([NO_EDITRES],[Define if XEditRes should not be used.])dnl

case ${opsys} in
  aix4-2)
    dnl# Unfortunately without libXmu we cannot support EditRes.
    if test "x${ac_cv_lib_Xmu_XmuConvertStandardSelection}" != "xyes"; then
      AC_DEFINE([NO_EDITRES],[1])
    fi
    ;;
  hpux*)
    dnl# Assar Westerlund <assar@sics.se> says this is necessary for
    dnl# HP-UX 10.20, and that it works for HP-UX 0 as well.
    AC_DEFINE([NO_EDITRES],[1])
    ;;
esac

dnl# why not combine this set of case(s) with the previous?
case ${opsys} in
  irix6-5 | sol2* | unixware )
    dnl# Some SVr4s do NOT define NSIG in sys/signal.h for ANSI
    dnl# environments; instead, there is a system variable _sys_nsig.
    dnl# Unfortunately, we need the constant to dimension an array. So
    dnl# wire in the appropriate value here.
    AC_DEFINE([NSIG_MINIMUM],[32],[Minimum value of NSIG.])
    ;;
esac

emacs_broken_SIGIO=no

case ${opsys} in
  dnl# SIGIO exists, but the feature does NOT work in the way Emacs needs.
  dnl# See e.g. <http://article.gmane.org/gmane.os.openbsd.ports/46831>.
  hpux* | irix6-5 | openbsd | sol2* | unixware )
    emacs_broken_SIGIO=yes
    ;;

  aix4-2)
    dnl# On AIX Emacs uses the gmalloc.c malloc implementation. But given
    dnl# the way this system works, libc functions that return malloced
    dnl# memory use the libc malloc implementation. Calling xfree or
    dnl# xrealloc on the results of such functions results in a crash.
    dnl#
    dnl# A solution for this could be to define SYSTEM_MALLOC in configure,
    dnl# but that does not currently work on this system.
    dnl#
    dnl# It is possible to completely override the malloc implementation on
    dnl# AIX, but that involves putting the malloc functions in a shared
    dnl# library & setting the MALLOCTYPE environment variable to point to
    dnl# that shared library.
    dnl#
    dnl# Emacs currently calls xrealloc on results of get_current_dir_name;
    dnl# to avoid a crash just use the Emacs implementation for that func.
    dnl#
    dnl# FIXME: We could change the AC_CHECK_FUNCS call near the start
    dnl# of this file, so that we do not check for get_current_dir_name
    dnl# on AIX. But that might be fragile if something else ends
    dnl# up testing for get_current_dir_name as a dependency.
    AC_DEFINE([BROKEN_GET_CURRENT_DIR_NAME],[1],
              [Define if get_current_dir_name should not be used.])
    ;;

  freebsd)
    dnl# Circumvent a bug in FreeBSD. In the following sequence of
    dnl# writes/reads on a PTY, read(2) returns bogus data:
    dnl#
    dnl# write(2)  1022 bytes
    dnl# write(2)   954 bytes, get EAGAIN
    dnl# read(2)   1024 bytes in process_read_output
    dnl# read(2)     11 bytes in process_read_output
    dnl#
    dnl# That is, read(2) returns more bytes than have ever been written
    dnl# successfully. The 1033 bytes read are the 1022 bytes written
    dnl# successfully after processing (for example with CRs added if the
    dnl# terminal is set up that way which it is here). The same bytes will
    dnl# be seen again in a later read(2), without the CRs.
    AC_DEFINE([BROKEN_PTY_READ_AFTER_EAGAIN],[1],
              [A FreeBSD workaround for a bug when reading from a PTY])
    ;;
esac

case ${opsys} in
  gnu-* | sol2-10 )
    dnl# FIXME: Can we not test if this exists (e.g. /proc/$$)?
    AC_DEFINE([HAVE_PROCFS],[1],[Define if you have the /proc filesystem.])
  ;;
esac

case ${opsys} in
  darwin | dragonfly | freebsd | netbsd | openbsd )
    AC_DEFINE([DONT_REOPEN_PTY],[1],[Define if process.c does not need to
      close a pty to make it a controlling terminal (it is already a
      controlling term of the subproc, because we did ioctl TIOCSCTTY).])
  ;;
esac

dnl# FIXME: Surely we can test for this rather than hard-code it.
dnl# the macro that autoconf has for checking for files triggers warnings
dnl# about cross-compiling though...
case ${opsys} in
  netbsd | openbsd) sound_device="/dev/audio" ;;
  *) sound_device="/dev/dsp" ;;
esac

dnl# Used in sound.c
AC_DEFINE_UNQUOTED([DEFAULT_SOUND_DEVICE],["${sound_device}"],
  [Name of the default sound device.])dnl

dnl########################################################################
dnl# Emacs can read input using SIGIO and buffering characters itself,
dnl# or using CBREAK mode and making C-g cause SIGINT.
dnl# The choice is controlled by the variable interrupt_input.
dnl#
dnl# Define INTERRUPT_INPUT to make interrupt_input = 1 the default
dnl# (use SIGIO)
dnl#
dnl# Emacs uses the presence of the USABLE_SIGIO macro
dnl# to indicate whether or not signal-driven I/O is possible.  It uses
dnl# INTERRUPT_INPUT to decide whether to use it by default.
dnl#
dnl# SIGIO can be used only on systems that implement it (4.2 and 4.3).
dnl# CBREAK mode has two disadvantages:
dnl# 1) At least in 4.2, it is impossible to handle the Meta key properly.
dnl#    I hear that in system V this problem does not exist.
dnl# 2) Control-G causes output to be discarded.
dnl#    I do not know whether this can be fixed in system V.
dnl#
dnl# Another method of doing input is planned but not implemented.
dnl# It would have Emacs fork off a separate process
dnl# to read the input and send it to the true Emacs process
dnl# through a pipe.
case ${opsys} in
  darwin | gnu-linux | gnu-kfreebsd )
    AC_DEFINE([INTERRUPT_INPUT],[1],[Define to read input using SIGIO.])
  ;;
esac

dnl########################################################################
dnl# If the system's imake configuration file defines `NeedWidePrototypes'
dnl# as `NO', we must define NARROWPROTO manually. Such a define is
dnl# generated in the Makefile generated by `xmkmf'. If we do NOT define
dnl# NARROWPROTO, we will see the wrong function prototypes for X functions
dnl# taking float or double parameters.
case ${opsys} in
  cygwin|gnu|gnu-linux|gnu-kfreebsd|irix6-5|freebsd|netbsd|openbsd)
    AC_DEFINE([NARROWPROTO],[1],[Define if system's imake configuration
      file defines `NeedWidePrototypes' as `NO'.])
  ;;
esac

dnl# speaking of prototypes:
dnl# the definition of the following macro has now moved to:
m4_include([m4/prototypes_extra.m4])dnl
AC_REQUIRE_PROTOTYPES

dnl########################################################################
dnl# Used in process.c, this must be a loop, even if it only runs once.
dnl# (Except on SGI; see below. Take that, clarity and consistency!)
AH_TEMPLATE([PTY_ITERATION],[How to iterate over PTYs.])
dnl# Only used if !PTY_ITERATION. Iterate from FIRST_PTY_LETTER to z,
dnl# trying suffixes 0-16.
AH_TEMPLATE([FIRST_PTY_LETTER],[Letter to use in finding device name of
  first PTY, if PTYs are supported.])
AH_TEMPLATE([PTY_OPEN],[How to open a PTY, if non-standard.])
AH_TEMPLATE([PTY_NAME_SPRINTF],[How to get the device name of the control
  end of a PTY, if non-standard.])
AH_TEMPLATE([PTY_TTY_NAME_SPRINTF],[How to get device name of the tty
  end of a PTY, if non-standard.])dnl

case ${opsys} in
  aix4-2 )
    AC_DEFINE([PTY_ITERATION],[int c; for (c = 0; !c ; c++)])
    dnl# You allocate a pty by opening /dev/ptc to get the master side.
    dnl# To get the name of the slave side, you just ttyname() the master
    dnl# side.
    AC_DEFINE([PTY_NAME_SPRINTF],[strcpy(pty_name, "/dev/ptc");])
    AC_DEFINE([PTY_TTY_NAME_SPRINTF],[strcpy(pty_name, ttyname(fd));])
    ;;

  cygwin )
    AC_DEFINE([PTY_ITERATION],[int i; for (i = 0; i < 1; i++)])
    dnl# multi-line AC_DEFINEs are hard. Oh well.
    AC_DEFINE([PTY_OPEN],[ do { int dummy; sigset_t blocked, procmask; sigemptyset(&blocked); sigaddset(&blocked, SIGCHLD); pthread_sigmask(SIG_BLOCK, &blocked, &procmask); if (-1 == openpty(&fd, &dummy, pty_name, 0, 0)) fd = -1; pthread_sigmask(SIG_SETMASK, &procmask, 0); if (fd >= 0) emacs_close(dummy); } while (false)])
    AC_DEFINE([PTY_NAME_SPRINTF],[])
    AC_DEFINE([PTY_TTY_NAME_SPRINTF],[])
    ;;

  dnl# FIXME?  Maybe use same as freebsd - see bug#12040.
  darwin )
    AC_DEFINE([PTY_ITERATION],[int i; for (i = 0; i < 1; i++)])
    dnl# Not used, because PTY_ITERATION is defined.
    AC_DEFINE([FIRST_PTY_LETTER],['p'])
    dnl# Note that openpty may fork via grantpt on Mac OS X 10.4/Darwin 8.
    dnl# But we do NOT have to block SIGCHLD because it is blocked in the
    dnl# implementation of grantpt.
    AC_DEFINE([PTY_OPEN],[ do { int slave; if (openpty(&fd, &slave, pty_name, NULL, NULL) == -1) fd = -1; else emacs_close(slave); } while (false)])
    AC_DEFINE([PTY_NAME_SPRINTF],[])
    AC_DEFINE([PTY_TTY_NAME_SPRINTF],[])
    ;;

  gnu | openbsd )
    AC_DEFINE([FIRST_PTY_LETTER],['p'])
    ;;

  gnu-linux | gnu-kfreebsd | dragonfly | freebsd | netbsd )
    dnl# if HAVE_GRANTPT
    if test "x${ac_cv_func_grantpt}" = "xyes"; then
      AC_DEFINE([UNIX98_PTYS],[1],[Define if the system has Unix98 PTYs.])
      AC_DEFINE([PTY_ITERATION],[int i; for (i = 0; i < 1; i++)])
      dnl# Note that grantpt and unlockpt may fork. We must block SIGCHLD
      dnl# to prevent sigchld_handler from intercepting the child's death.
      AC_DEFINE([PTY_TTY_NAME_SPRINTF],[{ char *ptyname = 0; sigset_t blocked; sigemptyset(&blocked); sigaddset(&blocked, SIGCHLD); pthread_sigmask(SIG_BLOCK, &blocked, 0); if ((grantpt(fd) != -1) && (unlockpt(fd) != -1)) ptyname = ptsname(fd); pthread_sigmask(SIG_UNBLOCK, &blocked, 0); if (!ptyname) { emacs_close(fd); return -1; } snprintf(pty_name, PTY_NAME_SIZE, "%s", ptyname); }])
      dnl# if HAVE_POSIX_OPENPT
      if test "x${ac_cv_func_posix_openpt}" = "xyes"; then
        AC_DEFINE(PTY_OPEN, [do { fd = posix_openpt (O_RDWR | O_CLOEXEC | O_NOCTTY); if (fd < 0 && errno == EINVAL) fd = posix_openpt (O_RDWR | O_NOCTTY); } while (false)])
        AC_DEFINE([PTY_NAME_SPRINTF],[])
      dnl# if HAVE_GETPT
      elif test "x${ac_cv_func_getpt}" = "xyes"; then
        AC_DEFINE([PTY_OPEN],[fd = getpt()])
        AC_DEFINE([PTY_NAME_SPRINTF],[])
      else
        AC_DEFINE([PTY_NAME_SPRINTF],[strcpy(pty_name, "/dev/ptmx");])
      fi
    else
      AC_DEFINE([FIRST_PTY_LETTER],['p'])
    fi
    ;;

  hpux*)
    AC_DEFINE([FIRST_PTY_LETTER],['p'])
    AC_DEFINE([PTY_NAME_SPRINTF],
              [sprintf(pty_name, "/dev/ptym/pty%c%x", c, i);])
    AC_DEFINE([PTY_TTY_NAME_SPRINTF],
              [sprintf(pty_name, "/dev/pty/tty%c%x", c, i);])
    ;;

  irix6-5 )
    dnl# It looks like this cannot be right, because it is not a loop.
    dnl# However, process.c actually does this:
    dnl# # ifndef __sgi
    dnl#   continue;
    dnl# # else
    dnl#   return -1;
    dnl# # endif /* __sgi */
    dnl# which presumably makes it OK, since irix == sgi (?).
    dnl# FIXME: it seems like this special treatment is unnecessary?
    dnl# Why can irix not use a single-trip loop like e.g. cygwin?
    AC_DEFINE([PTY_ITERATION],[])
    dnl# Not used, because PTY_ITERATION is defined.
    AC_DEFINE([FIRST_PTY_LETTER],['q'])
    AC_DEFINE([PTY_OPEN],[ { struct sigaction ocstat, cstat; struct stat stb; char * name; sigemptyset(&cstat.sa_mask); cstat.sa_handler = SIG_DFL; cstat.sa_flags = 0; sigaction(SIGCHLD, &cstat, &ocstat); name = _getpty(&fd, (O_RDWR | O_NDELAY), 0600, 0); sigaction(SIGCHLD, &ocstat, (struct sigaction *)0); if (name == 0) return -1; if (fd < 0) return -1; if (fstat(fd, &stb) < 0) return -1; strcpy(pty_name, name); }])
    dnl# No need to get the pty name at all.
    AC_DEFINE([PTY_NAME_SPRINTF],[])
    dnl# No need to use sprintf to get the tty name--we get that from the
    dnl# _getpty() function.
    AC_DEFINE([PTY_TTY_NAME_SPRINTF],[])
    ;;

  sol2* )
    dnl# On SysVr4, grantpt(3) forks a subprocess, so keep
    dnl# sigchld_handler() from intercepting that death. If any child but
    dnl# grantpt's should die within, it should be caught after
    dnl# sigrelse(2).
    AC_DEFINE([PTY_TTY_NAME_SPRINTF],[{ char *ptsname(int), *ptyname; int grantpt_result; sigset_t blocked; sigemptyset(&blocked); sigaddset(&blocked, SIGCHLD); pthread_sigmask(SIG_BLOCK, &blocked, 0); grantpt_result = grantpt(fd); pthread_sigmask(SIG_UNBLOCK, &blocked, 0); if ((grantpt_result == -1) || (unlockpt(fd) == -1) || !(ptyname = ptsname (fd))) { emacs_close(fd); return -1; } snprintf(pty_name, PTY_NAME_SIZE, "%s", ptyname); }])
    ;;

  unixware )
    dnl# Comments are as per sol2*.
    AC_DEFINE([PTY_TTY_NAME_SPRINTF],[{ char *ptsname(int), *ptyname; int grantpt_result; sigset_t blocked; sigemptyset(&blocked); sigaddset(&blocked, SIGCHLD); pthread_sigmask(SIG_BLOCK, &blocked, 0); grantpt_result = grantpt(fd); pthread_sigmask(SIG_UNBLOCK, &blocked, 0); if (grantpt_result == -1) fatal("could not grant slave pty"); if (unlockpt(fd) == -1) fatal("could not unlock slave pty"); if (!(ptyname = ptsname(fd))) fatal("could not enable slave pty"); snprintf(pty_name, PTY_NAME_SIZE, "%s", ptyname); }])
    ;;
esac

dnl# why not combine this set of case(s) with the previous?
case ${opsys} in
  sol2* | unixware )
    dnl# This change means that we do NOT loop through allocate_pty too
    dnl# many times in the (rare) event of a failure.
    AC_DEFINE([FIRST_PTY_LETTER],['z'])
    AC_DEFINE([PTY_NAME_SPRINTF],[strcpy(pty_name, "/dev/ptmx");])
    dnl# Push various streams modules onto a PTY channel.
    dnl# Used in process.c.
    AC_DEFINE([SETUP_SLAVE_PTY],[if (ioctl(xforkin, I_PUSH, "ptem") == -1) fatal("ioctl I_PUSH ptem"); if (ioctl(xforkin, I_PUSH, "ldterm") == -1) fatal("ioctl I_PUSH ldterm"); if (ioctl(xforkin, I_PUSH, "ttcompat") == -1) fatal("ioctl I_PUSH ttcompat");],[How to set up a slave PTY, if needed.])
    ;;
esac

dnl# another section for signal stuff:
AH_TEMPLATE([SIGNALS_VIA_CHARACTERS],[Make process_send_signal work by
"typing" a signal character on the pty.])dnl

case ${opsys} in
  dnl# Perry Smith <pedz@ddivt1.austin.ibm.com> says this is correct for
  dnl# AIX. thomas@mathematik.uni-bremen.de says this is needed for IRIX.
  aix4-2 | cygwin | gnu | irix6-5 | dragonfly | freebsd | netbsd | openbsd | darwin )
    AC_DEFINE([SIGNALS_VIA_CHARACTERS],[1])
    ;;

  dnl# 21 Jun 06: Eric Hanchrow <offby1@blarg.net> says this works.
  dnl# FIXME Does gnu-kfreebsd have linux/version.h?  It seems unlikely...
  gnu-linux | gnu-kfreebsd )
    dnl# "_ONCE" causes test to jump out of the conditional and be done at
    dnl# wrong location
    AC_CHECK_HEADERS([linux/version.h])
    AC_MSG_CHECKING([for signals via characters])
    AC_PREPROC_IFELSE([AC_LANG_PROGRAM([[
#include <linux/version.h>
#if LINUX_VERSION_CODE < 0x20400
# error "Linux version too old"
#endif /* old Linux */
      ]],[[]])],[emacs_signals_via_chars=yes],
                [emacs_signals_via_chars=no])dnl

    AC_MSG_RESULT([${emacs_signals_via_chars}])
    test "x${emacs_signals_via_chars}" = "xyes" && AC_DEFINE([SIGNALS_VIA_CHARACTERS],[1])
    ;;
esac

dnl########################################################################
dnl# Used in lisp.h, emacs.c, vm-limit.c
dnl# NEWS.18 describes this as "a number which contains
dnl# the high bits to be inclusive or'ed with pointers that are unpacked."
AH_TEMPLATE([DATA_SEG_BITS],[Extra bits to be or'd in with any pointers
stored in a Lisp_Object.])
dnl# if Emacs uses fewer than 32 bits for the value field of a LISP_OBJECT.

case ${opsys} in
  aix*)
    dnl# This works with 32-bit executables; Emacs does NOT support 64-bit.
    AC_DEFINE([DATA_SEG_BITS],[0x20000000])
    ;;
  hpux*)
    dnl# The data segment on this machine always starts at address 0x40000000.
    AC_DEFINE([DATA_SEG_BITS],[0x40000000])
    ;;
  irix6-5)
    AC_DEFINE([DATA_SEG_BITS],[0x10000000])
    ;;
esac

AH_TEMPLATE([TAB3],[Undocumented.])dnl

case ${opsys} in
  darwin) AC_DEFINE([TAB3],[OXTABS]) ;;

  gnu | dragonfly | freebsd | netbsd | openbsd )
    AC_DEFINE([TABDLY],[OXTABS],[Undocumented.])
    AC_DEFINE([TAB3],[OXTABS])
    ;;

  gnu-linux | gnu-kfreebsd )
    AC_PREPROC_IFELSE([AC_LANG_PROGRAM([[
#ifndef __ia64__
# error "not ia64"
#endif /* !__ia64__ */
      ]],[[]])],[AC_DEFINE([GC_MARK_SECONDARY_STACK()],
        [do { extern void *__libc_ia64_register_backing_store_base; __builtin_ia64_flushrs (); mark_memory (__libc_ia64_register_backing_store_base, __builtin_ia64_bsp ());} while (false)],
        [Mark a secondary stack, like the register stack on the ia64.])],
        [])
    ;;

  hpux*)
    AC_DEFINE([RUN_TIME_REMAP],[1],
              [Define if emacs.c needs to call run_time_remap; for HPUX.])
    ;;
esac

dnl########################################################################
dnl# This will NOT be used automatically yet. We also need to know,
dnl# at least, that the stack is continuous.
AH_TEMPLATE([GC_SETJMP_WORKS],[Define if setjmp is known to save all
  registers relevant for conservative garbage collection in a jmp_buf])dnl

dnl# check if we can actually use a define with the autoheader template we
dnl# just declared:
case ${opsys} in
  dnl# Not all the architectures are tested, but there are Debian packages
  dnl# for SCM and/or Guile on them, so the technique must work.  See also
  dnl# comments in alloc.c concerning setjmp and gcc.
  dnl# Fixme: it is probably safe to just use the GCC conditional below.
  gnu-linux | gnu-kfreebsd )
    AC_PREPROC_IFELSE([AC_LANG_PROGRAM([[
#if defined __i386__ || defined __sparc__ || defined __mc68000__ \
  || defined __alpha__ || defined __mips__ || defined __s390__ \
  || defined __arm__ || defined __powerpc__ || defined __amd64__ \
  || defined __ia64__ || defined __sh__
/* ok */
#else
# error "setjmp not known to work on this arch"
#endif /* architecture check */
    ]],[[]])],[AC_DEFINE([GC_SETJMP_WORKS],[1])])
    ;;
esac

dnl# GCC condition:
if test "x${GCC}" = "xyes"; then
   dnl# GC_SETJMP_WORKS is nearly always appropriate for GCC.
   AC_DEFINE([GC_SETJMP_WORKS],[1])
else
  case ${opsys} in
    dnl# irix: Tested on Irix 6.5.  SCM worked on earlier versions.
    dragonfly | freebsd | netbsd | openbsd | irix6-5 | sol2* )
      AC_DEFINE([GC_SETJMP_WORKS],[1])
      ;;
  esac
fi      dnl# GCC?

dnl# In a weird quirk, MS runtime uses _setjmp and longjmp.
dnl# adding "_ONCE" to macro causes test to be done at wrong location:
AC_CHECK_HEADERS([setjmp.h])
AC_CACHE_CHECK([for _setjmp],[emacs_cv_func__setjmp],
  [AC_LINK_IFELSE(
     [AC_LANG_PROGRAM(
       [[#include <setjmp.h>
         #ifdef __MINGW32__
         # define _longjmp longjmp
         #endif /* __MINGW32__ */
       ]],
       [[jmp_buf j;
	 if (! _setjmp (j))
	   _longjmp (j, 1);]])],
     [emacs_cv_func__setjmp=yes],
     [emacs_cv_func__setjmp=no])])
if test "x${emacs_cv_func__setjmp}" = "xyes"; then
  AC_DEFINE([HAVE__SETJMP],[1],[Define to 1 if _setjmp and _longjmp work.])
else
  AC_CACHE_CHECK([for sigsetjmp],[emacs_cv_func_sigsetjmp],
    [AC_LINK_IFELSE(
       [AC_LANG_PROGRAM(
	 [[#include <setjmp.h>
	 ]],
	 [[sigjmp_buf j;
	   if (! sigsetjmp(j, 1))
	     siglongjmp(j, 1);]])],
       [emacs_cv_func_sigsetjmp=yes],
       [emacs_cv_func_sigsetjmp=no])])
  if test "x${emacs_cv_func_sigsetjmp}" = "xyes"; then
    AC_DEFINE([HAVE_SIGSETJMP],[1],
      [Define to 1 if sigsetjmp and siglongjmp work.
       The value of this symbol is irrelevant if HAVE__SETJMP is defined.])
  fi
fi

case ${opsys} in
  sol2* | unixware )
    dnl# TIOCGPGRP is broken in SysVr4, so we cannot send signals to PTY
    dnl# subprocesses the usual way. But TIOCSIGNAL does work for PTYs,
    dnl# and this is all we need.
    AC_DEFINE([TIOCSIGSEND],[TIOCSIGNAL],[Some platforms redefine this.])
    ;;
esac

dnl# why not combine this set of case(s) with the previous?
case ${opsys} in
  hpux* | sol2* )
    dnl# Used in xfaces.c.
    AC_DEFINE([XOS_NEEDS_TIME_H],[1],[Compensate for a bug in Xos.h on
      some systems, where it requires time.h.])
    ;;
esac

dnl########################################################################
dnl# Define symbols to identify the version of Unix this is.
dnl# Define all the symbols that apply correctly.
AH_TEMPLATE([DOS_NT],[Define if the system is MS DOS or MS Windows.])
AH_TEMPLATE([MSDOS],[Define if the system is MS DOS.])
AH_TEMPLATE([USG],[Define if the system is compatible with System III.])
AH_TEMPLATE([USG5_4],
            [Define if system is compatible with System V Release 4])dnl

case ${opsys} in
  aix4-2)
    AC_DEFINE([USG],[])
    dnl# This symbol should be defined on AIX Version 3  ???????
    AC_PREPROC_IFELSE([AC_LANG_PROGRAM([[
#ifndef _AIX
# error "_AIX not defined"
#endif /* !_AIX */
    ]],[[]])],[],[AC_DEFINE([_AIX],[],[Define if the system is AIX.])])
    ;;

  cygwin)
    AC_DEFINE([CYGWIN],[1],[Define if the system is Cygwin.])
    ;;

  darwin)
    dnl# Not __APPLE__, as this may not be defined on non-OSX Darwin (lol).
    dnl# Not DARWIN, because Panther and lower CoreFoundation.h use DARWIN
    dnl# to distinguish OS X from pure Darwin.
    AC_DEFINE([DARWIN_OS],[],[Define if the system is Darwin.])
    ;;

  gnu-linux | gnu-kfreebsd )
    AC_DEFINE([USG],[])
    AC_DEFINE([GNU_LINUX],[],
              [Define if this system is compatible with GNU/Linux.])
    ;;

  hpux*)
    AC_DEFINE([USG],[])
    AC_DEFINE([HPUX],[],[Define if the system is HPUX.])
    ;;

  irix6-5)
    AC_DEFINE([USG],[])
    AC_DEFINE([USG5_4],[])
    AC_DEFINE([IRIX6_5],[],[Define if the system is IRIX.])
    ;;

  mingw32)
    AC_DEFINE([DOS_NT],[])
    AC_DEFINE([WINDOWSNT],[1],[Define if compiling for native MS Windows.])
    if test "x${ac_enable_checking}" != "x"; then
      AC_DEFINE([EMACSDEBUG],[1],
                [Define to 1 to enable w32 debug facilities.])
    fi
    ;;

  sol2*)
    AC_DEFINE([USG],[])
    AC_DEFINE([USG5_4],[])
    AC_DEFINE([SOLARIS2],[],[Define if the system is Solaris.])
    ;;

  unixware)
    AC_DEFINE([USG],[])
    AC_DEFINE([USG5_4],[])
    ;;
esac

dnl# adding "_ONCE" to macro causes test to be done at wrong location:
AC_CHECK_HEADERS([sys/filio.h])
AC_CACHE_CHECK([for usable FIONREAD],[emacs_cv_usable_FIONREAD],
  [case ${opsys} in
     aix4-2)
       dnl# BUILD 9008 - FIONREAD problem still exists in X-Windows.
       emacs_cv_usable_FIONREAD=no
       ;;
     mingw32)
       emacs_cv_usable_FIONREAD=yes
       ;;
     *)
       AC_COMPILE_IFELSE(
	 [AC_LANG_PROGRAM([[#include <sys/types.h>
			    #include <sys/ioctl.h>
			    #ifdef USG5_4
			    # include <sys/filio.h>
			    #endif /* USG5_4 */
			  ]],
			  [[int foo = ioctl(0, FIONREAD, &foo);]])],
	 [emacs_cv_usable_FIONREAD=yes],
	 [emacs_cv_usable_FIONREAD=no])
       ;;
   esac])
if test "x${emacs_cv_usable_FIONREAD}" = "xyes"; then
  AC_DEFINE([USABLE_FIONREAD],[1],[Define to 1 if FIONREAD is usable.])dnl

  if test "x${emacs_broken_SIGIO}" = "xno"; then
    AC_CACHE_CHECK([for usable SIGIO],[emacs_cv_usable_SIGIO],
      [AC_COMPILE_IFELSE(
	 [AC_LANG_PROGRAM([[#include <fcntl.h>
			    #include <signal.h>
			  ]],
			  [[int foo = (SIGIO | F_SETFL | FASYNC);]])],
	 [emacs_cv_usable_SIGIO=yes],
	 [emacs_cv_usable_SIGIO=no])],
      [emacs_cv_usable_SIGIO=yes],
      [emacs_cv_usable_SIGIO=no])
    if test "x${emacs_cv_usable_SIGIO}" = "xyes"; then
      AC_DEFINE([USABLE_SIGIO],[1],[Define to 1 if SIGIO is usable.])
    fi
  fi
fi

dnl# more malloc stuff:
case ${opsys} in
  dnl# Emacs supplies its own malloc, but glib calls posix_memalign,
  dnl# and on Cygwin prior to version 1.7.24 that becomes the
  dnl# Cygwin-supplied posix_memalign. As malloc is not the Cygwin
  dnl# malloc, the Cygwin posix_memalign always returns ENOSYS.  A
  dnl# workaround is to set G_SLICE=always-malloc. This is no longer
  dnl# needed starting with cygwin-1.7.24, and it is no longer
  dnl# effective starting with glib-2.36. */
  cygwin)
    AC_DEFINE([G_SLICE_ALWAYS_MALLOC],[1],[Define to set the
      G_SLICE environment variable to "always-malloc" at startup.])
    ;;

  hpux11)
    dnl# It works to open the pty's tty in the parent (Emacs), then
    dnl# close and reopen it in the child.
    AC_DEFINE([USG_SUBTTY_WORKS],[1],[Define for USG systems where it
      works to open a pty's tty in the parent process, then close and
      reopen it in the child.])
    ;;

  irix6-5)
    AC_DEFINE([PREFER_VSUSP],[1],[Define if process_send_signal should
      use VSUSP instead of VSWTCH.])
    ;;

  sol2-10)
    AC_DEFINE([_STRUCTURED_PROC],[1],[Needed for system_process_attributes
      on Solaris.])
    ;;
esac

# Set up the CFLAGS for real compilation, so we can substitute it.
CFLAGS="${REAL_CFLAGS}"
CPPFLAGS="${REAL_CPPFLAGS}"

## Hack to detect a buggy GCC version.
if test "x${GCC}" = "xyes" \
   && test x"`${CC} --version 2> /dev/null | grep 'gcc.* 4.5.0'`" != x"" \
   && test x"`echo ${CFLAGS} | grep '\-O@<:@23@:>@'`" != x"" \
   && test x"`echo ${CFLAGS} | grep '\-fno-optimize-sibling-calls'`" = x""; then
   AC_MSG_ERROR([GCC 4.5.0 has problems compiling Emacs; see etc/PROBLEMS'])
fi

version="${PACKAGE_VERSION}"

copyright="Copyright (C) 2014 Free Software Foundation, Inc."
copyright_holder="Free Software Foundation, Inc."
AC_DEFINE_UNQUOTED([COPYRIGHT],["${copyright}"],
  [Short copyright string for this version of Emacs.])
AC_DEFINE_UNQUOTED([COPYRIGHT_HOLDER],["${copyright_holder}"],
  [Alias for the Free Software Foundation])
AC_SUBST([copyright])dnl
AC_SUBST([copyright_holder])dnl

dnl### Specify what sort of things we will be editing into Makefile and
dnl### into config.h.
dnl# Use configuration here uncanonicalized to avoid exceeding size limits:
AC_SUBST([version])dnl
AC_SUBST([configuration])dnl
dnl## Unused?
AC_SUBST([canonical])dnl
AC_SUBST([srcdir])dnl
AC_SUBST([prefix])dnl
AC_SUBST([exec_prefix])dnl
AC_SUBST([bindir])dnl
AC_SUBST([datadir])dnl
AC_SUBST([sharedstatedir])dnl
AC_SUBST([libexecdir])dnl
AC_SUBST([mandir])dnl
AC_SUBST([infodir])dnl
AC_SUBST([lispdir])dnl
AC_SUBST([standardlisppath])dnl
AC_SUBST([locallisppath])dnl
AC_SUBST([lisppath])dnl
AC_SUBST([x_default_search_path])dnl
AC_SUBST([etcdir])dnl
AC_SUBST([archlibdir])dnl
AC_SUBST([etcdocdir])dnl
AC_SUBST([bitmapdir])dnl
AC_SUBST([gamedir])dnl
AC_SUBST([gameuser])dnl
dnl## FIXME? Nothing uses @LD_SWITCH_X_SITE@.
dnl## src/Makefile.in did add LD_SWITCH_X_SITE (as a cpp define) to the
dnl## end of LIBX_BASE, but nothing ever set it.
AC_SUBST([LD_SWITCH_X_SITE])dnl
AC_SUBST([C_SWITCH_X_SITE])dnl
AC_SUBST([GNUSTEP_CFLAGS])dnl
AC_SUBST([CFLAGS])dnl
dnl## Used in lwlib/Makefile.in:
AC_SUBST([X_TOOLKIT_TYPE])dnl
AC_SUBST([ns_appdir])dnl
AC_SUBST([ns_appbindir])dnl
AC_SUBST([ns_appresdir])dnl
AC_SUBST([ns_appsrc])dnl
AC_SUBST([GNU_OBJC_CFLAGS])dnl
AC_SUBST([OTHER_FILES])dnl

if test -n "${term_header}"; then
    AC_DEFINE_UNQUOTED([TERM_HEADER],["${term_header}"],
        [Define to the header for the built-in window system.])
fi

AC_DEFINE_UNQUOTED([EMACS_CONFIGURATION],["${canonical}"],
		   [Define to the canonical Emacs configuration name.])
AC_DEFINE_UNQUOTED([EMACS_CONFIG_OPTIONS],["${emacs_config_options}"],
		   [Define to the options passed to configure.])
AH_TEMPLATE([config_opsysfile],[Some platforms that do not use configure
  define this to include extra configuration information.])dnl

case ${opsys} in
  mingw32)
    AC_DEFINE([config_opsysfile],[<ms-w32.h>],[])
    ;;
esac

XMENU_OBJ=""
XOBJ=""
FONT_OBJ=""
if test "x${HAVE_X_WINDOWS}" = "xyes" ; then
  AC_DEFINE([HAVE_X_WINDOWS],[1],
	    [Define to 1 if you want to use the X window system.])
  XMENU_OBJ=xmenu.o
  XOBJ="xterm.o xfns.o xselect.o xrdb.o xsmfns.o xsettings.o"
  FONT_OBJ=xfont.o
  if test "x${HAVE_XFT}" = "xyes"; then
    FONT_OBJ="$FONT_OBJ ftfont.o xftfont.o ftxfont.o"
  elif test "x${HAVE_FREETYPE}" = "xyes"; then
    FONT_OBJ="${FONT_OBJ} ftfont.o ftxfont.o"
  fi
  AC_SUBST([FONT_OBJ])
fi
AC_SUBST([XMENU_OBJ])dnl
AC_SUBST([XOBJ])dnl
AC_SUBST([FONT_OBJ])dnl

WIDGET_OBJ=""
MOTIF_LIBW=""
if test "x${USE_X_TOOLKIT}" != "xnone"; then
  WIDGET_OBJ=widget.o
  AC_DEFINE([USE_X_TOOLKIT],[1],[Define to 1 if using an X toolkit.])
  if test "x${USE_X_TOOLKIT}" = "xLUCID"; then
    AC_DEFINE([USE_LUCID],[1],[Define to 1 if using the Lucid X toolkit.])
  elif test "x${USE_X_TOOLKIT}" = "xMOTIF"; then
    AC_DEFINE([USE_MOTIF],[1],[Define to 1 if using the Motif X toolkit.])
    MOTIF_LIBW='-lXm'
    case "${opsys}" in
      gnu-linux)
        ## Paul Abrahams <abrahams at equinox.shaysnet.com> says this is
        ## needed.
        MOTIF_LIBW="${MOTIF_LIBW} -lXpm"
        ;;

      unixware)
        ## Richard Anthony Ryan <ryanr at ellingtn.ftc.nrcs.usda.gov>
        ## says -lXimp is needed in UNIX_SV ... 4.2 1.1.2.
        MOTIF_LIBW="${MOTIF_LIBW} -lXimp"
        ;;

      aix4-2)
        ## olson@mcs.anl.gov says -li18n is needed by -lXm.
        MOTIF_LIBW="${MOTIF_LIBW} -li18n"
        ;;
    esac
    MOTIF_LIBW="${MOTIF_LIBW} ${LIBXP}"
  fi
fi
AC_SUBST([WIDGET_OBJ])dnl

TOOLKIT_LIBW=""
case "${USE_X_TOOLKIT}" in
  MOTIF) TOOLKIT_LIBW="${MOTIF_LIBW}" ;;
  LUCID) TOOLKIT_LIBW="${LUCID_LIBW}" ;;
  none) test "x${HAVE_GTK}" = "xyes" && export TOOLKIT_LIBW="${GTK_LIBS}"
        ;;
esac
AC_SUBST([TOOLKIT_LIBW])dnl

if test "x${opsys}" != "xmingw32"; then
  if test "x${USE_X_TOOLKIT}" = "xnone"; then
    LIBXT_OTHER="\$(LIBXSM)"
  else
    LIBXT_OTHER="\$(LIBXMU) -lXt \$(LIBXTR6) -lXext"
  fi
fi
AC_SUBST([LIBXT_OTHER])dnl

if test "x${HAVE_X11}" = "xyes" ; then
  AC_DEFINE([HAVE_X11],[1],
	    [Define to 1 if you want to use version 11 of X windows.])
  LIBX_OTHER="\$(LIBXT) \$(LIBX_EXTRA)"
else
  LIBX_OTHER=""
fi
AC_SUBST([LIBX_OTHER])dnl

if test "x${HAVE_GTK}" = "xyes" || test "x${HAVE_X11}" != "xyes"; then
  LIBXMENU=""
elif test "x${USE_X_TOOLKIT}" = "xnone"; then
  LIBXMENU='$(oldXMenudir)/libXMenu11.a'
else
  LIBXMENU='$(lwlibdir)/liblw.a'
fi
AC_SUBST([LIBXMENU])dnl

if test "x${GNU_MALLOC}" = "xyes" ; then
  AC_DEFINE([GNU_MALLOC],[1],
	    [Define to 1 if you want to use the GNU memory allocator.])
fi

RALLOC_OBJ=""
if test "x${REL_ALLOC}" = "xyes"; then
  AC_DEFINE([REL_ALLOC],[1],
	    [Define REL_ALLOC if you want to use the relocating allocator
	     for buffer space.])dnl

  test "x${system_malloc}" != "xyes" && export RALLOC_OBJ=ralloc.o
fi
AC_SUBST([RALLOC_OBJ])dnl

if test "x${opsys}" = "xcygwin"; then
  CYGWIN_OBJ="sheap.o cygw32.o"
  ## Cygwin differs because of its unexec().
  PRE_ALLOC_OBJ=""
  POST_ALLOC_OBJ=lastfile.o
elif test "x${opsys}" = "xmingw32"; then
  CYGWIN_OBJ=""
  PRE_ALLOC_OBJ=""
  POST_ALLOC_OBJ=lastfile.o
else
  CYGWIN_OBJ=""
  PRE_ALLOC_OBJ=lastfile.o
  POST_ALLOC_OBJ=""
fi
AC_SUBST([CYGWIN_OBJ])dnl
AC_SUBST([PRE_ALLOC_OBJ])dnl
AC_SUBST([POST_ALLOC_OBJ])dnl

dnl# Call this 'FORTIFY_SOUR' so that it sorts before the 'FORTIFY_SOURCE'
dnl# verbatim defined above. The tricky name is apropos, as this hack
dnl# makes Fortify go sour!
AH_VERBATIM([FORTIFY_SOUR],[
/* Without the following workaround, Emacs runs slowly on OS X 10.8.
 * The workaround disables some useful run-time checking, so it
 * should be conditional to the platforms with the performance bug.
 * Perhaps Apple will fix this some day; also see m4/extern-inline.m4.  */
#if defined __APPLE__ && defined __GNUC__
# ifndef _DONT_USE_CTYPE_INLINE_
#  define _DONT_USE_CTYPE_INLINE_
# endif /* !_DONT_USE_CTYPE_INLINE_ */
# ifndef _FORTIFY_SOURCE
#  define _FORTIFY_SOURCE 0
# endif /* !_FORTIFY_SOURCE */
#endif /* __APPLE__ && __GNUC__ */
])dnl

dnl# speaking of inline-ing:
AC_C_INLINE

dnl# gettext:
AC_MSG_CHECKING([gettext])
AC_MSG_RESULT([ ])
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.19.2])
AC_CHECK_LIB([intl],[gettext],[:],[
  AC_CHECK_LIB([intl],[libintl_gettext],[:],[
    AC_CHECK_LIB([intl],[main])
  ])
])
AC_SEARCH_LIBS([libintl_gettext],[intl asprintf gettextlib gettextpo \
                                  gettextsrc gettext libintl lintl \
                                  gnu c iberty iconv])dnl

# Configure gnulib. Although this does not affect CFLAGS or LIBS
# permanently, it temporarily reverts them to their pre-pkg-config values,
# because gnulib needs to work with both src (which uses the
# pkg-config stuff) and lib-src (which does not).  For example, gnulib
# may need to determine whether LIB_CLOCK_GETTIME should contain -lrt,
# and it therefore needs to run in an environment where LIBS does not
# already contain -lrt merely because 'pkg-config --libs' printed '-lrt'
# for some package unrelated to lib-src.
AC_MSG_CHECKING([things that gnulib needs])
AC_MSG_RESULT([ ])
SAVE_CFLAGS="${CFLAGS}"
SAVE_LIBS="${LIBS}"
CFLAGS="${pre_PKG_CONFIG_CFLAGS}"
LIBS="${LIB_PTHREAD} ${pre_PKG_CONFIG_LIBS}"
gl_ASSERT_NO_GNULIB_POSIXCHECK
gl_ASSERT_NO_GNULIB_TESTS
gl_INIT
CFLAGS="${SAVE_CFLAGS}"
LIBS="${SAVE_LIBS}"

AC_PATH_PROGS([UNIQ_BIN],[uniq guniq],[])dnl
dnl# m4-quotation block 13 begin (or 14 if you are superstitious):
[
## uniq the stuff in LIBS:
f=""
for i in ${LIBS}; do
  case " ${f} " in
    *" ${i} "*) ;;
    *) f="${f} ${i}" ;;
  esac
done
LIBS="${f}"

if test "x${opsys}" = "xmingw32"; then
  CPPFLAGS="${CPPFLAGS} -DUSE_CRT_DLL=1 -I \${abs_top_srcdir}/nt/inc"
  # Remove unneeded switches from the value of CC that goes to Makefiles
  CC=`echo ${CC} | sed -e "s,${GCC_TEST_OPTIONS},,"`
fi

case "${opsys}" in
  aix4-2) LD_SWITCH_SYSTEM_TEMACS="-Wl,-bnodelcsect" ;;

  darwin)
   ## The -headerpad option tells ld (see man page) to leave room at the
   ## end of the header for adding load commands. Needed for dumping.
   ## 0x690 is the total size of 30 segment load commands (at 56
   ## each); under Cocoa 31 commands are required.
   if test "x${HAVE_NS}" = "xyes"; then
     libs_nsgui="-framework AppKit"
     if test "${NS_IMPL_COCOA}" = "yes"; then
        libs_nsgui="${libs_nsgui} -framework IOKit"
     fi
     headerpad_extra=6C8
   else
     libs_nsgui=""
     headerpad_extra=690
   fi
   LD_SWITCH_SYSTEM_TEMACS="-fno-pie -prebind ${libs_nsgui} -Xlinker -headerpad -Xlinker ${headerpad_extra}"
   ## (why not just use '-headerpad_max_install_names'? Would that not be
   ## more inline with GNU policy of preferring dynamic limits over
   ## arbitrary, hard-coded ones? Or even using 0x1000 like Apple does
   ## would be better, I think...)

   ## Apple uses this in their Makefile, and other platforms use analogous
   ## flags:
   LD_SWITCH_SYSTEM_TEMACS="${LD_SWITCH_SYSTEM_TEMACS} -Wl,-no_pie"

   ## check the version to see if this is ld64; the argument to `tr` should
   ## be fine as long as this stays inside an m4-quotation block:
   LD_IS_LD64="`${LD} -v 2>&1 | grep 'ld64' | tr -d [:blank:]`"
   ## if so, add an ld64-specific flag that Apple uses:
   if test "x${LD_IS_LD64}" != "x"; then
     ## put it in a different variable though, until I find out which
     ## versions it works with:
     test -n "${LD_IS_LD64}" && LD64_SWITCH_SYSTEM_TEMACS="-Wl,-no_function_starts"
     ## (subst it outside of this block)
   else
     test -z "${LD_IS_LD64}" && unset LD64_SWITCH_SYSTEM_TEMACS
   fi

   ## This is here because src/Makefile.in did some extra fiddling around
   ## with LD_SWITCH_SYSTEM. It seems cleaner to put this in
   ## LD_SWITCH_SYSTEM_TEMACS instead,
   test "x${LD_SWITCH_SYSTEM}" = "x" && test "x${GCC}" != "xyes" && \
     LD_SWITCH_SYSTEM_TEMACS="-X ${LD_SWITCH_SYSTEM_TEMACS}"
   ;;

  ## LD_SWITCH_X_SITE_RPATH is a -rpath option saying where to
  ## find X at run-time.
  ## When handled by cpp, this was in LD_SWITCH_SYSTEM.  However, at the
  ## point where configure sourced the s/*.h file, LD_SWITCH_X_SITE_RPATH
  ## had not yet been defined and was expanded to null. Hence
  ## LD_SWITCH_SYSTEM had different values in configure (in ac_link) and
  ## src/Makefile.in. It seems clearer therefore to put this piece in
  ## LD_SWITCH_SYSTEM_TEMACS.
  gnu*) LD_SWITCH_SYSTEM_TEMACS="\$(LD_SWITCH_X_SITE_RPATH)" ;;

  mingw32)
   ## MinGW64 does not prepend an underscore to symbols, so we must
   ## pass a different -entry switch to linker.  FIXME: It is better
   ## to make the entry points the same by changing unexw32.c.
   case "${canonical}" in
     x86_64-*-*) LD_SWITCH_SYSTEM_TEMACS="-Wl,-stack,0x00800000 -Wl,-heap,0x00100000 -Wl,-image-base,0x01000000 -Wl,-entry,__start -Wl,-Map,./temacs.map" ;;
     *) LD_SWITCH_SYSTEM_TEMACS="-Wl,-stack,0x00800000 -Wl,-heap,0x00100000 -Wl,-image-base,0x01000000 -Wl,-entry,__start -Wl,-Map,./temacs.map" ;;
   esac
   ;;

  openbsd) LD_SWITCH_SYSTEM_TEMACS='-nopie' ;;

  *) LD_SWITCH_SYSTEM_TEMACS="" ;;
esac

if test "x${ac_enable_profiling}" != "x"; then
  case ${opsys} in
    *freebsd | gnu-linux) ;;
    *) LD_SWITCH_SYSTEM_TEMACS="${LD_SWITCH_SYSTEM_TEMACS} -pg" ;;
  esac
fi

LD_SWITCH_SYSTEM_TEMACS="${LDFLAGS_NOCOMBRELOC} ${LD_SWITCH_SYSTEM_TEMACS}"
]
dnl# m4-quotation block 13 (or 14) end

AC_SUBST([LD_SWITCH_SYSTEM_TEMACS])dnl
AC_SUBST([LD64_SWITCH_SYSTEM_TEMACS])dnl

## MinGW-specific post-link processing of temacs.
TEMACS_POST_LINK="echo 'post-link processing is unnecessary here.'"
ADDSECTION=""
EMACS_HEAPSIZE=""
if test "${opsys}" = "mingw32"; then
  TEMACS_POST_LINK="\$(MINGW_TEMACS_POST_LINK)"
  ADDSECTION="../nt/addsection\$(EXEEXT)"
  ## Preload heap size of temacs.exe in MB.
  case "${canonical}" in
    x86_64-*-*) EMACS_HEAPSIZE=42 ;;
    *) EMACS_HEAPSIZE=27 ;;
  esac
fi

AC_SUBST([ADDSECTION])dnl
AC_SUBST([TEMACS_POST_LINK])dnl
AC_SUBST([EMACS_HEAPSIZE])dnl

## Common for all window systems:
if test "x${window_system}" != "xnone"; then
  AC_DEFINE([HAVE_WINDOW_SYSTEM],[1],[Define if you have a window system.])
  WINDOW_SYSTEM_OBJ="fontset.o fringe.o image.o"
fi

AC_SUBST([WINDOW_SYSTEM_OBJ])dnl

AH_TOP([
/* src/config.h: GNU Emacs site configuration template file.
 *
 * Copyright (C) 1988, 1993-1994, 1999-2002, 2004-2014
 *  Free Software Foundation, Inc.
 *
 * This file is part of GNU Emacs.
 *
 * GNU Emacs is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * GNU Emacs is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
 */

/* No code in Emacs #includes config.h twice, but some bits of code
 * intended to work with other packages as well (like gmalloc.c)
 * think they can include it as many times as they like.  */
#ifndef EMACS_CONFIG_H
#define EMACS_CONFIG_H
])dnl

AH_BOTTOM([#include <conf_post.h>

#endif /* EMACS_CONFIG_H */

/*
Local Variables:
mode: c
End:
*/
])dnl

dnl# begin final m4-quotation block:
[
#### Report on what we decided to do.
#### Report GTK as a toolkit, even if it does NOT use Xt.
#### It makes printing result more understandable as using GTK sets
#### toolkit_scroll_bars to yes by default.
if test "x${HAVE_GTK}" = "xyes"; then
  USE_X_TOOLKIT="${USE_GTK_TOOLKIT}"
fi

if test ${USE_ACL} -ne 0; then
  acl_summary="yes ${LIB_ACL}"
else
  acl_summary=no
fi

if test "x${LDFLAGS}" = "x"; then
  if test "x${prefix}" != "xNONE"; then
    test -z "${LDFLAGS}" && test -n "${prefix}" && test -d "${prefix}/lib" && export LDFLAGS="-L${prefix}/lib"
  fi
fi

## multi-line call to 'echo'; closing quote is farther down @ end of block:
echo "
Configured for \`${canonical}'.

  Where should the build process find the source code? ${srcdir}
  What compiler should emacs be built with?            ${CC} ${CFLAGS}
  What linker should emacs be built with?              ${LD} ${LDFLAGS}
  Should Emacs use the GNU version of malloc?          ${GNU_MALLOC}${GNU_MALLOC_reason}
  Should Emacs use a relocating allocator for buffers? ${REL_ALLOC}
  Should Emacs use mmap(2) for buffer allocation?      ${use_mmap_for_buffers}
  What window system should Emacs use?                 ${window_system}
  What toolkit should Emacs use?                       ${USE_X_TOOLKIT}"
## see? told ya so.

echo ""
echo "X11 info:"
if test -n "${x_includes}"; then
   echo "  Where do we find X Windows header files? ${x_includes}"
else
   echo "  Where do we find X Windows header files? Standard dirs"
fi
if test -n "${x_libraries}"; then
   echo "  Where do we find X Windows libraries?    ${x_libraries}"
else
   echo "  Where do we find X Windows libraries?    Standard dirs"
fi

echo ""
echo "libraries used:"
## graphics libraries:
echo "  Does Emacs use -lXaw3d?                     ${HAVE_XAW3D}"
echo "  Does Emacs use -lXpm?                       ${HAVE_XPM}"
echo "  Does Emacs use -ljpeg?                      ${HAVE_JPEG}"
echo "  Does Emacs use -ltiff?                      ${HAVE_TIFF}"
echo "  Does Emacs use a gif library?               ${HAVE_GIF} ${LIBGIF}"
echo "  Does Emacs use -lpng?                       ${HAVE_PNG}"
echo "  Does Emacs use -lrsvg-2?                    ${HAVE_RSVG}"
echo "  Does Emacs use imagemagick?                 ${HAVE_IMAGEMAGICK}"
## sound libraries:
echo "  Does Emacs support sound?                   ${HAVE_SOUND}"
## security/configuration/utility libraries:
echo "  Does Emacs use -lgpm?                       ${HAVE_GPM}"
echo "  Does Emacs use -ldbus?                      ${HAVE_DBUS}"
echo "  Does Emacs use -lgconf?                     ${HAVE_GCONF}"
echo "  Does Emacs use GSettings?                   ${HAVE_GSETTINGS}"
echo "  Does Emacs use a file notification library? ${NOTIFY_SUMMARY}"
echo "  Does Emacs use access control lists?        ${acl_summary}"
echo "  Does Emacs use -lselinux?                   ${HAVE_LIBSELINUX}"
echo "  Does Emacs use -lgnutls?                    ${HAVE_GNUTLS}"
echo "  Does Emacs use -lxml2?                      ${HAVE_LIBXML2}"
## font libraries:
echo "  Does Emacs use -lfreetype?                  ${HAVE_FREETYPE}"
echo "  Does Emacs use -lm17n-flt?                  ${HAVE_M17N_FLT}"
echo "  Does Emacs use -lotf?                       ${HAVE_LIBOTF}"
echo "  Does Emacs use -lxft?                       ${HAVE_XFT}"
echo "  Does Emacs directly use zlib?               ${HAVE_ZLIB}"
## scrollbars:
echo "  Does Emacs use toolkit scroll bars?         ${USE_TOOLKIT_SCROLL_BARS}"
echo ""

echo "Special environment variables: "
if test -n "${EMACSDATA}"; then
   echo "  Environment variable EMACSDATA set to: ${EMACSDATA}"
else
   echo "  Environment variable EMACSDATA unset."
fi
if test -n "${EMACSDOC}"; then
   echo "  Environment variable EMACSDOC set to:  ${EMACSDOC}"
else
   echo "  Enivornment variable EMACSDOC unset."
fi

echo ""

if test "x${HAVE_NS}" = "xyes"; then
   echo ""
   echo "You must run \"${MAKE-make} install\" in order to test the built application.
The installed application will go to nextstep/Emacs.app and can be
run or moved from there."
   if test "x${EN_NS_SELF_CONTAINED}" = "xyes"; then
      echo "The application will be fully self-contained."
    else
      echo "The lisp resources for the application will be installed under ${prefix}.
You may need to run \"${MAKE-make} install\" with sudo. The application will fail
to run if these resources are not installed."
   fi
   echo ""
fi
]
dnl# end final m4-quotation block

if test "x${opsys}" = "xcygwin"; then
  case `uname -r` in
    1.5.*) AC_MSG_WARN([[building Emacs on Cygwin 1.5 is not supported.]])
           echo ""
	   ;;
  esac
fi

# Remove any trailing slashes in these variables.
[test "x${prefix}" != "xNONE" &&
  prefix=`echo "${prefix}" | sed 's,\([^/]\)/*$,\1,'`
test "x${exec_prefix}" != "xNONE" &&
  exec_prefix=`echo "${exec_prefix}" | sed 's,\([^/]\)/*$,\1,'`]

if test "x${HAVE_NS}" = "xyes"; then
  if test "x${NS_IMPL_GNUSTEP}" = "xyes"; then
    AC_CONFIG_FILES([nextstep/GNUstep/Emacs.base/Resources/Info-gnustep.plist:nextstep/templates/Info-gnustep.plist.in \
      nextstep/GNUstep/Emacs.base/Resources/Emacs.desktop:nextstep/templates/Emacs.desktop.in])
    ns_check_file=Resources/Info-gnustep.plist
  else
    AC_CONFIG_FILES([nextstep/Cocoa/Emacs.base/Contents/Info.plist:nextstep/templates/Info.plist.in \
      nextstep/Cocoa/Emacs.base/Contents/Resources/English.lproj/InfoPlist.strings:nextstep/templates/InfoPlist.strings.in])
    ns_check_file=Contents/Info.plist
  fi
  AC_SUBST([ns_check_file])
fi

dnl# Obviously there is duplication here wrt ${SUBDIR_MAKEFILES}.
dnl# You _can_ use that variable in AC_CONFIG_FILES, so long as any
dnl# directory using automake (ie lib/) is explicitly listed and not
dnl# "hidden" in a variable (else you get "no `Makefile.am' found for any
dnl# configure output"). This will work, but you get a config.status that
dnl# is not quite right (see:
dnl# http://lists.gnu.org/archive/html/bug-autoconf/2008-08/msg00028.html).
dnl# That does NOT have any obvious consequences for Emacs, but on the
dnl# whole it seems better to just live with the duplication.
SUBDIR_MAKEFILES="lib/Makefile lib-src/Makefile oldXMenu/Makefile doc/emacs/Makefile doc/lispintro/Makefile doc/lispref/Makefile doc/misc/Makefile src/Makefile lisp/Makefile lispintro/Makefile lispref/Makefile leim/Makefile lwlib/Makefile nextstep/Makefile nt/Makefile"

AC_CONFIG_FILES([Makefile lib/Makefile lib-src/Makefile oldXMenu/Makefile \
       doc/emacs/Makefile doc/lispintro/Makefile doc/lispref/Makefile \
       doc/misc/Makefile src/Makefile lisp/Makefile lispintro/Makefile \
       lispref/Makefile leim/Makefile lwlib/Makefile nextstep/Makefile \
       nt/Makefile po/Makevars])dnl

dnl# test/ is not present in release tarfiles.
opt_makefile=test/automated/Makefile

if test -f "${srcdir}/${opt_makefile}.in"; then
  SUBDIR_MAKEFILES="${SUBDIR_MAKEFILES} ${opt_makefile}"
  dnl# Again, it is best not to use a variable. Though you can add
  dnl# ",[],[opt_makefile='${opt_makefile}']" and it should work.
  AC_CONFIG_FILES([test/automated/Makefile])
fi

dnl########################################################################
dnl# The admin/ directory used to be excluded from tarfiles.
if test -d ${srcdir}/admin; then
  SUBDIR_MAKEFILES="${SUBDIR_MAKEFILES} admin/unidata/Makefile admin/grammars/Makefile"
  AC_CONFIG_FILES([admin/unidata/Makefile])
  AC_CONFIG_FILES([admin/grammars/Makefile])
fi                              dnl# -d admin

dnl# this seems kind of sketchy to me, but whatever:
SUBDIR_MAKEFILES_IN=`echo " ${SUBDIR_MAKEFILES}" | sed -e 's| | $(srcdir)/|g' -e 's|Makefile|Makefile.in|g'`

AC_SUBST([SUBDIR_MAKEFILES_IN])dnl

dnl# You might wonder (I did) why epaths.h is generated by running make,
dnl# rather than just letting configure generate it from epaths.in.
dnl# One reason is that the various paths are not fully expanded
dnl# (see above); e.g. gamedir="${prefix}/var/games/emacs".
dnl# Secondly, the GNU Coding standards require that one should be able
dnl# to run `make prefix=/some/where/else' and override the values set
dnl# by configure.  This also explains the `move-if-change' test and
dnl# the use of force in the `epaths-force' rule in Makefile.in.
AC_CONFIG_COMMANDS([src/epaths.h],[
if test "x${opsys}" = "xmingw32"; then
  ${MAKE-make} MAKEFILE_NAME=do-not-make-Makefile epaths-force-w32
else
  ${MAKE-make} MAKEFILE_NAME=do-not-make-Makefile epaths-force
fi
],[GCC="${GCC}"
   CPPFLAGS="${CPPFLAGS}"
   opsys="${opsys}"])dnl

dnl# NB we have to cheat and use the ac_... version because abs_top_srcdir
dnl# is not yet set, sigh.  Or we could use ../${srcdir}/src/.gdbinit,
dnl# or a symlink?
AC_CONFIG_COMMANDS([src/.gdbinit],[
if test ! -f src/.gdbinit && test -f "${srcdir}/src/.gdbinit"; then
  echo "source ${ac_abs_top_srcdir}/src/.gdbinit" > src/.gdbinit
fi
])dnl

AC_OUTPUT

# this is one of the longest configure.ac files that I have seen,
# even after splitting off some of the macros into separate files...

# (5385 lines as of the original writing of this comment...)
# (and overall it has ended up growing longer than that too)
