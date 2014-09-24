## handwritten part of automake Makefile for the parts of gnulib that emacs
## uses (in its lib/ subdir)

BUILT_SOURCES =
CLEANFILES =
EXTRA_DIST =
DISTCLEANFILES = .DS_Store
MOSTLYCLEANDIRS =
MOSTLYCLEANFILES =
noinst_HEADERS = 
noinst_LIBRARIES =
noinst_SCRIPTS = autoregnulib.sh
SUFFIXES =

AM_CFLAGS = $(PROFILING_CFLAGS) $(GNULIB_WARN_CFLAGS) @MAINT@ $(WERROR_CFLAGS)
AM_CPPFLAGS = -I. -I$(top_srcdir)/lib -I../src -I$(top_srcdir)/src

if BUILDING_FOR_WINDOWSNT
include ../nt/gnulib.mk
else
include gnulib.mk

libgnu_a_SOURCES += openat-die.c save-cwd.c
endif
