/* syssignal.h - System-dependent definitions for signals.

Copyright (C) 1993, 1999, 2001-2014 Free Software Foundation, Inc.

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

#ifndef EMACS_SYSSIGNAL_H
#define EMACS_SYSSIGNAL_H 1

#include <signal.h>

extern void init_signals (bool);

#ifdef HAVE_PTHREAD
# include <pthread.h>
/* If defined, asynchronous signals delivered to a non-main thread are
   forwarded to the main thread.  */
# define FORWARD_SIGNAL_TO_MAIN_THREAD
#endif /* HAVE_PTHREAD */

#if defined HAVE_TIMER_SETTIME && defined SIGEV_SIGNAL
# define HAVE_ITIMERSPEC
#endif /* HAVE_TIMER_SETTIME && SIGEV_SIGNAL */

#if (defined SIGPROF && !defined PROFILING \
     && (defined HAVE_SETITIMER || defined HAVE_ITIMERSPEC))
# define PROFILER_CPU_SUPPORT
#endif /* SIGPROF && !PROFILING && (HAVE_SETITIMER || HAVE_ITIMERSPEC) */

extern sigset_t empty_mask;

typedef void (*signal_handler_t) (int);

extern void emacs_sigaction_init (struct sigaction *, signal_handler_t);
char const *safe_strsignal (int) ATTRIBUTE_CONST;

#if defined(NSIG_MINIMUM) && (NSIG < NSIG_MINIMUM)
# undef NSIG
# define NSIG NSIG_MINIMUM
#endif /* NSIG_MINIMUM && (NSIG < NSIG_MINIMUM) */

#ifndef SA_SIGINFO
# define SA_SIGINFO 0
#endif /* !SA_SIGINFO */

#ifndef emacs_raise
# define emacs_raise(sig) raise (sig)
#endif /* !emacs_raise */

#if !defined(HAVE_STRSIGNAL) && !defined(strsignal)
# define strsignal(sig) safe_strsignal (sig)
#endif /* !HAVE_STRSIGNAL */

void deliver_process_signal (int, signal_handler_t);

#endif /* !EMACS_SYSSIGNAL_H */

/* arch-tag: 4580e86a-340d-4574-9e11-a742b6e1a152
   (do not change this comment) */
