## automake Makefile for admin/ subdir for emacs

SUBDIRS = grammars unidata

noinst_SCRIPTS = build-configs check-doc-strings diff-tar-files make-emacs \
  merge-gnulib merge-pkg-config quick-install-emacs update-copyright \
  update_autogen

check_PROGRAMS = alloc-colors$(EXEEXT)
alloc_colors_SOURCES = alloc-colors.c
alloc_colors_LDADD = -lX11

DISTCLEANFILES = .DS_Store *~

ACLOCAL_AMFLAGS = -I../m4 --install
AUTOMAKE_OPTIONS = 1.11 gnits

DEFS = @DEFS@ -D_IN_ADMIN_DIR
AM_CFLAGS = $(PROFILING_CFLAGS) $(GNULIB_WARN_CFLAGS) @MAINT@ $(WERROR_CFLAGS)
AM_CPPFLAGS = -I. -I$(top_srcdir)/lib -I../src -I$(top_srcdir)/src
AM_DEFAULT_INCLUDES = $(AM_CPPFLAGS)

bootstrap-clean: maintainer-clean
	@echo "no bootstrap-clean specific rules yet."
.PHONY: bootstrap-clean

maintainer-clean-local:
	@echo "no maintainer-clean-local specific rules yet."
.PHONY: maintainer-clean-local

extraclean: maintainer-clean
	-rm -f *~ \#*
.PHONY: extraclean

distclean-local:
	-rm -rf *.dSYM || rmdir *.dSYM
.PHONY: distclean-local
