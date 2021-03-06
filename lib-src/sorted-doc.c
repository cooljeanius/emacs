/* Give this program DOC-mm.nn.oo as standard input and it outputs to
   standard output a file of texinfo input containing the doc strings.

   Copyright (C) 1989, 1992, 1994, 1996, 1999, 2000, 2001, 2002, 2003,
                 2004, 2005, 2006, 2007 Free Software Foundation, Inc.

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
   along with GNU Emacs; see the file COPYING.  If not, write to the
   Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.

   This version sorts the output by function name.  */

#if defined(HAVE_CONFIG_H) || defined(emacs)
# ifdef emacs
#  undef emacs
# endif /* emacs */
# include <config.h>
/* On some systems, Emacs re-defines the malloc() and realloc() functions
 * for the sake of unexec.  We avoid doing that here since we do NOT use
 * unexec for this: */
# ifdef malloc
#  undef malloc
# endif /* malloc */
# ifdef realloc
#  undef realloc
# endif /* realloc */
#endif /* HAVE_CONFIG_H || emacs */

#include <stdio.h>
#include <ctype.h>
#ifdef DOS_NT
# include <fcntl.h>		/* for O_BINARY */
# include <io.h>		/* for setmode */
#endif /* DOS_NT */

#ifndef GCC_VERSION
# define GCC_VERSION (__GNUC__ * 1000 + __GNUC_MINOR__)
#endif /* GCC_VERSION */

#if (GCC_VERSION < 2007)
# define __attribute__(x)
#endif /* gcc pre-2.7 */

/* Attribute __malloc__ on functions was valid as of gcc 2.96. */
#ifndef ATTRIBUTE_MALLOC
# if (GCC_VERSION >= 2096)
#  define ATTRIBUTE_MALLOC __attribute__((__malloc__))
# else
#  define ATTRIBUTE_MALLOC
# endif /* GNUC >= 2.96 */
#endif /* ATTRIBUTE_MALLOC */

/* config.h includes <stdlib.h>: */
#if !defined(HAVE_STDLIB_H) && !defined(_STDLIB_H_)
# ifndef WINDOWSNT		/* src/s/ms-w32.h includes <stdlib.h> */
extern char *malloc(size_t size) ATTRIBUTE_MALLOC;
# endif /* !WINDOWSNT */
#endif /* !HAVE_STDLIB_H && !_STDLIB_H_ */

#define NUL	'\0'
#define MARKER '\037'

#ifndef DEBUG
# define DEBUG 0
#else
# undef DEBUG
#endif /* !DEBUG */

typedef struct line LINE;

struct line
{
  LINE *next;			/* ptr to next or NULL */
  char *line;			/* text of the line */
};

typedef struct docstr DOCSTR;

struct docstr			/* Allocated thing for an entry. */
{
  DOCSTR *next;			/* next in the chain */
  char *name;			/* name of the function or var */
  LINE *first;			/* first line of doc text. */
  char type;			/* 'F' for function, 'V' for variable */
};

/* prototypes: */
extern void error(const char *s1, const char *s2);
extern _Noreturn void fatal(const char *s1, const char *s2);
extern void *xmalloc(int size) ATTRIBUTE_MALLOC;
extern char *xstrdup(char *str);
extern int cmpdoc(DOCSTR **a, DOCSTR **b);

/* Print error message.
 * `s1' is printf control string, `s2' is arg for it: */
void
error(const char *s1, const char *s2)
{
  fprintf(stderr, "sorted-doc: ");
  fprintf(stderr, s1, s2);
  fprintf(stderr, "\n");
}

/* Print error message and exit: */
_Noreturn void
fatal(const char *s1, const char *s2)
{
  error(s1, s2);
  exit(EXIT_FAILURE);
}
/* ("../src/config.h" should define '_Noreturn' for that, if we do not
 * already have the C11 one) */

/* xmalloc(): Like malloc but get fatal error if memory is exhausted: */
void *
xmalloc(int size)
{
  void *result = malloc((unsigned)size);
  if (result == NULL) {
    fatal("%s", "virtual memory exhausted");
  }
  return result;
}

char *
xstrdup(char *str)
{
  char *buf = xmalloc(strlen(str) + 1);
  (void)strcpy(buf, str);
  return (buf);
}

/* Comparison function for qsort to call: */
int
cmpdoc(DOCSTR **a, DOCSTR **b)
{
  register int val = strcmp((*a)->name, (*b)->name);
  if (val) return val;
  return ((*a)->type - (*b)->type);
}


enum state
{
  WAITING, BEG_NAME, NAME_GET, BEG_DESC, DESC_GET
};

const char *states[] =
{
  "WAITING", "BEG_NAME", "NAME_GET", "BEG_DESC", "DESC_GET"
};

int
main(void)
{
  register DOCSTR *dp = NULL;	/* allocated DOCSTR */
  register LINE *lp = NULL;	/* allocated line */
  register char *bp;		/* ptr inside line buffer */
  register enum state state = WAITING; /* state at start */
  int cnt = 0;			/* number of DOCSTRs read */

  DOCSTR *docs = NULL;          /* chain of allocated DOCSTRS */
  char buf[512];		/* line buffer */

#ifdef DOS_NT
  /* DOC is a binary file: */
  if (!isatty(fileno(stdin))) {
    setmode(fileno(stdin), O_BINARY);
  }
#endif /* DOS_NT */

  bp = buf;

  while (1)			/* process one char at a time */
    {
      /* this char from the DOCSTR file: */
      register int ch = getchar();

      /* Beginnings: */
      if (state == WAITING)
	{
	  if (ch == MARKER) {
	    state = BEG_NAME;
          }
	}
      else if (state == BEG_NAME)
	{
	  cnt++;
	  if (dp == NULL)	/* first dp allocated */
	    {
	      docs = dp = (DOCSTR*)xmalloc(sizeof(DOCSTR));
	    }
	  else			/* all the rest */
	    {
	      dp->next = (DOCSTR*)xmalloc(sizeof(DOCSTR));
	      dp = dp->next;
	    }
	  lp = NULL;
	  dp->next = NULL;
	  bp = buf;
	  state = NAME_GET;
	  /* Record whether function or variable: */
	  dp->type = ch;
	  ch = getchar();
	}
      else if (state == BEG_DESC)
	{
	  if (lp == NULL)	/* first line for dp */
	    {
	      dp->first = lp = (LINE*)xmalloc(sizeof(LINE));
	    }
	  else			/* continuing lines */
	    {
	      lp->next = (LINE*)xmalloc(sizeof(LINE));
	      lp = lp->next;
	    }
	  lp->next = NULL;
	  bp = buf;
	  state = DESC_GET;
	}

      /* process gets: */
      if ((state == NAME_GET) || (state == DESC_GET))
	{
	  if ((ch != MARKER) && (ch != '\n') && (ch != EOF))
	    {
	      *bp++ = ch;
	    }
	  else			/* saving and changing state */
	    {
	      *bp = NUL;
	      bp = xstrdup(buf);

	      if (state == NAME_GET) {
		dp->name = bp;
	      } else {
		lp->line = bp;
              }

	      bp = buf;
	      state = ((ch == MARKER) ? BEG_NAME : BEG_DESC);
	    }
	}			/* NAME_GET || DESC_GET */
      if (ch == EOF) {
	break;
      }
    }

  {
    DOCSTR **array;
    register int i;		/* counter */

    /* build array of ptrs to DOCSTRs: */
    array = (DOCSTR**)xmalloc(cnt * sizeof(*array));
    for (dp = docs, i = 0; dp != NULL; dp = dp->next) {
      array[i++] = dp;
    }

    /* sort the array by name; within each name, by type: */
    qsort((char*)array, cnt, sizeof(DOCSTR*),
          (int (*)(const void *, const void *))cmpdoc);

    /* write the output header: */
    printf("\\input texinfo  @c -*-texinfo-*-\n");
    printf("@setfilename ../info/summary\n");
    printf("@settitle Command Summary for GNU Emacs\n");
    printf("@finalout\n");
    printf("@unnumbered Command Summary for GNU Emacs\n");
    printf("@table @asis\n");
    printf("\n");
    printf("@iftex\n");
    printf("@global@let@ITEM@item\n");
    printf("@def@item{@filbreak@vskip5pt@ITEM}\n");
    printf("@font@tensy cmsy10 scaled @magstephalf\n");
    printf("@font@teni cmmi10 scaled @magstephalf\n");
    printf("@def\\{{@tensy@char110}}\n"); /* this backslash goes with cmr10 */
    printf("@def|{{@tensy@char106}}\n");
    printf("@def@{{{@tensy@char102}}\n");
    printf("@def@}{{@tensy@char103}}\n");
    printf("@def<{{@teni@char62}}\n");
    printf("@def>{{@teni@char60}}\n");
    printf("@chardef@@64\n");
    printf("@catcode43=12\n");
    printf("@tableindent-0.2in\n");
    printf("@end iftex\n");

    /* print each function from the array: */
    for (i = 0; i < cnt; i++)
      {
	printf("\n@item %s @code{%s}\n@display\n",
               ((array[i]->type == 'F') ? "Function" : "Variable"),
               array[i]->name);

	for (lp = array[i]->first; lp != NULL ; lp = lp->next)
	  {
	    for (bp = lp->line; *bp; bp++)
	      {
		/* the characters "@{}" need special treatment: */
		if ((*bp == '@') || (*bp == '{') || (*bp == '}'))
		  {
		    putchar('@');
		  }
		putchar(*bp);
	      }
	    putchar('\n');
	  }
	printf("@end display\n");
	/* Try to avoid a save size overflow in the TeX output routine: */
	if (((i % 100) == 0) && (i > 0) && (i != cnt)) {
	  printf("\n@end table\n@table @asis\n");
        }
      }

    printf("@end table\n");
    printf("@bye\n");
  }

  return EXIT_SUCCESS;
}

/* arch-tag: ce28f204-1e70-4b34-8210-3d54a5662071
   (do not change this comment) */

#ifdef DEBUG
# undef DEBUG
#endif /* DEBUG */

/* sorted-doc.c ends here */
