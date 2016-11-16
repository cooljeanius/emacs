/* x-list-font.c */

#include <config.h>
#include "lisp.h"
#include "frame.h"
#include "fontset.h"
#include "font.h"

#ifdef HAVE_X_WINDOWS
# include "xterm.h"
#else
# ifdef HAVE_CARBON
#  include "macterm.h"
# else
#  ifdef HAVE_NTGUI
#   include "w32term.h"
#  endif /* HAVE_NTGUI */
# endif /* HAVE_CARBON */
#endif /* HAVE_X_WINDOWS */

/* defined in w32faces.c: */
extern int face_name_id_number(FRAME_PTR f, Lisp_Object name);

DEFUN ("x-list-fonts", Fx_list_fonts, Sx_list_fonts, 1, 5, 0,
       doc: /* Return a list of the names of available fonts matching PATTERN.
If optional arguments FACE and FRAME are specified, return only fonts
the same size as FACE on FRAME.
PATTERN is a string, perhaps with wildcard characters;
  the * character matches any substring, and
  the ? character matches any single character.
  PATTERN is case-insensitive.
FACE is a face name--a symbol.

The return value is a list of strings, suitable as arguments to
set-face-font.

Fonts Emacs cannot use (i.e. proportional fonts) may or may not be excluded
even if they match PATTERN and FACE.
The optional fourth argument MAXIMUM sets a limit on how many
fonts to match.  The first MAXIMUM fonts are reported.
The optional fifth argument WIDTH, if specified, is a number of columns
occupied by a character of a font.  In that case, return only fonts
the WIDTH times as wide as FACE on FRAME.  */)
  (Lisp_Object pattern, Lisp_Object face, Lisp_Object frame,
   Lisp_Object maximum, Lisp_Object width)
{
  FRAME_PTR f;
  int size, cols = 0;
  int maxnames;

  (*check_window_system_func)();
  CHECK_STRING(pattern, 0);
  if (!NILP(face))
    CHECK_SYMBOL(face, 1);

  if (NILP(maximum))
    maxnames = 2000;
  else
    {
      CHECK_NATNUM(maximum, 0);
      maxnames = XINT(maximum);
    }

  if (!NILP(width))
    CHECK_NUMBER(width, 4);

  /* We cannot simply call check_x_frame because this function may be
     called before any frame is created.  */
  if (NILP(frame))
    f = XFRAME(selected_frame);
  else
    {
      CHECK_LIVE_FRAME(frame, 0);
      f = XFRAME(frame);
    }
  if (! FRAME_WINDOW_P(f))
    {
      /* Perhaps we have not yet created any frame.  */
      f = NULL;
      face = Qnil;
    }

  /* Determine the width standard for comparison with the fonts we find.  */

  if (NILP(face))
    size = 0;
  else
    {
      int face_id;

      face_id = face_name_id_number(f, face);

#ifdef FRAME_PARAM_FACES
      if (face_id < 0 || face_id >= FRAME_N_PARAM_FACES (f)
	  || FRAME_PARAM_FACES (f) [face_id] == 0
	  || FRAME_PARAM_FACES (f) [face_id]->font == (XFontStruct *) (~0))
	size = FONT_WIDTH (FRAME_FONT (f));
      else
	size = FONT_WIDTH (FRAME_PARAM_FACES (f) [face_id]->font);
#else
      if (face_id < 0) {
	size = FONT_WIDTH(FRAME_FONT(f));
      } else {
	size = 0;
      }
#endif /* FRAME_PARAM_FACES */

      if (!NILP(width))
	  size *= XINT(width);
    }

  {
    Lisp_Object args[2];

    args[0] = (*list_fonts_func)(f, pattern, size, maxnames);
    if (f == NULL)
      /* We do NOT have to check fontsets.  */
      return args[0];
    args[1] = list_fontsets(f, pattern, size);
    return Fnconc(2, args);
  }

  IF_LINT((void)Sx_list_fonts);
  IF_LINT((void)cols);
}

/* EOF */
