#!/bin/sh

# The gnulib commit ID to use for the update.
# If you know your version works and is newer, feel free to replace:
GNULIB_COMMIT_SHA1="3b1e0bcd35f006ea401d3dbd2169fd75f31bee3b"

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
      absolute-header acl acl-permissions alignof alloca alloca-opt \
      allocator assert-h assure atan atan2 autobuild \
      bcopy binary-io builtin-expect byteswap \
      c-ctype c-strcase c-strcaseeq c99 canonicalize-lgpl careadlinkat \
      chdir clock-time close-stream closedir configmake count-one-bits \
      count-trailing-zeros \
      crypto/md5 crypto/sha1 crypto/sha256 crypto/sha512 \
      diffseq dirent dirfd dirname-lgpl dosname double-slash-root \
      dtoastr dtotimespec dup2 \
      environ errno error execinfo euidaccess explicit_bzero extensions \
      extern-inline \
      faccessat fcntl fcntl-h fdatasync fdopendir file-has-acl filemode \
      filevercmp flexmember float fopen fpending fpieee fpucw fseek fseeko \
      fstat fstatat fsusage fsync ftell ftello ftoastr ftruncate func \
      gendocs getdelim getdtablesize getgroups gethostname getline \
      getloadavg getlogin getopt-gnu getopt-posix getpagesize getpass \
      getpass-gnu getprogname gettext gettext-h gettime gettimeofday \
      git-version-gen gitlog-to-changelog gnu-make gpl-3.0 group-member \
      havelib host-cpu-c-abi host-os \
      ignore-value include_next inline intprops inttypes-incomplete \
      largefile ldd limits-h localcharset localtime-buffer longlong \
      lseek lstat \
      maintainer-makefile manywarnings math mbsinit memchr memrchr \
      mkostemp mktime multiarch \
      nextafter no-c++ nocrash nstrftime \
      obstack open openat openat-die openat-h openmp \
      pagealign_alloc pathmax perror pipe2 posix_spawnp printf-safe \
      progname pselect pthread_sigmask putenv \
      qacl qcopy-acl quote quotearg quotearg-simple \
      read readdir readlink readlinkat realloc-gnu realloc-posix rename \
      rmdir root-uid \
      sched secure_getenv setenv sig2str signal-h sigpipe sleep \
      snippet/_Noreturn snippet/arg-nonnull snippet/c++defs \
      snippet/link-warning snippet/unused-parameter snippet/warn-on-use \
      socketlib sockets socklen spawn ssize_t stat stat-time std-gnu11 \
      stdalign stdarg stdbool stddef stdint stdio stdlib stdnoreturn \
      stpcpy streq strerror strerror-override strftime string strings \
      strstr strstr-simple strtoimax strtoumax symlink sys_ioctl \
      sys_resource sys_select sys_socket sys_stat sys_time sys_types \
      sys_uio sys_utsname sys_wait \
      tempname time time_r time_rz timegm timer-time times timespec \
      timespec-add timespec-sub \
      u64 uname unistd unlink unlocked-io unsetenv update-copyright \
      useless-if-before-free utimens \
      vararrays va-args vc-list-files verify vla vma-iter \
      waitpid warnings wchar wctype-h winsz-ioctl winsz-termios write \
      xalloc xalloc-die xalloc-oversized"
   # Ones I am tempted to add, but cannot:
   # - atexit, which is obsolete
   # - getcwd, which depends on strdup-posix, which depends on malloc-posix
   #   (which is avoided)
   # - git-merge-changelog, which depends on... a lot of things
   # - memcpy, which is obsolete
   # - memmove, which is obsolete
   # - memset, which is obsolete
   # - obstack-printf, which drags in all the vasnprintf stuff
   # - fprintf-posix and vfprintf-posix also depend on vasnprintf, as does
   #   printf-posix, which gets it indirectly via vfprintf-posix
   # - likewise with snprintf and snprintf-posix
   # - sigaction, which depends on sigprocmask, which is avoided
   # (even though I cannot add them, there is no need to explicitly ignore
   # them below, though)
   # Other obsolete modules that I have stopped explicitly adding:
   # - dup2-obsolete
   # - memcmp
   # (likewise, even though I no longer explicitly import them, there is no
   # need to go to the other extreme and explicitly ignore them, either)
   # Reasons for explicitly ignoring some:
   # - gnumakefile: we have a handwritten one.
   echo "Actually beginning import now; this may take a while..."
  "${gnulib_tool}" --import --dir=. --lib=libgnu --source-base=lib \
    --m4-base=m4 --doc-base=doc --tests-base=tests --aux-dir=build-aux \
    --avoid=close --avoid=dup --avoid=fchdir --avoid=fstrcmp \
    --avoid=gnumakefile --avoid=lock --avoid=malloc --avoid=malloc-posix \
    --avoid=memchr-obsolete --avoid=msvc-inval --avoid=msvc-nothrow \
    --avoid=opendir --avoid=raise --avoid=save-cwd --avoid=select \
    --avoid=sigprocmask --avoid=strdup --avoid=strdup-posix \
    --avoid=threadlib --avoid=tls --avoid=vasnprintf --avoid=vasnprintf-posix \
    --makefile-name=gnulib.mk --conditional-dependencies --no-libtool \
    --macro-prefix=gl --no-vc-files --with-obsolete \
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
