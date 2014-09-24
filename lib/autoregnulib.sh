#!/bin/sh

if test "x`which gnulib-tool`" = "x" || test ! -x "`which gnulib-tool`"; then
  echo "Error: gnulib-tool needs to be in your path for this to work." >&2
  exit 1
else
  # The list of gnulib modules we are importing for emacs:
  module_list="alloca-opt byteswap \
      c-ctype c-strcase careadlinkat close-stream \
      count-one-bits count-trailing-zeros \
      crypto/md5 crypto/sha1 crypto/sha256 crypto/sha512 \
      dtoastr dtotimespec dup2 \
      environ error execinfo \
      faccessat fcntl fcntl-h fdatasync fdopendir filemode fstatat fsync \
      getloadavg getopt-gnu gettime gettimeofday \
      intprops \
      largefile lstat \
      manywarnings memrchr mkostemp mktime \
      obstack \
      pipe2 pselect pthread_sigmask putenv \
      qacl \
      readlink readlinkat \
      sig2str socklen stat-time stdalign stdarg stdbool stdio \
      strftime strtoimax strtoumax symlink sys_stat sys_time \
      time timer-time timespec-add timespec-sub \
      unsetenv update-copyright utimens warnings"
  gnulib-tool --import --dir=. --lib=libgnu --source-base=lib --m4-base=m4 \
    --doc-base=doc --tests-base=tests --aux-dir=build-aux --avoid=close \
    --avoid=dup --avoid=fchdir --avoid=malloc-posix \
    --avoid=msvc-inval --avoid=msvc-nothrow --avoid=open --avoid=openat-die \
    --avoid=opendir --avoid=raise --avoid=save-cwd --avoid=select \
    --avoid=sigprocmask --avoid=threadlib --makefile-name=gnulib.mk \
    --conditional-dependencies --no-libtool --macro-prefix=gl --no-vc-files \
    "${module_list}"
  if [ $? -ne 0 ]; then
    echo "Error: gnulib import failed.  Aborting."
    exit 1
  fi
fi
