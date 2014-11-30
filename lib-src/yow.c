/*
 * yow.c
 *
 * Print a quotation from Zippy the Pinhead.
 * Qux <Kaufman-David@Yale> March 6, 1986
 *
 * This file is in the public domain because the author published it
 * with no copyright notice before the US signed the Bern Convention.
 *
 * With dynamic memory allocation.
 */

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
#include <../src/epaths.h>      /* For PATH_DATA.  */

#ifdef HAVE_STRING_H
# include <string.h>
#endif /* HAVE_STRING_H */

#ifdef HAVE_UNISTD_H
# include <unistd.h> /* For getpid() */
#endif /* HAVE_UNISTD_H */

#define BUFSIZE  80
#define SEP      '\0'

#ifndef YOW_FILE
# define YOW_FILE "yow.lines"
#endif /* !YOW_FILE */

#ifdef MSDOS
#define rootrelativepath(rel) \
({ \
    static char res[BUFSIZE], *p; \
    strcpy(res, argv[0]); \
    p = (res + strlen(res)); \
    while ((p != res) && (*p != '/') && (*p != '\\') && (*p != ':')) p--; \
    strcpy((p + 1), "../"); \
    strcpy((p + 4), rel); \
    &res;})
#endif /* MSDOS */

#if !defined(HAVE_STDLIB_H) && !defined(_STDLIB_H_)
char *malloc(size_t size);
char *realloc(void *ptr, size_t size);
#endif /* !HAVE_STDLIB_H && !_STDLIB_H_ */

void yow(FILE *fp);
void setup_yow(FILE *fp);

int
main(int argc, char *argv[])
{
  FILE *fp;
  char file[BUFSIZ];

  if ((argc > 2) && !strcmp(argv[1], "-f")) {
    strcpy(file, argv[2]);
  } else {
#ifdef vms
    sprintf(file, "%s%s", PATH_DATA, YOW_FILE);
#else
    sprintf(file, "%s/%s", PATH_DATA, YOW_FILE);
#endif /* vms */
  }

  if ((fp = fopen(file, "r")) == NULL) {
    fprintf(stderr, "yow: ");
    perror(file);
    exit(1);
  }

  /* initialize random seed: */
  srand((int)(getpid() + time((long *) 0)));

  setup_yow(fp);
  yow(fp);
  fclose(fp);
  return 0;
}

static long len = -1;
static long header_len;

#define AVG_LEN 40    /* average length of a quotation */

/* Sets len and header_len: */
void
setup_yow(FILE *fp)
{
  int c;

  /* Get length of file */
  /* Because the header (stuff before the first SEP) can be very long,
   * thus biasing our search in favor of the first quotation in the file,
   * we explicitly skip that. */
  while ((c = getc(fp)) != SEP) {
    if (c == EOF) {
      fprintf(stderr, "yow: file contains no separators\n");
      exit(2);
    }
  }
  header_len = ftell(fp);
  if (header_len > AVG_LEN) {
    header_len -= AVG_LEN; /* allow the first quotation to appear */
  }

  if (fseek(fp, 0L, 2) == -1) {
    perror("yow");
    exit(1);
  }
  len = (ftell(fp) - header_len);
}


/* go to a random place in the file and print the quotation there: */
void
yow(FILE *fp)
{
  long offset;
  int c, i = 0;
  char *buf;
  unsigned int bufsize;

  offset = (rand() % len + header_len);
  if (fseek(fp, offset, 0) == -1) {
    perror("yow");
    exit(1);
  }

  /* Read until SEP, read next line, print it.
   * (Note that we will never print anything before the first separator.)
   * If we hit EOF looking for the first SEP, just recurse. */
  while ((c = getc(fp)) != SEP)
    if (c == EOF) {
      yow(fp);
      return;
    }

  /* Skip leading whitespace, then read in a quotation.
   * If we hit EOF before we find a non-whitespace char, recurse. */
  while (isspace(c = getc(fp))) {
    ;
  }
  if (c == EOF) {
    yow(fp);
    return;
  }

  bufsize = BUFSIZE;
  buf = malloc(bufsize);
  if (buf == (char *)0) {
    fprintf(stderr, "yow: virtual memory exhausted\n");
    exit (3);
  }

  buf[i++] = c;
  while ((c = getc(fp)) != SEP && c != EOF) {
    buf[i++] = c;

    if (i == bufsize-1) {
      /* Yow! Is this quotation too long yet? */
      bufsize *= 2;
      buf = realloc(buf, bufsize);
      if (buf == (char *)0) {
	fprintf(stderr, "yow: virtual memory exhausted\n");
	exit(3);
      }
    }
  }
  buf[i++] = 0;
  printf("%s\n", buf);
}

/* EOF */
