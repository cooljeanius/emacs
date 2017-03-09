dnl#                                               -*- Autoconf -*-
dnl#  Autoconf script for GNU Emacs
dnl# To rebuild the `configure' script from this, execute the command
dnl# 	autoconf
dnl# in the directory containing this script.
dnl# If you changed any AC_DEFINES, also run autoheader.
dnl#
dnl# Copyright (C) 1994-1996, 1999-2014 Free Software Foundation, Inc.
dnl# Copyright (C) 2014-2016 Eric Gallager
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
dnl# Note this is parsed by (at least) `make-dist` and
dnl# 'lisp/cedet/ede/emacs.el':
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
m4_ifdef([AC_REQUIRE_AUX_FILE],[
  AC_REQUIRE_AUX_FILE([compile])dnl
  AC_REQUIRE_AUX_FILE([depcomp])dnl
  AC_REQUIRE_AUX_FILE([missing])dnl
])dnl
dnl# automake 1.13 and later understand this, making -I m4 unnecessary.
dnl# With older versions this is a no-op:
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
      xcsdkdir="`${XCRUN} --show-sdk-path 2>/dev/null`"
      AC_MSG_NOTICE([xcsdkdir is "${xcsdkdir}"])
      AC_SUBST([xcsdkdir])
    else
      test -n "${MAKE_PROG}" && test -x "${MAKE_PROG}"
      AC_MSG_NOTICE([no need to find xcsdkdir when we already have a working 'make' program (i.e. '${MAKE_PROG}')])
    fi
  fi
else
  if test -z "${MAKE}"; then
    dnl# since we do NOT have xcrun in this case, we can check more names
    dnl# for 'make' than just what xcrun knows about:
    AC_CHECK_PROGS([MAKE_PROG],[make gmake gnumake remake])
  fi
fi

dnl# trunk requires GNU Make, so do NOT test for its individual features:
if test "x${am_cv_make_support_nested_variables}" = "x"; then
  test -z "${am_cv_make_support_nested_variables}" && am_cv_make_support_nested_variables=yes
fi  ##end cache value check
m4_ifdef([AC_PROG_MAKE_SET],[
dnl# trunk would normally override the definition, but I do not feel
dnl# comfortable doing that yet...
test -z "${ac_cv_prog_make_make_set}"
],[
  AC_DEFUN([AC_PROG_MAKE_SET],[
    if test "x${SET_MAKE}" = "x"; then
      ## actually this is redundant:
      SET_MAKE=""
      AC_SUBST([SET_MAKE])dnl
      ## ...but whatever...
    fi  ##end check for SET_MAKE
  ])dnl#end (re)defun of macro
])dnl# end ifdef

dnl# Check for GNU Make and possibly set MAKE before initializing automake:
[emacs_check_gnu_make ()
{
  emacs_makeout=`($1 --version) 2>/dev/null` &&
  case ${emacs_makeout} in
    'GNU Make '3.8[1-9]* | 'GNU Make '3.9[0-9]* | \
    'GNU Make '3.[1-9][0-9][0-9]* | 'GNU Make '[4-9]* | 'GNU Make '[1-9][0-9]* )
       ac_path_MAKE_found=:;;
  esac
}]
AC_CACHE_CHECK([for GNU Make],[emacs_cv_path_gnu_MAKE],
  [ac_path_MAKE_found=false
   if test -n "${MAKE}"; then
     emacs_check_gnu_make "${MAKE}"
     emacs_cv_path_gnu_MAKE="${MAKE}"
   else
     emacs_tried_make=false
     emacs_tried_gmake=false
     emacs_tried_gnumake=false
     AC_PATH_PROGS_FEATURE_CHECK([MAKE],[make gmake gnumake remake],
       [[emacs_check_gnu_make "${ac_path_MAKE}"
	 if ${ac_path_MAKE_found}; then
	   # Use the fully-qualified program name only if the basename
	   # would not resolve to it.
	   if eval \$emacs_tried_$ac_prog; then
	     emacs_cv_path_gnu_MAKE="${ac_path_MAKE}"
	   else
	     emacs_cv_path_gnu_MAKE="${ac_prog}"
	   fi
	 fi
	 eval emacs_tried_$ac_prog=:]])
   fi])
${ac_path_MAKE_found} || {
AC_MSG_WARN([[Building Emacs will require GNU Make, at least version 3.81.
If you have it installed under another name, configure with 'MAKE=...'.
For example, run '$0 MAKE=gnu-make'.]])
}
if test "x${emacs_cv_path_gnu_MAKE}" != "x" && test "x${MAKE}" = "x"; then
  test -z "${MAKE}"
  test -n "${emacs_cv_path_gnu_MAKE}" && MAKE="${emacs_cv_path_gnu_MAKE}"
  export MAKE
else
  test -n "${MAKE}" || test -z "${emacs_cv_path_gnu_MAKE}"
fi

dnl# Automake version is fairly arbitrary, older versions might work too:
AC_MSG_CHECKING([automake things])
AC_MSG_RESULT([ ])
unset am_cv_make_support_nested_variables
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
dnl# FIXME: hesiod support may not be present, so it seems like an error
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

OPTION_DEFAULT_OFF([carbon],
[use Carbon (classic Mac OS and old versions of OS X) windowing system])
AC_ARG_ENABLE([carbon-app],
  [AS_HELP_STRING([[--enable-carbon-app[=DIR]  [DIR=/Applications]]],
                  [specify install directory for Emacs.app on Mac OS X])],
  [carbon_appdir_x=${enableval}])
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

AC_ARG_ENABLE([xasserts],
[AS_HELP_STRING([--enable-xasserts],[compile code with xasserts enabled])],
      [USE_XASSERTS=${enableval}],
      [USE_XASSERTS=no])dnl

dnl# (we use the official automake macro for our maintainer-mode flag)

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

## This might be a 'configure' arg.
AC_ARG_VAR([ACLOCAL_PATH],
           [Where aclocal should look for autoconf macros.
            Only used when regenerating the build system.])dnl
AC_SUBST([ACLOCAL_PATH])dnl

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
       [disable self-contained build of Cocoa app under NeXTstep/darwin])],
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
if test "x${ac_gc_check_cons_list}" != "x"; then
  dnl# this one seems to cause the most slowdown:
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
		 of bugs by turning them into build errors.])],
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

###########################################################################
#### Make srcdir absolute, if it is NOT already. It is important to
#### avoid running the path through pwd unnecessarily, since pwd can
#### give you automounter prefixes, which can go away. We do all this
#### so Emacs can find its files when run uninstalled.
old_srcdir_value=""
if test "x${srcdir}" != "x"; then
  test -n "${srcdir}" && export old_srcdir_value="${srcdir}"
else
  export old_srcdir_value="."
fi
AC_SUBST([old_srcdir_value])dnl
## Make sure CDPATH does NOT affect cd (in case PWD is relative):
unset CDPATH
case "${srcdir}" in
  /* ) ;;
  . )
    ## We may be able to use the ${PWD} environment variable to make this
    ## absolute.  But sometimes PWD is inaccurate.
    ## Note: we used to use ${PWD} at the end instead of `pwd`,
    ## but that tested only for a well-formed and valid PWD,
    ## it did not object when PWD was well-formed and valid but just wrong.
    if test ".${PWD}" != "." && test ".`(cd ${PWD} ; sh -c pwd)`" = ".`pwd`";
    then
      srcdir="${PWD}"
    else
      srcdir="`(cd ${srcdir}; pwd)`"
    fi
  ;;
  *  ) srcdir="`(cd ${srcdir}; pwd)`" ;;
esac

#### Check if the source directory already has a configured system in it.
if test "x`pwd`" != "x`(cd ${srcdir} && pwd)`" \
   && test -f "${srcdir}/src/config.h" ; then
  AC_MSG_WARN([[The directory tree `${srcdir}' is being used
   as a build directory right now; it has been configured in its own
   right.  To configure in another directory as well, you MUST
   use GNU make.  If you do not have GNU make, then you must
   now do `make distclean' in ${srcdir},
   and then run $0 again.]])
fi

### Check for grep before using it:
dnl# The definition of the macro for it has been moved to:
m4_include([m4/grep.m4])dnl
AC_REQUIRE_GREPS

### and also sed, even though we have already used it:
AC_DEFUN([AC_REQUIRE_SED],[
  AC_REQUIRE([AC_PROG_SED])dnl
])dnl# end defun
AC_REQUIRE_SED

#### Given the configuration name, set machfile and opsysfile to the
#### names of the m/*.h and s/*.h files we should use.

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
### Try to keep things at least somewhat alphabetical.

### You would hope that you could choose an m/*.h file pretty much
### based on the machine portion of the configuration name, and an s-
### file based on the operating system portion.  However, it turns out
### that each m/*.h file is pretty manufacturer-specific - for
### example, apollo.h, hp9000s300.h, mega68k, news.h, and tad68k are
### all 68000 machines; mips.h, pmax.h, and news-risc are all MIPS
### machines.  So we basically have to have a special case for each
### configuration name.
###
### As far as handling version numbers on operating systems is
### concerned, make sure things will fail in a fixable way.  If
### /etc/MACHINES does NOT say anything about version numbers, be
### prepared to handle anything reasonably.  If version numbers
### matter, be sure /etc/MACHINES says something about it.
###
### Eric Raymond says we should accept strings like "sysvr4" to mean
### "System V Release 4"; he writes, "The old convention encouraged
### confusion between `system' and `release' levels'."

machine='' opsys='' unported=no unofficially_ported=no
## FIXME: still needs to be used in more places:
opsys_family=''
## This next string ends a sentence, and should end with a period:
unofficial_port_warning_reason='no reason.'
case "${canonical}" in
  ## GNU/Linux and similar ports:
  *-*-linux* )
    opsys=gnu-linux
  ;;

  ## FreeBSD ports:
  *-*-freebsd* )
    opsys=freebsd
    case "${canonical}" in
      alpha*-*-freebsd*)	machine=alpha ;;
      ia64-*-freebsd*)		machine=ia64 ;;
      sparc64-*-freebsd*)	machine=sparc ;;
      powerpc-*-freebsd*)	machine=macppc ;;
      i[3456]86-*-freebsd*)	machine=intel386 ;;
      amd64-*-freebsd*|x86_64-*-freebsd*) machine=amdx86-64 ;;
    esac
    opsys_family=bsd
  ;;

  ## DragonFly ports:
  *-*-dragonfly* )
    opsys=dragonfly
    opsys_family=bsd
  ;;

  ## FreeBSD kernel + glibc based userland:
  *-*-kfreebsd*gnu* )
    opsys=gnu-kfreebsd
    opsys_family=bsd
  ;;

  ## NetBSD ports:
  *-*-netbsd* )
    opsys=netbsd
    if test -f /usr/lib/crti.o; then]
dnl# The close and open brackets here are because this section is quoted --
dnl# see the `changequote' comment above.
      AC_DEFINE([HAVE_CRTIN],[1],
                [Define to 1 if you have the file "/usr/lib/crti.o".])
[   fi

    case "${canonical}" in
      alpha*-*-netbsd*)	machine=alpha ;;
      i[3456]86-*-netbsd*) machine=intel386 ;;
      m68k-*-netbsd*)
			# This is somewhat bogus.
			machine=hp9000s300 ;;
      mips-*-netbsd*)	machine=pmax ;;
      mipsel-*-netbsd*)	machine=pmax ;;
      mipseb-*-netbsd*)	machine=pmax ;;
      ns32k-*-netbsd*)	machine=ns32000 ;;
      powerpc-*-netbsd*) machine=macppc ;;
      sparc*-*-netbsd*)	machine=sparc ;;
      vax-*-netbsd*)	machine=vax ;;
      arm-*-netbsd*)	machine=arm ;;
      x86_64-*-netbsd*)	machine=amdx86-64 ;;
      hppa-*-netbsd*)	machine=hp800 ;;
      shle-*-netbsd*)	machine=sh3el ;;
    esac
    opsys_family=bsd
  ;;

  ## OpenBSD ports:
  *-*-openbsd* | *-*-mirbsd* )
    opsys=openbsd
    case "${canonical}" in
      alpha*-*-openbsd*)	machine=alpha ;;
      arm-*-openbsd*)          machine=arm ;;
      hppa-*-openbsd*)         machine=hp9000s300 ;;
      i386-*-openbsd*)         machine=intel386 ;;
      m68k-*-openbsd*)         machine=hp9000s300 ;;
      m88k-*-openbsd*)         machine=aviion ;;
      mips64-*-openbsd*)       machine=mips64 ;;
      powerpc-*-openbsd*)      machine=macppc ;;
      sh-*-openbsd*)           machine=sh3el ;;
      sparc*-*-openbsd*)       machine=sparc ;;
      vax-*-openbsd*)          machine=vax ;;
      x86_64-*-openbsd*)       machine=amdx86-64 ;;
    esac
    opsys_family=bsd
  ;;

  ## LynxOS ports:
  *-*-lynxos* )
    opsys=lynxos
    case "${canonical}" in
      i[3456]86-*-lynxos*) machine=intel386 ;;
      powerpc-*-lynxos*) machine=powerpc ;;
    esac
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} is old and deprecated."
  ;;

  ## Acorn RISCiX:
  arm-acorn-riscix1.1* )
    machine=acorn
    opsys=riscix1-1
    opsys_family=riscix
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  arm-acorn-riscix1.2* | arm-acorn-riscix )
    ## This name is riscix12 instead of riscix1.2
    ## to avoid a file name conflict on MSDOS.
    machine=acorn
    opsys=riscix12
    opsys_family=riscix
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## BSDI ports:
  *-*-bsdi* )
    opsys=bsdi
    case "${canonical}" in
      i[345]86-*-bsdi*) machine=intel386 ;;
      sparc-*-bsdi*)  machine=sparc ;;
      powerpc-*-bsdi*)  machine=macppc ;;
    esac
    case "${canonical}" in
      *-*-bsd386* | *-*-bsdi1* )	opsys=bsd386 ;;
      *-*-bsdi2.0* )		opsys=bsdos2 ;;
      *-*-bsdi2* )		opsys=bsdos2-1 ;;
      *-*-bsdi3* )		opsys=bsdos3 ;;
      *-*-bsdi[45]* )		opsys=bsdos4 ;;
    esac
    opsys_family=bsd
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} is old and deprecated."
  ;;

  ## Alliant machines.
  ## Strictly speaking, we need the version of the alliant operating
  ## system to choose the right machine file, but currently the
  ## configuration name does NOT tell us enough to choose the right
  ## one; we need to give alliants their own operating system name to
  ## do this right.  When someone cares, they can help us.
  fx80-alliant-* )
    machine=alliant4 opsys=bsd4-2
    opsys_family=bsd
    unofficially_ported=yes
    unofficial_port_warning_reason="it is hard to choose the correct machine file for alliant systems as it does not have its own operating system name and nobody cares about it."
  ;;
  i860-alliant-* )
    machine=alliant-2800 opsys=bsd4-3
    opsys_family=bsd
    unofficially_ported=yes
    unofficial_port_warning_reason="it is hard to choose the correct machine file for alliant systems as it does not have its own operating system name and nobody cares about it."
  ;;

  ## Alpha (DEC) machines.
  alpha*-dec-osf* )
    machine=alpha
    opsys=osf1
    # This is needed to find X11R6.1 libraries for certain tests.
    NON_GCC_LINK_TEST_OPTIONS=-Wl,-rpath,/usr/X11R6/lib
    GCC_LINK_TEST_OPTIONS=-Wl,-rpath,/usr/X11R6/lib
    # NON_GNU_CPP is necessary on 5.0 to avoid mangling src/Makefile
    # due to non-traditional preprocessing with the current compiler
    # defaults.  OSF 4 can also have that compiler version, and there
    # seems always to have been a usable /usr/bin/cpp.
    NON_GNU_CPP=/usr/bin/cpp
    case "${canonical}" in
    alpha*-dec-osf[5-9]*)
      opsys=osf5-0 ;;
    esac
    opsys_family=osf
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  alpha*-*-linux-gnu* )
    machine=alpha
    opsys=gnu-linux
    unofficially_ported=yes
    unofficial_port_warning_reason="this case (${opsys} on ${machine}) was reached in a deprecated way; all gnu-linux machines should have been handled at the top."
  ;;

  arm*-*-linux-gnu* )
    machine=arm
    opsys=gnu-linux
    unofficially_ported=yes
    unofficial_port_warning_reason="this case (${opsys} on ${machine}) was reached in a deprecated way; all gnu-linux machines should have been handled at the top."
  ;;

  ## Altos 3068:
  m68*-altos-sysv* )
    machine=altos
    opsys=usg5-2
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## Amdahl UTS:
  580-amdahl-sysv* )
    machine=amdahl
    opsys=usg5-2-2
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## Apollo, Domain/OS:
  m68*-apollo-* )
    machine=apollo
    opsys=bsd4-3
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## Apple Darwin / Mac OS X:
  *-apple-darwin* )
    case "${canonical}" in
      i[3456]86-* )  machine=intel386 ;;
      powerpc-* )    machine=powermac ;;
      x86_64-* )     machine=amdx86-64 ;;
      mac-apple-darwin* ) machine=mac;;
      *-apple-darwin[0-9].*)
        unofficially_ported=yes
        unofficial_port_warning_reason="upstream has dropped support for ${opsys}, but this fork will still attempt to support it."
        ;;
      * )            unported=yes ;;
    esac
    opsys=darwin
    # It used to be necessary to define CPP as follows to make autoconf
    # work correctly. However, this flag is old and deprecated, so put it
    # in an unused variable:
    CPP_NO_PRECOMP="${CC-cc} -E -no-cpp-precomp"
    ## Use Fink packages if available.
    ## FIXME: find a better way to do this: http://debbugs.gnu.org/11507
    ## How about ony doing it if a Fink-specific environment variable is
    ## present? Like this:
      if test "x${DBUS_FINK_SESSION_BUS_SOCKET}" != "x"; then
        ## force check for Fink in its default prefix, because a Fink in a
        ## non-default prefix may be mis-configured:
        if test -x /sw/bin/fink; then
          echo "Found installation of Fink in its default prefix; checking to see if we should use packages from it..."
          FINK_ARCH="`/sw/bin/fink --version --quiet | grep Distribution | cut -d\, -f3 | tr -d [:blank:]`"
          CANONICAL_ARCH="`echo ${canonical} | cut -d\- -f1`"
          if test "x${FINK_ARCH}" = "x${CANONICAL_ARCH}"; then
            if test -d /sw/include && test -d /sw/lib; then
              echo "configure: deciding to use Fink packages"
              GCC_TEST_OPTIONS="-I/sw/include -L/sw/lib -arch ${FINK_ARCH}"
              CPP_NO_PRECOMP="${CPP_NO_PRECOMP} ${GCC_TEST_OPTIONS}"
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

  ## Non-Apple Darwin, i.e. OpenDarwin/PureDarwin/GNUDarwin/others:
  *-gnu-darwin* | *-pc-darwin* | *-unknown-darwin* )
    case "${canonical}" in
      i[3456]86-* )  machine=intel386 ;;
      powerpc-* )    machine=powermac ;;
      x86_64-* )     machine=amdx86-64 ;;
      mac-gnu-darwin* | mac-unknown-darwin* ) machine=mac;;
      * )            unported=yes ;;
    esac
    opsys=darwin
    unofficially_ported=yes
    if test "x${host_vendor}" != "x"; then 
      test -n "${host_vendor}" && unofficial_port_warning_reason="Emacs expects darwin to be supplied by Apple, but your vendor is ${host_vendor}."
    else
      test -z "${host_vendor}" && unofficial_port_warning_reason="Emacs expects darwin to be supplied by Apple, but we cannot tell who your vendor is."
    fi
  ;;

  ## AT&T 3b2, 3b5, 3b15, 3b20:
  we32k-att-sysv* )
    machine=att3b
    opsys=usg5-2-2
    opsys_family=usg
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## AT&T 3b1 - The Mighty Unix PC!
  m68*-att-sysv* )
    machine=7300
    opsys=usg5-2-2
    opsys_family=usg
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## Bull dpx20:
  rs6000-bull-bosx* )
    machine=ibmrs6000
    opsys=aix3-2
    opsys_family=aix
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## Bull dpx2:
  m68*-bull-sysv3* )
    machine=dpx2
    opsys=usg5-3
    opsys_family=usg
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## Bull sps7:
  m68*-bull-sysv2* )
    machine=sps7
    opsys=usg5-2
    opsys_family=usg
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## CCI 5/32, 6/32 -- see "Tahoe".

  ## Celerity.
  ## I do NOT know what configuration name to use for this; config.sub
  ## does NOT seem to know anything about it.  Hey, Celerity users, get
  ## in touch with us!
  celerity-celerity-bsd* )
    machine=celerity
    opsys=bsd4-2
    opsys_family=bsd
    unofficially_ported=yes
    unofficial_port_warning_reason="no one seems to know anything about Celerity, and no one ever got in touch with us about it."
  ;;

  ## Clipper.
  ## What operating systems does this chip run that Emacs has been
  ## tested on?
  clipper-* )
    machine=clipper
    ## We will use the catch-all code at the bottom to guess the
    ## operating system.
    unofficially_ported=yes
    unofficial_port_warning_reason="${machine} is old and deprecated, and we do not even know what operating systems run on it."
  ;;

  ## Compaq Nonstop:
  mips-compaq-nonstopux* )
    machine=nonstopux
    opsys=nonstopux
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## Convex:
  *-convex-bsd* | *-convex-convexos* )
    machine=convex
    opsys=bsd4-3
    opsys_family=bsd
    ## Prevents spurious white space in makefiles - d.m.cooke@larc.nasa.gov
    NON_GNU_CPP="cc -E -P"
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## Cubix QBx/386:
  i[3456]86-cubix-sysv* )
    machine=intel386
    opsys=usg5-3
    opsys_family=usg
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## Cydra 5:
  cydra*-cydrome-sysv* )
    machine=cydra5
    opsys=usg5-3
    opsys_family=usg
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## Chromium Native Client:
  *-nacl )
    opsys=nacl
  ;;

  ## Cygwin ports:
  *-*-cygwin )
    opsys=cygwin
  ;;

  ## Data General AViiON Machines.
  ## DG changed naming conventions with the release of 5.4.4.10, they
  ## dropped the initial 5.4 but left the intervening R.  Because of the
  ## R this should NOT conflict with older versions of the OS (which I
  ## think were named like dgux4.*). In addition, DG had a "new" AViiONs
  ## series that uses either Motorola M88k or Intel Pentium CPUs.
  m88k-dg-dguxR4.* | m88k-dg-dgux4* )
    machine=aviion
    opsys=dgux4
    opsys_family=dgux
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} is old and deprecated and has confusing naming conventions."
  ;;
  m88k-dg-dgux5.4R3* | m88k-dg-dgux5.4.3* )
    ## This name is dgux5-4-3 instead of dgux5-4r3
    ## to avoid a file name conflict on MSDOS.
    machine=aviion
    opsys=dgux5-4-3
    opsys_family=dgux
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  m88k-dg-dgux5.4R2* | m88k-dg-dgux5.4.2* )
    machine=aviion
    opsys=dgux5-4r2
    opsys_family=dgux
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  m88k-dg-dgux* )
    machine=aviion
    opsys=dgux
    opsys_family=dgux
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## Data General AViiON Intel (x86) Machines
  ## Exists from 5.4.3 ("current" i586-dg-dguxR4.11)
  ## Ehud Karni, 1998-may-30, ehud@unix.simonwiesel.co.il
  i[345]86-dg-dguxR4* )
    machine=aviion-intel
    opsys=dgux4
    opsys_family=dgux
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## DECstations:
  mips-dec-ultrix[0-3].* | mips-dec-ultrix4.0* | mips-dec-bsd4.2* )
    machine=pmax
    opsys=bsd4-2
    opsys_family=bsd
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  mips-dec-ultrix4.[12]* | mips-dec-bsd* )
    machine=pmax
    opsys=bsd4-3
    opsys_family=bsd
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  mips-dec-ultrix* )
    machine=pmax
    opsys=ultrix4-3
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  mips-dec-osf* )
    machine=pmax
    opsys=osf1
    opsys_family=osf
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  mips-dec-mach_bsd4.3* )
    machine=pmax
    opsys=mach-bsd4-3
    opsys_family=bsd
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## Motorola Delta machines:
  m68k-motorola-sysv* | m68000-motorola-sysv* )
    machine=delta
    opsys=usg5-3
    if test -z "`type gnucc | grep 'not found'`"
    then
      if test -s /etc/167config
      then CC="gnucc -m68040"
      else CC="gnucc -m68881"
      fi
    else
      if test -z "`type gcc | grep 'not found'`"
      then CC=gcc
      else CC=cc
      fi
    fi
    opsys_family=usg
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  m88k-motorola-sysv4* )
    # jbotte@bnr.ca says that UNIX_System_V <hostName> 4.0 R40V4.3 m88k
    # mc88110 needs POSIX_SIGNALS and therefore needs usg5-4-2.
    # I hope there are not other 4.0 versions for this machine
    # which really need usg5-4 instead.
    machine=delta88k
    opsys=usg5-4-2
    opsys_family=usg
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} is old and deprecated and caused pain for the maintainers of its port."
  ;;
  m88k-motorola-sysv* | m88k-motorola-m88kbcs* )
    machine=delta88k
    opsys=usg5-3
    opsys_family=usg
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## Dual machines:
  m68*-dual-sysv* )
    machine=dual
    opsys=usg5-2
    opsys_family=usg
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  m68*-dual-uniplus* )
    machine=dual
    opsys=unipl5-2
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## Elxsi 6400:
  elxsi-elxsi-sysv* )
    machine=elxsi
    opsys=usg5-2
    opsys_family=usg
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## Encore machines:
  ns16k-encore-bsd* )
    machine=ns16000
    opsys=umax
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## The GEC 63 - apparently, this port is NOT really finished yet.
  #  I am sure we finished off the last of the machines, though.  -- fx

  ## Gould Power Node and NP1:
  pn-gould-bsd4.2* )
    machine=gould
    opsys=bsd4-2
    opsys_family=bsd
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  pn-gould-bsd4.3* )
    machine=gould
    opsys=bsd4-3
    opsys_family=bsd
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  np1-gould-bsd* )
    machine=gould-np1
    opsys=bsd4-3
    opsys_family=bsd
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## Harris Night Hawk machines running CX/UX (a 5000 looks just like a
  ## 4000 as far as Emacs is concerned).
  m88k-harris-cxux* )
    # Build needs to be different on 7.0 and later releases:
    case "`uname -r`" in
       [56].[0-9] ) machine=nh4000 opsys=cxux ;;
       [7].[0-9] ) machine=nh4000 opsys=cxux7 ;;
    esac
    NON_GNU_CPP="/lib/cpp"
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} is old and deprecated."
  ;;
  ## Harris ecx or gcx running CX/UX (Series 1200, Series 3000):
  m68k-harris-cxux* )
    machine=nh3000
    opsys=cxux
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  ## Harris power pc NightHawk running Power UNIX (Series 6000):
  powerpc-harris-powerunix )
    machine=nh6000
    opsys=powerunix
    NON_GNU_CPP="cc -Xo -E -P"
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  ## SR2001/SR2201 running HI-UX/MPP:
  hppa1.1-hitachi-hiuxmpp* )
    machine=sr2k
    opsys=hiuxmpp
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  hppa1.1-hitachi-hiuxwe2* )
    machine=sr2k
    opsys=hiuxwe2
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  ## Honeywell XPS100:
  xps*-honeywell-sysv* )
    machine=xps100
    opsys=usg5-2
    opsys_family=usg
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## HP 9000 series 200 or 300:
  m68*-hp-bsd* )
    machine=hp9000s300
    opsys=bsd4-3
    opsys_family=bsd
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  ## HP/UX 7, 8, 9, and 10 were supported on these machines.
  m68*-hp-hpux* )
    case "`uname -r`" in
      ## Someone had a system that reported A.B8.05 for this.
      ## I wonder what other possibilities there are.
      *.B8.* ) machine=hp9000s300 opsys=hpux8 ;;
      *.08.* ) machine=hp9000s300 opsys=hpux8 ;;
      *.09.* ) machine=hp9000s300 opsys=hpux9 ;;
      *.1[0-9].* ) machine=hp9000s300 opsys=hpux9shr ;;
      *) machine=hp9000s300 opsys=hpux ;;
    esac
    opsys_family=hpux
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## HP 9000 series 700 and 800, running HP/UX:
  hppa*-hp-hpux7* )
    machine=hp800
    opsys=hpux
    opsys_family=hpux
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  hppa*-hp-hpux8* )
    machine=hp800
    opsys=hpux8
    opsys_family=hpux
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  hppa*-hp-hpux9shr* )
    machine=hp800
    opsys=hpux9shr
    opsys_family=hpux
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  hppa*-hp-hpux9* )
    machine=hp800
    opsys=hpux9
    opsys_family=hpux
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  hppa*-hp-hpux10.2* )
    machine=hp800
    opsys=hpux10-20
    opsys_family=hpux
    ## (this one is official though)
  ;;
  hppa*-hp-hpux10* )
    machine=hp800
    opsys=hpux10
    opsys_family=hpux
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  hppa*-hp-hpux1[1-9]* )
    machine=hp800 opsys=hpux11
    opsys_family=hpux
    CFLAGS="-D_INCLUDE__STDC_A1_SOURCE ${CFLAGS}"
    ## (the other official one in this HP/UX grouping)
  ;;

  hppa*-*-linux-gnu* )
    machine=hp800 opsys=gnu-linux
    unofficially_ported=yes
    unofficial_port_warning_reason="this case (${opsys} on ${machine}) was reached in a deprecated way; all gnu-linux machines should have been handled at the top."
  ;;

  ## HP 9000 series 700 and 800, running HP/UX:
  hppa*-hp-hpux* )
    ## Cross-compilation?  Nah!
    case "`uname -r`" in
      ## Someone had a system that reported A.B8.05 for this.
      ## I wonder what other possibilities there are.
      *.B8.* ) machine=hp800 opsys=hpux8 ;;
      *.08.* ) machine=hp800 opsys=hpux8 ;;
      *.09.* ) machine=hp800 opsys=hpux9 ;;
      *) machine=hp800 opsys=hpux10 ;;
    esac
    opsys_family=hpux
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  hppa*-*-nextstep* )
    machine=hp800
    opsys=nextstep
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## Orion machines:
  orion-orion-bsd* )
    machine=orion
    opsys=bsd4-2
    opsys_family=bsd
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  clipper-orion-bsd* )
    machine=orion105
    opsys=bsd4-2
    opsys_family=bsd
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## IBM machines:
  i[3456]86-ibm-aix1.1* )
    machine=ibmps2-aix
    opsys=usg5-2-2
    opsys_family=usg
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  i[3456]86-ibm-aix1.[23]* | i[3456]86-ibm-aix* )
    machine=ibmps2-aix
    opsys=usg5-3
    opsys_family=usg
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  i370-ibm-aix*)
    machine=ibm370aix
    opsys=usg5-3
    opsys_family=usg
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  s390-*-linux-gnu* )
    machine=ibms390
    opsys=gnu-linux
    unofficially_ported=yes
    unofficially_ported=yes
    unofficial_port_warning_reason="this case (${opsys} on ${machine}) was reached in a deprecated way; all gnu-linux machines should have been handled at the top."
  ;;
  s390x-*-linux-gnu* )
    machine=ibms390x
    opsys=gnu-linux
    unofficially_ported=yes
    unofficial_port_warning_reason="this case (${opsys} on ${machine}) was reached in a deprecated way; all gnu-linux machines should have been handled at the top."
  ;;
  rs6000-ibm-aix3.1* | powerpc-ibm-aix3.1*  )
    machine=ibmrs6000
    opsys=aix3-1
    opsys_family=aix
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  rs6000-ibm-aix3.2.5 | powerpc-ibm-aix3.2.5 )
    machine=ibmrs6000
    opsys=aix3-2-5
    opsys_family=aix
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  rs6000-ibm-aix4.0* | powerpc-ibm-aix4.0*  )
    machine=ibmrs6000
    opsys=aix4
    opsys_family=aix
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  rs6000-ibm-aix4.1* | powerpc-ibm-aix4.1*  )
    machine=ibmrs6000
    opsys=aix4-1
    opsys_family=aix
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  rs6000-ibm-aix4* | powerpc-ibm-aix4*  )
    machine=ibmrs6000
    opsys=aix4-1
    opsys_family=aix
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  rs6000-ibm-aix4.[23]* )
    opsys=aix4-2
    opsys_family=aix
    ## (this IBM machine is still official)
  ;;
  powerpc-ibm-aix4.[23]*  )
    opsys=aix4-2
    opsys_family=aix
    ## (this IBM machine is still official)
  ;;
  rs6000-ibm-aix[56]* )
    opsys=aix4-2
    opsys_family=aix
    ## (this IBM machine is still official)
  ;;
  powerpc-ibm-aix[5-9]* | powerpc-ibm-aix[1-9][0-9]* )
    opsys=aix4-2
    opsys_family=aix
    ## (this IBM machine is still official)
  ;;
  rs6000-ibm-aix* | powerpc-ibm-aix* )
    machine=ibmrs6000
    opsys=aix3-2
    opsys_family=aix
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  romp-ibm-bsd4.3* )
    machine=ibmrt
    opsys=bsd4-3
    opsys_family=bsd
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  romp-ibm-bsd4.2* )
    machine=ibmrt
    opsys=bsd4-2
    opsys_family=bsd
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  romp-ibm-aos4.3* )
    machine=ibmrt
    opsys=bsd4-3
    opsys_family=bsd
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  romp-ibm-aos4.2* )
    machine=ibmrt
    opsys=bsd4-2
    opsys_family=bsd
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  romp-ibm-aos* )
    machine=ibmrt
    opsys=bsd4-3
    opsys_family=bsd
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  romp-ibm-bsd* )
    machine=ibmrt
    opsys=bsd4-3
    opsys_family=bsd
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  romp-ibm-aix* )
    machine=ibmrt-aix
    opsys=usg5-2-2    ## (really? not an "aix" variant?)
    opsys_family=usg
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## Integrated Solutions `Optimum V':
  m68*-isi-bsd4.2* )
    machine=isi-ov
    opsys=bsd4-2
    opsys_family=bsd
  ;;
  m68*-isi-bsd4.3* )
    machine=isi-ov
    opsys=bsd4-3
    opsys_family=bsd
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## Intel 386 machines where we DO care about the manufacturer:
  i[3456]86-intsys-sysv* )
    machine=is386
    opsys=usg5-2-2
    opsys_family=usg
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  ## (Intel 386 machines where we do NOT care are farther down,
  ## near the bottom of this block)

  ## Prime EXL:
  i[3456]86-prime-sysv* )
    machine=i386
    opsys=usg5-3
    opsys_family=usg
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## Sequent Symmetry running Dynix:
  i[3456]86-sequent-bsd* )
    machine=symmetry
    opsys=bsd4-3
    opsys_family=bsd
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## Sequent Symmetry running ptx 4, which is a modified SVR4.
  i[3456]86-sequent-ptx4* | i[3456]86-sequent-sysv4* )
    machine=sequent-ptx
    opsys=ptx4
    opsys_family=ptx
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
    NON_GNU_CPP=/lib/cpp
  ;;

  ## Sequent Symmetry running DYNIX/ptx.
  ## Use the old cpp rather than the newer ANSI one.
  i[3456]86-sequent-ptx* )
    machine=sequent-ptx
    opsys=ptx
    opsys_family=ptx
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated and requires an old cpp."
    NON_GNU_CPP="/lib/cpp"
  ;;

  ## ncr machine running svr4.3.
  i[3456]86-ncr-sysv4.3 )
    machine=ncr386
    opsys=usg5-4-3
    opsys_family=usg
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## Unspecified sysv on an ncr machine defaults to svr4.2.
  ## (Plain usg5-4 does NOT turn on POSIX signals, which we need.)
  i[3456]86-ncr-sysv* )
    machine=ncr386
    opsys=usg5-4-2
    opsys_family=usg
    unofficially_ported=yes
    unofficial_port_warning_reason="Unspecified ${opsys} on ${machine} is old and deprecated and possibly dangerous."
  ;;

  ## Intel Paragon OSF/1:
  i860-intel-osf1* )
    machine=paragon
    opsys=osf1
    opsys_family=osf
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
    NON_GNU_CPP=/usr/mach/lib/cpp
  ;;

  ## Intel 860:
  i860-*-sysv4* )
    machine=i860
    opsys=usg5-4
    opsys_family=usg
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
    NON_GNU_CC="/bin/cc" # I.e., not the one in /usr/ucb/cc.
    NON_GNU_CPP="/usr/ccs/lib/cpp" # cc -E tokenizes macro expansion.
  ;;

  ## Macintosh PowerPC:
  powerpc*-*-linux-gnu* )
    machine=macppc        ## (why not "powermac" like above?)
    opsys=gnu-linux
    unofficially_ported=yes
    unofficial_port_warning_reason="this case (${opsys} on ${machine}) was reached in a deprecated way; all gnu-linux machines should have been handled at the top."
  ;;

  ## Masscomp machines:
  m68*-masscomp-rtu* )
    machine=masscomp
    opsys=rtu
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## Megatest machines:
  m68*-megatest-bsd* )
    machine=mega68
    opsys=bsd4-2
    opsys_family=bsd
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## Workstations sold by MIPS.
  ## This is not necessarily all workstations using the MIPS processor:
  ## Irises are produced by SGI, and DECstations by DEC.

  ## etc/MACHINES lists mips.h and mips4.h as possible machine files,
  ## and usg5-2-2 and bsd4-3 as possible OS files.  The only guidance
  ## it gives for choosing between the alternatives seems to be "Use
  ## -machine=mips4 for RISCOS version 4; use -opsystem=bsd4-3 with
  ## the BSD world."  I will assume that these are instructions for
  ## handling two odd situations, and that every other situation
  ## should use mips.h and usg5-2-2, they being listed first.
  mips-mips-usg* )
    machine=mips4
    ## Fall through to the general code at the bottom to decide on the OS.
    unofficially_ported=yes
    unofficial_port_warning_reason="${machine} is old and deprecated, and we do not even know what operating systems run on it in this case."
  ;;
  mips-mips-riscos4* )
    machine=mips4
    opsys=bsd4-3
    opsys_family=bsd
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
    NON_GNU_CC="cc -systype bsd43"
    NON_GNU_CPP="cc -systype bsd43 -E"
  ;;
  mips-mips-riscos5* )
    machine=mips4
    opsys=riscos5
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
    NON_GNU_CC="cc -systype bsd43"
    NON_GNU_CPP="cc -systype bsd43 -E"
  ;;
  mips-mips-bsd* )
    machine=mips
    opsys=bsd4-3
    opsys_family=bsd
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  mips-mips-* )
    machine=mips
    opsys=usg5-2-2
    opsys_family=usg
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## NeXT:
  m68*-next-* | m68k-*-nextstep* )
    machine=m68k
    opsys=nextstep
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## The complete machine from National Semiconductor:
  ns32k-ns-genix* )
    machine=ns32000
    opsys=usg5-2
    opsys_family=usg
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## NCR machines:
  m68*-ncr-sysv2* | m68*-ncr-sysvr2* )
    machine=tower32
    opsys=usg5-2-2
    opsys_family=usg
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  m68*-ncr-sysv3* | m68*-ncr-sysvr3* )
    machine=tower32v3
    opsys=usg5-3
    opsys_family=usg
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## NEC EWS4800:
  mips-nec-sysv4*)
    machine=ews4800
    opsys=ux4800
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## Nixdorf Targon 31:
  m68*-nixdorf-sysv* )
    machine=targon31
    opsys=usg5-2-2
    opsys_family=usg
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## Nu (TI or LMI):
  m68*-nu-sysv* )
    machine=nu
    opsys=usg5-2
    opsys_family=usg
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## Plexus:
  m68*-plexus-sysv* )
    machine=plexus
    opsys=usg5-2
    opsys_family=usg
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## Pyramid machines.
  ## I do NOT really have any idea what sort of processor the Pyramid has,
  ## so I am assuming it is its own architecture.
  pyramid-pyramid-bsd* )
    machine=pyramid
    opsys=bsd4-2
    opsys_family=bsd
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated, and plus details about its architecture are scarce."
  ;;

  ## Sequent Balance:
  ns32k-sequent-bsd4.2* )
    machine=sequent
    opsys=bsd4-2
    opsys_family=bsd
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  ns32k-sequent-bsd4.3* )
    machine=sequent
    opsys=bsd4-3
    opsys_family=bsd
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## Siemens Nixdorf:
  mips-siemens-sysv* | mips-sni-sysv*)
    machine=mips-siemens
    opsys=usg5-4
    opsys_family=usg
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
    NON_GNU_CC=/usr/ccs/bin/cc
    NON_GNU_CPP=/usr/ccs/lib/cpp
  ;;

  ### Silicon Graphics machines:
  ## Iris 2500 and Iris 2500 Turbo (a.k.a. the Iris 3030):
  m68*-sgi-iris3.5* )
    machine=irist
    opsys=iris3-5
    opsys_family=iri
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  m68*-sgi-iris3.6* | m68*-sgi-iris*)
    machine=irist
    opsys=iris3-6
    opsys_family=iri
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  ## Iris 4D:
  mips-sgi-irix3* )
    machine=iris4d
    opsys=irix3-3
    opsys_family=iri
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  mips-sgi-irix4* )
    machine=iris4d
    opsys=irix4-0
    opsys_family=iri
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  mips-sgi-irix6.5 )
    opsys=irix6-5
    opsys_family=iri
    # Without defining _LANGUAGE_C, things get masked out in the headers
    # so that, for instance, grepping for `free' in stdlib.h fails and
    # AC_HEADER_STD_C fails.   (MIPSPro 7.2.1.2m compilers, Irix 6.5.3m).
    NON_GCC_TEST_OPTIONS="-D_LANGUAGE_C"
    ## (this SGI machine is still official)
  ;;
  mips-sgi-irix6* )
    machine=iris4d
    opsys=irix6-0
    opsys_family=iri
    # It is not clear whether -D_LANGUAGE_C is necessary as it is for 6.5,
    # but presumably it does no harm.
    NON_GNU_CPP="/lib/cpp -D_LANGUAGE_C"
    # -32 probably is NOT necessary in later v.6s -- anyone know which?
    NON_GCC_TEST_OPTIONS=-32
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated, and certain options for it are unclear."
  ;;
  mips-sgi-irix5.[01]* )
    machine=iris4d
    opsys=irix5-0
    opsys_family=iri
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  mips-sgi-irix5* | mips-sgi-irix* )
    machine=iris4d
    opsys=irix5-2
    opsys_family=iri
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## SONY machines:
  m68*-sony-bsd4.2* )
    machine=news
    opsys=bsd4-2
    opsys_family=bsd
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  m68*-sony-bsd4.3* )
    machine=news
    opsys=bsd4-3
    opsys_family=bsd
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  m68*-sony-newsos3* | m68*-sony-news3*)
    machine=news
    opsys=bsd4-3
    opsys_family=bsd
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  mips-sony-bsd* | mips-sony-newsos4* | mips-sony-news4*)
    machine=news-risc
    opsys=bsd4-3
    opsys_family=bsd
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  mips-sony-newsos6* )
    machine=news-r6
    opsys=newsos6
    opsys_family=newsos
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  mips-sony-news* )
    machine=news-risc
    opsys=newsos5
    opsys_family=newsos
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## Stride:
  m68*-stride-sysv* )
    machine=stride
    opsys=usg5-2
    opsys_family=usg
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## Suns:
  sparc-*-linux-gnu* | sparc64-*-linux-gnu* )
    machine=sparc
    opsys=gnu-linux
    unofficially_ported=yes
    unofficial_port_warning_reason="this case (${opsys} on ${machine}) was reached in a deprecated way; all gnu-linux machines should have been handled at the top."
  ;;
  *-auspex-sunos* | *-sun-sunos* | *-sun-bsd* | *-sun-solaris* \
    | i[3456]86-*-solaris2* | i[3456]86-*-sunos5* | powerpc*-*-solaris2* \
    | rs6000-*-solaris2* | x86_64-*-solaris2* | x86_64-*-sunos5*)
    case "${canonical}" in
      m68*-sunos1* )
        machine=sun1
        unofficially_ported=yes
        unofficial_port_warning_reason="${machine} is old and deprecated."
      ;;
      m68*-sunos2* )
        machine=sun2
        unofficially_ported=yes
        unofficial_port_warning_reason="${machine} is old and deprecated."
      ;;
      m68* )
        machine=sun3
        unofficially_ported=yes
        unofficial_port_warning_reason="${machine} is old and deprecated."
      ;;
      i[3456]86-sun-sunos[34]* )
        machine=sun386
        unofficially_ported=yes
        unofficial_port_warning_reason="${machine} is old and deprecated."
      ;;
      i[3456]86-*-* )
        machine=intel386
        ## (still official)
      ;;
      amd64-*-*|x86_64-*-*)
        machine=amdx86-64
        ## (still official)
      ;;
      powerpcle* )
        machine=powerpcle
        unofficially_ported=yes
        unofficial_port_warning_reason="${machine} is old and deprecated."
      ;;
      powerpc* | rs6000* )
        machine=ibmrs6000
        unofficially_ported=yes
        unofficial_port_warning_reason="${machine} is old and deprecated."
      ;;
      sparc* )
        machine=sparc
        ## (still official)
      ;;
      * )		unported=yes ;;
    esac
    case "${canonical}" in
      ## The Sun386 never got past 4.0.
      i[3456]86-*-sunos4  )
		opsys=sunos4-0
		## the "sun" family covers both sun and solaris:
		opsys_family=sun
		unofficially_ported=yes
		unofficial_port_warning_reason="${opsys} is old."
      ;;
      *-sunos4.0*  )
		opsys=sunos4-0
		unofficially_ported=yes
		unofficial_port_warning_reason="${opsys} is old."
      ;;
      *-sunos4.1.[3-9]*noshare )
		## This name is sunos413 instead of sunos4-1-3
		## to avoid a file name conflict on MSDOS.
		opsys=sunos413
		opsys_family=sun
		unofficially_ported=yes
		unofficial_port_warning_reason="${opsys} is old."
		NON_GNU_CPP=/usr/lib/cpp
		NON_GCC_TEST_OPTIONS=-Bstatic
		GCC_TEST_OPTIONS=-static
		;;
      *-sunos4.1.[3-9]* | *-sunos4shr*)
		opsys=sunos4shr
		opsys_family=sun
		unofficially_ported=yes
		unofficial_port_warning_reason="${opsys} is old."
		NON_GNU_CPP=/usr/lib/cpp
		;;
      *-sunos4* | *-sunos )
		opsys=sunos4-1
		opsys_family=sun
		unofficially_ported=yes
		unofficial_port_warning_reason="${opsys} is old."
		NON_GCC_TEST_OPTIONS=-Bstatic
		GCC_TEST_OPTIONS=-static
		;;
      *-sunos5.[1-9][0-9]* | *-solaris2.[1-9][0-9]* )
		opsys=sol2-10
		opsys_family=sun
		emacs_check_sunpro_c=yes
		## (still official)
		;;
      *-sunos5.[1-5]* | *-solaris2.[1-5]* )
		opsys=sol2-10
		opsys_family=sun
		unofficially_ported=yes
		unofficial_port_warning_reason="${opsys} is old."
		;;
      ## Note that Emacs 23.1's NEWS said the following would be dropped.
      *-sunos5.3* | *-solaris2.3* )
		opsys=sol2-3
		opsys_family=sun
		unofficially_ported=yes
		unofficial_port_warning_reason="${opsys} is old."
		NON_GNU_CPP=/usr/ccs/lib/cpp
		;;
      *-sunos5.4* | *-solaris2.4* )
		opsys=sol2-4
		opsys_family=sun
		unofficially_ported=yes
		unofficial_port_warning_reason="${opsys} is old."
		NON_GNU_CPP=/usr/ccs/lib/cpp
		RANLIB="ar -ts"
		;;
      *-sunos5.5* | *-solaris2.5* )
		opsys=sol2-5
		opsys_family=sun
		unofficially_ported=yes
		unofficial_port_warning_reason="${opsys} is old."
		NON_GNU_CPP=/usr/ccs/lib/cpp
		RANLIB="ar -ts"
		;;
      *-sunos5.6* | *-solaris2.6* )
		opsys=sol2-6
		opsys_family=sun
		RANLIB="ar -ts"
		## (still official)
		;;
      ## 5.7 EOL Aug 2008, 5.8 EOL Mar 2012.
      *-sunos5.[7-9]* | *-solaris2.[7-9]* )
		opsys=sol2-6
		opsys_family=sun
		emacs_check_sunpro_c=yes
		## (still official)
		;;
      *-sunos5* | *-solaris* )
		opsys=sol2-6
		opsys_family=sun
## FIXME: make this into a proper fix that checks the compiler type,
## rather than relying on path. Or is /usr/ccs/lib/cpp a bad default now?
		if test "x${CC}" = "x/opt/SUNWspro/bin/cc"; then
		  ## -Xs prevents spurious whitespace.
		  NON_GNU_CPP="/opt/SUNWspro/bin/cc -E -Xs"
		else
		  NON_GNU_CPP=/usr/ccs/lib/cpp
		fi
		;;
      *	  )
		opsys=bsd4-2
		opsys_family=bsd
		unofficially_ported=yes
		unofficial_port_warning_reason="${opsys} is unhandled."
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
    ## (this outer case for Sun machines is still mostly official,
    ## might want to double-check the inner ones, though...)
  ;;
  sparc-*-nextstep* )
    machine=sparc
    opsys=nextstep
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## Tadpole 68k:
  m68*-tadpole-sysv* )
    machine=tad68k
    opsys=usg5-3
    opsys_family=usg
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## Tahoe machines:
  tahoe-tahoe-bsd4.2* )
    machine=tahoe
    opsys=bsd4-2
    opsys_family=bsd
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;
  tahoe-tahoe-bsd4.3* )
    machine=tahoe
    opsys=bsd4-3
    opsys_family=bsd
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## Tandem Integrity S2:
  mips-tandem-sysv* )
    machine=tandem-s2
    opsys=usg5-3
    opsys_family=usg
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## Tektronix XD88:
  m88k-tektronix-sysv3* )
    machine=tekxd88
    opsys=usg5-3
    opsys_family=usg
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## Tektronix 16000 box (6130?):
  ns16k-tektronix-bsd* )
    machine=ns16000
    opsys=bsd4-2
    opsys_family=bsd
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated, and we are unsure about its number."
  ;;
  ## Tektronix 4300.
  ## src/m/tek4300.h hints that this is a m68k machine.
  m68*-tektronix-bsd* )
    machine=tek4300
    opsys=bsd4-3
    opsys_family=bsd
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated, and we can only make guesses about it based on obscure hints."
  ;;

  ## Titan P2 or P3.
  ## We seem to have lost the machine-description file titan.h!
  titan-titan-sysv* )
    machine=titan
    opsys=usg5-3
    opsys_family=usg
    unofficially_ported=yes
    unofficial_port_warning_reason="the machine-description file titan.h is missing."
  ;;

  ## Ustation E30 (SS5E):
  m68*-unisys-uniplus* )
    machine=ustation
    opsystem=unipl5-2
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## Vaxen.
  vax-dec-* )
    machine=vax
    case "${canonical}" in
      *-bsd4.1* )
        opsys=bsd4-1
        opsys_family=bsd
      ;;
      *-bsd4.2* | *-ultrix[0-3].* | *-ultrix4.0* )
        opsys=bsd4-2
        opsys_family=bsd
      ;;
      *-bsd4.3* | *-ultrix* )
        opsys=bsd4-3
        opsys_family=bsd
      ;;
      *-sysv[01]* | *-sysvr[01]* )
        opsys=usg5-0
        opsys_family=usg
      ;;
      *-sysv2* | *-sysvr2* )
        opsys=usg5-2
        opsys_family=usg
      ;;
      *-vms* )
        opsys=vms
      ;;
      * )
        unported=yes
      ;;
    esac
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## Whitechapel MG1:
  ns16k-whitechapel-* )
    machine=mg1
    ## We do NOT know what sort of OS runs on these; we will let the
    ## operating system guessing code below try.
    unofficially_ported=yes
    unofficial_port_warning_reason="${machine} is old and deprecated, and we do not even know what operating systems run on it."
  ;;

  ## Wicat:
  m68*-wicat-sysv* )
    machine=wicat
    opsys=usg5-2
    opsys_family=usg
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## IA-64:
  ia64*-*-linux* )
    machine=ia64
    opsys=gnu-linux
    unofficially_ported=yes
    unofficial_port_warning_reason="this case (${opsys} on ${machine}) was reached in a deprecated way; all gnu-linux machines should have been handled at the top."
  ;;

  ## Intel 386 machines where we do NOT care about the manufacturer.
  ## (why the big separation from the cases where we DO care?)
  i[3456]86-*-* )
    machine=intel386
    case "${canonical}" in
      ## FIXME: additional cases in here...
      *-darwin* )
		opsys=darwin
		machine=mac #For univeral building
		CPP_NO_PRECOMP="${CC-cc} -E -no-cpp-precomp"
		## (still official)
		;;
      *-mingw32 )
		opsys=mingw32
		# MinGW overrides and adds some system headers in nt/inc.
		GCC_TEST_OPTIONS="-I ${srcdir}/nt/inc"
		## (still official)
		;;
      *-sysv4.2uw* )
		opsys=unixware
		## (still official)
                ;;
      *-sysv5uw* )
		opsys=unixware
		## (still official)
                ;;
      *-sysv5OpenUNIX* )
		opsys=unixware
		## (still official)
                ;;
      *-nextstep* )
		opsys=nextstep
		unofficially_ported=yes
		unofficial_port_warning_reason="darwin has by-and-large replaced nextstep."
		;;
      ## Otherwise, we will fall through to the generic opsys code at the
      ## bottom.
    esac
  ;;

  ## m68k Linux-based GNU system:
  m68k-*-linux-gnu* )
    machine=m68k
    opsys=gnu-linux
    unofficially_ported=yes
    unofficial_port_warning_reason="this case (${opsys} on ${machine}) was reached in a deprecated way; all gnu-linux machines should have been handled at the top."
  ;;

  ## Mips Linux-based GNU system:
  mips-*-linux-gnu* | mipsel-*-linux-gnu* )
    machine=mips
    opsys=gnu-linux
    unofficially_ported=yes
    unofficial_port_warning_reason="this case (${opsys} on ${machine}) was reached in a deprecated way; all gnu-linux machines should have been handled at the top."
  ;;

  ## UXP/DS:
  sparc-fujitsu-sysv4* )
    machine=sparc
    opsys=uxpds
    opsys_family=uxp
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
    NON_GNU_CPP=/usr/ccs/lib/cpp
    RANLIB="ar -ts"
  ;;

  ## UXP/V:
  f301-fujitsu-uxpv4.1)
    machine=f301
    opsys=uxpv
    opsys_family=uxp
    unofficially_ported=yes
    unofficial_port_warning_reason="${opsys} on ${machine} is old and deprecated."
  ;;

  ## AMD x86-64 Linux-based GNU system:
  x86_64-*-linux-gnu* )
    machine=amdx86-64
    opsys=gnu-linux
    unofficially_ported=yes
    unofficial_port_warning_reason="this case (${opsys} on ${machine}) was reached in a deprecated way; all gnu-linux machines should have been handled at the top."
  ;;

  # x86-64 MinGW64:
  x86_64-*-* )
    case "${canonical}" in
      *-mingw32 )
		opsys=mingw32
		# MinGW overrides and adds some system headers in nt/inc.
		GCC_TEST_OPTIONS="-I ${srcdir}/nt/inc"
		;;
      *-mingw64 )
		opsys=mingw64
		# MinGW overrides and adds some system headers in nt/inc.
		GCC_TEST_OPTIONS="-I ${srcdir}/nt/inc"
                unofficially_ported=yes
                unofficial_port_warning_reason="The case for MinGW64 was just copied from the one MinGW32 without testing to see if it worked the same."
		;;
      ## Otherwise, we will fall through to the generic opsys code at the
      ## bottom.
    esac
  ;;

  ## Tensilica Xtensa Linux-based GNU system:
  xtensa-*-linux-gnu* )
    machine=xtensa
    opsys=gnu-linux
    unofficially_ported=yes
    unofficial_port_warning_reason="this case (${opsys} on ${machine}) was reached in a deprecated way; all gnu-linux machines should have been handled at the top."
  ;;

  ## ...and there we are!
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
    *-gnu* )
      opsys=gnu
      ## (still official)
    ;;
    *-bsd4.[01] )
        opsys=bsd4-1
        opsys_family=bsd
        unofficially_ported=yes
        unofficial_port_warning_reason="${opsys} is old and deprecated, and was unhandled in the first batch, meaning that we have no idea if this is correct."
    ;;
    *-bsd4.2 )
        opsys=bsd4-2
        opsys_family=bsd
        unofficially_ported=yes
        unofficial_port_warning_reason="${opsys} is old and deprecated, and was unhandled in the first batch, meaning that we have no idea if this is correct."
    ;;
    *-bsd4.3 )
        opsys=bsd4-3
        opsys_family=bsd
        unofficially_ported=yes
        unofficial_port_warning_reason="${opsys} is old and deprecated, and was unhandled in the first batch, meaning that we have no idea if this is correct."
    ;;
    *-sysv0 | *-sysvr0 )
        opsys=usg5-0
        opsys_family=usg
        unofficially_ported=yes
        unofficial_port_warning_reason="${opsys} is old and deprecated, and was unhandled in the first batch, meaning that we have no idea if this is correct."
    ;;
    *-sysv2 | *-sysvr2 )
        opsys=usg5-2
        opsys_family=usg
        unofficially_ported=yes
        unofficial_port_warning_reason="${opsys} is old and deprecated, and was unhandled in the first batch, meaning that we have no idea if this is correct."
    ;;
    *-sysv2.2 | *-sysvr2.2 )
        opsys=usg5-2-2
        opsys_family=usg
        unofficially_ported=yes
        unofficial_port_warning_reason="${opsys} is old and deprecated, and was unhandled in the first batch, meaning that we have no idea if this is correct."
    ;;
    *-sysv3* | *-sysvr3* )
        opsys=usg5-3
        opsys_family=usg
        unofficially_ported=yes
        unofficial_port_warning_reason="${opsys} is old and deprecated, and was unhandled in the first batch, meaning that we have no idea if this is correct."
    ;;
    *-sysv4.2uw* )
        opsys=unixware
        unofficially_ported=yes
        unofficial_port_warning_reason="${opsys} is old and deprecated, and was unhandled in the first batch, meaning that we have no idea if this is correct."
        ;;
    *-sysv5uw* )
        opsys=unixware
        unofficially_ported=yes
        unofficial_port_warning_reason="${opsys} is old and deprecated, and was unhandled in the first batch, meaning that we have no idea if this is correct."
        ;;
    *-sysv5OpenUNIX* )
        opsys=unixware
        unofficially_ported=yes
        unofficial_port_warning_reason="${opsys} is old and deprecated, and was unhandled in the first batch, meaning that we have no idea if this is correct."
        ;;
    *-sysv4.1* | *-sysvr4.1* )
        NON_GNU_CPP=/usr/lib/cpp
        opsys=usg5-4
        opsys_family=usg
        unofficially_ported=yes
        unofficial_port_warning_reason="${opsys} is old and deprecated, and was unhandled in the first batch, meaning that we have no idea if this is correct."
    ;;
    *-sysv4.[2-9]* | *-sysvr4.[2-9]* )
	if test "x${NON_GNU_CPP}" = "x"; then
	  if test -f /usr/ccs/lib/cpp; then
	    NON_GNU_CPP=/usr/ccs/lib/cpp
	  else
	    NON_GNU_CPP=/lib/cpp
	  fi
	fi
	opsys=usg5-4-2
	opsys_family=usg
        unofficially_ported=yes
        unofficial_port_warning_reason="${opsys} is old and deprecated, and was unhandled in the first batch, meaning that we have no idea if this is correct."
    ;;
    *-sysv4* | *-sysvr4* )
        opsys=usg5-4
        opsys_family=usg
        unofficially_ported=yes
        unofficial_port_warning_reason="${opsys} is old and deprecated, and was unhandled in the first batch, meaning that we have no idea if this is correct."
    ;;
    * )
        unported=yes
    ;;
  esac
fi
]
dnl# this batch of m4-quotation (block 1) ends, moving to also use similar
dnl# blocks in other places...

if test "x${unported}" = "xyes"; then
  if test "x${PACKAGE_BUGREPORT}" != "x"; then
    test -n "${PACKAGE_BUGREPORT}"
    AC_MSG_ERROR([Emacs does not support `${canonical}' systems.
If you think it should, please send a report to ${PACKAGE_BUGREPORT}.
Check `etc/MACHINES' for recognized configuration names.])
  else
    test -z "${PACKAGE_BUGREPORT}"
    AC_MSG_ERROR([Emacs does not support `${canonical}' systems.
If you think it should, please send a report to bug-gnu-emacs@gnu.org.
Check `etc/MACHINES' for recognized configuration names.])
  fi
elif test "x${unofficially_ported}" = "xyes"; then
  AC_MSG_WARN([Emacs does not officially support `${canonical}' systems, because: "${unofficial_port_warning_reason}" However, we will not stop you from trying to build it anyways...])
fi

machfile="m/${machine}.h"
opsysfile="s/${opsys}.h"

dnl# end check for ported-ness

#### Choose a compiler.
AC_MSG_CHECKING([compiler])
AC_MSG_RESULT([ ])dnl

test -n "${CC}" && cc_specified=yes
test -n "${LD}" && ld_specified=yes

# Save the value of CFLAGS that the user specified.
SPECIFIED_CFLAGS="${CFLAGS}"

AC_LANG([C])dnl

dnl# Old way of setting GCC=yes looked like this:
if test "x${with_gcc}" != "x"; then
  test -n "${with_gcc}"
  case ${with_gcc} in
    "yes" ) CC="gcc" GCC=yes ;;
    "no"  ) : ${CC=cc} ;;
    * )
  esac
fi

dnl# Suppression of test for C89 has been moved to m4/override.m4, which is
dnl# included below.
test -f m4/override.m4

dnl# This will set GCC=yes if using gcc:
AC_PROG_CC([gcc cc cl clang "${XCRUN} gcc" "${XCRUN} clang"])
if test -n "${XCRUN}"; then
  AC_CHECK_PROGS([AR],[ar "${XCRUN} ar"])
  test -n "${AR}" && export AR
else
  test -z "${AR}"
fi

m4_ifdef([gl_PROG_CC_C99],[
  dnl# Emacs needs C99 or later:
  gl_PROG_CC_C99
])dnl

m4_ifdef([AM_PROG_CC_C_O],[
  if test "x${am_cv_prog_cc_c_o}" = "x" && test "x${lt_cv_prog_compiler_c_o}" = "x"; then
    test -z "${am_cv_prog_cc_c_o}" && test -z "${lt_cv_prog_compiler_c_o}"
    AM_PROG_CC_C_O
  fi
])dnl

dnl# begin m4-quotation block one-and-a-half (split off from no. 2 below):
[
# On Suns, sometimes ${CPP} names a directory.
if test -n "${CPP}" && test -d "${CPP}" && test "x${opsys_family}" = "xsun"; then
  CPP=""
fi

#### Some systems specify a CPP to use unless we are using GCC.
#### Now that we know whether we are using GCC, we can decide whether
#### to use that one.
if test "x${NON_GNU_CPP}" != "x" && test "x${GCC}" != "xyes" && test "x${CPP}" = "x"; then
  test -z "${CPP}" && test -n "${NON_GNU_CPP}" && export CPP="${NON_GNU_CPP}"
fi
]
dnl# end m4-quotation block one-and-a-half

AC_PROG_CPP
AC_PROG_CXX
AC_PROG_CXXCPP
AC_PROG_GCC_TRADITIONAL
AC_PROG_OBJC
AC_PROG_OBJCPP
if test "x${OBJCPPFLAGS}" = "x" && test "x${CPPFLAGS}" != "x"; then
  test -z "${OBJCPPFLAGS}" && test -n "${CPPFLAGS}" && export OBJCPPFLAGS="${CPPFLAGS}"
fi
AC_SUBST([OBJCPPFLAGS])dnl
dnl# This next one is relatively new; I put it in m4/grep.m4, which should
dnl# already be included above:
AC_EGREP_PREPROC_SANITY

dnl# as above with the giant gob of m4-quotation (this is block 2):
[
#### Some systems specify a CC to use unless we are using GCC.
#### Now that we know whether we are using GCC, we can decide whether
#### to use that one.
if test "x${NON_GNU_CC}" != "x" && test "x${GCC}" != "xyes" && test "x${cc_specified}" != "xyes"; then
  test -n "${NON_GNU_CC}" && test -z "${CC}" && export CC="${NON_GNU_CC}"
fi

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
dnl# overrides for some gnulib macros have been moved to:
m4_include([m4/override.m4])dnl

dnl# make sure this is required before it is expanded:
m4_ifdef([gl_DOUBLE_SLASH_ROOT],[
  AC_DEFUN([gl_REQUIRE_DOUBLE_SLASH_ROOT],[
    AC_REQUIRE([gl_DOUBLE_SLASH_ROOT])dnl
  ])dnl
  gl_REQUIRE_DOUBLE_SLASH_ROOT
])dnl

# Initialize gnulib right after choosing the compiler.
dnl# Amongst other things, this sets AR and ARFLAGS.
gl_EARLY
AC_MSG_NOTICE([done initializing gnulib, back to more compiler checks])dnl

dnl# section for debugging flags begins here
AC_ARG_ENABLE([better-debugging],
  [AS_HELP_STRING([--enable-better-debugging],
                  [turn on compiler flags to improve the debugging
                   experience.])],
  [case ${enableval} in
     yes|no) ;;
     *) AC_MSG_ERROR([bad val ${enableval} for better-debugging option]) ;;
   esac
   gl_better_debugging="${enableval}"],
  [gl_better_debugging=no])dnl

AC_PATH_PROGS([GDB_BIN],[gdb ggdb gdb-apple])dnl

## if gdb is present, use gdb-specific debugging options; otherwise proceed
## as this script did previously:
if test "x${GDB_BIN}" != "x" && test -x "${GDB_BIN}"; then
  test -n "${GDB_BIN}" && test -x "${GDB_BIN}"
  if test "x${ac_cv_path_CLANG_ANALYZER}" = "x"; then
    test -z "${ac_cv_path_CLANG_ANALYZER}"
    gthree='-ggdb3'
  else
    test -n "${ac_cv_path_CLANG_ANALYZER}"
    gthree='-g3'
  fi
else
  test -z "${GDB_BIN}"
  gthree='-g3'
fi

dnl# we do this hunk of code 3 times; see the FIXME between this one and
dnl# the next one for more info:
if test "x${ac_test_CFLAGS}" != "xset"; then  
  # It is helpful to have C macros available to GDB, so prefer -g3 to -g
  # if -g3 works and the user does not specify CFLAGS.
  # This test must follow gl_EARLY; otherwise AC_LINK_IFELSE complains.
  case ${CFLAGS} in
    '-g')
      emacs_g3_CFLAGS="${gthree}";;
    '-g -O2')
      ## try to limit the flags tested here; others have been moved below.
      emacs_g3_CFLAGS="${gthree} -O2";;
    '-g -Og')
      emacs_g3_CFLAGS="${gthree} -Og";;
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
    if test "x${opsys}" = "xmingw32" && test "x${GCC}" = "xyes"; then
      CFLAGS="${CFLAGS} -gdwarf-2"
    elif test "x${opsys}" = "xdarwin" && test "x${GCC}" = "xyes" && test "x${host_vendor}" = "xapple" && test "x${GDB_BIN}" != "x"; then
      ## (I would prefer to check '${emacs_cv_apple}' instead of
      ## '${host_vendor}', but that has not been set yet by this point...)
      CFLAGS="${CFLAGS} -gdwarf-2 -gfull -mfix-and-continue"
    fi
  fi

  case ${CFLAGS} in
    *-O*)
      AC_MSG_NOTICE([CFLAGS already contain an optimization option])
      ;;
    *)
      if test "x${GCC}" = "xyes"; then
        # gcc as of 4.8 supports the '-Og' optimization level for improved
        # debugging; no need to conditionalize it on gcc version though,
        # as we can just test it:
        optflag="-Og"
      else
        # No optimization flag was inferred for this non-GCC compiler.
        # Try -O. This is needed for xlc on AIX; see Bug#14258.
        optflag="-O"
      fi
      emacs_save_CFLAGS="${CFLAGS}"
      test -z "${CFLAGS}" || CFLAGS="${CFLAGS} "
      CFLAGS="${CFLAGS}${optflag}"
      AC_CACHE_CHECK([whether ${CC} accepts ${optflag}],
        [emacs_cv_prog_cc_o],
	[AC_LINK_IFELSE([AC_LANG_PROGRAM([[]],[[]])],
	   [emacs_cv_prog_cc_o=yes],
	   [emacs_cv_prog_cc_o=no])])
      if test "x${emacs_cv_prog_cc_o}" != "xyes"; then
	CFLAGS="${emacs_save_CFLAGS}"
      fi
      ;;
  esac
else
  AC_MSG_NOTICE([CFLAGS already set (to '${CFLAGS}'); skipping attempt to add more])
fi

dnl# now do likewise with other languages.
dnl#FIXME: write a macro that allows looping through the languages,
dnl# instead of duplicating the same hunk of code for each of them...

AC_LANG_PUSH([Objective C])
if test "x${ac_test_OBJCFLAGS}" != "xset"; then
  # It is also helpful to have Objective C macros available to GDB,
  # so prefer -g3 to -g if -g3 works & the user does not specify OBJCFLAGS.
  case ${OBJCFLAGS} in
    '-g')
      emacs_g3_OBJCFLAGS="${gthree}";;
    '-g -O2')
      emacs_g3_OBJCFLAGS="${gthree} -O2";;
    '-g -Og')
      emacs_g3_OBJCFLAGS="${gthree} -Og";;
    *)
      emacs_g3_OBJCFLAGS='';;
  esac
  if test -n "${emacs_g3_OBJCFLAGS}"; then
    emacs_save_OBJCFLAGS="${OBJCFLAGS}"
    OBJCFLAGS="${emacs_g3_OBJCFLAGS}"
    AC_CACHE_CHECK([whether ${OBJC} accepts ${emacs_g3_OBJCFLAGS}],
      [emacs_cv_prog_objc_g3],
      [AC_LINK_IFELSE([AC_LANG_PROGRAM([[]],[[]])],
	 [emacs_cv_prog_objc_g3=yes],
	 [emacs_cv_prog_objc_g3=no])])
    if test "x${emacs_cv_prog_objc_g3}" != "xyes"; then
      OBJCFLAGS="${emacs_save_OBJCFLAGS}"
    fi
  fi

  case ${OBJCFLAGS} in
    *-O*)
      AC_MSG_NOTICE([OBJCFLAGS already contain an optimization option])
      ;;
    *)
      # No optimization flag was inferred for this compiler, so try -O.
      emacs_save_OBJCFLAGS="${OBJCFLAGS}"
      test -z "${OBJCFLAGS}" || OBJCFLAGS="${OBJCFLAGS} "
      OBJCFLAGS="${OBJCFLAGS}-O"
      AC_CACHE_CHECK([whether ${OBJC} accepts -O],
        [emacs_cv_prog_objc_o],
	[AC_LINK_IFELSE([AC_LANG_PROGRAM([[]],[[]])],
	   [emacs_cv_prog_objc_o=yes],
	   [emacs_cv_prog_objc_o=no])])
      if test "x${emacs_cv_prog_objc_o}" != "xyes"; then
	OBJCFLAGS="${emacs_save_OBJCFLAGS}"
      fi
      ;;
  esac
else
  AC_MSG_NOTICE([OBJCFLAGS already set (to '${OBJCFLAGS}'); skipping attempt to add more])
fi
AC_LANG_POP

dnl# after Objective C is C++:

AC_LANG_PUSH([C++])
if test "x${ac_test_CXXFLAGS}" != "xset"; then
  # It is also helpful to have C++ macros available to GDB, so prefer
  # -g3 to -g if -g3 works and the user does not specify CXXFLAGS.
  case ${CXXFLAGS} in
    '-g')
      emacs_g3_CXXFLAGS="${gthree}";;
    '-g -O2')
      emacs_g3_CXXFLAGS="${gthree} -O2";;
    '-g -Og')
      emacs_g3_CXXFLAGS="${gthree} -Og";;
    *)
      emacs_g3_CXXFLAGS='';;
  esac
  if test -n "${emacs_g3_CXXFLAGS}"; then
    emacs_save_CXXFLAGS="${CXXFLAGS}"
    CXXFLAGS="${emacs_g3_CXXFLAGS}"
    AC_CACHE_CHECK([whether ${CXX} accepts ${emacs_g3_CXXFLAGS}],
      [emacs_cv_prog_cxx_g3],
      [AC_LINK_IFELSE([AC_LANG_PROGRAM([[]],[[]])],
	 [emacs_cv_prog_cxx_g3=yes],
	 [emacs_cv_prog_cxx_g3=no])])
    if test "x${emacs_cv_prog_cxx_g3}" != "xyes"; then
      CXXFLAGS="${emacs_save_CXXFLAGS}"
    fi
  fi

  case ${CXXFLAGS} in
    *-O*)
      AC_MSG_NOTICE([CXXFLAGS already contain an optimization option])
      ;;
    *)
      # No optimization flag was inferred for this compiler, so try -O.
      emacs_save_CXXFLAGS="${CXXFLAGS}"
      test -z "${CXXFLAGS}" || CXXFLAGS="${CXXFLAGS} "
      CXXFLAGS="${CXXFLAGS}-O"
      AC_CACHE_CHECK([whether ${CXX} accepts -O],
        [emacs_cv_prog_cxx_o],
	[AC_LINK_IFELSE([AC_LANG_PROGRAM([[]],[[]])],
	   [emacs_cv_prog_cxx_o=yes],
	   [emacs_cv_prog_cxx_o=no])])
      if test "x${emacs_cv_prog_cxx_o}" != "xyes"; then
	CXXFLAGS="${emacs_save_CXXFLAGS}"
      fi
      ;;
  esac
else
  AC_MSG_NOTICE([CXXFLAGS already set (to '${CXXFLAGS}'); skipping attempt to add more])
fi
AC_LANG_POP

dnl########################################################################
dnl# warnings section begins here:
PRE_WARNING_TESTING_CFLAGS="${CFLAGS}"
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

dnl# definition of macro for checking version of gcc has been moved to:
m4_include([m4/gcc-version.m4])dnl

dnl# The definitions of the macros for checking compiler ids have moved to:
m4_include([m4/compiler_id.m4])dnl
dnl# Now actually call them:
EMACS_COMPILER_ID_CLANG
EMACS_COMPILER_ID_APPLE
dnl# speaking of clang:
EMACS_CLANG_STATIC_ANALYSIS

dnl# lto is compiler-version dependent:
gl_GCC_VERSION_IFELSE([4],[5],[
  AC_ARG_ENABLE([gcc-link-time-optimization],
  [AS_HELP_STRING([--enable-gcc-link-time-optimization],
                  [build emacs with link-time optimization via gcc.
                   This requires GCC 4.5.0 or later, and conflicts with
                   the clang-equivalent option.
                   It also makes Emacs harder to debug, and when we tried
                   it with GCC 4.9.0 x86-64 it actually made Emacs slower,
                   so it is not recommended for typical use.])],[
  if test "x${enableval}" != "xno"; then
     if test "x${GCC}" = "xyes"; then
        AC_PATH_PROG([GETCONF_PROG],[getconf])
        CPUS="`getconf _NPROCESSORS_ONLN 2>/dev/null`"
        if test "x${CPUS}" != "x"; then
      	   LTO="-flto=${CPUS}"
        else
	   LTO="-flto"
        fi  ##end cpu check
        old_CFLAGS="${CFLAGS}"
        CFLAGS="${CFLAGS} ${LTO}"
        AC_CACHE_CHECK([for link-time optimization support when compiling],
                       [ac_cv_lto_supported],[
           AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]],[[]])],
      	     [ac_cv_lto_supported=yes],[ac_cv_lto_supported=no])
        ])dnl# end compile cache check
        ## reset:
        CFLAGS="${old_CFLAGS}"
        ## do it again with the linker:
        old_LDLAGS="${LDFLAGS}"
        LDFLAGS="${LDFLAGS} ${LTO}"
        AC_CACHE_CHECK([for link-time optimization support when linking],
                       [ac_cv_ld_lto_supported],[
           AC_LINK_IFELSE([AC_LANG_PROGRAM([[]],[[]])],
      	     [ac_cv_ld_lto_supported=yes],[ac_cv_ld_lto_supported=no])
        ])dnl# end compile cache check
        ## reset:
        LDFLAGS="${old_LDFLAGS}"
        AC_SUBST([old_LDFLAGS])
     else
        AC_MSG_WARN([link-time optimization requested, but it currently requires a GCC-compatible compiler, which we do not have])
     fi  ##end gcc-specific checks
     ## now use it what we found, but only once:
     if test "x${ac_cv_lto_supported}" = "xyes"; then
        CFLAGS="${CFLAGS} ${LTO}"
        gl_GCC_VERSION_IFELSE([4],[9],[
          dnl# The following is needed for GCC 4.9.0. 
          dnl# The GCC 4.9.0 release notes suggest that instead of
          dnl# -ffat-lto-objects we should use gcc-ar and gcc-ranlib
          dnl# in place of ar and ranlib, but gcc-ar makes /usr/bin/ar
          dnl# dump core on Fedora 20, so play it safe for now:
          gl_COMPILER_OPTION_IF([-ffat-lto-objects],
            [CFLAGS="${CFLAGS} -ffat-lto-objects"])dnl# end check for flag
        ])dnl# end code for gcc4.9+
        ##end case for compiling with gcc lto
     elif test "x${ac_cv_ld_lto_supported}" = "xyes"; then
        LDFLAGS="${LDFLAGS} ${LTO}"
     fi
  else
     AC_MSG_NOTICE([skipping checks for link-time optimization via gcc])
  fi  ##end enableval check
  ],[
  if test "x${enableval}" = "xno" || test "x${enableval}" = "x"; then
     AC_MSG_NOTICE([disabling checks for gcc link-time optimization])
  fi  ##end enableval check
  ])dnl# end arg enable
],[
  AC_ARG_ENABLE([clang-link-time-optimization],
  [AS_HELP_STRING([--enable-clang-link-time-optimization],
                  [build emacs with link-time optimization via clang.
                   This requires clang, and will only be tested if the
                   gcc-equivalent option falls through to it.
                   (Note that clang support is experimental; check INSTALL)
                   It also makes Emacs harder to debug, and could actually
                   make Emacs slower, so it is not recommended for typical
                   use cases.])],[
  if test "x${enableval}" != "xno"; then
    ac_cv_lto_supported=no
    if test "x${emacs_cv_clang}" = "xyes"; then
      AC_MSG_CHECKING([whether link-time optimization is supported by clang])
      GOLD_PLUGIN=`${CC} -print-file-name=LLVMgold.so 2>/dev/null`
      if test -x "${GOLD_PLUGIN}"; then
         LTO="-flto"
      fi
      if test -z "${LTO}"; then
         ac_cv_lto_supported=no
      else
         old_CFLAGS="${CFLAGS}"
         CFLAGS="${CFLAGS} ${LTO}"
         AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]],[[]])],
            [ac_cv_lto_supported=yes],[ac_cv_lto_supported=no])
         CFLAGS="${old_CFLAGS}"
      fi
      AC_MSG_RESULT([${ac_cv_lto_supported}])
      if test "x${ac_cv_lto_supported}" = "xyes"; then
         CFLAGS="${CFLAGS} ${LTO}"
         ## (already in the clang case here)
         AC_MSG_WARN([Please check INSTALL before using link-time optimization with clang])
         # WARNING: 'ar --plugin ...' does NOT work without
         # command, so plugin name is appended to ARFLAGS.
         ARFLAGS="cru --plugin ${GOLD_PLUGIN}"
         RANLIB="${RANLIB} --plugin ${GOLD_PLUGIN}"
      fi
    else
      AC_MSG_NOTICE([need clang for LTO checks, when not using gcc 4.5.0 or newer.])
    fi  ##end check for clang
  else
     AC_MSG_NOTICE([skipping checks for link-time optimization via clang, and we already skipped the ones for gcc, too...])
  fi  ##end enableval check
  ],[
  if test "x${enableval}" = "xno" || test "x${enableval}" = "x"; then
     AC_MSG_NOTICE([would fall through to checks for clang link-time optimization, but they are disabled.])
  fi  ##end enableval check
  ])dnl# end arg enable
])dnl# end code for gcc4.5+

dnl# (compiler id checks moved above the lto checks)

# need this to properly check clang warning flags, and maybe also those
# for apple-gcc, and need to do it whether we have enabled gcc warnings
# or not, because we still test warning flags either way:
if test "x${emacs_cv_clang}" = "xyes" || test "x${emacs_cv_apple}" = "xyes"; then
  gl_UNKNOWN_WARNINGS_ARE_ERRORS
  CFLAGS="${CFLAGS} ${gl_unknown_warnings_are_errors}"
else
  AC_MSG_NOTICE([Hopefully your compiler should reject unknown warning flags without any extra options])
fi

# When compiling with GCC, prefer -isystem to -I when including system
# include files, to avoid generating useless diagnostics for the files.
## I am adding pseudo-"endif labels" to the shell conditional statements
## in this section, to try to debug one of them failing to be closed
## properly...
if test "x${gl_gcc_warnings}" != "xyes"; then
  isystem='-I'
  if test "x${emacs_cv_clang}" = "xyes"; then
     AC_MSG_NOTICE([Disabling some of the excessive warnings that clang produces])
     # Turn off some warnings if supported.
     gl_WARN_ADD([-Wno-switch])
     gl_WARN_ADD([-Wno-tautological-constant-out-of-range-compare])
     gl_WARN_ADD([-Wno-pointer-sign])
     gl_WARN_ADD([-Wno-string-plus-int])
     gl_WARN_ADD([-Wno-unknown-attributes])
  elif test "x${emacs_cv_apple}" = "xyes"; then
     gl_WARN_ADD([-Wno-pointer-sign])
  fi  ##end check for clang or apple compiler
else
  AC_MSG_CHECKING([warning flags])
  AC_MSG_RESULT([ ])

  isystem='-isystem '

  # This, ${nw}, is the list of warnings we disable.
  nw=""

  case ${with_x_toolkit} in
    lucid | athena | motif)
       # Old toolkits mishandle 'const'.
       AC_C_CONST
       if test "x${ac_cv_c_const}" = "xno" || test "x${ac_cv_c_const}" != "xyes"; then
         test -n "${ac_cv_c_const}"
         nw="${nw} -Wsuggest-attribute=const"
         nw="${nw} -Wwrite-strings"
         ##FIXME: this used to be done unconditionally; adding the
         ## condition seems to have introduced new warnings...
       else
         AC_MSG_WARN([did not expect 'const' to work with the ${with_x_toolkit} toolkit; this may result in some extra warnings...])
       fi
       gl_GCC_VERSION_IFELSE([5],[0],[
         nw="${nw} -Wdiscarded-qualifiers"
         gl_WARN_ADD([-Wno-discarded-qualifiers])
       ])
       ;;
    gtk*)
       AC_MSG_NOTICE([no special warning flags for using gtk as a toolkit])
       ;;
    no)
       AC_MSG_NOTICE([not using -Werror without an X toolkit])
       nw="${nw} -Werror"
       ;;
    yes)
       test -n "${WERROR_CFLAGS}" || test -z "${WERROR_CFLAGS}"
       ##FIXME: decide what do do here...
       ;;
    *)
       AC_MSG_WARN([X toolkit unspecified, assuming safe to add -Werror])
       gl_WARN_ADD([-Werror],[WERROR_CFLAGS])
       ;;
  esac
  AC_SUBST([WERROR_CFLAGS])dnl

  ## Carbon is deprecated, so naturally it will provoke lots of these:
  if test "x${HAVE_CARBON}" = "xyes" || test "x${with_carbon}" != "xno"; then
    nw="${nw} -Wdeprecated-declarations"
    gl_WARN_ADD([-Wno-deprecated-declarations])
  fi

  dnl# m4-quoted block number 3 begins:
  [
  nw="${nw} -W"                                    # redundant with -Wextra
  nw="${nw} -Wsystem-headers"  # Do NOT let system headers trigger warnings
  nw="${nw} -Wlogical-op"                 # any use of fwrite provokes this
  nw="${nw} -Wformat-nonliteral"                         # we do this a lot
  nw="${nw} -Wvla"       # warnings in "gettext.h", and Emacs uses <vla.h>.
  nw="${nw} -Wnested-externs"          # use of XARGMATCH/verify_function__
  nw="${nw} -Winline"                               # OK to ignore 'inline'
  # I am leaving "-Wjump-misses-init" on; I am distrustful of the assertion
  # that we sometimes safely jump over init.
  nw="${nw} -Wstrict-overflow"       # use it with a specific value instead
  nw="${nw} -Wsync-nand"       # irrelevant here, and provokes ObjC warning
  # I am leaving "-Wunsafe-loop-optimizations" on; the warning is not so
  # much about the unsafe optimizations being stopped, as it is about the
  # loops possibly going infinite unintentionally.
  if test "x${I_AM_GOOD_AT_SILENCING_WARNINGS}" != "xyes"; then
    nw="${nw} -Wbad-function-cast"  # These casts are no worse than others.
  fi
  nw="${nw} -Wignored-attributes"                        # invalid for ObjC
  nw="${nw} -Wignored-qualifiers"                        # invalid for ObjC
  nw="${nw} -Wopenmp-simd"                               # invalid for ObjC
  nw="${nw} -Wcomments" # redundant synonym for the un-pluralized -Wcomment
  nw="${nw} -Wmudflap"                                # no longer supported

  # Emacs does NOT care about shadowing; see
  # <http://lists.gnu.org/archive/html/emacs-diffs/2011-11/msg00265.html>.
  nw="${nw} -Wshadow"

  # Emacs's use of alloca inhibits protecting the stack.
  nw="${nw} -Wstack-protector"

  # Emacs's use of partly-pure functions such as CHECK_TYPE make this
  # option problematic.
  nw="${nw} -Wsuggest-attribute=pure"

  # This one is handled manually later:
  nw="${nw} -Wodr"

  # This part is merely for shortening the command line,
  # since -Wno-FOO needs to be added below regardless.
  nw="${nw} -Wmissing-field-initializers"
  ## keep this condition the same as below:
  if test "x${I_AM_GOOD_AT_SILENCING_WARNINGS}" != "xyes"; then
    nw="${nw} -Wsign-compare"
    nw="${nw} -Wswitch"
    nw="${nw} -Wtype-limits"
  fi  ##end check for undocumented environment variable
  nw="${nw} -Wunused-parameter"
  # (-Wformat-nonliteral is already added to nw above)
  ]
  dnl# m4-quoted block number 3 ends

  if test "x${emacs_cv_clang}" = "xyes"; then
    # Prints way too many warnings for now, but they might be needed:
    if test "x${with_ns}" != "xyes"; then
      nw="${nw} -Wcast-align"
    fi ##end for check for NS
    # Same "shorten-the-command-line" reasoning as above:
    nw="${nw} -Wformat-extra-args"
    nw="${nw} -Wtautological-constant-out-of-range-compare"
    nw="${nw} -Wpointer-sign"
    nw="${nw} -Wunused-command-line-argument"
    nw="${nw} -Wunused-value"
  else
    if test "x${WERROR_CFLAGS}" = "x" && test "x${OBJC}" = "x"; then
      dnl# m4/manywarnings.m4 is unaware of this one:
      gl_WARN_ADD([-Wcast-qual])
      dnl# (the gnulib gcc-warning.spec specifically filters it out though)
    fi  ##end check for werror or objc
  fi  ##end check for clang

  if test "x${emacs_cv_apple}" = "xyes"; then
    ## these ones provoke warnings saying they are only valid in
    ## C++/ObjC++ when using Apple's gcc:
    if test "x${GCC}" = "xyes" && test "x${CC}" != "x${CXX}"; then
      nw="${nw} -Wabi"
      nw="${nw} -Wdeprecated"
    fi
    ## for some reason Apple's gcc prints way more prototype warnings
    ## and such than normal; let us see if this is will fix it:
    if test -n "${emacs_g3_CFLAGS}" || test "x${emacs_cv_prog_cc_g3}" = "xyes" || test -n "${emacs_g3_OBJCFLAGS}"; then
      nw="${nw} -Wmissing-prototypes"
      nw="${nw} -Wmissing-declarations"
    fi  ##end check for g3 flags
    # Same "shorten-the-command-line" reasoning as above:
    nw="${nw} -Wpointer-sign"
  fi  ##end check for Apple compiler

  gl_MANYWARN_ALL_GCC([ws])
  gl_MANYWARN_COMPLEMENT([ws],[${ws}],[${nw}])
  for w in ${ws}; do
    gl_WARN_ADD([${w}])
  done
  if test "x${gl_cv_warn_c__Wno_missing_field_initializers}" = "x"; then
    test -z "${gl_cv_warn_c__Wno_missing_field_initializers}"
    gl_WARN_ADD([-Wno-missing-field-initializers]) # We need this one
  elif test "x${gl_cv_warn_c__Wno_missing_field_initializers}" = "xno"; then
    AC_MSG_WARN([missing support for required warning disabling flag])
  elif test "x${gl_cv_warn_c__Wno_missing_field_initializers}" = "xyes"; then
    test -n "${gl_cv_warn_c__Wno_missing_field_initializers}"
    ## (we should be good here)
  fi  ##end check cache value
  ## keep this condition the same as above:
  if test "x${I_AM_GOOD_AT_SILENCING_WARNINGS}" != "xyes"; then
    gl_WARN_ADD([-Wno-sign-compare])     # Too many warnings for now
    gl_WARN_ADD([-Wno-type-limits])      # Too many warnings for now
    gl_WARN_ADD([-Wno-switch])           # Too many warnings for now
  elif test "x${I_AM_GOOD_AT_SILENCING_WARNINGS}" = "xyes" || test "x${PRINT_CONVERSION_WARNINGS}" = "xyes"; then
    gl_WARN_ADD([-Wsign-compare])
  fi  ##end check for undocumented environment variable
  gl_WARN_ADD([-Wno-unused-parameter]) # Too many warnings for now
  gl_WARN_ADD([-Wno-format-nonliteral])dnl

  # In spite of excluding -Wlogical-op above, it is enabled, as of
  # gcc 4.5.0 20090517.
  gl_GCC_VERSION_IFELSE([4],[5],[
    gl_WARN_ADD([-Wno-logical-op])dnl
  ],[
    AC_MSG_NOTICE([no need to explicitly add '-Wno-logical-op' for versions of gcc before 4.5.0])
  ])dnl# end code for gcc4.5(+(?))

  # More things that clang is unduly picky about.
  if test "x${emacs_cv_clang}" = "xyes"; then
    gl_WARN_ADD([-Wno-format-extra-args])
    gl_WARN_ADD([-Wno-tautological-constant-out-of-range-compare])
    gl_WARN_ADD([-Wno-unused-command-line-argument])
    gl_WARN_ADD([-Wno-unused-value])
  fi  ##end check for clang

  dnl# these flags are all relatively newly added, so prevent them from
  dnl# triggering '-Werror' and screwing up the build:
  if test "x${WERROR_CFLAGS}" = "x"; then
    dnl# some Apple-specific warnings (for both apple-gcc and clang) that
    dnl# m4/manywarnings.m4 is unaware of:
    if test "x${emacs_cv_apple}" = "xyes"; then
      gl_WARN_ADD([-Wextra-tokens])
      gl_WARN_ADD([-Wfour-char-constants])
      gl_WARN_ADD([-Wnewline-eof])
      dnl# too much noise for now, so guard it with an undocument envvar:
      if test "x${PRINT_CONVERSION_WARNINGS}" = "xyes"; then
        test -n "${PRINT_CONVERSION_WARNINGS}"
        gl_WARN_ADD([-Wshorten-64-to-32])
      else
        test -z "${PRINT_CONVERSION_WARNINGS}"
        nw="${nw} -Wshorten-64-to-32"
      fi  ##end check for other undocumented environment variable
    fi  ##end check for Apple compiler

    dnl# speaking of m4/manywarnings.m4, running the command in one of the
    dnl# comments discovered the following warning flag with gcc 4.9:
    gl_WARN_ADD([-Wnonportable-cfstrings])dnl

    dnl# We removed this above to retry it with a specific value here:
    gl_WARN_ADD([-Wstrict-overflow=1])dnl

    dnl# These next ones are linker warnings, which we do need to keep
    dnl# conditionalized on the vendor, because preprocessor macro
    dnl# declarations (which is what 'emacs_cv_apple' is based on) tell us
    dnl# nothing about what the linker accepts. Before we can check them,
    dnl# though, we need to make sure that this variable is defined first,
    dnl# to prevent aclocal/autoconf/autom4te/gm4 from choking on the
    dnl# comma when it is used in arguments to the macro we use:
    if test "x${acl_cv_wl}" = "x"; then
      test -z "${acl_cv_wl}" && export acl_cv_wl='-Wl,'
    fi
    AC_SUBST([acl_cv_wl])
    WARN_LDFLAGS=""
    dnl# ok, now that set of vendor cases I was mentioning:
    case ${host_vendor} in 
      apple*)
        dnl#FIXME: make sure these flags are actually used properly
        dnl# everywhere.
        dnl# First set is guarded with a conditional because the messages
        dnl# can be noisy and are more informational than actual warnings:
        if test "x${DEBUGGING_SYMBOLS}" = "xYES" || test "x${LD_DEBUG_SNAPSHOT}" != "x"; then
          test -n "${DEBUGGING_SYMBOLS}" || test -n "${LD_DEBUG_SNAPSHOT}"
          if test "x${LD_TRACE_FILE}" != "x" || test "x${LD_TRACE_ARCHIVES}" != "x" || test "x${RC_TRACE_ARCHIVES}" != "x" || test "x${LD_TRACE_DYLIBS}" != "x" || test "x${RC_TRACE_DYLIBS}" != "x" || test "x${RC_TRACE_DYLIB_SEARCHING}" != "x"; then
            test -n "${LD_TRACE_FILE}" || test -n "${LD_TRACE_ARCHIVES}" || test -n "${RC_TRACE_ARCHIVES}" || test -n "${LD_TRACE_DYLIBS}" || test -n "${RC_TRACE_DYLIBS}" || test -n "${RC_TRACE_DYLIB_SEARCHING}"
            gl_WARN_ADD([${acl_cv_wl}-t],[WARN_LDFLAGS])
          fi  ##end check for a number of trace-related ld envvars
          gl_WARN_ADD([${acl_cv_wl}-why_load],[WARN_LDFLAGS])
          if test "x${LD_PRINT_OPTIONS}" != "x" || test "x${LD_PRINT_ORDER_FILE_STATISTICS}" != "x" || test "x${PRINT_COMMAND}" != "x"; then
            test -n "${PRINT_COMMAND}" || test -n "${LD_PRINT_OPTIONS}" || test -n "${LD_PRINT_ORDER_FILE_STATISTICS}"
            gl_WARN_ADD([${acl_cv_wl}-print_statistics],[WARN_LDFLAGS])
          fi  ##end check for a few printing-related ld envvars
        fi  ##end check for debugging symbols envvars
        test -n "${LD_WARN_FILE}"
        gl_WARN_ADD([${acl_cv_wl}-warn_compact_unwind],[WARN_LDFLAGS])
        gl_WARN_ADD([${acl_cv_wl}-warn_weak_exports],[WARN_LDFLAGS])
        gl_WARN_ADD([${acl_cv_wl}-warn_stabs],[WARN_LDFLAGS])
        gl_WARN_ADD([${acl_cv_wl}-warn_commons],[WARN_LDFLAGS])
        test -n "${LD_WARN_COMMONS}"
        ;;
      gnu*)
        dnl#FIXME: these flags are untested, and are only based on my
        dnl# my reading of the GNU ld manpage:
        gl_WARN_ADD([${acl_cv_wl}--warn-alternate-em],[WARN_LDFLAGS])
        gl_WARN_ADD([${acl_cv_wl}--warn-common],[WARN_LDFLAGS])
        gl_WARN_ADD([${acl_cv_wl}--warn-constructors],[WARN_LDFLAGS])
        gl_WARN_ADD([${acl_cv_wl}--warn-multiple-gp],[WARN_LDFLAGS])
        gl_WARN_ADD([${acl_cv_wl}--warn-section-align],[WARN_LDFLAGS])
        ;;
    esac
    AC_SUBST([WARN_LDFLAGS])dnl

    dnl# some other ones from the gcc4.2 manual that m4/manywarnings.m4
    dnl# leaves out:
    gl_GCC_VERSION_IFELSE([4],[2],[
      dnl# the gnulib gcc-warning.spec filters this out and calls it
      dnl# "obsolescent", so guard it with an undocument envvar:
      if test "x${I_AM_GOOD_AT_SILENCING_WARNINGS}" = "xyes"; then
        test -n "${I_AM_GOOD_AT_SILENCING_WARNINGS}"
        dnl# it also prints a lot when whe check the lisp object type:
        if test "x${enable_check_lisp_object_type}" != "xyes"; then
          gl_WARN_ADD([-Waggregate-return])
        fi
      else
        test -z "${I_AM_GOOD_AT_SILENCING_WARNINGS}"
        nw="${nw} -Waggregate-return"
      fi  ##end check for one undocumented environment variable
      dnl# the comment in the gnulib gcc-warning.spec for why this is
      dnl# filtered out from manywarnings.m4 says:
      dnl# "FIXME maybe? too much noise; encourages bad changes"
      dnl# so guard with another undocument envvar:
      if test "x${PRINT_CONVERSION_WARNINGS}" = "xyes"; then
        test -n "${PRINT_CONVERSION_WARNINGS}"
        gl_WARN_ADD([-Wconversion])
      else
        test -z "${PRINT_CONVERSION_WARNINGS}"
        nw="${nw} -Wconversion"
      fi  ##end check for other undocumented environment variable
      dnl# the comment in the gnulib gcc-warning.spec for why this is
      dnl# filtered out from manywarnings.m4 says:
      dnl# "FIXME: do not want.  others may"
      dnl# I am "others", so just leave it in:
      gl_WARN_ADD([-Wdeclaration-after-statement])
      dnl# this one is not even mentioned:
      gl_WARN_ADD([-Wmissing-noreturn])
      if test "x${I_AM_GOOD_AT_SILENCING_WARNINGS}" = "xyes"; then
        test -n "${I_AM_GOOD_AT_SILENCING_WARNINGS}"
        ## maintainer mode can also trigger '-Werror', so avoid:
        if test "x${USE_MAINTAINER_MODE}" != "xyes"; then
          dnl# the comment in the gnulib gcc-warning.spec for why this is
          dnl# filtered out from manywarnings.m4 says:
          dnl# "FIXME maybe? borderline.  some will want this"
          dnl# I am one of the "some", but only some of the time:
          gl_WARN_ADD([-Wfloat-equal])
        fi  ##end check for maintainer mode
        ## clang prints a lot of these:
        if test "x${emacs_cv_clang}" != "xyes"; then
          dnl# the comment in the gnulib gcc-warning.spec for why this is
          dnl# filtered out from manywarnings.m4 is too long to fit on a
          dnl# single line, so never mind about that...
          gl_WARN_ADD([-Wpadded])
        fi  ##end check for clang
        dnl# the comment in the gnulib gcc-warning.spec for why this is
        dnl# filtered out from manywarnings.m4 has gnulib-specific reasons:
        gl_WARN_ADD([-Wredundant-decls])
        ## newer versions of the Emacs configure.ac say to prefer this,
        ## as we do NOT use Bison... but it still prints too many though.
      else
        test -z "${I_AM_GOOD_AT_SILENCING_WARNINGS}"
        ## ('-Wfloat-equal' is added to 'nw' specifically for gnulib below)
        nw="${nw} -Wpadded"
        nw="${nw} -Wredundant-decls"
      fi  ##end check for undocumented environment variable
      dnl#FIXME: this will always end up in the first case, unless "${nw}"
      dnl# is entirely empty:
      if test -n "`echo ${nw} | grep Wswitch`"; then
        nw="${nw} -Wswitch-enum"
      else
        dnl# the comment in the gnulib gcc-warning.spec for why this is
        dnl# filtered out from manywarnings.m4 says:
        dnl# "FIXME maybe? borderline.  some will want this"
        dnl# Sometimes I am one of those "some", so for when I am:
        gl_WARN_ADD([-Wswitch-enum])
      fi  ##end check to see if 'nw' already contains '-Wswitch' (broken)
      if test "x${emacs_cv_clang}" = "xyes" || test "x${emacs_cv_apple}" = "xyes"; then
        nw="${nw} -Wunreachable-code"
      else
        gl_WARN_ADD([-Wunreachable-code])
      fi  ##end check for clang or Apple compiler
      dnl# even though emacs has no major C++ code, also check for some
      dnl# C++-specific flags anyways, just in case:
      if test "x${CXX}" != "x" || test "x${CC}" = "x${CXX}"; then
        WARN_CXXFLAGS=""
        AC_LANG_PUSH([C++])dnl
        gl_WARN_ADD([-Wabi],[WARN_CXXFLAGS])
        gl_WARN_ADD([-Wabi-tag],[WARN_CXXFLAGS])
        gl_WARN_ADD([-Wconditionally-supported],[WARN_CXXFLAGS])
        if test "x${PRINT_CONVERSION_WARNINGS}" = "xyes"; then
          test -n "${PRINT_CONVERSION_WARNINGS}"
          gl_WARN_ADD([-Wconversion-null],[WARN_CXXFLAGS])
        fi  ##end check for undocumented environment variable.
        gl_WARN_ADD([-Wctor-dtor-privacy],[WARN_CXXFLAGS])
        gl_WARN_ADD([-Wdeprecated],[WARN_CXXFLAGS])
        gl_WARN_ADD([-Wdelete-incomplete],[WARN_CXXFLAGS])
        gl_WARN_ADD([-Wdelete-non-virtual-dtor],[WARN_CXXFLAGS])
        if test "x${I_AM_GOOD_AT_SILENCING_WARNINGS}" = "xyes"; then
          test -n "${I_AM_GOOD_AT_SILENCING_WARNINGS}"
          gl_WARN_ADD([-Weffc++],[WARN_CXXFLAGS])
        fi  ##end check for undocumented environment variable.
        gl_WARN_ADD([-Wglobal-constructors],[WARN_CXXFLAGS])
        gl_WARN_ADD([-Winherited-variadic-ctor],[WARN_CXXFLAGS])
        gl_WARN_ADD([-Winvalid-offsetof],[WARN_CXXFLAGS])
        gl_WARN_ADD([-Wliteral-suffix],[WARN_CXXFLAGS])
        gl_WARN_ADD([-Wnoexcept],[WARN_CXXFLAGS])
        gl_WARN_ADD([-Wnon-template-friend],[WARN_CXXFLAGS])
        gl_WARN_ADD([-Wnon-virtual-dtor],[WARN_CXXFLAGS])
        if test "x${I_AM_GOOD_AT_SILENCING_WARNINGS}" = "xyes"; then
          test -n "${I_AM_GOOD_AT_SILENCING_WARNINGS}"
          gl_WARN_ADD([-Wold-style-cast],[WARN_CXXFLAGS])
        fi  ##end check for undocumented environment variable.
        gl_WARN_ADD([-Woverloaded-virtual],[WARN_CXXFLAGS])
        gl_WARN_ADD([-Wpmf-conversions],[WARN_CXXFLAGS])
        gl_WARN_ADD([-Wreorder],[WARN_CXXFLAGS])
        if test "x${PRINT_CONVERSION_WARNINGS}" = "xyes"; then
          test -n "${PRINT_CONVERSION_WARNINGS}"
          gl_WARN_ADD([-Wsign-promo],[WARN_CXXFLAGS])
        fi  ##end check for other undocumented environment variable.
        gl_WARN_ADD([-Wstrict-null-sentinel],[WARN_CXXFLAGS])
        if test "x${I_AM_GOOD_AT_SILENCING_WARNINGS}" = "xyes"; then
          test -n "${I_AM_GOOD_AT_SILENCING_WARNINGS}"
          gl_WARN_ADD([-Wuseless-cast],[WARN_CXXFLAGS])
        fi  ##end check for undocumented environment variable.
        gl_WARN_ADD([-Wvirtual-move-assign],[WARN_CXXFLAGS])
        if test "x${I_AM_GOOD_AT_SILENCING_WARNINGS}" = "xyes"; then
          test -n "${I_AM_GOOD_AT_SILENCING_WARNINGS}"
          gl_WARN_ADD([-Wzero-as-null-pointer-constant],[WARN_CXXFLAGS])
        fi  ##end check for undocumented environment variable.
        AC_SUBST([WARN_CXXFLAGS])dnl
        AC_LANG_POP
      else
        AC_MSG_NOTICE([skipping checks for C++-specific warnings])
      fi  ##end check for C++ compiler
    ],[
      AC_MSG_NOTICE([skipping checks for warnings for gcc 4.2 or newer])
    ])dnl# end block of code for gcc4.2+

    dnl# fails for same reason as why "-Wstack-protector" is in "${nw}":
    if test "x${I_AM_GOOD_AT_SILENCING_WARNINGS}" = "xyes"; then
      test -n "${I_AM_GOOD_AT_SILENCING_WARNINGS}"
      dnl# introduced in 4.7, apparently:
      gl_GCC_VERSION_IFELSE([4],[7],[
        dnl# gcc-warning.spec says to add a sane default for this, so I
        dnl# went by my current stack limit:
        gl_WARN_ADD([-Wstack-usage=8192000])
        dnl# (8192 kbytes = 8192000 bytes... right?)
      ],[
        AC_MSG_NOTICE([skipping checks for warnings for gcc 4.7 or newer])
      ])dnl# end block of code for gcc4.7+
    else
      test -z "${I_AM_GOOD_AT_SILENCING_WARNINGS}"
    fi  ##end check for undocumented environment variable.

    dnl# some other ones from the gcc4.9 manual that m4/manywarnings.m4
    dnl# leaves out:
    gl_GCC_VERSION_IFELSE([4],[9],[
      if test "x${CXX}" != "x" && test "x${CXX}" = "x${CC}"; then
        dnl# the comment in the gnulib gcc-warning.spec for why this is
        dnl# filtered out from manywarnings.m4 says:
        dnl# "FIXME maybe? borderline.  some will want this"
        dnl# I sometimes do, so for when I do:
        gl_WARN_ADD([-Wc++-compat])
      fi  ##end check for C++ compiler
      if test "x${PRINT_CONVERSION_WARNINGS}" = "xyes"; then
        dnl# leave this environment variable undocumented:
        test -n "${PRINT_CONVERSION_WARNINGS}"
        gl_WARN_ADD([-Wfloat-conversion])
        dnl# the gnulib gcc-warning.spec calls this next one "obsolescent":
        gl_WARN_ADD([-Wtraditional-conversion])
        dnl# the comment in the gnulib gcc-warning.spec for why this is
        dnl# filtered out from manywarnings.m4 says:
        dnl# "FIXME maybe? borderline.  some will want this"
        dnl# (which I sometimes do, but only sometimes)
        gl_WARN_ADD([-Wsign-conversion])
      else
        test -z "${PRINT_CONVERSION_WARNINGS}"
        nw="${nw} -Wfloat-conversion"
        nw="${nw} -Wtraditional-conversion"
        nw="${nw} -Wsign-conversion"
      fi  ##end dummy variable check
      if test "x${I_AM_GOOD_AT_SILENCING_WARNINGS}" = "xyes"; then
        dnl# leave this environment variable undocumented, too:
        test -n "${I_AM_GOOD_AT_SILENCING_WARNINGS}"
        dnl# the comment in the gnulib gcc-warning.spec for why this is
        dnl# filtered out from manywarnings.m4 has gnulib-specific reasons:
        gl_WARN_ADD([-Wunsuffixed-float-constants])
      else
        test -z "${I_AM_GOOD_AT_SILENCING_WARNINGS}"
        nw="${nw} -Wunsuffixed-float-constants"
      fi  ##end dummy variable check
    ],[
      AC_MSG_NOTICE([skipping checks for warnings for gcc 4.9 or newer])
    ])dnl# end block of code for gcc4.9+

    dnl# gcc 5 has been released:
    gl_GCC_VERSION_IFELSE([5],[0],[
      dnl# most of the new warnings for gcc 5 are now handled by
      dnl# manywarnings.m4, but handle a few here manually anyways:
      if test "x${I_AM_GOOD_AT_SILENCING_WARNINGS}" = "xyes"; then
        test -n "${I_AM_GOOD_AT_SILENCING_WARNINGS}"
        gl_WARN_ADD([-Wc90-c99-compat])
        gl_WARN_ADD([-Wc99-c11-compat])
      else
        test -z "${I_AM_GOOD_AT_SILENCING_WARNINGS}"
        nw="${nw} -Wc90-c99-compat"
        nw="${nw} -Wc99-c11-compat"
      fi  ##end dummy variable check
      dnl# this one failed to make it in on time:
      if test "x${PRINT_CONVERSION_WARNINGS}" = "xyes"; then
        test -n "${PRINT_CONVERSION_WARNINGS}"
        gl_WARN_ADD([-Wbitfield-conversion])
      fi  ##end dummy variable check
      if test "x${ac_cv_lto_supported}" = "xyes" || test -n "${LTO}"; then
        if test "x${CXX}" != "x" || test "x${CC}" = "x${CXX}"; then
          AC_LANG_PUSH([C++])dnl
          gl_WARN_ADD([-Wodr],[WARN_CXXFLAGS])
          AC_LANG_POP
        else
          AC_MSG_NOTICE([-Wodr is only relevant for C++, skipping it])
        fi  ##end check for C++ compiler
      else
        AC_MSG_NOTICE([-Wodr is only relevant when using LTO, skipping it])
      fi  ##end check for LTO
    ],[
      AC_MSG_NOTICE([skipping checks for warnings for gcc 5.0 or newer])
    ])dnl# end block of code for gcc 5

    dnl# gcc 6 has been released:
    gl_GCC_VERSION_IFELSE([6],[0],[
      dnl# as with gcc 5, most of the new warnings for gcc 6 are now
      dnl# handled by manywarnings.m4, but handle a few here manually
      dnl# anyways:
      gl_WARN_ADD([-Woverride-init-side-effects])
      gl_COMPILER_OPTION_IF([-Wshift-overflow=2],
                        [WARN_CFLAGS="${WARN_CFLAGS} -Wshift-overflow=2"],
                            [gl_WARN_ADD([-Wshift-overflow])])
      ## this got stricter, I think?
      nw="${nw} -Wbool-compare"
      ## but anyways...
      if test "x${CXX}" != "x" || test "x${CC}" = "x${CXX}"; then
        AC_LANG_PUSH([C++])dnl
        gl_WARN_ADD([-Wterminate],[WARN_CXXFLAGS])
        gl_WARN_ADD([-Wmultiple-inheritance],[WARN_CXXFLAGS])
        gl_WARN_ADD([-Wvirtual-inheritance],[WARN_CXXFLAGS])
        AC_LANG_POP
      else
        AC_MSG_NOTICE([-Wterminate is only relevant for C++, skipping it])
      fi  ##end check for C++ compiler
      if test "x${ac_cv_lto_supported}" = "xyes" || test -n "${LTO}"; then
        gl_WARN_ADD([-Wlto-type-mismatch])
        ## GCC bug 78077:
        case ${host_vendor} in
          apple*)
            gl_WARN_ADD([-D__builtin_unreachable=__builtin_trap])
            ;;
        esac
        gl_WARN_ADD([-fno-strict-aliasing],[WARN_LDFLAGS])
      else
        nw="${nw} -Wlto-type-mismatch"
        AC_MSG_NOTICE([-Wlto-type-mismatch is only relevant when using LTO, skipping it])
      fi  ##end check for LTO
    ],[
      AC_MSG_NOTICE([skipping checks for warnings for gcc 6.0 or newer])
    ])dnl# end block of code for gcc 6

    dnl# to be added in a future version of gcc:
    gl_GCC_VERSION_IFELSE([7],[0],[
      gl_WARN_ADD([-Wdangling-else])dnl
      gl_WARN_ADD([-Wduplicate-decl-specifier])dnl
      gl_WARN_ADD([-Wmemset-elt-size])dnl
      gl_WARN_ADD([-Wswitch-unreachable])dnl
      gl_WARN_ADD([-Wimplicit-fallthrough])dnl
      gl_COMPILER_OPTION_IF([-Wformat-length=2],
                          [WARN_CFLAGS="${WARN_CFLAGS} -Wformat-length=2"],
                          [gl_WARN_ADD([-Wformat-length])])
      gl_COMPILER_OPTION_IF([-Wformat-overflow=2],
                          [WARN_CFLAGS="${WARN_CFLAGS} -Wformat-overflow=2"],
                          [gl_WARN_ADD([-Wformat-overflow])])
      gl_COMPILER_OPTION_IF([-Wformat-truncation=2],
                          [WARN_CFLAGS="${WARN_CFLAGS} -Wformat-truncation=2"],
                          [gl_WARN_ADD([-Wformat-truncation])])
      gl_COMPILER_OPTION_IF([-Wstringop-overflow=2],
                          [WARN_CFLAGS="${WARN_CFLAGS} -Wstringop-overflow=2"],
                          [gl_WARN_ADD([-Wstringop-overflow])])
      gl_WARN_ADD([-Wexpansion-to-defined])dnl
      gl_WARN_ADD([-Wrestrict])dnl
      gl_WARN_ADD([-Wint-in-bool-context])dnl
      gl_WARN_ADD([-Wpointer-compare])dnl
      gl_WARN_ADD([-Wbool-operation])dnl
      gl_WARN_ADD([-Wduplicated-branches])dnl
      gl_WARN_ADD([-Walloca-larger-than=16384])dnl# MAX_ALLOCA = 16 * 1024
      gl_WARN_ADD([-Wvla-larger-than=16384])dnl# vlas = allocas
      gl_WARN_ADD([-Walloc-size-larger-than=32767])dnl# INT16_MAX
      gl_WARN_ADD([-Walloc-zero])dnl
      gl_WARN_ADD([-Wpsabi])dnl
      gl_WARN_ADD([-Wbuiltin-declaration-mismatch])dnl
      ## -faligned-new is checked later:
      if test "x${CXX}" != "x" || test "x${CC}" = "x${CXX}"; then
        AC_LANG_PUSH([C++])dnl
        gl_COMPILER_OPTION_IF([-Waligned-new=all],
                      [WARN_CXXFLAGS="${WARN_CXXFLAGS} -Waligned-new=all"],
                      [gl_WARN_ADD([-Waligned-new],[WARN_CXXFLAGS])])
        gl_WARN_ADD([-Wbuiltin-function-redefined],[WARN_CXXFLAGS])
        gl_WARN_ADD([-Wsubobject-linkage],[WARN_CXXFLAGS])
        AC_LANG_POP
      else
        AC_MSG_NOTICE([-Waligned-new only relevant for C++, skipping it])
      fi  ##end check for C++ compiler
    ],[
      AC_MSG_NOTICE([skipping checks for warnings for gcc 7.0 or newer])
    ])dnl# end block of code for gcc 7
    ##...okay, that should be it.
  else
    AC_MSG_WARN([WERROR_CFLAGS is set, so we have to skip some warnings])
  fi  ##end check for werror flags

  AC_DEFINE([lint],[1],[Define to 1 if the compiler is checking for lint.])
  AC_DEFINE([GCC_LINT],[1],[Define to 1 if --enable-gcc-warnings.])
  AH_VERBATIM([FORTIFY_SOURCE],
  [/* Enable compile-time and run-time bounds-checking, and some warnings,
    * without upsetting glibc 2.15+. */
   #if !defined _FORTIFY_SOURCE && defined __OPTIMIZE__ && __OPTIMIZE__
   # define _FORTIFY_SOURCE 2
   #endif /* !_FORTIFY_SOURCE && __OPTIMIZE__ */
  ])dnl
  AC_DEFINE([GNULIB_PORTCHECK],[1],
            [Define to 1 to enable some gnulib portability checks])dnl

  # We use a slightly smaller set of warning options for lib/.
  # Remove the following and save the result in GNULIB_WARN_CFLAGS.
  nw=""
  nw="${nw} -Wdouble-promotion"                     # warnings in ftoastr.c
  nw="${nw} -Wfloat-equal"                    # borderline, and hard to fix
  nw="${nw} -Wredundant-decls"                            # prints too many
  nw="${nw} -Wunsuffixed-float-constants"                             # meh
  nw="${nw} -Wunused-macros"                                     # annoying
  nw="${nw} -Wswitch-default"                # Too many warnings for now(?)
  # Same condition as where it was initially added:
  if test "x${emacs_cv_apple}" = "xyes"; then
    nw="${nw} -Wshorten-64-to-32"
  fi
  nw="${nw} -Wc90-c99-compat"   # Emacs/gnulib both use boolean types a lot
  nw="${nw} -Wc99-c11-compat"       # Emacs/gnulib both use _Noreturn a lot
  nw="${nw} -Wsuggest-attribute=const"                                # meh
  nw="${nw} -Wformat-signedness"                 # shorten the command line

  dnl# see if this will silence a warning in error.c:
  gl_WARN_ADD([-Wno-format-signedness],[GNULIB_WARN_CFLAGS])dnl

  dnl# m4/manywarnings.m4 skips this one, because, apparently:
  dnl# "FIXME maybe? too many false positives"
  dnl# (according to the gnulib gcc-warning.spec)
  gl_WARN_ADD([-Wundef],[GNULIB_WARN_CFLAGS])dnl
  dnl# it makes sense for gnulib, though, as it can be used to find modules
  dnl# that are missing as dependencies for ones being used...

  gl_MANYWARN_COMPLEMENT([GNULIB_WARN_CFLAGS],[${WARN_CFLAGS}],[${nw}])
  AC_SUBST([GNULIB_WARN_CFLAGS])dnl
  # end warning checks
fi  ##end conditional on gl_gcc_warnings

if test "x${gl_better_debugging}" = "xyes"; then
  ##### Not exactly warning flags, but adding them this way is easier:
  AC_MSG_CHECKING([other compiler flags])
  AC_MSG_RESULT([ ])
  dnl# Apple includes this as parts of its GCC_DEBUGGING_SYMBOLS checkbox
  dnl# that it exposes in its Xcode GUI:
  gl_WARN_ADD([-fno-eliminate-unused-debug-symbols],[BETTER_DEBUG_CFLAGS])
  dnl# etc/DEBUG mentions this:
  gl_WARN_ADD([-fno-crossjumping],[BETTER_DEBUG_CFLAGS])
  dnl# See note about '-fexceptions' below:
  gl_WARN_ADD([-fno-delete-dead-exceptions],[BETTER_DEBUG_CFLAGS])
  dnl# I have added a bunch of null pointer checks to satisfy the clang
  dnl# static analyzer, and want to make sure that I am not causing any
  dnl# code paths to be deleted when I use null pointer checks like that:
  gl_WARN_ADD([-fno-delete-null-pointer-checks],[BETTER_DEBUG_CFLAGS])
  gl_WARN_ADD([-fno-isolate-erroneous-paths-dereference],
              [BETTER_DEBUG_CFLAGS])
  dnl# etc/DEBUG mentions this:
  gl_WARN_ADD([-fno-omit-frame-pointer],[BETTER_DEBUG_CFLAGS])
  dnl# etc/PROBLEMS mentions this, as well as a later part of this file:
  gl_COMPILER_OPTION_IF([-fno-optimize-sibling-calls],
                        [if test "x${GCC}" = "xyes" \
  && test x"`${CC} --version 2>/dev/null | grep 'gcc.* 4.5.0'`" != x"" \
  && test x"`echo ${CFLAGS} | grep '\-O@<:@23@:>@'`" != x""; then
   CFLAGS="${CFLAGS} -fno-optimize-sibling-calls"
  else
   BETTER_DEBUG_CFLAGS="${BETTER_DEBUG_CFLAGS} -fno-optimize-sibling-calls"
  fi])
  dnl# I remember a thread on the emacs-devel mailing list about the
  dnl# positive version of this flag causing issues, so use its negative:
  gl_WARN_ADD([-fno-optimize-strlen],[BETTER_DEBUG_CFLAGS])
  dnl# It has "dangerous" in its name:
  gl_WARN_ADD([-fno-sched-spec-load-dangerous],[BETTER_DEBUG_CFLAGS])
  dnl# The GCC manual says that this can affect the debugging experience:
  gl_WARN_ADD([-fno-split-wide-types],[BETTER_DEBUG_CFLAGS])
  dnl# FIXME: manywarnings.m4 will now automatically add '-fno-common'
  dnl# for us; investigate if that affects dumping at all.
  ##
  dnl# This next one is not supported for C currently, but it is not
  dnl# rejected either, so why not try it, in case one day the C frontend
  dnl# does support it?
  gl_WARN_ADD([-fbounds-check],[BETTER_DEBUG_CFLAGS])
  dnl# More for debugging gcc than emacs, but whatever:
  gl_WARN_ADD([-fcheck-data-deps],[BETTER_DEBUG_CFLAGS])
  dnl# emacs does some weird exception handling stuff (specifically the
  dnl# .gdbinit file signals some sort of exception handler function in the
  dnl# program), so this might be necessary:
  gl_WARN_ADD([-fexceptions],[BETTER_DEBUG_CFLAGS])
  dnl# FIXME: might also want to check for the required runtime support
  dnl# for this next one:
  gl_WARN_ADD([-fstack-check],[BETTER_DEBUG_CFLAGS])
  dnl# For stuff that -Wstrict-overflow=1 might miss:
  gl_WARN_ADD([-ftrapv],[BETTER_DEBUG_CFLAGS])
  dnl# These next two should both already be enabled by '-g', but just in
  dnl# case something happened:
  gl_WARN_ADD([-fvar-tracking],[BETTER_DEBUG_CFLAGS])
  gl_WARN_ADD([-fvar-tracking-assignments],[BETTER_DEBUG_CFLAGS])
  dnl# Who knows, might be useful:
  gl_WARN_ADD([-gpubnames],[BETTER_DEBUG_CFLAGS])
  dnl# I think I added this to try to fix some issues with dsymutil:
  gl_WARN_ADD([-gstrict-dwarf],[BETTER_DEBUG_CFLAGS])
  AC_SUBST([BETTER_DEBUG_CFLAGS])dnl
  ## Is that enough?
fi

edit_cflags="
  s,///*,/,g
  s/^/ /
  s/ -I/ ${isystem}/g
  s/^ //
"

if test "x${gl_better_debugging}" = "xyes"; then
  ## might want to add back '&& test "x${CLANG_ANALYZER}" = "xfalse"':
  if test "x${GCC}" = "xyes"; then
    gl_COMPILER_OPTION_IF([-Og],[
    case ${CFLAGS} in
      *-O2*)
        edit_cflags="
          ${edit_cflags}
          s/ -O2/ -Og/g
        "
        ;;
      *-O3*)
        edit_cflags="
          ${edit_cflags}
          s/ -O3/ -Og/g
        "
        ;;
      *-Ofast*)
        edit_cflags="
          ${edit_cflags}
          s/ -Ofast/ -Og/g
        "
        ;;
    esac
    ])
  fi
fi

dnl# FIXME: I would like to be able to subst edit_cflags, but that leads to
dnl# make errors...

if test "x${edit_cflags}" != "x"; then
  test -x "`which sed`"
  test -n "${edit_cflags}" && export CFLAGS=`AS_ECHO(["${PRE_WARNING_TESTING_CFLAGS}"]) | sed -e "${edit_cflags}"`
  if test "x${CFLAGS}" != "x${PRE_WARNING_TESTING_CFLAGS}"; then
    AC_MSG_NOTICE([we have edited CFLAGS; they are now '${CFLAGS}'])
  fi
else
  CFLAGS="${PRE_WARNING_TESTING_CFLAGS}"
fi
dnl# This is a hack to avoid breaking the clang static analyzer:
AC_SUBST([PRE_WARNING_TESTING_CFLAGS])dnl

dnl########################################################################
dnl# Port to Automake 1.11.
dnl# This section can be removed once we assume Automake 1.14 or later.
: ${AM_DEFAULT_VERBOSITY=1}
: ${AM_V=${AM_DEFAULT_VERBOSITY}}
: ${AM_DEFAULT_V=${AM_DEFAULT_VERBOSITY}}
dnl# now subst:
AC_SUBST([AM_V])dnl
AM_SUBST_NOTMAKE([AM_V])dnl
AC_SUBST([AM_DEFAULT_V])dnl
AM_SUBST_NOTMAKE([AM_DEFAULT_V])dnl
AC_SUBST([AM_DEFAULT_VERBOSITY])dnl

dnl########################################################################
dnl# Some other nice autoconf tests.
AC_MSG_CHECKING([other programs])
AC_MSG_RESULT([ ])
dnl# These are ifdef-ed out, since gl_EARLY and/or Autoconf should have
dnl# already done them.
m4_ifdef([gl_00GNULIB],[:],[
AC_PROG_INSTALL
AC_PROG_MKDIR_P
if test "x${RANLIB}" = "x" && test "x${ac_cv_prog_ac_ct_RANLIB}" = "x"; then
  test -z "${RANLIB}" && test -z "${ac_cv_prog_ac_ct_RANLIB}"
  AC_PROG_RANLIB
fi
])dnl
dnl# this is ifdef-ed the other way (i.e. "in" instead of "out"):
m4_ifdef([AM_PROG_AR],[
  AM_PROG_AR
],[:])dnl

dnl# not sure where else to put these:
AC_PATH_PROG([FILE_MAGIC_BIN],[file])dnl
AC_PATH_PROGS([MACHOCHECK],[machocheck machocheck_debug])dnl
AC_PATH_PROG([RELPATH_BIN],[relpath])dnl
AC_PATH_PROGS([ID_BIN],[id gid])dnl

if test "x${ID_BIN}" != "x"; then
  test -n "${ID_BIN}"
  AC_DEFINE([HAVE_ID_BIN],[1],[Define to 1 if we have the 'id' program])
fi

AC_MSG_CHECKING([libtool things])
AC_MSG_RESULT([ ])
dnl# only need the libtool macro for some of the tools that it checks:
m4_ifdef([LT_INIT],[
  unset ac_cv_prog_ac_ct_RANLIB
  LT_INIT([disable-fast-install disable-shared static no-pic])dnl
  m4_ifdef([LT_LANG],[
    LT_LANG([C])dnl
  ])dnl
],[
  EMACS_REQUIRED_DARWIN_CHECKS
])dnl

dnl# back to other program checks again:
AC_MSG_CHECKING([other programs again])
AC_MSG_RESULT([ ])

dnl########################################################################
dnl# Sadly, AC_PROG_LN_S is too restrictive. It also tests whether links
dnl# can be made to directories. This is not relevant for our usage, and
dnl# excludes some cases that work fine for us. E.g. MS Windows or files
dnl# hosted on AFS, both examples where simple links work, but links to
dnl# directories fail. We use a cut-down version instead.
dnl# Keep autoscan happy with a dummy condition though:
if test "x" = "Y0"; then
  test -x "`which autoscan`"
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
## now with a cache value:
AC_CACHE_CHECK([for -znocombreloc ld flag],
               [emacs_cv_ld_znocombreloc],[
AC_LINK_IFELSE([AC_LANG_PROGRAM([[]],[[]])],
  [emacs_cv_ld_znocombreloc=yes],
  [emacs_cv_ld_znocombreloc=no])dnl
])dnl
  ## pseudo-unset if failed:
if test "x${emacs_cv_ld_znocombreloc}" = "xno"; then
   LDFLAGS_NOCOMBRELOC=""
fi
LDFLAGS="${late_LDFLAGS}"
AC_SUBST([late_LDFLAGS])dnl

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

AC_ARG_VAR([CANNOT_DUMP],
           [Set to "yes" to disable the dump-emacs function.])dnl

dnl# The function dump-emacs will not be defined and temacs will do
dnl# (load "loadup") automatically unless told otherwise:
test "x${CANNOT_DUMP}" = "x" && export CANNOT_DUMP="no"
case "${opsys}" in
  nacl) CANNOT_DUMP=yes ;;
  your-opsys-here) CANNOT_DUMP=yes ;;
esac

if test "x${CANNOT_DUMP}" = "xyes"; then
  AC_DEFINE([CANNOT_DUMP],[1],
            [Define if Emacs cannot be dumped on your system.])
elif test "x${emacs_cv_sanitize_address}" = "xyes"; then
  AC_MSG_WARN([[Addresses are sanitized; suggest CANNOT_DUMP=yes]])
fi

AC_SUBST([CANNOT_DUMP])dnl

dnl# m4-quotation block 5 begins (for UNEXEC_OBJ):
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
  nextstep)
   UNEXEC_OBJ=unexnext.o
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

### Compute the unexec source name from the object name:
if test "x${UNEXEC_OBJ}" != "x" && test "x${UNEXEC_SRC}" = "x"; then
  test -n "${UNEXEC_OBJ}" && test -z "${UNEXEC_SRC}" && export UNEXEC_SRC="`echo ${UNEXEC_OBJ} | sed 's/\.o/.c/'`"
fi
]
dnl# m4-quotation block 5 ends

AC_SUBST([UNEXEC_OBJ])dnl
AC_SUBST([UNEXEC_SRC])dnl

dnl# m4-quotation block 6 begins (for LD_SWITCH_SYSTEM):
[
LD_SWITCH_SYSTEM=""
case "${opsys}" in
  freebsd|dragonfly)
   ## Let `ld' find image libs and similar things in /usr/local/lib.
   ## The system compiler, GCC, has apparently been modified to not
   ## look there, contrary to what a stock GCC would do.
### It is not our place to do this.  See bug#10313#17.
### (hence the "BE_PRESUMPTUOUS" environment variable)
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
### (hence the "BE_PRESUMPTUOUS" environment variable)
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

AC_SUBST([LD_SWITCH_SYSTEM])dnl

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

C_SWITCH_SYSTEM=""
C_SWITCH_SYSTEM_TEMACS=""
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
elif test "x${opsys}" = "xdarwin"; then
  case "${canonical}" in
    *-apple-darwin*)
      if test "x${MAC_OSX_DEFINED}" != "x1"; then
        export MAC_OSX_DEFINED=1
        C_SWITCH_SYSTEM="-DMAC_OSX=1"
      fi
    ;;
    mac-apple-*) C_SWITCH_SYSTEM="-DMAC_OS" ;;
  esac
  if test "x${emacs_cv_apple}" = "xyes"; then
    ## some of the really old files in mac/ have pascal return types:
    C_SWITCH_SYSTEM="${C_SWITCH_SYSTEM} -fpascal-strings"
  fi
  C_SWITCH_SYSTEM_TEMACS="-Dtemacs"
fi
## gnu-linux might need -D_BSD_SOURCE on old libc5 systems.
## It is redundant in glibc2, since we define _GNU_SOURCE.
AC_SUBST([C_SWITCH_SYSTEM])dnl
AC_SUBST([C_SWITCH_SYSTEM_TEMACS])dnl

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
    if test "x${INTLLIBS}" != "x"; then
      echo "Trying to add INTLLIBS to LIBS_SYSTEM here for darwin"
      test -n "${INTLLIBS}" && LIBS_SYSTEM="${INTLLIBS}"
    elif test "x${LIBINTL}" != "x"; then
      echo "Trying to add LIBINTL to LIBS_SYSTEM here for darwin"
      test -n "${LIBINTL}" && LIBS_SYSTEM="${LIBINTL}"
    elif test -e /usr/lib/libintl.dylib || test -e "${prefix}/lib/libintl.dylib"; then
      echo "adding '-lintl' to LIBS_SYSTEM here for darwin"
      LIBS_SYSTEM="-lintl"
    elif test -e /usr/lib/libSystem.dylib; then
      echo "adding '-lSystem' to LIBS_SYSTEM here for darwin"
      LIBS_SYSTEM="-lSystem"
    fi
    ;;
esac
]
dnl# m4-quotation block 7 ends (for LIBS_SYSTEM)

AC_SUBST([LIBRARY_PATH])dnl
AC_ARG_VAR([LIBRARY_PATH],
           [Additional directories for the compiler to search for libraries
            when linking, after any '-L' options.])dnl

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

### The standard library on x86-64 GNU/Linux distributions can 
### be located in either /usr/lib64 or /usr/lib.
case "${canonical}" in
  x86_64-*-linux-gnu* )
    if test -d /usr/lib64; then
      AC_DEFINE([HAVE_X86_64_LIB64_DIR],[1],
        [Define to 1 if the file /usr/lib64 exists.])
fi
esac

#### Types section:
dnl# Suppression of obsolescent Autoconf test for size_t, uid_t, and gid_t
dnl# has been moved to m4/override.m4, which is already included.
test -e m4/override.m4

dnl# other type tests that autoscan wants, among other things:
AC_MSG_CHECKING([types])
AC_MSG_RESULT([ ])
AC_TYPE_INT8_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_OFF_T
AC_TYPE_SIGNAL
AC_TYPE_SSIZE_T
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
if test "x${ac_cv_type_max_align_t}" = "x"; then
  test -z "${ac_cv_type_max_align_t}"
  AC_CHECK_TYPES([max_align_t])dnl
  ## gnulib might also check it below...
fi
if test "x${enable_acl}" != "xno"; then
  AC_CHECK_TYPES([acl_t])dnl
  ## disable if not present?
fi
AC_CHECK_TYPES([ptrdiff_t])dnl
AC_CHECK_TYPES([rlim_t])dnl
AC_CHECK_TYPES([time_t])dnl

dnl# sizes/alignments, in order of what should be the smallest to what
dnl# should be the largest:
AC_CHECK_SIZEOF([char])dnl
AC_CHECK_ALIGNOF([char])dnl
AC_CHECK_SIZEOF([short])dnl
AC_CHECK_ALIGNOF([short])dnl
AC_CHECK_SIZEOF([int])dnl
AC_CHECK_ALIGNOF([int])dnl
AC_CHECK_SIZEOF([long])dnl
AC_CHECK_ALIGNOF([long])dnl
AC_CHECK_SIZEOF([char *])dnl# Notice that it is a pointer this time.
AC_CHECK_ALIGNOF([char *])dnl# Likewise.

if test "x${gl_better_debugging}" = "xyes"; then
  if test "x${ac_cv_sizeof_char_p}" != "x"; then
    test -n "${ac_cv_sizeof_char_p}"
    dnl# see if these will silence some '-Wcast-align' warnings:
    gl_WARN_ADD([-falign-functions=${ac_cv_alignof_char_p}],
                [BETTER_DEBUG_CFLAGS])
    gl_WARN_ADD([-falign-labels=${ac_cv_alignof_char_p}],
                [BETTER_DEBUG_CFLAGS])
    gl_WARN_ADD([-falign-loops=${ac_cv_alignof_char_p}],
                [BETTER_DEBUG_CFLAGS])
    gl_WARN_ADD([-falign-jumps=${ac_cv_alignof_char_p}],
                [BETTER_DEBUG_CFLAGS])
    AC_LANG_PUSH([C++])dnl
    gl_WARN_ADD([-faligned-new=${ac_cv_alignof_char_p}],
                [BETTER_DEBUG_CXXFLAGS])
    AC_SUBST([BETTER_DEBUG_CXXFLAGS])
    AC_LANG_POP
  fi
fi  

dnl# Check for speed_t typedef.
if test "x${ac_cv_header_termios_h}" = "x"; then
  test -z "${ac_cv_header_termios_h}"
  dnl# Need this first though:
  AC_CHECK_HEADERS([termios.h])
fi
AC_CACHE_CHECK([for speed_t],[emacs_cv_speed_t],
  [AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <termios.h>]],
                                      [[speed_t x = 1;]])],
    [emacs_cv_speed_t=yes],[emacs_cv_speed_t=no])])
if test "x${emacs_cv_speed_t}" = "xyes"; then
  AC_DEFINE([HAVE_SPEED_T],[1],
            [Define to 1 if `speed_t' is declared by <termios.h>.])
fi

# sqrt and other floating-point functions such as fmod and frexp
# are found in -lm on many systems.
dnl# (a similar libm check is also done farther below)
OLD_LIBS="${LIBS}"
AC_SEARCH_LIBS([sqrt],[m])
if test "X${LIBS}" = "X${OLD_LIBS}"; then
  LIB_MATH=""
else
  LIB_MATH="${ac_cv_search_sqrt}"
fi
LIBS="${OLD_LIBS}"

AC_MSG_CHECKING([for flag to link with libm])
dnl# libm checks:
LIB_MATH='-lm'
dnl# Current possibilities handled by sed (aix4-2 -> aix,
dnl# gnu-linux -> gnu/linux, etc.):
dnl# gnu, gnu/linux, gnu/kfreebsd, aix, cygwin, darwin, hpux, irix.
dnl# And special cases: berkeley-unix, usg-unix-v, ms-dos, windows-nt.
dnl# m4-quotation block 9 begins:
[
SYSTEM_TYPE="`echo ${opsys} | sed -e 's/[[0-9]].*//' -e 's|-|/|'`"

case ${opsys} in
  cygwin )
    LIB_MATH=""
    LIBM=""
    ;;
  darwin )
    ## Adding -lm confuses the dynamic linker, so omit it.
    LIB_MATH=""
    LIBM=""
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
    LIBM=""
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

m4_ifdef([PKG_PROG_PKG_CONFIG],[
  PKG_PROG_PKG_CONFIG([0.9.0])
  if test "x${PKG_CONFIG}" = "x"; then
    test -z "${PKG_CONFIG}" && export PKG_CONFIG="no"
  fi
],[
AC_PATH_PROG([PKG_CONFIG],[pkg-config],[no])dnl
])dnl# end ifdef

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
  test "x${with_sound}" = "xoss" && test "x${have_sound_header}" != "xyes" && \
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
    dnl# so far this appears to be the 1st call to this macro in this file:
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
  else
    AC_MSG_NOTICE([skipping checks for ALSA sound support])
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
  ##end sound support checks
else
  AC_MSG_NOTICE([actually skipping the sound support checks])
fi

dnl# checks for header files:
AC_MSG_CHECKING([headers])
AC_MSG_RESULT([ ])
dnl# "_ONCE" causes test to be done at wrong location, unfortunately,
dnl# so leave that part off and just check them normally, keeping this
dnl# first batch mostly in their historical order:
AC_CHECK_HEADERS([alloca.h assert.h \
  sys/devinfo.h sys/systeminfo.h sys/inode.h \
  coff.h pty.h sys/pty.h sys/tty.h sys/hft.h \
  sys/resource.h sys/resourcevar.h \
  sys/utsname.h pwd.h utmp.h utmpx.h util.h])dnl
dnl# checks for which it is okay if they jump out of order:
AC_CHECK_HEADERS_ONCE([sys/sysinfo.h])dnl
dnl# Try to keep this second batch in alphabetical order:
AC_CHECK_HEADERS([OS.h arpa/inet.h dpmi.h err.h errno.h fcntl.h float.h \
                  io.h langinfo.h libc.h libintl.h locale.h malloc.h \
                  mbstring.h msvcrt/string.h paths.h nl_types.h sgtty.h \
                  signal.h stddef.h sys/_mbstate_t.h sys/bsdtty.h \
                  sys/file.h sys/ptyio.h sys/signal.h sys/sysctl.h \
                  sys/syslimits.h sys/timeb.h sys/vfs.h sysexits.h \
                  termcap.h termio.h timespec.h unixlib.h])
dnl# (usually I would have checked <malloc/malloc.h> just after <malloc.h>,
dnl# but we do that elsewhere in this one)
if test "x${ac_cv_header_limits_h}" = "x"; then
  test -z "${ac_cv_header_limits_h}"
  AC_CHECK_HEADERS([limits.h])
fi
if test "x${ac_cv_header_stdalign_h}" = "x"; then
  test -z "${ac_cv_header_stdalign_h}"
  AC_CHECK_HEADERS([stdalign.h])
fi
if test "x${ac_cv_header_wctype_h}" = "x"; then
  test -z "${ac_cv_header_wctype_h}"
  AC_CHECK_HEADERS([wctype.h])
fi
dnl# Keep autoscan happy with a dummy condition that is never reached:
if test "x" = "Y0"; then
  test -x "`which autoscan`"
  AC_CHECK_HEADERS([inttypes.h memory.h stdint.h stdio_ext.h string.h \
                    strings.h sys/ioctl.h sys/mman.h sys/param.h \
                    sys/time.h unistd.h utime.h wchar.h])
  ## ...just in case.
fi
if test "x${ac_cv_header_wctype_h}" = "xyes" && test "x${ac_cv_func_wctype}" = "x"; then
  test -n "${ac_cv_header_wctype_h}" && test -z "${ac_cv_func_wctype}"
  AC_CHECK_FUNC([wctype])
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

# Note that Solaris has sys/sysinfo.h which defines struct
# sysinfo as well.  To make sure that we are using GNU/Linux
# sysinfo, we explicitly set one of its fields.
if test "x${ac_cv_header_sys_sysinfo_h}" = "xyes"; then
  AC_MSG_CHECKING([if Linux sysinfo may be used])
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <sys/sysinfo.h>]],
                                     [[struct sysinfo si;
                                       si.totalram = 0;
                                       sysinfo (&si)]])],
    [emacs_cv_linux_sysinfo=yes],[emacs_cv_linux_sysinfo=no])
  AC_MSG_RESULT([${emacs_cv_linux_sysinfo}])
  if test "x${emacs_cv_linux_sysinfo}" = "xyes"; then
    AC_DEFINE([HAVE_LINUX_SYSINFO],[1],
              [Define to 1 if you have Linux sysinfo function.])
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <sys/sysinfo.h>]],
                                       [[struct sysinfo si;
                                         return si.mem_unit]])],
      [AC_DEFINE([LINUX_SYSINFO_UNIT],[1],
                 [Define to 1 if Linux sysinfo sizes are in multiples of mem_unit bytes.])],
      [:])dnl
    ##end check for linux sysinfo
  fi
fi

dnl# On Solaris 8 there is a compilation warning for term.h because
dnl# it does NOT define `bool'.
AC_MSG_CHECKING([for term.h with the preprocessor])
AC_PREPROC_IFELSE([AC_LANG_PROGRAM([[
#include <term.h>
      ]],[[]])],[emacs_cv_preproc_term_h=yes],
                [emacs_cv_preproc_term_h=no])
AC_MSG_RESULT([${emacs_cv_preproc_term_h}])
AC_CHECK_HEADERS([term.h],[],[],[[
#ifdef HAVE_STDBOOL_H
# include <stdbool.h>
#endif /* HAVE_STDBOOL_H */
]])
dnl# header checks with specific macros for them:
AC_HEADER_ASSERT
AC_HEADER_DIRENT
AC_HEADER_MAJOR
AC_HEADER_RESOLV
dnl# definition of this next macro has been moved to the included file:
m4_include([m4/headers_extra.m4])dnl
AC_REQUIRE_VARIOUS_HEADER_CHECKS
if test "x${ac_cv_header_stdbool_h}" = "x"; then
  test -z "${ac_cv_header_stdbool_h}"
  AC_HEADER_STDBOOL
fi
AC_HEADER_TIME
dnl# not sure why these decls go in the headers section, but whatever:
AC_CHECK_DECLS([sys_siglist, sys_signame],[],[],[[#include <signal.h>
			                        ]])
if test "x${ac_cv_have_decl_sys_siglist}" != "xyes"; then
  # For Tru64, at least:
  AC_CHECK_DECLS([__sys_siglist],[],[],[[#include <signal.h>
			               ]])
  # If we have the version with underscores, but not the one without,
  # replace the latter with the former:
  if test "x${ac_cv_have_decl___sys_siglist}" = "xyes" && test "x${ac_cv_have_decl_sys_siglist}" = "xno"; then
    AC_DEFINE([sys_siglist],[__sys_siglist],
              [Define to any substitute for sys_siglist.])
  fi
elif test "x${ac_cv_have_decl_sys_siglist}" = "x"; then
  AC_CHECK_DECLS([sys_siglist])
fi
dnl# I forget about this next one:
AC_CHECK_DECLS([MD5_DIGEST_SIZE])dnl
dnl# well this one at least is here so we can know if we need to check
dnl# for mach headers:
AC_CHECK_DECLS([__MACH__])
if test "x${ac_cv_have_decl___MACH__}" = "xyes" || test -d /usr/include/mach; then
  AC_CHECK_HEADERS([mach/mach.h mach/machine/vm_param.h \
                    mach/processor_info.h mach/vm_region.h])
fi
if test "x${ac_cv_have_decl___MACH__}" = "xyes" || test -d /usr/include/mach-o; then
  ## this is referred to as "documentation" by unexmacosx.c:
  AC_CHECK_HEADERS([mach-o/loader.h])
  ## other possibly useful headers from that directory:
  AC_CHECK_HEADERS([mach-o/arch.h mach-o/compact_unwind_encoding.h \
                    mach-o/dyld.h mach-o/getsect.h mach-o/nlist.h \
                    mach-o/ldsyms.h mach-o/reloc.h])
  if test "x${machine}" = "xmacppc" || test "x${machine}" = "xpowermac"; then
    test -n "${machine}"
    AC_CHECK_HEADERS([mach-o/ppc/reloc.h])
  elif test "x${machine}" = "xarm" || test "x${machine}" = "xacorn"; then
    test -n "${machine}"
    AC_CHECK_HEADERS([mach-o/arm/reloc.h])
  fi
fi
if test "x${enable_acl}" != "xno"; then
  AC_CHECK_DECLS([acl])dnl
  ## disable if not present?
fi

dnl# ...anyways:
if test "x${ac_cv_header_sys_wait_h}" = "x"; then
  test -z "${ac_cv_header_sys_wait_h}"
  AC_HEADER_SYS_WAIT
fi

dnl# adding "_ONCE" causes tests to be done at wrong location, so just call
dnl# the macros regularly, and in a conditional if necessary:
if test "x${ac_cv_header_sys_socket_h}" = "x"; then
  test -z "${ac_cv_header_sys_socket_h}"
  AC_CHECK_HEADERS([sys/socket.h])
fi
AC_CHECK_HEADERS([net/if.h],[],[],[AC_INCLUDES_DEFAULT
#if defined(HAVE_SYS_SOCKET_H) && HAVE_SYS_SOCKET_H
# include <sys/socket.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest for <net/if.h> wants to include <sys/socket.h>"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_SYS_SOCKET_H */
])dnl
AC_CHECK_HEADERS([ifaddrs.h],[],[],[AC_INCLUDES_DEFAULT
#if defined(HAVE_SYS_SOCKET_H) && HAVE_SYS_SOCKET_H
# include <sys/socket.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest for <ifaddrs.h> wants to include <sys/socket.h>"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_SYS_SOCKET_H */
])dnl
AC_CHECK_HEADERS([net/if_dl.h],[],[],[AC_INCLUDES_DEFAULT
#if defined(HAVE_SYS_SOCKET_H) && HAVE_SYS_SOCKET_H
# include <sys/socket.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "The conftest for <net/if_dl.h> wants to include <sys/socket.h>"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_SYS_SOCKET_H */
])dnl

dnl# checks for structures and structure members:
AC_MSG_CHECKING([structures and structure members])
AC_MSG_RESULT([ ])
AC_CHECK_MEMBERS([struct stat.st_rdev])dnl
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
AC_CHECK_MEMBERS([struct processor_set_basic_info.default_policy,
                  struct processor_set_basic_info.load_average,
                  struct processor_set_basic_info.processor_count],[],[],[
AC_INCLUDES_DEFAULT
#ifdef HAVE_MACH_PROCESSOR_INFO_H
# include <mach/processor_info.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__) && defined(__MACH__)
#  warning "this conftest expects <mach/processor_info.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ && __MACH__ */
#endif /* HAVE_MACH_PROCESSOR_INFO_H */
])dnl
dnl# definition of macro for checking timezone structs has been moved to:
m4_include([m4/timezone_structs.m4])dnl
AC_REQUIRE_TIMEZONE_STRUCTS

dnl# GCC has a flag that I think might affect this behavior,
dnl# so check that afterwards:
AC_CACHE_CHECK([for struct exception],[emacs_cv_struct_exception],[
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <math.h>]],[[
static struct exception x;
x.arg1 = x.arg2 = x.retval;
x.name = ""; x.type = 1;
]])],[emacs_cv_struct_exception=yes],[emacs_cv_struct_exception=no])])
HAVE_EXCEPTION="${emacs_cv_struct_exception}"
AC_SUBST([HAVE_STRUCT_EXCEPTION],["${HAVE_EXCEPTION}"])
if test "x${emacs_cv_struct_exception}" != "xyes"; then
  dnl# Specifically, it was the name of this define that made me think that
  dnl# the next check might be related:
  AC_DEFINE([NO_MATHERR],[1],
            [Define to 1 if you do NOT have struct exception in <math.h>.])
  MATHLESS_CFLAGS="${CFLAGS}"
  CFLAGS="${CFLAGS} -fmath-errno"
  AC_CACHE_CHECK([if -fmath-errno will give us struct exception],
                 [emacs_cv_math_errno_struct_exception],[
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#if defined(HAVE_ERRNO_H) || defined(STDC_HEADERS) || defined(__STDC__)
# include <errno.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest expects <errno.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_ERRNO_H || STDC_HEADERS || __STDC__ */
#include <math.h>
  ]],[[
static struct exception x;
x.arg1 = x.arg2 = x.retval;
x.name = ""; x.type = 1;
  ]])],[emacs_cv_math_errno_struct_exception=yes],
       [emacs_cv_math_errno_struct_exception=no])])
  CFLAGS="${MATHLESS_CFLAGS}"
else
  SAFE_MATH_CFLAGS="${CFLAGS}"
  CFLAGS="${CFLAGS} -fno-math-errno"
  AC_CACHE_CHECK([if struct exception still works with -fno-math-errno],
                 [emacs_cv_no_math_errno_struct_exception],[
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#if defined(HAVE_ERRNO_H) || defined(STDC_HEADERS) || defined(__STDC__)
# include <errno.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest expects <errno.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_ERRNO_H || STDC_HEADERS || __STDC__ */
#include <math.h>
  ]],[[
static struct exception x;
x.arg1 = x.arg2 = x.retval;
x.name = ""; x.type = 1;
  ]])],[emacs_cv_no_math_errno_struct_exception=yes],
       [emacs_cv_no_math_errno_struct_exception=no])])
  CFLAGS="${SAFE_MATH_CFLAGS}"
fi

dnl# checks for compiler characteristics:
AC_MSG_CHECKING([the characteristics of your C compiler])
AC_MSG_RESULT([ ])

dnl# Signedness of char can produce some warnings:
AC_C_CHAR_UNSIGNED

dnl# "src/lisp.h" mentions flexible arrays:
if test "x${ac_cv_c_flexmember}" = "x"; then
  test -z "${ac_cv_c_flexmember}"
  AC_C_FLEXIBLE_ARRAY_MEMBER
fi

dnl# Testing __STDC__ to determine prototype support is NOT good enough.
dnl# DEC C, for instance, does NOT define it with default options, and
dnl# is used on 64-bit systems (OSF Alphas).  Similarly for volatile
dnl# and void *.
if test "x${ac_cv_c_prototypes}" = "x"; then
  test -z "${ac_cv_c_prototypes}"
  AC_C_PROTOTYPES
fi
AC_C_BACKSLASH_A
if test "x${ac_cv_c_restrict}" = "x" && test "x${emacs_cv_c_restrict}" = "x" && test "x${emacs_cv_c_restrict_arr}" = "x"; then
  test -z "${ac_cv_c_restrict}" && test -z "${emacs_cv_c_restrict}" test -z "${emacs_cv_c_restrict_arr}"
  AC_C_RESTRICT
fi
AC_C_TYPEOF
AC_C_VOLATILE
if test "x${ac_cv_c_const}" = "x"; then
  test -z "${ac_cv_c_const}"
  AC_C_CONST
fi
dnl# AC_C_INLINE is NOT really useful because we cannot turn on use of
dnl# `inline' unless the compiler groks also `extern inline'. Luckily there
dnl# is a gnulib macro for that. Also we call AC_C_INLINE later below
dnl# anyways.
AC_CACHE_CHECK([for void * support],[emacs_cv_void_star],
  [AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]],[[void *foo;]])],
                     [emacs_cv_void_star=yes],[emacs_cv_void_star=no])])
if test "x${emacs_cv_void_star}" = "xyes"; then
  AC_DEFINE([POINTER_TYPE],[void])
else
  AC_DEFINE([POINTER_TYPE],[char])
fi
AH_TEMPLATE([POINTER_TYPE],
	    [Define as `void' if your compiler accepts `void *'; otherwise
	     define as `char'.])dnl

dnl# Check for endianness.
dnl# This could be used for targets which can have both byte sexes.
dnl# We could presumably replace the hardwired WORDS_BIG_ENDIAN generally.
dnl# AC_C_BIGENDIAN is done by gnulib though, so ifdef it out:
m4_ifdef([gl_00GNULIB],[:],[AC_C_BIGENDIAN])dnl

dnl# check for Make feature:
AC_MSG_CHECKING([features of make])
AC_MSG_RESULT([ ])
dnl# AC_PROG_MAKE_SET is done by Automake, so do it conditionally, also
dnl# allowing for other possible cache values:
if test "x${ac_cv_prog_make_make_set}" = "x" && eval test "x\${ac_cv_prog_make_${ac_make}_set}" = "x"; then
  test -z "${ac_cv_prog_make_make_set}" && eval test -z "\${ac_cv_prog_make_${ac_make}_set}"
  AC_PROG_MAKE_SET
fi

DEPFLAGS=""
MKDEPDIR=":"
if test "x${AM_V}" = "x1" || test "x${V}" = "x1"; then
  MKDEPDIR="echo 'skipping making depdir for \$@.'"
fi
deps_frag=deps.mk
dnl# check for GNU Make if we have GCC and autodepend is on.
if test "x${GCC}" = "xyes" && test "x${ac_enable_autodepend}" = "xyes"; then
   AC_MSG_CHECKING([whether we are using GNU Make])
   HAVE_GNU_MAKE=no
   testval="`${MAKE-make} --version 2>/dev/null | grep 'GNU Make'`"
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
      ##FIXME: also add a check to see what file names this works with...
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
lwlib_deps_frag="${srcdir}/lwlib/${deps_frag}"
oldxmenu_deps_frag="${srcdir}/oldXMenu/${deps_frag}"
## keep this one last, because we overwrite its value, which could mess up
## the previous two:
deps_frag="${srcdir}/src/${deps_frag}"
AC_SUBST([MKDEPDIR])dnl
AC_SUBST([DEPFLAGS])dnl
AC_SUBST_FILE([deps_frag])dnl
AC_SUBST_FILE([lwlib_deps_frag])dnl
AC_SUBST_FILE([oldxmenu_deps_frag])dnl

lisp_frag="${srcdir}/src/lisp.mk"
AC_SUBST_FILE([lisp_frag])dnl

dnl# checks for operating system services:
AC_MSG_CHECKING([operating system services])
AC_MSG_RESULT([ ])
AC_SYS_LONG_FILE_NAMES
m4_ifdef([AC_SYS_RESTARTABLE_SYSCALLS],[
  dnl# This takes a while to execute, due to calls to sleep:
  if test "x${I_CAN_AFFORD_TO_WAIT_AROUND}" = "xyes"; then
    test -n "${I_CAN_AFFORD_TO_WAIT_AROUND}"
    AC_SYS_RESTARTABLE_SYSCALLS
  fi
])dnl
dnl# this one is kind of a different type of "service", but whatever:
AC_LANG_PUSH([Objective C])dnl
ORIG_LDFLAGS="${LDFLAGS}"
if test "x${with_ns}" != "xno"; then
  if test "x${opsys}" = "xdarwin" && test "x${emacs_cv_apple}" = "xyes" && test "x${host_vendor}" = "xapple" && test "x${OBJC}" != "x"; then
    test -n "${OBJC}"
    ## FIXME: test this better:
    LDFLAGS="${LDFLAGS} -framework AppKit"
  elif test "x${ac_cv_header_appkit_appkit_h}" = "xyes"; then
    APPKIT_FRAMEWORK="/System/Library/Frameworks/Appkit.framework"
    if test "x${xcsdkdir}" != "x"; then
      test -d "${xcsdkdir}" && export APPKIT_FRAMEWORK="${xcsdkdir}/System/Library/Frameworks/Appkit.framework"
    elif test "x${SDKROOT}" != "x"; then
      test -d "${SDKROOT}" && export APPKIT_FRAMEWORK="${SDKROOT}/System/Library/Frameworks/Appkit.framework"
    else
      APPKIT_FRAMEWORK="/System/Library/Frameworks/Appkit.framework"
    fi
    AC_PATH_PROGS([LOCATE_BIN],[locate glocate])
    AC_PATH_PROGS([HEAD_BIN],[head ghead])
    if test "x${LOCATE_BIN}" != "x" && test "x${HEAD_BIN}" != "x"; then
      test -x "${LOCATE_BIN}" && test -x "${HEAD_BIN}" && export APPKIT_FRAMEWORK="`${LOCATE_BIN} '/Library/Frameworks/AppKit.framework' 2>/dev/null | ${HEAD_BIN} -n 1`"
    fi
    if test -d "${APPKIT_FRAMEWORK}"; then
      LDFLAGS="${LDFLAGS} -framework AppKit"
    fi
  else
    AC_CHECK_LIB([gnustep-gui],[main],[LDFLAGS="${LDFLAGS} -lgnustep-gui"])
    AC_CHECK_LIB([gnustep-base],[main],
                 [LDFLAGS="${LDFLAGS} -lgnustep-base"])
    AC_CHECK_LIB([objc],[main],[LDFLAGS="${LDFLAGS} -lobjc"],
      [AC_CHECK_LIB([objc-gnu],[main],[LDFLAGS="${LDFLAGS} -lobjc-gnu"])])
  fi
fi
AC_CHECK_FUNCS([NSPerformService])dnl
LDFLAGS="${ORIG_LDFLAGS}"
AC_SUBST([ORIG_LDFLAGS])dnl
AC_LANG_POP

#### Choose a window system.
AC_MSG_CHECKING([windowing system])
AC_MSG_RESULT([ ])dnl

## We leave window_system equal to none if
## we end up building without one. Any new window system should
## set window_system to an appropriate value and add objects to
## window-system-specific substs.

window_system=none
dnl# these macros are now fixed in m4/x11.m4:
AC_PATH_X
AC_PATH_XTRA
if test "x${no_x}" != "xyes" && test "x${have_x}" = "xyes" && test "x${with_x}" != "xno"; then
  window_system=x11
  AC_MSG_NOTICE([it looks like we are using x11 for the window system at this point])
fi

dnl# actually it might be a bad idea to m4-quote this next block
dnl# (which was temporarily block number 10), as it looks like it already
dnl# had some quotation in it to begin with...
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
dnl# m4-quotation block 10 would end here, if we were still doing that

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
    bitmapdir="`echo ${bmd_acc} | sed -e "s/^dummyval://"`"
  fi
fi
]
dnl# m4-quotation block 11 ends

## same as "HAVE_MACGUI" in Yamamoto Mitsuharu's Mac port:
HAVE_CARBON=no
## "${with_carbon}" is "${with_mac}" in Yamamoto Mitsuharu's Mac port:
if test "x${with_carbon}" != "xno"; then
  AC_CHECK_HEADER([Carbon/Carbon.h],[HAVE_CARBON=yes])
  AC_CHECK_HEADERS([CarbonCore/Files.h CarbonCore/MacTypes.h \
                    CarbonCore/Script.h CarbonCore/StringCompare.h \
                    CarbonCore/TextUtils.h HIToolbox/Appearance.h \
                    HIToolbox/CarbonEvents.h HIToolbox/Controls.h \
                    HIToolbox/Dialogs.h HIToolbox/Drag.h \
                    HIToolbox/Events.h HIToolbox/HIButtonViews.h \
                    HIToolbox/HIImageViews.h HIToolbox/HIProgressViews.h \
                    HIToolbox/HITextViews.h HIToolbox/MacWindows.h \
                    HIToolbox/Menus.h HIToolbox/Scrap.h MacTypes.h \
                    NavigationServices/Navigation.h \
                    QD/QuickdrawAPI.h QD/QDOffscreen.h \
                    QuickTime/ImageCompression.h])dnl
  ## end Carbon headers check
fi
if test "x${window_system}" = "xx11" && test "x${HAVE_CARBON}" = "xyes"; then
  if test "x${with_carbon+set}" != "xset" \
     && test "${carbon_appdir_x+set}" != "xset"; then
    for var in with_x with_x_toolkit with_gtk with_xim \
               with_xpm with_jpeg with_tiff with_gif with_png; do
      if eval test \"\${$var+set}\" = set; then
        HAVE_CARBON=no
        break
      fi
    done
  fi
fi
LIBS_CARBON=""
MAC_OBJ=""
MAC_OBJC_OBJ=""
if test "x${HAVE_CARBON}" = "xyes"; then
  window_system=mac
  ## this header is needed for the conftest that follows after it:
  AC_CHECK_HEADERS([AvailabilityMacros.h])dnl
  # Check if the Quartz Core framework is necessary and always
  # available at runtime:
  AC_MSG_CHECKING([Mac OS X version max allowed >= 10.5])
  AC_CACHE_VAL([emacs_cv_mac_os_x_max_allowed_10_5],
     [AC_LINK_IFELSE([AC_LANG_PROGRAM([[
#ifdef HAVE_AVAILABILITYMACROS_H
# include <AvailabilityMacros.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__) && defined(__APPLE__)
#  warning "This conftest expects <AvailabilityMacros.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ && __APPLE__ */
#endif /* HAVE_AVAILABILITY_MACROS_H */
]],[[
#if MAC_OS_X_VERSION_MAX_ALLOWED >= 1050 && MAC_OS_X_VERSION_MIN_REQUIRED > 1020
# error "OSX 10.5 not allowed"
fail;
#endif
]])],[emacs_cv_mac_os_x_max_allowed_10_5=no], [emacs_cv_mac_os_x_max_allowed_10_5=yes])])dnl
  ## FIXME: is this redundant with the similar test in the NS case below?
  if test "x${emacs_cv_mac_os_x_max_allowed_10_5}" = "xyes"; then
    AC_MSG_RESULT([10.5 or later])
    libs_quartz_core="-framework QuartzCore"
  else
    AC_MSG_RESULT([before 10.5])
    libs_quartz_core=""
  fi
  ## used in libs_nsgui (i.e. libs_macgui) farther below:
  export LIBS_CARBON="-framework Carbon -framework WebKit -framework Quartz ${libs_quartz_core} -framework IOKit -framework OSAKit -framework Cocoa -framework QuickTime -framework CoreFoundation"
  export MAC_OBJ="mac.o macterm.o macfns.o macmenu.o macselect.o fontset.o fringe.o image.o"
  export MAC_OBJC_OBJ="macappkit.o macfont.o"
  AC_MSG_NOTICE([it looks like we are using Carbon for the window system at this point])
  case "${canonical}" in
    *64-*-darwin9.* | *64-*-darwin1[[012345]].*)
      NON_CARBON_LDFLAGS="${LDFLAGS}"
      LDFLAGS="${LDFLAGS} ${LIBS_CARBON} -m64"
      AC_CACHE_CHECK([if Carbon still works when compiling 64-bit],
                     [emacs_cv_carbon_sixtyfourbit],[
AC_LINK_IFELSE([AC_LANG_PROGRAM([[
#ifndef __LP64__
# define __LP64__ 1
#endif /* !__LP64__ */
#include <Carbon/Carbon.h>
#ifdef HAVE_SYS_PARAM_H
# include <sys/param.h>
#else
# ifndef MAXPATHLEN
#  ifdef PATH_MAX
#   define MAXPATHLEN PATH_MAX
#  else
#   define MAXPATHLEN 1024
#  endif /* PATH_MAX */
# endif /* !MAXPATHLEN */
#endif /* HAVE_SYS_PARAM_H */
static char in_string[MAXPATHLEN + 1] = "/System";
]],[[
/* call a function known to only work in 32-bit mode: */
char *out_string = p2cstr((StringPtr)in_string);
printf("%s", out_string);
]])],
[emacs_cv_carbon_sixtyfourbit=yes],[emacs_cv_carbon_sixtyfourbit=no])
      ])
      LDFLAGS="${NON_CARBON_LDFLAGS}"
      AC_SUBST([NON_CARBON_LDFLAGS])dnl
      ##FIXME: see also if it is possible to force 32-bit compilation,
      ## if the above fails (which it should)...
    ;;
  esac
fi
AC_SUBST([LIBS_CARBON])dnl
AC_SUBST([MAC_OBJ])dnl
AC_SUBST([MAC_OBJC_OBJ])dnl

HAVE_NS="no"
NS_GNUSTEP_CONFIG="no"
NS_IMPL=""
NS_IMPL_COCOA="no"
NS_IMPL_GNUSTEP="no"
tmp_CPPFLAGS="${CPPFLAGS}"
tmp_CFLAGS="${CFLAGS}"
CPPFLAGS="${CPPFLAGS} -x objective-c"
CFLAGS="${CFLAGS} -x objective-c"
GNU_OBJC_CFLAGS=""
LIBS_GNUSTEP=""
if test "x${with_ns}" != "xno"; then
  if test "x${opsys}" = "xdarwin"; then
     NS_IMPL=Cocoa
     NS_IMPL_COCOA=yes
     ns_appdir="`pwd`/nextstep/Emacs.app"
     ns_appbindir="${ns_appdir}/Contents/MacOS"
     ns_appresdir="${ns_appdir}/Contents/Resources"
     ns_appsrc=Cocoa/Emacs.base
     ns_fontfile=macfont.o
     dnl# always use exceptions on darwin; no need to test:
     GNU_OBJC_CFLAGS="-fobjc-exceptions"
     dnl# check for blocks support:
     DISPATCH_C_BLOCKS
     GNU_OBJC_CFLAGS="${GNU_OBJC_CFLAGS} ${CBLOCKS_FLAGS}"
     if test "x${have_cblocks}" = "xfalse"; then
       AC_MSG_WARN([the Objective C sources are better-tested with cblocks, but we do not have them.])
     else
       AC_DEFINE([HAVE_CBLOCKS_LANGUAGE_FEATURE],[1],
                 [Define to 1 if we can use cblocks in our code])
     fi
  elif flags=$( (gnustep-config --objc-flags) 2>/dev/null); then
     NS_IMPL_GNUSTEP=yes
     NS_GNUSTEP_CONFIG=yes
     GNU_OBJC_CFLAGS="${flags}"
     LIBS_GNUSTEP=$(gnustep-config --gui-libs) || exit
  elif test -f ${GNUSTEP_CONFIG_FILE}; then
     NS_IMPL=GNUstep
     NS_IMPL_GNUSTEP=yes
     ns_appdir="`pwd`/nextstep/Emacs.app"
     ns_appbindir="${ns_appdir}"
     ns_appresdir="${ns_appdir}/Resources"
     ns_appsrc=GNUstep/Emacs.base
     ns_fontfile=nsfont.o
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
  if test "x${NS_IMPL_GNUSTEP}" = "xyes"; then
     ns_appdir=`pwd`/nextstep/Emacs.app
     ns_appbindir="${ns_appdir}"
     ns_appresdir="${ns_appdir}/Resources"
     ns_appsrc=GNUstep/Emacs.base
     ns_fontfile=nsfont.o
  fi
elif test "x${MAC_OBJC_OBJ}" != "x"; then
  test -n "${MAC_OBJC_OBJ}"
  if test "x${opsys}" = "xdarwin"; then
     test -z "${GNU_OBJC_CFLAGS}" && GNU_OBJC_CFLAGS="-fobjc-exceptions"
     GNU_OBJC_CFLAGS="${GNU_OBJC_CFLAGS} -fno-strict-aliasing"
     test -z "${GNUSTEP_CFLAGS}" && GNUSTEP_CFLAGS="-fno-strict-aliasing"
  fi
fi  ##end ns conditional

if test "x${with_ns}" != "xno" || test "x${MAC_OBJC_OBJ}" != "x"; then
  test -n "${NS_IMPL}" || test -n "${MAC_OBJC_OBJ}"
  if test "x${gl_gcc_warnings}" = "xyes" && test "x${WERROR_CFLAGS}" = "x"; then
    AC_LANG_PUSH([Objective C])dnl
    gl_WARN_ADD([-Wassign-intercept],[GNU_OBJC_WARN_CFLAGS])
    gl_WARN_ADD([-Wcast-align],[GNU_OBJC_WARN_CFLAGS])
    gl_WARN_ADD([-Wglobal-constructors],[GNU_OBJC_WARN_CFLAGS])
    gl_WARN_ADD([-Wproperty-assign-default],[GNU_OBJC_WARN_CFLAGS])
    gl_WARN_ADD([-Wprotocol],[GNU_OBJC_WARN_CFLAGS])
    gl_GCC_VERSION_IFELSE([5],[0],[
      gl_WARN_ADD([-Wshadow-ivar],[GNU_OBJC_WARN_CFLAGS])
    ])dnl
    ## these two selector-related warnings can be overly noisy:
    if test "x${I_AM_GOOD_AT_SILENCING_WARNINGS}" = "xyes"; then
      test -n "${I_AM_GOOD_AT_SILENCING_WARNINGS}"
      gl_WARN_ADD([-Wselector],[GNU_OBJC_WARN_CFLAGS])
      gl_WARN_ADD([-Wstrict-selector-match],[GNU_OBJC_WARN_CFLAGS])
    else
      test -z "${I_AM_GOOD_AT_SILENCING_WARNINGS}"
      nw="${nw} -Wselector"
      nw="${nw} -Wstrict-selector-match"
    fi
    ## this selector-related warning is perfectly fine though:
    gl_WARN_ADD([-Wundeclared-selector],[GNU_OBJC_WARN_CFLAGS])
    ## some Objective C compilers have buggy support for method attributes:
    nw="${nw} -Wmissing-noreturn"
    gl_WARN_ADD([-Wno-missing-noreturn],[GNU_OBJC_WARN_CFLAGS])
    ## sometimes a built-in string gets generated that triggers this:
    nw="${nw} -Woverlength-strings"
    gl_WARN_ADD([-Wno-overlength-strings],[GNU_OBJC_WARN_CFLAGS])
    ## in case the C compiler forgot to add it:
    if test -z "${emacs_g3_OBJCFLAGS}"; then
      gl_WARN_ADD([-Wundef],[GNU_OBJC_WARN_CFLAGS])
    fi
    AC_SUBST([GNU_OBJC_WARN_CFLAGS])dnl
    AC_LANG_POP
  elif test "x${gl_gcc_warnings}" = "xno"; then
    GNU_OBJC_WARN_CFLAGS=""
  fi

  dnl# make sure the warning flags get included in the more-general version
  dnl# of the variable, which is substed in more places:
  GNU_OBJC_CFLAGS="${GNU_OBJC_CFLAGS} ${GNU_OBJC_WARN_CFLAGS}"
fi  ##end stuff for objc sources, whether ns or mac

if test "x${with_ns}" != "xno"; then
  dnl# This is only used while we test the NS headers, it gets reset below:
  CPPFLAGS="${CPPFLAGS} ${GNU_OBJC_CFLAGS}"
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
    dnl# wrong location, so instead check the cache value:
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
    AC_MSG_CHECKING([for OSX 10.6 or newer])
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <AppKit/AppKit.h>]],
                                       [[
#ifdef MAC_OS_X_VERSION_MAX_ALLOWED
# if MAC_OS_X_VERSION_MAX_ALLOWED >= 1060
 ; /* OK */
# else
#  error "OSX 10.6 not found"
# endif /* 10.6+ */
#endif /* MAC_OS_X_VERSION_MAX_ALLOWED */
		    ]])],
		    [ns_osx_have_106=yes],
		    [ns_osx_have_106=no])
    AC_MSG_RESULT([${ns_osx_have_106}])
    if test "x${ns_osx_have_106}" = "xno"; then
       AC_MSG_WARN([`OSX 10.6 or newer is required in mainline trunk, where this is an error (downgraded to a warning in this fork though)']);
    fi
  fi
  dnl# "_ONCE" causes test to jump out of the conditional and be done at
  dnl# wrong location, so leave that part off and just call it normally:
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
  if test "x${opsys}" = "xdarwin" && test "x${host_vendor}" = "xapple" && test "x${OBJC}" != "x" && test "x${emacs_cv_apple}" = "xyes" && test "x${have_cblocks}" = "xtrue"; then
    test -n "${OBJC}"
    AC_MSG_WARN(['--with-ns' is recommended for darwin, but it was not specified.])
  fi
fi  ##end ns-only checks

# if we never tested these, then assume the worst:
if test -z "${HAVE_CBLOCKS_TRUE}" && test -z "${HAVE_CBLOCKS_FALSE}"; then
  HAVE_CBLOCKS_TRUE='#'
  HAVE_CBLOCKS_FALSE=''
fi
if test -z "${HAVE_CXXBLOCKS_TRUE}" && test -z "${HAVE_CXXBLOCKS_FALSE}"; then
  HAVE_CXXBLOCKS_TRUE='#'
  HAVE_CXXBLOCKS_FALSE=''
fi

if test "x${have_cblocks}" = "xtrue" || test "x${CBLOCKS_FLAGS}" != "x"; then
  test -n "${have_cblocks}"
  BLOCKLESS_CFLAGS="${CFLAGS}"
  CFLAGS="${CFLAGS} ${CBLOCKS_FLAGS}"
  AC_CHECK_HEADERS([dispatch/dispatch.h dispatch/group.h])
  AC_CHECK_FUNCS([dispatch_group_async])
  CFLAGS="${BLOCKLESS_CFLAGS}"
else
  ac_cv_header_dispatch_dispatch_h=no
  ac_cv_header_dispatch_group_h=no
  ac_cv_func_dispatch_group_async=no
  AC_MSG_NOTICE([skipping tests for things that require cblocks support])
fi
dnl# now subst from before that:
AC_SUBST([LIBS_GNUSTEP])dnl
AC_SUBST([NS_IMPL])dnl

INSTALL_ARCH_INDEP_EXTRA=install-etc
ns_self_contained=no
NS_OBJ=""
NS_OBJC_OBJ=""
if test "x${HAVE_NS}" = "xyes"; then
  test "x${window_system}" = "xmac" ## disable Carbon/MACGUI based on this?
  if test "x${with_toolkit_scroll_bars}" = "xno"; then
    AC_MSG_ERROR([Non-toolkit scroll bars are not implemented for Nextstep.])
  fi

  window_system=nextstep
  AC_MSG_NOTICE([it looks like we are using a Nextstep-style window system at this point])
  # set up packaging dirs:
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
     AC_DEFINE([NS_SELF_CONTAINED_IS_TRUE],[1],
               [Define to 1 if the Nextstep application is self-contained])
  fi

  NS_OBJC_OBJ="nsterm.o nsfns.o nsmenu.o nsselect.o nsimage.o nsfont.o"
  if test "x${ns_fontfile}" != "x"; then
    test -n "${ns_fontfile}" && NS_OBJC_OBJ="${NS_OBJC_OBJ} ${ns_fontfile}"
  elif test "x${macfont_file}" != "x"; then
    test -n "${macfont_file}" && export NS_OBJC_OBJ="${NS_OBJC_OBJ} ${macfont_file}"
  else
    test -z "${ns_fontfile}" && test -z "${macfont_file}"
    AC_MSG_WARN([could be missing required file for NS/Mac fonts])
  fi
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
      ## Using --with-w32 with MinGW is basically a no-op, but we allow it.
      AC_CHECK_HEADERS([basetsd.h commctrl.h direct.h winbase.h wincon.h \
                        windef.h winnt.h winresrc.h winuser.h winuser.rh])
    ;;
    *)
      AC_MSG_ERROR([Using w32 with an autotools build is only supported for Cygwin and MinGW32.])
    ;;
  esac
fi

if test "x${opsys}" = "xmingw32"; then
  AC_CHECK_HEADERS([usp10.h])
  if test "x${ac_cv_header_windows_h}" = "x"; then
    test -z "${ac_cv_header_windows_h}"
    AC_CHECK_HEADERS([windows.h])
  fi
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
  AC_MSG_NOTICE([it looks like we are using the w32 window system at this point])
  with_xft=no
fi

## ${window_system} is now set to the window system we will
## ultimately use.
AC_MSG_CHECKING([final window system that we will ultimately use])
AC_MSG_RESULT([going with ${window_system} for real now])

if test "x${window_system}" = "xnone" && test "x${gl_gcc_warnings}" = "xyes"; then
   # Too many warnings for now.
   nw=""
   nw="${nw} -Wmissing-format-attribute"
   nw="${nw} -Wmissing-noreturn"
   nw="${nw} -Wsuggest-attribute"
   nw="${nw} -Wsuggest-attribute=const"
   nw="${nw} -Wsuggest-attribute=noreturn"
   gl_MANYWARN_COMPLEMENT([WARN_CFLAGS],[${WARN_CFLAGS}],[${nw}])dnl

   nw="${nw} -Wunused-variable"
   nw="${nw} -Wunused-but-set-variable"
   nw="${nw} -Wunused-but-set-parameter"
   gl_WARN_ADD([-Wno-unused-variable])
   gl_WARN_ADD([-Wno-unused-but-set-variable])
   gl_WARN_ADD([-Wno-unused-but-set-parameter])
   if test -z "${emacs_g3_CFLAGS}" || test "x${emacs_cv_prog_cc_g3}" != "xyes" || test -z "${emacs_g3_OBJCFLAGS}"; then
      test -z "${emacs_cv_prog_cc_g3}"
      gl_WARN_ADD([-Wundef])
   fi  ##end check for '-g3' flag
fi  ##end check for no windowing system, plus warnings

AC_MSG_CHECKING([initial X toolkit to go with])
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
      open-look ) USE_X_TOOLKIT=OPEN_LOOK ;;
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
  mac )
    term_header=macterm.h
  ;;
  none )
    HAVE_X_WINDOWS=no
    HAVE_X11=no
    USE_X_TOOLKIT=none
  ;;
esac
AC_MSG_RESULT(["${USE_X_TOOLKIT}"])dnl

### If we are using X11, then we should use the X menu package:
HAVE_MENUS=no
if test "x${HAVE_X11}" != "x"; then
  test -n "${HAVE_X11}"
  case ${HAVE_X11} in
    yes ) HAVE_MENUS=yes ;;
  esac
fi

if test "x${opsys}" = "xhpux9"; then
  case "${x_libraries}" in
    *X11R4* )
      opsysfile="s/hpux9-x11r4.h"
      ;;
  esac
fi

if test "x${opsys}" = "xhpux9shr"; then
  case "${x_libraries}" in
    *X11R4* )
      opsysfile="s/hpux9shxr4.h"
      ;;
  esac
fi

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
else
   AC_MSG_NOTICE([skipping additional X server checks])
fi

# Does the opsystem file prohibit the use of the GNU malloc?
# Assume not, until told otherwise.
AC_MSG_CHECKING([malloc characteristics (GNU, system, or hybrid)])
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

hybrid_malloc=""

case "${opsys}" in
  ## darwin ld insists on the use of malloc routines in the System
  ## framework, and other systems do similar things:
  darwin | mingw32 | nacl | sol2-10) system_malloc=yes ;;
  cygwin) hybrid_malloc=yes;;
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
elif test "x${hybrid_malloc}" = "xyes"; then
  AC_DEFINE([HYBRID_MALLOC],[1],
    [Define to use gmalloc before dumping and the system malloc after.])
  GNU_MALLOC=""
  GNU_MALLOC_reason="only before dumping"
  GMALLOC_OBJ=gmalloc.o
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
    mingw32|gnu*) REL_ALLOC=no ;;
  esac
fi

if test x"${REL_ALLOC}" = x""; then
  REL_ALLOC="${GNU_MALLOC}"
fi

use_mmap_for_buffers=no
case "${opsys}" in
  cygwin|mingw32|freebsd|irix6-5) use_mmap_for_buffers=yes ;;
esac

AC_FUNC_MMAP
if test "x${use_mmap_for_buffers}" = "xyes"; then
  AC_DEFINE([USE_MMAP_FOR_BUFFERS],[1],
            [Define to use mmap to allocate buffer text.])
  REL_ALLOC=no
fi

dnl# misc. libraries section:
AC_MSG_CHECKING([other libraries])
AC_MSG_RESULT([ ])
LIBS="${LIBS_SYSTEM} ${LIBS}"

dnl# what AC\_ISC\_POSIX autoupdates to:
AC_SEARCH_LIBS([strerror],[cposix])dnl

dnl# part of what AC\_IRIX\_SUN autoupdates to:
AC_CHECK_LIB([sun],[getpwnam])dnl

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

dnl# Some old versions might have put cuserid in libcompat; if not, check
dnl# other places:
AC_SEARCH_LIBS([cuserid],[compat c System gnu])dnl

dnl# Check if pthreads is available.
AC_MSG_CHECKING([pthreads characteristics])
AC_MSG_RESULT([ ])
LIB_PTHREAD=""
dnl# "_ONCE" causes test to be done at wrong location, so leave that off:
AC_CHECK_HEADERS([pthread.h])
if test "x${ac_cv_header_pthread_h}" != "x" && test "x${ac_cv_header_pthread_h}" != "xno"; then
  test -n "${ac_cv_header_pthread_h}"
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

AC_MSG_CHECKING([for XFree86 in /usr/X386])
if test -d /usr/X386/include; then
  HAVE_XFREE386=yes
  : ${C_SWITCH_X_SITE="-I/usr/X386/include"}
else
  HAVE_XFREE386=no
fi
AC_MSG_RESULT([${HAVE_XFREE386}])

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
                   [gdb_cv_bigtoc=""])dnl
  ])dnl
  ##end AIX case
  ;;
esac

# Change CFLAGS, CPPFLAGS, LDFLAGS, and LIBS temporarily so that
# C_SWITCH_X_SITE is used for the tests that follow.  We set them back to
# REAL_CFLAGS, REAL_CPPFLAGS, REAL_LDFLAGS and REAL_LIBS later on.

REAL_CFLAGS="${CFLAGS}"
REAL_CPPFLAGS="${CPPFLAGS}"
REAL_LDFLAGS="${LDFLAGS}"
REAL_LIBS="${LIBS}"
AC_SUBST([REAL_CFLAGS])dnl
AC_SUBST([REAL_CPPFLAGS])dnl
AC_SUBST([REAL_LDFLAGS])dnl
AC_SUBST([REAL_LIBS])dnl

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
  else
    AC_MSG_NOTICE([skipping X11 checks that are only relevant for GNU-Linux systems])
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

  dnl# check some misc. X funcs:
  AC_CHECK_FUNCS([XrmSetDatabase XScreenResourceString \
                  XScreenNumberOfScreen XSetWMProtocols])dnl
  dnl# and also these:
  AC_CHECK_FUNCS([check_x_display_info x_get_arg x_get_string_resource])dnl
  dnl# and some misc. X headers, if not yet checked:
  ## use cache value as guard:
  if test "x${ac_cv_header_X11_X_h}" = "x"; then
    test -z "${ac_cv_header_X11_X_h}"
    AC_CHECK_HEADER([X11/X.h])
  fi
else
  AC_MSG_NOTICE([skipping miscellaneous X11 checks])
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
      sol2-*) AC_MSG_WARN([Solaris has a bug with X11R6-style XIM support])
         ;;
      *) AC_DEFINE([HAVE_X11R6_XIM],[1],
           [Define if you have usable X11R6-style XIM support.])
         ;;
    esac
  else
    AC_MSG_RESULT([before 6])
  fi
else
  AC_MSG_NOTICE([skipping check to see if X11 is version 6 or newer])
fi

if test "x${window_system}" = "xx11"; then
  AC_MSG_CHECKING([X11 version 5])
  AC_CACHE_VAL([emacs_cv_x11_version_5],
  [AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <X11/Xlib.h>]],
[[#if XlibSpecificationRelease < 5
fail;
#endif /* XlibSpecificationRelease < 5 */
]])],[emacs_cv_x11_version_5=yes],[emacs_cv_x11_version_5=no])])
  if test "x${emacs_cv_x11_version_5}" = "xyes"; then
    AC_MSG_RESULT([5 or newer])
    HAVE_X11R5=yes
    AC_DEFINE([HAVE_X11R5],[1],
	      [Define to 1 if you have the X11R5 or newer version of Xlib])
  else
    HAVE_X11R5=no
    AC_MSG_RESULT([before 5])
  fi
else
  AC_MSG_NOTICE([skipping check to see if X11 is version 5 or newer])
fi

AC_MSG_CHECKING([graphics packages])
AC_MSG_RESULT([ ])
### Use -lrsvg-2 if available, unless `--with-rsvg=no' is specified.
HAVE_RSVG=no
if test "x${HAVE_X11}" = "xyes" || test "x${HAVE_NS}" = "xyes" || test "x${opsys}" = "xmingw32"; then
  if test "x${with_rsvg}" != "xno"; then
    RSVG_REQUIRED=2.11.0
    RSVG_MODULE="librsvg-2.0 >= ${RSVG_REQUIRED}"

    PKG_CHECK_MODULES([RSVG],[${RSVG_MODULE}],[HAVE_RSVG=yes],[:])dnl
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
  else
    AC_MSG_NOTICE([skipping checks for librsvg])
  fi
else
  AC_MSG_NOTICE([skipping checks for librsvg])
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
    ## imagemagick sometimes has '-fopenmp' in its CFLAGS, but some
    ## compilers (such as clang) cannot handle this flag, so check and then
    ## remove it if necessary:
    if test "x${ac_cv_prog_c_openmp}" = "x"; then
      test -z "${ac_cv_prog_c_openmp}"
      AC_OPENMP
    fi
    if test "x${emacs_cv_clang}" = "xyes" || test "x${ac_cv_prog_c_openmp}" = "xunsupported"; then
      AC_MSG_NOTICE([removing any instances of -fopenmp from IMAGEMAGICK_CFLAGS])
      IMAGEMAGICK_CFLAGS="`echo ${IMAGEMAGICK_CFLAGS} | sed 's|-fopenmp||g'`"
    fi
    AC_SUBST([IMAGEMAGICK_CFLAGS])dnl
    AC_SUBST([IMAGEMAGICK_LIBS])dnl

    if test "x${HAVE_IMAGEMAGICK}" = "xyes"; then
      AC_DEFINE([HAVE_IMAGEMAGICK],[1],[Define to 1 if using imagemagick.])
      CFLAGS="${CFLAGS} ${IMAGEMAGICK_CFLAGS}"
      LIBS="${IMAGEMAGICK_LIBS} ${LIBS}"
      AC_CHECK_FUNCS([MagickExportImagePixels MagickMergeImageLayers])
    else
      AC_MSG_NOTICE([we do not have imagemagick so we will do no further checks for it.])
    fi
  else
    AC_MSG_NOTICE([skipping checks for imagemagick])
  fi
else
  AC_MSG_NOTICE([skipping checks for imagemagick])
fi

dnl# section for gtk environment:
AC_MSG_CHECKING([gtk features])
AC_MSG_RESULT([ ])
HAVE_GTK=no
if test "x${with_gtk}" = "xyes" && test "x${USE_X_TOOLKIT}" = "xgtk"; then
  USE_X_TOOLKIT=none
fi
GTK_OBJ=""
gtk_term_header="${term_header}"
check_gtk2=no
gtk3_pkg_errors=""
if test "x${opsys}" != "xmingw32"; then
  if test "x${with_gtk3}" = "xyes" || test "x${with_gtk}" = "xyes" || test "x${USE_X_TOOLKIT}" = "xmaybe" || test "x${USE_X_TOOLKIT}" = "xgtk"; then
    if test "x${USE_X_TOOLKIT}" != "xnone" && test "x${USE_X_TOOLKIT}" != "xmaybe"; then
      AC_MSG_ERROR([Conflicting options, --with-gtk is incompatible with --with-x-toolkit=${with_x_toolkit}]);
    fi
    GLIB_REQUIRED=2.28
    GTK_REQUIRED=3.0
    GTK_MODULES="gtk+-3.0 >= ${GTK_REQUIRED} glib-2.0 >= ${GLIB_REQUIRED}"

    dnl# Check if --with-pkg-config-prog has been given.
    if test "X${with_pkg_config_prog}" != "X" && test "X${PKG_CONFIG}" = "X"; then
      test -n "${with_pkg_config_prog}" && test -z "${PKG_CONFIG}" && export PKG_CONFIG="${with_pkg_config_prog}"
    fi
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
       AC_MSG_NOTICE([gtk3 is unavailable so we shall try gtk2 next])
       check_gtk2=yes
       gtk3_pkg_errors="${GTK_PKG_ERRORS} "
    fi
  else
    AC_MSG_NOTICE([skipping initial gtk3 checks])
  fi

  if test "x${with_gtk2}" = "xyes" || test "x${check_gtk2}" = "xyes"; then
    if test "x${USE_X_TOOLKIT}" != "xnone" && test "x${USE_X_TOOLKIT}" != "xmaybe"; then
      AC_MSG_ERROR([Conflicting options, --with-gtk is incompatible with --with-x-toolkit=${with_x_toolkit}]);
    fi
    GLIB_REQUIRED=2.10
    GTK_REQUIRED=2.10
    GTK_MODULES="gtk+-2.0 >= ${GTK_REQUIRED} glib-2.0 >= ${GLIB_REQUIRED}"

    dnl# Checks for libraries:
    PKG_CHECK_MODULES([GTK],[${GTK_MODULES}],
                      [pkg_check_gtk=yes],[pkg_check_gtk=no])
    if test "x${pkg_check_gtk}" = "xno" &&
       { test "x${with_gtk}" = "xyes" || test "x${with_gtk2}" = "xyes"; }
    then
      AC_MSG_ERROR([${gtk3_pkg_errors}${GTK_PKG_ERRORS}])
    fi
    test "x${pkg_check_gtk}" = "xyes" && export USE_GTK_TOOLKIT="GTK2"
  else
    AC_MSG_NOTICE([skipping initial gtk2 checks])
  fi
else
  AC_MSG_NOTICE([skipping initial gtk checks])
fi

if test x"${pkg_check_gtk}" = x"yes"; then
  AC_SUBST([GTK_LIBS])dnl

  C_SWITCH_X_SITE="${C_SWITCH_X_SITE} ${GTK_CFLAGS}"
  CFLAGS="${CFLAGS} ${GTK_CFLAGS}"
  CPPFLAGS="${CPPFLAGS} ${GTK_CFLAGS}"
  LIBS="${GTK_LIBS} ${LIBS}"
  dnl# Try to compile a simple GTK program, after checking the headers that
  dnl# it needs:
  AC_CHECK_HEADERS([gtk/gtk.h glib-object.h])
  AC_CHECK_FUNCS([gtk_main])
  AC_MSG_CHECKING([whether GTK compiles])
  GTK_COMPILES=no
  AC_LINK_IFELSE(
    [AC_LANG_PROGRAM(
       [[/* Check the Gtk and Glib APIs: */
	 #include <gtk/gtk.h>
	 #include <glib-object.h>
	 static void
	 callback(GObject *go, GParamSpec *spec, gpointer user_data)
	 {}
       ]],[[
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
      AC_SUBST([MY_GTK_VERSION_IS_GOOD_ENOUGH])
    else
      AC_MSG_WARN([[Your version of Gtk+ will have problems with
       closing open displays. This is no problem if you just use
       one display, but if you use more than one and close one of them
       Emacs may crash.]])
      sleep 3
    fi
  fi
else
  AC_MSG_NOTICE([skipping gtk compilation checks])
fi
AC_SUBST([GTK_OBJ])dnl

dnl# more GTK stuff:
if test "x${HAVE_GTK}" = "xyes"; then
  dnl# GTK scrollbars resemble toolkit scrollbars a lot, so to avoid
  dnl# a lot if #ifdef:s, say we have toolkit scrollbars:
  if test "x${with_toolkit_scroll_bars}" != "xno"; then
    with_toolkit_scroll_bars=yes
  fi

  dnl# Check if we can use multiple displays with this GTK version.
  dnl# If gdk_display_open exists, assume all others are there also.
  HAVE_GTK_MULTIDISPLAY=no
  AC_CHECK_FUNCS([gdk_display_open],[HAVE_GTK_MULTIDISPLAY=yes])
  if test "x${HAVE_GTK_MULTIDISPLAY}" = "xyes"; then
    AC_DEFINE([HAVE_GTK_MULTIDISPLAY],[1],
              [Define to 1 if GTK can handle more than one display.])
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

  dnl# Check if we have the new file chooser dialog.
  dnl# If gtk_file_chooser_dialog_new exists, assume others are there too.
  HAVE_GTK_FILE_CHOOSER=no
  AC_CHECK_FUNCS([gtk_file_chooser_dialog_new],[HAVE_GTK_FILE_CHOOSER=yes])

  if test "x${HAVE_GTK_FILE_SELECTION}" = "xyes" \
     && test "x${HAVE_GTK_FILE_CHOOSER}" = "xyes"; then
    AC_DEFINE([HAVE_GTK_FILE_BOTH],[1],
              [Define to 1 if GTK has both file selection/chooser dialog])
  fi

  dnl# Check if pthreads are available.  Emacs only needs this when using
  dnl# gtk_file_chooser under Gnome.
  if test "x${HAVE_GTK_FILE_CHOOSER}" = "xyes"; then
    HAVE_GTK_AND_PTHREAD=no
    if test "x${ac_cv_header_pthread_h}" = "x"; then
      test -z "${ac_cv_header_pthread_h}"
      AC_CHECK_HEADERS([pthread.h])
    fi
    if test "x${ac_cv_header_pthread_h}" != "x" && test "x${ac_cv_header_pthread_h}" != "xno"; then
      test -n "${ac_cv_header_pthread_h}"
      unset ac_cv_lib_pthread_pthread_self
      AC_CHECK_LIB([pthread],[pthread_self],[HAVE_GTK_AND_PTHREAD=yes])
    elif test "x${ac_cv_lib_pthread_pthread_self}" = "xyes"; then
      test -z "${HAVE_GTK_AND_PTHREAD}" && export HAVE_GTK_AND_PTHREAD=yes
    fi
    if test "x${HAVE_GTK_AND_PTHREAD}" = "xyes"; then
      GTK_LIBS="${GTK_LIBS} -lpthread"
      AC_DEFINE([HAVE_GTK_AND_PTHREAD],[1],
                [Define to 1 if you have GTK and pthread (-lpthread).])
    fi
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
  ## this will be the header regardless:
  term_header=gtkutil.h
else
  AC_MSG_NOTICE([skipping even more gtk checks])
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
     AC_CHECK_FUNCS([dbus_watch_get_unix_fd dbus_type_is_valid \
		     dbus_validate_bus_name dbus_validate_path \
		     dbus_validate_interface dbus_validate_member])dnl
     ## set object file name:
     DBUS_OBJ=dbusbind.o
   fi
else
   AC_MSG_NOTICE([skipping dbus checks])
fi
AC_SUBST([DBUS_OBJ])dnl

dnl# GSettings has been tested under GNU/Linux only.
HAVE_GSETTINGS=no
if test "x${HAVE_X11}" = "xyes" && test "x${with_gsettings}" = "xyes"; then
   PKG_CHECK_MODULES([GSETTINGS],[gio-2.0 >= 2.26],
                     [HAVE_GSETTINGS=yes],[HAVE_GSETTINGS=no])
   if test "x${HAVE_GSETTINGS}" = "xyes"; then
      old_CFLAGS="${CFLAGS}"
      CFLAGS="${CFLAGS} ${GSETTINGS_CFLAGS}"
      old_LIBS="${LIBS}"
      LIBS="${LIBS} ${GSETTINGS_LIBS}"
      AC_MSG_CHECKING([whether GSettings is in gio])
      AC_LINK_IFELSE(
         [AC_LANG_PROGRAM(
             [[/* Check that gsettings really is present: */
             #include <glib-object.h>
             #include <gio/gio.h>
             ]],[[
               GSettings *settings;
               GVariant *val = g_settings_get_value(settings, "");
             ]])],
         [HAVE_GSETTINGS=yes],[HAVE_GSETTINGS=no])dnl

      AC_MSG_RESULT([${HAVE_GSETTINGS}])dnl

      if test "x${HAVE_GSETTINGS}" = "xyes"; then
         AC_DEFINE([HAVE_GSETTINGS],[1],[Define to 1 if using GSettings.])
         SETTINGS_CFLAGS="${GSETTINGS_CFLAGS}"
         SETTINGS_LIBS="${GSETTINGS_LIBS}"
      fi
      CFLAGS="${old_CFLAGS}"
      LIBS="${old_LIBS}"
   else
      AC_MSG_NOTICE([no need for a gsettings compilation test if we do not even have the package in the first place])
   fi
else
   AC_MSG_NOTICE([skipping GSettings checks])
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
else
   AC_MSG_NOTICE([skipping GConf checks])
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
else
    AC_MSG_NOTICE([skipping gobject checks])
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
else
   AC_MSG_NOTICE([skipping SELinux checks])
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
else
  AC_MSG_NOTICE([skipping gnutls checks])
fi
dnl# now subst:
AC_SUBST([LIBGNUTLS_LIBS])dnl
AC_SUBST([LIBGNUTLS_CFLAGS])dnl

dnl# notify section:
AC_MSG_CHECKING([notification packages])
AC_MSG_RESULT([ ])
NOTIFY_OBJ=""
NOTIFY_SUMMARY=no

dnl# FIXME?  Do NOT auto-detect on NS, but do allow someone to specify
dnl# a particular library.  This does NOT make much sense, does it?
if test "x${with_ns}" = "xyes" && test "x${with_file_notification}" = "xyes"; then
  with_file_notification=no
  AC_MSG_NOTICE([file notifications disabled])
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
  *) AC_MSG_ERROR([File notification `${with_file_notification}' requested but requirements not found.]) ;;
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
  if test x"${HAVE_X11R5}" = x"yes"; then
    AC_MSG_CHECKING([X11 version 5 with Xaw])
    AC_CACHE_VAL([emacs_cv_x11_version_5_with_xaw],
    [AC_LINK_IFELSE([AC_LANG_PROGRAM([[
#include <X11/Intrinsic.h>
#include <X11/Xaw/Simple.h>]],
      [[]])],
      [emacs_cv_x11_version_5_with_xaw=yes],
      [emacs_cv_x11_version_5_with_xaw=no])])
    if test "x${emacs_cv_x11_version_5_with_xaw}" = "xyes"; then
      AC_MSG_RESULT([5 or newer, with Xaw; use toolkit by default])
      USE_X_TOOLKIT=LUCID
    else
      if test x"${USE_X_TOOLKIT}" = x"LUCID"; then
        AC_MSG_ERROR([Lucid toolkit requires X11/Xaw include files])
      else
        AC_MSG_RESULT([before 5 or no Xaw; do not use toolkit by default])
        USE_X_TOOLKIT=none
      fi
    fi
  else
    USE_X_TOOLKIT=none
  fi
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
else
  AC_MSG_NOTICE([skipping Lucid toolkit checks for Xaw/Xaw3d])
fi

AC_MSG_CHECKING([what X toolkit type we ended up with])
X_TOOLKIT_TYPE="${USE_X_TOOLKIT}"
AC_MSG_RESULT(["${X_TOOLKIT_TYPE}"])dnl
dnl# since this is the same as USE_X_TOOLKIT, we only need to subst one
dnl# of them here:
AC_SUBST([X_TOOLKIT_TYPE])dnl

LIBXTR6=""
LIBXMU=""
if test "x${USE_X_TOOLKIT}" != "xnone"; then
  dnl# "_ONCE" causes test to jump out of the conditional and be done at
  dnl# wrong location, so check ac_cv_header_X11_Intrinsic_h instead:
  if test "x${ac_cv_header_X11_Intrinsic_h}" = "x"; then
    test -z "${ac_cv_header_X11_Intrinsic_h}"
    AC_CHECK_HEADERS([X11/Intrinsic.h])
  fi
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
    OTHERLIBS='-lXt -lSM -lICE'
  else
    LIBS="-lXt ${LIBS}"
    OTHERLIBS='-lXt'
  fi
  AC_CHECK_LIB([Xmu],[XmuConvertStandardSelection])
  test "x${ac_cv_lib_Xmu_XmuConvertStandardSelection}" = "xno" && export LIBS="${OLDLIBS}"
  dnl# ac_cv_lib_Xmu_XmuConvertStandardSelection is also referenced below.
  dnl# if not, though, there is this:
  AC_SEARCH_LIBS([XmuConvertStandardSelection],[Xmu],[],[],[${OTHERLIBS}])
  if test "X${LIBS}" != "X${OLDLIBS}"; then
    LIBXMU="${ac_cv_search_XmuConvertStandardSelection}"
    LIBS="${OLDLIBS}"
  fi
  dnl# ac_cv_search_XmuConvertStandardSelection might also be referenced
  dnl# below...
else
  AC_MSG_NOTICE([skipping checks for Xt version])
fi
AC_SUBST([LIBXTR6])dnl
AC_SUBST([LIBXMU])dnl

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
  if test "x${USE_X_TOOLKIT}" != "xnone" || test -e ${prefix}/lib/libXext.a; then
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
      REAL_LDFLAGS="-L/usr/X11R6/LessTif/Motif1.2/lib ${REAL_LDFLAGS}"
    else
      CFLAGS="${OLD_CFLAGS}"
      CPPFLAGS="${OLD_CPPFLAGS}"
    fi
  fi
  AC_CHECK_HEADER([Xm/BulletinB.h],[],
    [AC_MSG_ERROR([Motif toolkit requested, but requirements not found.])])
else
  AC_MSG_NOTICE([skipping checks only needed for the Motif family of toolkits])
fi

### Is -lXaw3d available?
HAVE_XAW3D=no
if test "x${HAVE_X11}" = "xyes"; then
  if test "x${USE_X_TOOLKIT}" != "xnone" && test "x${with_toolkit_scroll_bars}" != "xno"; then
    dnl# FIXME: determine what Scrollbar.h needs to avoid compilation
    dnl# errors from the test now that the `-' has been removed.
    AC_CHECK_HEADER([X11/Xaw3d/Scrollbar.h],[
      if test "x${emacs_cv_xaw3d}" = "x" && test "x${ac_cv_lib_Xaw3d_XawScrollbarSetThumb}" = "x"; then
        test -z "${ac_cv_lib_Xaw3d_XawScrollbarSetThumb}"
        AC_CHECK_LIB([Xaw3d],[XawScrollbarSetThumb],[HAVE_XAW3D=yes])
      elif test "x${emacs_cv_xaw3d}" = "xyes"; then
        HAVE_XAW3D=yes
      fi
    ],[],[[]])
    if test "x${HAVE_XAW3D}" = "xyes"; then
	AC_DEFINE([HAVE_XAW3D],[1],
		  [Define to 1 if you have the Xaw3d library (-lXaw3d).])
    fi
  fi
else
  AC_MSG_NOTICE([skipping checks for libXaw3d])
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
      ## gtk matters here, too:
      if test "x${HAVE_GTK}" = "xyes"; then
        dnl# Check if --with-pkg-config-prog has been given.
        if test "X${with_pkg_config_prog}" != "X" && test "X${PKG_CONFIG}" = "X"; then
          test -n "${with_pkg_config_prog}" && test -z "${PKG_CONFIG}" && export PKG_CONFIG="${with_pkg_config_prog}"
        fi
      fi

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
        if test "x${ac_cv_header_X11_X_h}" != "x"; then
          test -z "${ac_cv_header_X11_X_h}"
          AC_CHECK_HEADER([X11/X.h])
        fi
	AC_CHECK_HEADER([X11/Xft/Xft.h],
	  [AC_CHECK_LIB([Xft],[XftFontOpen],[HAVE_XFT=yes],[],
                        [${XFT_LIBS}])],[],
          [[#include <X11/X.h>]])dnl

	if test "x${HAVE_XFT}" = "xyes"; then
	  AC_DEFINE([HAVE_XFT],[1],
                    [Define to 1 if you have the Xft library.])dnl
	  AC_SUBST([XFT_LIBS])dnl
          ## this is the relevant variable to put the revelant CFLAGS in:
	  C_SWITCH_X_SITE="${C_SWITCH_X_SITE} ${XFT_CFLAGS}"
	else
	  CPPFLAGS="${OLD_CPPFLAGS}"
	  CFLAGS="${OLD_CFLAGS}"
	  LIBS="${OLD_LIBS}"
	fi                        # "x${HAVE_XFT}" = "xyes"
      else
        test -z "${ac_cv_header_X11_X_h}"
        AC_CHECK_HEADER([X11/X.h])
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
    AC_MSG_NOTICE([X11 is unavailable, so skipping checks for X11-specific font-rendering backends])
    HAVE_XFT=no
    HAVE_FREETYPE=no
    HAVE_LIBOTF=no
    HAVE_M17N_FLT=no
fi

### End of font-backend (under X11) section.
dnl# subst the stuff from the previous section:
AC_SUBST([FREETYPE_CFLAGS])dnl
AC_SUBST([FREETYPE_LIBS])dnl
AC_SUBST([FONTCONFIG_CFLAGS])dnl
AC_SUBST([FONTCONFIG_LIBS])dnl
AC_SUBST([LIBOTF_CFLAGS])dnl
AC_SUBST([LIBOTF_LIBS])dnl
AC_SUBST([M17N_FLT_CFLAGS])dnl
AC_SUBST([M17N_FLT_LIBS])dnl

dnl# image formats section:
AC_MSG_CHECKING([more graphics packages])
AC_MSG_RESULT([ ])
### Use -lXpm if available, unless `--with-xpm=no'.
### mingw32 does NOT use -lXpm, since it loads the library dynamically.
### In the Cygwin-w32 build, we need to use /usr/include/noX/X11/xpm.h
### rather than /usr/include/X11/xpm.h, so we set our CPPFLAGS
### (and LDFLAGS) accordingly.
HAVE_XPM=no
LIBXPM=""
if test "x${HAVE_W32}" = "xyes" && test "x${opsys}" = "xcygwin"; then
  if test "x${with_xpm}" != "xno"; then
    SAVE_CPPFLAGS="${CPPFLAGS}"
    SAVE_LDFLAGS="${LDFLAGS}"
    CPPFLAGS="${CPPFLAGS} -I/usr/include/noX"
    LDFLAGS="${LDFLAGS} -L/usr/lib/noX"
    AC_CHECK_HEADER([X11/xpm.h],
      [AC_CHECK_LIB([Xpm],[XpmReadFileToImage],[HAVE_XPM=yes],
        [if test "x${ac_cv_lib_Xpm_XpmReadFileToPixmap}" = "x"; then
           test -z "${ac_cv_lib_Xpm_XpmReadFileToPixmap}"
           AC_CHECK_LIB([Xpm],[XpmReadFileToPixmap],[HAVE_XPM=yes],[],
                        [-lX11])
         elif test "x${ac_cv_lib_Xpm_XpmReadFileToPixmap}" = "xyes"; then
           test -n "${ac_cv_lib_Xpm_XpmReadFileToPixmap}" && HAVE_XPM=yes
         fi
      ])dnl
    ])dnl
    ## ...and also:
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
        REAL_LDFLAGS="${REAL_LDFLAGS} -L/usr/lib/noX"
	AC_MSG_RESULT([yes])
      else
	AC_MSG_RESULT([no])
        CPPFLAGS="${SAVE_CPPFLAGS}"
        LDFLAGS="${SAVE_LDFLAGS}"
      fi
    fi
    ## in case we missed the conditional where these previously got popped:
    CPPFLAGS="${SAVE_CPPFLAGS}"
    LDFLAGS="${SAVE_LDFLAGS}"
    AC_SUBST([SAVE_CPPFLAGS])dnl
    AC_SUBST([SAVE_LDFLAGS])dnl
    ## check their value where they get substed, just in case...
  fi

  if test "x${HAVE_XPM}" = "xyes"; then
    AC_DEFINE([HAVE_XPM],[1],
              [Define to 1 if you have the Xpm library (-lXpm).])
    LIBXPM='-lXpm'
  fi
fi

## this seems duplicated (this is because the previous, similar-looking
## block of code is for Windows)...
if test "x${HAVE_X11}" = "xyes"; then
  dnl# Avoid Xpm on AIX unless requested, as it crashes; see Bug#17598:
  test "x${opsys}${with_xpm_set}" = "xaix4-2" && export with_xpm=no

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
  elif test "${opsys},${LUCID_LIBW}" = "aix4-2,-lXaw"; then
    dnl# AIX -lXaw needs -lXpm linked too; see Bug#17598 Message#152:
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

if test "x${HAVE_XPM}" = "x" && test "x${LIBXPM}" = "x"; then
  test -z "${HAVE_XPM}" && test -z "${LIBXPM}"
  AC_MSG_NOTICE([skipped all checks for xpm])
fi
dnl# now subst:
AC_SUBST([LIBXPM])dnl

### Use -ljpeg if available, unless `--with-jpeg=no'.
HAVE_JPEG=no
LIBJPEG=""
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
      ])dnl
    ])dnl
  ])dnl
  ## ...okay, should be good... for that part, at least.

  if test "x${HAVE_JPEG}" = "xyes"; then
    AC_CACHE_CHECK([for jpeglib 6b or later],
      [emacs_cv_jpeglib],
      [OLD_LIBS="${LIBS}"
       for emacs_cv_jpeglib in yes -ljpeg -ljpeg-turbo -lmozjpeg no; do
         case ${emacs_cv_jpeglib} in
	   yes) ;;
           no) break;;
	   *) LIBS="${LIBS} ${emacs_cv_jpeglib}";;
         esac
         AC_LINK_IFELSE([AC_LANG_PROGRAM([[
#undef HAVE_STDLIB_H /* Avoid config.h/jpeglib.h collision.  */
#include <stdio.h> /* jpeglib.h needs FILE and size_t.  */
#include <jpeglib.h>
#include <jerror.h>
char verify[JPEG_LIB_VERSION < 62 ? -1 : 1];
struct jpeg_decompress_struct cinfo;
	      ]],[[
  jpeg_create_decompress(&cinfo);
  WARNMS(&cinfo, JWRN_JPEG_EOF);
  jpeg_destroy_decompress(&cinfo);
	      ]])],
	   [emacs_link_ok=yes],
	   [emacs_link_ok=no])
         LIBS="${OLD_LIBS}"
         test "x${emacs_link_ok}" = "xyes" && break
       done])dnl
    ## hopefully it sets the cache value correctly...
    if test "x${emacs_cv_jpeglib}" != "xno"; then
      HAVE_JPEG=yes
      AC_DEFINE([HAVE_JPEG],[1],
        [Define to 1 if you have the jpeg library (typically -ljpeg).])
    else
      HAVE_JPEG=no
    fi
  fi
  if test "x${HAVE_JPEG}" = "xyes"; then
    ### mingw32 does NOT use -ljpeg, since it loads the library
    ### dynamically when needed, and does NOT want a run-time
    ### dependency on the jpeglib DLL.
    test "x${emacs_cv_jpeglib}" != "xyes" && test "${opsys}" != "mingw32" \
      && LIBJPEG="${emacs_cv_jpeglib}"
  fi
else
  AC_MSG_NOTICE([skipping checks for libjpeg])
fi
AC_SUBST([LIBJPEG])dnl

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

### Use -lpng if available, unless `--with-png=no'.
### mingw32 does NOT use -lpng, since it loads the library dynamically.
HAVE_PNG=no
LIBPNG=""
PNG_CFLAGS=""
if test "${NS_IMPL_COCOA}" = yes; then
  : # Nothing to do, besides print a message:
  AC_MSG_NOTICE([no need to check for libpng when using Cocoa])
elif test "x${opsys}" = "xmingw32"; then
  if test "x${with_png}" != "xno"; then
    AC_CHECK_HEADER([png.h],[HAVE_PNG=yes],[HAVE_PNG=no])
  fi
  if test "x${HAVE_PNG}" = "xyes"; then
    AC_DEFINE([HAVE_PNG],[1],
              [Define to 1 if you have the png library (-lpng).])dnl

    AC_CHECK_FUNCS([png_set_longjmp_fn png_longjmp])
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
    AC_MSG_CHECKING([for png])
    png_cflags=`(libpng-config --cflags) 2>&AS_MESSAGE_LOG_FD` &&
    png_libs=`(libpng-config --libs) 2>&AS_MESSAGE_LOG_FD` || {
      # libpng-config does not work; configure by hand.
      # Debian unstable as of July 2003 has multiple libpngs, & puts png.h
      # in /usr/include/libpng.
      if test -r /usr/include/libpng/png.h &&
	 test ! -r /usr/include/png.h; then
	png_cflags=-I/usr/include/libpng
      else
	png_cflags=""
      fi
      png_libs='-lpng'
    }
    SAVE_CFLAGS="${CFLAGS}"
    SAVE_LIBS="${LIBS}"
    CFLAGS="${CFLAGS} ${png_cflags}"
    LIBS="${png_libs} -lz -lm ${LIBS}"
    AC_LINK_IFELSE(
      [AC_LANG_PROGRAM([[#include <png.h>]],
	 [[return !png_get_channels(0, 0);]])],
      [HAVE_PNG=yes
       test -n "${edit_cflags}"
       PNG_CFLAGS=`AS_ECHO(["${png_cflags}"]) | sed -e "${edit_cflags}"`
       LIBPNG="${png_libs}"
       # ${LIBPNG} requires explicit -lz in some cases.
       # We do NOT know what those cases are, exactly, so play it safe and
       # append -lz to any nonempty ${LIBPNG}, unless we are already using
       # LIBZ:
       if test -n "${LIBPNG}" && test -z "${LIBZ}"; then
	 LIBPNG="${LIBPNG} -lz"
       fi])dnl# end link test
    ## reset:
    CFLAGS="${SAVE_CFLAGS}"
    LIBS="${SAVE_LIBS}"

    AC_MSG_RESULT([${HAVE_PNG}])dnl

    # Old way of checking for libpng:
    # As mentioned above, Debian unstable as of July 2003 has multiple
    # libpngs, and puts png.h in /usr/include/libpng.
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
    if test "x${LIBPNG}" = "x"; then
      test -z "${LIBPNG}" && LIBPNG="-lpng -lz -lm"
    fi

    SAVE_CFLAGS="${CFLAGS}"
    CFLAGS="${CFLAGS} ${PNG_CFLAGS}"
    AC_CHECK_FUNCS([png_set_longjmp_fn png_longjmp])
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
    CFLAGS="${SAVE_CFLAGS}"
  fi
else
  AC_MSG_NOTICE([skipping checks for libpng])
fi
AC_SUBST([LIBPNG])dnl
AC_SUBST([PNG_CFLAGS])dnl

dnl# (zlib checks have been moved to before the libpng ones)

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
else
  AC_MSG_NOTICE([skipping checks for libtiff])
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
else
  AC_MSG_NOTICE([skipping checks for libgif or libungif])
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
else
  AC_MSG_NOTICE([skipping checks for libgpm])
fi
AC_SUBST([LIBGPM])dnl

AC_MSG_CHECKING([darwin/NeXTstep])
AC_MSG_RESULT([ ])
dnl# Check for malloc/malloc.h on darwin:
case "${opsys}" in
  darwin)
    test -z "${ac_cv_header_malloc_malloc_h}"
    dnl# "_ONCE" causes test to jump out of the conditional and be done at
    dnl# wrong location, so leave that part off:
    AC_CHECK_HEADERS([malloc/malloc.h])dnl
    AC_LANG_PUSH([Objective C])dnl
    ## unexmacosx.c uses this as a backup in case <malloc/malloc.h> is bad:
    if test "x${HAVE_NS}" = "xyes"; then
      if test "x${ac_cv_header_malloc_malloc_h}" != "xyes"; then
        AC_CHECK_HEADERS([objc/malloc.h])
      fi
    fi
    AC_CHECK_HEADERS([objc/runtime.h])
    AC_LANG_POP
    ;;
esac

AC_CHECK_HEADERS([Availability.h])dnl
  ## in case we skipped it, due to no Carbon or something:
if test "x${ac_cv_header_AvailabilityMacros_h}" = "x"; then
  test -z "${ac_cv_header_AvailabilityMacros_h}"
  AC_CHECK_HEADERS([AvailabilityMacros.h])dnl
  ## (just in case)
fi

MAC_CFLAGS=""
### The Mac OS X Carbon API is one way to implement the GUI.
### ("${HAVE_CARBON}" is the same as "${HAVE_MACGUI}" from Y.M.'s Mac port)
if test "x${HAVE_CARBON}" = "xyes"; then
  AC_DEFINE([HAVE_CARBON],[1],
            [Define to 1 if you are using the Carbon API on Mac OS X.])
  ## (maybe also define HAVE_SOUND here?)
  if test "x${emacs_cv_apple}" = "xyes"; then
    MAC_CFLAGS="-fconstant-cfstrings"
  fi
  ## Specify the install directory:
  AC_MSG_CHECKING([directory for the Carbon app])
  carbon_appdir=""
  if test "x${carbon_appdir_x}" != "x"; then
    case ${carbon_appdir_x} in
      y | ye | yes)  carbon_appdir=/Applications ;;
      * ) carbon_appdir="${carbon_appdir_x}" ;;
    esac
  fi
  if test "x${carbon_appdir}" != "x"; then
    test -n ${carbon_appdir}
    AC_MSG_RESULT([${carbon_appdir}])
  else
    test -z ${carbon_appdir}
    AC_MSG_RESULT([none])
  fi
  # We also have mouse menus.
  HAVE_MENUS=yes
  # Tell src/Makefile.in to create files in the Mac OS X application
  # bundle mac/Emacs.app.
  OTHER_FILES=macosx-app
fi

GNUSTEP_CFLAGS=""
### The NeXTstep API is another way to implement the GUI.
if test "x${HAVE_NS}" = "xyes"; then
  AC_DEFINE([HAVE_NS],[1],
            [Define to 1 if you are using the NeXTstep API, either GNUstep, or Cocoa on Mac OS X.])
  if test "x${NS_IMPL}" != "x"; then
    test -n "${NS_IMPL}"
    AC_DEFINE_UNQUOTED([NS_IMPL],["${NS_IMPL}"],
     [Define to the string representing the NeXTstep implementation used.])
  fi
  if test "x${NS_IMPL_COCOA}" = "xyes"; then
    AC_DEFINE([NS_IMPL_COCOA],[1],
              [Define to 1 if you are using NS windowing under MacOS X.])
  fi
  if test "x${NS_IMPL_GNUSTEP}" = "xyes"; then
    AC_DEFINE([NS_IMPL_GNUSTEP],[1],
              [Define to 1 if you are using NS windowing under GNUstep.])
    if test "x${NS_GNUSTEP_CONFIG}" != "xyes"; then
      # See also .m.o rule in Makefile.in
      # FIXME: are all these flags really needed?  Document here why:
      GNUSTEP_CFLAGS="-D_REENTRANT -fPIC -fno-strict-aliasing -I${GNUSTEP_SYSTEM_HEADERS} ${GNUSTEP_LOCAL_HEADERS}"
      ## Extra CFLAGS applied to src/*.m files:
      GNU_OBJC_CFLAGS="${GNU_OBJC_CFLAGS} -fgnu-runtime -Wno-import -fconstant-string-class=NSConstantString -DGNUSTEP_BASE_LIBRARY=1 -DGNU_GUI_LIBRARY=1 -DGNU_RUNTIME=1 -DGSWARN -DGSDIAGNOSE"
    elif test "x${NS_GNUSTEP_CONFIG}" = "xyes"; then
      test -n "${NS_GNUSTEP_CONFIG}"
      AC_MSG_NOTICE([using values from gnustep-config for flags])
    fi
  elif test "x${NS_IMPL_COCOA}" = "xyes"; then
    test -z "${GNUSTEP_CFLAGS}" && GNUSTEP_CFLAGS="-fno-strict-aliasing"
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
else
  AC_MSG_NOTICE([skipping checks for libSM])
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
    AC_CHECK_HEADER([X11/extensions/Xrender.h])
    # Test old way in case pkg-config does NOT have it (older machines).
    # Include Xrender.h by hand to work around bug in older Xrandr.h
    # (e.g. RHEL5) and silence (harmless) configure warning (bug#18465).
    AC_CHECK_HEADER([X11/extensions/Xrandr.h],
      [AC_CHECK_LIB([Xrandr],[XRRGetScreenResources],[HAVE_XRANDR=yes])],
      [],[AC_INCLUDES_DEFAULT
#include <X11/extensions/Xrender.h>])
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
else
  AC_MSG_NOTICE([skipping checks for XRandr])
fi
AC_SUBST([XRANDR_CFLAGS])dnl
AC_SUBST([XRANDR_LIBS])dnl

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
else
  AC_MSG_NOTICE([skipping checks for Xinerama])
fi
AC_SUBST([XINERAMA_CFLAGS])dnl
AC_SUBST([XINERAMA_LIBS])dnl

### Use Xfixes (-lXfixes) if available
HAVE_XFIXES=no
if test "x${HAVE_X11}" = "xyes"; then
  XFIXES_REQUIRED=4.0.0
  XFIXES_MODULES="xfixes >= ${XFIXES_REQUIRED}"
  m4_ifdef([EMACS_CHECK_MODULES],[
    EMACS_CHECK_MODULES([XFIXES],[${XFIXES_MODULES}])
  ],[
    PKG_CHECK_MODULES([XFIXES],[${XFIXES_MODULES}],[HAVE_XFIXES=yes],
                      [HAVE_XFIXES=no])
  ])
  if test "x${HAVE_XFIXES}" = "xno"; then
    # Test old way in case pkg-config does NOT have it (older machines).
    AC_CHECK_HEADER([X11/extensions/Xfixes.h],
      [AC_CHECK_LIB([Xfixes],[XFixesHideCursor],[HAVE_XFIXES=yes])])
    if test "x${HAVE_XFIXES}" = "xyes"; then
      XFIXES_LIBS='-lXfixes'
    fi
  fi
  if test "x${HAVE_XFIXES}" = "xyes"; then
    AC_DEFINE([HAVE_XFIXES],[1],
              [Define to 1 if you have the Xfixes extension.])
  fi
else
  AC_MSG_NOTICE([skipping checks for Xfixes])
fi
AC_SUBST([XFIXES_CFLAGS])dnl
AC_SUBST([XFIXES_LIBS])dnl

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
    if test -z "${xcsdkdir}" -a -n "${XCRUN}" -a ! -d /usr/include; then
      dnl# /usr/include is not found.  Try Xcode SDK dir if it is sane:
      xcsdkdir=`${XCRUN} --show-sdk-path 2>/dev/null`
      case ${xcsdkdir} in
	*[[\\\"\#\$\&\'\`$am_lf\ \	]]*)
	xcsdkdir="" ;;
      esac
    fi
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
else
  AC_MSG_NOTICE([skipping checks for libxml2])
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

# sqrt and other floating-point functions such as fmod, logb, and frexp
# are found in -lm on most systems, but mingw32 does NOT use -lm.
if test "x${opsys}" != "xmingw32"; then
  m4_ifdef([LT_LIB_M],[
    LT_LIB_M
  ],[
    # On HPUX 9.01, -lm does not contain logb, so skip it there.
    case ${opsys} in
      hpux9*)
        AC_MSG_WARN([skipping check for logb in libm on HPUX 9])
        export libm_has_logb=no
        ;;
      *)
        AC_CHECK_LIB([m],[logb],[libm_has_logb=yes],[libm_has_logb=no])
        ;;
    esac
    AC_CHECK_LIB([m],[sqrt],[libm_has_sqrt=yes],[libm_has_sqrt=no])
    AC_CHECK_LIB([m],[fmod],[libm_has_fmod=yes],[libm_has_fmod=no])
    AC_CHECK_LIB([m],[frexp],[libm_has_frexp=yes],[libm_has_frexp=no])
    if test "x${libm_has_sqrt}" = "xyes" && test "x${libm_has_fmod}" = "xyes" && test "x${libm_has_frexp}" = "xyes"; then
      AC_MSG_NOTICE([libm has at least 3 symbols that need to be in it])
      export LIBM="-lm"
      AC_SUBST([LIBM])
    else
      AC_MSG_WARN([your libm is missing symbols])
    fi
  ])dnl
  ## (else do nothing)
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
  AC_SUBST([LD_LIBRARY_PATH])dnl
  ## error message:
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
dnl# definition of this next macro has been moved to the included file:
m4_include([m4/functions_extra.m4])dnl
AC_REQUIRE_VARIOUS_FUNC_CHECKS
if test "x${emacs_cv_var_doug_lea_malloc}" = "x"; then
  if test "x${system_malloc}" = "x"; then
    if test "x${GNU_MALLOC}" -a "x${GMALLOC_OBJ}" = "x"; then
      test -z "${ac_cv_func_malloc}"
      AC_FUNC_MALLOC
    fi
  fi
fi
if test "x${ac_cv_func_mbrtowc}" = "x"; then
  test -z "${ac_cv_func_mbrtowc}"
  AC_FUNC_MBRTOWC
fi
m4_ifdef([AC_FUNC_MEMCMP],[
  AC_FUNC_MEMCMP
],[
  AC_CHECK_FUNCS([memcmp])dnl
])dnl
dnl# (mktime checks moved into m4/functions_extra.m4)
AC_FUNC_REALLOC
AC_FUNC_SELECT_ARGTYPES
AC_FUNC_SETPGRP
AC_FUNC_SETVBUF_REVERSED
AC_FUNC_STAT
if test "x${ac_cv_func_strerror_r_char_p}" = "x"; then
  test -z "${ac_cv_func_strerror_r_char_p}"
  test -n "${ac_cv_have_decl_strerror_r}"
  test -n "${ac_cv_func_strerror_r}"
  AC_FUNC_STRERROR_R
fi
AC_FUNC_STRFTIME
AC_FUNC_STRTOD
AC_FUNC_UTIME_NULL
dnl# these ones are approximately in their historical order, and get
dnl# special LIBS while checking for them:
OLD_LIBS="${LIBS}"
LIBS="${LIB_PTHREAD} ${LIB_MATH} ${LIBS}"
dnl## ('fchdir', 'random', 'rint', and 'getpagesize' are checked elsewhere)
AC_CHECK_FUNCS([accept4 gethostname getdomainname rename sysinfo \
getrusage get_current_dir_name \
lrand48 bcopy bcmp cbrt setsid \
fpathconf select setlocale newlocale \
utimes getrlimit setrlimit setpgid getwd shutdown getaddrinfo \
mblen psignal strsignal setitimer ualarm index rindex \
sendto recvfrom getsockopt setsockopt getsockname getpeername \
getifaddrs freeifaddrs gai_strerror mkstemp getline mremap sync \
difftime memcpy mempcpy getpwent endpwent getgrent endgrent \
touchlock \
cfmakeraw cfsetspeed copysign __executable_start log2])dnl
AC_CHECK_DECLS([cfmakeraw],[],[],[[
#ifdef HAVE_TERMIOS_H
# include <termios.h>
#endif /* HAVE_TERMIOS_H */
]])dnl
m4_ifdef([AC_CHECK_FUNCS_ONCE],[
  AC_CHECK_FUNCS_ONCE([getdelim pthread_sigmask])dnl
],[
  if test "x${ac_cv_func_getdelim}" = "x"; then
    test -z "${ac_cv_func_getdelim}"
    AC_CHECK_FUNCS([getdelim])dnl
    ## (in case gnulib fails to do it)
  fi
  if test "x${ac_cv_func_pthread_sigmask}" = "x"; then
    test -z "${ac_cv_func_pthread_signmask}"
    AC_CHECK_FUNCS([pthread_sigmask])dnl
    ## (in case gnulib fails to do it)
  fi
])dnl# end ifdef
  ## reset:
LIBS="${OLD_LIBS}"
  ## okay, onto more regular function checks...
dnl# some libm funcs that might have been checked conditionally previously:
if test "x${libm_has_logb}" = "x" && test "x${ac_cv_func_logb}" = "x"; then
  test -z "${libm_has_logb}" && test -z "${ac_cv_func_logb}"
  AC_CHECK_FUNCS([logb])
fi
if test "x${libm_has_frexp}" = "x" && test "x${ac_cv_func_frexp}" = "x"; then
  test -z "${libm_has_frexp}" && test -z "${ac_cv_func_frexp}"
  AC_CHECK_FUNCS([frexp])
fi
if test "x${libm_has_fmod}" = "x" && test "x${ac_cv_func_fmod}" = "x"; then
  test -z "${libm_has_fmod}" && test -z "${ac_cv_func_fmod}"
  AC_CHECK_FUNCS([fmod])
fi
dnl# these ones should be in alphabetical order:
AC_CHECK_FUNCS([__fpending _stricmp acl atexit atof atoi btowc bzero \
                cuserid doprnt errc fabs fabsf fgets floor ftime \
                get_etext get_edata get_end getpass getpwuid inet_ntoa \
                isascii iswprint mbrlen memmove memset mkdir munmap \
                nl_langinfo pathconf posix_spawnp pow putenv re_comp \
                reallocf regcomp rmdir socket sqrt strcasecmp strcasestr \
                strchr strcmp strcspn strdup strerror stricmp strlcat \
                strlcpy strncasecmp strncat strncmp strnicmp strnstr \
                strpbrk strrchr strtok strtok_r strtol strtoul sysctl \
                system xstricmp utime utmpname vfprintf])
dnl# Keep autoscan happy with a dummy condition:
if test "x" = "Y0"; then
  test -x "`which autoscan`"
  if test "x${ac_cv_lib_error_at_line}" = "x" || test "x${ac_cv_lib_error_at_line}" = "xno"; then
    test -z "${ac_cv_lib_error_at_line}"
    AC_FUNC_ERROR_AT_LINE
  fi
  m4_ifdef([AC_CHECK_FUNCS_ONCE],[
    AC_CHECK_FUNCS([getdelim])dnl
  ])dnl
  AC_CHECK_FUNCS([alarm dup2 fchdir ftruncate getcwd geteuid \
                  gethostbyname getpagesize gettimeofday localtime_r \
                  setenv strtoumax tzset uname])dnl
  ## just in case...
fi

dnl# No need to check for aligned_alloc and posix_memalign if using
dnl# gmalloc.o, as it supplies them, unless we are using hybrid_malloc.
dnl# Do NOT use these functions on Darwin though, as they are incompatible
dnl# with unexmacosx.c.
if (test -z "${GMALLOC_OBJ}" || test "x${hybrid_malloc}" = "xyes") \
  && test "x${opsys}" != "xdarwin"; then
  AC_CHECK_FUNCS([aligned_alloc posix_memalign],[break])
elif test "x${opsys}" = "xdarwin"; then
  AC_MSG_NOTICE([skipping check for aligned_alloc() and posix_memalign() functions on darwin.])
fi
AC_CHECK_DECLS([aligned_alloc],[],[],[[#include <stdlib.h>]])dnl

## Eric Backus <ericb@lsid.hp.com> says, HP-UX 9.x on HP 700 machines
## has a broken `rint' in some library versions including math library
## version number A.09.05.
## You can fix the math library by installing patch number PHSS_4630.
## But we can fix it more reliably for Emacs by just not using rint.
## We also skip HAVE_RANDOM - see comments in src/conf_post.h.
case ${opsys} in
   hpux*) AC_MSG_WARN([skipping checks for random() and rint() on HPUX]) ;;
   *) AC_CHECK_FUNCS([random rint]) ;;
esac

dnl# Cannot use AC_CHECK_FUNCS for this:
AC_CACHE_CHECK([for __builtin_unwind_init],
	       [emacs_cv_func___builtin_unwind_init],
[AC_LINK_IFELSE([AC_LANG_PROGRAM([[]],[[__builtin_unwind_init();]])],
		[emacs_cv_func___builtin_unwind_init=yes],
		[emacs_cv_func___builtin_unwind_init=no])])
if test "x${emacs_cv_func___builtin_unwind_init}" = "xyes"; then
  AC_DEFINE([HAVE___BUILTIN_UNWIND_INIT],[1],
	    [Define to 1 if you have the `__builtin_unwind_init' function])
fi

dnl# "_ONCE" causes test to be done at wrong location, so leave that off:
AC_CHECK_HEADERS([sys/un.h])dnl

AC_DEFUN([AC_REQUIRE_FUNC_FSEEKO],[
  AC_REQUIRE([AC_FUNC_FSEEKO])dnl
])dnl
AC_REQUIRE_FUNC_FSEEKO

# Configure getopt:
m4_include([m4/getopt.m4])dnl
m4_ifdef([gl_GETOPT_IFELSE],[
  gl_GETOPT_IFELSE([
    m4_ifdef([gl_GETOPT_SUBSTITUTE_HEADER],[
      gl_GETOPT_SUBSTITUTE_HEADER
    ])dnl
    m4_ifdef([gl_PREREQ_GETOPT],[
      gl_PREREQ_GETOPT
    ])dnl
    ## these might be unnecessary with gnulib, but whatever:
    GETOPTOBJS='getopt.o getopt1.o'
  ])dnl
])dnl
AC_SUBST([GETOPTOBJS])dnl

# UNIX98 PTYs:
AC_CHECK_FUNCS([grantpt])dnl

# PTY-related GNU extensions:
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
  darwin)
    LIBS_TERMCAP="-lncurses"
    ;;
  gnu*)
    test -z "${LIBS_TERMCAP}" && LIBS_TERMCAP="-lncurses"
    ;;
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
  openbsd | dragonfly)
    LIBS_TERMCAP="-lncurses"
    ;;

  ## hpux: Make sure we get select from libc rather than from libcurses
  ##  because libcurses on HPUX 10.10 has a broken version of select.
  ##  We used to use -lc -lcurses, but this may be cleaner.
  ## FIXME?  But TERMINFO = yes on hpux (it used to be explicitly
  # set that way, now it uses the default). Is this not a contradiction?
  hpux*)
    LIBS_TERMCAP="-ltermcap"
    ;;
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

# GNU/Linux-specific timer functions.
AC_CHECK_HEADER([sys/timerfd.h])
AC_CACHE_CHECK([for timerfd interface],[emacs_cv_have_timerfd],
  [AC_COMPILE_IFELSE(
     [AC_LANG_PROGRAM([[#include <sys/timerfd.h>
		      ]],
		      [[timerfd_create (CLOCK_REALTIME,
					(TFD_CLOEXEC | TFD_NONBLOCK));
			timerfd_settime(0, TFD_TIMER_ABSTIME, 0, 0);]])],
     [emacs_cv_have_timerfd=yes],
     [emacs_cv_have_timerfd=no])])
if test "x${emacs_cv_have_timerfd}" = "xyes"; then
  AC_DEFINE([HAVE_TIMERFD],[1],
    [Define to 1 if timerfd functions are supported as in GNU/Linux.])
fi

# Alternate stack for signal handlers.
AC_CACHE_CHECK([whether signals can be handled on alternate stack],
	       [emacs_cv_alternate_stack],
  [AC_COMPILE_IFELSE(
     [AC_LANG_PROGRAM([[#include <signal.h>
		      ]],
		      [[stack_t ss;
			struct sigaction sa;
			ss.ss_sp = malloc(SIGSTKSZ);
			ss.ss_size = SIGSTKSZ;
			sa.sa_flags = (SA_SIGINFO | SA_ONSTACK);
			sigaltstack(&ss, 0);
			sigaction(SIGSEGV, &sa, 0);]])],
     [emacs_cv_alternate_stack=yes],
     [emacs_cv_alternate_stack=no])])dnl

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
else
  AC_MSG_NOTICE([skipping checks for Hesiod])
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
dnl# now actually check stuff:
if test "x${with_kerberos}" != "xno"; then
  OLD_LIBS="${LIBS}"
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
                                       ])dnl
                     ])dnl
    ## ...why did Kerberos change between roman and arabic numerals?
  fi
  AC_CHECK_HEADERS([com_err.h])
  LIBS="${OLD_LIBS}"
else
  AC_MSG_NOTICE([skipping kerberos checks])
fi
dnl# now subst values set by all of that:
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
dnl# okay to do this one in a different location though:
AC_CHECK_FUNCS_ONCE([tzalloc])dnl
dnl# now do a more specific conftest:
AC_MSG_CHECKING([whether localtime caches TZ])
AC_CACHE_VAL([emacs_cv_localtime_cache],
[if test "x${ac_cv_func_tzset}" = "xyes"; then
AC_RUN_IFELSE([AC_LANG_SOURCE([[#include <time.h>
char TZ_GMT0[] = "TZ=GMT0";
char TZ_PST8[] = "TZ=PST8";
int main(void)
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

if test "x${HAVE_TIMEVAL}" = "xyes"; then
  if test "x${ac_cv_func_gettimeofday}" = "x"; then
    test -z "${ac_cv_func_gettimeofday}"
    AC_CHECK_FUNCS([gettimeofday])
  fi
  if test "x${ac_cv_func_gettimeofday}" = "xyes"; then
    AC_CACHE_CHECK([whether gettimeofday can accept two arguments],
		   [emacs_cv_gettimeofday_two_arguments],
    [AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#ifdef TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# ifdef HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  ifdef HAVE_TIME_H
#   include <time.h>
#  else
#   if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#    warning "conftest for gettimeofday args wants a time-related header."
#   endif /* __GNUC__ && !__STRICT_ANSI__ */
#  endif /* HAVE_TIME_H */
# endif /* HAVE_SYS_TIME_H */
#endif /* TIME_WITH_SYS_TIME */
      ]],
      [[struct timeval time;
        gettimeofday(&time, 0);]])],
      [emacs_cv_gettimeofday_two_arguments=yes],
      [emacs_cv_gettimeofday_two_arguments=no])])
    if test "x${emacs_cv_gettimeofday_two_arguments}" = "xno"; then
      AC_DEFINE([GETTIMEOFDAY_ONE_ARGUMENT],[1],
		[Define to 1 if gettimeofday accepts only one argument.])
    elif test "x${emacs_cv_gettimeofday_two_arguments}" = "xyes"; then
      AC_DEFINE([GETTIMEOFDAY_TWO_ARGUMENTS],[2],
		[Define to 2 if gettimeofday accepts two arguments.])
    fi
  fi
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

if test -f /usr/lpp/X11/bin/smt.exp; then
  AC_DEFINE([HAVE_AIX_SMT_EXP],[1],
	    [Define to 1 if the file /usr/lpp/X11/bin/smt.exp exists.])
fi

dnl# Check for a Solaris 2.4 vfork bug that Autoconf misses (through 2.69).
dnl# This can be removed once we assume Autoconf 2.70.
case ${canonical} in
  *-solaris2.4 | *-solaris2.4.*)
    dnl# Disable the Autoconf-generated vfork test.
    AC_MSG_WARN([disabling test for vfork on Solaris 2.4])
    : ${ac_cv_func_vfork_works=no}
    ;;
  *-darwin5.[[012345]]*)
    AC_MSG_WARN([vfork had issues on Mac OS X Puma.])
    : ${ac_cv_func_vfork_works=no}
    dnl# actually nvm, test it anyways:
    unset ac_cv_func_vfork_works
    ;;
esac

AC_MSG_CHECKING([whether system supports dynamic ptys])
if test -d /dev/pts && ls -d /dev/ptmx > /dev/null 2>&1; then
  AC_MSG_RESULT([yes])
  AC_DEFINE([HAVE_DEV_PTMX],[1],
            [Define to 1 if dynamic ptys exist and are supported.])
else
  AC_MSG_RESULT([no])
fi

AC_FUNC_FORK

if test "x${ac_cv_func_vfork_works}" = "xyes"; then
  case ${canonical} in
    *-darwin[[56789]].* | *-darwin1[[012345]].*)
      AC_MSG_NOTICE([looks like the darwin vfork issues have cleared up])
      AC_DEFINE([PUMA_VFORK_ISSUES_CLEARED_UP],[1],
                [Define to 1 on versions of Mac OS X newer than Puma with a working vfork().])
      ;;
  esac
fi

AC_CHECK_FUNCS([snprintf])dnl

dnl# Check for glib.  This differs from other library checks in that
dnl# Emacs need not link to glib unless some other library is already
dnl# linking to glib.  Although glib provides no facilities that Emacs
dnl# needs for its own purposes, when glib is present Emacs needs to
dnl# use primitives like g_main_context_query to avoid clashing with
dnl# glib at a low level.
dnl#
dnl# Check this late, since it depends on what other libraries
dnl# (lrsvg, Gtk+, and other things appearing in ${GTK_CFLAGS})
dnl# Emacs uses.
XGSELOBJ=""
OLDCFLAGS="${CFLAGS}"
OLDLIBS="${LIBS}"
## set these in two steps, to prevent lines from growing too long:
CFLAGS="${CFLAGS} ${GTK_CFLAGS} ${RSVG_CFLAGS} ${DBUS_CFLAGS}"
LIBS="${LIBS} ${GTK_LIBS} ${RSVG_LIBS} ${DBUS_LIBS} ${SETTINGS_LIBS}"
CFLAGS="${CFLAGS} ${SETTINGS_CFLAGS} ${GFILENOTIFY_CFLAGS}"
LIBS="${LIBS} ${GFILENOTIFY_LIBS}"
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
CFLAGS="${OLDCFLAGS}"
LIBS="${OLDLIBS}"
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

dnl# The test for size_t is suppressed, see above where the rest of the
dnl# typedefs are checked. The test for mbstate_t can stay here though:
unset ac_cv_type_mbstate_t
AC_TYPE_MBSTATE_T

dnl# Restrict could probably be used effectively other than in "regex.c".
dnl# Note that there used to also be a hand-written test here for the
dnl# keyword in general, but that should now be covered by the call to
dnl# AC\_C\_RESTRICT farther above, so instead just test the usage of the
dnl# keyword with arrays:
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
            [Name of the file to open to get a null file or data sink.])dnl
## just assume based on system:
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

  AC_DEFINE([IS_ANY_SEP(_c_)],[(IS_DIRECTORY_SEP(_c_))],
    [Returns true if character is any form of separator.])
fi

AH_TEMPLATE([NO_EDITRES],[Define if XEditRes should not be used.])dnl

case ${opsys} in
  aix4-2)
    dnl# Unfortunately without libXmu we cannot support EditRes.
    if test "x${ac_cv_lib_Xmu_XmuConvertStandardSelection}" != "xyes"; then
      AC_DEFINE([NO_EDITRES],[1])
    elif test "x${ac_cv_search_XmuConvertStandardSelection}" = "xno"; then
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
  hpux* | irix6-5 | nacl | openbsd | sol2* | unixware )
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
AH_TEMPLATE([PTY_ITERATION],[How to iterate over PTYs.])dnl
dnl# Only used if !PTY_ITERATION. Iterate from FIRST_PTY_LETTER to z,
dnl# trying suffixes 0-16.
AH_TEMPLATE([FIRST_PTY_LETTER],[Letter to use in finding device name of
  first PTY, if PTYs are supported.])dnl
AH_TEMPLATE([PTY_OPEN],[How to open a PTY, if non-standard.])dnl
AH_TEMPLATE([PTY_NAME_SPRINTF],[How to get the device name of the control
  end of a PTY, if non-standard.])dnl
AH_TEMPLATE([PTY_TTY_NAME_SPRINTF],[How to get device name of the tty
  end of a PTY, if non-standard.])dnl

case ${opsys} in
  aix4-2 )
    AC_DEFINE([PTY_ITERATION],[int c; for (c = 0; !c ; c++)])dnl
    dnl# You allocate a pty by opening /dev/ptc to get the master side.
    dnl# To get the name of the slave side, you just ttyname() the master
    dnl# side.
    AC_DEFINE([PTY_NAME_SPRINTF],[strcpy(pty_name, "/dev/ptc");])dnl
    AC_DEFINE([PTY_TTY_NAME_SPRINTF],[strcpy(pty_name, ttyname(fd));])
    ;;
  cygwin )
    AC_DEFINE([PTY_ITERATION],[int i; for (i = 0; i < 1; i++)])dnl
    dnl# multi-line AC_DEFINEs are hard. Oh well.
    AC_DEFINE([PTY_OPEN],[ do { int dummy; sigset_t blocked, procmask; sigemptyset(&blocked); sigaddset(&blocked, SIGCHLD); pthread_sigmask(SIG_BLOCK, &blocked, &procmask); if (-1 == openpty(&fd, &dummy, pty_name, 0, 0)) fd = -1; pthread_sigmask(SIG_SETMASK, &procmask, 0); if (fd >= 0) emacs_close(dummy); } while (false)])
    AC_DEFINE([PTY_NAME_SPRINTF],[])dnl
    AC_DEFINE([PTY_TTY_NAME_SPRINTF],[])
    ;;

  dnl# FIXME?  Maybe use same as freebsd - see bug#12040.
  darwin )
    AC_DEFINE([PTY_ITERATION],[int i; for (i = 0; i < 1; i++)])dnl
    dnl# Not used, because PTY_ITERATION is defined:
    AC_DEFINE([FIRST_PTY_LETTER],['p'])dnl
    dnl# Note that openpty may fork via grantpt on Mac OS X 10.4/Darwin 8.
    dnl# But we do NOT have to block SIGCHLD because it is blocked in the
    dnl# implementation of grantpt:
    AC_DEFINE([PTY_OPEN],[ do { int slave; if (openpty(&fd, &slave, pty_name, NULL, NULL) == -1) fd = -1; else emacs_close(slave); } while (false)])
    AC_DEFINE([PTY_NAME_SPRINTF],[])dnl
    AC_DEFINE([PTY_TTY_NAME_SPRINTF],[])
    ;;
  gnu | openbsd )
    AC_DEFINE([FIRST_PTY_LETTER],['p'])
    ;;
  gnu-linux | gnu-kfreebsd | dragonfly | freebsd | netbsd | nacl )
    dnl# if HAVE_GRANTPT
    if test "x${ac_cv_func_grantpt}" = "xyes"; then
      AC_DEFINE([UNIX98_PTYS],[1],[Define if the system has Unix98 PTYs.])
      AC_DEFINE([PTY_ITERATION],[int i; for (i = 0; i < 1; i++)])dnl
      dnl# Note that grantpt and unlockpt may fork. We must block SIGCHLD
      dnl# to prevent sigchld_handler from intercepting the child's death.
      AC_DEFINE([PTY_TTY_NAME_SPRINTF],[{ char *ptyname = 0; sigset_t blocked; sigemptyset(&blocked); sigaddset(&blocked, SIGCHLD); pthread_sigmask(SIG_BLOCK, &blocked, 0); if ((grantpt(fd) != -1) && (unlockpt(fd) != -1)) ptyname = ptsname(fd); pthread_sigmask(SIG_UNBLOCK, &blocked, 0); if (!ptyname) { emacs_close(fd); return -1; } snprintf(pty_name, PTY_NAME_SIZE, "%s", ptyname); }])
      dnl# if HAVE_POSIX_OPENPT
      if test "x${ac_cv_func_posix_openpt}" = "xyes"; then
        AC_DEFINE(PTY_OPEN, [do { fd = posix_openpt (O_RDWR | O_CLOEXEC | O_NOCTTY); if (fd < 0 && errno == EINVAL) fd = posix_openpt (O_RDWR | O_NOCTTY); } while (false)])
        AC_DEFINE([PTY_NAME_SPRINTF],[])
      dnl# if HAVE_GETPT
      elif test "x${ac_cv_func_getpt}" = "xyes"; then
        AC_DEFINE([PTY_OPEN],[fd = getpt()])dnl
        AC_DEFINE([PTY_NAME_SPRINTF],[])
      else
        AC_DEFINE([PTY_NAME_SPRINTF],[strcpy(pty_name, "/dev/ptmx");])
      fi
    else
      AC_DEFINE([FIRST_PTY_LETTER],['p'])
    fi
    ;;
  hpux*)
    AC_DEFINE([FIRST_PTY_LETTER],['p'])dnl
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
    AC_DEFINE([PTY_ITERATION],[])dnl
    dnl# Not used, because PTY_ITERATION is defined:
    AC_DEFINE([FIRST_PTY_LETTER],['q'])dnl
    AC_DEFINE([PTY_OPEN],[ { struct sigaction ocstat, cstat; struct stat stb; char * name; sigemptyset(&cstat.sa_mask); cstat.sa_handler = SIG_DFL; cstat.sa_flags = 0; sigaction(SIGCHLD, &cstat, &ocstat); name = _getpty(&fd, (O_RDWR | O_NDELAY), 0600, 0); sigaction(SIGCHLD, &ocstat, (struct sigaction *)0); if (name == 0) return -1; if (fd < 0) return -1; if (fstat(fd, &stb) < 0) return -1; strcpy(pty_name, name); }])
    dnl# No need to get the pty name at all:
    AC_DEFINE([PTY_NAME_SPRINTF],[])dnl
    dnl# No need to use sprintf to get the tty name--we get that from the
    dnl# _getpty() function:
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
dnl# (because unixware does separate things in both)
case ${opsys} in
  sol2* | unixware )
    dnl# This change means that we do NOT loop through allocate_pty too
    dnl# many times in the (rare) event of a failure.
    AC_DEFINE([FIRST_PTY_LETTER],['z'])dnl
    AC_DEFINE([PTY_NAME_SPRINTF],[strcpy(pty_name, "/dev/ptmx");])dnl
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
  dnl# FIXME: Does gnu-kfreebsd have linux/version.h?  It seems unlikely...
  gnu-linux | gnu-kfreebsd )
    dnl# "_ONCE" causes test to jump out of the conditional and be done at
    dnl# wrong location:
    AC_CHECK_HEADERS([linux/version.h])dnl
    ## now we can do the conftest that uses that header:
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
AH_TEMPLATE([DATA_SEG_BITS],
  [Extra bits to be or'd in with any pointers stored in a Lisp_Object.])dnl
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
  darwin)
    AC_DEFINE([TAB3],[OXTABS])
    ;;
  gnu | dragonfly | freebsd | netbsd | openbsd )
    AC_DEFINE([TABDLY],[OXTABS],[Undocumented.])dnl
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
	 if (! _setjmp(j))
	   _longjmp(j, 1);]])],
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

# We need all of these features to handle C stack overflows.
if test "x${ac_cv_header_sys_resource_h}" = "xyes" -a \
	"x${ac_cv_func_getrlimit}" = "xyes" -a \
	"x${emacs_cv_func_sigsetjmp}" = "xyes" -a \
	"x${emacs_cv_alternate_stack}" = "xyes"; then
  AC_DEFINE([HAVE_STACK_OVERFLOW_HANDLING],[1],
    [Define to 1 if C stack overflow can be handled in some cases.])
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
dnl# (because sol2 is in both, and needs both to happen instead of just 1)
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
AH_TEMPLATE([DOS_NT],[Define if the system is MS DOS or MS Windows.])dnl
AH_TEMPLATE([MSDOS],[Define if the system is MS DOS.])dnl
AH_TEMPLATE([USG],[Define if the system is compatible with System III.])dnl
AH_TEMPLATE([USG5_4],
            [Define if system is compatible with System V Release 4])dnl

case ${opsys} in
  aix4-2)
    AC_DEFINE([USG],[])dnl
    dnl# This symbol should be defined on AIX Version 3 ???????
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
    dnl# Not __APPLE__, as this may not be defined on non-OSX Darwin.
    dnl# (lol, if only people used non-OSX Darwin... one can dream...)
    dnl# Not DARWIN, because Panther and lower CoreFoundation.h use DARWIN
    dnl# to distinguish OS X from pure Darwin.
    AC_DEFINE([DARWIN_OS],[],[Define if the system is Darwin.])
    dnl# We need a way to distinguish OS X from pure Darwin as well:
    if test "x${host_vendor}" = "xapple" && test "x${ac_cv_have_decl___MACH__}" = "xyes"; then
      export MAC_OSX_DEFINED=1
      AC_DEFINE([MAC_OSX],[1],[Define to 1 if the system is the darwin distribution known as Mac OS X.])
    else
      test -n "${host_vendor}" ##...and do something else...
    fi
    ;;
  gnu-linux | gnu-kfreebsd )
    AC_DEFINE([USG],[])dnl
    AC_DEFINE([GNU_LINUX],[],
              [Define if this system is compatible with GNU/Linux.])
    ;;
  hpux*)
    AC_DEFINE([USG],[])dnl
    AC_DEFINE([HPUX],[],[Define if the system is HPUX.])
    ;;
  irix6-5)
    AC_DEFINE([USG],[])dnl
    AC_DEFINE([USG5_4],[])dnl
    AC_DEFINE([IRIX6_5],[],[Define if the system is IRIX.])
    ;;
  mingw32)
    AC_DEFINE([DOS_NT],[])dnl
    AC_DEFINE([WINDOWSNT],[1],[Define if compiling for native MS Windows.])
    if test "x${ac_enable_checking}" != "x"; then
      AC_DEFINE([EMACSDEBUG],[1],
                [Define to 1 to enable w32 debug facilities.])
    fi
    ;;
  sol2*)
    AC_DEFINE([USG],[])dnl
    AC_DEFINE([USG5_4],[])dnl
    AC_DEFINE([SOLARIS2],[],[Define if the system is Solaris.])
    ;;
  unixware)
    AC_DEFINE([USG],[])dnl
    AC_DEFINE([USG5_4],[])dnl
    AC_DEFINE([SCO_UNIXWARE_IS_EVIL],[1],[Just because.])
    ;;
esac

dnl# adding "_ONCE" to macro causes test to be done at wrong location:
AC_CHECK_HEADERS([sys/filio.h])
AC_CACHE_CHECK([for usable FIONREAD],[emacs_cv_usable_FIONREAD],
  [case ${opsys} in
     aix4-2 | nacl)
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
  dnl# effective starting with glib-2.36:
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
# First save the current values to some other, unused, variables though:
if test "x${FAKE_CFLAGS}" = "x"; then
  test -z "${FAKE_CFLAGS}" && export FAKE_CFLAGS="${CFLAGS}"
  AC_SUBST([FAKE_CFLAGS])dnl
  ## ok
fi
if test "x${FAKE_CPPFLAGS}" = "x"; then
  test -z "${FAKE_CPPFLAGS}" && export FAKE_CPPFLAGS="${CPPFLAGS}"
  AC_SUBST([FAKE_CPPFLAGS])dnl
  ## ok
fi
if test "x${FAKE_LDFLAGS}" = "x"; then
  test -z "${FAKE_LDFLAGS}" && export FAKE_LDFLAGS="${LDFLAGS}"
  AC_SUBST([FAKE_LDFLAGS])dnl
  ## ok
fi
if test "x${FAKE_LIBS}" = "x"; then
  test -z "${FAKE_LIBS}" && export FAKE_LIBS="${LIBS}"
  AC_SUBST([FAKE_LIBS])dnl
  ## ok
fi
CFLAGS="${REAL_CFLAGS}"
CPPFLAGS="${REAL_CPPFLAGS}"
LDFLAGS="${REAL_LDFLAGS}"
LIBS="${REAL_LIBS}"

## Hack to detect a buggy GCC version.
if test "x${GCC}" = "xyes" \
   && test x"`${CC} --version 2> /dev/null | grep 'gcc.* 4.5.0'`" != x"" \
   && test x"`echo ${CFLAGS} | grep '\-O@<:@23@:>@'`" != x"" \
   && test x"`echo ${CFLAGS} | grep '\-fno-optimize-sibling-calls'`" = x""; then
   AC_MSG_ERROR([GCC 4.5.0 has problems compiling Emacs; see etc/PROBLEMS'])
fi

#### Find out which version of Emacs this is.
if test -e ${srcdir}/lisp/version.el; then
[version=`grep 'defconst[	 ]*emacs-version' ${srcdir}/lisp/version.el \
	 | sed -e 's/^[^"]*"\([^"]*\)".*$/\1/'`]
elif test -e ./lisp/version.el; then
[version=`grep 'defconst[	 ]*emacs-version' ./lisp/version.el \
	 | sed -e 's/^[^"]*"\([^"]*\)".*$/\1/'`]
fi
if test x"${version}" = x""; then
  if test "x${PACKAGE_VERSION}" != "x"; then
    if test -e ${srcdir}/lisp/version.el; then
      if test -r ${srcdir}/lisp/version.el; then
        AC_MSG_NOTICE([cannot find current emacs version in `${srcdir}/lisp/version.el', using ${PACKAGE_VERSION} instead.])
      elif test ! -r ${srcdir}/lisp/version.el; then
        AC_MSG_WARN([cannot read current emacs version from `${srcdir}/lisp/version.el', using ${PACKAGE_VERSION} instead.])
      fi
    fi
    test -z "${version}" && test -n "${PACKAGE_VERSION}" && export version="${PACKAGE_VERSION}"
  else
    AC_MSG_ERROR([cannot find current emacs version in `${srcdir}/lisp/version.el', and PACKAGE_VERSION is undefined.])
  fi
else
  test -n "${version}"
fi

test -z "${version}" && test -n "${PACKAGE_VERSION}" && export version="${PACKAGE_VERSION}"
major_vers="`echo ${version} | cut -d. -f1`"
AC_SUBST([major_vers])dnl

if test "x${gl_better_debugging}" = "xyes"; then
  ## See release notes for GCC 3.3 and 3.4:
  if test ${major_vers} -lt 22; then
    gl_WARN_ADD([-fno-zero-initialized-in-bss],[BETTER_DEBUG_CFLAGS])
  else
    AC_MSG_NOTICE([Emacs is new enough (${major_vers} >= 22)])
  fi
fi

copyright_year="2014"
copyright="Copyright (C) ${copyright_year} Free Software Foundation, Inc."
copyright_holder="Free Software Foundation, Inc."
AC_DEFINE_UNQUOTED([COPYRIGHT],["${copyright}"],
  [Short copyright string for this version of Emacs.])
AC_DEFINE_UNQUOTED([COPYRIGHT_HOLDER],["${copyright_holder}"],
  [Alias for the Free Software Foundation])
AC_SUBST([copyright])dnl
AC_SUBST([copyright_holder])dnl
AC_SUBST([copyright_year])dnl

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
AC_SUBST([MAC_CFLAGS])dnl
AC_SUBST([GNUSTEP_CFLAGS])dnl
AC_SUBST([CFLAGS])dnl
dnl## Used in lwlib/Makefile.in:
AC_SUBST([X_TOOLKIT_TYPE])dnl
AC_SUBST([machfile])dnl
AC_SUBST([opsysfile])dnl
AC_SUBST([carbon_appdir])dnl
AC_SUBST([ns_appdir])dnl
AC_SUBST([ns_appbindir])dnl
AC_SUBST([ns_appresdir])dnl
AC_SUBST([ns_appsrc])dnl
AC_SUBST([GNU_OBJC_CFLAGS])dnl
AC_SUBST([OTHER_FILES])dnl

if test -n "${term_header}"; then
    AC_DEFINE_UNQUOTED([TERM_HEADER],["${term_header}"],
        [Define to the header for the built-in window system.])
else
    ## this is the same condition as used below to define
    ## HAVE_WINDOW_SYSTEM:
    if test "x${window_system}" != "xnone"; then
        AC_MSG_WARN([you seem to be using a window system, but no header seems to be defined for it.])
    fi
fi

AC_DEFINE_UNQUOTED([EMACS_CONFIGURATION],["${canonical}"],
		   [Define to the canonical Emacs configuration name.])dnl
AC_DEFINE_UNQUOTED([EMACS_CONFIG_OPTIONS],["${emacs_config_options}"],
		   [Define to the options passed to configure.])dnl
AC_DEFINE_UNQUOTED([config_machfile],["${machfile}"],
		   [Define to the used machine dependent file.])dnl
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
  else
    AC_MSG_WARN([not sure what to use for libw with unknown toolkit ${USE_X_TOOLKIT}])
  fi
fi
AC_SUBST([WIDGET_OBJ])dnl

AC_MSG_CHECKING([libs to use for TOOLKIT_LIBW])
TOOLKIT_LIBW=""
case "${USE_X_TOOLKIT}" in
  MOTIF) TOOLKIT_LIBW="${MOTIF_LIBW}" ;;
  LUCID) TOOLKIT_LIBW="${LUCID_LIBW}" ;;
  none) test "x${HAVE_GTK}" = "xyes" && export TOOLKIT_LIBW="${GTK_LIBS}"
        ;;
esac
AC_MSG_RESULT(['${TOOLKIT_LIBW}'])dnl
AC_SUBST([TOOLKIT_LIBW])dnl

AC_MSG_CHECKING([libs to use for LIBXT_OTHER])
LIBXT_OTHER=""
if test "x${opsys}" != "xmingw32"; then
  if test "x${USE_X_TOOLKIT}" = "xnone"; then
    LIBXT_OTHER="\$(LIBXSM)"
    if test "x${LIBXMU}" != "x"; then
      test -n "${LIBXMU}" && LIBXT_OTHER="${LIBXT_OTHER} \$(LIBXMU)"
    fi
    if test "x${LIBXTR6}" != "x"; then
      test -n "${LIBXTR6}" && LIBXT_OTHER="${LIBXT_OTHER} \$(LIBXTR6)"
    fi
  else
    LIBXT_OTHER="\$(LIBXMU) -lXt \$(LIBXTR6) -lXext"
  fi
fi
AC_MSG_RESULT([${LIBXT_OTHER}])dnl
AC_SUBST([LIBXT_OTHER])dnl

AC_MSG_CHECKING([libs to use for LIBX_OTHER])
LIBX_OTHER=""
if test "x${HAVE_X11}" = "xyes"; then
  AC_DEFINE([HAVE_X11],[1],
	    [Define to 1 if you want to use version 11 of X windows.])
  LIBX_OTHER="\$(LIBXT) \$(LIBX_EXTRA)"
else
  LIBX_OTHER=""
fi
AC_MSG_RESULT([${LIBX_OTHER}])dnl
AC_SUBST([LIBX_OTHER])dnl

AC_MSG_CHECKING([what to use for LIBXMENU])
LIBXMENU=""
if test "x${HAVE_GTK}" = "xyes" || test "x${HAVE_X11}" != "xyes"; then
  LIBXMENU=""
elif test "x${USE_X_TOOLKIT}" = "xnone"; then
  LIBXMENU='$(oldXMenudir)/libXMenu11.a'
else
  LIBXMENU='$(lwlibdir)/liblw.a'
fi
if test "x${LIBXMENU}" != "x"; then
  test -n "${LIBXMENU}"
  AC_MSG_RESULT([${LIBXMENU}])
else
  test -z "${LIBXMENU}"
  AC_MSG_RESULT([nothing])
fi
AC_SUBST([LIBXMENU])dnl

if test "x${HAVE_MENUS}" = "xyes"; then
  AC_DEFINE([HAVE_MENUS],[1],
   [Define to 1 if you have mouse menus.
    (This is automatic if you use X, but the option to specify it remains.)
    It is also defined with other window systems that support xmenu.c.])
fi

AC_CACHE_CHECK([for struct alignment],
  [emacs_cv_struct_alignment],
  [AC_COMPILE_IFELSE(
     [AC_LANG_PROGRAM([[#include <stddef.h>
     		        struct __attribute__ ((aligned (8))) s { char c; };
     			struct t { char c; struct s s; };
			char verify[offsetof (struct t, s) == 8 ? 1 : -1];
		      ]])],
     [emacs_cv_struct_alignment=yes],
     [emacs_cv_struct_alignment=no])])dnl
  ## definition to go with that:
if test "x${emacs_cv_struct_alignment}" = "xyes"; then
  AC_DEFINE([HAVE_STRUCT_ATTRIBUTE_ALIGNED],[1],
    [Define to 1 if 'struct __attribute__ ((aligned (N)))' aligns the
     structure to an N-byte boundary.])
fi

if test "x${GNU_MALLOC}" = "xyes"; then
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
if test "x${ac_cv_c_inline}" = "x"; then
  test -z "${ac_cv_c_inline}"
  AC_C_INLINE
fi

dnl# gettext:
AC_MSG_CHECKING([gettext])
AC_MSG_RESULT([ ])
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.19.7])
# Solaris requires -lintl if you want strerror (which calls dgettext)
# to return localized messages.
if test "x${ac_cv_func_strerror}" = "xyes" || test "x${ac_cv_search_strerror}" != "x"; then
  test -n "${ac_cv_func_strerror}" || test -n "${ac_cv_search_strerror}" || test "x${opsys_family}" = "xsun"
  AC_CHECK_LIB([intl],[dgettext])
fi
# Also check for -lintl anyways, but with different functions:
AC_CHECK_LIB([intl],[gettext],[:],[
  AC_CHECK_LIB([intl],[libintl_gettext],[:],[
    ## oh no, neither worked!
    AC_CHECK_LIB([intl],[main])dnl
  ])dnl#end case where symbols are prefixed with "libintl_"
])dnl#end case where symbols are unprefixed
  ## just in case:
AC_SEARCH_LIBS([libintl_gettext],[intl asprintf gettextlib gettextpo \
                                  gettextsrc gettext libintl lintl \
                                  gnu c iberty iconv])dnl

# If user asks to omit features, disable optional features that gnulib
# might otherwise enable.
if test "x${with_features}" = "xno" && test "x${enable_acl}" != "xyes"; then
  enable_acl=no
elif test "x${enable_acl}" != "xno" && test "x${ac_cv_func_acl_delete_def_file}" = "x"; then
  test -z "${ac_cv_func_acl_delete_def_file}"
  AC_CHECK_FUNCS([acl_delete_def_file])dnl
  ## might want to do this somewhere else...
fi

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
gl_ASSERT_NO_GNULIB_POSIXCHECK([])dnl
gl_ASSERT_NO_GNULIB_TESTS([])dnl
gl_INIT
CFLAGS="${SAVE_CFLAGS}"
LIBS="${SAVE_LIBS}"
AC_MSG_NOTICE([should be done with gnulib checks now])

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
unset f
## ...and in some other library variables:
# (not cflags though, because they could have ${isystem} splitting them up)
# 1: SETTINGS_LIBS:
f=""
for i in ${SETTINGS_LIBS}; do
  case " ${f} " in
    *" ${i} "*) ;;
    *) f="${f} ${i}" ;;
  esac
done
SETTINGS_LIBS="${f}"
unset f
# 2: LIBOTF_LIBS:
f=""
for i in ${LIBOTF_LIBS}; do
  case " ${f} " in
    *" ${i} "*) ;;
    *) f="${f} ${i}" ;;
  esac
done
LIBOTF_LIBS="${f}"
unset f

if test "x${opsys}" = "xmingw32"; then
  CPPFLAGS="${CPPFLAGS} -DUSE_CRT_DLL=1 -I \${abs_top_srcdir}/nt/inc"
  # Remove unneeded switches from the value of CC that goes to Makefiles
  CC=`echo ${CC} | sed -e "s,${GCC_TEST_OPTIONS},,"`
fi

case "${opsys}" in
  aix4-2) LD_SWITCH_SYSTEM_TEMACS="-Wl,-bnodelcsect" ;;

  cygwin) LD_SWITCH_SYSTEM_TEMACS="-Wl,-stack,0x00800000" ;;

  darwin)
   libs_nsgui=""
   ## The -headerpad option tells ld (see man page) to leave room at the
   ## end of the header for adding load commands. Needed for dumping.
   ## 0x1000 is enough for roughly 52 load commands on the x86_64
   ## architecture (where they are 78 bytes each); under Cocoa, at one
   ## point at least 31 commands were required. The actual number of
   ## load commands added is not consistent but normally ranges from
   ## about 14 to about 34. Setting it high gets us plenty of slop and
   ## only costs about 1.5K of wasted binary space.
   headerpad_extra=1000
   # libs_nsgui is equivalent to libs_macgui from Y.M.'s Mac port:
   if test "x${HAVE_NS}" = "xyes"; then
     libs_nsgui="-framework AppKit"
     if test "x${NS_IMPL_COCOA}" = "xyes"; then
        libs_nsgui="${libs_nsgui} -framework IOKit"
     fi
     if test "x${headerpad_extra}" = "x"; then
       test -z "${headerpad_extra}" && export headerpad_extra=6C8
     fi
   elif test "x${HAVE_CARBON}" = "xyes" && test "x${LIBS_CARBON}" != "x"; then
     test -n "${LIBS_CARBON}" && libs_nsgui="${LIBS_CARBON}"
     ## This used to be 0x690, which was the total size of 30 segment load
     ## commands (at 56 each), but we bumped it to 0xBD0 because of all the
     ## additional frameworks added to LIBS_CARBON above:
     headerpad_extra=BD0
   else
     libs_nsgui=""
     headerpad_extra=1000
   fi
   if test "x${LD_FORCE_NO_PREBIND}" = "xYES" || test "x${LD_PREBIND}" = "xNO"; then
     prebind=""
   else
     prebind='-prebind'
   fi
   LD_SWITCH_SYSTEM_TEMACS="-fno-pie ${prebind} ${libs_nsgui} -Xlinker -headerpad -Xlinker ${headerpad_extra}"
   ## (why not just use '-headerpad_max_install_names'? Would that not be
   ##  more in line with GNU policy of preferring dynamic limits over
   ##  arbitrary, hard-coded ones?)

   if test "x${LD_NO_PIE}" != "xNO"; then
     echo "assuming ld wants -no_pie; set LD_NO_PIE to NO to override"
     ## Apple uses this in their Makefile, and other platforms use
     ## analogous flags:
     LD_SWITCH_SYSTEM_TEMACS="${LD_SWITCH_SYSTEM_TEMACS} -Wl,-no_pie"
     ##FIXME: older Apple linkers fail to understand it though...
   fi

   ## check the version to see if this is ld64; the argument to `tr` should
   ## be fine as long as this stays inside an m4-quotation block:
   LD_IS_LD64="`${LD} -v 2>&1 | grep 'ld64' | tr -d [:blank:]`"
   ## if so, add an ld64-specific flag that Apple uses in their Makefile:
   if test "x${LD_IS_LD64}" != "x"; then
     ## put it in a different variable though, until I find out which
     ## versions it works with:
     test -n "${LD_IS_LD64}" && LD64_SWITCH_SYSTEM_TEMACS="-Wl,-no_function_starts"
     ## (subst it outside of this block, because of m4-quotation)
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
dnl# now subst them:
AC_SUBST([ADDSECTION])dnl
AC_SUBST([TEMACS_POST_LINK])dnl
AC_SUBST([EMACS_HEAPSIZE])dnl

## Common for all window systems:
if test "x${window_system}" != "xnone"; then
  AC_DEFINE([HAVE_WINDOW_SYSTEM],[1],[Define if you have a window system.])
  AC_DEFINE([POLL_FOR_INPUT],[1],
            [Define to 1 if you poll periodically to detect C-g.])
  WINDOW_SYSTEM_OBJ="fontset.o fringe.o image.o"
fi
dnl# now subst it:
AC_SUBST([WINDOW_SYSTEM_OBJ])dnl

## this cache value should be set by now, probably by gnulib:
if test "x${ac_cv_type_socklen_t}" = "xyes"; then
  AC_DEFINE([HAVE_SOCKLEN_T],[1],
            [Define to 1 if `socklen_t' is available.])
fi

AH_TOP([
/* src/config.h: GNU Emacs site configuration template file.
 *
 * Copyright (C) 1988, 1993-1994, 1999-2002, 2004-2014
 *  Free Software Foundation, Inc.
 * Copyright (C) 2014-2016 Eric Gallager
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
/* in case "conf_post.h" left out a few things: */
#ifdef HAVE_X_WINDOWS
# ifndef HAVE_WINDOW_SYSTEM
#  define HAVE_WINDOW_SYSTEM
# endif /* !HAVE_WINDOW_SYSTEM */
# ifndef MULTI_KBOARD
#  define MULTI_KBOARD
# endif /* !MULTI_KBOARD */
# ifndef HAVE_MOUSE
#  define HAVE_MOUSE
# endif /* !HAVE_MOUSE */
#endif /* HAVE_X_WINDOWS */

/* see the prototype-checking macro: */
#ifndef __P
# if defined(PROTOTYPES) || defined(__PROTOTYPES)
#  define __P(args) args
# else
#  define __P(args) ()
# endif  /* GCC.  */
#endif /* __P */

/* MAC_OS is used to conditionally compile code common to both MAC_OS8
 * and MAC_OSX.  */
#if defined(MAC_OS8) || (defined(MAC_OSX) && !defined(HAVE_NS))
# if defined(HAVE_CARBON) && defined(ALLOW_OBSOLETE_CARBON)
#  if !defined(__LP64__) || !__LP64__
#   ifndef MAC_OS
#    define MAC_OS
#   endif /* !MAC_OS */
#  endif /* !__LP64__ */
# endif /* HAVE_CARBON && ALLOW_OBSOLETE_CARBON */
#endif /* MAC_OS8 || (MAC_OSX && !HAVE_NS) */

/* Some other debugging defines: */
#if defined(ENABLE_CHECKING) || defined(GLYPH_DEBUG) || defined(EMACSDEBUG)
/* The check for the '_IN_GNULIB' macro is to prevent the gnulib mktime.c
 * from breaking: */
# if !defined(DEBUG) && !defined(_IN_GNULIB)
#  define DEBUG 1
# endif /* !DEBUG && !_IN_GNULIB */
# ifndef _DEBUG
#  define _DEBUG 1
# endif /* !_DEBUG */
#endif /* ENABLE_CHECKING || GLYPH_DEBUG || EMACSDEBUG */

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
    echo "Trying to make sure LDFLAGS are set..."
    test -z "${LDFLAGS}" && test -n "${prefix}" && test -d "${prefix}/lib" && export LDFLAGS="-L${prefix}/lib"
    test -z "${LDFLAGS}" && test -n "${with_libintl_prefix}" && test -d "${with_libintl_prefix}/lib" && export LDFLAGS="-L${with_libintl_prefix}/lib"
    test -z "${LDFLAGS}" && test -n "${with_libiconv_prefix}" && test -d "${with_libiconv_prefix}/lib" && export LDFLAGS="-L${with_libiconv_prefix}/lib"
    test -z "${LDFLAGS}" && test -n "${x_libraries}" && test -d "${x_libraries}" && export LDFLAGS="-L${x_libraries}"
  elif test "x${prefix}" = "xNONE" && test ! -d "NONE"; then
    echo "Warning: invalid prefix"
  else
    echo "Well, this situation is improbable..."
  fi
else
  echo "LDFLAGS are already set."
fi

## multi-line call to 'echo'; closing quote is farther down @ end of block:
echo "
Configured for \`${canonical}'.

  Where should the build process find the source code? ${srcdir}
  What operating system and machine description files should Emacs use?
        \`${opsysfile}' and \`${machfile}'
  What compiler should emacs be built with?            ${CC} ${CFLAGS}
  What preprocessor should emacs be built with?        ${CPP} ${CPPFLAGS}
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
]
dnl# temporary break in final m4-quotation block

optsep=""
emacs_config_features=""
for opt in XAW3D XPM JPEG TIFF GIF PNG RSVG IMAGEMAGICK SOUND GPM DBUS \
  GCONF GSETTINGS NOTIFY ACL LIBSELINUX GNUTLS LIBXML2 FREETYPE M17N_FLT \
  LIBOTF XFT ZLIB; do

    case ${opt} in
      NOTIFY|ACL) eval val=\${${opt}_SUMMARY} ;;
      *) eval val=\${HAVE_${opt}} ;;
    esac
    test x"${val}" = x"no" && continue
    AS_VAR_APPEND([emacs_config_features],["${optsep}${opt}"])
    optsep=' '
done
AC_DEFINE_UNQUOTED([EMACS_CONFIG_FEATURES],["${emacs_config_features}"],
  [Summary of some of the main features enabled by configure.])dnl

dnl# back to the final m4-quotation block:
[
echo ""
echo "libraries used:"
## graphics libraries:
echo "  Does Emacs use -lXaw3d?                     ${HAVE_XAW3D}"
echo "  Does Emacs use -lXpm?                       ${HAVE_XPM}"
echo "  Does Emacs use -ljpeg?                      ${HAVE_JPEG}"
echo "  Does Emacs use -ltiff?                      ${HAVE_TIFF}"
echo "  Does Emacs use a gif library?               ${HAVE_GIF} ${LIBGIF}"
echo "  Does Emacs use -lpng?                       ${HAVE_PNG} ${LIBPNG}"
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

if test "x${USE_XASSERTS}" = "xyes"; then
   echo "  Compiling with xasserts turned on."
   CPPFLAGS="${CPPFLAGS} -DXASSERTS=1"
   echo ""
fi

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

if test "x${with_carbon}" = "xyes"; then
   echo ""
   echo "You must run \"${MAKE-make} install\" in order to test the built Carbon application.
The installed application will go to mac/Emacs.app and can be
run or moved from there."
   if test "x${carbon_appdir_x}" != "x" && test "x${carbon_appdir_x}" != "xno"; then
      echo "The Carbon application will be fully self-contained."
    else
      echo "The lisp resources for the application will be installed under ${prefix}.
You may need to run \"${MAKE-make} install\" with sudo. The application will fail
to run if these resources are not installed."
   fi
   if test "x${with_wide_int}" = "xyes"; then
      echo "Warning: \`--with-wide-int' is untested on the Mac port."
      echo "Use it at your own risk."
   fi
   echo ""
fi

if test "x${HAVE_NS}" = "xyes"; then
   echo ""
   echo "You must run \"${MAKE-make} install\" in order to test the built Cocoa application.
The installed application will go to nextstep/Emacs.app and can be
run or moved from there."
   if test "x${EN_NS_SELF_CONTAINED}" = "xyes"; then
      echo "The Cocoa application will be fully self-contained."
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

MAC_TEMPLATE_FILES=""
if test "x${HAVE_CARBON}" = "xyes"; then
    AC_CONFIG_FILES([mac/Emacs.app/Contents/Info.plist:mac/templates/Info.plist.in \
    mac/Emacs.app/Contents/Resources/English.lproj/InfoPlist.strings:mac/templates/InfoPlist.strings.in])
    MAC_TEMPLATE_FILES="mac/templates/Info.plist.in mac/templates/InfoPlist.strings.in"
fi
AC_SUBST([MAC_TEMPLATE_FILES])dnl

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

dnl# trunk seems to think that using ${srcdir} in here is okay, but I am
dnl# not quite as sure:
if test -f doc/man/emacs.1.in && test ! -e doc/man/emacs.1; then
  test -n "${srcdir}"
  AC_CONFIG_FILES([doc/man/emacs.1])
fi

dnl# Obviously there is duplication here wrt ${SUBDIR_MAKEFILES}.
dnl# You _can_ use that variable in AC_CONFIG_FILES, so long as any
dnl# directory using automake (i.e. lib/) is explicitly listed and not
dnl# "hidden" in a variable (else you get "no `Makefile.am' found for any
dnl# configure output"). This will work, but you get a config.status that
dnl# is not quite right (see:
dnl# http://lists.gnu.org/archive/html/bug-autoconf/2008-08/msg00028.html).
dnl# That does NOT have any obvious consequences for Emacs, but on the
dnl# whole it seems better to just live with the duplication.
SUBDIR_MAKEFILES="lib/Makefile lib-src/Makefile oldXMenu/Makefile doc/emacs/Makefile doc/Makefile doc/lispintro/Makefile doc/lispref/Makefile doc/man/Makefile doc/misc/Makefile src/Makefile lisp/Makefile lispintro/Makefile lispref/Makefile leim/Makefile lwlib/Makefile man/Makefile mac/Makefile nextstep/Makefile nt/Makefile"

AC_CONFIG_FILES([Makefile lib/Makefile lib-src/Makefile oldXMenu/Makefile \
       doc/Makefile doc/emacs/Makefile doc/lispintro/Makefile \
       doc/lispref/Makefile doc/man/Makefile doc/misc/Makefile \
       src/Makefile lisp/Makefile lispintro/Makefile lispref/Makefile \
       leim/Makefile lwlib/Makefile man/Makefile mac/Makefile \
       nextstep/Makefile nt/Makefile po/Makevars])dnl

### Make the necessary directories, if they do NOT exist.
for dir in etc lisp; do
  test -d ${dir} || mkdir ${dir}
done

dnl# test/ is not present in release tarfiles.
opt_makefile=test/automated/Makefile

if test -f "${srcdir}/${opt_makefile}.in"; then
  SUBDIR_MAKEFILES="${SUBDIR_MAKEFILES} ${opt_makefile}"
  dnl# Again, it is best not to use a variable. Though you can add
  dnl# ",[],[opt_makefile='${opt_makefile}']" and it should work.
  AC_CONFIG_FILES([test/automated/Makefile])
fi

dnl# this subdir is imported from ld64, and used like a library for the
dnl# isemacsvalid executable:
if test -d machocheck || test -d ${srcdir}/machocheck; then
  SUBDIR_MAKEFILES="${SUBDIR_MAKEFILES} machocheck/Makefile"
  AC_CONFIG_FILES([machocheck/Makefile])
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

dnl# N.B.: we have to cheat and use the ac_... version because
dnl# abs_top_srcdir is not yet set, sigh.
dnl# Or we could use ../${srcdir}/src/.gdbinit, or a symlink?
AC_CONFIG_COMMANDS([src/.gdbinit],[
if test ! -f src/.gdbinit && test -f "${srcdir}/src/.gdbinit"; then
  echo "source ${ac_abs_top_srcdir}/src/.gdbinit" > src/.gdbinit
fi
])dnl

dnl# Perhaps this would be better named doc-emacs-emacsver.texi?
dnl# See comments for etc-refcards-emacsver.tex.
dnl# Since we get a doc/emacs directory generated anyway, for the Makefile,
dnl# it is not quite the same.  But we are generating in ${srcdir}.
AC_CONFIG_COMMANDS([doc/emacs/emacsver.texi],[
${MAKE-make} -s --no-print-directory -C doc/emacs doc-emacsver || \
AC_MSG_WARN(['doc/emacs/emacsver.texi' could not be made.])
])dnl

dnl# If we give this the more natural name, etc/refcards/emacsver.texi,
dnl# then a directory etc/refcards is created in the build directory,
dnl# which is probably harmless, but confusing (in out-of-tree builds).
dnl# (If we were to generate etc/refcards/Makefile, this might change.)
dnl# It is really ${srcdir}/etc/refcards/emacsver.tex that we generate.
AC_CONFIG_COMMANDS([etc-refcards-emacsver.tex],[
${MAKE-make} -s MAKEFILE_NAME=do-not-make-Makefile etc-emacsver || \
AC_MSG_WARN(['etc/refcards/emacsver.tex' could not be made.])
])dnl

AC_OUTPUT

test "x${MAKE}" = "xmake" || \
  (test -n "${MAKE}" && echo "configure: Now you can run ${MAKE}.")

# this is one of the longest configure.ac files that I have seen,
# even after splitting off some of the macros into separate files...

# (5385 lines as of the original writing of this comment...)
# (and overall it has ended up growing longer than that too)
