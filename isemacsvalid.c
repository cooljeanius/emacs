/* isemacsvalid.c */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

#include "dumpemacs.h"

int is_emacs_valid(int debugflag)
{
  struct stat sb;
  int ret;
  time_t lastMod;

  ret = lstat(kEmacsArchPath, &sb);
  if (ret == -1) {
    if (errno == ENOENT) {
      return 0; /* regenerate it */
    } else {
      return -1; /* error */
    }
  }

  if (S_ISDIR(sb.st_mode)) {
    errno = EISDIR;
    return -1;
  } else if (!S_ISREG(sb.st_mode)) {
    /* if it is not a file, then nuke it: */
    return 0;
  }

  /* It is a file. Check the times: */
  lastMod = sb.st_mtime;

  /* validate that the per-arch executable is
   * newer than the wrapper or dumpemacs: */
  ret = lstat(kEmacsWrapperPath, &sb);
  if (ret == -1) {
    return -1; /* error */
  }

  if (sb.st_mtime > lastMod) {
    /* if the wrapper is more recent than the per-arch, then: */
    return 0; /* (i.e. regenerate it) */
  }

  ret = lstat(kDumpEmacsPath, &sb);
  if (ret == -1) {
    return -1; /* error */
  }

  if (sb.st_mtime > lastMod) {
    /* if the dumpemacs is more recent than the per-arch, then: */
    return 0; /* (i.e. regenerate it) */
  }

  if (debugflag > 0) {
    ; /* FIXME: do something with it... */
  }

  /* per-arch emacs is present and newer than the
   * infrastructure tools, so let it be: */
  return 1;
}

/* FIXME: steal code from the machocheck program that comes with ld64 to
 * call from here */

/* The Makefile will define this for us when building the executable: */
#ifdef STANDALONE_ISEMACSVALID
int main(void)
{
  int ret = is_emacs_valid(0);
  printf("Is emacs valid?\n");
  switch (ret) {
    case -1:
      fprintf(stderr, "Error");
      if (errno != 0) {
        fprintf(stderr, ": %d (i.e. \"%s\")\n", errno, strerror(errno));
      } else {
        fprintf(stderr, ".\n");
      }
      break;
    case 0:
      printf("Per-arch emacs is outdated; need to regenerate it.\n");
      break;
    case 1:
      printf("Per-arch emacs is present and new enough, so let it be.\n");
      break;
    default:
      fprintf(stderr, "Unhandled return value for is_emacs_valid()!\n");
      break;
  }
}
#endif /* STANDALONE_ISEMACSVALID */

/* EOF */
