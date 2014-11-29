/* The emacs frame widget public header file.
   Copyright (C) 1993, 2001-2014 Free Software Foundation, Inc.

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

/* Emacs 19 face widget ported by Fred Pierresteguy */

#ifndef _EmacsFrame_h
#define _EmacsFrame_h

/* this file contains way too many of these for now: */
#if defined(__GNUC__) && defined(__GNUC_MINOR__) && \
    ((__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6)))
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wredundant-decls"
#endif /* gcc 4.6+ */

#define XtNminibuffer "minibuffer"
#define XtCMinibuffer "Minibuffer"
#define XtNunsplittable "unsplittable"
#define XtCUnsplittable "Unsplittable"
#define XtNinternalBorderWidth "internalBorderWidth"
#define XtCInternalBorderWidth "InternalBorderWidth"
#define XtNinterline "interline"
#define XtCInterline "Interline"

#ifndef XtNfont
# define XtNfont "font"
#endif /* !XtNfont */
#ifndef XtCFont
# define XtCFont "Font"
#endif /* !XtCFont */
#ifndef XtNforeground
# define XtNforeground "foreground"
#endif /* !XtNforeground */
#ifndef XtCForeground
# define XtCForeground "Foreground"
#endif /* !XtCForeground */

#define XtNcursorColor "cursorColor"
#define XtCCursorColor "CursorColor"
#define XtNbarCursor "barCursor"
#define XtCBarCursor "BarCursor"

#define XtNvisualBell "visualBell"
#define XtCVisualBell "VisualBell"
#define XtCBellVolume "BellVolume"
#define XtNbellVolume "bellVolume"

#define XtNpointerBackground "pointerBackground"
#define XtNpointerColor "pointerColor"

#define XtNtextPointer "textPointer"
#define XtNspacePointer "spacePointer"
#define XtNmodeLinePointer "modePointer"
#define XtNgcPointer "gcPointer"

#define XtNemacsFrame "emacsFrame"
#define XtCEmacsFrame "EmacsFrame"

#ifndef XtNgeometry
# define XtNgeometry "geometry"
#endif /* !XtNgeometry */
#ifndef XtCGeometry
# define XtCGeometry "Geometry"
#endif /* !XtCGeometry */
#ifndef XtNshowGrip
# define XtNshowGrip "showGrip"
#endif /* !XtNshowGrip */
#ifndef XtNallowResize
# define XtNallowResize "allowResize"
#endif /* !XtNallowResize */
#ifndef XtNresizeToPreferred
# define XtNresizeToPreferred "resizeToPreferred"
#endif /* !XtNresizeToPreferred */

#define XtNinitialGeometry "initialGeometry"
#define XtCInitialGeometry "InitialGeometry"

/* structures
 */
typedef struct _EmacsFrameRec *EmacsFrame;
typedef struct _EmacsFrameClassRec *EmacsFrameClass;

extern WidgetClass emacsFrameClass;

extern struct _DisplayContext* display_context;

/* Special entry points */
void EmacsFrameSetCharSize (Widget, int, int);
void widget_store_internal_border (Widget widget);
void widget_update_wm_size_hints (Widget widget);

/* keep condition the same as when we push away the redundancy warning: */
#if defined(__GNUC__) && defined(__GNUC_MINOR__) && \
    ((__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6)))
# pragma GCC diagnostic pop
#endif /* gcc 4.6+ */

#endif /* _EmacsFrame_h */
