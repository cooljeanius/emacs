/* sysstdio.h -*- C -*- */

#ifndef EMACS_SYSSTDIO_H
#define EMACS_SYSSTDIO_H 1

#if defined(__GNUC__) && (__GNUC__ >= 3)
# pragma GCC system_header
#endif /* gcc 3+ */

#include <stdio.h>
extern FILE *emacs_fopen (char const *, char const *);

#endif /* !EMACS_SYSSTDIO_H */

/* EOF */
