/* Mocklisp compatibility functions for GNU Emacs Lisp interpreter.
   Copyright (C) 1985, 1986, 1995 Free Software Foundation, Inc.

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


/* Compatibility for mocklisp */

#include <config.h>
#include "lisp.h"
#include "buffer.h"
#include "mocklisp.h"

/* warning: Feval() now takes 2 arguments, but this file appears to have
 * been originally written when Feval() only took 1 argument...
 * most of the 2nd arguments I have added to it in this file are just
 * dummies... */

/* Now in lisp code ("macrocode..."): */
#if 0
/**/DEFUN ("ml-defun", Fml_defun, Sml_defun, 0, UNEVALLED, 0,
 doc: /* Define mocklisp functions.  */)
 (Lisp_Object args)
{
 Lisp_Object elt;

  while (!NILP (args))
    {
      elt = Fcar (args);
      Ffset (Fcar (elt), Fcons (Qmocklisp, Fcdr (elt)));
      args = Fcdr (args);
    }
  return Qnil;
}
#endif /* 0 */

DEFUN ("ml-if", Fml_if, Sml_if, 0, UNEVALLED, 0,
       doc: /* Mocklisp version of `if'.  */)
  (Lisp_Object args)
{
  register Lisp_Object val;
  struct gcpro gcpro1;

  val = Qnil;
  GCPRO1 (args);
  while (!NILP (args))
    {
      val = Feval (Fcar (args), Qnil);
      args = Fcdr (args);
      if (NILP (args)) break;
      if (XINT (val))
	{
	  val = Feval (Fcar (args), Qnil);
	  break;
	}
      args = Fcdr (args);
    }
  UNGCPRO;
  return val;
}

#if 0 /* Now converted to regular "while" by hairier conversion code.  */
/**/DEFUN ("ml-while", Fml_while, Sml_while, 1, UNEVALLED, 0,
           doc: /* `while' for mocklisp programs.  */)
  (Lisp_Object args)
{
  Lisp_Object test, body, tem;
  struct gcpro gcpro1, gcpro2;

  GCPRO2 (test, body);

  test = Fcar (args);
  body = Fcdr (args);
  while (tem = Feval (test, Qnil), XINT (tem))
    {
      QUIT;
      Fprogn (body);
   }

  UNGCPRO;
  return Qnil;
}
#endif /* 0 */

/* This is the main entry point to mocklisp execution.
 * When eval sees a mocklisp function being called, it calls here
 * with the unevaluated argument list */

Lisp_Object
ml_apply (Lisp_Object function, Lisp_Object args)
{
  register int count = (specpdl_ptr - specpdl);
  register Lisp_Object val;

  specbind (Qmocklisp_arguments, args);
  val = Fprogn (Fcdr (function));
  return unbind_to (count, val);
}

DEFUN ("ml-nargs", Fml_nargs, Sml_nargs, 0, 0, 0,
  doc: /* Number of arguments to currently executing mocklisp function.  */)
  (void)
{
  if (EQ (Vmocklisp_arguments, Qinteractive))
    return make_number (0);
  return Flength (Vmocklisp_arguments);
}

DEFUN ("ml-arg", Fml_arg, Sml_arg, 1, 2, 0,
  doc: /* Argument number N to currently executing mocklisp function.  */)
  (Lisp_Object n, Lisp_Object prompt)
{
  if (EQ (Vmocklisp_arguments, Qinteractive))
    return Fread_string (prompt, Qnil, Qnil, Qnil, Qnil);
  CHECK_NUMBER (n, 0);
  XSETINT (n, XINT (n) - 1);	/* Mocklisp likes to be origin-1 */
  return Fcar (Fnthcdr (n, Vmocklisp_arguments));
}

DEFUN ("ml-interactive", Fml_interactive, Sml_interactive, 0, 0, 0,
 doc: /* True if currently executing mocklisp function was called interactively.  */)
  (void)
{
  return (EQ (Vmocklisp_arguments, Qinteractive)) ? Qt : Qnil;
}

DEFUN ("ml-provide-prefix-argument", Fml_provide_prefix_argument, Sml_provide_prefix_argument,
  2, UNEVALLED, 0,
  doc: /* Evaluate second argument, using first argument as prefix arg value.  */)
  (Lisp_Object args)
{
  struct gcpro gcpro1;
  GCPRO1 (args);
  Vcurrent_prefix_arg = Feval (Fcar (args), Qnil);
  UNGCPRO;
  return Feval (Fcar (Fcdr (args)), Qnil);
}

DEFUN ("ml-prefix-argument-loop", Fml_prefix_argument_loop, Sml_prefix_argument_loop,
       0, UNEVALLED, 0,
       doc: /* undocumented.  */)
  (Lisp_Object args)
{
  register Lisp_Object tem;
  register int i;
  struct gcpro gcpro1;

  /* Set `arg' in case we call a built-in function that looks at it.  Still are a few. */
  if (NILP (Vcurrent_prefix_arg))
    i = 1;
  else
    {
      tem = Vcurrent_prefix_arg;
      if (CONSP (tem))
	tem = Fcar (tem);
      if (EQ (tem, Qminus))
	i = -1;
      else i = XINT (tem);
    }

  GCPRO1 (args);
  while (i-- > 0)
    Fprogn (args);
  UNGCPRO;
  return Qnil;
}

#if 0 /* Now in mlsupport.el */

DEFUN ("ml-substr", Fml_substr, Sml_substr, 3, 3, 0,
  doc: /* Return a substring of STRING, starting at index FROM and of length LENGTH.
If either FROM or LENGTH is negative, the length of STRING is added to it.  */)
  (Lisp_Object string, Lisp_Object from, Lisp_Object to)
{
  CHECK_STRING (string, 0);
  CHECK_NUMBER (from, 1);
  CHECK_NUMBER (to, 2);

  if (XINT (from) < 0)
    XSETINT (from, XINT (from) + XSTRING (string)->size);
  if (XINT (to) < 0)
    XSETINT (to, XINT (to) + XSTRING (string)->size);
  XSETINT (to, XINT (to) + XINT (from));
  return Fsubstring (string, from, to);
}
#endif /* 0 */
DEFUN ("insert-string", Finsert_string, Sinsert_string, 0, MANY, 0,
  doc: /* Mocklisp-compatibility insert function.
Like the function `insert' except that any argument that is a number
is converted into a string by expressing it in decimal.  */)
  (int nargs, Lisp_Object *args)
{
  register int argnum;
  register Lisp_Object tem;

  for (argnum = 0; argnum < nargs; argnum++)
    {
      tem = args[argnum];
    retry:
      if (INTEGERP (tem))
	tem = Fnumber_to_string (tem);
      if (STRINGP (tem))
	insert1 (tem);
      else
	{
	  tem = wrong_type_argument (Qstringp, tem);
	  goto retry;
	}
    }

  return Qnil;
}


void
syms_of_mocklisp (void)
{
  Qmocklisp = intern ("mocklisp");
  staticpro (&Qmocklisp);

#if 0
  defsubr (&Sml_defun);
#endif /* 0 */
  defsubr (&Sml_if);
#if 0
  defsubr (&Sml_while);
#endif /* 0 */
  defsubr (&Sml_arg);
  defsubr (&Sml_nargs);
  defsubr (&Sml_interactive);
  defsubr (&Sml_provide_prefix_argument);
  defsubr (&Sml_prefix_argument_loop);
#if 0
  defsubr (&Sml_substr);
#endif /* 0 */
  defsubr (&Sinsert_string);
  
  return;
}

/* EOF */
