/* src/macterm.h: Display module for Mac OS.
 * Copyright (C) 2000, 2001, 2002, 2003, 2004,
 *************** 2005, 2006, 2007 Free Software Foundation, Inc.  */
/*
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
the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.  */

/* Contributed by Andrew Choi <akochoi@mac.com>.  */

#ifndef _EMACS_MACTERM_H
#define _EMACS_MACTERM_H 1

#include "macgui.h"
#include "frame.h"
#include "fontset.h"

#ifndef EMACS_KEYBOARD_H
# include "keyboard.h" /* widget_value */
#endif /* !EMACS_KEYBOARD_H */

#if defined(HAVE_AVAILABILITYMACROS_H) && !defined(__AVAILABILITYMACROS__)
# include <AvailabilityMacros.h>
#endif /* HAVE_AVAILABILITYMACROS_H && !__AVAILABILITYMACROS__ */

#ifndef P_
# if defined(PROTOTYPES) || defined(__PROTOTYPES) || defined(__STDC__)
#  define P_(args) args
# else
#  define P_(args) ()
# endif  /* GCC.  */
#endif /* P_ */

#define RGB_TO_ULONG(r, g, b) (((r) << 16) | ((g) << 8) | (b))
#define ARGB_TO_ULONG(a, r, g, b) (((a) << 24) | ((r) << 16) | ((g) << 8) | (b))

#define ALPHA_FROM_ULONG(color) ((color) >> 24)
#ifdef MAC_APPKIT_M
# define RED_FROM_ULONG(color) (((color) >> 16) & 0xff)
#else
# define RED_FROM_ULONG(color) ((color) >> 16)
#endif /* MAC_APPKIT_M */
#define GREEN_FROM_ULONG(color) (((color) >> 8) & 0xff)
#define BLUE_FROM_ULONG(color) ((color) & 0xff)

/* Do not change `* 0x101' in the following lines to `<< 8'.  If
   changed, image masks in 1-bit depth will not work. */
#define RED16_FROM_ULONG(color) (RED_FROM_ULONG(color) * 0x101)
#define GREEN16_FROM_ULONG(color) (GREEN_FROM_ULONG(color) * 0x101)
#define BLUE16_FROM_ULONG(color) (BLUE_FROM_ULONG(color) * 0x101)

#define BLACK_PIX_DEFAULT(f) RGB_TO_ULONG(0,0,0)
#define WHITE_PIX_DEFAULT(f) RGB_TO_ULONG(255,255,255)

#ifndef FONT_WIDTH
# if (defined(HAVE_CARBON) && defined(TARGET_API_MAC_CARBON)) || defined(MAC_OS)
#  define FONT_WIDTH(f)	((f)->max_bounds.width) /* (what was originally in this file) */
# else
#  define FONT_WIDTH(f) ((f)->max_width) /* (definition in "font.h") */
# endif /* (HAVE_CARBON && TARGET_API_MAC_CARBON) || MAC_OS */
#endif /* !FONT_WIDTH */
#ifndef FONT_HEIGHT
# if (defined(HAVE_CARBON) && defined(TARGET_API_MAC_CARBON)) || defined(MAC_OS)
#  define FONT_HEIGHT(f) ((f)->ascent + (f)->descent) /* (what was originally in this file) */
# else
#  define FONT_HEIGHT(f) ((f)->height) /* (definition in "font.h") */
# endif /* (HAVE_CARBON && TARGET_API_MAC_CARBON) || MAC_OS */
#endif /* !FONT_HEIGHT */
#define FONT_BASE(f)    ((f)->ascent)
#define FONT_DESCENT(f) ((f)->descent)

#ifndef __pascal_string
# define __pascal_string(s)						\
   ((const unsigned char *)&(struct {					\
                               unsigned char __len;			\
                               const char __string[__builtin_strlen(s)];\
                             }){ __builtin_strlen(s), s })
#endif /* !__pascal_string */

/* Structure recording bitmaps and reference count.
   If REFCOUNT is 0 then this record is free to be reused.  */
struct mac_bitmap_record
{
  char *bitmap_data;
  char *file;
  int refcount;
  int height, width;
};


/* For each display (currently only one on mac), we have a structure that
   records information about it.  */
struct mac_display_info
{
  /* Chain of all mac_display_info structures: */
  struct mac_display_info *next;

#ifdef MAC_APPKIT_M
  /* The generic display parameters corresponding to this display. */
  struct terminal *terminal;
#endif /* MAC_APPKIT_M */

  /* This is a cons cell of the form (NAME . FONT-LIST-CACHE).
     The same cons cell also appears in x_display_name_list.  */
  Lisp_Object name_list_element;

  /* Number of frames that are on this display: */
  int reference_count;

  /* Dots per inch of the screen: */
  double resx, resy;

  /* Number of planes on this screen: */
  int n_planes;

  /* Whether the screen supports color */
  int color_p;

  /* Dimensions of this screen.  */
  int height, width;

  /* Mask of things that cause the mouse to be grabbed.  */
  int grabbed;

#if 0
  /* Emacs bitmap-id of the default icon bitmap for this frame.
     Or -1 if none has been allocated yet.  */
# ifdef MAC_APPKIT_M
  ptrdiff_t icon_bitmap_id;
# else
  int icon_bitmap_id;
# endif /* MAC_APPKIT_M */
#endif /* 0 */

  /* The root window of this screen: */
  Window root_window;

  /* The cursor to use for vertical scroll bars: */
  Cursor vertical_scroll_bar_cursor;

  /* Resource data base: */
  XrmDatabase xrdb;

  /* A table of all the fonts we have already loaded: */
  struct font_info *font_table;

  /* The current capacity of font_table.  */
  int font_table_size;

  /* Minimum width over all characters in all fonts in font_table.  */
  int smallest_char_width;

  /* Minimum font height over all fonts in font_table.  */
  int smallest_font_height;

  /* Reusable Graphics Context for drawing a cursor in a non-default face. */
  GC scratch_cursor_gc;

  /* These variables describe the range of text currently shown in its
     mouse-face, together with the window they apply to.  As long as
     the mouse stays within this range, we need not redraw anything on
     its account.  Rows and columns are glyph matrix positions in
     MOUSE_FACE_WINDOW.  */
  int mouse_face_beg_row, mouse_face_beg_col;
  int mouse_face_beg_x, mouse_face_beg_y;
  int mouse_face_end_row, mouse_face_end_col;
  int mouse_face_end_x, mouse_face_end_y;
  int mouse_face_past_end;
  Lisp_Object mouse_face_window;
  int mouse_face_face_id;
  Lisp_Object mouse_face_overlay;

  /* 1 if a mouse motion event came and we did NOT handle it right away
   * because gc was in progress: */
  int mouse_face_deferred_gc;

  /* FRAME and X, Y position of mouse when last checked for
     highlighting.  X and Y can be negative or out of range for the frame.  */
  struct frame *mouse_face_mouse_frame;
  int mouse_face_mouse_x, mouse_face_mouse_y;

  /* Nonzero means defer mouse-motion highlighting.  */
  int mouse_face_defer;

  /* Nonzero means that the mouse highlight should not be shown.  */
  int mouse_face_hidden;

  int mouse_face_image_state;

#ifdef MAC_APPKIT_M
  /* Information about the range of text currently shown in
   * mouse-face: */
  Mouse_HLInfo mouse_highlight;
#endif /* MAC_APPKIT_M */

  /* Default name for all frames on this display: */
  char *mac_id_name;

  /* The number of fonts actually stored in the font table.
     font_table[n] is used and valid iff 0 <= n < n_fonts.  0 <=
     n_fonts <= font_table_size and font_table[i].name != 0.  */
  int n_fonts;

  /* Pointer to bitmap records: */
  struct mac_bitmap_record *bitmaps;

  /* Allocated size of bitmaps field: */
#ifdef MAC_APPKIT_M
  ptrdiff_t bitmaps_size;
#else
  int bitmaps_size;
#endif /* MAC_APPKIT_M */

  /* Last used bitmap index: */
#ifdef MAC_APPKIT_M
  ptrdiff_t bitmaps_last;
#else
  int bitmaps_last;
#endif /* MAC_APPKIT_M */

  /* The frame (if any) which has the window that has keyboard focus.
     Zero if none.  This is examined by Ffocus_frame in macfns.c.  Note
     that a mere EnterNotify event can set this; if you need to know the
     last frame specified in a FocusIn or FocusOut event, use
     x_focus_event_frame.  */
  struct frame *x_focus_frame;

  /* The last frame mentioned in a FocusIn or FocusOut event.  This is
     separate from x_focus_frame, because whether or not LeaveNotify
     events cause us to lose focus depends on whether or not we have
     received a FocusIn event for it.  */
  struct frame *x_focus_event_frame;

  /* The frame which currently has the visual highlight, and should get
     keyboard input (other sorts of input have the frame encoded in the
     event).  It points to the focus frame's selected window's
     frame.  It differs from x_focus_frame when we are using a global
     minibuffer.  */
  struct frame *x_highlight_frame;

#ifdef MAC_APPKIT_M
  /* The frame waiting to be auto-raised in XTread_socket: */
  struct frame *x_pending_autoraise_frame;

  /* The frame where the mouse was last time we reported a ButtonPress event: */
  struct frame *last_mouse_frame;

  /* The frame where the mouse was last time we reported a mouse position: */
  struct frame *last_mouse_glyph_frame;

  /* Where the mouse was last time we reported a mouse position.
   * This is a rectangle on last_mouse_glyph_frame: */
  NativeRectangle last_mouse_glyph;

  /* Time of last mouse movement on this display.  This is a hack because
   * we would really prefer that XTmouse_position would return the time
   * associated with the position it returns, but there does NOT seem to be
   * any way to wrest the timestamp from the server along with the position
   * query.  So, we just keep track of the time of the last movement we
   * received, and return that in hopes that it is somewhat accurate: */
  Time last_mouse_movement_time;

  /* This is a button event that wants to activate the menubar.
   * We save it here until the command loop gets to think about it: */
  EventRef saved_menu_event;
#else
  /* Cache of images: */
  struct image_cache *image_cache;
#endif /* MAC_APPKIT_M */
};

/* This checks to make sure we have a display.  */
extern void check_mac P_((void));

#define x_display_info mac_display_info

/* This is a chain of structures for all the X displays currently in use.  */
extern struct x_display_info *x_display_list;

/* This is a chain of structures for all the displays currently in use.  */
extern struct mac_display_info one_mac_display_info;

/* This is a list of cons cells, each of the form (NAME . FONT-LIST-CACHE),
   one for each element of x_display_list and in the same order.
   NAME is the name of the frame.
   FONT-LIST-CACHE records previous values returned by x-list-fonts.  */
extern Lisp_Object x_display_name_list;

extern struct x_display_info *x_display_info_for_name P_((Lisp_Object));

extern struct mac_display_info *mac_term_init P_((Lisp_Object, char *, char *));

extern Lisp_Object x_list_fonts P_((struct frame *, Lisp_Object, int, int));
extern struct font_info *x_get_font_info P_((struct frame *f, int));
extern struct font_info *x_load_font P_((struct frame *, char *, int));
extern struct font_info *x_query_font P_((struct frame *, char *));
extern void x_find_ccl_program P_((struct font_info *));

#ifndef _STRUCT_X_OUTPUT_DECLARED
# define _STRUCT_X_OUTPUT_DECLARED 1
/* When Emacs uses a tty window, tty_display in frame.c points to an
   x_output struct .  */
struct x_output
{
  unsigned long background_pixel;
  unsigned long foreground_pixel;
};
#endif /* !_STRUCT_X_OUTPUT_DECLARED */

/* The collection of data describing a window on the Mac.  */
struct mac_output
{
  /* Placeholder for things accessed through output_data.x.  Must
     appear first.  */
  struct x_output x_compatible;

  /* Menubar "widget" handle: */
#ifdef MAC_APPKIT_M
  /* this should actually be located further down in this struct in this
   * case: */
  bool_bf menubar_widget : 1;
#else
  int menubar_widget;
#endif /* MAC_APPKIT_M */

  FRAME_PTR mFP;		/* points back to the frame struct */

  /* Here are the Graphics Contexts for the default font.  */
  GC normal_gc;				/* Normal video */
  GC reverse_gc;			/* Reverse video */
  GC cursor_gc;				/* cursor drawing */

  /* The window used for this frame.
     May be zero while the frame object is being created
     and the window has not yet been created.  */
  Window window_desc;

  /* The window that is the parent of this window.
     Usually this is a window that was made by the window manager,
     but it can be the root window, and it can be explicitly specified
     (see the explicit_parent field, below).  */
  Window parent_desc;

  /* Default ASCII font of this frame: */
#ifdef MAC_APPKIT_M
  struct font *font;
#else
  XFontStruct *font;
#endif /* MAC_APPKIT_M */

  /* The baseline offset of the default ASCII font: */
  int baseline_offset;

  /* If a fontset is specified for this frame instead of font, this
     value contains an ID of the fontset, else -1.  */
  int fontset;

  /* Pixel values used for various purposes.
     border_pixel may be -1 meaning use a gray tile.  */
  unsigned long cursor_pixel;
  unsigned long border_pixel;
  unsigned long mouse_pixel;
  unsigned long cursor_foreground_pixel;

#if 0
  /* Foreground color for scroll bars.  A value of -1 means use the
     default (black for non-toolkit scroll bars).  */
  unsigned long scroll_bar_foreground_pixel;

  /* Background color for scroll bars.  A value of -1 means use the
     default (background color of the frame for non-toolkit scroll
     bars).  */
  unsigned long scroll_bar_background_pixel;
#endif /* 0 */

  /* Descriptor for the cursor in use for this window.  */
  Cursor text_cursor;
  Cursor nontext_cursor;
  Cursor modeline_cursor;
  Cursor hand_cursor;
  Cursor hourglass_cursor;
  Cursor horizontal_drag_cursor;
#ifdef MAC_APPKIT_M
  Cursor vertical_drag_cursor;
  Cursor current_cursor;	/* unretained */
#endif /* MAC_APPKIT_M */
#if 0
  /* Window whose cursor is hourglass_cursor.  This window is temporarily
     mapped to display a hourglass-cursor.  */
  Window hourglass_window;

  /* Non-zero means hourglass cursor is currently displayed.  */
  unsigned hourglass_p : 1;

  /* Flag to set when the window needs to be completely repainted.  */
  int needs_exposure;
#endif /* 0 */

#if TARGET_API_MAC_CARBON
  /* The Mac control reference for the hourglass (progress indicator)
     shown at the upper-right corner of the window.  */
  ControlRef hourglass_control;
#endif /* TARGET_API_MAC_CARBON */

  /* This is the Emacs structure for the display this frame is on.  */
  /* struct w32_display_info *display_info; */

  /* Nonzero (i.e. "True") means our parent is another application's window
     and was explicitly specified.  */
#ifdef MAC_APPKIT_M
  bool_bf explicit_parent : 1;
#else
  char explicit_parent;
#endif /* MAC_APPKIT_M */

  /* Nonzero ("True") means tried already to make this frame visible: */
#ifdef MAC_APPKIT_M
  bool_bf asked_for_visible : 1;
#else
  char asked_for_visible;
#endif /* MAC_APPKIT_M */

#ifdef MAC_APPKIT_M
  /* True means this frame is for tooltip: */
  bool_bf tooltip_p : 1;

  /* True means x_check_fullscreen is not called yet after fullscreen
   * request for this frame: */
  bool_bf check_fullscreen_needed_p : 1;

  /* True means this frame uses a native tool bar (as opposed to a
   * toolkit one): */
  bool_bf native_tool_bar_p : 1;

  /* True means background alpha value is enabled for this frame: */
  bool_bf background_alpha_enabled_p : 1;

  /* True means synthetic bold workaround is disabled for this frame: */
  bool_bf synthetic_bold_workaround_disabled_p : 1;

  /* Backing scale factor (1 or 2), used for rendering images: */
  unsigned backing_scale_factor : 2;

  /* State for image vs. backing scaling factor mismatch detection: */
  unsigned scale_mismatch_state : 2;

  /* This variable records the gravity value of the window position if the
   * window has an external tool bar when it is created.  The position of
   * the window is adjusted using this information when the tool bar is
   * first redisplayed.  Once the tool bar is redisplayed, it is set to 0
   * in order to avoid further adjustment: */
  unsigned toolbar_win_gravity : 4;
#endif /* MAC_APPKIT_M */

  /* Relief GCs, colors etc.  */
  struct relief
  {
    GC gc;
    unsigned long pixel;
    int allocated_p;
  }
  black_relief, white_relief;

  /* The background for which the above relief GCs were set up.
     They are changed only when a different background is involved.  */
  unsigned long relief_background;

  /* Width of the internal border: */
  int internal_border_width;

  /* Hints for the size and the position of a window.  */
  XSizeHints *size_hints;

#if USE_CG_DRAWING
  /* Quartz 2D graphics context.  */
  CGContextRef cg_context;
#endif /* USE_CG_DRAWING */
};

typedef struct mac_output mac_output;

/* Return the X output data for frame F.  */
#define FRAME_X_OUTPUT(f) ((f)->output_data.mac)

/* Return the Mac window used for displaying data in frame F.  */
#define FRAME_MAC_WINDOW(f) ((f)->output_data.mac->window_desc)
#define FRAME_X_WINDOW(f) ((f)->output_data.mac->window_desc)

#ifndef FRAME_FOREGROUND_PIXEL
# ifdef MAC_OS
#  define FRAME_FOREGROUND_PIXEL(f) ((f)->output_data.x->foreground_pixel)
# else
#  define FRAME_FOREGROUND_PIXEL(f) ((f)->foreground_pixel) /* (definition from "frame.h") */
# endif /* MAC_OS */
#endif /* !FRAME_FOREGROUND_PIXEL */
#ifndef FRAME_BACKGROUND_PIXEL
# ifdef MAC_OS
#  define FRAME_BACKGROUND_PIXEL(f) ((f)->output_data.x->background_pixel)
# else
#  define FRAME_BACKGROUND_PIXEL(f) ((f)->background_pixel) /* (definition from "frame.h") */
# endif /* MAC_OS */
#endif /* !FRAME_BACKGROUND_PIXEL */

#define FRAME_FONT(f) ((f)->output_data.mac->font)
#define FRAME_FONTSET(f) ((f)->output_data.mac->fontset)

#define FRAME_BASELINE_OFFSET(f) ((f)->output_data.mac->baseline_offset)

#define FRAME_SIZE_HINTS(f) ((f)->output_data.mac->size_hints)

#define FRAME_TOOLTIP_P(f) ((f)->output_data.mac->tooltip_p)

#define FRAME_CHECK_FULLSCREEN_NEEDED_P(f) \
  ((f)->output_data.mac->check_fullscreen_needed_p)
#define FRAME_NATIVE_TOOL_BAR_P(f) ((f)->output_data.mac->native_tool_bar_p)
#define FRAME_BACKGROUND_ALPHA_ENABLED_P(f) \
  ((f)->output_data.mac->background_alpha_enabled_p)
#define FRAME_SYNTHETIC_BOLD_WORKAROUND_DISABLED_P(f) \
  ((f)->output_data.mac->synthetic_bold_workaround_disabled_p)
#define FRAME_BACKING_SCALE_FACTOR(f)		\
  ((f)->output_data.mac->backing_scale_factor)
#define FRAME_SCALE_MISMATCH_STATE(f) \
  ((f)->output_data.mac->scale_mismatch_state)

/* This gives the mac_display_info structure for the display F is on.  */
#define FRAME_MAC_DISPLAY_INFO(f) (&one_mac_display_info)
#define FRAME_X_DISPLAY_INFO(f) (&one_mac_display_info)
#ifndef FRAME_DISPLAY_INFO
# define FRAME_DISPLAY_INFO(f) (&one_mac_display_info)
#endif /* !FRAME_DISPLAY_INFO */

/* This is the `Display *' which frame F is on.  */
#define FRAME_MAC_DISPLAY(f) (0)
#define FRAME_X_DISPLAY(f) (0)

/* This is the 'font_info *' which frame F has.  */
#define FRAME_MAC_FONT_TABLE(f) (FRAME_MAC_DISPLAY_INFO (f)->font_table)

/* Value is the smallest width of any character in any font on frame F.  */
#ifndef FRAME_SMALLEST_CHAR_WIDTH
# define FRAME_SMALLEST_CHAR_WIDTH(F) \
      FRAME_MAC_DISPLAY_INFO (F)->smallest_char_width
#endif /* !FRAME_SMALLEST_CHAR_WIDTH */

/* Value is the smallest height of any font on frame F.  */
#ifndef FRAME_SMALLEST_FONT_HEIGHT
# define FRAME_SMALLEST_FONT_HEIGHT(F) \
      FRAME_MAC_DISPLAY_INFO (F)->smallest_font_height
#endif /* !FRAME_SMALLEST_FONT_HEIGHT */

/* Return a pointer to the image cache of frame F.  */

#define FRAME_X_IMAGE_CACHE(F) FRAME_MAC_DISPLAY_INFO ((F))->image_cache


/* Mac-specific scroll bar stuff.  */

/* We represent scroll bars as lisp vectors.  This allows us to place
   references to them in windows without worrying about whether we will
   end up with windows referring to dead scroll bars; the garbage
   collector will free it when its time comes.

   We use struct scroll_bar as a template for accessing fields of the
   vector.  */

struct scroll_bar
{
  /* These fields are shared by all vectors: */
#ifdef MAC_APPKIT_M
  struct vectorlike_header header;
#else
  EMACS_INT size_from_Lisp_Vector_struct;
  struct Lisp_Vector *next_from_Lisp_Vector_struct;
#endif /* MAC_APPKIT_M */

  /* The window for which we are a scroll bar: */
  Lisp_Object window;

  /* The next and previous in the chain of scroll bars in this frame.  */
  Lisp_Object next, prev;

  /* The Mac control handle of this scroll bar.  Since this is a
     pointer value, we store it split into two Lisp integers.  */
  Lisp_Object control_handle_low, control_handle_high;

  /* Fields from `mac_control_ref' down will not be traced by the GC.  */

  /* The Mac control reference of this scroll bar: */
  void *mac_control_ref;

  /* The position and size of the scroll bar in pixels, relative to the
     frame.  */
#ifdef MAC_APPKIT_M
  int top, left, width, height;
#else
  Lisp_Object top, left, width, height;
#endif /* MAC_APPKIT_M */

  /* The starting and ending positions of the handle, relative to the
     handle area (i.e. zero is the top position, not
     SCROLL_BAR_TOP_BORDER).  If they are equal, that means the handle
     has NOT been drawn yet.

     These are not actually the locations where the beginning and end
     are drawn; in order to keep handles from becoming invisible when
     editing large files, we establish a minimum height by always
     drawing handle bottoms VERTICAL_SCROLL_BAR_MIN_HANDLE pixels below
     where they would be normally; the bottom and top are in a
     different co-ordinate system.  */
  Lisp_Object start, end;

  /* If the scroll bar handle is currently being dragged by the user,
     this is the number of pixels from the top of the handle to the
     place where the user grabbed it.  If the handle is pressed but
     not dragged yet, this is a negative integer whose absolute value
     is the number of pixels plus 1.  If the handle is NOT currently
     being dragged, this is Qnil.  */
  Lisp_Object dragging;

#ifdef USE_TOOLKIT_SCROLL_BARS
  /* The position and size of the scroll bar handle track area in
     pixels, relative to the frame.  */
  Lisp_Object track_top, track_height;

  /* Minimum length of the scroll bar handle, in pixels.  */
  Lisp_Object min_handle;
#endif /* USE_TOOLKIT_SCROLL_BARS */

#ifdef MAC_APPKIT_M
  /* True if redraw needed in the next XTset_vertical_scroll_bar call: */
  bool_bf redraw_needed_p : 1;
#endif /* MAC_APPKIT_M */
};

/* The number of elements a vector holding a struct scroll_bar needs: */
#define SCROLL_BAR_VEC_SIZE					\
  ((sizeof(struct scroll_bar)					\
    - sizeof(EMACS_INT) - sizeof(struct Lisp_Vector *)) 	\
   / sizeof(Lisp_Object))

/* Turning a lisp vector value into a pointer to a struct scroll_bar: */
#define XSCROLL_BAR(vec) ((struct scroll_bar *)XVECTOR(vec))


/* Extract the reference to the scroller control from a struct
 * scroll_bar: */
#define SCROLL_BAR_SCROLLER(ptr) ((__bridge EmacsScroller *)(ptr)->mac_control_ref)

/* Store the reference to a scroller control in a struct
 * scroll_bar: */
#define SET_SCROLL_BAR_SCROLLER(ptr, ref) ((ptr)->mac_control_ref = (__bridge void *)(ref))

/* Building a C long integer from two lisp integers: */
#define SCROLL_BAR_PACK(low, high) (XINT(high) << 16 | XINT(low))

/* Setting two lisp integers to two parts of a C unsigned long: */
#define SCROLL_BAR_UNPACK(low, high, ulong) \
  (XSETINT((low),  (ulong) & 0xffff), \
   XSETINT((high), (ulong) >> 16))

/* Extract the Mac control handle of the scroll bar from a struct
 * scroll_bar. */
#define SCROLL_BAR_CONTROL_REF(ptr) \
 ((ControlRef)SCROLL_BAR_PACK((ptr)->control_handle_low, \
			      (ptr)->control_handle_high))

/* Store a Mac control handle in a struct scroll_bar. */
#define SET_SCROLL_BAR_CONTROL_REF(ptr, ref) \
 (SCROLL_BAR_UNPACK((ptr)->control_handle_low, \
		    (ptr)->control_handle_high, (unsigned long)(ref)))


/* Extract the Mac control handle of the scroll bar from a struct
   scroll_bar.  */
#define SCROLL_BAR_CONTROL_HANDLE(ptr) \
  ((ControlHandle)SCROLL_BAR_PACK((ptr)->control_handle_low, \
                                  (ptr)->control_handle_high))

/* Store a Mac control handle in a struct scroll_bar.  */
#define SET_SCROLL_BAR_CONTROL_HANDLE(ptr, handle) \
  (SCROLL_BAR_UNPACK((ptr)->control_handle_low, \
                     (ptr)->control_handle_high, (unsigned long)(handle)))

/* Return the inside width of a vertical scroll bar, given the outside
   width.  */
#define VERTICAL_SCROLL_BAR_INSIDE_WIDTH(f, width) \
  ((width) \
   - VERTICAL_SCROLL_BAR_LEFT_BORDER \
   - VERTICAL_SCROLL_BAR_RIGHT_BORDER \
   - VERTICAL_SCROLL_BAR_WIDTH_TRIM * 2)

/* Return the length of the rectangle within which the top of the
   handle must stay.  This is NOT equivalent to the inside height,
   because the scroll bar handle has a minimum height.

   This is the real range of motion for the scroll bar, so when we are
   scaling buffer positions to scroll bar positions, we use this, not
   VERTICAL_SCROLL_BAR_INSIDE_HEIGHT.  */
#define VERTICAL_SCROLL_BAR_TOP_RANGE(f,height) \
  (VERTICAL_SCROLL_BAR_INSIDE_HEIGHT(f, height) \
   - VERTICAL_SCROLL_BAR_MIN_HANDLE - UP_AND_DOWN_ARROWS)

/* Return the inside height of vertical scroll bar, given the outside
   height.  See VERTICAL_SCROLL_BAR_TOP_RANGE too.  */
#define VERTICAL_SCROLL_BAR_INSIDE_HEIGHT(f,height) \
  ((height) - VERTICAL_SCROLL_BAR_TOP_BORDER \
   - VERTICAL_SCROLL_BAR_BOTTOM_BORDER)


/* Border widths for scroll bars.

   Scroll bar windows do NOT have any borders; their border width is
   set to zero, and we redraw borders ourselves.  This makes the code
   a bit cleaner, since we do NOT have to convert between outside width
   (used when relating to the rest of the screen) and inside width
   (used when sizing and drawing the scroll bar window itself).

   The handle moves up and down/back and forth in a rectangle inset
   from the edges of the scroll bar.  These are widths by which we
   inset the handle boundaries from the scroll bar edges.  */
#define VERTICAL_SCROLL_BAR_LEFT_BORDER (0)
#define VERTICAL_SCROLL_BAR_RIGHT_BORDER (0)
#define VERTICAL_SCROLL_BAR_TOP_BORDER (0)
#define VERTICAL_SCROLL_BAR_BOTTOM_BORDER (0)

/* Minimum lengths for scroll bar handles, in pixels.  */
#define VERTICAL_SCROLL_BAR_MIN_HANDLE (16)

/* Combined length of up and down arrow boxes in scroll bars, in pixels.  */
#define UP_AND_DOWN_ARROWS (32)

/* Trimming off a few pixels from each side prevents
   text from glomming up against the scroll bar */
#define VERTICAL_SCROLL_BAR_WIDTH_TRIM (0)

/* Variations of possible Aqua scroll bar width.  */
#define MAC_AQUA_VERTICAL_SCROLL_BAR_WIDTH (15)
#define MAC_AQUA_SMALL_VERTICAL_SCROLL_BAR_WIDTH (11)

/* Size of hourglass controls: */
#ifdef MAC_APPKIT_M
# define HOURGLASS_WIDTH (18)
# define HOURGLASS_HEIGHT (18)
#else
# define HOURGLASS_WIDTH (16)
# define HOURGLASS_HEIGHT (16)
#endif /* MAC_APPKIT_M */
#define HOURGLASS_TOP_MARGIN (2)
#define HOURGLASS_RIGHT_MARGIN (32)

#if defined(__GNUC__) && defined(__GNUC_MINOR__)
# if (defined(__APPLE__) && defined(__APPLE_CC__) && (__APPLE_CC__ > 1))
#  if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6))
#   pragma GCC diagnostic push
#  endif /* GCC 4.6 (version in which push/pop was introduced) */
#  pragma GCC diagnostic ignored "-Wfour-char-constants"
# endif /* Apple GCC (not sure the exact one) */
# if (__GNUC__ >= 2)
#  if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6))
#   pragma GCC diagnostic push
#  endif /* GCC 4.6 (version in which push/pop was introduced) */
#  pragma GCC diagnostic ignored "-Wmultichar"
# endif /* GCC 2+ (not sure the exact one) */
#endif /* gcc */

/* Some constants that are used locally.  */
/* Creator code for Emacs on Mac OS.  */
enum {
  MAC_EMACS_CREATOR_CODE	= 'EMAx'
};

/* Apple event descriptor types: */
enum {
  TYPE_FILE_NAME		= 'fNam'
};

/* Keywords for Apple event attributes: */
enum {
  KEY_EMACS_SUSPENSION_ID_ATTR	= 'esId' /* typeUInt32 */
};

/* Carbon event parameter names: */
enum {
  EVENT_PARAM_TEXT_INPUT_SEQUENCE_NUMBER = 'tsSn' /* typeUInt32 */
};

/* Some constants that are not defined in older versions.  */
#if MAC_OS_X_VERSION_MAX_ALLOWED < 1030
/* Keywords for Apple event attributes */
enum {
  keyReplyRequestedAttr		= 'repq'
};
#endif /* older than 10.3 */

#if MAC_OS_X_VERSION_MAX_ALLOWED < 1040
/* Gestalt selectors */
enum {
  gestaltSystemVersionMajor	= 'sys1',
  gestaltSystemVersionMinor	= 'sys2',
  gestaltSystemVersionBugFix	= 'sys3'
};
#endif /* older than 10.4 */

/* kCGBitmapByteOrder32Host is defined in Universal SDK for 10.4 but
 * not in PPC SDK for 10.4.0: */
#if (MAC_OS_X_VERSION_MAX_ALLOWED < 1050) && !defined(kCGBitmapByteOrder32Host)
# define kCGBitmapByteOrder32Host 0
#endif /* older than 10.5 && !kCGBitmapByteOrder32Host */

#ifndef _Block_H_
# if !defined(BLOCK_EXPORT)
#  if defined(__cplusplus)
#   define BLOCK_EXPORT extern "C"
#  else
#   define BLOCK_EXPORT extern
#  endif /* __cplusplus */
# endif /* !BLOCK_EXPORT */
#endif /* !_Block_H_ */

#if (MAC_OS_X_VERSION_MAX_ALLOWED == 1060) && defined(BLOCK_EXPORT) && \
    defined(AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER)
BLOCK_EXPORT void _Block_object_assign(void *, const void *, const int) AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;
BLOCK_EXPORT void _Block_object_dispose(const void *, const int) AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;
BLOCK_EXPORT void *_NSConcreteGlobalBlock[32] AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;
BLOCK_EXPORT void *_NSConcreteStackBlock[32] AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;
#endif /* 10.6+ && BLOCK_EXPORT */

#ifdef MAC_OSX
# if MAC_OS_X_VERSION_MAX_ALLOWED < 1020
/* Apple event descriptor types */
enum {
  typeUTF8Text			= 'utf8'
};

/* Carbon event parameter names */
enum {
  kEventParamWindowMouseLocation = 'wmou'
};
# endif /* older than 10.2 */
#endif /* MAC_OSX */

#if defined(__GNUC__) && defined(__GNUC_MINOR__)
# if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6))
#  pragma GCC diagnostic pop
# endif /* GCC 4.6 (version in which push/pop was introduced) */
#endif /* gcc */

struct frame;
struct face;
struct image;

Lisp_Object display_x_get_resource P_((struct x_display_info *,
                                       Lisp_Object, Lisp_Object,
                                       Lisp_Object, Lisp_Object));
struct frame *check_x_frame P_((Lisp_Object));
EXFUN(Fx_display_color_p, 1);
EXFUN(Fx_display_grayscale_p, 1);
EXFUN(Fx_display_planes, 1);
extern void x_free_gcs P_((struct frame *));
extern int XParseGeometry P_((char *, int *, int *, unsigned int *,
			      unsigned int *));

/* Defined in macterm.c.  */
extern bool mac_screen_config_changed;
extern CGColorSpaceRef mac_cg_color_space_rgb;
extern Lisp_Object Qpanel_closed, Qselection;
extern Lisp_Object Qtext_input, Qinsert_text, Qset_marked_text;
extern Lisp_Object Qaction, Qmac_action_key_paths;
extern Lisp_Object Qaccessibility;
extern Lisp_Object Qservice, Qpaste, Qperform;
extern void x_set_window_size P_((struct frame *, int, int, int, bool));
extern void x_set_mouse_position P_((struct frame *, int, int));
extern void x_set_mouse_pixel_position P_((struct frame *, int, int));
extern void x_raise_frame P_((struct frame *));
extern void x_lower_frame P_((struct frame *));
extern void x_delete_terminal P_((struct terminal *terminal));
#define x_display_pixel_height(dpyinfo)	((dpyinfo)->height)
#define x_display_pixel_width(dpyinfo)	((dpyinfo)->width)
extern void x_make_frame_visible P_((struct frame *));
extern void x_make_frame_invisible P_((struct frame *));
extern void x_iconify_frame P_((struct frame *));
extern void x_free_frame_resources P_((struct frame *));
extern void x_destroy_window P_((struct frame *));
extern void x_wm_set_size_hint P_((struct frame *, long, bool));
extern void x_wm_set_icon_position P_((struct frame *f,
                                       int icon_x, int icon_y));
extern void x_delete_display P_((struct x_display_info *));
extern void mac_initialize P_((void));
extern Pixmap XCreatePixmap P_((Display *, WindowPtr, unsigned int,
                                unsigned int, unsigned int));
extern Pixmap XCreatePixmapFromBitmapData P_((Display *, WindowPtr, char *,
					      unsigned int, unsigned int,
					      unsigned long, unsigned long,
					      unsigned int));
extern void XFreePixmap P_((Display *, Pixmap));
extern GC XCreateGC P_((Display *, void *, unsigned long, XGCValues *));
extern void XFreeGC P_((Display *, GC));
#define XGetGCValues(display, gc, mask, xgcv)	\
  mac_get_gc_values(gc, mask, xgcv)
extern void XSetForeground P_((Display *, GC, unsigned long));
extern void XSetBackground P_((Display *, GC, unsigned long));
extern void XSetWindowBackground P_((Display *, WindowPtr, unsigned long));
extern void XDrawLine P_((Display *, Pixmap, GC, int, int, int, int));
extern void x_set_sticky (struct frame *, Lisp_Object, Lisp_Object);
#if (MAC_OS_X_VERSION_MAX_ALLOWED < 1050) || (MAC_OS_X_VERSION_MIN_REQUIRED < 1050)
extern CMProfileRef mac_open_srgb_profile (void);
#endif /* pre-10.5 */
extern void mac_begin_scale_mismatch_detection (struct frame *);
extern bool mac_end_scale_mismatch_detection (struct frame *);
extern void mac_draw_line_to_pixmap (Pixmap, GC, int, int, int, int);
extern void mac_clear_area P_((struct frame *, int, int,
                               unsigned int, unsigned int));
extern Pixmap mac_create_pixmap (unsigned int, unsigned int, unsigned int);
extern Pixmap mac_create_pixmap_from_bitmap_data (char *,
						  unsigned int, unsigned int,
						  unsigned long, unsigned long,
						  unsigned int);
extern void mac_free_pixmap (Pixmap);
extern GC mac_create_gc (unsigned long, XGCValues *);
#if DRAWING_USE_GCD
extern GC mac_duplicate_gc (GC);
#endif
extern void mac_free_gc (GC);
extern void mac_set_foreground (GC, unsigned long);
extern void mac_set_background (GC, unsigned long);
extern void mac_focus_changed (int, struct mac_display_info *,
			       struct frame *, struct input_event *);
extern struct frame *mac_focus_frame (struct mac_display_info *);
extern void mac_move_window_to_gravity_reference_point (struct frame *,
							int, short, short);
extern void mac_get_window_gravity_reference_point (struct frame *, int,
						    short *, short *);
extern void mac_handle_origin_change (struct frame *);
extern void mac_handle_size_change (struct frame *, int, int);
extern void mac_handle_visibility_change (struct frame *);
extern UInt32 mac_cgevent_flags_to_modifiers (CGEventFlags);
extern CGEventFlags mac_cgevent_to_input_event (CGEventRef,
						struct input_event *);
extern CGRect mac_get_first_rect_for_range (struct window *, const CFRange *,
					    CFRange *);
extern void mac_ax_selected_text_range (struct frame *, CFRange *);
extern EMACS_INT mac_ax_number_of_characters (struct frame *);
extern void mac_ax_visible_character_range (struct frame *, CFRange *);
extern EMACS_INT mac_ax_line_for_index (struct frame *, EMACS_INT);
extern int mac_ax_range_for_line (struct frame *, EMACS_INT, CFRange *);
extern CFStringRef mac_ax_create_string_for_range (struct frame *,
						   const CFRange *, CFRange *);
extern bool mac_keydown_cgevent_quit_p (CGEventRef);
extern void mac_store_apple_event (Lisp_Object, Lisp_Object, const AEDesc *);
extern OSStatus mac_store_event_ref_as_apple_event (AEEventClass, AEEventID,
						    Lisp_Object, Lisp_Object,
						    EventRef, UInt32,
						    const EventParamName *,
						    const EventParamType *);
extern void mac_unload_font P_((struct mac_display_info *, XFontStruct *));
#ifdef MAC_APPKIT_M
extern bool mac_font_panel_visible_p (void);
#else
extern int mac_font_panel_visible_p P_((void));
#endif /* MAC_APPKIT_M */
extern OSStatus mac_show_hide_font_panel P_((void));
#ifdef MAC_APPKIT_M
extern OSStatus mac_set_font_info_for_selection (struct frame *, int, int, int,
						 Lisp_Object);
#else
extern OSStatus mac_set_font_info_for_selection P_((struct frame *, int, int));
#endif /* MAC_APPKIT_M */
extern OSStatus install_window_handler P_((WindowPtr));
extern void remove_window_handler P_((WindowPtr));
extern OSStatus mac_post_mouse_moved_event P_((void));
#if !TARGET_API_MAC_CARBON
extern void do_apple_menu P_((SInt16));
#endif /* !TARGET_API_MAC_CARBON */
#if USE_CG_DRAWING
extern void mac_prepare_for_quickdraw P_((struct frame *));
#endif /* USE_CG_DRAWING */
extern int mac_quit_char_key_p P_((UInt32, UInt32));
#if !defined(EMACS_FRAME_H) || defined(COMING_FROM_MACTERM_C)
Lisp_Object x_new_font P_((struct frame *, register const char *));
#endif /* !EMACS_FRAME_H || COMING_FROM_MACTERM_C */
Lisp_Object x_new_fontset P_((struct frame *, const char *));
extern void mac_get_window_bounds P_((struct frame *, Rect *, Rect *));
extern void mac_initialize P_((void));
extern void syms_of_macterm P_((void));

#define FONT_TYPE_FOR_UNIBYTE(font, ch) 0
#define FONT_TYPE_FOR_MULTIBYTE(font, ch) 0

/* Defined in macselect.c */
extern Lisp_Object Qmac_pasteboard_name, Qmac_pasteboard_data_type;
extern Lisp_Object Qmac_apple_event_class, Qmac_apple_event_id;
extern Lisp_Object QCactions, Qcopy, Qlink, Qgeneric, Qprivate, Qmove, Qdelete;
extern void x_clear_frame_selections P_((struct frame *));
extern Lisp_Object mac_find_apple_event_spec P_((AEEventClass, AEEventID,
                                                 Lisp_Object *,
                                                 Lisp_Object *,
                                                 Lisp_Object *));
extern pascal OSErr mac_handle_apple_event P_((const AppleEvent *, AppleEvent *,
                                               SInt32));
extern void cleanup_all_suspended_apple_events P_((void));
extern void init_apple_event_handler P_((void));
extern OSErr install_drag_handler P_((WindowRef));
extern void remove_drag_handler P_((WindowRef));

/* Defined in macfns.c */
extern Lisp_Object Qbacking_scale_factor;
extern Lisp_Object QCdirection, QCduration;
extern Lisp_Object Qfade_in, Qmove_in;
extern Lisp_Object Qbars_swipe, Qcopy_machine, Qdissolve, Qflash, Qmod;
extern Lisp_Object Qpage_curl, Qpage_curl_with_shadow, Qripple, Qswipe;
extern int have_menus_p P_((void));

extern void x_real_positions P_((struct frame *, int *, int *));
extern void x_set_menu_bar_lines P_((struct frame *, Lisp_Object, Lisp_Object));
extern int x_pixel_width P_((struct frame *));
extern int x_pixel_height P_((struct frame *));
extern int x_char_width P_((struct frame *));
extern int x_char_height P_((struct frame *));
extern int x_screen_planes P_((struct frame *));
extern void x_sync P_((struct frame *));
extern void x_set_tool_bar_lines P_((struct frame *, Lisp_Object, Lisp_Object));
#if 0
extern bool mac_defined_color(struct frame *, const char *, XColor *, bool);
#else
extern int mac_defined_color(FRAME_PTR, char *, XColor *, int);
#endif /* 0 */
extern void mac_update_title_bar P_((struct frame *, int));
extern Lisp_Object x_get_focus_frame P_((struct frame *));

/* Defined in macmenu.c */
extern int popup_activated_flag;
extern void x_activate_menubar P_((struct frame *));
extern void free_frame_menubar P_((struct frame *));
extern Lisp_Object mac_popup_dialog P_((struct frame *, Lisp_Object,
                                        Lisp_Object));
extern OSStatus install_menu_target_item_handler P_((WindowPtr));
#ifdef MAC_APPKIT_M
extern bool name_is_separator P_((const char *));
#endif /* MAC_APPKIT_M */

/* Defined in mac.c.  */
extern Lisp_Object Qstring, Qnumber, Qarray, Qdictionary;
extern Lisp_Object Qrange, Qpoint;
extern Lisp_Object Qapp_name;
extern Lisp_Object QCinfo, QCversion, QCsub_type, QCmanufacturer, QCfeatures;
extern struct mac_operating_system_version
{
  CFIndex major, minor, patch;
} mac_operating_system_version;

extern void string_cat_and_replace P_((char *, const char *, int,
                                       char, char));
extern int mac_to_posix_pathname P_((const char *mfn, char *ufn,
                                     int ufnbuflen));
extern int posix_to_mac_pathname P_((const char *ufn, char *mfn,
                                     int mfnbuflen));
extern void mac_clear_font_name_table P_((void));
extern Lisp_Object mac_four_char_code_to_string (FourCharCode);
extern Boolean mac_string_to_four_char_code (Lisp_Object, FourCharCode *);
extern Lisp_Object mac_aedesc_to_lisp P_((const AEDesc *));
extern OSErr mac_ae_put_lisp P_((AEDescList *, UInt32, Lisp_Object));
extern OSErr create_apple_event_from_lisp (Lisp_Object, AppleEvent *);
extern OSErr init_coercion_handler P_((void));
extern OSErr create_apple_event (AEEventClass, AEEventID, AppleEvent *);
extern Lisp_Object mac_event_parameters_to_lisp (EventRef, UInt32,
						 const EventParamName *,
						 const EventParamType *);
#if TARGET_API_MAC_CARBON
extern OSStatus create_apple_event_from_event_ref P_((EventRef, UInt32,
						      const EventParamName *,
						      const EventParamType *,
						      AppleEvent *));
extern OSErr create_apple_event_from_drag_ref P_((DragRef, UInt32,
						  const FlavorType *,
						  AppleEvent *));
extern CFStringRef cfstring_create_with_utf8_cstring P_((const char *));
extern CFStringRef cfstring_create_with_string_noencode (Lisp_Object);
extern CFStringRef cfstring_create_with_string P_((Lisp_Object));
extern Lisp_Object cfdata_to_lisp P_((CFDataRef));
extern Lisp_Object cfstring_to_lisp_nodecode P_((CFStringRef));
extern Lisp_Object cfstring_to_lisp P_((CFStringRef));
extern Lisp_Object cfstring_to_lisp_utf_16 (CFStringRef);
extern Lisp_Object cfnumber_to_lisp P_((CFNumberRef));
extern Lisp_Object cfdate_to_lisp P_((CFDateRef));
extern Lisp_Object cfboolean_to_lisp P_((CFBooleanRef));
extern Lisp_Object cfobject_desc_to_lisp P_((CFTypeRef));
extern Lisp_Object cfobject_to_lisp (CFTypeRef, int, int);
extern Lisp_Object cfproperty_list_to_lisp P_((CFPropertyListRef, int, int));
extern CFPropertyListRef cfproperty_list_create_with_lisp (Lisp_Object);
extern Lisp_Object cfproperty_list_to_string (CFPropertyListRef,
					      CFPropertyListFormat);
extern CFPropertyListRef cfproperty_list_create_with_string (Lisp_Object);
extern int init_wakeup_fds (void);
extern void mac_wakeup_from_rne P_((void));
extern void mac_wakeup_from_run_loop_run_once (void);
extern EventRef mac_peek_next_event (void);
#endif /* TARGET_API_MAC_CARBON */
extern void xrm_merge_string_database P_((XrmDatabase, const char *));
extern Lisp_Object xrm_get_resource P_((XrmDatabase, const char *,
                                        const char *));
extern XrmDatabase xrm_get_preference_database P_((const char *));
extern int mac_select (int, fd_set *, fd_set *, fd_set *,
		       struct timespec const *, sigset_t const *);
extern void mac_reinvoke_from_shell (int, char *[]);
extern bool mac_service_provider_registered_p (void);
extern Lisp_Object mac_carbon_version_string (void);
extern void initialize_applescript P_((void));
extern void terminate_applescript P_((void));
extern OSType mac_get_code_from_arg P_((Lisp_Object, OSType));
extern Lisp_Object mac_get_object_from_code P_((OSType));
EXFUN(Fmac_get_preference, 4);
extern void init_mac_osx_environment P_((void));
extern void syms_of_mac P_((void));

/* Defined in macappkit.m.  */

extern struct mac_accessibility_display_options
{
  bool increase_contrast_p;
  bool differentiate_without_color_p;
  bool reduce_transparency_p;
} mac_accessibility_display_options;
extern Lisp_Object mac_nsobject_to_lisp (CFTypeRef);
extern void mac_alert_sound_play (void);
extern double mac_appkit_version (void);
extern double mac_system_uptime (void);
extern Boolean mac_is_current_process_frontmost (void);
extern void mac_bring_current_process_to_front (Boolean);
#if (MAC_OS_X_VERSION_MAX_ALLOWED >= 1050)
extern bool mac_trash_file (const char *, CFErrorRef *);
#endif /* 10.5+ */
extern OSStatus install_application_handler (void);
extern void mac_set_frame_window_title (struct frame *, CFStringRef);
extern void mac_set_frame_window_modified (struct frame *, Boolean);
extern Boolean mac_is_frame_window_visible (struct frame *);
extern Boolean mac_is_frame_window_collapsed (struct frame *);
extern void mac_bring_frame_window_to_front (struct frame *);
extern void mac_send_frame_window_behind (struct frame *);
extern void mac_hide_frame_window (struct frame *);
extern void mac_show_frame_window (struct frame *);
extern OSStatus mac_collapse_frame_window (struct frame *, Boolean);
extern Boolean mac_is_frame_window_frontmost (struct frame *);
extern void mac_activate_frame_window (struct frame *);
extern OSStatus mac_move_frame_window_structure (struct frame *, short, short);
extern void mac_move_frame_window (struct frame *, short, short, Boolean);
extern void mac_size_frame_window (struct frame *, short, short, Boolean);
extern OSStatus mac_set_frame_window_alpha (struct frame *, CGFloat);
extern OSStatus mac_get_frame_window_alpha (struct frame *, CGFloat *);
extern void mac_get_global_mouse (Point *);
extern Boolean mac_is_frame_window_toolbar_visible (struct frame *);
#if (MAC_OS_X_VERSION_MIN_REQUIRED < 1070)
extern CGRect mac_rect_make (struct frame *, CGFloat, CGFloat,
			     CGFloat, CGFloat);
#else
# define mac_rect_make(f, x, y, w, h)	CGRectMake(x, y, w, h)
#endif /* older than 10.7 */
extern void mac_get_window_structure_bounds (struct frame *, NativeRectangle *);
extern void mac_get_frame_mouse (struct frame *, Point *);
extern void mac_convert_frame_point_to_global (struct frame *, int *, int *);
extern void mac_update_proxy_icon (struct frame *);
extern void mac_set_frame_window_background (struct frame *, unsigned long);
extern void mac_update_begin (struct frame *);
extern void mac_update_end (struct frame *);
extern void mac_cursor_to (int, int, int, int);
extern void x_flush (struct frame *);
extern void mac_flush (struct frame *);
extern void mac_create_frame_window (struct frame *);
extern void mac_dispose_frame_window (struct frame *);
extern void mac_change_frame_window_wm_state (struct frame *, WMState, WMState);
#if defined(DRAWING_USE_GCD) && DRAWING_USE_GCD && defined(HAVE_CBLOCKS_LANGUAGE_FEATURE)
extern void mac_draw_to_frame (struct frame *, GC, void (^)(CGContextRef, GC));
#else
extern CGContextRef mac_begin_cg_clip (struct frame *, GC);
extern void mac_end_cg_clip (struct frame *);
#endif /* DRAWING_USE_GCD && HAVE_CBLOCKS_LANGUAGE_FEATURE */
extern void mac_scroll_area(struct frame *, GC, int, int, int, int, int, int);
extern Lisp_Object mac_display_monitor_attributes_list (struct mac_display_info *);
extern void mac_create_scroll_bar (struct scroll_bar *);
extern void mac_dispose_scroll_bar (struct scroll_bar *);
extern void mac_update_scroll_bar_bounds (struct scroll_bar *);
extern void mac_redraw_scroll_bar (struct scroll_bar *);
extern void x_set_toolkit_scroll_bar_thumb (struct scroll_bar *, int, int, int);
extern int mac_get_default_scroll_bar_width (struct frame *);
extern OSStatus mac_show_hide_font_panel (void);
extern void mac_get_screen_info (struct mac_display_info *);
extern EventTimeout mac_run_loop_run_once (EventTimeout);
extern int mac_read_socket (struct terminal *, struct input_event *);
extern void update_frame_tool_bar(struct frame *f);
extern void free_frame_tool_bar(struct frame *f);
extern void mac_show_hourglass(struct frame *);
extern void mac_hide_hourglass(struct frame *);
extern Lisp_Object mac_file_dialog(Lisp_Object, Lisp_Object, Lisp_Object,
				   Lisp_Object, Lisp_Object);
extern Lisp_Object mac_font_dialog(struct frame *f);
extern int mac_activate_menubar(struct frame *);
extern void mac_fill_menubar(widget_value *, bool);
extern int create_and_show_popup_menu(struct frame *, widget_value *, int, int,
				      bool);
extern int create_and_show_dialog(struct frame *, widget_value *);
extern OSStatus mac_get_selection_from_symbol(Lisp_Object, bool, Selection *);
extern bool mac_valid_selection_target_p(Lisp_Object);
extern OSStatus mac_clear_selection(Selection *);
extern Lisp_Object mac_get_selection_ownership_info(Selection);
extern bool mac_valid_selection_value_p(Lisp_Object, Lisp_Object);
extern OSStatus mac_put_selection_value(Selection, Lisp_Object, Lisp_Object);
extern bool mac_selection_has_target_p(Selection, Lisp_Object);
extern Lisp_Object mac_get_selection_value(Selection, Lisp_Object);
extern Lisp_Object mac_get_selection_target_list(Selection);
extern Lisp_Object mac_dnd_default_known_types(void);

#if defined(__clang__) && (MAC_OS_X_VERSION_MIN_REQUIRED >= 1060) && \
    defined(HAVE_CBLOCKS_LANGUAGE_FEATURE)
# define MAC_USE_AUTORELEASE_LOOP 1
extern void mac_autorelease_loop (Lisp_Object (^)(void));
#else
extern void *mac_alloc_autorelease_pool (void);
extern void mac_release_autorelease_pool (void *);
#endif /* __clang__ && 10.6+ && HAVE_CBLOCKS_LANGUAGE_FEATURE */

extern bool mac_tracking_area_works_with_cursor_rects_invalidation_p (void);
extern Cursor mac_cursor_create (ThemeCursor, const XColor *, const XColor *);
extern void mac_cursor_set (Cursor);
extern void mac_cursor_release (Cursor);
extern void mac_invalidate_frame_cursor_rects (struct frame *f);
extern void mac_mask_rounded_bottom_corners (struct frame *, CGRect, Boolean);
extern void mac_invalidate_rectangles (struct frame *, NativeRectangle *, int);
extern Lisp_Object mac_osa_language_list (bool);
extern Lisp_Object mac_osa_compile (Lisp_Object, Lisp_Object, bool,
				    Lisp_Object *);
extern Lisp_Object mac_osa_script (Lisp_Object, Lisp_Object, bool, Lisp_Object,
				   Lisp_Object, ptrdiff_t, Lisp_Object *,
				   Lisp_Object *);
extern bool mac_webkit_supports_svg_p(void);
extern bool mac_svg_load_image(struct frame *, struct image *, unsigned char *,
			       ptrdiff_t, XColor *,
			       bool (*)(struct frame *, int, int),
			       void (*)(const char *, Lisp_Object,
					Lisp_Object));
extern CFArrayRef mac_document_copy_type_identifiers(void);
extern EmacsDocumentRef mac_document_create_with_url(CFURLRef,
						     CFDictionaryRef);
extern EmacsDocumentRef mac_document_create_with_data(CFDataRef,
						      CFDictionaryRef);
extern size_t mac_document_get_page_count(EmacsDocumentRef);
extern void mac_document_copy_page_info(EmacsDocumentRef, size_t, CGSize *,
					CGColorRef *, CFDictionaryRef *);
extern void mac_document_draw_page P_((CGContextRef, CGRect,
                                       EmacsDocumentRef, size_t));
extern void mac_update_accessibility_status P_((struct frame *));
#if (MAC_OS_X_VERSION_MAX_ALLOWED >= 1050)
extern void mac_start_animation(Lisp_Object, Lisp_Object);
#endif /* 10.5+ */
extern CFTypeRef mac_sound_create P_((Lisp_Object, Lisp_Object));
extern void mac_sound_play P_((CFTypeRef, Lisp_Object, Lisp_Object));

#if DRAWING_USE_GCD && defined(HAVE_CBLOCKS_LANGUAGE_FEATURE)
# define MAC_BEGIN_DRAW_TO_FRAME(f, gc, context)			\
   mac_draw_to_frame(f, gc, ^(CGContextRef context, GC gc) {
# define MAC_END_DRAW_TO_FRAME(f)		\
   })
#else
# define MAC_BEGIN_DRAW_TO_FRAME(f, gc, context)		\
   do { CGContextRef context = mac_begin_cg_clip(f, gc)
# define MAC_END_DRAW_TO_FRAME(f)		\
   mac_end_cg_clip(f); } while (0)
#endif /* DRAWING_USE_GCD && HAVE_CBLOCKS_LANGUAGE_FEATURE */

#define CG_CONTEXT_FILL_RECT_WITH_GC_BACKGROUND(f, context, rect, gc)	\
  do {									\
    if ((gc)->xgcv.background_transparency == 0)			\
      CGContextSetFillColorWithColor(context, (gc)->cg_back_color);	\
    else if (FRAME_BACKGROUND_ALPHA_ENABLED_P(f)			\
             && !mac_accessibility_display_options.reduce_transparency_p) \
      {									\
        CGContextClearRect(context, rect);				\
        CGContextSetFillColorWithColor(context, (gc)->cg_back_color);	\
      }									\
    else								\
      {									\
        CGColorRef color =						\
        CGColorCreateCopyWithAlpha((gc)->cg_back_color, 1.0f);  	\
        CGContextSetFillColorWithColor(context, color); 		\
        CGColorRelease(color);						\
      }									\
    CGContextFillRects(context, &(rect), 1);				\
  } while (0)

/* Defined in macfont.m */
extern void macfont_update_antialias_threshold (void);
extern void *macfont_get_nsctfont (struct font *);
extern Lisp_Object macfont_nsctfont_to_spec (void *);

/* Defined in xdisp.c */
extern struct glyph *x_y_to_hpos_vpos (struct window *, int, int,
				       int *, int *, int *, int *, int *);
extern void frame_to_window_pixel_xy (struct window *, int *, int *);
extern void rows_from_pos_range (struct window *, ptrdiff_t , ptrdiff_t,
				 Lisp_Object, struct glyph_row **,
				 struct glyph_row **);

/* Defined in keyboard.c */
extern Lisp_Object Qundefined;

/* Defined in coding.c */
extern Lisp_Object QCcategory;

#endif /* !_EMACS_MACTERM_H */

/* arch-tag: 6b4ca125-5bef-476d-8ee8-31ed808b7e79
   (do not change this comment) */
