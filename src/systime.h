/* systime.h - System-dependent definitions for time manipulations.
   Copyright (C) 1993-1994, 2002-2014 Free Software Foundation, Inc.

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

#ifndef EMACS_SYSTIME_H
#define EMACS_SYSTIME_H

#include <timespec.h>

INLINE_HEADER_BEGIN

#ifdef emacs
# if defined(HAVE_X_WINDOWS) || defined(HAVE_X11_X_H)
#  include <X11/X.h>
# else
#  ifndef EMACS_MACGUI_H
typedef unsigned long Time;
#  endif /* !EMACS_MACGUI_H */
# endif /* HAVE_X_WINDOWS || HAVE_X11_X_H */
#endif /* emacs */

/* this is defined by the AC_HEADER_TIME autoconf check: */
#ifdef TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# ifdef HAVE_SYS_TIME_H
#  include <sys/time.h> /* for 'struct timeval' */
# else
#  include <time.h>
# endif /* HAVE_SYS_TIME_H */
#endif /* TIME_WITH_SYS_TIME */

#ifdef HAVE_TZNAME
# ifndef tzname		/* For SGI.  */
extern char *tzname[];	/* RS6000 and others want it this way.  */
# endif /* !tzname */
#endif /* HAVE_TZNAME */

/* SVr4 does NOT actually declare the 'timezone' global variable
 * in its #include files:  */
#if defined(USG5_4) && (defined(HAVE_TIME_T) || defined(time_t) || defined(_TIME_T))
extern time_t timezone;
#endif /* USG5_4 && (HAVE_TIME_T || time_t || _TIME_T) */

#ifdef VMS
# if defined(VAXC) && 0
#  include "vmstime.h" /* (this header no longer exists) */
# endif /* VAXC && 0 */
#endif /* VMS */

/* On some configurations (hpux8.0, X11R4), sys/time.h and X11/Xos.h
   disagree about the name of the guard symbol.  */
#ifdef HPUX
# ifdef _STRUCT_TIMEVAL
#  ifndef __TIMEVAL__
#   define __TIMEVAL__
#  endif /* !__TIMEVAL__ */
# endif /* _STRUCT_TIMEVAL */
#endif /* HPUX */

/* EMACS_TIME is the type to use to represent temporal intervals -
 * struct timeval on some systems, int on others.  It can be passed as
 * the timeout argument to the select  system call.
 *
 * EMACS_SECS (TIME) is an rvalue for the seconds component of TIME.
 * EMACS_SET_SECS (TIME, SECONDS) sets that to SECONDS.
 *
 * EMACS_HAS_USECS is defined iff EMACS_TIME has a usecs component.
 * EMACS_USECS (TIME) is an rvalue for the microseconds component of TIME.
 * This returns zero if EMACS_TIME doesn't have a microseconds component.
 * EMACS_SET_USECS (TIME, MICROSECONDS) sets that to MICROSECONDS.
 * This does nothing if EMACS_TIME doesn't have a microseconds component.
 *
 * EMACS_SET_SECS_USECS (TIME, SECS, USECS) sets both components of TIME.
 *
 * EMACS_GET_TIME (TIME) stores the current system time in TIME, which
 * should be an lvalue.
 *
 * EMACS_ADD_TIME (DEST, SRC1, SRC2) adds SRC1 to SRC2 and stores the
 * result in DEST.  SRC should not be negative.
 *
 * EMACS_SUB_TIME (DEST, SRC1, SRC2) subtracts SRC2 from SRC1 and
 * stores the result in DEST.  SRC should not be negative.
 * EMACS_TIME_NEG_P (TIME) is true iff TIME is negative.
 */

#ifdef HAVE_TIMEVAL
# ifndef EMACS_HAS_USECS
#  define EMACS_HAS_USECS 1
# endif /* !EMACS_HAS_USECS */

# define EMACS_TIME struct timeval
# define EMACS_SECS(time)		    ((time).tv_sec  + 0)
# define EMACS_USECS(time)		    ((time).tv_usec + 0)
# define EMACS_SET_SECS(time, seconds)	    ((time).tv_sec  = (seconds))
# define EMACS_SET_USECS(time, microseconds) ((time).tv_usec = (microseconds))

/* On SVR4, the compiler may complain if given this extra BSD arg: */
# ifdef GETTIMEOFDAY_ONE_ARGUMENT
#  define EMACS_GET_TIME(time) gettimeofday(&(time))
# else /* not GETTIMEOFDAY_ONE_ARGUMENT */
/* Presumably the second arg is ignored: */
#  define EMACS_GET_TIME(time) gettimeofday(&(time), NULL)
# endif /* not GETTIMEOFDAY_ONE_ARGUMENT */

# define EMACS_ADD_TIME(dest, src1, src2)		\
   do {							\
     (dest).tv_sec  = (src1).tv_sec  + (src2).tv_sec;	\
     (dest).tv_usec = (src1).tv_usec + (src2).tv_usec;	\
     if ((dest).tv_usec > 1000000)			\
       (dest).tv_usec -= 1000000, (dest).tv_sec++;	\
   } while (0)

# define EMACS_SUB_TIME(dest, src1, src2)		\
   do {							\
     (dest).tv_sec  = (src1).tv_sec  - (src2).tv_sec;	\
     (dest).tv_usec = (src1).tv_usec - (src2).tv_usec;	\
     if ((dest).tv_usec < 0)				\
       (dest).tv_usec += 1000000, (dest).tv_sec--;	\
   } while (0)

# define EMACS_TIME_NEG_P(time)					\
   ((long)(time).tv_sec < 0					\
    || ((time).tv_sec == 0					\
        && (long)(time).tv_usec < 0))

#else /* ! defined (HAVE_TIMEVAL) */

# define EMACS_TIME int
# define EMACS_SECS(time)		    (time)
# define EMACS_USECS(time)		    0
# define EMACS_SET_SECS(time, seconds)	    ((time) = (seconds))
# define EMACS_SET_USECS(time, usecs)	    0

# define EMACS_GET_TIME(t) ((t) = time ((long *) 0))
# define EMACS_ADD_TIME(dest, src1, src2) ((dest) = (src1) + (src2))
# define EMACS_SUB_TIME(dest, src1, src2) ((dest) = (src1) - (src2))
# define EMACS_TIME_NEG_P(t) ((t) < 0)

#endif /* ! defined (HAVE_TIMEVAL) */

#define EMACS_SET_SECS_USECS(time, secs, usecs) 		\
  (EMACS_SET_SECS(time, secs), EMACS_SET_USECS(time, usecs))

/* Emacs uses struct timespec to represent nonnegative temporal intervals.
 *
 * WARNING: Since tv_sec might be an unsigned value, do not use struct
 * timespec as a general-purpose data type for adding or subtracting
 * arbitrary time values!  When computing A + B or A - B, typically A
 * should be an absolute time, since the epoch and B have a nonnegative
 * offset.  */

/* Return an invalid timespec.  */
INLINE struct timespec
invalid_timespec(void)
{
  return make_timespec(0, -1);
}

/* Return true if TIME is a valid timespec.  This currently doesn't worry
   about whether tv_nsec is less than TIMESPEC_RESOLUTION; leap seconds
   might cause a problem if it did.  */
INLINE bool timespec_valid_p(struct timespec t)
{
  return (t.tv_nsec >= 0);
}

/* Return current system time.  */
INLINE struct timespec current_timespec(void)
{
  struct timespec r;
  gettime(&r);
  return r;
}

/* defined in sysdep.c */
extern int set_file_times(int, const char *, struct timespec, struct timespec);
extern struct timeval make_timeval(struct timespec) ATTRIBUTE_CONST;

/* defined in keyboard.c */
extern void set_waiting_for_input(struct timespec *);

/* When lisp.h is not included Lisp_Object is not defined (this can
   happen when this files is used outside the src directory).
   Use GCPRO1 to determine if lisp.h was included.  */
#ifdef GCPRO1
/* originally defined in dired.c, but now static in editfns.c, so ifdef
 * it out: */
# if 0
extern Lisp_Object make_time(time_t);
# endif /* 0 */
/* defined in editfns.c */
extern Lisp_Object make_lisp_time(struct timespec);
extern bool decode_time_components(Lisp_Object, Lisp_Object, Lisp_Object,
                                   Lisp_Object, struct timespec *, double *);
extern struct timespec lisp_time_argument(Lisp_Object);
#endif /* GCPRO1 */

/* Compare times T1 and T2.  Value is 0 if T1 and T2 are the same.
 * Value is < 0 if T1 is less than T2.  Value is > 0 otherwise.  */

#define EMACS_TIME_CMP(T1, T2)			\
  (EMACS_SECS (T1) - EMACS_SECS (T2)		\
   + (EMACS_SECS (T1) == EMACS_SECS (T2)	\
      ? EMACS_USECS (T1) - EMACS_USECS (T2)	\
      : 0))

/* Compare times T1 and T2 for equality, inequality, and so on:  */
#define EMACS_TIME_EQ(T1, T2) (EMACS_TIME_CMP(T1, T2) == 0)
#define EMACS_TIME_NE(T1, T2) (EMACS_TIME_CMP(T1, T2) != 0)
#define EMACS_TIME_GT(T1, T2) (EMACS_TIME_CMP(T1, T2) > 0)
#define EMACS_TIME_GE(T1, T2) (EMACS_TIME_CMP(T1, T2) >= 0)
#define EMACS_TIME_LT(T1, T2) (EMACS_TIME_CMP(T1, T2) < 0)
#define EMACS_TIME_LE(T1, T2) (EMACS_TIME_CMP(T1, T2) <= 0)

INLINE_HEADER_END

#endif /* EMACS_SYSTIME_H */

/* arch-tag: dcb79915-cf99-4bce-9778-aade71d07651
   (do not change this comment) */
