#!/bin/sh

# The gnulib commit ID to use for the update.
# If you know your version works and is newer, feel free to replace:
GNULIB_COMMIT_SHA1="1afbb510b0c8a0812d4e4cfc889044503138648f"
# (this hash last updated October 5, 2021)

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
      absolute-header access acl acl-permissions aligned-malloc alignof alloca \
      alloca-opt allocator assert-h assure atan atan2 autobuild \
      basename-lgpl bcopy binary-io bison bitset builtin-expect byteswap \
      c-ctype c-dtoastr c-strcase c-strcaseeq c-strtod c99 canonicalize-lgpl \
      careadlinkat chdir clock-time close-stream closedir configmake \
      copy-file-range count-one-bits count-trailing-zeros \
      creat crypto/md5 crypto/sha1 crypto/sha256 crypto/sha512 \
      diffseq dirent dirfd dirname-lgpl double-slash-root dtoastr dtotimespec dup2 \
      environ errno error execinfo euidaccess explicit_bzero extensions \
      extern-inline \
      faccessat fchdir fchmodat fchownat fclose fcntl fcntl-h fdatasync \
      fdopendir fflush file-has-acl filemode filename filevercmp flexmember \
      float fopen fopen-gnu fpending fpieee fpucw fseek fseeko fstat fstatat \
      fsusage fsync ftell ftello ftoastr ftruncate func \
      gendocs getdelim getdtablesize getgroups gethostname getline \
      getloadavg getlogin getopt-gnu getopt-posix getpagesize getpass \
      getpass-gnu getprogname gettext-h gettime gettimeofday \
      git-version-gen gitlog-to-changelog gnu-make gpl-3.0 group-member \
      havelib host-cpu-c-abi host-os \
      ignore-value include_next inline intprops inttypes-incomplete ioctl \
      isnanl \
      largefile ldd limits-h localcharset longlong lseek lstat \
      maintainer-makefile manywarnings math mbschr mbsinit mbsrchr memchr \
      memrchr mktime multiarch \
      nextafter no-c++ nocrash nstrftime \
      obstack open openat openat-die openat-h openmp \
      pagealign_alloc pathmax perror pipe2 posix_openpt posix_spawnp \
      printf-safe progname pselect pthread-h pthread_sigmask pty putenv \
      qacl qcopy-acl quote quotearg quotearg-simple \
      read readdir readlink readlinkat realloc-gnu realloc-posix rename \
      rmdir root-uid \
      sched secure_getenv setenv setlocale-null sh-filename sig2str signal-h \
      sigpipe sleep snippet/_Noreturn snippet/arg-nonnull snippet/c++defs \
      snippet/link-warning snippet/unused-parameter snippet/warn-on-use \
      socketlib sockets socklen spawn ssize_t stat stat-time std-gnu11 \
      stdalign stdarg stdbool stddef stdint stdio stdlib stdnoreturn \
      stpcpy streq strerror strerror-override string strings \
      strstr strstr-simple strtoimax strtold strtoumax symlink sys_ioctl \
      sys_resource sys_select sys_socket sys_stat sys_time sys_types \
      sys_uio sys_utsname sys_wait \
      tempname threads time time_r time_rz timegm timer-time times timespec \
      timespec-add timespec-sub \
      u64 uchar uname unistd unlink unlocked-io unsetenv update-copyright \
      useless-if-before-free utimens \
      vararrays va-args vc-list-files vcs-to-changelog verify vla vma-iter \
      waitpid warnings wchar wctype-h winsz-ioctl winsz-termios write \
      xalloc xalloc-die xalloc-oversized"
   # Ones I am tempted to add, but cannot:
   # - atexit, which is obsolete
   # - attribute, which has definitions that conflict with ones in "src/conf_post.h"
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
   # - posixcheck, which breaks stuff
   # - mkostemp, which drags <unistd.h> into <stdlib.h> which breaks
   #   unexmacosx.c
   # (even though I cannot add them, there is no need to explicitly ignore
   # them below, though)
   # Other obsolete modules that I have stopped explicitly adding:
   # - dup2-obsolete
   # - memcmp
   # - localtime-buffer (no longer exists)
   # - dosname (replaced by filename)
   # - gettext (replaced by gettext-h)
   # - strftime (replaced by nstrftime)
   # (likewise, even though I no longer explicitly import them, there is no
   # need to go to the other extreme and explicitly ignore them, either)
   # Reasons for explicitly ignoring some:
   # - gnumakefile: we have a handwritten one.
   echo "Actually beginning import now; this may take a while..."
  "${gnulib_tool}" --import --dir=. --lib=libgnu --source-base=lib \
    --m4-base=m4 --doc-base=doc --tests-base=tests --aux-dir=build-aux \
    --avoid=arpa_inet --avoid=c-ldtoastr --avoid=chmodat --avoid=chownat --avoid=close \
    --avoid=dup --avoid=fstrcmp --avoid=gnumakefile --avoid=lock \
    --avoid=malloc --avoid=malloc-posix \
    --avoid=memchr-obsolete --avoid=msvc-inval --avoid=msvc-nothrow \
    --avoid=opendir --avoid=raise --avoid=save-cwd --avoid=select \
    --avoid=sigprocmask --avoid=strdup --avoid=strdup-posix \
    --avoid=threadlib --avoid=tls --avoid=vasnprintf --avoid=vasnprintf-posix \
    --avoid=windows-cond --avoid=windows-mutex --avoid=windows-once \
    --avoid=windows-recmutex --avoid=windows-thread --avoid=windows-timedmutex \
    --avoid=windows-timedrecmutex --avoid=windows-tls \
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
