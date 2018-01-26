/* menu.h: Functions to manipulate menus.
   Copyright (C) 2008-2014 Free Software Foundation, Inc.

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

#ifndef MENU_H
#define MENU_H

/* this file contains way too many of these for now: */
#if defined(__GNUC__) && defined(__GNUC_MINOR__) && \
    ((__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6)))
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wredundant-decls"
#endif /* gcc 4.6+ */

#include "systime.h" /* for Time */

#ifdef HAVE_NTGUI
extern Lisp_Object Qunsupported__w32_dialog;
#endif /* HAVE_NTGUI */

extern void x_set_menu_bar_lines(struct frame *f,
                                 Lisp_Object value,
                                 Lisp_Object oldval);

extern void init_menu_items(void);
extern void finish_menu_items(void);
extern void discard_menu_items(void);
extern void save_menu_items(void);
extern bool parse_single_submenu(Lisp_Object, Lisp_Object, Lisp_Object);
extern void list_of_panes(Lisp_Object);
#if defined(USE_X_TOOLKIT) || defined(USE_GTK) || defined(HAVE_NTGUI) \
  || defined(HAVE_CARBON) || defined(HAVE_NS)
extern void free_menubar_widget_value_tree(widget_value *);
extern void update_submenu_strings(widget_value *);
extern void find_and_call_menu_selection(struct frame *, int,
                                         Lisp_Object, void *);
extern widget_value *xmalloc_widget_value(void);
extern widget_value *digest_single_submenu(int, int, bool);
#endif /*USE_X_TOOLKIT || USE_GTK || HAVE_NTGUI || HAVE_CARBON || HAVE_NS*/

#ifdef HAVE_X_WINDOWS
extern void mouse_position_for_popup(struct frame *f, int *x, int *y);
#endif /* HAVE_X_WINDOWS */

extern Lisp_Object w32_menu_show(struct frame *, int, int, int, int,
				 Lisp_Object, const char **);
extern Lisp_Object ns_menu_show(struct frame *, int, int, bool, bool,
				Lisp_Object, const char **);
extern Lisp_Object xmenu_show(struct frame *, int, int, bool, bool,
			      Lisp_Object, const char **);
extern Lisp_Object tty_menu_show(struct frame *, int, int, bool, bool,
				 Lisp_Object, bool, const char **);
extern ptrdiff_t menu_item_width(const unsigned char *);

/* keep condition the same as when we push away the redundancy warning: */
#if defined(__GNUC__) && defined(__GNUC_MINOR__) && \
    ((__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6)))
# pragma GCC diagnostic pop
#endif /* gcc 4.6+ */

#endif /* MENU_H */

/* EOF */
