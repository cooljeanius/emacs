#!/bin/sh
### autogen.sh - tool to help build Emacs from a repository checkout

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

## The Emacs bzr (or git) repository does not include the configure script
## (and associated helpers).  The first time you fetch Emacs from the repo,
## run this script to generate the necessary files.
## For more details, see the file INSTALL.REPO.

### Code:

# before we can set up portability code for 'echo', put the rest of the stuff
# that autoconf normally puts before it:
# Be more Bourne compatible:
DUALCASE=1; export DUALCASE # for MKS sh
if test -n "${ZSH_VERSION+set}" && (emulate sh) >/dev/null 2>&1; then :
  emulate sh
  NULLCMD=:
  # Pre-4.2 versions of Zsh do word splitting on ${1+"$@"}, which
  # is contrary to our usage.  Disable this feature.
  alias -g '${1+"$@"}'='"$@"'
  setopt NO_GLOB_SUBST
else
  case `(set -o) 2>/dev/null` in #(
  *posix*) :
    set -o posix ;; #(
  *) :
     ;;
esac
fi

# Now we can steal some "echo" portability code from what autoconf generates:
as_nl='
'
export as_nl
# Printing a long string crashes Solaris 7 /usr/bin/printf.
as_echo='\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\'
as_echo=${as_echo}${as_echo}${as_echo}${as_echo}${as_echo}
as_echo=${as_echo}${as_echo}${as_echo}${as_echo}${as_echo}${as_echo}
# Prefer a ksh shell builtin over an external printf program on Solaris,
# but without wasting forks for bash or zsh.
if test -z "${BASH_VERSION}${ZSH_VERSION}" \
    && (test "X`print -r -- ${as_echo}`" = "X${as_echo}") 2>/dev/null; then
  as_echo='print -r --'
  as_echo_n='print -rn --'
elif (test "X`printf %s ${as_echo}`" = "X${as_echo}") 2>/dev/null; then
  as_echo='printf %s\n'
  as_echo_n='printf %s'
else
  if test "X`(/usr/ucb/echo -n -n ${as_echo}) 2>/dev/null`" = "X-n ${as_echo}"; then
    as_echo_body='eval /usr/ucb/echo -n "$1${as_nl}"'
    as_echo_n='/usr/ucb/echo -n'
  else
    as_echo_body='eval expr "X$1" : "X\\(.*\\)"'
    as_echo_n_body='eval
      arg=$1;
      case ${arg} in #(
      *"${as_nl}"*)
	expr "X${arg}" : "X\\(.*\\)${as_nl}";
	arg=`expr "X${arg}" : ".*${as_nl}\\(.*\\)"`;;
      esac;
      expr "X${arg}" : "X\\(.*\\)" | tr -d "${as_nl}"
    '
    export as_echo_n_body
    as_echo_n='sh -c ${as_echo_n_body} as_echo'
  fi
  export as_echo_body
  as_echo='sh -c ${as_echo_body} as_echo'
fi

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

## prevent perl version from polluting automake version:
if test "x${VERSIONER_DEBUG}" != "x"; then
  test -n "${VERSIONER_DEBUG}" && unset VERSIONER_DEBUG
  test -z "${WE_HAD_SET_VERSIONER_DEBUG_PREVIOUSLY}" && export WE_HAD_SET_VERSIONER_DEBUG_PREVIOUSLY=1
fi


## $1 = program, e.g. "autoconf".
## Echo the version string, e.g. "2.59".
## FIXME: does not handle things like "1.4a", but AFAIK those are
## all old versions, so it is OK to fail there.
## Also note that we do not handle micro versions.
get_version ()
{
    ## Remove e.g. "./autogen.sh: line 50: autoconf: command not found".
    $1 --version 2>&1 | sed -e '/not found/d' -e 's/.* //' -n -e '1 s/\([0-9][0-9\.]*\).*/\1/p'
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
    uprog0=`echo $1 | sed -e 's/-/_/g' -e 'y/abcdefghijklmnopqrstuvwxyz/ABCDEFGHIJKLMNOPQRSTUVWXYZ/'`

    eval uprog=\$${uprog0}

    if [ x"${uprog}" = x"" ]; then
        uprog=$1
    else
        printf '%s' "(using $uprog0=$uprog) "
    fi

    ## /bin/sh should always define the "command" builtin, but for
    ## some odd reason sometimes it does not on hydra.nixos.org.
    ## /bin/sh = "BusyBox v1.27.2", "built-in shell (ash)". ?
    if command -v command > /dev/null 2>&1; then
        command -v $uprog > /dev/null || return 1
    else
        $uprog --version > /dev/null 2>&1 || return 1
    fi
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

    sprog=`echo "$prog" | sed 's/-/_/g'`

    eval min=\$${sprog}_min

    # Eat newline by using autoconf-style compatibility variable:
    ${as_echo_n} "Checking for ${prog} (need at least version ${min})... "

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
        eval ${sprog}_why=\""${stat}"\"
        # stray, unescaped quote to fix the syntax highlighting: "
    fi

done


if [ x"${missing}" != x"" ]; then

    cat <<EOF

Building Emacs from the repository requires the following specialized programs:
EOF

    for prog in ${progs}; do
        sprog=`echo "${prog}" | sed 's/-/_/g'`

        eval min=\$${sprog}_min

        echo "${prog} (minimum version ${min})"
    done


    cat <<EOF

Your system seems to be missing the following tool(s):
EOF

    for prog in ${missing}; do
        sprog=`echo "$prog" | sed 's/-/_/g'`

        eval why=\$${sprog}_why

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

autoreconf -fvi -Wall -Wno-cross -I m4

instead of this script.

Please report any problems with this script to bug-gnu-emacs@gnu.org .
EOF

    exit 1
fi

if test "x${WE_HAD_SET_VERSIONER_DEBUG_PREVIOUSLY}" = "x1"; then
  test -n "${WE_HAD_SET_VERSIONER_DEBUG_PREVIOUSLY}"
  test -z "${VERSIONER_DEBUG}" && export VERSIONER_DEBUG=1
fi

if test "x${AUTORECONF}" = "x"; then
  test -z "${AUTORECONF}" && export AUTORECONF=autoreconf
else
  test -n "${AUTORECONF}" && echo "Using ${AUTORECONF} to autoreconf."
fi

echo 'Your system has the required tools.'
echo "Running '${AUTORECONF} -fvi -Wall -Wno-cross -I m4' ..."

# No longer need to override autopoint; in fact, doing so would be harmful
# because gnulib would have us use an older version of gettext if we kept
# autopoint from overriding it...

## Let autoreconf figure out what, if anything, needs doing.
## Use autoreconf's -f option in case autoreconf itself has changed.
${AUTORECONF} -fvi -Wall -Wno-cross -I m4 || exit $?
## ('-Wno-cross' is because gnulib-tool overwrites our patched macros with ones
## that produce warnings...)

## Create a timestamp, so that './autogen.sh; make' does NOT
## cause 'make' to needlessly run 'autoheader'.
echo "timestamp: `date`" > src/stamp-h.in || echo timestamp > src/stamp-h.in || exit

## Configure Git, if using Git.
if test -d .git && (git status -s) >/dev/null 2>&1; then

	# Check hashes when transferring objects among repositories.

    git config transfer.fsckObjects true || exit


    # Configure 'git diff' hunk header format.

    git config 'diff.elisp.xfuncname' \
		'^\(def[^[:space:]]+[[:space:]]+([^()[:space:]]+)' || exit
    git config 'diff.m4.xfuncname' \
		'^((m4_)?define|A._DEFUN(_ONCE)?)\([^),]*' || exit
    git config 'diff.make.xfuncname' \
	   '^([$.[:alnum:]_].*:|[[:alnum:]_]+[[:space:]]*([*:+]?[:?]?|!?)=|define .*)' || exit
    git config 'diff.shell.xfuncname' \
	   '^([[:space:]]*[[:alpha:]_][[:alnum:]_]*[[:space:]]*\(\)|[[:alpha:]_][[:alnum:]_]*=)' || exit
    git config 'diff.texinfo.xfuncname' \
		'^@node[[:space:]]+([^,[:space:]][^,]+)' || exit


    # Install Git hooks.

    tailored_hooks=""
    sample_hooks=""

    for hook in commit-msg pre-commit; do
		cmp build-aux/git-hooks/${hook} .git/hooks/${hook} >/dev/null 2>&1 ||
		tailored_hooks="${tailored_hooks} ${hook}"
    done
    for hook in applypatch-msg pre-applypatch; do
		test ! -r .git/hooks/${hook}.sample ||
		cmp .git/hooks/${hook}.sample .git/hooks/${hook} >/dev/null 2>&1 ||
		sample_hooks="${sample_hooks} ${hook}"
    done

    if test -n "${tailored_hooks}${sample_hooks}"; then
		echo "Installing git hooks..."

		case `cp --help 2>/dev/null` in
		  *--backup*--verbose*)
			cp_options='--backup=numbered --verbose';;
		  *)
			cp_options='-f';;
		esac

		if test -n "${tailored_hooks}"; then
			for hook in ${tailored_hooks}; do
				cp ${cp_options} build-aux/git-hooks/${hook} .git/hooks || exit
				chmod a-w .git/hooks/${hook} || exit
			done
		fi

		if test -n "${sample_hooks}"; then
			for hook in ${sample_hooks}; do
				cp ${cp_options} .git/hooks/${hook}.sample .git/hooks/${hook} || exit
				chmod a-w .git/hooks/${hook} || exit
			done
		fi
    fi
fi

echo ""
echo "You can now run './configure' with your favorite arguments."

exit 0

### autogen.sh ends here
