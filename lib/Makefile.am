## handwritten part of automake Makefile for the parts of gnulib that emacs
## uses (in its lib/ subdir)

BUILT_SOURCES =
CLEANFILES = stamp-headers *.gcno */*.gcno
EXTRA_DIST =
DISTCLEANFILES = .DS_Store *~ bitset/*~ glthread/*~ malloc/*~ uniwidth/*~
MOSTLYCLEANDIRS =
MOSTLYCLEANFILES =
noinst_HEADERS = 
noinst_LIBRARIES =
noinst_SCRIPTS = autoregnulib.sh
SUFFIXES =

EXTRA_OBJECTS = lstat.o obstack.o

ACLOCAL_AMFLAGS = -I../m4 --install
AUTOMAKE_OPTIONS = 1.11.6 gnits subdir-objects

DEFS = @DEFS@ -D_IN_GNULIB
AM_CFLAGS = $(PROFILING_CFLAGS) $(GNULIB_WARN_CFLAGS) @MAINT@ $(WERROR_CFLAGS)
AM_CPPFLAGS = ${acl_cv_wp}-I. ${acl_cv_wp}-I$(top_srcdir)/lib \
  ${acl_cv_wp}-I../src ${acl_cv_wp}-I$(top_srcdir)/src
AM_DEFAULT_INCLUDES = $(AM_CPPFLAGS)

all-local: stamp-headers $(EXTRA_OBJECTS)

if BUILDING_FOR_WINDOWSNT
include ../nt/gnulib.mk
distclean-local:
	@echo "making distclean-local in `pwd`"
	-rm -rf .deps || rmdir .deps
	-rm -rf $(srcdr)/*/.deps || rmdir $(srcdir)/*/.deps
	-rm -rf $(builddir)/*/.deps || rmdir $(builddir)/*/.deps

maintainer-clean-local:
	rm -rf glthread || rmdir glthread
else
include gnulib.mk
## remember to update the former to match when the latter is updated.

libgnu_a_SOURCES += openat-die.c save-cwd.c
TESTS_ENVIRONMENT = @LOCALCHARSET_TESTS_ENVIRONMENT@
endif

## a rule to make sure that all generated headers are built:
stamp-headers: $(BUILT_SOURCES)
	@echo "ensuring all generated headers (in `pwd`) have been generated..."
	$(AM_V_at)ls *.h > $@

bootstrap-clean: maintainer-clean
	@echo "making bootstrap-clean in `pwd`"
if BUILDING_FOR_WINDOWSNT
	-rm -f *.obj *.exe
else
	-rm -rf .deps || rmdir .deps
	-rm -rf $(srcdr)/*/.deps || rmdir $(srcdir)/*/.deps
	-rm -rf $(builddir)/*/.deps || rmdir $(builddir)/*/.deps
endif
.PHONY: bootstrap-clean

extraclean: maintainer-clean
	-rm -f *~ \#* stamp-headers
if BUILDING_FOR_WINDOWSNT
	-rm -f *.obj *.exe
else
	-rm -rf .deps || rmdir .deps
	-rm -rf $(srcdr)/*/.deps || rmdir $(srcdir)/*/.deps
	-rm -rf $(builddir)/*/.deps || rmdir $(builddir)/*/.deps
	-rm -rf glthread || rmdir glthread
endif
.PHONY: extraclean

if GNU_MAKE
@ifGNUmake@
UserType = Developer
ToolType = Commands
else
NON_GNU_MAKE = true
endif
