/* blockinput.h - interface to blocking complicated interrupt-driven input.
   Copyright (C) 1989, 1993, 2001-2014 Free Software Foundation, Inc.

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

#ifndef EMACS_BLOCKINPUT_H
#define EMACS_BLOCKINPUT_H

#include "atimer.h"

INLINE_HEADER_BEGIN

/* Emacs should avoid doing anything hairy in a signal handler, because
   so many system functions are non-reentrant.  For example, malloc
   and the Xlib functions are NOT usually re-entrant, so if they were
   used by the SIGIO handler, we would lose.

   To avoid this, we make the following requirements:

   * Everyone must evaluate BLOCK_INPUT before performing actions that
   might conflict with a signal handler, and then call UNBLOCK_INPUT
   after performing them.  Calls BLOCK_INPUT and UNBLOCK_INPUT may be
   nested.

   * Any complicated interrupt handling code should test
   INPUT_BLOCKED_P, and put off its work until later.

   * If the interrupt handling code wishes, it may set
   pending_signals to a non-zero value.  If that flag is set
   when input becomes unblocked, UNBLOCK_INPUT will then read
   input and process timers.

   Historically, Emacs signal handlers did much more than they do now,
   and this caused many BLOCK_INPUT calls to be sprinkled around the code.
   FIXME: Remove calls that are NOT needed now.  */

extern volatile int interrupt_input_blocked;

/* Begin critical section. */
#ifndef BLOCK_INPUT
# define BLOCK_INPUT (interrupt_input_blocked++)
#endif /* !BLOCK_INPUT */

INLINE void block_input(void)
{
  interrupt_input_blocked++;
}

extern void unblock_input (void);
extern void totally_unblock_input (void);
extern void unblock_input_to (int);

/* End critical section.

   If doing signal-driven input, and a signal came in when input was
   blocked, reinvoke the signal handler now to deal with it.

   We used to have two possible definitions of this macro - one for
   when SIGIO was #defined, and one for when it was NOT; when SIGIO
   was NOT #defined, we would NOT bother to check if we should re-invoke
   the signal handler.  But that does NOT work very well; some of the
   files which use this macro do NOT #include the right files to get
   SIGIO.

   So, we always test interrupt_input_pending now; that is not too
   expensive, and it will never get set if we do NOT need to resignal. */

#ifndef UNBLOCK_INPUT
# define UNBLOCK_INPUT 				\
   do {						\
     --interrupt_input_blocked;                 \
     if (interrupt_input_blocked == 0) {	\
       if (0) {		                        \
         ;                                      \
       }                                        \
       if (0)                                   \
         do_pending_atimers();                  \
     } else if (interrupt_input_blocked < 0) {	\
       abort();                                 \
     }                                          \
   } while (0)
#endif /* !UNBLOCK_INPUT */

/* Undo any number of BLOCK_INPUT calls,
 * and also reinvoke any pending signal: */
#ifndef TOTALLY_UNBLOCK_INPUT
# define TOTALLY_UNBLOCK_INPUT			\
   do if (interrupt_input_blocked != 0)	 {	\
     interrupt_input_blocked = 1;		\
     UNBLOCK_INPUT;				\
   } while (0)
#endif /* !TOTALLY_UNBLOCK_INPUT */

/* Undo any number of BLOCK_INPUT calls down to level LEVEL,
 * and also (if the level is now 0) reinvoke any pending signal: */
#ifndef UNBLOCK_INPUT_TO
# define UNBLOCK_INPUT_TO(LEVEL)			\
   do {							\
     interrupt_input_blocked = (LEVEL) + 1;		\
     UNBLOCK_INPUT;					\
   } while (0)
#endif /* !UNBLOCK_INPUT_TO */

#ifndef UNBLOCK_INPUT_RESIGNAL
# define UNBLOCK_INPUT_RESIGNAL UNBLOCK_INPUT
#endif /* !UNBLOCK_INPUT_RESIGNAL */

/* In critical section ? */
#ifndef INPUT_BLOCKED_P
# define INPUT_BLOCKED_P (interrupt_input_blocked > 0)
#endif /* !INPUT_BLOCKED_P */

INLINE bool input_blocked_p(void)
{
  return (interrupt_input_blocked > 0);
}

INLINE_HEADER_END

#endif /* EMACS_BLOCKINPUT_H */

/* arch-tag: 51a9ec86-945a-4966-8f04-2d1341250e03
   (do not change this comment) */
