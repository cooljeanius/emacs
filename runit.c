/* runit.c -*- C -*- */

#ifdef HAVE_CONFIG_H
# include "src/config.h"
#endif /* HAVE_CONFIG_H */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <pwd.h>

/* "config.h" needs to go first */

#include <unistd.h>
#include <err.h>
#include <signal.h>
#include <sys/param.h>

#include "dumpemacs.h"

#if defined(malloc) || defined(realloc) || defined(temacs)
# include "src/unexec.h"
#endif /* malloc || realloc || temacs */

int decreasepriv(int debugflag);

int runit(const char * const argv[], int dropprivs)
{
  pid_t child;
  int ret;

#ifdef _DEBUG
  char *whole_argstring = NULL;
  int i;
  size_t prev_size = (ARG_MAX + 1UL);
  char *retstring;

  whole_argstring = (char *)malloc(prev_size);

  bzero(whole_argstring, prev_size);

  for (i = 0; argv[i] != NULL; i++) {
# if (_DEBUG > 1)
    printf("arg %d is %s.\n", i, argv[i]);
# endif /* (_DEBUG > 1) */
    prev_size += strlen(argv[i]);
    if (whole_argstring != NULL) {
      whole_argstring = (char *)reallocf(whole_argstring, prev_size);
    } else {
      err(1, "failed to allocate memory for string");
    }
    if ((strlen(argv[i]) + 1UL) > prev_size) {
      warn("%lu is greater than %lu; argstring printed will be truncated for arg %d",
           (strlen(argv[i]) + 1UL), prev_size, i);
    }

    retstring = strncat(whole_argstring, argv[i], (prev_size - 1UL));
    if (retstring == NULL) {
      warn("argstring concatenation failed for arg %d", i);
    }
    retstring = strncat(whole_argstring, " ", (prev_size - 1UL));
    if (retstring == NULL) {
      warn("failed to pad arg %d with an extra space", i);
    }
    retstring = NULL;
  }
# ifndef __clang_analyzer__
  prev_size = 0UL;
# endif /* !__clang_analyzer__ */
  printf("Forking; will try to exec %s after the fork...\n",
         whole_argstring);
  free(whole_argstring);
  whole_argstring = NULL;
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
      /* (oops, I think clang was right about that variable; moved above) */
    } while (1);
  }

  return 0;
}

/* */
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
