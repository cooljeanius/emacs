## handwritten part of automake Makefile for the parts of gnulib that emacs
## uses (in its lib/ subdir)

BUILT_SOURCES =
CLEANFILES =
EXTRA_DIST =
DISTCLEANFILES = .DS_Store *~
MOSTLYCLEANDIRS =
MOSTLYCLEANFILES =
noinst_HEADERS = 
noinst_LIBRARIES =
noinst_SCRIPTS = autoregnulib.sh
SUFFIXES =

ACLOCAL_AMFLAGS = -I../m4 --install
AUTOMAKE_OPTIONS = 1.11 gnits

AM_CFLAGS = $(PROFILING_CFLAGS) $(GNULIB_WARN_CFLAGS) @MAINT@ $(WERROR_CFLAGS)
AM_CPPFLAGS = -I. -I$(top_srcdir)/lib -I../src -I$(top_srcdir)/src

if BUILDING_FOR_WINDOWSNT
include ../nt/gnulib.mk
else
include gnulib.mk
## remember to update the former to match when the latter is updated.

libgnu_a_SOURCES += openat-die.c save-cwd.c
endif

extraclean: maintainer-clean
	-rm -f *~ \#*
.PHONY: extraclean

if GNU_MAKE
@ifGNUmake@
UserType = Developer
ToolType = Commands
else
NON_GNU_MAKE = true
endif
