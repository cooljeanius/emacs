#!/bin/sh
### autogen.sh - tool to help build Emacs from a bzr checkout

## Copyright (C) 2011-2014 Free Software Foundation, Inc.

## Author: Glenn Morris <rgm@gnu.org>
## Maintainer: emacs-devel@gnu.org

## This file is part of GNU Emacs.

## GNU Emacs is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.

## GNU Emacs is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.

## You should have received a copy of the GNU General Public License
## along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.

### Commentary:

## The Emacs bzr repository does not include the configure script
## (and associated helpers).  The first time you fetch Emacs from bzr,
## run this script to generate the necessary files.
## For more details, see the file INSTALL.REPO.

### Code:

## Tools we need:
## Note that we respect the values of AUTOCONF etc, like autoreconf does.
progs="autoconf automake"

## Minimum versions we need:
autoconf_min="`sed -n 's/^ *AC_PREREQ(\([0-9\.]*\)).*/\1/p' configure.ac`"
if test "x${autoconf_min}" = "x"; then
  test -z "${autoconf_min}"
  echo "warning: failed to extract required autoconf version from configure.ac" >&2
  autoconf_min="2.59"
  echo "falling back to default set in $0 for autoconf (i.e. '${autoconf_min}')"
fi

## This will need improving if more options are ever added to the
## AM_INIT_AUTOMAKE call.
automake_min="`sed -n 's/^ *AM_INIT_AUTOMAKE(\([0-9\.]*\)).*/\1/p' configure.ac`"
if test "x${automake_min}" = "x"; then
  test -z "${automake_min}"
  echo "warning: failed to extract required automake version from configure.ac" >&2
  automake_min="1.11"
  echo "falling back to default set in $0 for automake (i.e. '${automake_min}')"
fi


## $1 = program, e.g. "autoconf".
## Echo the version string, e.g. "2.59".
## FIXME: does not handle things like "1.4a", but AFAIK those are
## all old versions, so it is OK to fail there.
## Also note that we do not handle micro versions.
get_version ()
{
    ## Remove e.g. "./autogen.sh: line 50: autoconf: command not found".
    $1 --version 2>&1 | sed -e '/not found/d' -n -e '1 s/.* \([1-9][0-9\.]*\).*/\1/p'
}

## $1 = version string, e.g. "2.59"
## Echo the major version, e.g. "2".
major_version ()
{
    echo $1 | sed -e 's/\([0-9][0-9]*\)\..*/\1/'
}

## $1 = version string, e.g. "2.59"
## Echo the minor version, e.g. "59".
minor_version ()
{
    echo $1 | sed -e 's/[0-9][0-9]*\.\([0-9][0-9]*\).*/\1/'
}

## $1 = program
## $2 = minimum version.
## Return 0 if program is present with version >= minimum version.
## Return 1 if program is missing.
## Return 2 if program is present but too old.
## Return 3 for unexpected error (e.g. failed to parse version).
check_version ()
{
    ## Respect e.g. ${AUTOMAKE} if it is set, like autoreconf does.
    uprog=`echo $1 | sed 'y/abcdefghijklmnopqrstuvwxyz/ABCDEFGHIJKLMNOPQRSTUVWXYZ/'`

    eval uprog=\$${uprog}

    [ x"${uprog}" = x"" ] && uprog=$1

    have_version=`get_version $uprog`

    [ x"${have_version}" = x"" ] && return 1

    have_maj=`major_version ${have_version}`
    need_maj=`major_version $2`

    [ x"${have_maj}" != x"" ] && [ x"${need_maj}" != x"" ] || return 3

    [ ${have_maj} -gt ${need_maj} ] && return 0
    [ ${have_maj} -lt ${need_maj} ] && return 2

    have_min=`minor_version ${have_version}`
    need_min=`minor_version $2`

    [ x"${have_min}" != x"" ] && [ x"${need_min}" != x"" ] || return 3

    [ ${have_min} -ge ${need_min} ] && return 0
    return 2
}


cat <<EOF

Checking whether you have the necessary tools...
(Read INSTALL.REPO for more details on building Emacs)

EOF

missing=""

for prog in ${progs}; do

    eval min=\$${prog}_min

    echo "Checking for ${prog} (need at least version ${min})..."

    check_version ${prog} ${min}

    retval=$?

    case ${retval} in
        0) stat="ok" ;;
        1) stat="missing" ;;
        2) stat="too old" ;;
        *) stat="unable to check" ;;
    esac

    echo "${stat}"

    if [ ${retval} -ne 0 ]; then
        missing="${missing} ${prog}"
        eval ${prog}_why=\""${stat}"\"
    fi

done


if [ x"${missing}" != x"" ]; then

    cat <<EOF

Building Emacs from Bzr requires the following specialized programs:
EOF

    for prog in ${progs}; do
        eval min=\$${prog}_min

        echo "${prog} (minimum version ${min})"
    done


    cat <<EOF

Your system seems to be missing the following tool(s):
EOF

    for prog in ${missing}; do
        eval why=\$${prog}_why

        echo "${prog} (${why})"
    done

    cat <<EOF

If you think you have the required tools, please add them to your PATH
and re-run this script.

Otherwise, please try installing them.
On systems using rpm and yum, try: "yum install PACKAGE"
On systems using dpkg and apt, try: "apt-get install PACKAGE"
On systems using other package managers, try a similar command for that
other package manager.
Then re-run this script.

If you do not have permission to do this, or if the version provided
by your system is too old, it is normally straightforward to build
these packages from source.  You can find the sources at:

ftp://ftp.gnu.org/gnu/PACKAGE/

Download the package (make sure you get at least the minimum version
listed above), extract it using tar, then run configure, make,
make install.  Add the installation directory to your PATH and re-run
this script.

If you know that the required versions are in your PATH, but this
script has made an error, then you can simply run

AUTOPOINT=true autoreconf -fvi -Wall -I m4

instead of this script.

Please report any problems with this script to bug-gnu-emacs@gnu.org .
EOF

    exit 1
fi

echo "Your system has the required tools, running autoreconf..."

# keep autopoint from overwriting our modified copy of m4/extern-inline.m4,
# at least until we check and see if a newer version of gettext is okay:
export AUTOPOINT=true

## Let autoreconf figure out what, if anything, needs doing.
autoreconf -fvi -Wall -I m4 || exit $?

## Create a timestamp, so that './autogen.sh; make' does NOT
## cause 'make' to needlessly run 'autoheader'.
echo "timestamp: `date`" > src/stamp-h.in || echo timestamp > src/stamp-h.in || exit

echo ""
echo "You can now run \`./configure' with your favorite arguments."

exit 0

### autogen.sh ends here
