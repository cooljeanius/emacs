## handwritten part of automake Makefile for the parts of gnulib that emacs
## uses (in its lib/ subdir)

BUILT_SOURCES =
CLEANFILES =
EXTRA_DIST =
DISTCLEANFILES = .DS_Store *~ glthread/*~
MOSTLYCLEANDIRS =
MOSTLYCLEANFILES =
noinst_HEADERS = 
noinst_LIBRARIES =
noinst_SCRIPTS = autoregnulib.sh
SUFFIXES =

ACLOCAL_AMFLAGS = -I../m4 --install
AUTOMAKE_OPTIONS = 1.11 gnits

DEFS = @DEFS@ -D_IN_GNULIB
AM_CFLAGS = $(PROFILING_CFLAGS) $(GNULIB_WARN_CFLAGS) @MAINT@ $(WERROR_CFLAGS)
AM_CPPFLAGS = -I. -I$(top_srcdir)/lib -I../src -I$(top_srcdir)/src
AM_DEFAULT_INCLUDES = $(AM_CPPFLAGS)

if BUILDING_FOR_WINDOWSNT
include ../nt/gnulib.mk
else
include gnulib.mk
## remember to update the former to match when the latter is updated.

libgnu_a_SOURCES += openat-die.c save-cwd.c
TESTS_ENVIRONMENT = @LOCALCHARSET_TESTS_ENVIRONMENT@
endif

## a rule to make sure that all generated headers are built:
headers: $(BUILT_SOURCES)
	@echo "ensuring all generated headers have been generated..."
	$(AM_V_at)ls *.h
.PHONY: headers

bootstrap-clean: maintainer-clean
	@echo "no bootstrap-clean specific rules yet."
.PHONY: bootstrap-clean

maintainer-clean-local:
	rm -rf glthread || rmdir glthread
.PHONY: maintainer-clean-local

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
