# Build Emacs from a fresh tarball or version-control checkout.

# Copyright (C) 2011-2014 Free Software Foundation, Inc.
#
# This file is part of GNU Emacs.
#
# GNU Emacs is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# GNU Emacs is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
#
# written by Paul Eggert


# This GNUmakefile is for GNU Make.  It is for convenience, so that
# one can run 'make' in an unconfigured source tree.  In such a tree,
# this file causes GNU Make to first create a standard configuration
# with the default options, and then reinvokes itself on the
# newly-built Makefile.  If the source tree is already configured,
# this file defers to the existing Makefile.

# If you are using a non-GNU 'make', or if you want non-default build
# options, or if you want to build in an out-of-source tree, please
# run "configure" by hand.  But run autogen.sh first, if the source
# was checked out directly from the repository.

_gl-Makefile := $(wildcard [M]akefile)

# If a Makefile already exists, just use it.

ifeq ($(wildcard Makefile),Makefile)

# Make tar archive easier to reproduce.
export TAR_OPTIONS = --owner=0 --group=0 --numeric-owner

# Allow the user to add to this in the Makefile.
ALL_RECURSIVE_TARGETS =

include Makefile

# Some projects override e.g., _autoreconf here.
-include $(srcdir)/cfg.mk

# Allow cfg.mk to override these.
_build-aux ?= build-aux
_autoreconf ?= autoreconf -v

include $(srcdir)/maint.mk


# Ensure that $(VERSION) is up to date for dist-related targets, but not
# for others: rerunning autoreconf and recompiling everything isn't cheap.
_have-git-version-gen := \
  $(shell test -f $(srcdir)/$(_build-aux)/git-version-gen && echo yes)
ifeq ($(_have-git-version-gen)0,yes$(MAKELEVEL))
  _is-dist-target ?= $(filter-out %clean, \
    $(filter maintainer-% dist% alpha beta stable,$(MAKECMDGOALS)))
  _is-install-target ?= $(filter-out %check, $(filter install%,$(MAKECMDGOALS)))
  ifneq (,$(_is-dist-target)$(_is-install-target))
    _curr-ver := $(shell cd $(srcdir)				\
                   && $(_build-aux)/git-version-gen		\
                         .tarball-version			\
                         $(git-version-gen-tag-sed-script))
    ifneq ($(_curr-ver),$(VERSION))
      ifeq ($(_curr-ver),UNKNOWN)
        $(info WARNING: unable to verify if $(VERSION) is the correct version)
      else
        ifneq (,$(_is-install-target))
          # GNU Coding Standards state that 'make install' should not cause
          # recompilation after 'make all'.  But as long as changing the version
          # string alters config.h, the cost of having 'make all' always have an
          # up-to-date version is prohibitive.  So, as a compromise, we merely
          # warn when installing a version string that is out of date; the user
          # should run 'autoreconf' (or something like 'make distcheck') to
          # fix the version, 'make all' to propagate it, then 'make install'.
          $(info WARNING: version string $(VERSION) is out of date;)
          $(info run '$(MAKE) _version' to fix it)
        else
          $(info INFO: running autoreconf for new version string: $(_curr-ver))
GNUmakefile: _version
	touch GNUmakefile
        endif
      endif
    endif
  endif
endif

.PHONY: _version
_version:
	cd $(srcdir) && rm -rf autom4te.cache .version && $(_autoreconf)
	$(MAKE) $(AM_MAKEFLAGS) Makefile

else

.DEFAULT_GOAL := default
srcdir = .

# The package can override .DEFAULT_GOAL to run actions like autoreconf.
-include ./cfg.mk

# Allow cfg.mk to override these.
_build-aux ?= build-aux
_autoreconf ?= autoreconf -v

include ./maint.mk

# If cleaning and Makefile does not exist, do NOT bother creating it.
# The source tree is already clean, or is in a weird state that
# requires expert attention.

ifeq ($(filter-out %clean,$(or $(MAKECMDGOALS),default)),)

$(MAKECMDGOALS):
	@echo >&2 'No Makefile; skipping $@.'

default:
	@echo '$@'

else

# No Makefile, and not cleaning.
# If 'configure' does not exist, Emacs must have been checked
# out directly from the repository; run ./autogen.sh.
# Once 'configure' exists, run it.
# Finally, run the actual 'make'.

ORDINARY_GOALS = $(filter-out configure Makefile bootstrap,$(MAKECMDGOALS))

default $(ORDINARY_GOALS): Makefile
	$(MAKE) -f Makefile $(MAKECMDGOALS)
# Execute in sequence, so that multiple user goals do NOT conflict.
.NOTPARALLEL:

configure:
	@echo >&2 'There seems to be no "configure" file in this directory.'
	@echo >&2 'Running ./autogen.sh ...'
	./autogen.sh
	@echo >&2 '"configure" file built.'

Makefile: configure
	@echo >&2 'There seems to be no Makefile in this directory.'
	@echo >&2 'Running ./configure ...'
	./configure
	@echo >&2 'Makefile built.'

# 'make bootstrap' in a fresh checkout need not run 'configure' twice:
bootstrap: Makefile
	$(MAKE) -f Makefile all

.PHONY: bootstrap default $(ORDINARY_GOALS)

endif
endif

# Tell version 3.79 and up of GNU make to not build goals in this
# directory in parallel, in case someone tries to build multiple
# targets, and one of them can cause a recursive target to be invoked.

# Only set this if Automake doesn't provide it.
AM_RECURSIVE_TARGETS ?= $(RECURSIVE_TARGETS:-recursive=) \
  $(RECURSIVE_CLEAN_TARGETS:-recursive=) \
  dist distcheck tags ctags

ALL_RECURSIVE_TARGETS += $(AM_RECURSIVE_TARGETS)

ifneq ($(word 2, $(MAKECMDGOALS)), )
ifneq ($(filter $(ALL_RECURSIVE_TARGETS), $(MAKECMDGOALS)), )
.NOTPARALLEL:
endif
endif
