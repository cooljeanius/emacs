/* src/macgui.h: Definitions and headers for communication on the Mac OS.
   Copyright (C) 2000, 2001, 2002, 2003, 2004,
                 2005, 2006, 2007 Free Software Foundation, Inc.

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

#ifndef EMACS_MACGUI_H
#define EMACS_MACGUI_H

#ifndef __has_include
# define __has_include(x) 0
#endif /* !__has_include */

typedef struct _XDisplay Display; /* opaque */

#ifndef _XRM_DATABASE_DECLARED
# define _XRM_DATABASE_DECLARED 1
typedef Lisp_Object XrmDatabase;
#endif /* !_XRM_DATABASE_DECLARED */

#if !(defined(EMACS_SYSTIME_H) && defined(emacs) && \
      !(defined(HAVE_X_WINDOWS) || defined(HAVE_X11_X_H)))
typedef unsigned long Time;
#endif /* !(EMACS_SYSTIME_H && emacs && !(HAVE_X_WINDOWS || HAVE_X11_X_H)) */

#ifdef HAVE_CARBON
# undef Z
# ifdef MAC_OSX
#  if defined(mktime) && \
      ((!defined(HAVE_MKTIME) || !HAVE_MKTIME) || (defined(BROKEN_MKTIME) && (BROKEN_MKTIME)))
#   undef mktime
#  endif /* mktime !HAVE_MKTIME || BROKEN_MKTIME */
#  undef DEBUG
#  undef free
#  undef malloc
#  undef realloc
/* Macros max and min defined in lisp.h conflict with those in
   precompiled header Carbon.h.  */
#  undef max
#  undef min
#  undef init_process
#  ifndef __ASSERT_MACROS_DEFINE_VERSIONS_WITHOUT_UNDERSCORES
#   define __ASSERT_MACROS_DEFINE_VERSIONS_WITHOUT_UNDERSCORES 0
#  endif /* !__ASSERT_MACROS_DEFINE_VERSIONS_WITHOUT_UNDERSCORES */
#  if defined(HAVE_CARBON_CARBON_H) || __has_include(<Carbon/Carbon.h>)
#   include <Carbon/Carbon.h>
#  else
#   if defined(HAVE_CARBON_H) || __has_include(<Carbon.h>)
#    include <Carbon.h>
#   else
#    if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#     warning "macgui.h expects <Carbon/Carbon.h> to be included."
#    endif /* __GNUC__ && !__STRICT_ANSI__ */
#   endif /* HAVE_CARBON_H */
#  endif /* HAVE_CARBON_CARBON_H */
#  ifdef check /* __ASSERT_MACROS_DEFINE_VERSIONS_WITHOUT_UNDERSCORES is
                * not in effect.  */
#   undef check
#   undef verify
#   undef _GL_VERIFY_H
#   include <verify.h>
#  endif /* check */
#  if (!defined(HAVE_MKTIME) || !HAVE_MKTIME) || (defined(BROKEN_MKTIME) && (BROKEN_MKTIME))
#   if defined(mktime)
#    undef mktime
#   endif /* mktime */
#   define mktime emacs_mktime
#  endif /* !HAVE_MKTIME || BROKEN_MKTIME */
#  undef free
#  define free unexec_free
#  undef malloc
#  define malloc unexec_malloc
#  undef realloc
#  define realloc unexec_realloc
#  undef min
#  define min(a, b) ((a) < (b) ? (a) : (b))
#  undef max
#  define max(a, b) ((a) > (b) ? (a) : (b))
#  undef init_process
#  define init_process emacs_init_process
#  undef INFINITY
# else  /* not MAC_OSX */
#  undef SIGHUP
#  define OLDP2C 1
#  include <Carbon.h>
# endif  /* not MAC_OSX */
# undef Z
# define Z (current_buffer->text->z)
# ifdef ALIGN
#  undef ALIGN
# endif /* ALIGN */
#else /* not HAVE_CARBON */
# include <QuickDraw.h>		/* for WindowPtr */
# include <QDOffscreen.h>	/* for GWorldPtr */
# include <Appearance.h>		/* for ThemeCursor */
# include <Windows.h>
# include <Controls.h>
# include <Gestalt.h>
#endif /* not HAVE_CARBON */

#if !defined(__QDOFFSCREEN__)
# if defined(HAVE_QD_QDOFFSCREEN_H) || __has_include(<QD/QDOffscreen.h>)
#  include <QD/QDOffscreen.h>
# else
#  if defined(HAVE_QDOFFSCREEN_H) || __has_include(<QDOffscreen.h>)
#   include <QDOffscreen.h>
#  else
#   if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#    warning "macgui.h expects <QD/QDOffscreen.h> to be included."
#   endif /* __GNUC__ && !__STRICT_ANSI__ */
#  endif /* HAVE_QDOFFSCREEN_H */
# endif /* HAVE_QD_QDOFFSCREEN_H */
#endif /* !__QDOFFSCREEN__ */

#if !defined(__QUICKDRAWAPI__)
enum {
  kQDUseDefaultTextRendering = 0,
  kQDUseTrueTypeScalerGlyphs = (1 << 0),
  kQDUseCGTextRendering = (1 << 1),
  kQDUseCGTextMetrics = (1 << 2),
  kQDSupportedFlags = (kQDUseTrueTypeScalerGlyphs | kQDUseCGTextRendering | kQDUseCGTextMetrics),
  kQDDontChangeFlags = (int)0xFFFFFFFF
};
#endif /* !__QUICKDRAWAPI__ */

#if !defined(__QUICKDRAWTYPES__)
enum {
  srcOr              = 1,
  srcXor             = 2,
  srcBic             = 3,
  notSrcCopy         = 4,
  notSrcOr           = 5,
  notSrcXor          = 6,
  notSrcBic          = 7,
  patCopy            = 8,
  patOr              = 9,
  patXor             = 10,
  patBic             = 11,
  notPatCopy         = 12,
  notPatOr           = 13,
  notPatXor          = 14,
  notPatBic          = 15,
  grayishTextOr      = 49,
  hilitetransfermode = 50,
  hilite             = 50,
  blend              = 32,
  addPin             = 33,
  addOver            = 34,
  subPin             = 35,
  addMax             = 37,
  adMax              = 37,
  subOver            = 38,
  adMin              = 39,
  ditherCopy         = 64,
  transparent        = 36
};
#endif /* !__QUICKDRAWTYPES__ */

#ifndef CGFLOAT_DEFINED
typedef float CGFloat;
# define CGFLOAT_MIN FLT_MIN
# define CGFLOAT_MAX FLT_MAX
#endif /* !CGFLOAT_DEFINED */

/* Whether to use ATSUI (Apple Type Services for Unicode Imaging) for
   text drawing.  */
#ifndef USE_ATSUI
# ifdef MAC_OSX
#  define USE_ATSUI 1
# endif /* MAC_OSX */
#endif /* !USE_ATSUI */

/* Whether to use low-level Quartz 2D (aka Core Graphics) text drawing
   in preference to ATSUI for ASCII and Latin-1 characters.  */
#ifndef USE_CG_TEXT_DRAWING
# if USE_ATSUI && (MAC_OS_X_VERSION_MAX_ALLOWED >= 1030)
#  define USE_CG_TEXT_DRAWING 1
# endif /* USE_ATSUI && 10.3+ */
#endif /* !USE_CG_TEXT_DRAWING */

/* Whether to use Quartz 2D routines for drawing operations other than
   texts.  */
#ifndef USE_CG_DRAWING
# if USE_ATSUI && (MAC_OS_X_VERSION_MAX_ALLOWED >= 1020)
#  define USE_CG_DRAWING 1
# endif /* USE_ATSUI && 10.2+ */
#endif /* !USE_CG_DRAWING */

/* Whether to use the standard Font Panel floating dialog.  */
#ifndef USE_MAC_FONT_PANEL
# if USE_ATSUI && MAC_OS_X_VERSION_MAX_ALLOWED >= 1020
#  define USE_MAC_FONT_PANEL 1
# endif /* USE_ATSUI && 10.2+ */
#endif /* !USE_MAC_FONT_PANEL */

/* Whether to use Text Services Manager.  */
#ifndef USE_MAC_TSM
# if TARGET_API_MAC_CARBON
#  define USE_MAC_TSM 1
# endif /* TARGET_API_MAC_CARBON */
#endif /* !USE_MAC_TSM */

typedef WindowPtr Window; /* void pointer in Y.M.'s Mac port */
typedef void *VoidWindow;
typedef void *Selection;
typedef GWorldPtr Pixmap;

typedef struct _XImage
{
  int width, height;		/* size of image */
  char *data;			/* pointer to image data */
  int bytes_per_line;		/* accelarator to next line */
  int bits_per_pixel;		/* bits per pixel (ZPixmap) */
} *MacPixmap;

typedef const struct _EmacsDocument *EmacsDocumentRef; /* opaque */

#define Cursor ThemeCursor /* CFTypeRef in Y.M.'s Mac port */
#ifndef CFCursorRef
# define CFCursorRef CFTypeRef
#endif /* !CFCursorRef */
#define No_Cursor (-1) /* NULL in Y.M.'s Mac port */
#ifndef No_Cursor_Ref
# define No_Cursor_Ref NULL
#endif /* !No_Cursor_Ref */

#define FACE_DEFAULT (~0)

#if !TARGET_API_MAC_CARBON
# define GetPixDepth(pmh) ((*(pmh))->pixelSize)
#endif /* !TARGET_API_MAC_CARBON */


/* Emulate XCharStruct.  */
/* If the sum of ascent and descent is negative, that means some
   special status specified by enum pcm_status.  */
typedef struct _XCharStruct
{
  short	lbearing;		/* origin to left edge of raster */
  short	rbearing;		/* origin to right edge of raster */
  short	width;			/* advance to next char's origin */
  short	ascent;			/* baseline to top edge of raster */
  short	descent;		/* baseline to bottom edge of raster */
#if 0
  unsigned short attributes;	/* per char flags (not predefined) */
#endif /* 0 */
} XCharStruct;

enum pcm_status
  {
    PCM_VALID = 0,		/* pcm data is valid */
    PCM_INVALID = -1,		/* pcm data is invalid */
  };

#define STORE_XCHARSTRUCT(xcs, w, bds)			\
  ((xcs).width = (w),					\
   (xcs).lbearing = (bds).left,				\
   (xcs).rbearing = (bds).right,			\
   (xcs).ascent = -(bds).top,				\
   (xcs).descent = (bds).bottom)

struct MacFontStruct {
  char *full_name;

  short mac_fontnum;  /* font number of font used in this window */
  int mac_fontsize;  /* size of font */
  short mac_fontface;  /* plain, bold, italics, etc. */
#if TARGET_API_MAC_CARBON
  int mac_scriptcode;  /* Mac OS script code for font used */
#else
  short mac_scriptcode;  /* Mac OS script code for font used */
#endif /* TARGET_API_MAC_CARBON */
#if USE_ATSUI
  ATSUStyle mac_style;		/* NULL if QuickDraw Text is used */
# if USE_CG_TEXT_DRAWING
  CGFontRef cg_font;		/* NULL if ATSUI text drawing is used */
  CGGlyph *cg_glyphs;		/* Likewise  */
# endif /* USE_CG_TEXT_DRAWING */
#endif /* USE_ATSUI */

/* from Xlib.h */
#if 0
  XExtData *ext_data;      /* hook for extension to hang data */
  Font fid;                /* Font id for this font */
  unsigned direction;      /* hint about the direction font is painted */
#endif /* 0 */
  unsigned min_char_or_byte2;/* first character */
  unsigned max_char_or_byte2;/* last character */
  unsigned min_byte1;      /* first row that exists */
  unsigned max_byte1;      /* last row that exists */
#if 0
  Bool all_chars_exist;    /* flag if all characters have nonzero size */
  unsigned default_char;   /* char to print for undefined character */
  int n_properties;        /* how many properties there are */
  XFontProp *properties;   /* pointer to array of additional properties */
#endif /* 0 */
  XCharStruct min_bounds;  /* minimum bounds over all existing char */
  XCharStruct max_bounds;  /* maximum bounds over all existing char */
  union {
    XCharStruct *per_char; /* first_char to last_char information */
    XCharStruct **rows;    /* first row to last row information */
  } bounds;
  int ascent;              /* logical extent above baseline for spacing */
  int descent;             /* logical decent below baseline for spacing */
};

typedef struct MacFontStruct MacFontStruct;
#ifndef _XFONTSTRUCT_DEFINED
# define _XFONTSTRUCT_DEFINED 1
typedef struct MacFontStruct XFontStruct;
#endif /* !_XFONTSTRUCT_DEFINED */

/* Structure borrowed from Xlib.h to represent two-byte characters: */
typedef struct {
  unsigned char byte1;
  unsigned char byte2;
} XChar2b; /* CGGlyph in Y.M.'s Mac port */

typedef CGGlyph CGXChar2b;

#define STORE_XCHAR2B(chp, b1, b2) \
  ((chp)->byte1 = (b1), (chp)->byte2 = (b2))

#define XCHAR2B_BYTE1(chp) \
  ((chp)->byte1)

#define XCHAR2B_BYTE2(chp) \
  ((chp)->byte2)


#if MAC_OS_X_VERSION_MIN_REQUIRED >= 1060
# ifndef DRAWING_USE_GCD
#  define DRAWING_USE_GCD 1
# endif /* !DRAWING_USE_GCD */
#endif /* 10.6+ */

/* Emulate X GC's by keeping color and font info in a structure.  */
typedef struct _XGCValues
{
  unsigned long foreground; /* 32-bit bitfield in Y.M.'s Mac port */
  unsigned long background; /* 24-bit bitfield in Y.M.'s Mac port */
#ifdef MAC_APPKIT_M
  /* Background transparency: 0 = opaque, 255 = transparent.  */
  unsigned background_transparency : 8;
#else
  XFontStruct *font;
#endif /* MAC_APPKIT_M */
} XGCValues;

typedef struct _XGC
{
  /* Original value.  */
  XGCValues xgcv;

  /* Cached data members follow.  */

  /* QuickDraw foreground color.  */
  RGBColor fore_color;

  /* QuickDraw background color.  */
  RGBColor back_color;

#if USE_CG_DRAWING && (MAC_OS_X_VERSION_MAX_ALLOWED >= 1030)
  /* Quartz 2D foreground color.  */
  CGColorRef cg_fore_color;

  /* Quartz 2D background color.  */
  CGColorRef cg_back_color;
#endif /* USE_CG_DRAWING && 10.3+ */

#define MAX_CLIP_RECTS 2
  /* Number of clipping rectangles.  */
  int n_clip_rects;

  /* QuickDraw clipping region.  Ignored if n_clip_rects == 0.  */
  RgnHandle clip_region;

#if defined(MAC_OSX) && (USE_ATSUI || USE_CG_DRAWING)
  /* Clipping rectangles used in Quartz 2D drawing.  The y-coordinate
     is in QuickDraw's.  */
  CGRect clip_rects[MAX_CLIP_RECTS];
#endif /* MAC_OSX && (USE_ATSUI || USE_CG_DRAWING) */

#ifdef MAC_APPKIT_M
  /* Data consisting of clipping rectangles used in Quartz 2D drawing.
   * The y-coordinate is in the flipped coordinates.  */
  CFDataRef clip_rects_data;
#endif /* MAC_APPKIT_M */
} *GC;

#define GCForeground            (1L<<2)
#define GCBackground            (1L<<3)
#define GCFont 			(1L<<14)
#define GCGraphicsExposures	0
#ifndef GCBackgroundTransparency
# define GCBackgroundTransparency (1L<<16)
#endif /* !GCBackgroundTransparency */

/* Bit Gravity */

#define ForgetGravity		0
#define NorthWestGravity	1
#define NorthGravity		2
#define NorthEastGravity	3
#define WestGravity		4
#define CenterGravity		5
#define EastGravity		6
#define SouthWestGravity	7
#define SouthGravity		8
#define SouthEastGravity	9
#define StaticGravity		10

#define NoValue		0x0000
#define XValue  	0x0001
#define YValue		0x0002
#define WidthValue  	0x0004
#define HeightValue  	0x0008
#define AllValues 	0x000F
#define XNegative 	0x0010
#define YNegative 	0x0020

typedef struct {
    	long flags;  /* marks which fields in this structure are defined */
#if 0
	int x, y;	    /* obsolete for new window mgrs, but clients */
	int width, height;  /* should set so old wm's do NOT mess up */
#endif /* 0 */
	int min_width, min_height;
#if 0
	int max_width, max_height;
#endif /* 0 */
    	int width_inc, height_inc;
#if 0
	struct {
		int x;	/* numerator */
		int y;	/* denominator */
	} min_aspect, max_aspect;
#endif /* 0 */
	int base_width, base_height;		/* added by ICCCM version 1 */
#if 0
	int win_gravity;			/* added by ICCCM version 1 */
#endif /* 0 */
} XSizeHints;

#define USPosition	(1L << 0) /* user specified x, y */
#define USSize		(1L << 1) /* user specified width, height */

#define PPosition	(1L << 2) /* program specified position */
#define PSize		(1L << 3) /* program specified size */
#define PMinSize	(1L << 4) /* program specified minimum size */
#define PMaxSize	(1L << 5) /* program specified maximum size */
#define PResizeInc	(1L << 6) /* program specified resize increments */
#define PAspect		(1L << 7) /* program specified min and max aspect ratios */
#define PBaseSize	(1L << 8) /* program specified base for incrementing */
#define PWinGravity	(1L << 9) /* program specified window gravity */

/* Constants corresponding to window state hint atoms in X11 Extended
 * Window Manager Hints (without "_NET_" prefix).  Mostly unimplemented: */
enum
{
  WM_STATE_MODAL		= 1 << 0,
  WM_STATE_STICKY		= 1 << 1,
  WM_STATE_MAXIMIZED_VERT	= 1 << 2,
  WM_STATE_MAXIMIZED_HORZ	= 1 << 3,
  WM_STATE_SHADED		= 1 << 4,
  WM_STATE_SKIP_TASKBAR		= 1 << 5,
  WM_STATE_SKIP_PAGER		= 1 << 6,
  WM_STATE_HIDDEN		= 1 << 7,
  WM_STATE_FULLSCREEN		= 1 << 8,
  WM_STATE_ABOVE		= 1 << 9,
  WM_STATE_BELOW		= 1 << 10,
  WM_STATE_DEMANDS_ATTENTION	= 1 << 11
};

/* These are not derived from X11 EWMH window state hints, but used
 * like them: */
enum
{
  WM_STATE_NO_MENUBAR		= 1 << 12,
  WM_STATE_DEDICATED_DESKTOP	= 1 << 13
};

typedef uint32_t WMState;

typedef struct {
    int x, y;
    unsigned width, height;
} XRectangle;

#ifndef NativeRectangle
# define NativeRectangle Rect /* XRectangle in Y.M.'s Mac port */
#endif /* !NativeRectangle */

#define CONVERT_TO_XRECT(xr,nr)			\
  ((xr).x = (nr).left,				\
   (xr).y = (nr).top,				\
   (xr).width = ((nr).right - (nr).left),	\
   (xr).height = ((nr).bottom - (nr).top))

#define CONVERT_TO_XRECT_PTR(xrp,nrp)		\
  ((xrp)->x = (nrp)->left,			\
   (xrp)->y = (nrp)->top,			\
   (xrp)->width = ((nrp)->right - (nrp)->left),	\
   (xrp)->height = ((nrp)->bottom - (nrp)->top))

#define CONVERT_FROM_XRECT(xr,nr)		\
  ((nr).left = (xr).x,				\
   (nr).top = (xr).y,				\
   (nr).right = ((xr).x + (xr).width),		\
   (nr).bottom = ((xr).y + (xr).height))

/* FIXME: change depending on whether NativeRectangle is an XRectangle or
 * a Rect: */
#define STORE_NATIVE_RECT(nr,x,y,width,height)	\
  ((nr).left = (x),				\
   (nr).top = (y),				\
   (nr).right = ((nr).left + (width)),		\
   (nr).bottom = ((nr).top + (height)))

enum {
  CFOBJECT_TO_LISP_WITH_TAG			= 1 << 0,
  CFOBJECT_TO_LISP_DONT_DECODE_STRING		= 1 << 1,
  CFOBJECT_TO_LISP_DONT_DECODE_DICTIONARY_KEY	= 1 << 2
};

/* Assumed by other routines to zero area returned: */
#define malloc_widget_value() (void*)memset(xmalloc(sizeof(widget_value)),\
                                            0, (sizeof(widget_value)))
#define free_widget_value(wv) xfree(wv)

#define DIALOG_LEFT_MARGIN (112)
#define DIALOG_TOP_MARGIN (24)
#define DIALOG_RIGHT_MARGIN (24)
#define DIALOG_BOTTOM_MARGIN (20)
#define DIALOG_MIN_INNER_WIDTH (338)
#define DIALOG_MAX_INNER_WIDTH (564)
#define DIALOG_BUTTON_BUTTON_HORIZONTAL_SPACE (12)
#define DIALOG_BUTTON_BUTTON_VERTICAL_SPACE (12)
#define DIALOG_BUTTON_MIN_WIDTH (68)
#define DIALOG_TEXT_MIN_HEIGHT (50)
#define DIALOG_TEXT_BUTTONS_VERTICAL_SPACE (10)
#define DIALOG_ICON_WIDTH (64)
#define DIALOG_ICON_HEIGHT (64)
#define DIALOG_ICON_LEFT_MARGIN (24)
#define DIALOG_ICON_TOP_MARGIN (15)

#endif /* EMACS_MACGUI_H */

/* arch-tag: 5a0da49a-35e2-418b-a58c-8a55778ae849
   (do not change this comment) */
