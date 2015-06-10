/* runit.c -*- C -*- */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <pwd.h>
#include <unistd.h>
#include <err.h>
#include <signal.h>

#include "dumpemacs.h"

int decreasepriv(int debugflag);

int runit(const char * const argv[], int dropprivs)
{
  pid_t child;
  int ret;

#ifdef _DEBUG
  printf("Forking; will try to exec %s %s after the fork...\n", argv[0],
         ((argv[1] != NULL) ? argv[1] : ""));
#endif /* _DEBUG */
  child = fork();
  if (child == 0) {
    if (dropprivs && (geteuid() == 0)) {
      ret = decreasepriv(0);
      if (ret) {
	err(1, "failed to decrease privileges");
      }
    }

    ret = execve(argv[0], (char * const *)argv, 0);
    if (ret) {
      err(1, "execve(%s) failed", argv[0]);
    }
  } else {
    /* declare out here to avoid being re-declared and initialized each
     * loop iteration: */
    int loop_counter = 0;
    do {
      int status = 0;
      int wait_opts = WUNTRACED; /* leave out WNOHANG for now */
      pid_t rch = waitpid(child, &status, wait_opts);
      if (rch == -1) {
	err(1, "waitpid(%d, &%d, %d) failed (code %d) after %d attempt(s)",
            child, status, wait_opts, errno, loop_counter);
      } else if ((rch == 0) && (wait_opts & WNOHANG)) {
        warn("waitpid(%d, &%d, %d) child failed before reporting back",
             child, status, wait_opts);
      }

      if (WIFSTOPPED(status)) {
	continue;
      }

      if (WIFSIGNALED(status)) {
	errx(1, "child exited on signal %d (i.e. \"%s\").",
             WTERMSIG(status), strsignal(WTERMSIG(status)));
      }

      if (WIFEXITED(status)) {
	if (WEXITSTATUS(status) == 0) {
	  break; /* success */
	} else {
	  errx(1, "child exited with status %d", WEXITSTATUS(status));
        }
      }
      loop_counter++;
      /* oops, nvm, I think clang was right... */
    } while (1);
  }

  return 0;
}


int decreasepriv(int debugflag)
{
  struct passwd *nobody = NULL;
  int ret;

  nobody = getpwnam("nobody");
  if (nobody == NULL) {
    err(1, "getpwnam(nobody) failed");
  }

  ret = initgroups(nobody->pw_name, (int)nobody->pw_gid);
  if (ret) {
    err(1, "initgroups(%s, %lud) failed", nobody->pw_name, nobody->pw_gid);
  }

  ret = setgid(nobody->pw_gid);
  if (ret) {
    err(1, "setgid(%lud) failed", nobody->pw_gid);
  }

  ret = setuid(nobody->pw_uid);
  if (ret) {
    err(1, "setuid(%lud) failed", nobody->pw_uid);
  }

#if defined(HAVE_SYSTEM) && defined(HAVE_ID_BIN)
  ret = system("id");
  if (ret == -1) {
    err(1, "system(id) failed when invoking fork() or waitpid()");
  } else if (ret == 127) {
    err(1, "system(id) failed during execution of shell");
  }
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "not checking id"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_SYSTEM && HAVE_ID_BIN */

  if (debugflag > 0) {
    printf("debugflag is %d.\n", debugflag);
  }

  return 0;
}

/* EOF */
