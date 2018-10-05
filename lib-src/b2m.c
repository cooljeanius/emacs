/* b2m.c -*- C -*-
 * b2m - a filter for Babyl -> Unix mail files
 * The copyright on this file has been disclaimed.
 *
 * usage:	b2m < babyl > mailbox
 *
 * I find this useful whenever I have to use a
 * system which - shock horror! - fails to run
 * GNU Emacs. At least now I can read all my
 * GNU Emacs Babyl format mail files!
 *
 * it is not much but it is free!
 *
 *   Ed Wilkinson
 *   E.Wilkinson@massey.ac.nz
 *   Mon Nov 7 15:54:06 PDT 1988
 */

/* Made conformant to the GNU coding standards January, 1995
   by Francesco Potorti` <pot@cnuce.cnr.it>. */

#if defined(HAVE_CONFIG_H) || defined(emacs)
# ifdef emacs
#  undef emacs
# endif /* emacs */
# include <config.h>
/* On some systems, Emacs defines static as nothing for the sake
 * of unexec.  We do NOT want that here since we do NOT use unexec. */
# ifdef static
#  undef static
# endif /* static */
/* likewise with the malloc() and realloc() functions: */
# ifdef malloc
#  undef malloc
# endif /* malloc */
# ifdef realloc
#  undef realloc
# endif /* realloc */
#endif /* HAVE_CONFIG_H || emacs */

#include <stdio.h>
#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif /* HAVE_STDLIB_H */
#ifdef HAVE_MALLOC_H
# include <malloc.h>
#else
# ifdef HAVE_MALLOC_MALLOC_H
#  include <malloc/malloc.h>
# endif /* HAVE_MALLOC_MALLOC_H */
#endif /* HAVE_MALLOC_H */
#include <time.h>
#include <sys/types.h>
#include <getopt.h>
#if defined(MSDOS) || defined(HAVE_FCNTL_H)
# include <fcntl.h>
#endif /* MSDOS || HAVE_FCNTL_H */

#undef TRUE
#define TRUE	1
#undef FALSE
#define FALSE	0

#define streq(s,t)	(strcmp(s, t) == 0)
#define strneq(s,t,n)	(strncmp(s, t, n) == 0)

typedef int logical;

#define TM_YEAR_BASE 1900

/* Nonzero if TM_YEAR is a struct tm's tm_year value that causes
   asctime to have well-defined behavior.  */
#ifndef TM_YEAR_IN_ASCTIME_RANGE
# define TM_YEAR_IN_ASCTIME_RANGE(tm_year) \
    (1000 - TM_YEAR_BASE <= (tm_year) && (tm_year) <= 9999 - TM_YEAR_BASE)
#endif /* !TM_YEAR_IN_ASCTIME_RANGE */

/*
 * A `struct linebuffer' is a structure which holds a line of text.
 * `readline' reads a line from a stream into a linebuffer and works
 * regardless of the length of the line.
 */
struct linebuffer
{
  long size;
  char *buffer;
};

#ifndef _STRING_H_
extern char *strtok(char *, const char *);
#endif /* !_STRING_H_ */

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

long *xmalloc(unsigned int size) ATTRIBUTE_MALLOC;
long *xrealloc(char *ptr, unsigned int size);
char *concat(const char *s1, const char *s2, const char *s3);
long readline(struct linebuffer *linebuffer, register FILE *stream);
void fatal(const char *message);

/*
 * xnew -- allocate storage.  SYNOPSIS: Type *xnew (int n, Type);
 */
#define xnew(n, Type)	((Type *)xmalloc((n) * sizeof (Type)))



char *progname;

struct option longopts[] =
{
  { "help",			no_argument,	   NULL,     'h'   },
  { "version",			no_argument,	   NULL,     'V'   },
  { 0 }
};

#if !defined(optind) && !defined(_GETOPT_H)
extern int optind;
#endif /* !optind && !_GETOPT_H */

int
main(int argc, char **argv)
{
  logical labels_saved, printing, header;
  time_t ltoday;
  struct tm *tm;
  char *labels, *p, *today;
  struct linebuffer data;

#ifdef MSDOS
  _fmode = O_BINARY;	/* all of files are treated as binary files */
# if __DJGPP__ > 1
  if (!isatty(fileno(stdout))) {
    setmode(fileno(stdout), O_BINARY);
  }
  if (!isatty(fileno(stdin))) {
    setmode(fileno(stdin), O_BINARY);
  }
# else /* not __DJGPP__ > 1 */
  (stdout)->_flag &= ~_IOTEXT;
  (stdin)->_flag &= ~_IOTEXT;
# endif /* not __DJGPP__ > 1 */
#endif /* MSDOS */
  progname = argv[0];

  while (1)
    {
      int opt = getopt_long(argc, argv, "hV", longopts, 0);
      if (opt == EOF) {
	break;
      }

      switch (opt)
	{
	case 'V':
	  printf("%s (GNU Emacs %s)\n", "b2m", VERSION);
	  puts("b2m is in the public domain.");
	  exit(EXIT_SUCCESS);

	case 'h':
	  fprintf(stderr, "Usage: %s <babylmailbox >unixmailbox\n",
                  progname);
	  exit(EXIT_SUCCESS);

        default:
          break;
	}
    }

  if (optind != argc)
    {
      fprintf(stderr, "Usage: %s <babylmailbox >unixmailbox\n", progname);
      exit(EXIT_SUCCESS);
    }

  labels_saved = printing = header = FALSE;
  ltoday = time(0);
  /* Convert to a string, checking for out-of-range time stamps.
   * Do NOT use 'ctime', as that might dump core if the hardware clock
   * is set to a bizarre value.  */
  tm = localtime(&ltoday);
  if (!(tm && TM_YEAR_IN_ASCTIME_RANGE(tm->tm_year)
        && (today = asctime(tm)))) {
    fatal("current time is out of range");
  }
  data.size = 200;
  data.buffer = xnew(200, char);

  if ((readline(&data, stdin) == 0)
      || !strneq(data.buffer, "BABYL OPTIONS:", 14)) {
    fatal("standard input is not a Babyl mailfile.");
  }

  labels = (char *)"";

  while (readline(&data, stdin) > 0)
    {
      if (streq(data.buffer, "*** EOOH ***") && !printing)
	{
	  printing = header = TRUE;
	  printf("From \"Babyl to mail by %s\" %s", progname, today);
	  continue;
	}

      if (data.buffer[0] == '\037')
	{
	  if (data.buffer[1] == '\0') {
              continue;
	  } else if (data.buffer[1] == '\f') {
	      /* Save labels. */
	      readline(&data, stdin);
	      p = strtok(data.buffer, " ,\r\n\t");
	      labels = (char *)"X-Babyl-Labels: ";

              while ((p = strtok(NULL, " ,\r\n\t"))) {
		labels = concat(labels, p, ", ");
              }

	      p = &labels[strlen(labels) - 2];
              if (*p == ',') {
		*p = '\0';
              }
	      printing = header = FALSE;
	      labels_saved = TRUE;
	      continue;
          }
	}

      if ((data.buffer[0] == '\0') && header)
	{
	  header = FALSE;
	  if (labels_saved) {
	    puts(labels);
          }
	}

      if (printing) {
	puts(data.buffer);
      }
    }

  return EXIT_SUCCESS;
}



/*
 * Return a newly-allocated string whose contents
 * concatenate those of s1, s2, s3.
 */
char *
concat(const char *s1, const char *s2, const char *s3)
{
  int len1 = strlen(s1), len2 = strlen(s2), len3 = strlen(s3);
  char *result = xnew(len1 + len2 + len3 + 1, char);

  strcpy(result, s1);
  strcpy((result + len1), s2);
  strcpy((result + len1 + len2), s3);
  result[len1 + len2 + len3] = '\0';

  return result;
}

/*
 * Read a line of text from `stream' into `linebuffer'.
 * Return the number of characters read from `stream',
 * which is the length of the line including the newline, if any.
 */
long
readline(struct linebuffer *linebuffer, register FILE *stream)
{
  char *buffer = linebuffer->buffer;
  register char *p = linebuffer->buffer;
  register char *pend;
  int chars_deleted;

  pend = p + linebuffer->size; /* Separate to avoid 386/IX compiler bug. */

  while (1)
    {
      register int c = getc(stream);
      if (p == pend)
	{
	  linebuffer->size *= 2;
	  buffer = (char *)xrealloc(buffer, linebuffer->size);
	  p += (buffer - linebuffer->buffer);
	  pend = (buffer + linebuffer->size);
	  linebuffer->buffer = buffer;
	}
      if (c == EOF)
	{
	  *p = '\0';
	  chars_deleted = 0;
	  break;
	}
      if (c == '\n')
	{
	  if ((p > buffer) && (p[-1] == '\r'))
	    {
	      *--p = '\0';
	      chars_deleted = 2;
	    }
	  else
	    {
	      *p = '\0';
	      chars_deleted = 1;
	    }
	  break;
	}
      *p++ = c;
    }

  return (p - buffer + chars_deleted);
}

/*
 * Like malloc but get fatal error if memory is exhausted.
 */
long *
xmalloc(unsigned int size)
{
  long *result = (long *)malloc(size);
  if (result == NULL) {
    fatal("virtual memory exhausted");
  }
  return result;
}

long *
xrealloc(char *ptr, unsigned int size)
{
  long *result = (long *)realloc(ptr, size);
  if (result == NULL) {
    fatal("virtual memory exhausted");
  }
  return result;
}

/* "../src/config.h" should define '_Noreturn' for us, if we do not
 * already have the C11 one: */
_Noreturn void
fatal(const char *message)
{
  fprintf(stderr, "%s: %s\n", progname, message);
  exit(EXIT_FAILURE);
}

/* arch-tag: 5a3ad2af-a802-408f-83cc-e7cf5e98653e
   (do not change this comment) */

/* b2m.c ends here */
