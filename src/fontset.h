/* Header for fontset handler.
   Copyright (C) 1998, 2001-2014 Free Software Foundation, Inc.
   Copyright (C) 1995, 1996, 1997, 1998, 1999, 2000, 2001, 2002, 2003, 2004,
     2005, 2006, 2007, 2008, 2009, 2010, 2011
     National Institute of Advanced Industrial Science and Technology (AIST)
     Registration Number H14PRO021
   Copyright (C) 2003, 2006
     National Institute of Advanced Industrial Science and Technology (AIST)
     Registration Number H13PRO009

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

#ifndef EMACS_FONTSET_H
#define EMACS_FONTSET_H

#ifndef EMACS_FRAME_H
/* Forward declaration for prototypes: */
struct frame;
#endif /* !EMACS_FRAME_H */

/* The following six are window system dependent functions.
 * Initialization routine of each window system should set appropriate
 * functions to these variables.  For instance, in case of X windows,
 * x_term_init used to do this.  */

/* Return a pointer to struct font_info of font FONT_IDX of frame F: */
extern struct font_info *(*get_font_info_func)(struct frame *f,
                                               int font_idx);

/* Return a list of font names which matches PATTERN.  See the document of
 * `x-list-fonts' for more detail.  */
extern Lisp_Object (*list_fonts_func)(struct frame *f, Lisp_Object pattern,
                                      int size, int maxnames);

/* Load a font named NAME for frame F and return a pointer to the
 * information of the loaded font.  If loading is failed, return -1: */
extern struct font_info *(*load_font_func)(struct frame *f,
                                           char *name, int);

/* Return a pointer to struct font_info of a font named NAME for frame F.
 * If no such font is loaded, return NULL: */
extern struct font_info *(*query_font_func)(struct frame *f, char *name);

/* Additional function for setting fontset or changing fontset
 * contents of frame F.  This function may change the coordinate of
 * the frame.  */
extern void (*set_frame_fontset_func)(struct frame *f, Lisp_Object arg,
                                      Lisp_Object oldval);

/* To find a CCL program, fs_load_font calls this function.
 * The argument is a pointer to the struct font_info.
 * This function set the memer `encoder' of the structure: */
extern void (*find_ccl_program_func)(struct font_info *);

/* Check if any window system is used now: */
extern void (*check_window_system_func)(void);

extern void free_face_fontset (struct frame *, struct face *);
extern int face_for_char (struct frame *, struct face *, int,
                          ptrdiff_t, Lisp_Object);
extern Lisp_Object font_for_char (struct face *, int, ptrdiff_t, Lisp_Object);

extern int make_fontset_for_ascii_face (struct frame *, int, struct face *);
extern int fontset_from_font (Lisp_Object);
extern int fs_query_fontset (Lisp_Object, int);
extern Lisp_Object list_fontsets (struct frame *, Lisp_Object, int);

extern Lisp_Object Qlatin;
extern Lisp_Object fontset_name (int);
extern Lisp_Object fontset_ascii (int);

extern int face_for_font (struct frame *, Lisp_Object, struct face *);

#endif /* EMACS_FONTSET_H */

/* EOF */
