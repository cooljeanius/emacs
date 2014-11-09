#restartable_syscalls.m4 serial 1                          -*- Autoconf -*-
dnl# taken from specific.m4 in autoconf2.69, but with the obsolescence
dnl# warning removed.

dnl# AC_SYS_RESTARTABLE_SYSCALLS
dnl# ---------------------------
dnl# If the system automatically restarts a system call that is
dnl# interrupted by a signal, define `HAVE_RESTARTABLE_SYSCALLS'.
AC_DEFUN([AC_SYS_RESTARTABLE_SYSCALLS],[
AC_REQUIRE([AC_HEADER_SYS_WAIT])dnl
AC_CACHE_CHECK([for restartable system calls],
               [ac_cv_sys_restartable_syscalls],
[AC_RUN_IFELSE([AC_LANG_SOURCE(
[/* Exit 0 (true) if wait returns something other than -1,
   i.e. the pid of the child, which means that wait was restarted
   after getting the signal.  */

AC_INCLUDES_DEFAULT
#include <signal.h>
#ifdef HAVE_SYS_WAIT_H
# include <sys/wait.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "conftest for restartable syscalls wants <sys/wait.h>."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_SYS_WAIT_H */

/* Some platforms explicitly require an extern "C" signal handler
   when using C++. */
#ifdef __cplusplus
extern "C" void ucatch(int dummy) { }
#else
void ucatch(dummy) int dummy; { }
#endif /* __cplusplus */

int main(void)
{
  int i = fork(), status;

  if (i == 0)
    {
      sleep(3);
      kill(getppid(), SIGINT);
      sleep(3);
      return 0;
    }

  signal(SIGINT, ucatch);

  status = wait(&i);
  if (status == -1)
    wait(&i);

  return status == -1;
}])],
	       [ac_cv_sys_restartable_syscalls=yes],
	       [ac_cv_sys_restartable_syscalls=no],
               [ac_cv_sys_restartable_syscalls=yes])])
if test "x${ac_cv_sys_restartable_syscalls}" = "xyes"; then
  AC_DEFINE([HAVE_RESTARTABLE_SYSCALLS],[1],
	    [Define to 1 if system calls automatically restart after
	     interruption by a signal.])
fi
])dnl# AC_SYS_RESTARTABLE_SYSCALLS
