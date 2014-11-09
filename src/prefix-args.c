/* prefix-args.c - echo each argument, prefixed by a string.
   Copyright (C) 1992, 2001, 2002, 2003, 2004, 2005,
                 2006, 2007  Free Software Foundation, Inc.

This file is part of GNU Emacs.

GNU Emacs is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU Emacs is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Emacs; see the file COPYING.  If not, write to
the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.  */

/* Jim Blandy <jimb@occs.cs.oberlin.edu> - September 1992

   When using GCC 2 as the linker in the build process, options
   intended for the linker need to be prefixed with the "-Xlinker"
   option.  If an option takes an argument, we need to use -Xlinker
   twice - once for the option and once for its argument.  For
   example, to run the linker with the options "-Bstatic" "-e"
   "_start", you would need to pass the following options to GCC:

   -Xlinker -Bstatic -Xlinker -e -Xlinker _start.

   The Emacs makefile used to use a Bourne Shell `for' loop to prefix
   each linker option with "-Xlinker", but 1) the for loop was hairier
   than one might hope because it had to work when there were no
   arguments to pass to the linker - the shell barfs on a loop like
   this:

       for arg in ; do echo -Xlinker "$arg"; done

   and 2) the whole compilation command containing this loop seems to
   exit with a non-zero status and halt the build under Ultrix.

   If I cannot write a completely portable program to do this in C,
   I am quitting and taking up gardening.  */

#if defined(HAVE_CONFIG_H) || defined(emacs)
# include <config.h>
#endif /* HAVE_CONFIG_H || emacs */

#if (defined(STDC_HEADERS) && STDC_HEADERS) || defined(HAVE_STDLIB_H)
# include <stdlib.h>		/* for proper declaration of `exit' */
#endif /* STDC_HEADERS || HAVE_STDLIB_H */

#include <stdio.h>

int main(int argc, char **argv)
{
  char *progname;
  char *prefix;
  int i;

  progname = argv[0];
  argc--, argv++;

  if (argc < 1)
    {
      fprintf(stderr, "Usage: %s PREFIX ARGS...\n\
Echo each ARG preceded by PREFIX and a space.\n", progname);
      exit(2);
    }

  prefix = argv[0];
  argc--, argv++;

  for (i = 0; argc > 0; argc--, argv++) {
    printf("%s %s%c", prefix, argv[0], (argc > 1) ? ' ' : '\n');
    i++;
  }

  exit(0);
}

/* arch-tag: 08136d70-e5c0-49c7-bcd8-b4850233977a
   (do not change this comment) */
