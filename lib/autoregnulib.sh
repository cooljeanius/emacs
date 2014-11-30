#!/bin/sh

# The gnulib commit ID to use for the update.
# If you know your version is newer, feel free to replace:
GNULIB_COMMIT_SHA1="e9dd4906da30642172e6bb1ff2703e8e2c912fcb"

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
      absolute-header alignof alloca alloca-opt allocator assert-h autobuild \
      binary-io byteswap \
      c-ctype c-strcase careadlinkat chdir clock-time close-stream closedir \
      configmake count-one-bits count-trailing-zeros \
      crypto/md5 crypto/sha1 crypto/sha256 crypto/sha512 \
      dirent dosname double-slash-root dtoastr dtotimespec dup2 \
      environ errno error execinfo euidaccess extensions extern-inline \
      faccessat fcntl fcntl-h fdatasync fdopendir filemode float fopen \
      fpending fpieee fpucw fstat fstatat fsync func \
      gendocs getdtablesize getgroups getloadavg getlogin getopt-gnu \
      getpagesize getpass getpass-gnu gettext gettext-h gettime gettimeofday \
      git-version-gen gitlog-to-changelog gnu-make group-member \
      havelib host-cpu-c-abi host-os \
      include_next inline intprops inttypes-incomplete \
      largefile ldd longlong lseek lstat \
      manywarnings math memchr memrchr mkostemp mktime multiarch \
      nextafter no-c++ nocrash \
      obstack openat openat-h openmp \
      pathmax pipe2 posix_spawnp printf-safe progname pselect \
      pthread_sigmask putenv \
      qacl quote \
      readdir readlink readlinkat rename root-uid \
      sched sig2str signal-h sigpipe sleep snippet/_Noreturn \
      snippet/link-warning snippet/unused-parameter snippet/warn-on-use \
      socklen spawn ssize_t stat stat-time stdalign stdarg stdbool stddef \
      stdint stdio stdlib stdnoreturn streq strerror strftime string strstr \
      strtoimax strtoumax symlink sys_ioctl sys_resource sys_select sys_stat \
      sys_time sys_types \
      tempname time time_r timer-time timespec timespec-add timespec-sub \
      u64 unistd unsetenv update-copyright utimens \
      vararrays va-args vc-list-files verify vla vma-iter \
      warnings wchar wctype-h winsz-ioctl winsz-termios \
      xalloc xalloc-die xalloc-oversized"
   # Ones I am tempted to add, but cannot:
   # - getcwd, which depends on strdup-posix, which depends on malloc-posix,
   #   which is avoided
   # - git-merge-changelog, which depends on... a lot of things
   # - memmove, which is obsolete
   # - obstack-printf, which drags in all the vasnprintf stuff
   # - fprintf-posix and vfprintf-posix also depend on vasnprintf, as does
   #   printf-posix, which gets it indirectly via vfprintf-posix
   # (even though I cannot add them, there is no need to explicitly ignore them
   # below, though)
   # Other obsolete modules that I have stopped explicitly adding:
   # - dup2-obsolete
   # - memcmp
   # (likewise, even though I no longer explicitly import them, there is no need
   # to go to the other extreme and explicitly ignore them, either)
   echo "Actually beginning import now; this may take a while..."
  "${gnulib_tool}" --import --dir=. --lib=libgnu --source-base=lib \
    --m4-base=m4 --doc-base=doc --tests-base=tests --aux-dir=build-aux \
    --avoid=close --avoid=dup --avoid=fchdir --avoid=fstrcmp \
    --avoid=localcharset --avoid=lock --avoid=malloc --avoid=malloc-posix \
    --avoid=memchr-obsolete --avoid=msvc-nothrow --avoid=open \
    --avoid=openat-die --avoid=opendir --avoid=raise --avoid=save-cwd \
    --avoid=select --avoid=sigprocmask --avoid=strdup --avoid=strdup-posix \
    --avoid=threadlib --avoid=tls --avoid=vasnprintf --avoid=vasnprintf-posix \
    --makefile-name=gnulib.mk --conditional-dependencies \
    --no-libtool --macro-prefix=gl --no-vc-files --with-obsolete \
    "${module_list}"
  if [ $? -ne 0 ]; then
    echo "Error: gnulib import failed.  Aborting."
    exit 1
  fi
fi

echo ""
echo "$0 should now be done re-importing gnulib."
echo "You should now re-generate the build system with ./autogen.sh, or by running autoreconf with your favorite flags."
echo "Or, if you are unhappy with the results of this script ($0), you can try re-importing gnulib with ./admin/merge-gnulib instead, although that script is probably out of sync with this one."
#TODO: integrate with the emacs local autogen.sh script (as in, actually run it)
