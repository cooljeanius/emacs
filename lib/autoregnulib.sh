#!/bin/sh

# The gnulib commit ID to use for the update.
GNULIB_COMMIT_SHA1="8a695b675d018ecfab9db14fce4b13379f5ac908"

if [ $# -ne 1 ]; then
   echo "Warning: Path to gnulib repository missing."
   echo "Assuming you have cloned it to your home directory."
   echo "Correct usage: ./lib/autoregnulib.sh <path-to-gnulib-repository>"
   gnulib_prefix=~/gnulib
else
   gnulib_prefix=$1
   echo "Using '${gnulib_prefix}' as initial gnulib_prefix."
fi

gnulib_tool="${gnulib_prefix}/gnulib-tool"

# Verify that the gnulib directory does exist...
if [ ! -f "${gnulib_tool}" ]; then
   echo "Error: Invalid gnulib directory. Cannot find gnulib tool"
   echo "       (${gnulib_tool})."
   echo "Checking to see if it is in your path...."
   if test "x$(which gnulib-tool)" = "x" || test ! -x "$(which gnulib-tool)"; then
     echo "Error: gnulib-tool needs to be in your path for this to work." >&2
     exit 1
   else
     gnulib_tool="$(which gnulib-tool)"
     if test ! -d "${gnulib_prefix}"; then
       if test -d "$(dirname "${gnulib_tool}")"; then
         gnulib_prefix="$(dirname "${gnulib_tool}")"
       fi
     fi
   fi
else
   echo "Found '${gnulib_tool}'."
fi

# Verify that we have the right version of gnulib...
gnulib_head_sha1=$(cd "${gnulib_prefix}" && git rev-parse HEAD 2>/dev/null)
if [ "${gnulib_head_sha1}" != "${GNULIB_COMMIT_SHA1}" ]; then
   echo "Error: Wrong version of gnulib: '${gnulib_head_sha1}'"
   echo "       (we expected it to be ${GNULIB_COMMIT_SHA1})"
   echo "Aborting."
   exit 1
fi

# Verify that we are in the top directory of the source tree.
if [ ! -f ./dumpemacs.c -o ! -d lib ]; then
   echo "Error: This script should be called from the top-level directory."
   echo "Aborting."
   exit 1
fi

if test -z "${gnulib_tool}" || test ! -x "${gnulib_tool}"; then
  echo "Error: gnulib-tool is unavailable." >&2
  exit 1
else
  # The list of gnulib modules we are importing for emacs:
  module_list="\
      alloca-opt \
      byteswap \
      c-ctype c-strcase careadlinkat close-stream \
      count-one-bits count-trailing-zeros \
      crypto/md5 crypto/sha1 crypto/sha256 crypto/sha512 \
      dtoastr dtotimespec dup2 \
      environ error execinfo \
      faccessat fcntl fcntl-h fdatasync fdopendir filemode fstatat fsync \
      getloadavg getopt-gnu gettime gettimeofday \
      git-version-gen gitlog-to-changelog gnu-make \
      intprops \
      largefile lstat \
      manywarnings memrchr mkostemp mktime \
      obstack \
      pipe2 posix_spawnp progname pselect pthread_sigmask putenv \
      qacl \
      readlink readlinkat \
      sig2str snippet/warn-on-use socklen stat-time stdalign stdarg stdbool \
      stdio strftime strtoimax strtoumax symlink sys_stat sys_time \
      time timer-time timespec-add timespec-sub \
      unsetenv update-copyright utimens \
      warnings"
   echo "Actually beginning import now; this may take a while..."
  "${gnulib_tool}" --import --dir=. --lib=libgnu --source-base=lib --m4-base=m4 \
    --doc-base=doc --tests-base=tests --aux-dir=build-aux --avoid=close \
    --avoid=dup --avoid=fchdir --avoid=malloc-posix \
    --avoid=msvc-nothrow --avoid=open --avoid=openat-die \
    --avoid=opendir --avoid=raise --avoid=save-cwd --avoid=select \
    --avoid=sigprocmask --avoid=threadlib --makefile-name=gnulib.mk \
    --conditional-dependencies --no-libtool --macro-prefix=gl --no-vc-files \
    "${module_list}"
  if [ $? -ne 0 ]; then
    echo "Error: gnulib import failed.  Aborting."
    exit 1
  fi
fi

#TODO: integrate with the emacs local autogen.sh script.
