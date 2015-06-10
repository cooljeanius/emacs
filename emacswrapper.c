/* emacswrapper.c -*- C -*- */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <errno.h>
#include <err.h>
#include <sysexits.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "dumpemacs.h"

/* only one function so far: */
int main(int argc, char *argv[])
{
  int ret;

#if defined(DEBUG) || defined(lint)
  printf("Program is running from path '%s' with '%i' argument(s).\n",
         argv[0], argc);
#endif /* DEBUG || lint */

  if (!is_emacs_valid(0)) {
    const char *newargs[2];
    newargs[0] = kDumpEmacsPath;
    if (geteuid() != 0) {
      newargs[1] = "-n";
      newargs[2] = NULL;
    } else {
      newargs[1] = NULL;
    }

    ret = runit(newargs, 0);
    if (ret) {
      errx(1, "Failed to dump emacs");
    }
  }

#ifdef DEBUG
  printf("Done running dumpemacs.\n");
#endif /* DEBUG */

  ret = execv(kEmacsArchPath, argv);
  if (ret) {
    err(EX_OSERR, "execv(%s) failed", kEmacsArchPath);
  }

  /* we should never get here: */
  return EX_OSERR;
}

/* EOF */
