/* Utility and Unix shadow routines for GNU Emacs support programs on NT.

Copyright (C) 1994, 2001-2014 Free Software Foundation, Inc.

Author: Geoff Voelker <voelker@cs.washington.edu>
Created: 10-8-94

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

/* try to keep <sys/time.h> from defining 'struct timezone' for us
 * when cross-compiling (fails, but whatever): */
#ifdef _DARWIN_C_SOURCE
# undef _DARWIN_C_SOURCE
# ifndef _POSIX_C_SOURCE
#  define _POSIX_C_SOURCE 1
# endif /* !_POSIX_C_SOURCE */
#endif /* !_DARWIN_C_SOURCE */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif /* HAVE_CONFIG_H */

#if !defined(__has_include)
# define __has_include(foo) 0
#endif /* !__has_include */

#if defined(_WIN32) || defined(HAVE_WINDOWS_H) || defined(WINDOWSNT) || \
    defined(_WIN32_WINNT) || defined(HAVE_NTGUI) || __has_include(<windows.h>)
# include <windows.h>
#else
# ifdef WINDOWS_COMPATIBILITY_CHECK
#  include "wincompat.h"
# else
#  if defined(__GNUC__) && !defined(__STRICT_ANSI__) && defined(lint)
#   warning "ntlib.c expects <windows.h> to be included."
#  endif /* __GNUC__ && !__STRICT_ANSI__ && lint */
# endif /* WINDOWS_COMPATIBILITY_CHECK */
#endif /* _WIN32 || HAVE_WINDOWS_H || WINDOWSNT || _WIN32_WINNT || HAVE_NTGUI */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#if defined(_WIN32) || defined(HAVE_DIRECT_H) || defined(WINDOWSNT) || \
    defined(HAVE_NTGUI) || __has_include(<direct.h>)
# include <direct.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__) && defined(lint)
#  warning "ntlib.c expects <direct.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ && lint */
#endif /* _WIN32 || HAVE_DIRECT_H || WINDOWSNT || HAVE_NTGUI */
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>
#include <sys/timeb.h>
#if defined(_WIN32) || defined(HAVE_MBSTRING_H) || defined(WINDOWSNT) || \
    defined(HAVE_NTGUI) || __has_include(<mbstring.h>)
# include <mbstring.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__) && defined(lint)
#  warning "ntlib.c expects <mbstring.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ && lint */
#endif /* _WIN32 || HAVE_MBSTRING_H || WINDOWSNT || HAVE_NTGUI */

#include "ntlib.h"

/* MinGW64 defines _TIMEZONE_DEFINED and defines 'struct timespec' in
 * its system headers.  */
#if !defined(_TIMEZONE_DEFINED) && !defined(_SYS_TIME_H_)
struct timezone
{
  int tz_minuteswest;	/* minutes west of Greenwich */
  int tz_dsttime;	/* type of dst correction */
};
#endif /* !_TIMEZONE_DEFINED && !_SYS_TIME_H_ */

#ifndef _POSIX_PATH_MAX
# define _POSIX_PATH_MAX 256
#endif /* !_POSIX_PATH_MAX */

/* Do NOT include sys/param.h if it already has been: */
#if defined HAVE_SYS_PARAM_H && !defined PATH_MAX && !defined MAXPATHLEN
# include <sys/param.h>
#endif /* HAVE_SYS_PARAM_H && !PATH_MAX && !MAXPATHLEN */

#ifndef MAXPATHLEN
# ifdef _MAX_PATH
#  define MAXPATHLEN _MAX_PATH
# else
#  ifdef PATH_MAX
#   define MAXPATHLEN PATH_MAX
#  else
#   define MAXPATHLEN 260
#  endif /* PATH_MAX */
# endif /* _MAX_PATH */
#endif /* !MAXPATHLEN */

#if defined(__GNUC__) && defined(__STDC_VERSION__)
# if (__GNUC__ >= 5) || (__STDC_VERSION__ >= 201112L)
#  pragma GCC diagnostic ignored "-Wimplicit-function-declaration"
#  pragma GCC diagnostic ignored "-Wmissing-prototypes"
# endif /* gcc 5, or C11 */
#endif /* __GNUC__ && __STDC_VERSION__ */

/* Emulate sleep... we could have done this with a define, but that
 * would necessitate including windows.h in the files that used it.
 * This is much easier.  */
unsigned int
sleep(unsigned seconds)
{
  Sleep(seconds * 1000);
  return 0;
}

/* Get the current working directory: */
char *
getwd(char *dir)
{
  if (GetCurrentDirectory(MAXPATHLEN, dir) > 0)
    return dir;
  return NULL;
}

static HANDLE getppid_parent;
static int getppid_ppid;

int
getppid(void)
{
  char *ppid;
  DWORD result;

  ppid = getenv("EM_PARENT_PROCESS_ID");
  if (!ppid)
    {
      printf("no pid.\n");
      return 0;
    }
  else
    {
      getppid_ppid = atoi(ppid);
    }

  if (!getppid_parent)
    {
      getppid_parent = OpenProcess(SYNCHRONIZE, FALSE, atoi(ppid));
      if (!getppid_parent)
	{
	  printf("Failed to open handle to parent process: %d\n",
		 GetLastError());
	  exit(1);
	}
    }

  result = WaitForSingleObject(getppid_parent, 0);
  switch (result)
    {
    case WAIT_TIMEOUT:
      /* The parent is still alive.  */
      return getppid_ppid;
    case WAIT_OBJECT_0:
      /* The parent is gone.  Return the pid of Unix init (1): */
      return 1;
    case WAIT_FAILED:
    default:
      printf("Checking parent status failed: %d\n", GetLastError());
      exit(1);
    }
}

char *
getlogin(void)
{
  static char user_name[256];
  DWORD length = sizeof(user_name);

  if (GetUserName(user_name, &length))
    return user_name;
  return NULL;
}

char *
cuserid(char * s)
{
  char * name = getlogin();
  if (s)
    return strcpy(s, name ? name : "");
  return name;
}

unsigned
getuid(void)
{
  return 0;
}

unsigned
geteuid(void)
{
  return getuid();
}

unsigned
getgid(void)
{
  return 0;
}

unsigned
getegid(void)
{
  return 0;
}

int
setuid(unsigned uid)
{
  return 0;
}

int
setregid(unsigned rgid, unsigned gid)
{
  return 0;
}

struct passwd *
getpwuid(unsigned uid)
{
  return NULL;
}

char *
getpass(const char * prompt)
{
  static char input[256];
  HANDLE in;
  HANDLE err;
  DWORD  count;

  in = GetStdHandle(STD_INPUT_HANDLE);
  err = GetStdHandle(STD_ERROR_HANDLE);

  if ((in == INVALID_HANDLE_VALUE) || (err == INVALID_HANDLE_VALUE))
    return NULL;

  if (WriteFile(err, prompt, strlen (prompt), &count, NULL))
    {
      int istty = (GetFileType(in) == FILE_TYPE_CHAR);
      DWORD old_flags;
      int rc;

      if (istty)
	{
	  if (GetConsoleMode(in, &old_flags))
	    SetConsoleMode(in, ENABLE_LINE_INPUT | ENABLE_PROCESSED_INPUT);
	  else
	    istty = 0;
	}
      rc = ReadFile(in, input, sizeof(input), &count, NULL);
      if ((count >= 2) && (input[count - 2] == '\r'))
	input[count - 2] = '\0';
      else
	{
	  char buf[256];
	  while (ReadFile(in, buf, sizeof(buf), &count, NULL) > 0)
	    if ((count >= 2) && (buf[count - 2] == '\r'))
	      break;
	}
      WriteFile(err, "\r\n", 2, &count, NULL);
      if (istty)
	SetConsoleMode(in, old_flags);
      if (rc)
	return input;
    }

  return NULL;
}

#if !defined(HAVE_GETTIMEOFDAY) && !defined(gettimeofday)
/* This is needed because lib/gettime.c calls gettimeofday, which MSVC
 * does NOT have.  Copied from w32.c.  */
# if defined(GETTIMEOFDAY_TWO_ARGUMENTS) && GETTIMEOFDAY_TWO_ARGUMENTS
void
gettimeofday(struct timeval *tv, struct timezone *tz)
{
  struct _timeb tb;
  _ftime(&tb);

  tv->tv_sec = tb.time;
  tv->tv_usec = (tb.millitm * 1000L);
  /* Implementation note: _ftime sometimes does NOT update the dstflag
   * according to the new timezone when the system timezone is
   * changed.  We could fix that by using GetSystemTime and
   * GetTimeZoneInformation, but that does NOT seem necessary, since
   * Emacs always calls gettimeofday with the 2nd argument NULL (see
   * current_emacs_time).  */
  if (tz)
    {
      tz->tz_minuteswest = tb.timezone;	/* minutes west of Greenwich  */
      tz->tz_dsttime = tb.dstflag;	/* type of dst correction  */
    }
}
# else
#  if defined(GETTIMEOFDAY_ONE_ARGUMENT) && GETTIMEOFDAY_ONE_ARGUMENT
void
gettimeofday(struct timeval *tv)
{
  struct _timeb tb;
  _ftime(&tb);

  tv->tv_sec = tb.time;
  tv->tv_usec = (tb.millitm * 1000L);
}
#  else
#   if defined(__GNUC__) && !defined(__STRICT_ANSI__) && defined(lint)
#    warning "need to re-implement gettimeofday(); but unsure how many arguments it takes..."
#   endif /* __GNUC__ && !__STRICT_ANSI__ && lint */
#  endif /* GETTIMEOFDAY_ONE_ARGUMENT */
# endif /* GETTIMEOFDAY_TWO_ARGUMENTS */
#endif /* !HAVE_GETTIMEOFDAY && !gettimeofday */

int
fchown(int fd, unsigned uid, unsigned gid)
{
  return 0;
}

/* Place a wrapper around the MSVC version of ctime.  It returns NULL
 * on network directories, so we handle that case here.
 * (Ulrich Leodolter, 1/11/95).  */
char *
sys_ctime(const time_t *t)
{
  char *str = (char *)ctime(t);
  return (str ? str : (char *)"Sun Jan 01 00:00:00 1970");
}

FILE *
sys_fopen(const char * path, const char * mode)
{
  return fopen(path, mode);
}

int
sys_chdir(const char * path)
{
  return _chdir(path);
}

static FILETIME utc_base_ft;
static long double utc_base;
static int init = 0;

static time_t
convert_time(FILETIME ft)
{
  long double ret;

  if (CompareFileTime(&ft, &utc_base_ft) < 0)
    return 0;

  ret = ((long double)ft.dwHighDateTime
         * 4096.0L * 1024.0L * 1024.0L + ft.dwLowDateTime);
  ret -= utc_base;
  return (time_t)(ret * 1e-7L);
}

static int
is_exec(const char * name)
{
  char * p = strrchr(name, '.');
  return
    (p != NULL
     && ((stricmp(p, ".exe") == 0) ||
	 (stricmp(p, ".com") == 0) ||
	 (stricmp(p, ".bat") == 0) ||
	 (stricmp(p, ".cmd") == 0)));
}

/* FIXME?  This is in config.nt now - is this still needed?
 * ifdef it out for now: */
#ifndef IS_DIRECTORY_SEP
# define IS_DIRECTORY_SEP(x) ((x) == '/' || (x) == '\\')
#endif /* !IS_DIRECTORY_SEP */

/* We need this because nt/inc/sys/stat.h defines struct stat that is
 * incompatible with the MS run-time libraries: */
int
stat(const char * path, struct stat * buf)
{
  WIN32_FIND_DATA wfd;
  HANDLE fh;
  int permission;
  int len;
  int rootdir = FALSE;
  char *name = alloca(FILENAME_MAX);

  if (!init)
    {
      /* Determine the delta between 1-Jan-1601 and 1-Jan-1970. */
      SYSTEMTIME st;

      st.wYear = 1970;
      st.wMonth = 1;
      st.wDay = 1;
      st.wHour = 0;
      st.wMinute = 0;
      st.wSecond = 0;
      st.wMilliseconds = 0;

      SystemTimeToFileTime (&st, &utc_base_ft);
      utc_base = ((long double)utc_base_ft.dwHighDateTime
	* 4096.0L * 1024.0L * 1024.0L + utc_base_ft.dwLowDateTime);
      init = 1;
    }

  if ((path == NULL) || (buf == NULL) || (*path == '\0'))
    {
      errno = EFAULT;
      return -1;
    }
  if (_mbspbrk(path, "*?|<>\""))
    {
      errno = ENOENT;
      return -1;
    }

  strcpy(name, path);
  /* Remove trailing directory separator, unless name is the root
   * directory of a drive in which case ensure there is a trailing
   * separator: */
  len = strlen(name);
  rootdir = (IS_DIRECTORY_SEP(name[0])
    || ((len == 3) && (name[1] == ':') && IS_DIRECTORY_SEP(name[2])));
  if (rootdir)
    {
      if (GetDriveType(name) < 2)
	{
	  errno = ENOENT;
	  return -1;
	}
      memset(&wfd, 0, sizeof(wfd));
      wfd.dwFileAttributes = FILE_ATTRIBUTE_DIRECTORY;
      wfd.ftCreationTime = utc_base_ft;
      wfd.ftLastAccessTime = utc_base_ft;
      wfd.ftLastWriteTime = utc_base_ft;
      strcpy(wfd.cFileName, name);
    }
  else
    {
      if (IS_DIRECTORY_SEP(name[len - 1]))
	name[len - 1] = 0;

      fh = FindFirstFile(name, &wfd);
      if (fh == INVALID_HANDLE_VALUE)
	{
	  errno = ENOENT;
	  return -1;
	}
      FindClose(fh);
    }
  buf->st_mode = ((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ?
                  S_IFDIR : S_IFREG);
  buf->st_nlink = 1;
  buf->st_ino = 0;

  if (name[0] && (name[1] == ':'))
    buf->st_dev = (tolower(name[0]) - 'a' + 1);
  else
    buf->st_dev = _getdrive();
  buf->st_rdev = buf->st_dev;

  buf->st_size = wfd.nFileSizeLow;

  /* Convert timestamps to Unix format: */
  buf->st_mtime = convert_time(wfd.ftLastWriteTime);
  buf->st_atime = convert_time(wfd.ftLastAccessTime);
  if (buf->st_atime == 0) buf->st_atime = buf->st_mtime;
  buf->st_ctime = convert_time(wfd.ftCreationTime);
  if (buf->st_ctime == 0) buf->st_ctime = buf->st_mtime;

  /* determine rwx permissions: */
  if (wfd.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
    permission = S_IREAD;
  else
    permission = (S_IREAD | S_IWRITE);

  if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    permission |= S_IEXEC;
  else if (is_exec(name))
    permission |= S_IEXEC;

  buf->st_mode |= (permission | (permission >> 3) | (permission >> 6));

  return 0;
}

int
lstat(const char * path, struct stat * buf)
{
  return stat(path, buf);
}

/* Implementation of mkostemp for MS-Windows, to avoid race conditions
 * when using mktemp.  Copied from w32.c.
 *
 * This is used only in update-game-score.c.  It is overkill for that
 * use case, since update-game-score renames the temporary file into
 * the game score file, which is NOT atomic on MS-Windows anyway, when
 * the game score already existed before running the program, which it
 * almost always does.  But using a simpler implementation just to
 * make a point is uneconomical...  */
int
mkostemp(char * template, int flags)
{
  char * p;
  int i, fd = -1;
  unsigned uid = GetCurrentThreadId();
  int save_errno = errno;
  static char first_char[] = "abcdefghijklmnopqrstuvwyz0123456789!%-_@#";

  errno = EINVAL;
  if (template == NULL)
    return -1;

  p = template + strlen(template);
  i = 5;
  /* replace up to the last 5 X's with uid in decimal: */
  while ((--p >= template) && (p[0] == 'X') && (--i >= 0))
    {
      p[0] = ('0' + uid % 10);
      uid /= 10;
    }

  if ((i < 0) && (p[0] == 'X'))
    {
      i = 0;
      do {
	  p[0] = first_char[i];
	  if ((fd = open(template,
                         (flags | _O_CREAT | _O_EXCL | _O_RDWR),
                         (S_IRUSR | S_IWUSR))) >= 0
	      || (errno != EEXIST))
	    {
	      if (fd >= 0)
		errno = save_errno;
	      return fd;
	    }
      } while (++i < sizeof(first_char));
    }

  /* Template is badly formed or else we cannot generate a unique name: */
  return -1;
}

/* On Windows, you cannot rename into an existing file: */
int
sys_rename(const char *from, const char *to)
{
  int retval = rename(from, to);

  if ((retval < 0) && (errno == EEXIST))
    {
      if (unlink(to) == 0)
	retval = rename(from, to);
    }
  return retval;
}

/* '-Wunused-macros': */
#ifdef _POSIX_PATH_MAX
# undef _POSIX_PATH_MAX
#endif /* _POSIX_PATH_MAX */

/* EOF */
