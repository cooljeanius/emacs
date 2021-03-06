/* sysselect.h - System-dependent definitions for the select function.
   Copyright (C) 1995, 2001-2014 Free Software Foundation, Inc.

This file is part of GNU Emacs.

GNU Emacs is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

GNU Emacs is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.  */

#ifndef EMACS_SYSSELECT_H
#define EMACS_SYSSELECT_H 1

#ifndef DOS_NT
# include <sys/select.h>
#endif /* !DOS_NT */

/* The w32 build defines select stuff in w32.h, which is included
   where w32 needs it, but not where sysselect.h is included.  The w32
   definitions in w32.h are incompatible with the below.  */
#ifndef WINDOWSNT
# ifdef FD_SET
#  ifndef FD_SETSIZE
#   define FD_SETSIZE 64
#  endif /* !FD_SETSIZE */
# else /* no FD_SET: */
#  define FD_SETSIZE 32
typedef int fd_set;

/* Define the macros to access a single-int bitmap of descriptors: */
#  define FD_SET(n, p) (*(p) |= (1 << (n)))
#  define FD_CLR(n, p) (*(p) &= ~(1 << (n)))
#  define FD_ISSET(n, p) (*(p) & (1 << (n)))
#  define FD_ZERO(p) (*(p) = 0)
# endif /* no FD_SET */
#endif /* not WINDOWSNT */

#if !defined (HAVE_SELECT)
# define select sys_select
#endif /* !HAVE_SELECT */

#ifdef MSDOS
# define pselect sys_select
#endif /* MSDOS */

#endif /* !EMACS_SYSSELECT_H */

/* arch-tag: 36d05500-8cf6-4847-8e78-6721f18c06ef
   (do not change this comment) */
