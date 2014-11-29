/* mocklisp.h: Fundamental definitions for emulating mocklisp.
   Copyright (C) 1985, 1986, 1987 Free Software Foundation, Inc.

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
along with GNU Emacs; see the file COPYING.  If not, write to
the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#ifndef EMACS_MOCKLISP_H
#define EMACS_MOCKLISP_H 1

/* This is the main entry point to mocklisp execution.
 * When eval sees a mocklisp function being called, it calls here
 * with the unevaluated argument list */

extern Lisp_Object ml_apply (Lisp_Object function, Lisp_Object args);
extern Lisp_Object Fml_if (Lisp_Object args);
extern Lisp_Object Fml_nargs (void);
extern Lisp_Object Fml_arg (Lisp_Object n, Lisp_Object prompt);
extern Lisp_Object Fml_interactive (void);
extern Lisp_Object Fml_provide_prefix_argument (Lisp_Object args);
extern Lisp_Object Fml_prefix_argument_loop (Lisp_Object args);
extern Lisp_Object Finsert_string (int nargs, Lisp_Object *args);

#ifndef syms_of_mocklisp
void syms_of_mocklisp (void);
#endif /* !syms_of_mocklisp */

#endif /* !EMACS_MOCKLISP_H */

/* EOF */
