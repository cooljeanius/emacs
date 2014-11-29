/* lib-src/ntlib.h
 * Utility and Unix shadow routines for GNU Emacs support programs on NT.
 * Copyright (C) 1994, 2002-2014 Free Software Foundation, Inc. */
/*
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

#ifndef EMACS_NTLIB_H
#define EMACS_NTLIB_H 1

#include <pwd.h>
#if defined(_WIN32) || defined(HAVE_MALLOC_H) || defined(WINDOWSNT) || \
    defined(HAVE_NTGUI)
# include <malloc.h>
#else
# ifdef HAVE_MALLOC_MALLOC_H
#  include <malloc/malloc.h>
# else
#  if defined(__GNUC__) && !defined(__STRICT_ANSI__) && defined(lint)
#   warning "ntlib.h expects a malloc-related header to be included."
#  endif /* __GNUC__ && !__STRICT_ANSI__ && lint */
# endif /* HAVE_MALLOC_MALLOC_H */
#endif /* _WIN32 || HAVE_MALLOC_H || WINDOWSNT || HAVE_NTGUI */

/* Include these headers now so we do NOT have to worry about include
 * order dependencies in common source files.  */
#if defined(_WIN32) || defined(HAVE_DIRECT_H) || defined(WINDOWSNT) || \
    defined(HAVE_NTGUI)
# include <direct.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__) && defined(lint)
#  warning "ntlib.h expects <direct.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ && lint */
#endif /* _WIN32 || HAVE_DIRECT_H || WINDOWSNT || HAVE_NTGUI */
#include <fcntl.h>
#if defined(_WIN32) || defined(HAVE_IO_H) || defined(WINDOWSNT) || \
    defined(HAVE_NTGUI)
# include <io.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__) && defined(lint)
#  warning "ntlib.h expects <io.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ && lint */
#endif /* _WIN32 || HAVE_IO_H || WINDOWSNT || HAVE_NTGUI */
#include <stdio.h>

#ifdef sleep
# undef sleep
#endif /* sleep */
unsigned sleep (unsigned seconds);
char *getwd (char *dir);
int getppid (void);
char * getlogin (void);
char * cuserid (char * s);
unsigned getuid (void);
unsigned geteuid (void);
unsigned getegid (void);
unsigned getgid (void);
int setuid (unsigned uid);
int setregid (unsigned rgid, unsigned gid);
char * getpass (const char * prompt);
int fchown (int fd, unsigned uid, unsigned gid);
int mkostemp (char * template, int flags);

/* redirect or undo interceptions created by config.h */
#undef access
#define access  _access
#undef chdir
#define chdir   _chdir
#undef chmod
#define chmod   _chmod
#undef close
#define close   _close
#undef creat
#define creat   _creat
#undef ctime
#undef dup
#define dup     _dup
#undef dup2
#define dup2    _dup2
#undef fopen
#undef mkdir
#define mkdir   _mkdir
#undef open
#define open    _open
#undef pipe
#define pipe    _pipe
#undef read
#define read    _read
#undef rmdir
#define rmdir   _rmdir
#undef unlink
#define unlink  _unlink
#undef write
#define write   _write

/* map to MSVC names: */
#define execlp    _execlp
#define execvp    _execvp
#define fdopen	  _fdopen
#ifndef fileno
# define fileno	  _fileno
#endif /* !fileno */
#define getcwd	  _getcwd
#define getw	  _getw
#define getpid    _getpid
#define isatty    _isatty
#define locking   _locking
#define logb      _logb
#define _longjmp  longjmp
#define lseek     _lseek
#define popen     _popen
#define pclose    _pclose
#define umask	  _umask
#define utime	  _utime
#define index     strchr
#define rindex    strrchr

/* Make standard winsock definitions available if needed: */
#undef _WINSOCKAPI_
#undef _WINSOCK_H

#endif /* !EMACS_NTLIB_H */

/* end of ntlib.h */
