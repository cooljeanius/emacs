/* image.c: Functions for image support on window system.

Copyright (C) 1989, 1992-2014 Free Software Foundation, Inc.

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

/* because of <pngconf.h>: */
#ifndef NO_POISON
# define NO_POISON 1
#endif /* !NO_POISON */

#include <config.h>
#include "sysstdio.h"
#include <math.h>
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif /* HAVE_UNISTD_H */

#ifdef HAVE_PNG
# if defined HAVE_LIBPNG_PNG_H
#  include <libpng/png.h>
# else
#  include <png.h>
# endif /* HAVE_LIBPNG_PNG_H */
#endif /* HAVE_PNG */

#include <setjmp.h>
#include <c-ctype.h>

#ifndef XLIB_ILLEGAL_ACCESS
/* This makes the fields of a Display accessible, in Xlib header files: */
# define XLIB_ILLEGAL_ACCESS
#endif /* !XLIB_ILLEGAL_ACCESS */

#include "lisp.h"
#include "frame.h"
#include "window.h"
#include "dispextern.h"
#include "blockinput.h"
#include "systime.h"
#include <epaths.h>
#include "character.h"
#include "coding.h"
#include "termhooks.h"
#include "font.h"

#ifdef HAVE_SYS_STAT_H
# include <sys/stat.h>
#endif /* HAVE_SYS_STAT_H */

#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif /* HAVE_SYS_TYPES_H */

#ifdef HAVE_WINDOW_SYSTEM
# include TERM_HEADER
#endif /* HAVE_WINDOW_SYSTEM */

#ifdef HAVE_X_WINDOWS
# define COLOR_TABLE_SUPPORT 1

typedef struct x_bitmap_record Bitmap_Record;
# define GET_PIXEL(ximg, x, y) XGetPixel(ximg, x, y)
# define NO_PIXMAP None

# define PIX_MASK_RETAIN 0
# define PIX_MASK_DRAW 1
#endif /* HAVE_X_WINDOWS */

#ifdef HAVE_NTGUI

/* We need (or want) w32.h only when we are _not_ compiling for Cygwin: */
# ifdef WINDOWSNT
#  include "w32.h"
# endif /* WINDOWSNT */

/* W32_TODO : Color tables on W32.  */
# undef COLOR_TABLE_SUPPORT

typedef struct w32_bitmap_record Bitmap_Record;
# define GET_PIXEL(ximg, x, y) GetPixel (ximg, x, y)
# define NO_PIXMAP 0

# define PIX_MASK_RETAIN 0
# define PIX_MASK_DRAW 1

# define x_defined_color w32_defined_color
# define DefaultDepthOfScreen(screen) (one_w32_display_info.n_cbits)

/* Versions of libpng, libgif, and libjpeg that we were compiled with,
   or -1 if no PNG/GIF support was compiled in.  This is tested by
   w32-win.el to correctly set up the alist used to search for the
   respective image libraries.  */
Lisp_Object Qlibpng_version, Qlibgif_version, Qlibjpeg_version;

#endif /* HAVE_NTGUI */

#if defined(MAC_OS) || (defined(HAVE_CARBON) && !defined(HAVE_NS))
# include "macterm.h"
# include <sys/stat.h>
# ifndef MAC_OSX
#  include <alloca.h>
#  include <sys/param.h>
# endif /* !MAC_OSX */
# if defined(TARGET_API_MAC_CARBON) && TARGET_API_MAC_CARBON
#  ifdef MAC_OSX
#   include <QuickTime/QuickTime.h>
#  else  /* not MAC_OSX: */
#   include <QuickTime.h>
#  endif /* not MAC_OSX */
# else  /* not TARGET_API_MAC_CARBON */
#  include <Windows.h>
#  include <Gestalt.h>
#  include <TextUtils.h>
#  include <ImageCompression.h>
#  include <QuickTimeComponents.h>
# endif /* not TARGET_API_MAC_CARBON */

/* MAC_TODO : Color tables on Mac.  */
# undef COLOR_TABLE_SUPPORT

# define ZPixmap 0 		/* arbitrary */
typedef struct mac_bitmap_record Bitmap_Record;

# ifndef GET_PIXEL
static unsigned long XGetPixel(XImagePtr, int, int);
#  define GET_PIXEL(ximg, x, y) XGetPixel(ximg, x, y)
# endif /* !GET_PIXEL */
# ifndef PUT_PIXEL
static void XPutPixel(XImagePtr, int, int, unsigned long);
#  define PUT_PIXEL(ximg, x, y) XPutPixel(ximg, x, y)
# endif /* !PUT_PIXEL */
# define NO_PIXMAP 0

# define RGB_PIXEL_COLOR unsigned long

/* A black pixel in a mask bitmap/pixmap means ``draw a source
 * pixel''.  A white pixel means ``retain the current pixel''. */
# define PIX_MASK_DRAW RGB_TO_ULONG(0,0,0)
# define PIX_MASK_RETAIN RGB_TO_ULONG(255,255,255)

# define FRAME_X_VISUAL(f) FRAME_X_DISPLAY_INFO (f)->visual
# define x_defined_color(f, name, color_def, alloc) \
    mac_defined_color(f, name, color_def, alloc)
# define DefaultDepthOfScreen(screen) (one_mac_display_info.n_planes)
#endif /* MAC_OS || (HAVE_CARBON && !HAVE_NS && !__LP64__) */

/* some of these so-called "unused" macros are actually used; but it is
 * easier to just ignore the warnings about them than it is to navigate
 * the ifdefs and make sure they are the same both where they are used
 * and where they are defined... */
#if defined(__GNUC__) && defined(__GNUC_MINOR__) && \
    ((__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 5)))
# pragma GCC diagnostic ignored "-Wunused-macros"
#endif /* gcc 4.5+ */
/* (4.5 is the same version as in the other place where "-Wunused-macros"
 *  is ignored in this project) */

#ifdef HAVE_NS
# undef COLOR_TABLE_SUPPORT

typedef struct ns_bitmap_record Bitmap_Record;

# define GET_PIXEL(ximg, x, y) XGetPixel(ximg, x, y)
# define NO_PIXMAP 0

# define PIX_MASK_RETAIN 0
# ifndef PIX_MASK_DRAW
#  define PIX_MASK_DRAW 1
# endif /* !PIX_MASK_DRAW */

# define x_defined_color(f, name, color_def, alloc) \
    ns_defined_color(f, name, color_def, alloc, 0)
# ifndef DefaultDepthOfScreen
#  define DefaultDepthOfScreen(screen) x_display_list->n_planes
# endif /* !DefaultDepthOfScreen */
#endif /* HAVE_NS */


/* The symbol `postscript' identifying images of this type: */
static Lisp_Object Qpostscript;

static void x_disable_image(struct frame *, struct image *);
static void x_edge_detection(struct frame *, struct image *, Lisp_Object,
                             Lisp_Object);

static void init_color_table(void);
static unsigned long lookup_rgb_color(struct frame *, int, int, int);
#ifdef COLOR_TABLE_SUPPORT
static void free_color_table(void);
static unsigned long *colors_in_color_table(int *);
#endif /* COLOR_TABLE_SUPPORT */

/* where are these prototypes actually supposed to be? */
extern RGB_PIXEL_COLOR image_background(struct image *, struct frame *,
                                        XImagePtr_or_DC);
extern int image_background_transparent(struct image *, struct frame *,
                                        XImagePtr_or_DC);

#if defined(MAC_OS) || defined(HAVE_CARBON)
extern void init_image(void);
#endif /* MAC_OS || HAVE_CARBON */

static Lisp_Object QCmax_width, QCmax_height;

/* Code to deal with bitmaps.  Bitmaps are referenced by their bitmap
   id, which is just an int that this section returns.  Bitmaps are
   reference counted so they can be shared among frames.

   Bitmap indices are guaranteed to be > 0, so a negative number can
   be used to indicate no bitmap.

   If you use x_create_bitmap_from_data, then you must keep track of
   the bitmaps yourself.  That is, creating a bitmap from the same
   data more than once will not be caught.  */
#if defined(MAC_OS) || \
    (defined(HAVE_CARBON) && !defined(HAVE_NS) && (!defined(__LP64__) || !__LP64__))
static XImagePtr ATTRIBUTE_USED
XGetImage(Display *display, Pixmap pixmap, int x, int y,
          unsigned int width, unsigned int height,
          unsigned long plane_mask, int format)
{
# pragma unused (display, x, y, width, height, plane_mask, format)
# if defined(GLYPH_DEBUG) && GLYPH_DEBUG
  xassert((x == 0) && (y == 0));
  {
    Rect ri, rp;
    SetRect(&ri, 0, 0, width, height);
    xassert(EqualRect(&ri, GetPixBounds(GetGWorldPixMap(pixmap), &rp)));
  }
  xassert(!(pixelsLocked & GetPixelsState(GetGWorldPixMap(pixmap))));
# endif /* GLYPH_DEBUG */

  LockPixels(GetGWorldPixMap(pixmap));

  return pixmap;
}

static void XPutPixel(XImagePtr ximage, int x, int y, unsigned long pixel)
{
  PixMapHandle pixmap = GetGWorldPixMap(ximage);
  short depth = GetPixDepth(pixmap);

# if defined(WORDS_BIG_ENDIAN) || !USE_CG_DRAWING
  if (depth == 32) {
    char *base_addr = GetPixBaseAddr(pixmap);
    short row_bytes = GetPixRowBytes(pixmap);

    ((unsigned long *)(base_addr + y * row_bytes))[x] = (0xff000000 | pixel);
  } else
# endif /* WORDS_BIG_ENDIAN || !USE_CG_DRAWING */
    if (depth == 1) {
      char *base_addr = GetPixBaseAddr(pixmap);
      short row_bytes = GetPixRowBytes(pixmap);

      if (pixel == PIX_MASK_DRAW)
	base_addr[(y * row_bytes) + (x / 8)] |= ((1 << 7) >> (x & 7));
      else
	base_addr[(y * row_bytes) + (x / 8)] &= (~((1 << 7) >> (x & 7)));
    } else {
      CGrafPtr old_port;
      GDHandle old_gdh;
      RGBColor color;

      GetGWorld(&old_port, &old_gdh);
      SetGWorld(ximage, NULL);

      color.red = RED16_FROM_ULONG(pixel);
      color.green = GREEN16_FROM_ULONG(pixel);
      color.blue = BLUE16_FROM_ULONG(pixel);

      SetCPixel(x, y, &color);

      SetGWorld(old_port, old_gdh);
    }
}

static unsigned long XGetPixel(XImagePtr ximage, int x, int y)
{
  PixMapHandle pixmap = GetGWorldPixMap(ximage);
  short depth = GetPixDepth(pixmap);

# if defined(WORDS_BIG_ENDIAN) || !USE_CG_DRAWING
  if (depth == 32) {
    char *base_addr = GetPixBaseAddr(pixmap);
    short row_bytes = GetPixRowBytes(pixmap);

    return ((unsigned long *)(base_addr + y * row_bytes))[x] & 0x00ffffff;
  } else
# endif /* WORDS_BIG_ENDIAN || !USE_CG_DRAWING */
    if (depth == 1) {
      char *base_addr = GetPixBaseAddr(pixmap);
      short row_bytes = GetPixRowBytes(pixmap);

      if (base_addr[(y * row_bytes) + (x / 8)] & (1 << (~x & 7)))
	return PIX_MASK_DRAW;
      else
	return PIX_MASK_RETAIN;
    } else {
      CGrafPtr old_port;
      GDHandle old_gdh;
      RGBColor color;

      GetGWorld(&old_port, &old_gdh);
      SetGWorld(ximage, NULL);

      GetCPixel(x, y, &color);

      SetGWorld(old_port, old_gdh);
      return RGB_TO_ULONG((color.red >> 8), (color.green >> 8),
                          (color.blue >> 8));
    }
}

/* */
static void ATTRIBUTE_USED
XDestroyImage(XImagePtr ximg)
{
  UnlockPixels(GetGWorldPixMap(ximg));
}

# if USE_CG_DRAWING
static CGImageRef ATTRIBUTE_USED
mac_create_cg_image_from_image(struct frame *f, struct image *img)
{
  Pixmap mask;
  CGImageRef result = NULL;

  BLOCK_INPUT;
  if (img->mask)
    mask = img->mask;
  else {
    mask = XCreatePixmap(FRAME_X_DISPLAY(f), FRAME_X_WINDOW(f),
                         img->width, img->height, 1);
    if (mask) {
      CGrafPtr old_port;
      GDHandle old_gdh;
      Rect r;

      GetGWorld(&old_port, &old_gdh);
      SetGWorld(mask, NULL);
      BackColor(blackColor); /* Do NOT mask.  */
      SetRect(&r, 0, 0, img->width, img->height);
      EraseRect(&r);
      SetGWorld(old_port, old_gdh);
    }
  }
  if (mask) {
    CreateCGImageFromPixMaps(GetGWorldPixMap(img->pixmap),
                             GetGWorldPixMap(mask), &result);
    if (mask != img->mask)
      XFreePixmap(FRAME_X_DISPLAY(f), mask);
  }
  UNBLOCK_INPUT;

  return result;
}
# endif /* USE_CG_DRAWING */
#else
# if defined(MAC_OS) || \
     (defined(HAVE_CARBON) && !defined(HAVE_NS) && (defined(__LP64__) && __LP64__))
static void XPutPixel(XImagePtr ximage, int x, int y, unsigned long pixel)
{
  short depth = 0;

  if (depth == 1) {
      Ptr base_addr = (Ptr)NULL;
      short row_bytes = 0;

      if (pixel == PIX_MASK_DRAW)
	base_addr[y * row_bytes + x / 8] |= ((1 << 7) >> (x & 7));
      else
	base_addr[y * row_bytes + x / 8] &= (~((1 << 7) >> (x & 7)));
  } else {
      RGBColor color;

      color.red = RED16_FROM_ULONG(pixel);
      color.green = GREEN16_FROM_ULONG(pixel);
      color.blue = BLUE16_FROM_ULONG(pixel);

      IF_LINT((void)color);
  }

  return;
}

static unsigned long XGetPixel(XImagePtr ximage, int x, int y)
{
  short depth = 0;

  if (depth == 1) {
      Ptr base_addr = (Ptr)NULL;
      short row_bytes = 0;

      if (base_addr[(y * row_bytes) + (x / 8)] & (1 << (~x & 7)))
	return PIX_MASK_DRAW;
      else
	return PIX_MASK_RETAIN;
  } else {
      RGBColor color = { 0U, 0U, 0U };

      return RGB_TO_ULONG((color.red >> 8), (color.green >> 8),
                          (color.blue >> 8));
  }
}
# endif /* MAC_OS || (HAVE_CARBON && !HAVE_NS && __LP64__) */
#endif /* MAC_OS || (HAVE_CARBON && !HAVE_NS && !__LP64__)  */

#ifdef HAVE_NS
/* Use with images created by ns_image_for_XPM: */
static unsigned long XGetPixel(XImagePtr ximage, int x, int y)
{
  return ns_get_pixel(ximage, x, y);
}

/* Use with images created by ns_image_for_XPM; alpha set to 1;
   pixel is assumed to be in RGB form.  */
static void XPutPixel(XImagePtr ximage, int x, int y, unsigned long pixel)
{
  ns_put_pixel(ximage, x, y, pixel);
}
#endif /* HAVE_NS */


/* Functions to access the contents of a bitmap, given an id: */
static int
x_bitmap_height(struct frame *f, ptrdiff_t id)
{
  return FRAME_DISPLAY_INFO(f)->bitmaps[id - 1].height;
}

static int
x_bitmap_width(struct frame *f, ptrdiff_t id)
{
  return FRAME_DISPLAY_INFO(f)->bitmaps[id - 1].width;
}

#if defined(HAVE_X_WINDOWS) || defined(HAVE_NTGUI)
ptrdiff_t
x_bitmap_pixmap(struct frame *f, ptrdiff_t id)
{
  /* HAVE_NTGUI needs the explicit cast here: */
  return (ptrdiff_t)FRAME_DISPLAY_INFO(f)->bitmaps[id - 1].pixmap;
}
#endif /* HAVE_X_WINDOWS || HAVE_NTGUI */

#ifdef HAVE_X_WINDOWS
int
x_bitmap_mask(struct frame *f, ptrdiff_t id)
{
  return FRAME_DISPLAY_INFO(f)->bitmaps[id - 1].mask;
}
#endif /* HAVE_X_WINDOWS || HAVE_NTGUI */

/* Allocate a new bitmap record.  Returns index of new record: */
static ptrdiff_t x_allocate_bitmap_record(struct frame *f)
{
  Display_Info *dpyinfo = FRAME_DISPLAY_INFO (f);
  ptrdiff_t i;

  if (dpyinfo->bitmaps_last < dpyinfo->bitmaps_size)
    return ++dpyinfo->bitmaps_last;

  for (i = 0; i < dpyinfo->bitmaps_size; ++i)
    if (dpyinfo->bitmaps[i].refcount == 0)
      return (i + 1);

  dpyinfo->bitmaps =
    xpalloc (dpyinfo->bitmaps, (ptrdiff_t *)&dpyinfo->bitmaps_size,
	     10, -1, sizeof(*dpyinfo->bitmaps));
  return ++dpyinfo->bitmaps_last;
}

/* Add one reference to the reference count of the bitmap with id ID: */
void x_reference_bitmap(struct frame *f, ptrdiff_t id)
{
  ++FRAME_DISPLAY_INFO(f)->bitmaps[id - 1].refcount;
}

/* Create a bitmap for frame F from a HEIGHT x WIDTH array of bits at BITS: */
ptrdiff_t
x_create_bitmap_from_data(struct frame *f, char *bits, unsigned int width,
                          unsigned int height)
{
  Display_Info *dpyinfo = FRAME_DISPLAY_INFO(f);
  ptrdiff_t id;

#ifdef HAVE_X_WINDOWS
  Pixmap bitmap;
  bitmap = XCreateBitmapFromData(FRAME_X_DISPLAY(f), FRAME_X_WINDOW(f),
				 bits, width, height);
  if (! bitmap)
    return -1;
#endif /* HAVE_X_WINDOWS */

#ifdef HAVE_NTGUI
  Pixmap bitmap;
  bitmap = CreateBitmap(width, height,
                        FRAME_DISPLAY_INFO(XFRAME(frame))->n_planes,
                        FRAME_DISPLAY_INFO(XFRAME(frame))->n_cbits, bits);
  if (! bitmap)
    return -1;
#endif /* HAVE_NTGUI */

#ifdef MAC_OS
  /* MAC_TODO: for now fail if width is not mod 16 (toolbox requires it) */
  if ((width % 16) != 0)
    return -1;
#endif /* MAC_OS */

#ifdef HAVE_NS
  void *bitmap = ns_image_from_XBM((unsigned char *)bits, width, height);
  if (!bitmap)
      return -1;
#endif /* HAVE_NS */

  id = x_allocate_bitmap_record(f);
#ifdef MAC_OS
  dpyinfo->bitmaps[id - 1].bitmap_data = (char *)xmalloc(height * width);
  bcopy(bits, dpyinfo->bitmaps[id - 1].bitmap_data, (height * width));
#endif  /* MAC_OS */

#ifdef HAVE_NS
  dpyinfo->bitmaps[id - 1].img = bitmap;
  dpyinfo->bitmaps[id - 1].depth = 1;
#endif /* HAVE_NS */

  dpyinfo->bitmaps[id - 1].file = NULL;
  dpyinfo->bitmaps[id - 1].height = height;
  dpyinfo->bitmaps[id - 1].width = width;
  dpyinfo->bitmaps[id - 1].refcount = 1;

#ifdef HAVE_X_WINDOWS
  dpyinfo->bitmaps[id - 1].pixmap = bitmap;
  dpyinfo->bitmaps[id - 1].have_mask = false;
  dpyinfo->bitmaps[id - 1].depth = 1;
#endif /* HAVE_X_WINDOWS */

#ifdef HAVE_NTGUI
  dpyinfo->bitmaps[id - 1].pixmap = bitmap;
  dpyinfo->bitmaps[id - 1].hinst = NULL;
  dpyinfo->bitmaps[id - 1].depth = 1;
#endif /* HAVE_NTGUI */

  return id;
}

/* Create bitmap from file FILE for frame F: */
ptrdiff_t x_create_bitmap_from_file(struct frame *f, Lisp_Object file)
{
  Display_Info *dpyinfo = FRAME_DISPLAY_INFO(f);

#ifdef MAC_OS
# pragma unused (dpyinfo)
  return -1;  /* MAC_TODO : bitmap support */
#endif  /* MAC_OS */

#ifdef HAVE_NTGUI
  return -1;  /* W32_TODO : bitmap support */
#endif /* HAVE_NTGUI */

#ifdef HAVE_NS
  ptrdiff_t id;
  void *bitmap = ns_image_from_file (file);

  if (!bitmap) {
    return -1;
  }

  id = x_allocate_bitmap_record(f);
  dpyinfo->bitmaps[id - 1].img = bitmap;
  dpyinfo->bitmaps[id - 1].refcount = 1;
  dpyinfo->bitmaps[id - 1].file = xlispstrdup(file);
  dpyinfo->bitmaps[id - 1].depth = 1;
  dpyinfo->bitmaps[id - 1].height = ns_image_width(bitmap);
  dpyinfo->bitmaps[id - 1].width = ns_image_height(bitmap);
  return id;
#else
# if defined(MAC_OS) || defined(HAVE_CARBON)
  IF_LINT((void)dpyinfo);
  IF_LINT((void)fprintf(stderr, "bitmap support is unimplemented.\n"));
  return -1;
# endif /* MAC_OS || HAVE_CARBON */
#endif /* HAVE_NS */

#ifdef HAVE_X_WINDOWS
  unsigned int width, height;
  Pixmap bitmap;
  int xhot, yhot, result;
  ptrdiff_t id;
  Lisp_Object found;
  char *filename;

  /* Look for an existing bitmap with the same name.  */
  for (id = 0; id < dpyinfo->bitmaps_last; ++id)
    {
      if (dpyinfo->bitmaps[id].refcount
	  && dpyinfo->bitmaps[id].file
	  && !strcmp (dpyinfo->bitmaps[id].file, SSDATA (file)))
	{
	  ++dpyinfo->bitmaps[id].refcount;
	  return (id + 1);
	}
    }

  /* Search bitmap-file-path for the file, if appropriate.  */
  if (openp (Vx_bitmap_file_path, file, Qnil, &found,
	     make_number (R_OK), false)
      < 0)
    return -1;

  filename = SSDATA (found);

  result = XReadBitmapFile (FRAME_X_DISPLAY (f), FRAME_X_WINDOW (f),
			    filename, &width, &height, &bitmap, &xhot, &yhot);
  if (result != BitmapSuccess)
    return -1;

  id = x_allocate_bitmap_record (f);
  dpyinfo->bitmaps[id - 1].pixmap = bitmap;
  dpyinfo->bitmaps[id - 1].have_mask = false;
  dpyinfo->bitmaps[id - 1].refcount = 1;
  dpyinfo->bitmaps[id - 1].file = xlispstrdup(file);
  dpyinfo->bitmaps[id - 1].depth = 1;
  dpyinfo->bitmaps[id - 1].height = height;
  dpyinfo->bitmaps[id - 1].width = width;

  return id;
#endif /* HAVE_X_WINDOWS */
}

/* Free bitmap B: */
static void
free_bitmap_record(Display_Info *dpyinfo, Bitmap_Record *bm)
{
#ifdef HAVE_X_WINDOWS
  XFreePixmap(dpyinfo->display, bm->pixmap);
  if (bm->have_mask)
    XFreePixmap(dpyinfo->display, bm->mask);
#endif /* HAVE_X_WINDOWS */

#ifdef HAVE_NTGUI
  DeleteObject(bm->pixmap);
#endif /* HAVE_NTGUI */

#ifdef MAC_OS
  xfree(bm->bitmap_data);  /* Added ++kfs */
  bm->bitmap_data = NULL;
#endif  /* MAC_OS */

#ifdef HAVE_NS
  ns_release_object(bm->img);
#endif /* HAVE_NS */

  if (bm->file)
    {
      xfree(bm->file);
      bm->file = NULL;
    }
}

/* Remove reference to bitmap with id number ID: */
void x_destroy_bitmap(struct frame *f, ptrdiff_t id)
{
  Display_Info *dpyinfo = FRAME_DISPLAY_INFO(f);

  if (id > 0)
    {
      Bitmap_Record *bm = &dpyinfo->bitmaps[id - 1];

      if (--bm->refcount == 0)
	{
	  block_input ();
	  free_bitmap_record (dpyinfo, bm);
	  unblock_input ();
	}
    }
}

/* Free all the bitmaps for the display specified by DPYINFO.  */

void
x_destroy_all_bitmaps (Display_Info *dpyinfo)
{
  ptrdiff_t i;
  Bitmap_Record *bm = dpyinfo->bitmaps;

  for (i = 0; i < dpyinfo->bitmaps_last; i++, bm++)
    if (bm->refcount > 0)
      free_bitmap_record (dpyinfo, bm);

  dpyinfo->bitmaps_last = 0;
}

static bool x_create_x_image_and_pixmap (struct frame *, int, int, int,
					 XImagePtr *, Pixmap *);
static void x_destroy_x_image (XImagePtr ximg);

#ifdef HAVE_NTGUI
static XImagePtr_or_DC image_get_x_image_or_dc (struct frame *, struct image *,
						bool, HGDIOBJ *);
static void image_unget_x_image_or_dc (struct image *, bool, XImagePtr_or_DC,
				       HGDIOBJ);
#else
static XImagePtr image_get_x_image (struct frame *, struct image *, bool);
static void image_unget_x_image (struct image *, bool, XImagePtr);
# define image_get_x_image_or_dc(f, img, mask_p, dummy)	\
   image_get_x_image (f, img, mask_p)
# define image_unget_x_image_or_dc(img, mask_p, ximg, dummy)	\
   image_unget_x_image (img, mask_p, ximg)
#endif /* HAVE_NTGUI */

#ifdef HAVE_X_WINDOWS

static void image_sync_to_pixmaps(struct frame *, struct image *);

/* Useful functions defined in the section
   `Image type independent image structures' below. */

static unsigned long four_corners_best(XImagePtr ximg, int *corners,
                                       unsigned long width,
                                       unsigned long height);


/* Create a mask of a bitmap. Note is this not a perfect mask.
   It is nicer with some borders in this context */
void x_create_bitmap_mask(struct frame *f, ptrdiff_t id)
{
  Pixmap pixmap, mask;
  XImagePtr ximg, mask_img;
  unsigned long width, height;
  bool result;
  unsigned long bg;
  unsigned long x, y, xp, xm, yp, ym;
  GC gc;

  Display_Info *dpyinfo = FRAME_DISPLAY_INFO(f);

  if (!(id > 0))
    return;

  pixmap = x_bitmap_pixmap(f, id);
  width = x_bitmap_width(f, id);
  height = x_bitmap_height(f, id);

  block_input();
  ximg = XGetImage(FRAME_X_DISPLAY(f), pixmap, 0, 0, width, height,
		   ~0, ZPixmap);

  if (!ximg)
    {
      unblock_input();
      return;
    }

  result = x_create_x_image_and_pixmap(f, width, height, 1, &mask_img, &mask);

  unblock_input();
  if (!result)
    {
      XDestroyImage(ximg);
      return;
    }

  bg = four_corners_best(ximg, NULL, width, height);

  for (y = 0; y < ximg->height; ++y)
    {
      for (x = 0; x < ximg->width; ++x)
	{
	  xp = ((x != (ximg->width - 1)) ? (x + 1) : 0);
	  xm = ((x != 0) ? (x - 1) : (ximg->width - 1));
	  yp = ((y != (ximg->height - 1)) ? (y + 1) : 0);
	  ym = ((y != 0) ? (y - 1) : (ximg->height - 1));
	  if ((XGetPixel(ximg, x, y) == bg)
              && (XGetPixel(ximg, x, yp) == bg)
	      && (XGetPixel(ximg, x, ym) == bg)
	      && (XGetPixel(ximg, xp, y) == bg)
	      && (XGetPixel(ximg, xp, yp) == bg)
	      && (XGetPixel(ximg, xp, ym) == bg)
	      && (XGetPixel(ximg, xm, y) == bg)
	      && (XGetPixel(ximg, xm, yp) == bg)
	      && (XGetPixel(ximg, xm, ym) == bg))
	    XPutPixel(mask_img, x, y, 0);
	  else
	    XPutPixel(mask_img, x, y, 1);
	}
    }

  eassert(input_blocked_p());
  gc = XCreateGC(FRAME_X_DISPLAY(f), mask, 0, NULL);
  XPutImage(FRAME_X_DISPLAY(f), mask, gc, mask_img, 0, 0, 0, 0,
	    width, height);
  XFreeGC(FRAME_X_DISPLAY(f), gc);

  dpyinfo->bitmaps[id - 1].have_mask = true;
  dpyinfo->bitmaps[id - 1].mask = mask;

  XDestroyImage(ximg);
  x_destroy_x_image(mask_img);
}

#else

/* dummy function to use the x_bitmap_width() and x_bitmap_height()
 * functions: */
extern unsigned long x_width_by_height(struct frame *f, ptrdiff_t id);

unsigned long
x_width_by_height(struct frame *f, ptrdiff_t id)
{
  unsigned long width, height;

  if (!(id > 0))
    return 0UL;

  width = x_bitmap_width(f, id);
  height = x_bitmap_height(f, id);

  return (width * height);
}

#endif /* HAVE_X_WINDOWS */


/***********************************************************************
			    Image types
 ***********************************************************************/

/* List of supported image types.  Use define_image_type to add new
   types.  Use lookup_image_type to find a type for a given symbol.  */
static struct image_type *image_types;

/* The symbol `xbm' which is used as the type symbol for XBM images: */
static Lisp_Object Qxbm;

/* Keywords: */
Lisp_Object QCascent, QCmargin, QCrelief;
Lisp_Object QCconversion;
static Lisp_Object QCheuristic_mask;
static Lisp_Object QCcolor_symbols;
static Lisp_Object QCindex, QCmatrix, QCcolor_adjustment, QCmask, QCgeometry;
static Lisp_Object QCcrop, QCrotation;

/* Other symbols: */
static Lisp_Object Qcount, Qextension_data, Qdelay;
static Lisp_Object Qlaplace, Qemboss, Qedge_detection, Qheuristic;

/* Forward function prototypes: */
static struct image_type *lookup_image_type (Lisp_Object);
static void x_laplace (struct frame *, struct image *);
static void x_emboss (struct frame *, struct image *);
static void x_build_heuristic_mask (struct frame *, struct image *,
                                    Lisp_Object);
#ifdef WINDOWSNT
# define CACHE_IMAGE_TYPE(type, status) \
   do { Vlibrary_cache = Fcons (Fcons (type, status), Vlibrary_cache); } while (0)
#else
# define CACHE_IMAGE_TYPE(type, status)
#endif /* WINDOWSNT */

#define ADD_IMAGE_TYPE(type) \
  do { Vimage_types = Fcons (type, Vimage_types); } while (0)

/* Define a new image type from TYPE.  This adds a copy of TYPE to
   image_types and caches the loading status of TYPE.  */
static struct image_type *define_image_type(struct image_type *type)
{
  struct image_type *p = NULL;
  Lisp_Object target_type = *type->type;
  bool type_valid = 1;

  block_input();

  for (p = image_types; p; p = p->next)
    if (EQ(*p->type, target_type))
      goto done;

  if (type->init)
    {
#if defined HAVE_NTGUI && defined WINDOWSNT
      /* If we failed to load the library before, don't try again.  */
      Lisp_Object tested = Fassq(target_type, Vlibrary_cache);
      if (CONSP(tested) && NILP(XCDR(tested)))
	type_valid = 0;
      else
#endif
	{
	  type_valid = type->init();
	  CACHE_IMAGE_TYPE(target_type, (type_valid ? Qt : Qnil));
	}
    }

  if (type_valid)
    {
      /* Make a copy of TYPE to avoid a bus error in a dumped Emacs.
         The initialized data segment is read-only.  */
      p = xmalloc(sizeof(*p));
      *p = *type;
      p->next = image_types;
      image_types = p;
    }

 done:
  unblock_input();
  return p;
}


/* Value is true if OBJECT is a valid Lisp image specification.  A
   valid image specification is a list whose car is the symbol
   `image', and whose rest is a property list.  The property list must
   contain a value for key `:type'.  That value must be the name of a
   supported image type.  The rest of the property list depends on the
   image type.  */
bool valid_image_p(Lisp_Object object)
{
  bool valid_p = 0;

  if (IMAGEP (object))
    {
      Lisp_Object tem;

      for (tem = XCDR (object); CONSP (tem); tem = XCDR (tem))
	if (EQ (XCAR (tem), QCtype))
	  {
	    tem = XCDR (tem);
	    if (CONSP (tem) && SYMBOLP (XCAR (tem)))
	      {
		struct image_type *type;
		type = lookup_image_type (XCAR (tem));
		if (type)
		  valid_p = type->valid_p (object);
	      }

	    break;
	  }
    }

  return valid_p;
}


/* Log error message with format string FORMAT and argument ARG.
   Signaling an error, e.g. when an image cannot be loaded, is not a
   good idea because this would interrupt redisplay, and the error
   message display would lead to another redisplay.  This function
   therefore simply displays a message.  */
static void
image_error(const char *format, Lisp_Object arg1, Lisp_Object arg2)
{
  add_to_log(format, arg1, arg2);
}



/***********************************************************************
			 Image specifications
 ***********************************************************************/

enum image_value_type
{
  IMAGE_DONT_CHECK_VALUE_TYPE,
  IMAGE_STRING_VALUE,
  IMAGE_STRING_OR_NIL_VALUE,
  IMAGE_SYMBOL_VALUE,
  IMAGE_POSITIVE_INTEGER_VALUE,
  IMAGE_NON_NEGATIVE_INTEGER_VALUE_OR_PAIR,
  IMAGE_NON_NEGATIVE_INTEGER_VALUE,
  IMAGE_ASCENT_VALUE,
  IMAGE_INTEGER_VALUE,
  IMAGE_FUNCTION_VALUE,
  IMAGE_NUMBER_VALUE,
  IMAGE_BOOL_VALUE
};

/* Structure used when parsing image specifications: */
struct image_keyword
{
  /* Name of keyword.  */
  const char *name;

  /* The type of value allowed.  */
  enum image_value_type type;

  /* True means key must be present.  */
  bool mandatory_p;

  /* Used to recognize duplicate keywords in a property list.  */
  int count;

  /* The value that was found.  */
  Lisp_Object value;
};


/* Parse image spec SPEC according to KEYWORDS.  A valid image spec
   has the format (image KEYWORD VALUE ...).  One of the keyword/
   value pairs must be `:type TYPE'.  KEYWORDS is a vector of
   image_keywords structures of size NKEYWORDS describing other
   allowed keyword/value pairs.  Value is true if SPEC is valid.  */
static bool
parse_image_spec (Lisp_Object spec, struct image_keyword *keywords,
		  int nkeywords, Lisp_Object type)
{
  int i;
  Lisp_Object plist;

  if (!IMAGEP (spec))
    return 0;

  plist = XCDR (spec);
  while (CONSP (plist))
    {
      Lisp_Object key, value;

      /* First element of a pair must be a symbol.  */
      key = XCAR (plist);
      plist = XCDR (plist);
      if (!SYMBOLP (key))
	return 0;

      /* There must follow a value.  */
      if (!CONSP (plist))
	return 0;
      value = XCAR (plist);
      plist = XCDR (plist);

      /* Find key in KEYWORDS.  Error if not found.  */
      for (i = 0; i < nkeywords; ++i)
	if (strcmp (keywords[i].name, SSDATA (SYMBOL_NAME (key))) == 0)
	  break;

      if (i == nkeywords)
	continue;

      /* Record that we recognized the keyword.  If a keywords
	 was found more than once, it's an error.  */
      keywords[i].value = value;
      if (keywords[i].count > 1)
	return 0;
      ++keywords[i].count;

      /* Check type of value against allowed type: */
      switch (keywords[i].type)
	{
	case IMAGE_STRING_VALUE:
	  if (!STRINGP(value))
	    return 0;
	  break;
	case IMAGE_STRING_OR_NIL_VALUE:
	  if (!STRINGP(value) && !NILP(value))
	    return 0;
	  break;
	case IMAGE_SYMBOL_VALUE:
	  if (!SYMBOLP(value))
	    return 0;
	  break;
	case IMAGE_POSITIVE_INTEGER_VALUE:
	  if (! RANGED_INTEGERP(1, value, INT_MAX))
	    return 0;
	  break;
	case IMAGE_NON_NEGATIVE_INTEGER_VALUE_OR_PAIR:
	  if (RANGED_INTEGERP(0, value, INT_MAX))
	    break;
	  if (CONSP(value) && RANGED_INTEGERP(0, XCAR(value), INT_MAX)
	      && RANGED_INTEGERP(0, XCDR(value), INT_MAX))
	    break;
	  return 0;
	case IMAGE_ASCENT_VALUE:
	  if (SYMBOLP(value) && EQ(value, Qcenter))
	    break;
	  else if (RANGED_INTEGERP(0, value, 100))
	    break;
	  return 0;
	case IMAGE_NON_NEGATIVE_INTEGER_VALUE:
	  /* Unlike the other integer-related cases, this one does not
	     verify that VALUE fits in 'int'.  This is because callers
	     want EMACS_INT.  */
	  if (!INTEGERP(value) || (XINT(value) < 0))
	    return 0;
	  break;
	case IMAGE_DONT_CHECK_VALUE_TYPE:
	  break;
	case IMAGE_FUNCTION_VALUE:
	  value = indirect_function(value);
	  if (!NILP(Ffunctionp(value)))
	    break;
	  return 0;
	case IMAGE_NUMBER_VALUE:
	  if (!INTEGERP(value) && !FLOATP(value))
	    return 0;
	  break;
	case IMAGE_INTEGER_VALUE:
	  if (!TYPE_RANGED_INTEGERP(int, value))
	    return 0;
	  break;
	case IMAGE_BOOL_VALUE:
	  if (!NILP(value) && !EQ(value, Qt))
	    return 0;
	  break;
	default:
	  emacs_abort();
	  break;
	}

      if (EQ(key, QCtype) && !EQ(type, value))
	return 0;
    }

  /* Check that all mandatory fields are present: */
  for (i = 0; i < nkeywords; ++i)
    if (keywords[i].mandatory_p && keywords[i].count == 0)
      return 0;

  return NILP (plist);
}


/* Return the value of KEY in image specification SPEC.  Value is nil
   if KEY is not present in SPEC.  Set *FOUND depending on whether KEY
   was found in SPEC.  */

static Lisp_Object
image_spec_value (Lisp_Object spec, Lisp_Object key, bool *found)
{
  Lisp_Object tail;

  eassert (valid_image_p (spec));

  for (tail = XCDR (spec);
       CONSP (tail) && CONSP (XCDR (tail));
       tail = XCDR (XCDR (tail)))
    {
      if (EQ (XCAR (tail), key))
	{
	  if (found)
	    *found = 1;
	  return XCAR (XCDR (tail));
	}
    }

  if (found)
    *found = 0;
  return Qnil;
}


DEFUN("image-size", Fimage_size, Simage_size, 1, 3, 0,
      doc: /* Return the size of image SPEC as pair (WIDTH . HEIGHT).
PIXELS non-nil means return the size in pixels, otherwise return the
size in canonical character units.
FRAME is the frame on which the image will be displayed.  FRAME nil
or omitted means use the selected frame.  */)
  (Lisp_Object spec, Lisp_Object pixels, Lisp_Object frame)
{
  Lisp_Object size;

  size = Qnil;
  if (valid_image_p(spec))
    {
      struct frame *f = decode_window_system_frame(frame);
      ptrdiff_t id = lookup_image(f, spec);
      struct image *img = IMAGE_FROM_ID(f, id);
      int width;
      int height;
      eassert(img != NULL);
      xassert(img != NULL);
      width = (img->width + (2 * img->hmargin));
      height = (img->height + (2 * img->vmargin));

      if (NILP (pixels))
	size = Fcons(make_float((double)width / FRAME_COLUMN_WIDTH(f)),
		     make_float((double)height / FRAME_LINE_HEIGHT(f)));
      else
	size = Fcons(make_number(width), make_number(height));
    }
  else
    {
      if (NILP(size)) {
        ; /* ??? */
      }
      error("Invalid image specification");
    }

  return size;
}


DEFUN ("image-mask-p", Fimage_mask_p, Simage_mask_p, 1, 2, 0,
       doc: /* Return t if image SPEC has a mask bitmap.
FRAME is the frame on which the image will be displayed.  FRAME nil
or omitted means use the selected frame.  */)
  (Lisp_Object spec, Lisp_Object frame)
{
  Lisp_Object mask;

  mask = Qnil;
  if (valid_image_p(spec))
    {
      struct frame *f = decode_window_system_frame(frame);
      ptrdiff_t id = lookup_image(f, spec);
      struct image *img = IMAGE_FROM_ID(f, id);
      if ((img != NULL) && img->mask)
	mask = Qt;
    }
  else
    error("Invalid image specification");

  return mask;
}

DEFUN ("image-metadata", Fimage_metadata, Simage_metadata, 1, 2, 0,
       doc: /* Return metadata for image SPEC.
FRAME is the frame on which the image will be displayed.  FRAME nil
or omitted means use the selected frame.  */)
  (Lisp_Object spec, Lisp_Object frame)
{
  Lisp_Object ext;

  ext = Qnil;
  if (valid_image_p(spec))
    {
      struct frame *f = decode_window_system_frame(frame);
      ptrdiff_t id = lookup_image(f, spec);
      struct image *img = IMAGE_FROM_ID(f, id);
      if (img != NULL)
	ext = img->lisp_data;
    }

  return ext;
}


/***********************************************************************
		 Image type independent image structures
 ***********************************************************************/

#define MAX_IMAGE_SIZE 10.0
/* Allocate and return a new image structure for image specification
   SPEC.  SPEC has a hash value of HASH.  */

static struct image *
make_image (Lisp_Object spec, EMACS_UINT hash)
{
  struct image *img = xzalloc (sizeof *img);
  Lisp_Object file = image_spec_value (spec, QCfile, NULL);

  eassert (valid_image_p (spec));
  img->dependencies = NILP (file) ? Qnil : list1 (file);
  img->type = lookup_image_type (image_spec_value (spec, QCtype, NULL));
  eassert (img->type != NULL);
  img->spec = spec;
  img->lisp_data = Qnil;
  img->ascent = DEFAULT_IMAGE_ASCENT;
  img->hash = hash;
  img->corners[BOT_CORNER] = -1;  /* Full image */
  return img;
}


/* Free image IMG which was used on frame F, including its resources.  */

static void
free_image (struct frame *f, struct image *img)
{
  if (img)
    {
      struct image_cache *c = FRAME_IMAGE_CACHE (f);

      /* Remove IMG from the hash table of its cache.  */
      if (img->prev)
	img->prev->next = img->next;
      else
	c->buckets[img->hash % IMAGE_CACHE_BUCKETS_SIZE] = img->next;

      if (img->next)
	img->next->prev = img->prev;

      c->images[img->id] = NULL;

      /* Free resources, then free IMG.  */
      img->type->free (f, img);
      xfree (img);
    }
}

/* Return true if the given widths and heights are valid for display.  */

static bool
check_image_size (struct frame *f, int width, int height)
{
  int w, h;

  if (width <= 0 || height <= 0)
    return 0;

  if (INTEGERP (Vmax_image_size))
    return (width <= XINT (Vmax_image_size)
	    && height <= XINT (Vmax_image_size));
  else if (FLOATP (Vmax_image_size))
    {
      if (f != NULL)
	{
	  w = FRAME_PIXEL_WIDTH (f);
	  h = FRAME_PIXEL_HEIGHT (f);
	}
      else
	w = h = 1024;  /* Arbitrary size for unknown frame. */
      return (width <= XFLOAT_DATA (Vmax_image_size) * w
	      && height <= XFLOAT_DATA (Vmax_image_size) * h);
    }
  else
    return 1;
}

/* Prepare image IMG for display on frame F.  Must be called before
   drawing an image.  */

void
prepare_image_for_display(struct frame *f, struct image *img)
{
  /* We are about to display IMG, so set its timestamp to `now': */
  img->timestamp = current_timespec();

  /* If IMG does NOT have a pixmap yet, load it now, using the image
     type dependent loader function.  */
  if (img->pixmap == NO_PIXMAP && !img->load_failed_p)
    img->load_failed_p = !img->type->load(f, img);

#if defined(MAC_OS) && (defined(USE_CG_DRAWING) && USE_CG_DRAWING)
  if (!img->load_failed_p && (img->data.ptr_val == NULL)) {
    img->data.ptr_val = mac_create_cg_image_from_image(f, img);
    if (img->data.ptr_val == NULL) {
      img->load_failed_p = 1;
      img->type->free(f, img);
    }
  }
#endif /* MAC_OS && USE_CG_DRAWING */

#ifdef HAVE_X_WINDOWS
  if (!img->load_failed_p)
    {
      block_input();
      image_sync_to_pixmaps(f, img);
      unblock_input();
    }
#endif /* HAVE_X_WINDOWS */
}


/* Value is the number of pixels for the ascent of image IMG when drawn
 * in face FACE.  */
int
image_ascent(struct image *img, struct face *face,
             struct glyph_slice *slice)
{
  int height;
  int ascent;

  if (slice->height == img->height)
    height = img->height + img->vmargin;
  else if (slice->y == 0)
    height = slice->height + img->vmargin;
  else
    height = slice->height;

  if (img->ascent == CENTERED_IMAGE_ASCENT)
    {
      if (face->font)
	{
#ifdef HAVE_NTGUI
	  /* W32 specific version.  Why?. ++kfs  */
	  ascent = height / 2 - (FONT_DESCENT (face->font)
				 - FONT_BASE (face->font)) / 2;
#else
	  /* This expression is arranged so that if the image can't be
	     exactly centered, it will be moved slightly up.  This is
	     because a typical font is `top-heavy' (due to the presence
	     uppercase letters), so the image placement should err towards
	     being top-heavy too.  It also just generally looks better.  */
	  ascent = ((height + FONT_BASE(face->font)
                     - FONT_DESCENT(face->font) + 1) / 2);
#endif /* HAVE_NTGUI */
	}
      else
	ascent = (height / 2);
    }
  else
    ascent = (height * (img->ascent / 100.0));

  return ascent;
}


/* Image background colors.  */

/* Find the "best" corner color of a bitmap.
 * On W32, XIMG is assumed to a device context with the bitmap selected: */
static RGB_PIXEL_COLOR
four_corners_best(XImagePtr_or_DC ximg, int *corners,
		  unsigned long width, unsigned long height)
{
  RGB_PIXEL_COLOR corner_pixels[4], best IF_LINT(= 0);
  int i, best_count;

  if (corners && (corners[BOT_CORNER] >= 0))
    {
      /* Get the colors at the corner_pixels of ximg: */
      corner_pixels[0] = GET_PIXEL(ximg, corners[LEFT_CORNER], corners[TOP_CORNER]);
      corner_pixels[1] = GET_PIXEL(ximg, (corners[RIGHT_CORNER] - 1), corners[TOP_CORNER]);
      corner_pixels[2] = GET_PIXEL(ximg, (corners[RIGHT_CORNER] - 1), (corners[BOT_CORNER] - 1));
      corner_pixels[3] = GET_PIXEL(ximg, corners[LEFT_CORNER], (corners[BOT_CORNER] - 1));
    }
  else
    {
      /* Get the colors at the corner_pixels of ximg: */
      corner_pixels[0] = GET_PIXEL(ximg, 0, 0);
      corner_pixels[1] = GET_PIXEL(ximg, (width - 1), 0);
      corner_pixels[2] = GET_PIXEL(ximg, (width - 1), (height - 1));
      corner_pixels[3] = GET_PIXEL(ximg, 0, (height - 1));
    }
  /* Choose the most frequently found color as background.  */
  for (i = best_count = 0; i < 4; ++i)
    {
      int j, n;

      for (j = n = 0; j < 4; ++j)
	if (corner_pixels[i] == corner_pixels[j])
	  ++n;

      if (n > best_count)
	best = corner_pixels[i], best_count = n;
    }

  return best;
}

/* Portability macros: */
#ifdef HAVE_NTGUI
# define Free_Pixmap(display, pixmap) \
   DeleteObject (pixmap)
#elif defined(HAVE_NS)
# define Free_Pixmap(display, pixmap) \
   ns_release_object(pixmap)
#else
# define Free_Pixmap(display, pixmap) \
   XFreePixmap(display, pixmap)
#endif /* !HAVE_NTGUI && !HAVE_NS */


/* Return the `background' field of IMG.  If IMG does NOT have one yet,
   it is guessed heuristically.  If non-zero, XIMG is an existing
   XImage object (or device context with the image selected on W32) to
   use for the heuristic.  */
RGB_PIXEL_COLOR
image_background(struct image *img, struct frame *f, XImagePtr_or_DC ximg)
{
  if (! img->background_valid)
    /* IMG does NOT have a background yet, so try to guess a reasonable
     * value here: */
    {
      bool free_ximg = !ximg;
#ifdef HAVE_NTGUI
      HGDIOBJ prev;
#endif /* HAVE_NTGUI */

      if (free_ximg)
	ximg = image_get_x_image_or_dc(f, img, 0, &prev);

      img->background = four_corners_best(ximg, img->corners, img->width, img->height);

      if (free_ximg)
	image_unget_x_image_or_dc(img, 0, ximg, prev);

      img->background_valid = 1;
    }

  return img->background;
}

/* Return the `background_transparent' field of IMG.  If IMG does NOT
   have one yet, it is guessed heuristically.  If non-zero, MASK is an
   existing XImage object to use for the heuristic.  */
int
image_background_transparent(struct image *img, struct frame *f, XImagePtr_or_DC mask)
{
  if (! img->background_transparent_valid)
    /* IMG does NOT have a background yet, so try to guess a reasonable
     * value here: */
    {
      if (img->mask)
	{
	  bool free_mask = !mask;
#ifdef HAVE_NTGUI
	  HGDIOBJ prev;
#endif /* HAVE_NTGUI */

	  if (free_mask)
	    mask = image_get_x_image_or_dc(f, img, 1, &prev);

	  img->background_transparent
	    = (four_corners_best(mask, img->corners, img->width, img->height) == PIX_MASK_RETAIN);

	  if (free_mask)
	    image_unget_x_image_or_dc(img, 1, mask, prev);
	}
      else
	img->background_transparent = 0;

      img->background_transparent_valid = 1;
    }

  return img->background_transparent;
}


/***********************************************************************
		  Helper functions for X image types
 ***********************************************************************/

/* Clear X resources of image IMG on frame F according to FLAGS.
   FLAGS is bitwise-or of the following masks:
   CLEAR_IMAGE_PIXMAP free the pixmap if any.
   CLEAR_IMAGE_MASK means clear the mask pixmap if any.
   CLEAR_IMAGE_COLORS means free colors allocated for the image, if
     any.  */

#define CLEAR_IMAGE_PIXMAP	(1 << 0)
#define CLEAR_IMAGE_MASK	(1 << 1)
#define CLEAR_IMAGE_COLORS	(1 << 2)

static void
x_clear_image_1(struct frame *f, struct image *img, int flags)
{
  if (flags & CLEAR_IMAGE_PIXMAP)
    {
      if (img->pixmap)
	{
	  Free_Pixmap(FRAME_X_DISPLAY(f), img->pixmap);
	  img->pixmap = NO_PIXMAP;
	  /* NOTE (HAVE_NS): background color is NOT an indexed color! */
	  img->background_valid = 0;
	}
#ifdef HAVE_X_WINDOWS
      if (img->ximg)
	{
	  x_destroy_x_image(img->ximg);
	  img->ximg = NULL;
	  img->background_valid = 0;
	}
#endif /* HAVE_X_WINDOWS */
    }

  if (flags & CLEAR_IMAGE_MASK)
    {
      if (img->mask)
	{
	  Free_Pixmap(FRAME_X_DISPLAY(f), img->mask);
	  img->mask = NO_PIXMAP;
	  img->background_transparent_valid = 0;
	}
#ifdef HAVE_X_WINDOWS
      if (img->mask_img)
	{
	  x_destroy_x_image (img->mask_img);
	  img->mask_img = NULL;
	  img->background_transparent_valid = 0;
	}
#endif /* HAVE_X_WINDOWS */
    }

  if ((flags & CLEAR_IMAGE_COLORS) && img->ncolors)
    {
      /* MAC_TODO: color table support.  */
      /* W32_TODO: color table support.  */
#ifdef HAVE_X_WINDOWS
      x_free_colors(f, img->colors, img->ncolors);
#endif /* HAVE_X_WINDOWS */
      xfree(img->colors);
      img->colors = NULL;
      img->ncolors = 0;
    }

#if defined(MAC_OS) && (defined(USE_CG_DRAWING) && USE_CG_DRAWING)
  if (img->data.ptr_val) {
    CGImageRelease(img->data.ptr_val);
    img->data.ptr_val = NULL;
  }
#endif /* MAC_OS && USE_CG_DRAWING */
}

/* Free X resources of image IMG which is used on frame F: */
static void
x_clear_image(struct frame *f, struct image *img)
{
  block_input();
  x_clear_image_1(f, img,
		  (CLEAR_IMAGE_PIXMAP | CLEAR_IMAGE_MASK
                   | CLEAR_IMAGE_COLORS));
  unblock_input();
}


/* Allocate color COLOR_NAME for image IMG on frame F.  If color
   cannot be allocated, use DFLT.  Add a newly allocated color to
   IMG->colors, so that it can be freed again.  Value is the pixel
   color.  */

static unsigned long
x_alloc_image_color (struct frame *f, struct image *img, Lisp_Object color_name,
		     unsigned long dflt)
{
  XColor color;
  unsigned long result;

  eassert (STRINGP (color_name));

  if (x_defined_color (f, SSDATA (color_name), &color, 1)
      && img->ncolors < min (min (PTRDIFF_MAX, SIZE_MAX) / sizeof *img->colors,
			     INT_MAX))
    {
      /* This is NOT called frequently so we get away with simply
	 reallocating the color vector to the needed size, here.  */
      ptrdiff_t ncolors = img->ncolors + 1;
      img->colors = xrealloc (img->colors, ncolors * sizeof *img->colors);
      img->colors[ncolors - 1] = color.pixel;
      img->ncolors = ncolors;
      result = color.pixel;
    }
  else
    result = dflt;

  return result;
}



/***********************************************************************
			     Image Cache
 ***********************************************************************/

static void cache_image (struct frame *f, struct image *img);

/* Return a new, initialized image cache that is allocated from the
   heap.  Call free_image_cache to free an image cache.  */

struct image_cache *
make_image_cache (void)
{
  struct image_cache *c = xmalloc (sizeof *c);

  c->size = 50;
  c->used = c->refcount = 0;
  c->images = xmalloc (c->size * sizeof *c->images);
  c->buckets = xzalloc (IMAGE_CACHE_BUCKETS_SIZE * sizeof *c->buckets);
  return c;
}


/* Find an image matching SPEC in the cache, and return it.  If no
   image is found, return NULL.  */
static struct image *
search_image_cache (struct frame *f, Lisp_Object spec, EMACS_UINT hash)
{
  struct image *img;
  struct image_cache *c = FRAME_IMAGE_CACHE (f);
  int i = (hash % IMAGE_CACHE_BUCKETS_SIZE);

  if (!c) return NULL;

  /* If the image spec does not specify a background color, the cached
     image must have the same background color as the current frame.
     The foreground color must also match, for the sake of monochrome
     images.

     In fact, we could ignore the foreground color matching condition
     for color images, or if the image spec specifies :foreground;
     similarly we could ignore the background color matching condition
     for formats that don't use transparency (such as jpeg), or if the
     image spec specifies :background.  However, the extra memory
     usage is probably negligible in practice, so we don't bother.  */

  for (img = c->buckets[i]; img; img = img->next)
    if (img->hash == hash
	&& !NILP (Fequal (img->spec, spec))
	&& img->frame_foreground == FRAME_FOREGROUND_PIXEL (f)
	&& img->frame_background == FRAME_BACKGROUND_PIXEL (f))
      break;
  return img;
}


/* Search frame F for an image with spec SPEC, and free it.  */

static void
uncache_image (struct frame *f, Lisp_Object spec)
{
  struct image *img = search_image_cache (f, spec, sxhash (spec, 0));
  if (img)
    {
      free_image (f, img);
      /* As display glyphs may still be referring to the image ID, we
	 must garbage the frame (Bug#6426).  */
      SET_FRAME_GARBAGED (f);
    }
}


/* Free image cache of frame F.  Be aware that X frames share images
   caches.  */

void
free_image_cache (struct frame *f)
{
  struct image_cache *c = FRAME_IMAGE_CACHE (f);
  if (c)
    {
      ptrdiff_t i;

      /* Cache should not be referenced by any frame when freed.  */
      eassert (c->refcount == 0);

      for (i = 0; i < c->used; ++i)
	free_image (f, c->images[i]);
      xfree (c->images);
      xfree (c->buckets);
      xfree (c);
      FRAME_IMAGE_CACHE (f) = NULL;
    }
}


/* Clear image cache of frame F.  FILTER=t means free all images.
   FILTER=nil means clear only images that haven't been
   displayed for some time.
   Else, only free the images which have FILTER in their `dependencies'.
   Should be called from time to time to reduce the number of loaded images.
   If image-cache-eviction-delay is non-nil, this frees images in the cache
   which weren't displayed for at least that many seconds.  */

static void
clear_image_cache (struct frame *f, Lisp_Object filter)
{
  struct image_cache *c = FRAME_IMAGE_CACHE (f);

  if (c)
    {
      ptrdiff_t i, nfreed = 0;

      /* Block input so that we won't be interrupted by a SIGIO
	 while being in an inconsistent state.  */
      block_input ();

      if (!NILP (filter))
	{
	  /* Filter image cache.  */
	  for (i = 0; i < c->used; ++i)
	    {
	      struct image *img = c->images[i];
	      if (img && (EQ (Qt, filter)
			  || !NILP (Fmember (filter, img->dependencies))))
		{
		  free_image (f, img);
		  ++nfreed;
		}
	    }
	}
      else if (INTEGERP (Vimage_cache_eviction_delay))
	{
	  /* Free cache based on timestamp.  */
	  struct timespec old, t;
	  double delay;
	  ptrdiff_t nimages = 0;

	  for (i = 0; i < c->used; ++i)
	    if (c->images[i])
	      nimages++;

	  /* If the number of cached images has grown unusually large,
	     decrease the cache eviction delay (Bug#6230).  */
	  delay = XINT (Vimage_cache_eviction_delay);
	  if (nimages > 40)
	    delay = 1600 * delay / nimages / nimages;
	  delay = max (delay, 1);

	  t = current_timespec ();
	  old = timespec_sub (t, dtotimespec (delay));

	  for (i = 0; i < c->used; ++i)
	    {
	      struct image *img = c->images[i];
	      if (img && timespec_cmp (img->timestamp, old) < 0)
		{
		  free_image (f, img);
		  ++nfreed;
		}
	    }
	}

      /* We may be clearing the image cache because, for example,
	 Emacs was iconified for a longer period of time.  In that
	 case, current matrices may still contain references to
	 images freed above.  So, clear these matrices.  */
      if (nfreed)
	{
	  Lisp_Object tail, frame;

	  FOR_EACH_FRAME (tail, frame)
	    {
	      struct frame *fr = XFRAME (frame);
	      if (FRAME_IMAGE_CACHE (fr) == c)
		clear_current_matrices (fr);
	    }

	  windows_or_buffers_changed = 19;
	}

      unblock_input ();
    }
}

void
clear_image_caches (Lisp_Object filter)
{
  /* FIXME: We want to do
   * struct terminal *t;
   * for (t = terminal_list; t; t = t->next_terminal)
   *   clear_image_cache (t, filter); */
  Lisp_Object tail, frame;
  FOR_EACH_FRAME (tail, frame)
    if (FRAME_WINDOW_P (XFRAME (frame)))
      clear_image_cache (XFRAME (frame), filter);
}

DEFUN ("clear-image-cache", Fclear_image_cache, Sclear_image_cache,
       0, 1, 0,
       doc: /* Clear the image cache.
FILTER nil or a frame means clear all images in the selected frame.
FILTER t means clear the image caches of all frames.
Anything else, means only clear those images which refer to FILTER,
which is then usually a filename.  */)
  (Lisp_Object filter)
{
  if (!(EQ (filter, Qnil) || FRAMEP (filter)))
    clear_image_caches (filter);
  else
    clear_image_cache (decode_window_system_frame (filter), Qt);

  return Qnil;
}


DEFUN ("image-flush", Fimage_flush, Simage_flush,
       1, 2, 0,
       doc: /* Flush the image with specification SPEC on frame FRAME.
This removes the image from the Emacs image cache.  If SPEC specifies
an image file, the next redisplay of this image will read from the
current contents of that file.

FRAME nil or omitted means use the selected frame.
FRAME t means refresh the image on all frames.  */)
  (Lisp_Object spec, Lisp_Object frame)
{
  if (!valid_image_p (spec))
    error ("Invalid image specification");

  if (EQ (frame, Qt))
    {
      Lisp_Object tail;
      FOR_EACH_FRAME (tail, frame)
	{
	  struct frame *f = XFRAME (frame);
	  if (FRAME_WINDOW_P (f))
	    uncache_image (f, spec);
	}
    }
  else
    uncache_image (decode_window_system_frame (frame), spec);

  return Qnil;
}


/* Compute masks and transform image IMG on frame F, as specified
   by the image's specification,  */

static void
postprocess_image (struct frame *f, struct image *img)
{
  /* Manipulation of the image's mask.  */
  if (img->pixmap)
    {
      Lisp_Object conversion, spec;
      Lisp_Object mask;

      spec = img->spec;

      /* `:heuristic-mask t'
	 `:mask heuristic'
	 means build a mask heuristically.
	 `:heuristic-mask (R G B)'
	 `:mask (heuristic (R G B))'
	 means build a mask from color (R G B) in the
	 image.
	 `:mask nil'
	 means remove a mask, if any.  */

      mask = image_spec_value (spec, QCheuristic_mask, NULL);
      if (!NILP (mask))
	x_build_heuristic_mask (f, img, mask);
      else
	{
	  bool found_p;

	  mask = image_spec_value (spec, QCmask, &found_p);

	  if (EQ (mask, Qheuristic))
	    x_build_heuristic_mask (f, img, Qt);
	  else if (CONSP (mask)
		   && EQ (XCAR (mask), Qheuristic))
	    {
	      if (CONSP (XCDR (mask)))
		x_build_heuristic_mask (f, img, XCAR (XCDR (mask)));
	      else
		x_build_heuristic_mask (f, img, XCDR (mask));
	    }
	  else if (NILP (mask) && found_p && img->mask)
	    x_clear_image_1 (f, img, CLEAR_IMAGE_MASK);
	}


      /* Should we apply an image transformation algorithm?  */
      conversion = image_spec_value (spec, QCconversion, NULL);
      if (EQ (conversion, Qdisabled))
	x_disable_image (f, img);
      else if (EQ (conversion, Qlaplace))
	x_laplace (f, img);
      else if (EQ (conversion, Qemboss))
	x_emboss (f, img);
      else if (CONSP (conversion)
	       && EQ (XCAR (conversion), Qedge_detection))
	{
	  Lisp_Object tem;
	  tem = XCDR (conversion);
	  if (CONSP (tem))
	    x_edge_detection (f, img,
			      Fplist_get (tem, QCmatrix),
			      Fplist_get (tem, QCcolor_adjustment));
	}
    }
}


/* Return the id of image with Lisp specification SPEC on frame F.
   SPEC must be a valid Lisp image specification (see valid_image_p).  */

ptrdiff_t
lookup_image (struct frame *f, Lisp_Object spec)
{
  struct image *img;
  EMACS_UINT hash;

  /* F must be a window-system frame, and SPEC must be a valid image
     specification.  */
  eassert (FRAME_WINDOW_P (f));
  eassert (valid_image_p (spec));

  /* Look up SPEC in the hash table of the image cache.  */
  hash = sxhash (spec, 0);
  img = search_image_cache (f, spec, hash);
  if (img && img->load_failed_p)
    {
      free_image (f, img);
      img = NULL;
    }

  /* If not found, create a new image and cache it.  */
  if (img == NULL)
    {
      block_input ();
      img = make_image (spec, hash);
      cache_image (f, img);
      img->load_failed_p = ! img->type->load (f, img);
      img->frame_foreground = FRAME_FOREGROUND_PIXEL (f);
      img->frame_background = FRAME_BACKGROUND_PIXEL (f);

      /* If we can't load the image, and we don't have a width and
	 height, use some arbitrary width and height so that we can
	 draw a rectangle for it.  */
      if (img->load_failed_p)
	{
	  Lisp_Object value;

	  value = image_spec_value (spec, QCwidth, NULL);
	  img->width = (INTEGERP (value)
			? XFASTINT (value) : DEFAULT_IMAGE_WIDTH);
	  value = image_spec_value (spec, QCheight, NULL);
	  img->height = (INTEGERP (value)
			 ? XFASTINT (value) : DEFAULT_IMAGE_HEIGHT);
	}
      else
	{
	  /* Handle image type independent image attributes
	     `:ascent ASCENT', `:margin MARGIN', `:relief RELIEF',
	     `:background COLOR'.  */
	  Lisp_Object ascent, margin, relief, bg;
	  int relief_bound;

	  ascent = image_spec_value (spec, QCascent, NULL);
	  if (INTEGERP (ascent))
	    img->ascent = XFASTINT (ascent);
	  else if (EQ (ascent, Qcenter))
	    img->ascent = CENTERED_IMAGE_ASCENT;

	  margin = image_spec_value (spec, QCmargin, NULL);
	  if (INTEGERP (margin))
	    img->vmargin = img->hmargin = XFASTINT (margin);
	  else if (CONSP (margin))
	    {
	      img->hmargin = XFASTINT (XCAR (margin));
	      img->vmargin = XFASTINT (XCDR (margin));
	    }

	  relief = image_spec_value (spec, QCrelief, NULL);
	  relief_bound = INT_MAX - max (img->hmargin, img->vmargin);
	  if (RANGED_INTEGERP (- relief_bound, relief, relief_bound))
	    {
	      img->relief = XINT (relief);
	      img->hmargin += eabs (img->relief);
	      img->vmargin += eabs (img->relief);
	    }

	  if (! img->background_valid)
	    {
	      bg = image_spec_value (img->spec, QCbackground, NULL);
	      if (!NILP (bg))
		{
		  img->background
		    = x_alloc_image_color (f, img, bg,
					   FRAME_BACKGROUND_PIXEL (f));
		  img->background_valid = 1;
		}
	    }

	  /* Do image transformations and compute masks, unless we
	     don't have the image yet.  */
	  if (!EQ (*img->type->type, Qpostscript))
	    postprocess_image (f, img);
	}

      unblock_input ();
    }

  /* We're using IMG, so set its timestamp to `now'.  */
  img->timestamp = current_timespec ();

  /* Value is the image id.  */
  return img->id;
}


/* Cache image IMG in the image cache of frame F.  */

static void
cache_image (struct frame *f, struct image *img)
{
  struct image_cache *c = FRAME_IMAGE_CACHE (f);
  ptrdiff_t i;

  /* Find a free slot in c->images.  */
  for (i = 0; i < c->used; ++i)
    if (c->images[i] == NULL)
      break;

  /* If no free slot found, maybe enlarge c->images.  */
  if (i == c->used && c->used == c->size)
    c->images = xpalloc (c->images, &c->size, 1, -1, sizeof *c->images);

  /* Add IMG to c->images, and assign IMG an id.  */
  c->images[i] = img;
  img->id = i;
  if (i == c->used)
    ++c->used;

  /* Add IMG to the cache's hash table.  */
  i = img->hash % IMAGE_CACHE_BUCKETS_SIZE;
  img->next = c->buckets[i];
  if (img->next)
    img->next->prev = img;
  img->prev = NULL;
  c->buckets[i] = img;
}


/* Call FN on every image in the image cache of frame F.  Used to mark
   Lisp Objects in the image cache.  */

/* Mark Lisp objects in image IMG.  */

static void
mark_image (struct image *img)
{
  mark_object (img->spec);
  mark_object (img->dependencies);

  if (!NILP (img->lisp_data))
    mark_object (img->lisp_data);
}


void
mark_image_cache (struct image_cache *c)
{
  if (c)
    {
      ptrdiff_t i;
      for (i = 0; i < c->used; ++i)
	if (c->images[i])
	  mark_image (c->images[i]);
    }
}



/***********************************************************************
                      X / MAC / NS / W32 support code
 ***********************************************************************/

#ifdef WINDOWSNT

/* Macro for defining functions that will be loaded from image DLLs.  */
#define DEF_IMGLIB_FN(rettype,func,args) static rettype (FAR CDECL *fn_##func)args

/* Macro for loading those image functions from the library.  */
#define LOAD_IMGLIB_FN(lib,func) {					\
    fn_##func = (void *) GetProcAddress (lib, #func);			\
    if (!fn_##func) return 0;						\
  }

#endif /* WINDOWSNT */

/* Return true if XIMG's size WIDTH x HEIGHT doesn't break the
   windowing system.
   WIDTH and HEIGHT must both be positive.
   If XIMG is null, assume it is a bitmap.  */
static bool
x_check_image_size(XImagePtr ximg, int width, int height)
{
#ifdef HAVE_X_WINDOWS
  /* Respect Xlib's limits: it cannot deal with images that have more
     than INT_MAX (and/or UINT_MAX) bytes.  And respect Emacs's limits
     of PTRDIFF_MAX (and/or SIZE_MAX) bytes for any object.  */
  enum
  {
    XLIB_BYTES_MAX = min(INT_MAX, UINT_MAX),
    X_IMAGE_BYTES_MAX = min(XLIB_BYTES_MAX, min(PTRDIFF_MAX, SIZE_MAX))
  };

  int bitmap_pad, depth, bytes_per_line;
  if (ximg)
    {
      bitmap_pad = ximg->bitmap_pad;
      depth = ximg->depth;
      bytes_per_line = ximg->bytes_per_line;
    }
  else
    {
      bitmap_pad = 8;
      depth = 1;
      bytes_per_line = ((width >> 3) + ((width & 7) != 0));
    }
  return ((width <= ((INT_MAX - (bitmap_pad - 1)) / depth))
	  && (height <= (X_IMAGE_BYTES_MAX / bytes_per_line)));
#else
  /* FIXME: Implement this check for the HAVE_NS and HAVE_NTGUI cases.
     For now, assume that every image size is allowed on these systems.  */
  return 1;
#endif /* HAVE_X_WINDOWS */
}

/* Create an XImage and a pixmap of size WIDTH x HEIGHT for use on
   frame F.  Set *XIMG and *PIXMAP to the XImage and Pixmap created.
   Set (*XIMG)->data to a raster of WIDTH x HEIGHT pixels allocated
   via xmalloc.  Print error messages via image_error if an error
   occurs.  Value is true if successful.

   On W32, a DEPTH of zero signifies a 24 bit image, otherwise DEPTH
   should indicate the bit depth of the image.  */

static bool
x_create_x_image_and_pixmap(struct frame *f, int width, int height,
                            int depth, XImagePtr *ximg, Pixmap *pixmap)
{
#ifdef HAVE_X_WINDOWS
  Display *display = FRAME_X_DISPLAY(f);
  Window window = FRAME_X_WINDOW(f);
  Screen *screen = FRAME_X_SCREEN(f);

  eassert(input_blocked_p());

  if (depth <= 0)
    depth = DefaultDepthOfScreen(screen);
  *ximg = XCreateImage(display, DefaultVisualOfScreen(screen),
                       depth, ZPixmap, 0, NULL, width, height,
                       ((depth > 16) ? 32 : ((depth > 8) ? 16 : 8)), 0);
  if (*ximg == NULL)
    {
      image_error("Unable to allocate X image", Qnil, Qnil);
      return 0;
    }

  if (! x_check_image_size(*ximg, width, height))
    {
      x_destroy_x_image(*ximg);
      *ximg = NULL;
      image_error("Image too large (%dx%d)",
		  make_number(width), make_number(height));
      return 0;
    }

  /* Allocate image raster: */
  (*ximg)->data = xmalloc((*ximg)->bytes_per_line * height);

  /* Allocate a pixmap of the same size: */
  *pixmap = XCreatePixmap(display, window, width, height, depth);
  if (*pixmap == NO_PIXMAP)
    {
      x_destroy_x_image(*ximg);
      *ximg = NULL;
      image_error("Unable to create X pixmap", Qnil, Qnil);
      return 0;
    }

  return 1;
#endif /* HAVE_X_WINDOWS */

#ifdef HAVE_NTGUI
  BITMAPINFOHEADER *header;
  HDC hdc;
  int scanline_width_bits;
  int remainder;
  int palette_colors = 0;

  if (depth == 0)
    depth = 24;

  if ((depth != 1) && (depth != 4) && (depth != 8)
      && (depth != 16) && (depth != 24) && (depth != 32))
    {
      image_error("Invalid image bit depth specified", Qnil, Qnil);
      return 0;
    }

  scanline_width_bits = (width * depth);
  remainder = (scanline_width_bits % 32);

  if (remainder)
    scanline_width_bits += (32 - remainder);

  /* Bitmaps with a depth less than 16 need a palette.  */
  /* BITMAPINFO structure already contains the first RGBQUAD.  */
  if (depth < 16)
    palette_colors = (1 << (depth - 1));

  *ximg = xmalloc(sizeof(XImage) + palette_colors * sizeof(RGBQUAD));

  header = &(*ximg)->info.bmiHeader;
  memset(&(*ximg)->info, 0, sizeof(BITMAPINFO));
  header->biSize = sizeof(*header);
  header->biWidth = width;
  header->biHeight = -height;  /* negative indicates a top-down bitmap.  */
  header->biPlanes = 1;
  header->biBitCount = depth;
  header->biCompression = BI_RGB;
  header->biClrUsed = palette_colors;

  /* TODO: fill in palette.  */
  if (depth == 1)
    {
      (*ximg)->info.bmiColors[0].rgbBlue = 0;
      (*ximg)->info.bmiColors[0].rgbGreen = 0;
      (*ximg)->info.bmiColors[0].rgbRed = 0;
      (*ximg)->info.bmiColors[0].rgbReserved = 0;
      (*ximg)->info.bmiColors[1].rgbBlue = 255;
      (*ximg)->info.bmiColors[1].rgbGreen = 255;
      (*ximg)->info.bmiColors[1].rgbRed = 255;
      (*ximg)->info.bmiColors[1].rgbReserved = 0;
    }

  hdc = get_frame_dc(f);

  /* Create a DIBSection and raster array for the bitmap,
     and store its handle in *pixmap.  */
  *pixmap = CreateDIBSection(hdc, &((*ximg)->info),
			     ((depth < 16)
                              ? DIB_PAL_COLORS : DIB_RGB_COLORS),
			     /* casting avoids a GCC warning: */
			     (void **)&((*ximg)->data), NULL, 0);

  /* Realize display palette and garbage all frames: */
  release_frame_dc(f, hdc);

  if (*pixmap == NULL)
    {
      DWORD err = GetLastError();
      Lisp_Object errcode;
      /* All system errors are < 10000, so the following is safe: */
      XSETINT(errcode, err);
      image_error("Unable to create bitmap, error code %d", errcode, Qnil);
      x_destroy_x_image(*ximg);
      *ximg = NULL;
      return 0;
    }

  return 1;
#endif /* HAVE_NTGUI */

#if defined(MAC_OS) || (defined(HAVE_CARBON) && !defined(HAVE_NS))
  Display *display = FRAME_X_DISPLAY(f);
  Window window = FRAME_X_WINDOW(f);

  xassert(interrupt_input_blocked);

  /* Allocate a pixmap of the same size: */
  *pixmap = XCreatePixmap(display, window, width, height, depth);
  if (*pixmap == NO_PIXMAP) {
    *ximg = NULL;
    image_error("Unable to create X pixmap", Qnil, Qnil);
    return 0;
  }

# if defined(LockPixels) && defined(GetGWorldPixMap)
  LockPixels(GetGWorldPixMap(*pixmap));
# endif /* LockPixels && GetGWorldPixMap */
  *ximg = *pixmap;
  return 1;
#endif  /* MAC_OS || (HAVE_CARBON && !HAVE_NS) */

#ifdef HAVE_NS
  *pixmap = ns_image_for_XPM(width, height, depth);
  if (*pixmap == 0)
    {
      *ximg = NULL;
      image_error("Unable to allocate NSImage for XPM pixmap", Qnil, Qnil);
      return 0;
    }
  *ximg = *pixmap;
  return 1;
#endif /* HAVE_NS */
}


/* Destroy XImage XIMG.  Free XIMG->data: */
static void
x_destroy_x_image(XImagePtr ximg)
{
  eassert(input_blocked_p());
  if (ximg)
    {
#ifdef HAVE_X_WINDOWS
      xfree(ximg->data);
      ximg->data = NULL;
      XDestroyImage(ximg);
#endif /* HAVE_X_WINDOWS */
#ifdef HAVE_NTGUI
      /* Data will be freed by DestroyObject: */
      ximg->data = NULL;
      xfree(ximg);
#endif /* HAVE_NTGUI */
#ifdef MAC_OS
      XDestroyImage(ximg);
#endif /* MAC_OS */
#ifdef HAVE_NS
      ns_release_object(ximg);
#endif /* HAVE_NS */
    }
}


/* Put XImage XIMG into pixmap PIXMAP on frame F.  WIDTH and HEIGHT
   are width and height of both the image and pixmap.  */

static void
x_put_x_image(struct frame *f, XImagePtr ximg, Pixmap pixmap,
              int width, int height)
{
#ifdef HAVE_X_WINDOWS
  GC gc;

  eassert(input_blocked_p());
  gc = XCreateGC(FRAME_X_DISPLAY(f), pixmap, 0, NULL);
  XPutImage(FRAME_X_DISPLAY(f), pixmap, gc, ximg, 0, 0, 0, 0,
            width, height);
  XFreeGC(FRAME_X_DISPLAY(f), gc);
#endif /* HAVE_X_WINDOWS */

#ifdef HAVE_NTGUI
# if 0 /* I do NOT think this is necessary looking at where it is used: */
  HDC hdc = get_frame_dc(f);
  SetDIBits(hdc, pixmap, 0, height, ximg->data, &(ximg->info), DIB_RGB_COLORS);
  release_frame_dc(f, hdc);
# endif /* 0 */
#endif /* HAVE_NTGUI */

#ifdef MAC_OS
  xassert(ximg == pixmap);
#endif /* MAC_OS */
#ifdef HAVE_NS
  eassert(ximg == pixmap);
  ns_retain_object(ximg);
#endif /* HAVE_NS */
}

/* Thin wrapper for x_create_x_image_and_pixmap, so that it matches
   with image_put_x_image.  */

static bool
image_create_x_image_and_pixmap (struct frame *f, struct image *img,
				 int width, int height, int depth,
				 XImagePtr *ximg, bool mask_p)
{
  eassert ((!mask_p ? img->pixmap : img->mask) == NO_PIXMAP);

  return x_create_x_image_and_pixmap (f, width, height, depth, ximg,
				      !mask_p ? &img->pixmap : &img->mask);
}

/* Put X image XIMG into image IMG on frame F, as a mask if and only
   if MASK_P.  On X, this simply records XIMG on a member of IMG, so
   it can be put into the pixmap afterwards via image_sync_to_pixmaps.
   On the other platforms, it puts XIMG into the pixmap, then frees
   the X image and its buffer.  */

static void
image_put_x_image (struct frame *f, struct image *img, XImagePtr ximg,
		   bool mask_p)
{
#ifdef HAVE_X_WINDOWS
  if (!mask_p)
    {
      eassert(img->ximg == NULL);
      img->ximg = ximg;
    }
  else
    {
      eassert(img->mask_img == NULL);
      img->mask_img = ximg;
    }
#else
  x_put_x_image(f, ximg, (!mask_p ? img->pixmap : img->mask),
                img->width, img->height);
  x_destroy_x_image(ximg);
#endif /* HAVE_X_WINDOWS */
}

#ifdef HAVE_X_WINDOWS
/* Put the X images recorded in IMG on frame F into pixmaps, then free
   the X images and their buffers.  */

static void
image_sync_to_pixmaps(struct frame *f, struct image *img)
{
  if (img->ximg)
    {
      x_put_x_image(f, img->ximg, img->pixmap, img->width, img->height);
      x_destroy_x_image(img->ximg);
      img->ximg = NULL;
    }
  if (img->mask_img)
    {
      x_put_x_image(f, img->mask_img, img->mask, img->width, img->height);
      x_destroy_x_image(img->mask_img);
      img->mask_img = NULL;
    }
}
#endif /* HAVE_X_WINDOWS */

#ifdef HAVE_NTGUI
/* Create a memory device context for IMG on frame F.  It stores the
   currently selected GDI object into *PREV for future restoration by
   image_unget_x_image_or_dc.  */

static XImagePtr_or_DC
image_get_x_image_or_dc(struct frame *f, struct image *img, bool mask_p,
                        HGDIOBJ *prev)
{
  HDC frame_dc = get_frame_dc(f);
  XImagePtr_or_DC ximg = CreateCompatibleDC(frame_dc);

  release_frame_dc(f, frame_dc);
  *prev = SelectObject(ximg, (!mask_p ? img->pixmap : img->mask));

  return ximg;
}

static void
image_unget_x_image_or_dc(struct image *img, bool mask_p,
			  XImagePtr_or_DC ximg, HGDIOBJ prev)
{
  SelectObject(ximg, prev);
  DeleteDC(ximg);
}
#else  /* !HAVE_NTGUI */
/* Get the X image for IMG on frame F.  The resulting X image data
   should be treated as read-only at least on X.  */

static XImagePtr
image_get_x_image(struct frame *f, struct image *img, bool mask_p)
{
# ifdef HAVE_X_WINDOWS
  XImagePtr ximg_in_img = (!mask_p ? img->ximg : img->mask_img);

  if (ximg_in_img)
    return ximg_in_img;
  else
    return XGetImage(FRAME_X_DISPLAY(f), !mask_p ? img->pixmap : img->mask,
		     0, 0, img->width, img->height, ~0, ZPixmap);
# elif defined(HAVE_NS)
  XImagePtr pixmap = (!mask_p ? img->pixmap : img->mask);

  ns_retain_object(pixmap);
  return pixmap;
# else
  XImagePtr pixmap = (XImagePtr)NULL;
  return pixmap;
# endif /* HAVE_X_WINDOWS || HAVE_NS */
}

static void
image_unget_x_image(struct image *img, bool mask_p, XImagePtr ximg)
{
# ifdef HAVE_X_WINDOWS
  XImagePtr ximg_in_img = (!mask_p ? img->ximg : img->mask_img);

  if (ximg_in_img)
    eassert(ximg == ximg_in_img);
  else
    XDestroyImage (ximg);
# elif defined(HAVE_NS)
  ns_release_object(ximg);
# else
  return;
# endif /* HAVE_X_WINDOWS || HAVE_NS */
}
#endif	/* !HAVE_NTGUI */


/***********************************************************************
			      File Handling
 ***********************************************************************/

/* Find image file FILE.  Look in data-directory/images, then
   x-bitmap-file-path.  Value is the encoded full name of the file
   found, or nil if not found.  */

Lisp_Object
x_find_image_file (Lisp_Object file)
{
  Lisp_Object file_found, search_path;
  int fd;

  /* TODO: I think this should use something like image-load-path
     instead.  Unfortunately, that can contain non-string elements.  */
  search_path = Fcons (Fexpand_file_name (build_string ("images"),
					  Vdata_directory),
		       Vx_bitmap_file_path);

  /* Try to find FILE in data-directory/images, then x-bitmap-file-path.  */
  fd = openp (search_path, file, Qnil, &file_found, Qnil, false);

  if (fd == -1)
    file_found = Qnil;
  else
    {
      file_found = ENCODE_FILE (file_found);
      if (fd != -2)
	emacs_close (fd);
    }

  return file_found;
}


/* Read FILE into memory.  Value is a pointer to a buffer allocated
   with xmalloc holding FILE's contents.  Value is null if an error
   occurred.  *SIZE is set to the size of the file.  */

static unsigned char *
slurp_file(char *file, ptrdiff_t *size)
{
  FILE *fp = emacs_fopen(file, "rb");
  unsigned char *buf = NULL;
  struct stat st;

  if (fp)
    {
      ptrdiff_t count = SPECPDL_INDEX();
      record_unwind_protect_ptr(fclose_unwind, fp);

      if (fstat(fileno(fp), &st) == 0
	  && 0 <= st.st_size && st.st_size < min(PTRDIFF_MAX, SIZE_MAX))
	{
	  /* Report an error if we read past the purported EOF.
	     This can happen if the file grows as we read it.  */
	  ptrdiff_t buflen = st.st_size;
	  buf = xmalloc(buflen + 1UL);
	  if (fread(buf, 1, (buflen + 1), fp) == buflen)
	    *size = buflen;
	  else
	    {
	      xfree(buf);
	      buf = NULL;
	    }
	}

      unbind_to(count, Qnil);
    }

  return buf;
}

/* This part does actually compile for 64-bit, but with a bunch of warnings
 * about implicit function declarations for 32-bit-only functions, which
 * would probably turn into errors at link time, so conditionalize on
 * __LP64__, as well as the other stuff: */
#if defined(MAC_OS) || \
    (defined(HAVE_CARBON) && !defined(HAVE_NS) && (!defined(__LP64__) || !__LP64__))

/***********************************************************************
			MAC Image Load Functions
 ***********************************************************************/

static int image_load_quicktime(struct frame *, struct image *img, OSType);
# ifdef MAC_OSX
static int image_load_quartz2d(struct frame *, struct image *img, int);
# endif /* MAC_OSX */

static OSErr
find_image_fsspec(Lisp_Object specified_file, Lisp_Object *file,
                  FSSpec *fss)
{
  OSErr err;
  AEDesc desc;

  *file = x_find_image_file(specified_file);
  if (!STRINGP(*file))
    return fnfErr;	/* file or directory not found;
                         * incomplete pathname */
  /* Try to open the image file: */
# if !defined(__LP64__) || !__LP64__
  err = AECoercePtr(TYPE_FILE_NAME, SDATA(*file),
		    SBYTES(*file), typeFSS, &desc);
# else
  err = -32766; /* Undocumented value */
# endif /* !__LP64__ */
  if (err == noErr) {
# if TARGET_API_MAC_CARBON
    err = AEGetDescData(&desc, fss, sizeof(FSSpec));
# else
    *fss = *(FSSpec *)(*(desc.dataHandle));
# endif /* TARGET_API_MAC_CARBON */
    AEDisposeDesc(&desc);
  }
  return err;
}

# if defined(HAVE_CARBON) && defined(HAVE_QUICKTIME_IMAGECOMPRESSION_H)
#  include <QuickTime/ImageCompression.h>
# endif /* HAVE_CARBON && HAVE_QUICKTIME_IMAGECOMPRESSION_H */
# if !defined(__IMAGECOMPRESSION__) || (defined(__LP64__) && __LP64__)
typedef ComponentInstance GraphicsImportComponent;
/* Flags for GraphicsImportDoesDrawAllPixels: */
enum {
  graphicsImporterDrawsAllPixels = 0,
  graphicsImporterDoesntDrawAllPixels = 1,
  graphicsImporterDontKnowIfDrawAllPixels = 2
};
# endif /* !__IMAGECOMPRESSION__ || __LP64__ */

static int
image_load_qt_1(struct frame *f, struct image *img, OSType type,
                const FSSpec *fss, Handle dh)
{
  ComponentResult err;
  GraphicsImportComponent gi;
  Rect rect;
  int width, height;
  ImageDescriptionHandle desc_handle;
  short draw_all_pixels;
  Lisp_Object specified_bg;
  XColor color;
  XImagePtr ximg;
  RGBColor bg_color;

#if !defined(__LP64__) || !__LP64__
  err = OpenADefaultComponent(GraphicsImporterComponentType, type, &gi);
#else
  err = -32766; /* Undocumented value */
#endif /* !__LP64__ || !__LP64__ */
  if (err != noErr) {
    image_error("Cannot get importer component for `%s'", img->spec, Qnil);
    return 0;
  }
  if (dh == NULL) {
    /* read from file system spec: */
    err = GraphicsImportSetDataFile(gi, fss);
    if (err != noErr) {
      image_error("Cannot set fsspec to graphics importer for '%s'",
                  img->spec, Qnil);
      goto error;
    }
  } else {
    /* read from data handle */
    err = GraphicsImportSetDataHandle(gi, dh);
    if (err != noErr) {
      image_error("Cannot set data handle to graphics importer for `%s'",
                  img->spec, Qnil);
      goto error;
    }
  }
  err = GraphicsImportGetImageDescription(gi, &desc_handle);
  if (err != noErr || desc_handle == NULL) {
    image_error("Error reading `%s'", img->spec, Qnil);
    goto error;
  }
  width = img->width = (*desc_handle)->width;
  height = img->height = (*desc_handle)->height;
  DisposeHandle((Handle)desc_handle);

  if (!check_image_size(f, width, height)) {
    image_error("Invalid image size", Qnil, Qnil);
    goto error;
  }

  err = GraphicsImportDoesDrawAllPixels(gi, &draw_all_pixels);
# ifdef __clang_analyzer__
  /* Do NOT check the error code here.  It may have an undocumented
   * value -32766. */
  if (err != noErr) {
    image_error("Error reading `%s'", img->spec, Qnil);
    goto error;
  }
# endif /* __clang_analyzer__ */
  if (draw_all_pixels != graphicsImporterDrawsAllPixels) {
    specified_bg = image_spec_value(img->spec, QCbackground, NULL);
    if (!STRINGP(specified_bg) ||
        !mac_defined_color(f, (char *)SDATA(specified_bg), &color, 0))
    {
      color.pixel = FRAME_BACKGROUND_PIXEL(f);
      color.red = RED16_FROM_ULONG(color.pixel);
      color.green = GREEN16_FROM_ULONG(color.pixel);
      color.blue = BLUE16_FROM_ULONG(color.pixel);
    }
  }

  if (!x_create_x_image_and_pixmap(f, width, height, 0, &ximg, &img->pixmap))
    goto error;
  if (draw_all_pixels != graphicsImporterDrawsAllPixels) {
    CGrafPtr old_port;
    GDHandle old_gdh;

    GetGWorld(&old_port, &old_gdh);
    SetGWorld(ximg, NULL);
    bg_color.red = color.red;
    bg_color.green = color.green;
    bg_color.blue = color.blue;
    RGBBackColor(&bg_color);
# if TARGET_API_MAC_CARBON
    GetPortBounds(ximg, &rect);
    EraseRect(&rect);
# else
    EraseRect(&(ximg->portRect));
# endif /* TARGET_API_MAC_CARBON */
    SetGWorld(old_port, old_gdh);
  }
  GraphicsImportSetGWorld(gi, ximg, NULL);
  GraphicsImportDraw(gi);
  CloseComponent(gi);

  /* Maybe fill in the background field while we have ximg handy: */
  if (NILP(image_spec_value(img->spec, QCbackground, NULL)))
    IMAGE_BACKGROUND(img, f, ximg);

  /* Put the image into the pixmap: */
  x_put_x_image(f, ximg, img->pixmap, width, height);
  x_destroy_x_image(ximg);
  return 1;

error:
  CloseComponent(gi);
  return 0;
}


/* Load an image using the QuickTime Graphics Importer.
 * Note: The alpha channel does not work for PNG images. */
static int ATTRIBUTE_USED
image_load_quicktime(struct frame *f, struct image *img, OSType type)
{
  Lisp_Object specified_file;
  Lisp_Object specified_data;
  OSErr err;

  specified_file = image_spec_value(img->spec, QCfile, NULL);
  specified_data = image_spec_value(img->spec, QCdata, NULL);

  if (NILP(specified_data)) {
    /* Read from a file: */
    Lisp_Object file;
    FSSpec fss;

    err = find_image_fsspec(specified_file, &file, &fss);
    if (err != noErr) {
      if (err == fnfErr)
        image_error("Cannot find image file `%s'", specified_file, Qnil);
      else
        image_error("Cannot open `%s'", file, Qnil);
      return 0;
    }
    return image_load_qt_1(f, img, type, &fss, NULL);
  } else {
    /* Memory source! */
    int success_p;
    Handle dh;

    err = PtrToHand(SDATA(specified_data), &dh, SBYTES(specified_data));
    if (err != noErr) {
      image_error("Cannot allocate data handle for `%s'",
                  img->spec, Qnil);
      return 0;
    }
    success_p = image_load_qt_1(f, img, type, NULL, dh);
    DisposeHandle(dh);
    return success_p;
  }
}


# ifdef MAC_OSX
/* Load a PNG/JPEG image using Quartz 2D decoding routines.
 * CGImageCreateWithPNGDataProvider is provided after Mac OS X 10.2.
 * So do NOT use this function directly but determine at runtime
 * whether it exists. */
typedef CGImageRef (*CGImageCreateWithPNGDataProviderProcType)
  (CGDataProviderRef, const float [], bool, CGColorRenderingIntent);
static CGImageCreateWithPNGDataProviderProcType MyCGImageCreateWithPNGDataProvider;

#  if defined(HAVE_MACH_O_DYLD_H) || defined(__MACH__)
#   include <mach-o/dyld.h>
#  else
#   ifdef HAVE_DLFCN_H
#    include <dlfcn.h>
#   endif /* HAVE_DLFCN_H */
#  endif /* HAVE_MACH_O_DYLD_H || __MACH__ */

static void ATTRIBUTE_USED
init_image_func_pointer(void)
{
  if (NSIsSymbolNameDefined("_CGImageCreateWithPNGDataProvider")) {
    MyCGImageCreateWithPNGDataProvider =
      ((CGImageCreateWithPNGDataProviderProcType)
       NSAddressOfSymbol(NSLookupAndBindSymbol("_CGImageCreateWithPNGDataProvider")));
  } else
    MyCGImageCreateWithPNGDataProvider = NULL;
}

/* */
static int ATTRIBUTE_USED
image_load_quartz2d(struct frame *f, struct image *img, int png_p)
{
  Lisp_Object file, specified_file;
  Lisp_Object specified_data, specified_bg;
  struct gcpro gcpro1;
  CGDataProviderRef source;
  CGImageRef image;
  int width, height;
  XColor color;
  XImagePtr ximg = NULL;
  CGContextRef context;
  CGRect rectangle;

  /* Open the file: */
  specified_file = image_spec_value(img->spec, QCfile, NULL);
  specified_data = image_spec_value(img->spec, QCdata, NULL);

  file = Qnil;
  GCPRO1(file);

  if (NILP(file)) {
    ; /* ??? */
  }

  if (NILP(specified_data)) {
    CFStringRef path;
    CFURLRef url;

    file = x_find_image_file(specified_file);
    if (!STRINGP(file)) {
      image_error("Cannot find image file `%s'", specified_file, Qnil);
      UNGCPRO;
      return 0;
    }
    path = cfstring_create_with_utf8_cstring((const char *)SDATA(file));
    url = CFURLCreateWithFileSystemPath(NULL, path,
                                        kCFURLPOSIXPathStyle, 0);
    CFRelease(path);
    source = CGDataProviderCreateWithURL(url);
    CFRelease(url);
  } else
    source = CGDataProviderCreateWithData(NULL, SDATA(specified_data),
					  SBYTES(specified_data), NULL);

  if (png_p)
    image = (*MyCGImageCreateWithPNGDataProvider)(source, NULL, FALSE,
						  kCGRenderingIntentDefault);
  else
    image = CGImageCreateWithJPEGDataProvider(source, NULL, FALSE,
					      kCGRenderingIntentDefault);

  CGDataProviderRelease(source);
  if (image == NULL) {
    UNGCPRO;
    image_error("Error reading image `%s'", img->spec, Qnil);
    return 0;
  }
  width = img->width = CGImageGetWidth(image);
  height = img->height = CGImageGetHeight(image);

  if (!check_image_size(f, width, height)) {
    CGImageRelease(image);
    UNGCPRO;
    image_error("Invalid image size", Qnil, Qnil);
    return 0;
  }

  if (png_p) {
    specified_bg = image_spec_value(img->spec, QCbackground, NULL);
    if (!STRINGP(specified_bg) ||
        !mac_defined_color(f, (char *)SDATA(specified_bg), &color, 0))
    {
      color.pixel = FRAME_BACKGROUND_PIXEL(f);
      color.red = RED16_FROM_ULONG(color.pixel);
      color.green = GREEN16_FROM_ULONG(color.pixel);
      color.blue = BLUE16_FROM_ULONG(color.pixel);
    }
  }

  if (!x_create_x_image_and_pixmap(f, width, height, 0, &ximg, &img->pixmap))
    {
      CGImageRelease(image);
      UNGCPRO;
      return 0;
    }
  rectangle = CGRectMake(0, 0, width, height);
  QDBeginCGContext(ximg, &context);
  if (png_p) {
    CGContextSetRGBFillColor(context, (color.red / 65535.0),
                             (color.green / 65535.0),
                             (color.blue / 65535.0), 1.0);
    CGContextFillRect(context, rectangle);
  }
  CGContextDrawImage(context, rectangle, image);
  QDEndCGContext(ximg, &context);
  CGImageRelease(image);

  /* Maybe fill in the background field while we have ximg handy: */
  if (NILP(image_spec_value(img->spec, QCbackground, NULL)))
    IMAGE_BACKGROUND(img, f, ximg);

  /* Put the image into the pixmap: */
  x_put_x_image(f, ximg, img->pixmap, width, height);
  x_destroy_x_image(ximg);
  UNGCPRO;
  return 1;
}
# endif /* MAC_OSX */

#endif  /* MAC_OS || (HAVE_CARBON && !HAVE_NS && !__LP64__) */

/***********************************************************************
			      XBM images
 ***********************************************************************/

static bool xbm_load(struct frame *f, struct image *img);
static bool xbm_image_p(Lisp_Object object);
static bool xbm_file_p(Lisp_Object);


/* Indices of image specification fields in xbm_format, below: */
enum xbm_keyword_index
{
  XBM_TYPE,
  XBM_FILE,
  XBM_WIDTH,
  XBM_HEIGHT,
  XBM_DATA,
  XBM_FOREGROUND,
  XBM_BACKGROUND,
  XBM_ASCENT,
  XBM_MARGIN,
  XBM_RELIEF,
  XBM_ALGORITHM,
  XBM_HEURISTIC_MASK,
  XBM_MASK,
  XBM_LAST
};

/* Vector of image_keyword structures describing the format
   of valid XBM image specifications.  */

static const struct image_keyword xbm_format[XBM_LAST] =
{
  {":type",		IMAGE_SYMBOL_VALUE,			1},
  {":file",		IMAGE_STRING_VALUE,			0},
  {":width",		IMAGE_POSITIVE_INTEGER_VALUE,		0},
  {":height",		IMAGE_POSITIVE_INTEGER_VALUE,		0},
  {":data",		IMAGE_DONT_CHECK_VALUE_TYPE,		0},
  {":foreground",	IMAGE_STRING_OR_NIL_VALUE,		0},
  {":background",	IMAGE_STRING_OR_NIL_VALUE,		0},
  {":ascent",		IMAGE_ASCENT_VALUE,			0},
  {":margin",		IMAGE_NON_NEGATIVE_INTEGER_VALUE_OR_PAIR, 0},
  {":relief",		IMAGE_INTEGER_VALUE,			0},
  {":conversion",	IMAGE_DONT_CHECK_VALUE_TYPE,		0},
  {":heuristic-mask",	IMAGE_DONT_CHECK_VALUE_TYPE,		0},
  {":mask",		IMAGE_DONT_CHECK_VALUE_TYPE,		0}
};

/* Structure describing the image type XBM: */
static struct image_type xbm_type =
{
  &Qxbm,
  xbm_image_p,
  xbm_load,
  x_clear_image,
  NULL,
  NULL
};

/* Tokens returned from xbm_scan: */
enum xbm_token
{
  XBM_TK_IDENT = 256,
  XBM_TK_NUMBER
};


/* Return true if OBJECT is a valid XBM-type image specification.
   A valid specification is a list starting with the symbol `image'
   The rest of the list is a property list which must contain an
   entry `:type xbm'.

   If the specification specifies a file to load, it must contain
   an entry `:file FILENAME' where FILENAME is a string.

   If the specification is for a bitmap loaded from memory it must
   contain `:width WIDTH', `:height HEIGHT', and `:data DATA', where
   WIDTH and HEIGHT are integers > 0.  DATA may be:

   1. a string large enough to hold the bitmap data, i.e. it must
   have a size >= (WIDTH + 7) / 8 * HEIGHT

   2. a bool-vector of size >= WIDTH * HEIGHT

   3. a vector of strings or bool-vectors, one for each line of the
   bitmap.

   4. a string containing an in-memory XBM file.  WIDTH and HEIGHT
   may not be specified in this case because they are defined in the
   XBM file.

   Both the file and data forms may contain the additional entries
   `:background COLOR' and `:foreground COLOR'.  If not present,
   foreground and background of the frame on which the image is
   displayed is used.  */

static bool
xbm_image_p (Lisp_Object object)
{
  struct image_keyword kw[XBM_LAST];

  memcpy (kw, xbm_format, sizeof kw);
  if (!parse_image_spec (object, kw, XBM_LAST, Qxbm))
    return 0;

  eassert (EQ (kw[XBM_TYPE].value, Qxbm));

  if (kw[XBM_FILE].count)
    {
      if (kw[XBM_WIDTH].count || kw[XBM_HEIGHT].count || kw[XBM_DATA].count)
	return 0;
    }
  else if (kw[XBM_DATA].count && xbm_file_p (kw[XBM_DATA].value))
    {
      /* In-memory XBM file.  */
      if (kw[XBM_WIDTH].count || kw[XBM_HEIGHT].count || kw[XBM_FILE].count)
	return 0;
    }
  else
    {
      Lisp_Object data;
      int width, height;

      /* Entries for `:width', `:height' and `:data' must be present.  */
      if (!kw[XBM_WIDTH].count
	  || !kw[XBM_HEIGHT].count
	  || !kw[XBM_DATA].count)
	return 0;

      data = kw[XBM_DATA].value;
      width = XFASTINT (kw[XBM_WIDTH].value);
      height = XFASTINT (kw[XBM_HEIGHT].value);

      /* Check type of data, and width and height against contents of
	 data.  */
      if (VECTORP (data))
	{
	  EMACS_INT i;

	  /* Number of elements of the vector must be >= height.  */
	  if (ASIZE (data) < height)
	    return 0;

	  /* Each string or bool-vector in data must be large enough
	     for one line of the image.  */
	  for (i = 0; i < height; ++i)
	    {
	      Lisp_Object elt = AREF (data, i);

	      if (STRINGP (elt))
		{
		  if (SCHARS (elt)
		      < (width + BITS_PER_CHAR - 1) / BITS_PER_CHAR)
		    return 0;
		}
	      else if (BOOL_VECTOR_P (elt))
		{
		  if (bool_vector_size (elt) < width)
		    return 0;
		}
	      else
		return 0;
	    }
	}
      else if (STRINGP (data))
	{
	  if (SCHARS (data)
	      < (width + BITS_PER_CHAR - 1) / BITS_PER_CHAR * height)
	    return 0;
	}
      else if (BOOL_VECTOR_P (data))
	{
	  if (bool_vector_size (data) / height < width)
	    return 0;
	}
      else
	return 0;
    }

  return 1;
}


/* Scan a bitmap file.  FP is the stream to read from.  Value is
   either an enumerator from enum xbm_token, or a character for a
   single-character token, or 0 at end of file.  If scanning an
   identifier, store the lexeme of the identifier in SVAL.  If
   scanning a number, store its value in *IVAL.  */

static int
xbm_scan(unsigned char **s, unsigned char *end, char *sval, int *ival)
{
  unsigned int c IF_LINT(= 0U);

 loop:

  /* Skip white space: */
  while (*s < end && (c = *(*s)++, c_isspace(c)))
    ;

  if (*s >= end)
    c = 0;
  else if (c_isdigit(c))
    {
      int value = 0, digit;

      if (c == '0' && *s < end)
	{
	  c = *(*s)++;
	  if (c == 'x' || c == 'X')
	    {
	      while (*s < end)
		{
		  c = *(*s)++;
		  if (c_isdigit (c))
		    digit = c - '0';
		  else if (c >= 'a' && c <= 'f')
		    digit = c - 'a' + 10;
		  else if (c >= 'A' && c <= 'F')
		    digit = c - 'A' + 10;
		  else
		    break;
		  value = 16 * value + digit;
		}
	    }
	  else if (c_isdigit (c))
	    {
	      value = c - '0';
	      while (*s < end
		     && (c = *(*s)++, c_isdigit (c)))
		value = 8 * value + c - '0';
	    }
	}
      else
	{
	  value = c - '0';
	  while (*s < end
		 && (c = *(*s)++, c_isdigit (c)))
	    value = 10 * value + c - '0';
	}

      if (*s < end)
	*s = *s - 1;
      *ival = value;
      c = XBM_TK_NUMBER;
    }
  else if (c_isalpha (c) || c == '_')
    {
      *sval++ = c;
      while (*s < end
	     && (c = *(*s)++, (c_isalnum (c) || c == '_')))
	*sval++ = c;
      *sval = 0;
      if (*s < end)
	*s = *s - 1;
      c = XBM_TK_IDENT;
    }
  else if (c == '/' && **s == '*')
    {
      /* C-style comment.  */
      ++*s;
      while (**s && (**s != '*' || *(*s + 1) != '/'))
	++*s;
      if (**s)
	{
	  *s += 2;
	  goto loop;
	}
    }

  return c;
}

#ifdef HAVE_NTGUI
/* Create a Windows bitmap from X bitmap data: */
static HBITMAP
w32_create_pixmap_from_bitmap_data (int width, int height, char *data)
{
  static unsigned char swap_nibble[16]
    = { 0x0, 0x8, 0x4, 0xc,    /* 0000 1000 0100 1100 */
	0x2, 0xa, 0x6, 0xe,    /* 0010 1010 0110 1110 */
	0x1, 0x9, 0x5, 0xd,    /* 0001 1001 0101 1101 */
	0x3, 0xb, 0x7, 0xf };  /* 0011 1011 0111 1111 */
  int i, j, w1, w2;
  unsigned char *bits, *p;
  HBITMAP bmp;

  w1 = (width + 7) / 8;         /* nb of 8bits elt in X bitmap */
  w2 = ((width + 15) / 16) * 2; /* nb of 16bits elt in W32 bitmap */
  bits = alloca (height * w2);
  memset (bits, 0, height * w2);
  for (i = 0; i < height; i++)
    {
      p = bits + i*w2;
      for (j = 0; j < w1; j++)
	{
	  /* Bitswap XBM bytes to match how Windows does things.  */
	  unsigned char c = *data++;
	  *p++ = (unsigned char)((swap_nibble[c & 0xf] << 4)
				 | (swap_nibble[(c>>4) & 0xf]));
	}
    }
  bmp = CreateBitmap (width, height, 1, 1, (char *) bits);

  return bmp;
}

static void
convert_mono_to_color_image (struct frame *f, struct image *img,
			     COLORREF foreground, COLORREF background)
{
  HDC hdc, old_img_dc, new_img_dc;
  HGDIOBJ old_prev, new_prev;
  HBITMAP new_pixmap;

  hdc = get_frame_dc (f);
  old_img_dc = CreateCompatibleDC (hdc);
  new_img_dc = CreateCompatibleDC (hdc);
  new_pixmap = CreateCompatibleBitmap (hdc, img->width, img->height);
  release_frame_dc (f, hdc);
  old_prev = SelectObject (old_img_dc, img->pixmap);
  new_prev = SelectObject (new_img_dc, new_pixmap);
  /* Windows convention for mono bitmaps is black = background,
     white = foreground.  */
  SetTextColor (new_img_dc, background);
  SetBkColor (new_img_dc, foreground);

  BitBlt (new_img_dc, 0, 0, img->width, img->height, old_img_dc,
	  0, 0, SRCCOPY);

  SelectObject (old_img_dc, old_prev);
  SelectObject (new_img_dc, new_prev);
  DeleteDC (old_img_dc);
  DeleteDC (new_img_dc);
  DeleteObject (img->pixmap);
  if (new_pixmap == 0)
    fprintf (stderr, "Failed to convert image to color.\n");
  else
    img->pixmap = new_pixmap;
}
# define XBM_BIT_SHUFFLE(b) (~(b))
#else
# define XBM_BIT_SHUFFLE(b) (b)
#endif /* HAVE_NTGUI */


static void
Create_Pixmap_From_Bitmap_Data (struct frame *f, struct image *img, char *data,
				RGB_PIXEL_COLOR fg, RGB_PIXEL_COLOR bg,
				bool non_default_colors)
{
#ifdef HAVE_NTGUI
  img->pixmap
    = w32_create_pixmap_from_bitmap_data (img->width, img->height, data);

  /* If colors were specified, transfer the bitmap to a color one.  */
  if (non_default_colors)
    convert_mono_to_color_image (f, img, fg, bg);

#elif defined (HAVE_NS)
  img->pixmap = ns_image_from_XBM((unsigned char *)data, img->width, img->height);

#else
  img->pixmap =
   (x_check_image_size (0, img->width, img->height)
    ? XCreatePixmapFromBitmapData (FRAME_X_DISPLAY (f),
				   FRAME_X_WINDOW (f),
				   data,
				   img->width, img->height,
				   fg, bg,
				   DefaultDepthOfScreen (FRAME_X_SCREEN (f)))
    : NO_PIXMAP);
#endif /* !HAVE_NTGUI && !HAVE_NS */
}



/* Replacement for XReadBitmapFileData which isn't available under old
   X versions.  CONTENTS is a pointer to a buffer to parse; END is the
   buffer's end.  Set *WIDTH and *HEIGHT to the width and height of
   the image.  Return in *DATA the bitmap data allocated with xmalloc.
   Value is true if successful.  DATA null means just test if
   CONTENTS looks like an in-memory XBM file.  If INHIBIT_IMAGE_ERROR,
   inhibit the call to image_error when the image size is invalid (the
   bitmap remains unread).  */

static bool
xbm_read_bitmap_data (struct frame *f, unsigned char *contents, unsigned char *end,
		      int *width, int *height, char **data,
		      bool inhibit_image_error)
{
  unsigned char *s = contents;
  char buffer[BUFSIZ];
  bool padding_p = 0;
  bool v10 = 0;
  int bytes_per_line, i, nbytes;
  char *p;
  int value;
  int LA1;

#define match() \
     LA1 = xbm_scan (&s, end, buffer, &value)

#define expect(TOKEN)		\
  do				\
    {				\
      if (LA1 != (TOKEN)) 	\
	goto failure;		\
      match ();			\
    }				\
  while (0)

#define expect_ident(IDENT)					\
     if (LA1 == XBM_TK_IDENT && strcmp (buffer, (IDENT)) == 0)	\
       match ();						\
     else							\
       goto failure

  *width = *height = -1;
  if (data)
    *data = NULL;
  LA1 = xbm_scan (&s, end, buffer, &value);

  /* Parse defines for width, height and hot-spots.  */
  while (LA1 == '#')
    {
      match ();
      expect_ident ("define");
      expect (XBM_TK_IDENT);

      if (LA1 == XBM_TK_NUMBER)
	{
	  char *q = strrchr (buffer, '_');
	  q = q ? q + 1 : buffer;
	  if (strcmp (q, "width") == 0)
	    *width = value;
	  else if (strcmp (q, "height") == 0)
	    *height = value;
	}
      expect (XBM_TK_NUMBER);
    }

  if (!check_image_size (f, *width, *height))
    {
      if (!inhibit_image_error)
	image_error ("Invalid image size (see `max-image-size')", Qnil, Qnil);
      goto failure;
    }
  else if (data == NULL)
    goto success;

  /* Parse bits.  Must start with `static'.  */
  expect_ident ("static");
  if (LA1 == XBM_TK_IDENT)
    {
      if (strcmp (buffer, "unsigned") == 0)
	{
	  match ();
	  expect_ident ("char");
	}
      else if (strcmp (buffer, "short") == 0)
	{
	  match ();
	  v10 = 1;
	  if (*width % 16 && *width % 16 < 9)
	    padding_p = 1;
	}
      else if (strcmp (buffer, "char") == 0)
	match ();
      else
	goto failure;
    }
  else
    goto failure;

  expect (XBM_TK_IDENT);
  expect ('[');
  expect (']');
  expect ('=');
  expect ('{');

  if (! x_check_image_size (0, *width, *height))
    {
      if (!inhibit_image_error)
	image_error ("Image too large (%dx%d)",
		     make_number (*width), make_number (*height));
      goto failure;
    }
  bytes_per_line = (*width + 7) / 8 + padding_p;
  nbytes = bytes_per_line * *height;
  p = *data = xmalloc (nbytes);

  if (v10)
    {
      for (i = 0; i < nbytes; i += 2)
	{
	  int val = value;
	  expect (XBM_TK_NUMBER);

	  *p++ = XBM_BIT_SHUFFLE (val);
	  if (!padding_p || ((i + 2) % bytes_per_line))
	    *p++ = XBM_BIT_SHUFFLE (value >> 8);

	  if (LA1 == ',' || LA1 == '}')
	    match ();
	  else
	    goto failure;
	}
    }
  else
    {
      for (i = 0; i < nbytes; ++i)
	{
	  int val = value;
	  expect (XBM_TK_NUMBER);

	  *p++ = XBM_BIT_SHUFFLE (val);

	  if (LA1 == ',' || LA1 == '}')
	    match ();
	  else
	    goto failure;
	}
    }

 success:
  return 1;

 failure:

  if (data && *data)
    {
      xfree (*data);
      *data = NULL;
    }
  return 0;

#undef match
#undef expect
#undef expect_ident
}


/* Load XBM image IMG which will be displayed on frame F from buffer
   CONTENTS.  END is the end of the buffer.  Value is true if
   successful.  */

static bool
xbm_load_image (struct frame *f, struct image *img, unsigned char *contents,
		unsigned char *end)
{
  bool rc;
  char *data;
  bool success_p = 0;

  rc = xbm_read_bitmap_data (f, contents, end, &img->width, &img->height,
			     &data, 0);
  if (rc)
    {
      unsigned long foreground = FRAME_FOREGROUND_PIXEL (f);
      unsigned long background = FRAME_BACKGROUND_PIXEL (f);
      bool non_default_colors = 0;
      Lisp_Object value;

      eassert (img->width > 0 && img->height > 0);

      /* Get foreground and background colors, maybe allocate colors.  */
      value = image_spec_value (img->spec, QCforeground, NULL);
      if (!NILP (value))
	{
	  foreground = x_alloc_image_color (f, img, value, foreground);
	  non_default_colors = 1;
	}
      value = image_spec_value (img->spec, QCbackground, NULL);
      if (!NILP (value))
	{
	  background = x_alloc_image_color (f, img, value, background);
	  img->background = background;
	  img->background_valid = 1;
	  non_default_colors = 1;
	}

      Create_Pixmap_From_Bitmap_Data (f, img, data,
				      foreground, background,
				      non_default_colors);
      xfree (data);

      if (img->pixmap == NO_PIXMAP)
	{
	  x_clear_image (f, img);
	  image_error ("Unable to create X pixmap for `%s'", img->spec, Qnil);
	}
      else
	success_p = 1;
    }
  else
    image_error ("Error loading XBM image `%s'", img->spec, Qnil);

  return success_p;
}


/* Value is true if DATA looks like an in-memory XBM file: */
static bool
xbm_file_p(Lisp_Object data)
{
  int w, h;
  return (STRINGP (data)
	  && xbm_read_bitmap_data (NULL, SDATA (data),
				   (SDATA (data) + SBYTES (data)),
				   &w, &h, NULL, 1));
}


/* Fill image IMG which is used on frame F with pixmap data.  Value is
   true if successful.  */

static bool
xbm_load (struct frame *f, struct image *img)
{
  bool success_p = 0;
  Lisp_Object file_name;

  eassert (xbm_image_p (img->spec));

  /* If IMG->spec specifies a file name, create a non-file spec from it.  */
  file_name = image_spec_value (img->spec, QCfile, NULL);
  if (STRINGP (file_name))
    {
      Lisp_Object file;
      unsigned char *contents;
      ptrdiff_t size;

      file = x_find_image_file (file_name);
      if (!STRINGP (file))
	{
	  image_error ("Cannot find image file `%s'", file_name, Qnil);
	  return 0;
	}

      contents = slurp_file (SSDATA (file), &size);
      if (contents == NULL)
	{
	  image_error ("Error loading XBM image `%s'", img->spec, Qnil);
	  return 0;
	}

      success_p = xbm_load_image (f, img, contents, contents + size);
      xfree (contents);
    }
  else
    {
      struct image_keyword fmt[XBM_LAST];
      Lisp_Object data;
      unsigned long foreground = FRAME_FOREGROUND_PIXEL(f);
      unsigned long background = FRAME_BACKGROUND_PIXEL(f);
      bool non_default_colors = 0;
      char *bits;
      bool parsed_p;
      bool in_memory_file_p = 0;

      /* See if data looks like an in-memory XBM file: */
      data = image_spec_value(img->spec, QCdata, NULL);
      in_memory_file_p = xbm_file_p(data);

      /* Parse the image specification: */
      memcpy(fmt, xbm_format, sizeof(fmt));
      parsed_p = parse_image_spec(img->spec, fmt, XBM_LAST, Qxbm);
      eassert(parsed_p);

      if (parsed_p) {
        ; /* ??? */
      }

      /* Get specified width, and height: */
      if (!in_memory_file_p)
	{
	  img->width = XFASTINT(fmt[XBM_WIDTH].value);
	  img->height = XFASTINT(fmt[XBM_HEIGHT].value);
	  eassert((img->width > 0) && (img->height > 0));
	  if (!check_image_size(f, img->width, img->height))
	    {
	      image_error("Invalid image size (see `max-image-size')",
			  Qnil, Qnil);
	      return 0;
	    }
	}

      /* Get foreground and background colors, maybe allocate colors.  */
      if (fmt[XBM_FOREGROUND].count
	  && STRINGP (fmt[XBM_FOREGROUND].value))
	{
	  foreground = x_alloc_image_color (f, img, fmt[XBM_FOREGROUND].value,
					    foreground);
	  non_default_colors = 1;
	}

      if (fmt[XBM_BACKGROUND].count
	  && STRINGP (fmt[XBM_BACKGROUND].value))
	{
	  background = x_alloc_image_color (f, img, fmt[XBM_BACKGROUND].value,
					    background);
	  non_default_colors = 1;
	}

      if (in_memory_file_p)
	success_p = xbm_load_image (f, img, SDATA (data),
				    (SDATA (data)
				     + SBYTES (data)));
      else
	{
	  if (VECTORP (data))
	    {
	      int i;
	      char *p;
	      int nbytes = (img->width + BITS_PER_CHAR - 1) / BITS_PER_CHAR;

	      p = bits = alloca (nbytes * img->height);
	      for (i = 0; i < img->height; ++i, p += nbytes)
		{
		  Lisp_Object line = AREF (data, i);
		  if (STRINGP (line))
		    memcpy (p, SDATA (line), nbytes);
		  else
		    memcpy (p, bool_vector_data (line), nbytes);
		}
	    }
	  else if (STRINGP (data))
	    bits = SSDATA (data);
	  else
	    bits = (char *) bool_vector_data (data);

#ifdef HAVE_NTGUI
          {
            char *invertedBits;
            int nbytes, i;
            /* Windows mono bitmaps are reversed compared with X.  */
            invertedBits = bits;
            nbytes = (img->width + BITS_PER_CHAR - 1) / BITS_PER_CHAR
              * img->height;
            bits = alloca (nbytes);
            for (i = 0; i < nbytes; i++)
              bits[i] = XBM_BIT_SHUFFLE (invertedBits[i]);
          }
#endif
	  /* Create the pixmap.  */

	  if (x_check_image_size (0, img->width, img->height))
	    Create_Pixmap_From_Bitmap_Data (f, img, bits,
					    foreground, background,
					    non_default_colors);
	  else
	    img->pixmap = NO_PIXMAP;

	  if (img->pixmap)
	    success_p = 1;
	  else
	    {
	      image_error ("Unable to create pixmap for XBM image `%s'",
			   img->spec, Qnil);
	      x_clear_image (f, img);
	    }
	}
    }

  return success_p;
}



/***********************************************************************
			      XPM images
 ***********************************************************************/

#if defined (HAVE_XPM) || defined (MAC_OS) || defined (HAVE_NS)

static bool xpm_image_p (Lisp_Object object);
static bool xpm_load (struct frame *f, struct image *img);

#endif /* HAVE_XPM || MAC_OS || HAVE_NS */

#ifdef HAVE_XPM
# ifdef HAVE_NTGUI
/* Indicate to xpm.h that we do NOT have Xlib.  */
#  define FOR_MSW
/* simx.h in xpm defines XColor and XImage differently than Emacs.  */
/* It also defines Display the same way as Emacs, but gcc 3.3 still barfs.  */
#  define XColor xpm_XColor
#  define XImage xpm_XImage
#  define Display xpm_Display
#  define PIXEL_ALREADY_TYPEDEFED
#  include "X11/xpm.h"
#  undef FOR_MSW
#  undef XColor
#  undef XImage
#  undef Display
#  undef PIXEL_ALREADY_TYPEDEFED
# else
#  include "X11/xpm.h"
# endif /* HAVE_NTGUI */
#endif /* HAVE_XPM */

#if defined (HAVE_XPM) || defined (MAC_OS) || defined (HAVE_NS)
/* The symbol `xpm' identifying XPM-format images: */
static Lisp_Object Qxpm;

/* Indices of image specification fields in xpm_format, below: */
enum xpm_keyword_index
{
  XPM_TYPE,
  XPM_FILE,
  XPM_DATA,
  XPM_ASCENT,
  XPM_MARGIN,
  XPM_RELIEF,
  XPM_ALGORITHM,
  XPM_HEURISTIC_MASK,
  XPM_MASK,
  XPM_COLOR_SYMBOLS,
  XPM_BACKGROUND,
  XPM_LAST
};

/* Vector of image_keyword structures describing the format
   of valid XPM image specifications: */
static const struct image_keyword xpm_format[XPM_LAST] =
{
  {":type",		IMAGE_SYMBOL_VALUE,			1},
  {":file",		IMAGE_STRING_VALUE,			0},
  {":data",		IMAGE_STRING_VALUE,			0},
  {":ascent",		IMAGE_ASCENT_VALUE,			0},
  {":margin",		IMAGE_NON_NEGATIVE_INTEGER_VALUE_OR_PAIR, 0},
  {":relief",		IMAGE_INTEGER_VALUE,			0},
  {":conversion",	IMAGE_DONT_CHECK_VALUE_TYPE,		0},
  {":heuristic-mask",	IMAGE_DONT_CHECK_VALUE_TYPE,		0},
  {":mask",		IMAGE_DONT_CHECK_VALUE_TYPE,		0},
  {":color-symbols",	IMAGE_DONT_CHECK_VALUE_TYPE,		0},
  {":background",	IMAGE_STRING_OR_NIL_VALUE,		0}
};

# if defined HAVE_NTGUI && defined WINDOWSNT
static bool init_xpm_functions (void);
# else
#  define init_xpm_functions NULL
# endif /* HAVE_NTGUI && WINDOWSNT */

/* Structure describing the image type XPM: */
static struct image_type xpm_type =
{
  &Qxpm,
  xpm_image_p,
  xpm_load,
  x_clear_image,
  init_xpm_functions,
  NULL
};

# ifdef HAVE_X_WINDOWS
/* Define ALLOC_XPM_COLORS if we can use Emacs' own color allocation
   functions for allocating image colors.  Our own functions handle
   color allocation fails more gracefully than the ones on the XPM lib. */
#  if defined XpmAllocColor && defined XpmFreeColors && defined XpmColorClosure
#   define ALLOC_XPM_COLORS
#  endif /* XpmAllocColor && XpmFreeColors && XpmColorClosure */
# endif /* HAVE_X_WINDOWS */

# ifdef ALLOC_XPM_COLORS
static struct xpm_cached_color *xpm_cache_color (struct frame *, char *,
                                                 XColor *, int);

/* An entry in a hash table used to cache color definitions of named
   colors.  This cache is necessary to speed up XPM image loading in
   case we do color allocations ourselves.  Without it, we would need
   a call to XParseColor per pixel in the image.  */
struct xpm_cached_color
{
  /* Next in collision chain.  */
  struct xpm_cached_color *next;

  /* Color definition (RGB and pixel color).  */
  XColor color;

  /* Color name.  */
  char name[FLEXIBLE_ARRAY_MEMBER];
};

/* The hash table used for the color cache, and its bucket vector size: */
#  define XPM_COLOR_CACHE_BUCKETS 1001
static struct xpm_cached_color **xpm_color_cache;

/* Initialize the color cache: */
static void xpm_init_color_cache(struct frame *f, XpmAttributes *attrs)
{
  size_t nbytes = XPM_COLOR_CACHE_BUCKETS * sizeof *xpm_color_cache;
  xpm_color_cache = xzalloc (nbytes);
  init_color_table ();

  if (attrs->valuemask & XpmColorSymbols)
    {
      int i;
      XColor color;

      for (i = 0; i < attrs->numsymbols; ++i)
	if (XParseColor (FRAME_X_DISPLAY (f), FRAME_X_COLORMAP (f),
			 attrs->colorsymbols[i].value, &color))
	  {
	    color.pixel = lookup_rgb_color (f, color.red, color.green,
					    color.blue);
	    xpm_cache_color (f, attrs->colorsymbols[i].name, &color, -1);
	  }
    }
}

/* Free the color cache: */
static void xpm_free_color_cache(void)
{
  struct xpm_cached_color *p, *next;
  int i;

  for (i = 0; i < XPM_COLOR_CACHE_BUCKETS; ++i)
    for (p = xpm_color_cache[i]; p; p = next)
      {
	next = p->next;
	xfree (p);
      }

  xfree (xpm_color_cache);
  xpm_color_cache = NULL;
  free_color_table ();
}

/* Return the bucket index for color named COLOR_NAME in color cache: */
static int xpm_color_bucket(char *color_name)
{
  EMACS_UINT hash = hash_string (color_name, strlen (color_name));
  return hash % XPM_COLOR_CACHE_BUCKETS;
}


/* On frame F, cache values COLOR for color with name COLOR_NAME.
   BUCKET, if >= 0, is a precomputed bucket index.  Value is the cache
   entry added.  */
static struct xpm_cached_color *
xpm_cache_color (struct frame *f, char *color_name, XColor *color, int bucket)
{
  size_t nbytes;
  struct xpm_cached_color *p;

  if (bucket < 0)
    bucket = xpm_color_bucket (color_name);

  nbytes = offsetof (struct xpm_cached_color, name) + strlen (color_name) + 1;
  p = xmalloc (nbytes);
  strcpy (p->name, color_name);
  p->color = *color;
  p->next = xpm_color_cache[bucket];
  xpm_color_cache[bucket] = p;
  return p;
}

/* Look up color COLOR_NAME for frame F in the color cache.  If found,
   return the cached definition in *COLOR.  Otherwise, make a new
   entry in the cache and allocate the color.  Value is false if color
   allocation failed.  */
static bool
xpm_lookup_color (struct frame *f, char *color_name, XColor *color)
{
  struct xpm_cached_color *p;
  int h = xpm_color_bucket (color_name);

  for (p = xpm_color_cache[h]; p; p = p->next)
    if (strcmp (p->name, color_name) == 0)
      break;

  if (p != NULL)
    *color = p->color;
  else if (XParseColor (FRAME_X_DISPLAY (f), FRAME_X_COLORMAP (f),
			color_name, color))
    {
      color->pixel = lookup_rgb_color (f, color->red, color->green,
				       color->blue);
      p = xpm_cache_color (f, color_name, color, h);
    }
  /* You get `opaque' at least from ImageMagick converting pbm to xpm
     with transparency, and it's useful.  */
  else if (strcmp ("opaque", color_name) == 0)
    {
      memset (color, 0, sizeof (XColor));  /* Is this necessary/correct?  */
      color->pixel = FRAME_FOREGROUND_PIXEL (f);
      p = xpm_cache_color (f, color_name, color, h);
    }

  return p != NULL;
}


/* Callback for allocating color COLOR_NAME.  Called from the XPM lib.
   CLOSURE is a pointer to the frame on which we allocate the
   color.  Return in *COLOR the allocated color.  Value is non-zero
   if successful.  */
static int
xpm_alloc_color (Display *dpy, Colormap cmap, char *color_name, XColor *color,
		 void *closure)
{
  return xpm_lookup_color (closure, color_name, color);
}


/* Callback for freeing NPIXELS colors contained in PIXELS.  CLOSURE
   is a pointer to the frame on which we allocate the color.  Value is
   non-zero if successful.  */

static int
xpm_free_colors (Display *dpy, Colormap cmap, Pixel *pixels, int npixels, void *closure)
{
  return 1;
}

# endif /* ALLOC_XPM_COLORS */


# ifdef WINDOWSNT
/* XPM library details: */
DEF_IMGLIB_FN (void, XpmFreeAttributes, (XpmAttributes *));
DEF_IMGLIB_FN (int, XpmCreateImageFromBuffer, (Display *, char *, xpm_XImage **,
					  xpm_XImage **, XpmAttributes *));
DEF_IMGLIB_FN (int, XpmReadFileToImage, (Display *, char *, xpm_XImage **,
				    xpm_XImage **, XpmAttributes *));
DEF_IMGLIB_FN (void, XImageFree, (xpm_XImage *));

static bool init_xpm_functions(void)
{
  HMODULE library;

  if (!(library = w32_delayed_load (Qxpm)))
    return 0;

  LOAD_IMGLIB_FN (library, XpmFreeAttributes);
  LOAD_IMGLIB_FN (library, XpmCreateImageFromBuffer);
  LOAD_IMGLIB_FN (library, XpmReadFileToImage);
  LOAD_IMGLIB_FN (library, XImageFree);
  return 1;
}

# endif /* WINDOWSNT */

# if defined HAVE_NTGUI && !defined WINDOWSNT
/* Glue for code below: */
#  define fn_XpmReadFileToImage XpmReadFileToImage
#  define fn_XpmCreateImageFromBuffer XpmCreateImageFromBuffer
#  define fn_XImageFree XImageFree
#  define fn_XpmFreeAttributes XpmFreeAttributes
# endif /* HAVE_NTGUI && !WINDOWSNT */

/* Value is true if COLOR_SYMBOLS is a valid color symbols list
   for XPM images.  Such a list must consist of conses whose car and
   cdr are strings.  */
static bool xpm_valid_color_symbols_p(Lisp_Object color_symbols)
{
  while (CONSP (color_symbols))
    {
      Lisp_Object sym = XCAR (color_symbols);
      if (!CONSP (sym)
	  || !STRINGP (XCAR (sym))
	  || !STRINGP (XCDR (sym)))
	break;
      color_symbols = XCDR (color_symbols);
    }

  return NILP (color_symbols);
}


/* Value is true if OBJECT is a valid XPM image specification: */
static bool xpm_image_p(Lisp_Object object)
{
  struct image_keyword fmt[XPM_LAST];
  memcpy (fmt, xpm_format, sizeof fmt);
  return (parse_image_spec (object, fmt, XPM_LAST, Qxpm)
	  /* Either `:file' or `:data' must be present.  */
	  && fmt[XPM_FILE].count + fmt[XPM_DATA].count == 1
	  /* Either no `:color-symbols' or it's a list of conses
	     whose car and cdr are strings.  */
	  && (fmt[XPM_COLOR_SYMBOLS].count == 0
	      || xpm_valid_color_symbols_p (fmt[XPM_COLOR_SYMBOLS].value)));
}

#endif /* HAVE_XPM || MAC_OS || HAVE_NS */

#if defined HAVE_XPM && defined HAVE_X_WINDOWS && !defined USE_GTK
ptrdiff_t
x_create_bitmap_from_xpm_data (struct frame *f, const char **bits)
{
  Display_Info *dpyinfo = FRAME_DISPLAY_INFO (f);
  ptrdiff_t id;
  int rc;
  XpmAttributes attrs;
  Pixmap bitmap, mask;

  memset (&attrs, 0, sizeof attrs);

  attrs.visual = FRAME_X_VISUAL (f);
  attrs.colormap = FRAME_X_COLORMAP (f);
  attrs.valuemask |= XpmVisual;
  attrs.valuemask |= XpmColormap;

  rc = XpmCreatePixmapFromData (FRAME_X_DISPLAY (f), FRAME_X_WINDOW (f),
				(char **) bits, &bitmap, &mask, &attrs);
  if (rc != XpmSuccess)
    {
      XpmFreeAttributes (&attrs);
      return -1;
    }

  id = x_allocate_bitmap_record (f);
  dpyinfo->bitmaps[id - 1].pixmap = bitmap;
  dpyinfo->bitmaps[id - 1].have_mask = true;
  dpyinfo->bitmaps[id - 1].mask = mask;
  dpyinfo->bitmaps[id - 1].file = NULL;
  dpyinfo->bitmaps[id - 1].height = attrs.height;
  dpyinfo->bitmaps[id - 1].width = attrs.width;
  dpyinfo->bitmaps[id - 1].depth = attrs.depth;
  dpyinfo->bitmaps[id - 1].refcount = 1;

  XpmFreeAttributes (&attrs);
  return id;
}
#endif /* defined (HAVE_XPM) && defined (HAVE_X_WINDOWS) */

/* Load image IMG which will be displayed on frame F.  Value is
   true if successful.  */
#ifdef HAVE_XPM
static bool xpm_load(struct frame *f, struct image *img)
{
  int rc;
  XpmAttributes attrs;
  Lisp_Object specified_file, color_symbols;
#ifdef HAVE_NTGUI
  HDC hdc;
  xpm_XImage * xpm_image = NULL, * xpm_mask = NULL;
#endif /* HAVE_NTGUI */

  /* Configure the XPM lib.  Use the visual of frame F.  Allocate
     close colors.  Return colors allocated.  */
  memset (&attrs, 0, sizeof attrs);

#ifndef HAVE_NTGUI
  attrs.visual = FRAME_X_VISUAL (f);
  attrs.colormap = FRAME_X_COLORMAP (f);
  attrs.valuemask |= XpmVisual;
  attrs.valuemask |= XpmColormap;
#endif /* HAVE_NTGUI */

#ifdef ALLOC_XPM_COLORS
  /* Allocate colors with our own functions which handle
     failing color allocation more gracefully.  */
  attrs.color_closure = f;
  attrs.alloc_color = xpm_alloc_color;
  attrs.free_colors = xpm_free_colors;
  attrs.valuemask |= XpmAllocColor | XpmFreeColors | XpmColorClosure;
#else /* not ALLOC_XPM_COLORS */
  /* Let the XPM lib allocate colors.  */
  attrs.valuemask |= XpmReturnAllocPixels;
# ifdef XpmAllocCloseColors
  attrs.alloc_close_colors = 1;
  attrs.valuemask |= XpmAllocCloseColors;
# else /* not XpmAllocCloseColors */
  attrs.closeness = 600;
  attrs.valuemask |= XpmCloseness;
# endif /* not XpmAllocCloseColors */
#endif /* ALLOC_XPM_COLORS */

  /* If image specification contains symbolic color definitions, add
     these to `attrs'.  */
  color_symbols = image_spec_value (img->spec, QCcolor_symbols, NULL);
  if (CONSP (color_symbols))
    {
      Lisp_Object tail;
      XpmColorSymbol *xpm_syms;
      int i, size;

      attrs.valuemask |= XpmColorSymbols;

      /* Count number of symbols.  */
      attrs.numsymbols = 0;
      for (tail = color_symbols; CONSP (tail); tail = XCDR (tail))
	++attrs.numsymbols;

      /* Allocate an XpmColorSymbol array.  */
      size = attrs.numsymbols * sizeof *xpm_syms;
      xpm_syms = alloca (size);
      memset (xpm_syms, 0, size);
      attrs.colorsymbols = xpm_syms;

      /* Fill the color symbol array.  */
      for (tail = color_symbols, i = 0;
	   CONSP (tail);
	   ++i, tail = XCDR (tail))
	{
	  Lisp_Object name;
	  Lisp_Object color;
	  char *empty_string = (char *) "";

	  if (!CONSP (XCAR (tail)))
	    {
	      xpm_syms[i].name = empty_string;
	      xpm_syms[i].value = empty_string;
	      continue;
	    }
	  name = XCAR (XCAR (tail));
	  color = XCDR (XCAR (tail));
	  if (STRINGP (name))
	    {
	      xpm_syms[i].name = alloca (SCHARS (name) + 1);
	      strcpy (xpm_syms[i].name, SSDATA (name));
	    }
	  else
	    xpm_syms[i].name = empty_string;
	  if (STRINGP (color))
	    {
	      xpm_syms[i].value = alloca (SCHARS (color) + 1);
	      strcpy (xpm_syms[i].value, SSDATA (color));
	    }
	  else
	    xpm_syms[i].value = empty_string;
	}
    }

  /* Create a pixmap for the image, either from a file, or from a
     string buffer containing data in the same format as an XPM file.  */
#ifdef ALLOC_XPM_COLORS
  xpm_init_color_cache (f, &attrs);
#endif /* ALLOC_XPM_COLORS */

  specified_file = image_spec_value (img->spec, QCfile, NULL);

#ifdef HAVE_NTGUI
  {
    HDC frame_dc = get_frame_dc (f);
    hdc = CreateCompatibleDC (frame_dc);
    release_frame_dc (f, frame_dc);
  }
#endif /* HAVE_NTGUI */

  if (STRINGP (specified_file))
    {
      Lisp_Object file = x_find_image_file (specified_file);
      if (!STRINGP (file))
	{
	  image_error ("Cannot find image file `%s'", specified_file, Qnil);
#ifdef ALLOC_XPM_COLORS
	  xpm_free_color_cache ();
#endif /* ALLOC_XPM_COLORS */
	  return 0;
	}

#ifdef HAVE_NTGUI
# ifdef WINDOWSNT
      /* FILE is encoded in UTF-8, but image libraries on Windows
	 support neither UTF-8 nor UTF-16 encoded file names.  So we
	 need to re-encode it in ANSI.  */
      file = ansi_encode_filename (file);
# endif /* WINDOWSNT */
      /* XpmReadFileToPixmap is not available in the Windows port of
	 libxpm.  But XpmReadFileToImage almost does what we want.  */
      rc = fn_XpmReadFileToImage (&hdc, SDATA (file),
				  &xpm_image, &xpm_mask,
				  &attrs);
#else
      rc = XpmReadFileToImage (FRAME_X_DISPLAY (f), SSDATA (file),
			       &img->ximg, &img->mask_img,
			       &attrs);
#endif /* HAVE_NTGUI */
    }
  else
    {
      Lisp_Object buffer = image_spec_value (img->spec, QCdata, NULL);
      if (!STRINGP (buffer))
	{
	  image_error ("Invalid image data `%s'", buffer, Qnil);
#ifdef ALLOC_XPM_COLORS
	  xpm_free_color_cache ();
#endif /* ALLOC_XPM_COLORS */
	  return 0;
	}
#ifdef HAVE_NTGUI
      /* XpmCreatePixmapFromBuffer is not available in the Windows port
	 of libxpm.  But XpmCreateImageFromBuffer almost does what we want.  */
      rc = fn_XpmCreateImageFromBuffer (&hdc, SDATA (buffer),
					&xpm_image, &xpm_mask,
					&attrs);
#else
      rc = XpmCreateImageFromBuffer (FRAME_X_DISPLAY (f), SSDATA (buffer),
				     &img->ximg, &img->mask_img,
				     &attrs);
#endif /* HAVE_NTGUI */
    }

#ifdef HAVE_X_WINDOWS
  if (rc == XpmSuccess)
    {
      img->pixmap = XCreatePixmap (FRAME_X_DISPLAY (f), FRAME_X_WINDOW (f),
				   img->ximg->width, img->ximg->height,
				   img->ximg->depth);
      if (img->pixmap == NO_PIXMAP)
	{
	  x_clear_image (f, img);
	  rc = XpmNoMemory;
	}
      else if (img->mask_img)
	{
	  img->mask = XCreatePixmap (FRAME_X_DISPLAY (f), FRAME_X_WINDOW (f),
				     img->mask_img->width,
				     img->mask_img->height,
				     img->mask_img->depth);
	  if (img->mask == NO_PIXMAP)
	    {
	      x_clear_image (f, img);
	      rc = XpmNoMemory;
	    }
	}
    }
#endif

  if (rc == XpmSuccess)
    {
#if defined (COLOR_TABLE_SUPPORT) && defined (ALLOC_XPM_COLORS)
      img->colors = colors_in_color_table (&img->ncolors);
#else /* not ALLOC_XPM_COLORS */
      int i;

#ifdef HAVE_NTGUI
      /* W32 XPM uses XImage to wrap what W32 Emacs calls a Pixmap,
	 plus some duplicate attributes.  */
      if (xpm_image && xpm_image->bitmap)
	{
	  img->pixmap = xpm_image->bitmap;
	  /* XImageFree in libXpm frees XImage struct without destroying
	     the bitmap, which is what we want.  */
	  fn_XImageFree (xpm_image);
	}
      if (xpm_mask && xpm_mask->bitmap)
	{
	  /* The mask appears to be inverted compared with what we expect.
	     TODO: invert our expectations.  See other places where we
	     have to invert bits because our idea of masks is backwards.  */
	  HGDIOBJ old_obj;
	  old_obj = SelectObject (hdc, xpm_mask->bitmap);

	  PatBlt (hdc, 0, 0, xpm_mask->width, xpm_mask->height, DSTINVERT);
	  SelectObject (hdc, old_obj);

	  img->mask = xpm_mask->bitmap;
	  fn_XImageFree (xpm_mask);
	  DeleteDC (hdc);
	}

      DeleteDC (hdc);
#endif /* HAVE_NTGUI */

      /* Remember allocated colors.  */
      img->colors = xnmalloc (attrs.nalloc_pixels, sizeof *img->colors);
      img->ncolors = attrs.nalloc_pixels;
      for (i = 0; i < attrs.nalloc_pixels; ++i)
	{
	  img->colors[i] = attrs.alloc_pixels[i];
# ifdef DEBUG_X_COLORS
	  register_color(img->colors[i]);
# endif /* DEBUG_X_COLORS */
	}
#endif /* not ALLOC_XPM_COLORS */

      img->width = attrs.width;
      img->height = attrs.height;
      eassert(img->width > 0 && img->height > 0);

      /* The call to XpmFreeAttributes below frees attrs.alloc_pixels: */
#ifdef HAVE_NTGUI
      fn_XpmFreeAttributes(&attrs);
#else
      XpmFreeAttributes(&attrs);
#endif /* HAVE_NTGUI */

#ifdef HAVE_X_WINDOWS
      /* Maybe fill in the background field while we have ximg handy: */
      IMAGE_BACKGROUND(img, f, img->ximg);
      if (img->mask_img)
	/* Fill in the background_transparent field while we have the
	   mask handy.  */
	image_background_transparent(img, f, img->mask_img);
#endif /* HAVE_X_WINDOWS */
    }
  else
    {
#ifdef HAVE_NTGUI
      DeleteDC(hdc);
#endif /* HAVE_NTGUI */

      switch (rc)
	{
	case XpmOpenFailed:
	  image_error("Error opening XPM file (%s)", img->spec, Qnil);
	  break;
	case XpmFileInvalid:
	  image_error("Invalid XPM file (%s)", img->spec, Qnil);
	  break;
	case XpmNoMemory:
	  image_error("Out of memory (%s)", img->spec, Qnil);
	  break;
	case XpmColorFailed:
	  image_error("Color allocation error (%s)", img->spec, Qnil);
	  break;
	default:
	  image_error("Unknown error (%s)", img->spec, Qnil);
	  break;
	}
    }

# ifdef ALLOC_XPM_COLORS
  xpm_free_color_cache();
# endif /* ALLOC_XPM_COLORS */
  return (rc == XpmSuccess);
}
#endif /* HAVE_XPM */

#if defined(MAC_OS) || (defined(HAVE_NS) && !defined(HAVE_XPM))
/* XPM support functions for MAC_OS/NS where libxpm is not available.
   Only XPM version 3 (without any extensions) is supported.  */
static void xpm_put_color_table_v (Lisp_Object, const unsigned char *,
                                   int, Lisp_Object);
static Lisp_Object xpm_get_color_table_v (Lisp_Object,
                                          const unsigned char *, int);
static void xpm_put_color_table_h (Lisp_Object, const unsigned char *,
                                   int, Lisp_Object);
static Lisp_Object xpm_get_color_table_h (Lisp_Object,
                                          const unsigned char *, int);

/* Tokens returned from xpm_scan: */
enum xpm_token
{
  XPM_TK_IDENT = 256,
  XPM_TK_STRING,
  XPM_TK_EOF
};

/* Scan an XPM data and return a character (< 256) or a token defined
   by enum xpm_token above.  *S and END are the start (inclusive) and
   the end (exclusive) addresses of the data, respectively.  Advance
   *S while scanning.  If token is either XPM_TK_IDENT or
   XPM_TK_STRING, *BEG and *LEN are set to the start address and the
   length of the corresponding token, respectively.  */

static int
xpm_scan(const unsigned char **s, const unsigned char *end,
         const unsigned char **beg, ptrdiff_t *len)
{
  int c IF_LINT(= 0);

  while (*s < end)
    {
      /* Skip white-space: */
      while (*s < end && (c = *(*s)++, c_isspace(c)))
	;

      /* gnus-pointer.xpm uses '-' in its identifier.
	 sb-dir-plus.xpm uses '+' in its identifier.  */
      if (c_isalpha(c) || c == '_' || c == '-' || c == '+')
	{
	  *beg = *s - 1;
	  while (*s < end
		 && (c = **s, c_isalnum(c)
		     || c == '_' || c == '-' || c == '+'))
	      ++*s;
	  *len = *s - *beg;
	  return XPM_TK_IDENT;
	}
      else if (c == '"')
	{
	  *beg = *s;
	  while (*s < end && **s != '"')
	    ++*s;
	  *len = *s - *beg;
	  if (*s < end)
	    ++*s;
	  return XPM_TK_STRING;
	}
      else if (c == '/')
	{
	  if (*s < end && **s == '*')
	    {
	      /* C-style comment.  */
	      ++*s;
	      do {
		  while (*s < end && *(*s)++ != '*')
		    ;
              } while (*s < end && **s != '/');
	      if (*s < end)
		++*s;
	    }
	  else
	    return c;
	}
      else
	return c;
    }

  return XPM_TK_EOF;
}

/* Functions for color table lookup in XPM data.  A key is a string
   specifying the color of each pixel in XPM data.  A value is either
   an integer that specifies a pixel color, Qt that specifies
   transparency, or Qnil for the unspecified color.  If the length of
   the key string is one, a vector is used as a table.  Otherwise, a
   hash table is used.  */
static Lisp_Object
xpm_make_color_table_v (void (**put_func) (Lisp_Object,
                                           const unsigned char *,
                                           int,
                                           Lisp_Object),
                        Lisp_Object (**get_func) (Lisp_Object,
                                                  const unsigned char *,
                                                  int))
{
  *put_func = xpm_put_color_table_v;
  *get_func = xpm_get_color_table_v;
  return Fmake_vector (make_number (256), Qnil);
}

static void
xpm_put_color_table_v (Lisp_Object color_table,
                       const unsigned char *chars_start,
                       int chars_len,
                       Lisp_Object color)
{
  ASET (color_table, *chars_start, color);
}

static Lisp_Object
xpm_get_color_table_v (Lisp_Object color_table,
                       const unsigned char *chars_start,
                       int chars_len)
{
  return AREF (color_table, *chars_start);
}

static Lisp_Object
xpm_make_color_table_h (void (**put_func) (Lisp_Object,
                                           const unsigned char *,
                                           int,
                                           Lisp_Object),
                        Lisp_Object (**get_func) (Lisp_Object,
                                                  const unsigned char *,
                                                  int))
{
  *put_func = xpm_put_color_table_h;
  *get_func = xpm_get_color_table_h;
  return make_hash_table (hashtest_equal, make_number (DEFAULT_HASH_SIZE),
			  make_float (DEFAULT_REHASH_SIZE),
			  make_float (DEFAULT_REHASH_THRESHOLD),
			  Qnil);
}

static void
xpm_put_color_table_h (Lisp_Object color_table,
                       const unsigned char *chars_start,
                       int chars_len,
                       Lisp_Object color)
{
  struct Lisp_Hash_Table *table = XHASH_TABLE (color_table);
  EMACS_UINT hash_code;
  Lisp_Object chars = make_unibyte_string((const char *)chars_start,
                                          chars_len);

  hash_lookup (table, chars, &hash_code);
  hash_put (table, chars, color, hash_code);
}

static Lisp_Object
xpm_get_color_table_h (Lisp_Object color_table,
                       const unsigned char *chars_start,
                       int chars_len)
{
  struct Lisp_Hash_Table *table = XHASH_TABLE (color_table);
  ptrdiff_t i =
    hash_lookup (table, make_unibyte_string((const char *)chars_start,
                                            chars_len), NULL);

  return ((i >= 0) ? HASH_VALUE(table, i) : Qnil);
}

enum xpm_color_key {
  XPM_COLOR_KEY_S,
  XPM_COLOR_KEY_M,
  XPM_COLOR_KEY_G4,
  XPM_COLOR_KEY_G,
  XPM_COLOR_KEY_C
};

static const char xpm_color_key_strings[][4] = {"s", "m", "g4", "g", "c"};

static int xpm_str_to_color_key (const char *s)
{
  int i;

  for (i = 0;
       i < sizeof xpm_color_key_strings / sizeof xpm_color_key_strings[0];
       i++)
    if (strcmp (xpm_color_key_strings[i], s) == 0)
      return i;
  return -1;
}

static bool
xpm_load_image(struct frame *f, struct image *img,
               const unsigned char *contents,
               const unsigned char *end)
{
  const unsigned char *s = contents, *beg, *str;
  unsigned char buffer[BUFSIZ];
  int width, height, x, y;
  int num_colors, chars_per_pixel;
  ptrdiff_t len;
  int LA1;
  void (*put_color_table)(Lisp_Object, const unsigned char *, int, Lisp_Object);
  Lisp_Object (*get_color_table)(Lisp_Object, const unsigned char *, int);
  Lisp_Object frame, color_symbols, color_table;
  int best_key;
  bool have_mask = false;
  XImagePtr ximg = NULL, mask_img = NULL;

  if (have_mask) {
    ; /* ??? */
  }

#define match() \
     LA1 = xpm_scan(&s, end, &beg, &len)

#define expect(TOKEN)		\
  do {				\
      if (LA1 != (TOKEN)) 	\
	goto failure;		\
      match();			\
  } while (0)

#define expect_ident(IDENT)					\
     if (LA1 == XPM_TK_IDENT \
         && strlen((IDENT)) == len && memcmp((IDENT), beg, len) == 0)	\
       match();						\
     else							\
       goto failure

  if (!(((end - s) >= 9) && memcmp(s, "/* XPM */", 9) == 0))
    goto failure;
  s += 9;
  match();
  expect_ident("static");
  expect_ident("char");
  expect('*');
  expect(XPM_TK_IDENT);
  expect('[');
  expect(']');
  expect('=');
  expect('{');
  expect(XPM_TK_STRING);
  if (len >= BUFSIZ)
    goto failure;
  memcpy(buffer, beg, len);
  buffer[len] = '\0';
  if ((sscanf((const char *)buffer, "%d %d %d %d", &width, &height,
              &num_colors, &chars_per_pixel) != 4)
      || (width <= 0) || (height <= 0)
      || (num_colors <= 0) || (chars_per_pixel <= 0))
    goto failure;

  if (!check_image_size(f, width, height))
    {
      image_error("Invalid image size (see `max-image-size')", Qnil, Qnil);
      goto failure;
    }

  if (!image_create_x_image_and_pixmap(f, img, width, height, 0, &ximg, 0)
#ifndef HAVE_NS
      || !image_create_x_image_and_pixmap(f, img, width, height, 1,
					  &mask_img, 1)
#endif /* !HAVE_NS */
      || (len >= BUFSIZ))
    {
      image_error("Image too large", Qnil, Qnil);
      goto failure;
    }

  expect(',');

  XSETFRAME(frame, f);
  if (!NILP(Fxw_display_color_p(frame)))
    best_key = XPM_COLOR_KEY_C;
  else if (!NILP(Fx_display_grayscale_p(frame)))
    best_key = ((XFASTINT(Fx_display_planes(frame)) > 2)
		? XPM_COLOR_KEY_G : XPM_COLOR_KEY_G4);
  else
    best_key = XPM_COLOR_KEY_M;

  color_symbols = image_spec_value(img->spec, QCcolor_symbols, NULL);
  if (chars_per_pixel == 1)
    color_table = xpm_make_color_table_v(&put_color_table,
                                         &get_color_table);
  else
    color_table = xpm_make_color_table_h(&put_color_table,
					 &get_color_table);

  while (num_colors-- > 0)
    {
      char *color;
      char *max_color = (char *)"";
      char *context_ptr;
      int key, next_key, max_key = 0;
      Lisp_Object symbol_color = Qnil, color_val;
      XColor cdef;

      expect(XPM_TK_STRING);
      if ((len <= chars_per_pixel) || (len >= (BUFSIZ + chars_per_pixel)))
	goto failure;
      memcpy(buffer, (beg + chars_per_pixel), (len - chars_per_pixel));
      buffer[len - chars_per_pixel] = '\0';

      str = (const unsigned char *)strtok_r((char *)buffer, " \t",
					    &context_ptr);
      if (str == NULL)
	goto failure;
      key = xpm_str_to_color_key((const char *)str);
      if (key < 0)
	goto failure;
      do {
        color = strtok_r(NULL, " \t", &context_ptr);
        if (color == NULL)
          goto failure;

        while ((str = (const unsigned char *)strtok_r(NULL, " \t",
						      &context_ptr)) != NULL)
          {
            next_key = xpm_str_to_color_key((const char *)str);
            if (next_key >= 0)
              break;
            color[strlen(color)] = ' ';
          }

        if (key == XPM_COLOR_KEY_S)
          {
            if (NILP(symbol_color))
              symbol_color = build_string(color);
          }
        else if ((max_key < key) && (key <= best_key))
          {
            max_key = key;
            max_color = color;
          }
        key = next_key;
      } while (str);

      color_val = Qnil;
      if (!NILP(color_symbols) && !NILP(symbol_color))
	{
	  Lisp_Object specified_color = Fassoc(symbol_color, color_symbols);

	  if (CONSP(specified_color) && STRINGP(XCDR(specified_color)))
	    {
	      if (xstrcasecmp(SSDATA(XCDR(specified_color)), "None") == 0)
		color_val = Qt;
	      else if (x_defined_color(f, SSDATA(XCDR(specified_color)),
                                       &cdef, 0))
		color_val = make_number(cdef.pixel);
	    }
	}
      if (NILP(color_val) && (max_key > 0))
	{
	  if (xstrcasecmp(max_color, "None") == 0)
	    color_val = Qt;
	  else if (x_defined_color(f, max_color, &cdef, 0))
	    color_val = make_number(cdef.pixel);
	}
      if (!NILP(color_val))
	(*put_color_table)(color_table, beg, chars_per_pixel, color_val);

      expect(',');
    }

  for (y = 0; y < height; y++)
    {
      expect(XPM_TK_STRING);
      str = beg;
      if (len < (width * chars_per_pixel))
	goto failure;
      for (x = 0; x < width; x++, str += chars_per_pixel)
	{
	  Lisp_Object color_val =
	    (*get_color_table)(color_table, str, chars_per_pixel);

	  XPutPixel(ximg, x, y,
		    (INTEGERP(color_val) ? XINT(color_val)
		     : FRAME_FOREGROUND_PIXEL(f)));
#ifndef HAVE_NS
	  XPutPixel(mask_img, x, y,
		    (!EQ(color_val, Qt) ? PIX_MASK_DRAW
		     : (have_mask = true, PIX_MASK_RETAIN)));
#else
          if (EQ (color_val, Qt))
            ns_set_alpha(ximg, x, y, 0);
#endif /* HAVE_NS */
	}
      if ((y + 1) < height)
	expect(',');
    }

  img->width = width;
  img->height = height;

  /* Maybe fill in the background field while we have ximg handy. */
  if (NILP(image_spec_value(img->spec, QCbackground, NULL)))
    IMAGE_BACKGROUND(img, f, ximg);

  image_put_x_image(f, img, ximg, 0);
#ifndef HAVE_NS
  if (have_mask)
    {
      /* Fill in the background_transparent field while we have the
	 mask handy.  */
      image_background_transparent(img, f, mask_img);

      image_put_x_image(f, img, mask_img, 1);
    }
  else
    {
      x_destroy_x_image(mask_img);
      x_clear_image_1(f, img, CLEAR_IMAGE_MASK);
    }
#endif /* HAVE_NS */
  return 1;

 failure:
  image_error("Invalid XPM file (%s)", img->spec, Qnil);
  x_destroy_x_image(ximg);
  x_destroy_x_image(mask_img);
  x_clear_image(f, img);
  return 0;

#undef match
#undef expect
#undef expect_ident
}

static bool
xpm_load(struct frame *f, struct image *img)
{
  bool success_p = 0;
  Lisp_Object file_name;

  /* If IMG->spec specifies a file name, create a non-file spec from it: */
  file_name = image_spec_value (img->spec, QCfile, NULL);
  if (STRINGP (file_name))
    {
      Lisp_Object file;
      unsigned char *contents;
      ptrdiff_t size;

      file = x_find_image_file (file_name);
      if (!STRINGP (file))
	{
	  image_error ("Cannot find image file `%s'", file_name, Qnil);
	  return 0;
	}

      contents = slurp_file (SSDATA (file), &size);
      if (contents == NULL)
	{
	  image_error ("Error loading XPM image `%s'", img->spec, Qnil);
	  return 0;
	}

      success_p = xpm_load_image (f, img, contents, contents + size);
      xfree (contents);
    }
  else
    {
      Lisp_Object data;

      data = image_spec_value (img->spec, QCdata, NULL);
      if (!STRINGP (data))
	{
	  image_error ("Invalid image data `%s'", data, Qnil);
	  return 0;
	}
      success_p = xpm_load_image (f, img, SDATA (data),
				  SDATA (data) + SBYTES (data));
    }

  return success_p;
}

#endif /* MAC_OS || (HAVE_NS && !HAVE_XPM) */



/***********************************************************************
			     Color table
 ***********************************************************************/

#ifdef COLOR_TABLE_SUPPORT

/* An entry in the color table mapping an RGB color to a pixel color.  */

struct ct_color
{
  int r, g, b;
  unsigned long pixel;

  /* Next in color table collision list.  */
  struct ct_color *next;
};

/* The bucket vector size to use.  Must be prime.  */

#define CT_SIZE 101

/* Value is a hash of the RGB color given by R, G, and B.  */

#define CT_HASH_RGB(R, G, B) (((R) << 16) ^ ((G) << 8) ^ (B))

/* The color hash table.  */

static struct ct_color **ct_table;

/* Number of entries in the color table.  */

static int ct_colors_allocated;
enum
{
  ct_colors_allocated_max =
    min (INT_MAX,
	 min (PTRDIFF_MAX, SIZE_MAX) / sizeof (unsigned long))
};

/* Initialize the color table.  */

static void
init_color_table (void)
{
  int size = CT_SIZE * sizeof (*ct_table);
  ct_table = xzalloc (size);
  ct_colors_allocated = 0;
}


/* Free memory associated with the color table.  */

static void
free_color_table (void)
{
  int i;
  struct ct_color *p, *next;

  for (i = 0; i < CT_SIZE; ++i)
    for (p = ct_table[i]; p; p = next)
      {
	next = p->next;
	xfree (p);
      }

  xfree (ct_table);
  ct_table = NULL;
}


/* Value is a pixel color for RGB color R, G, B on frame F.  If an
   entry for that color already is in the color table, return the
   pixel color of that entry.  Otherwise, allocate a new color for R,
   G, B, and make an entry in the color table.  */

static unsigned long
lookup_rgb_color (struct frame *f, int r, int g, int b)
{
  unsigned hash = CT_HASH_RGB (r, g, b);
  int i = hash % CT_SIZE;
  struct ct_color *p;
  Display_Info *dpyinfo;

  /* Handle TrueColor visuals specially, which improves performance by
     two orders of magnitude.  Freeing colors on TrueColor visuals is
     a nop, and pixel colors specify RGB values directly.  See also
     the Xlib spec, chapter 3.1.  */
  dpyinfo = FRAME_DISPLAY_INFO (f);
  if (dpyinfo->red_bits > 0)
    {
      unsigned long pr, pg, pb;

      /* Apply gamma-correction like normal color allocation does.  */
      if (f->gamma)
	{
	  XColor color;
	  color.red = r, color.green = g, color.blue = b;
	  gamma_correct (f, &color);
	  r = color.red, g = color.green, b = color.blue;
	}

      /* Scale down RGB values to the visual's bits per RGB, and shift
	 them to the right position in the pixel color.  Note that the
	 original RGB values are 16-bit values, as usual in X.  */
      pr = (r >> (16 - dpyinfo->red_bits))   << dpyinfo->red_offset;
      pg = (g >> (16 - dpyinfo->green_bits)) << dpyinfo->green_offset;
      pb = (b >> (16 - dpyinfo->blue_bits))  << dpyinfo->blue_offset;

      /* Assemble the pixel color.  */
      return pr | pg | pb;
    }

  for (p = ct_table[i]; p; p = p->next)
    if (p->r == r && p->g == g && p->b == b)
      break;

  if (p == NULL)
    {

#ifdef HAVE_X_WINDOWS
      XColor color;
      Colormap cmap;
      bool rc;
#else
      COLORREF color;
#endif

      if (ct_colors_allocated_max <= ct_colors_allocated)
	return FRAME_FOREGROUND_PIXEL (f);

#ifdef HAVE_X_WINDOWS
      color.red = r;
      color.green = g;
      color.blue = b;

      cmap = FRAME_X_COLORMAP (f);
      rc = x_alloc_nearest_color (f, cmap, &color);
      if (rc)
	{
	  ++ct_colors_allocated;
	  p = xmalloc (sizeof *p);
	  p->r = r;
	  p->g = g;
	  p->b = b;
	  p->pixel = color.pixel;
	  p->next = ct_table[i];
	  ct_table[i] = p;
	}
      else
	return FRAME_FOREGROUND_PIXEL (f);

#else
#ifdef HAVE_NTGUI
      color = PALETTERGB (r, g, b);
#else
      color = RGB_TO_ULONG (r, g, b);
#endif /* HAVE_NTGUI */
      ++ct_colors_allocated;
      p = xmalloc (sizeof *p);
      p->r = r;
      p->g = g;
      p->b = b;
      p->pixel = color;
      p->next = ct_table[i];
      ct_table[i] = p;
#endif /* HAVE_X_WINDOWS */

    }

  return p->pixel;
}


/* Look up pixel color PIXEL which is used on frame F in the color
   table.  If not already present, allocate it.  Value is PIXEL.  */

static unsigned long
lookup_pixel_color (struct frame *f, unsigned long pixel)
{
  int i = pixel % CT_SIZE;
  struct ct_color *p;

  for (p = ct_table[i]; p; p = p->next)
    if (p->pixel == pixel)
      break;

  if (p == NULL)
    {
      XColor color;
      Colormap cmap;
      bool rc;

      if (ct_colors_allocated_max <= ct_colors_allocated)
	return FRAME_FOREGROUND_PIXEL (f);

#ifdef HAVE_X_WINDOWS
      cmap = FRAME_X_COLORMAP (f);
      color.pixel = pixel;
      x_query_color (f, &color);
      rc = x_alloc_nearest_color (f, cmap, &color);
#else
      block_input ();
      cmap = DefaultColormapOfScreen (FRAME_X_SCREEN (f));
      color.pixel = pixel;
      XQueryColor (NULL, cmap, &color);
      rc = x_alloc_nearest_color (f, cmap, &color);
      unblock_input ();
#endif /* HAVE_X_WINDOWS */

      if (rc)
	{
	  ++ct_colors_allocated;

	  p = xmalloc (sizeof *p);
	  p->r = color.red;
	  p->g = color.green;
	  p->b = color.blue;
	  p->pixel = pixel;
	  p->next = ct_table[i];
	  ct_table[i] = p;
	}
      else
	return FRAME_FOREGROUND_PIXEL (f);
    }
  return p->pixel;
}


/* Value is a vector of all pixel colors contained in the color table,
   allocated via xmalloc.  Set *N to the number of colors.  */

static unsigned long *
colors_in_color_table (int *n)
{
  int i, j;
  struct ct_color *p;
  unsigned long *colors;

  if (ct_colors_allocated == 0)
    {
      *n = 0;
      colors = NULL;
    }
  else
    {
      colors = xmalloc (ct_colors_allocated * sizeof *colors);
      *n = ct_colors_allocated;

      for (i = j = 0; i < CT_SIZE; ++i)
	for (p = ct_table[i]; p; p = p->next)
	  colors[j++] = p->pixel;
    }

  return colors;
}

#else /* COLOR_TABLE_SUPPORT */

static unsigned long
lookup_rgb_color (struct frame *f, int r, int g, int b)
{
  unsigned long pixel;

#if defined(MAC_OS) || (defined(HAVE_CARBON) && !defined(HAVE_NS))
  pixel = RGB_TO_ULONG (r >> 8, g >> 8, b >> 8);
  gamma_correct (f, &pixel);
#endif /* MAC_OS || (HAVE_CARBON && !HAVE_NS) */

#ifdef HAVE_NTGUI
  pixel = PALETTERGB (r >> 8, g >> 8, b >> 8);
#endif /* HAVE_NTGUI */

#ifdef HAVE_NS
  pixel = RGB_TO_ULONG (r >> 8, g >> 8, b >> 8);
#endif /* HAVE_NS */
  return pixel;
}

static void
init_color_table (void)
{
  return;
}
#endif /* COLOR_TABLE_SUPPORT */


/***********************************************************************
			      Algorithms
 ***********************************************************************/

/* Edge detection matrices for different edge-detection
   strategies.  */

static int emboss_matrix[9] = {
   /* x - 1	x	x + 1  */
        2,     -1,  	  0,		/* y - 1 */
       -1,      0,        1,		/* y     */
        0,      1,       -2		/* y + 1 */
};

static int laplace_matrix[9] = {
   /* x - 1	x	x + 1  */
        1,      0,  	  0,		/* y - 1 */
        0,      0,        0,		/* y     */
        0,      0,       -1		/* y + 1 */
};

/* Value is the intensity of the color whose red/green/blue values
   are R, G, and B.  */

#define COLOR_INTENSITY(R, G, B) ((2 * (R) + 3 * (G) + (B)) / 6)


/* On frame F, return an array of XColor structures describing image
   IMG->pixmap.  Each XColor structure has its pixel color set.  RGB_P
   means also fill the red/green/blue members of the XColor
   structures.  Value is a pointer to the array of XColors structures,
   allocated with xmalloc; it must be freed by the caller.  */

static XColor *
x_to_xcolors(struct frame *f, struct image *img, bool rgb_p)
{
  int x, y;
  XColor *colors, *p;
  XImagePtr_or_DC ximg;
#ifdef HAVE_NTGUI
  HGDIOBJ prev;
#endif /* HAVE_NTGUI */

  if ((min(PTRDIFF_MAX, SIZE_MAX) / sizeof(*colors) / img->width)
      < img->height)
    memory_full(SIZE_MAX);
  colors = xmalloc(sizeof(*colors) * img->width * img->height);

  /* Get the X image or create a memory device context for IMG. */
  ximg = image_get_x_image_or_dc (f, img, 0, &prev);

  /* Fill the `pixel' members of the XColor array.  I wished there
     were an easy and portable way to circumvent XGetPixel.  */
  p = colors;
  for (y = 0; y < img->height; ++y)
    {
#if defined(HAVE_X_WINDOWS) || defined(HAVE_NTGUI)
      XColor *row = p;
      for (x = 0; x < img->width; ++x, ++p)
	p->pixel = GET_PIXEL(ximg, x, y);
      if (rgb_p)
	x_query_colors(f, row, img->width);
#else
      for (x = 0; x < img->width; ++x, ++p)
	{
	  /* W32_TODO: palette support needed here?  */
	  p->pixel = GET_PIXEL(ximg, x, y);
	  if (rgb_p)
	    {
# if defined(MAC_OS) || 1
	      p->red = RED16_FROM_ULONG(p->pixel);
	      p->green = GREEN16_FROM_ULONG(p->pixel);
	      p->blue = BLUE16_FROM_ULONG(p->pixel);
# else
#  ifdef HAVE_NTGUI
	      p->red = (256 * GetRValue(p->pixel));
	      p->green = (256 * GetGValue(p->pixel));
	      p->blue = (256 * GetBValue(p->pixel));
#  endif /* HAVE_NTGUI */
# endif  /* MAC_OS || 1 */
	    }
	}
#endif /* HAVE_X_WINDOWS */
    }

  image_unget_x_image_or_dc(img, 0, ximg, prev);

  return colors;
}

#ifdef HAVE_NTGUI

/* Put a pixel of COLOR at position X, Y in XIMG.  XIMG must have been
   created with CreateDIBSection, with the pointer to the bit values
   stored in ximg->data.  */

static void
XPutPixel (XImagePtr ximg, int x, int y, COLORREF color)
{
  int width = ximg->info.bmiHeader.biWidth;
  unsigned char * pixel;

  /* True color images.  */
  if (ximg->info.bmiHeader.biBitCount == 24)
    {
      int rowbytes = width * 3;
      /* Ensure scanlines are aligned on 4 byte boundaries.  */
      if (rowbytes % 4)
	rowbytes += 4 - (rowbytes % 4);

      pixel = ximg->data + y * rowbytes + x * 3;
      /* Windows bitmaps are in BGR order.  */
      *pixel = GetBValue (color);
      *(pixel + 1) = GetGValue (color);
      *(pixel + 2) = GetRValue (color);
    }
  /* Monochrome images.  */
  else if (ximg->info.bmiHeader.biBitCount == 1)
    {
      int rowbytes = width / 8;
      /* Ensure scanlines are aligned on 4 byte boundaries.  */
      if (rowbytes % 4)
	rowbytes += 4 - (rowbytes % 4);
      pixel = ximg->data + y * rowbytes + x / 8;
      /* Filter out palette info.  */
      if (color & 0x00ffffff)
	*pixel = *pixel | (1 << x % 8);
      else
	*pixel = *pixel & ~(1 << x % 8);
    }
  else
    image_error ("XPutPixel: palette image not supported", Qnil, Qnil);
}

#endif /* HAVE_NTGUI */

/* Create IMG->pixmap from an array COLORS of XColor structures, whose
   RGB members are set.  F is the frame on which this all happens.
   COLORS will be freed; an existing IMG->pixmap will be freed, too.  */

static void
x_from_xcolors (struct frame *f, struct image *img, XColor *colors)
{
  int x, y;
  XImagePtr oimg = NULL;
  XColor *p;

  init_color_table ();

  x_clear_image_1 (f, img, CLEAR_IMAGE_PIXMAP | CLEAR_IMAGE_COLORS);
  image_create_x_image_and_pixmap (f, img, img->width, img->height, 0,
				   &oimg, 0);
  p = colors;
  for (y = 0; y < img->height; ++y)
    for (x = 0; x < img->width; ++x, ++p)
      {
	unsigned long pixel;
	pixel = lookup_rgb_color (f, p->red, p->green, p->blue);
	XPutPixel (oimg, x, y, pixel);
      }

  xfree (colors);

  image_put_x_image (f, img, oimg, 0);
#ifdef COLOR_TABLE_SUPPORT
  img->colors = colors_in_color_table (&img->ncolors);
  free_color_table ();
#endif /* COLOR_TABLE_SUPPORT */
}


/* On frame F, perform edge-detection on image IMG.

   MATRIX is a nine-element array specifying the transformation
   matrix.  See emboss_matrix for an example.

   COLOR_ADJUST is a color adjustment added to each pixel of the
   outgoing image.  */

static void
x_detect_edges (struct frame *f, struct image *img, int *matrix, int color_adjust)
{
  XColor *colors = x_to_xcolors (f, img, 1);
  XColor *new, *p;
  int x, y, i, sum;

  for (i = sum = 0; i < 9; ++i)
    sum += eabs (matrix[i]);

#define COLOR(A, X, Y) ((A) + (Y) * img->width + (X))

  if (min (PTRDIFF_MAX, SIZE_MAX) / sizeof *new / img->width < img->height)
    memory_full (SIZE_MAX);
  new = xmalloc (sizeof *new * img->width * img->height);

  for (y = 0; y < img->height; ++y)
    {
      p = COLOR (new, 0, y);
      p->red = p->green = p->blue = 0xffff/2;
      p = COLOR (new, img->width - 1, y);
      p->red = p->green = p->blue = 0xffff/2;
    }

  for (x = 1; x < img->width - 1; ++x)
    {
      p = COLOR (new, x, 0);
      p->red = p->green = p->blue = 0xffff/2;
      p = COLOR (new, x, img->height - 1);
      p->red = p->green = p->blue = 0xffff/2;
    }

  for (y = 1; y < img->height - 1; ++y)
    {
      p = COLOR (new, 1, y);

      for (x = 1; x < img->width - 1; ++x, ++p)
	{
	  int r, g, b, yy, xx;

	  r = g = b = i = 0;
	  for (yy = y - 1; yy < y + 2; ++yy)
	    for (xx = x - 1; xx < x + 2; ++xx, ++i)
	      if (matrix[i])
	        {
	          XColor *t = COLOR (colors, xx, yy);
		  r += matrix[i] * t->red;
		  g += matrix[i] * t->green;
		  b += matrix[i] * t->blue;
		}

	  r = (r / sum + color_adjust) & 0xffff;
	  g = (g / sum + color_adjust) & 0xffff;
	  b = (b / sum + color_adjust) & 0xffff;
	  p->red = p->green = p->blue = COLOR_INTENSITY (r, g, b);
	}
    }

  xfree (colors);
  x_from_xcolors (f, img, new);

#undef COLOR
}


/* Perform the pre-defined `emboss' edge-detection on image IMG
   on frame F.  */

static void
x_emboss (struct frame *f, struct image *img)
{
  x_detect_edges (f, img, emboss_matrix, 0xffff / 2);
}


/* Transform image IMG which is used on frame F with a Laplace
   edge-detection algorithm.  The result is an image that can be used
   to draw disabled buttons, for example.  */

static void
x_laplace (struct frame *f, struct image *img)
{
  x_detect_edges (f, img, laplace_matrix, 45000);
}


/* Perform edge-detection on image IMG on frame F, with specified
   transformation matrix MATRIX and color-adjustment COLOR_ADJUST.

   MATRIX must be either

   - a list of at least 9 numbers in row-major form
   - a vector of at least 9 numbers

   COLOR_ADJUST nil means use a default; otherwise it must be a
   number.  */

static void
x_edge_detection (struct frame *f, struct image *img, Lisp_Object matrix,
		  Lisp_Object color_adjust)
{
  int i = 0;
  int trans[9];

  if (CONSP (matrix))
    {
      for (i = 0;
	   i < 9 && CONSP (matrix) && NUMBERP (XCAR (matrix));
	   ++i, matrix = XCDR (matrix))
	trans[i] = XFLOATINT (XCAR (matrix));
    }
  else if (VECTORP (matrix) && ASIZE (matrix) >= 9)
    {
      for (i = 0; i < 9 && NUMBERP (AREF (matrix, i)); ++i)
	trans[i] = XFLOATINT (AREF (matrix, i));
    }

  if (NILP (color_adjust))
    color_adjust = make_number (0xffff / 2);

  if (i == 9 && NUMBERP (color_adjust))
    x_detect_edges (f, img, trans, XFLOATINT (color_adjust));
}


/* Transform image IMG on frame F so that it looks disabled.  */

static void
x_disable_image (struct frame *f, struct image *img)
{
  Display_Info *dpyinfo = FRAME_DISPLAY_INFO (f);
#ifdef HAVE_NTGUI
  int n_planes = dpyinfo->n_planes * dpyinfo->n_cbits;
#else
  int n_planes = dpyinfo->n_planes;
#endif /* HAVE_NTGUI */

  if (n_planes >= 2)
    {
      /* Color (or grayscale).  Convert to gray, and equalize.  Just
	 drawing such images with a stipple can look very odd, so
	 we're using this method instead.  */
      XColor *colors = x_to_xcolors (f, img, 1);
      XColor *p, *end;
      const int h = 15000;
      const int l = 30000;

      for (p = colors, end = colors + img->width * img->height;
	   p < end;
	   ++p)
	{
	  int i = COLOR_INTENSITY (p->red, p->green, p->blue);
	  int i2 = (0xffff - h - l) * i / 0xffff + l;
	  p->red = p->green = p->blue = i2;
	}

      x_from_xcolors (f, img, colors);
    }

  /* Draw a cross over the disabled image, if we must or if we
     should.  */
  if (n_planes < 2 || cross_disabled_images)
    {
#ifndef HAVE_NTGUI
#ifndef HAVE_NS  /* TODO: NS support, however this not needed for toolbars */

#ifdef MAC_OS
# define MaskForeground(f)  PIX_MASK_DRAW
#else
# define MaskForeground(f)  WHITE_PIX_DEFAULT (f)
#endif /* MAC_OS */

      Display *dpy = FRAME_X_DISPLAY (f);
      GC gc;

#ifdef HAVE_X_WINDOWS
      image_sync_to_pixmaps (f, img);
#endif /* HAVE_X_WINDOWS */
      gc = XCreateGC (dpy, img->pixmap, 0, NULL);
      XSetForeground (dpy, gc, BLACK_PIX_DEFAULT (f));
      XDrawLine (dpy, img->pixmap, gc, 0, 0,
		 img->width - 1, img->height - 1);
      XDrawLine (dpy, img->pixmap, gc, 0, img->height - 1,
		 img->width - 1, 0);
      XFreeGC (dpy, gc);

      if (img->mask)
	{
	  gc = XCreateGC (dpy, img->mask, 0, NULL);
	  XSetForeground (dpy, gc, MaskForeground (f));
	  XDrawLine (dpy, img->mask, gc, 0, 0,
		     img->width - 1, img->height - 1);
	  XDrawLine (dpy, img->mask, gc, 0, img->height - 1,
		     img->width - 1, 0);
	  XFreeGC (dpy, gc);
	}
#endif /* !HAVE_NS */
#else
      HDC hdc, bmpdc;
      HGDIOBJ prev;

      hdc = get_frame_dc (f);
      bmpdc = CreateCompatibleDC (hdc);
      release_frame_dc (f, hdc);

      prev = SelectObject (bmpdc, img->pixmap);

      SetTextColor (bmpdc, BLACK_PIX_DEFAULT (f));
      MoveToEx (bmpdc, 0, 0, NULL);
      LineTo (bmpdc, img->width - 1, img->height - 1);
      MoveToEx (bmpdc, 0, img->height - 1, NULL);
      LineTo (bmpdc, img->width - 1, 0);

      if (img->mask)
	{
	  SelectObject (bmpdc, img->mask);
	  SetTextColor (bmpdc, WHITE_PIX_DEFAULT (f));
	  MoveToEx (bmpdc, 0, 0, NULL);
	  LineTo (bmpdc, img->width - 1, img->height - 1);
	  MoveToEx (bmpdc, 0, img->height - 1, NULL);
	  LineTo (bmpdc, img->width - 1, 0);
	}
      SelectObject (bmpdc, prev);
      DeleteDC (bmpdc);
#endif /* HAVE_NTGUI */
    }
}


/* Build a mask for image IMG which is used on frame F.  FILE is the
   name of an image file, for error messages.  HOW determines how to
   determine the background color of IMG.  If it is a list '(R G B)',
   with R, G, and B being integers >= 0, take that as the color of the
   background.  Otherwise, determine the background color of IMG
   heuristically.  */

static void
x_build_heuristic_mask (struct frame *f, struct image *img, Lisp_Object how)
{
  XImagePtr_or_DC ximg;
#ifndef HAVE_NTGUI
# ifndef HAVE_NS
  XImagePtr mask_img = NULL; /* (same condition in which it is used) */
# endif /* !HAVE_NS */
#else
  HGDIOBJ prev;
  char *mask_img;
  int row_width;
#endif /* HAVE_NTGUI */
  int x, y;
  bool use_img_background;
  unsigned long bg = 0UL;

  if (img->mask)
    x_clear_image_1 (f, img, CLEAR_IMAGE_MASK);

#ifndef HAVE_NTGUI
# ifndef HAVE_NS
  /* Create an image and pixmap serving as mask.  */
  if (! image_create_x_image_and_pixmap (f, img, img->width, img->height, 1,
					 &mask_img, 1))
    return;
# endif /* !HAVE_NS */
#else
  /* Create the bit array serving as mask.  */
  row_width = (img->width + 7) / 8;
  mask_img = xzalloc (row_width * img->height);
#endif /* HAVE_NTGUI */

  /* Get the X image or create a memory device context for IMG.  */
  ximg = image_get_x_image_or_dc (f, img, 0, &prev);

  /* Determine the background color of ximg.  If HOW is `(R G B)'
     take that as color.  Otherwise, use the image's background color. */
  use_img_background = 1;

  if (CONSP (how))
    {
      int rgb[3], i;

      for (i = 0; i < 3 && CONSP (how) && NATNUMP (XCAR (how)); ++i)
	{
	  rgb[i] = XFASTINT (XCAR (how)) & 0xffff;
	  how = XCDR (how);
	}

      if (i == 3 && NILP (how))
	{
	  char color_name[30];
	  snprintf(color_name, sizeof(color_name), "#%04x%04x%04x",
		   (unsigned int)rgb[0], (unsigned int)rgb[1],
		   (unsigned int)rgb[2]);
	  bg = (
#ifdef HAVE_NTGUI
		0x00ffffff & /* Filter out palette info.  */
#endif /* HAVE_NTGUI */
		x_alloc_image_color (f, img, build_string (color_name), 0));
	  use_img_background = 0;
	}
    }

  if (use_img_background)
    bg = four_corners_best (ximg, img->corners, img->width, img->height);

  /* Set all bits in mask_img to 1 whose color in ximg is different
     from the background color bg.  */
#ifndef HAVE_NTGUI
  for (y = 0; y < img->height; ++y)
    for (x = 0; x < img->width; ++x)
# ifndef HAVE_NS
      XPutPixel(mask_img, x, y, ((XGetPixel(ximg, x, y) != bg)
                                 ? PIX_MASK_DRAW : PIX_MASK_RETAIN));
# else
      if (XGetPixel(ximg, x, y) == bg)
        ns_set_alpha(ximg, x, y, 0);
# endif /* HAVE_NS */
# ifndef HAVE_NS
  /* Fill in the background_transparent field while we have the mask handy. */
  image_background_transparent(img, f, mask_img);

  /* Put mask_img into the image: */
  image_put_x_image(f, img, mask_img, 1);
# endif /* !HAVE_NS */
#else /* do HAVE_NTGUI in this next part: */
  for (y = 0; y < img->height; ++y)
    for (x = 0; x < img->width; ++x)
      {
	COLORREF p = GetPixel(ximg, x, y);
	if (p != bg)
	  mask_img[(y * row_width) + (x / 8)] |= (1 << (x % 8));
      }

  /* Create the mask image.  */
  img->mask = w32_create_pixmap_from_bitmap_data(img->width, img->height,
						 mask_img);
  /* Fill in the background_transparent field while we have the mask handy. */
  SelectObject(ximg, img->mask);
  image_background_transparent(img, f, ximg);

  /* Was: x_destroy_x_image ((XImagePtr )mask_img); which seems bogus ++kfs */
  xfree(mask_img);
#endif /* HAVE_NTGUI */

  image_unget_x_image_or_dc(img, 0, ximg, prev);
}


/***********************************************************************
		       PBM (mono, gray, color)
 ***********************************************************************/

static bool pbm_image_p (Lisp_Object object);
static bool pbm_load (struct frame *f, struct image *img);

/* The symbol `pbm' identifying images of this type.  */

static Lisp_Object Qpbm;

/* Indices of image specification fields in gs_format, below.  */

enum pbm_keyword_index
{
  PBM_TYPE,
  PBM_FILE,
  PBM_DATA,
  PBM_ASCENT,
  PBM_MARGIN,
  PBM_RELIEF,
  PBM_ALGORITHM,
  PBM_HEURISTIC_MASK,
  PBM_MASK,
  PBM_FOREGROUND,
  PBM_BACKGROUND,
  PBM_LAST
};

/* Vector of image_keyword structures describing the format
   of valid user-defined image specifications.  */

static const struct image_keyword pbm_format[PBM_LAST] =
{
  {":type",		IMAGE_SYMBOL_VALUE,			1},
  {":file",		IMAGE_STRING_VALUE,			0},
  {":data",		IMAGE_STRING_VALUE,			0},
  {":ascent",		IMAGE_ASCENT_VALUE,			0},
  {":margin",		IMAGE_NON_NEGATIVE_INTEGER_VALUE_OR_PAIR, 0},
  {":relief",		IMAGE_INTEGER_VALUE,			0},
  {":conversion",	IMAGE_DONT_CHECK_VALUE_TYPE,		0},
  {":heuristic-mask",	IMAGE_DONT_CHECK_VALUE_TYPE,		0},
  {":mask",		IMAGE_DONT_CHECK_VALUE_TYPE,		0},
  {":foreground",	IMAGE_STRING_OR_NIL_VALUE,		0},
  {":background",	IMAGE_STRING_OR_NIL_VALUE,		0}
};

/* Structure describing the image type `pbm'.  */

static struct image_type pbm_type =
{
  &Qpbm,
  pbm_image_p,
  pbm_load,
  x_clear_image,
  NULL,
  NULL
};


/* Return true if OBJECT is a valid PBM image specification.  */

static bool
pbm_image_p (Lisp_Object object)
{
  struct image_keyword fmt[PBM_LAST];

  memcpy (fmt, pbm_format, sizeof fmt);

  if (!parse_image_spec (object, fmt, PBM_LAST, Qpbm))
    return 0;

  /* Must specify either :data or :file.  */
  return fmt[PBM_DATA].count + fmt[PBM_FILE].count == 1;
}


/* Get next char skipping comments in Netpbm header.  Returns -1 at
   end of input.  */

static int
pbm_next_char (unsigned char **s, unsigned char *end)
{
  int c = -1;

  while (*s < end && (c = *(*s)++, c == '#'))
    {
      /* Skip to the next line break.  */
      while (*s < end && (c = *(*s)++, c != '\n' && c != '\r'))
        ;

      c = -1;
    }

  return c;
}


/* Scan a decimal number from *S and return it.  Advance *S while
   reading the number.  END is the end of the string.  Value is -1 at
   end of input.  */

static int
pbm_scan_number (unsigned char **s, unsigned char *end)
{
  int c = 0, val = -1;

  /* Skip white-space.  */
  while ((c = pbm_next_char (s, end)) != -1 && c_isspace (c))
    ;

  if (c_isdigit (c))
    {
      /* Read decimal number.  */
      val = c - '0';
      while ((c = pbm_next_char (s, end)) != -1 && c_isdigit (c))
        val = 10 * val + c - '0';
    }

  return val;
}


/* Load PBM image IMG for use on frame F.  */

static bool
pbm_load (struct frame *f, struct image *img)
{
  bool raw_p;
  int x, y;
  int width, height, max_color_idx = 0;
  XImagePtr ximg;
  Lisp_Object file, specified_file;
  enum {PBM_MONO, PBM_GRAY, PBM_COLOR} type;
  unsigned char *contents = NULL;
  unsigned char *end, *p;
  ptrdiff_t size;

  specified_file = image_spec_value (img->spec, QCfile, NULL);

  if (STRINGP (specified_file))
    {
      file = x_find_image_file (specified_file);
      if (!STRINGP (file))
	{
	  image_error ("Cannot find image file `%s'", specified_file, Qnil);
	  return 0;
	}

      contents = slurp_file (SSDATA (file), &size);
      if (contents == NULL)
	{
	  image_error ("Error reading `%s'", file, Qnil);
	  return 0;
	}

      p = contents;
      end = contents + size;
    }
  else
    {
      Lisp_Object data;
      data = image_spec_value (img->spec, QCdata, NULL);
      if (!STRINGP (data))
	{
	  image_error ("Invalid image data `%s'", data, Qnil);
	  return 0;
	}
      p = SDATA (data);
      end = p + SBYTES (data);
    }

  /* Check magic number: */
  if (end - p < 2 || *p++ != 'P')
    {
      image_error("Not a PBM image: `%s'", img->spec, Qnil);
    error:
      xfree(contents);
      img->pixmap = NO_PIXMAP;
      return 0;
    }

  switch (*p++)
    {
    case '1': raw_p = 0, type = PBM_MONO; break;
    case '2': raw_p = 0, type = PBM_GRAY; break;
    case '3': raw_p = 0, type = PBM_COLOR; break;
    case '4': raw_p = 1, type = PBM_MONO; break;
    case '5': raw_p = 1, type = PBM_GRAY; break;
    case '6': raw_p = 1, type = PBM_COLOR; break;
    default:
      image_error("Not a PBM image: `%s'", img->spec, Qnil);
      goto error;
    }

  /* Read width, height, maximum color-component.  Characters
     starting with `#' up to the end of a line are ignored.  */
  width = pbm_scan_number(&p, end);
  height = pbm_scan_number(&p, end);

  if (type != PBM_MONO)
    {
      max_color_idx = pbm_scan_number(&p, end);
      if ((max_color_idx > 65535) || (max_color_idx < 0))
	{
	  image_error("Unsupported maximum PBM color value", Qnil, Qnil);
	  goto error;
	}
    }

  if (!check_image_size (f, width, height))
    {
      image_error ("Invalid image size (see `max-image-size')", Qnil, Qnil);
      goto error;
    }

  if (!image_create_x_image_and_pixmap (f, img, width, height, 0, &ximg, 0))
    goto error;

  /* Initialize the color hash table.  */
  init_color_table ();

  if (type == PBM_MONO)
    {
      int c = 0, g;
      struct image_keyword fmt[PBM_LAST];
      unsigned long fg = FRAME_FOREGROUND_PIXEL (f);
      unsigned long bg = FRAME_BACKGROUND_PIXEL (f);

      /* Parse the image specification.  */
      memcpy (fmt, pbm_format, sizeof fmt);
      parse_image_spec (img->spec, fmt, PBM_LAST, Qpbm);

      /* Get foreground and background colors, maybe allocate colors.  */
      if (fmt[PBM_FOREGROUND].count
	  && STRINGP (fmt[PBM_FOREGROUND].value))
	fg = x_alloc_image_color (f, img, fmt[PBM_FOREGROUND].value, fg);
      if (fmt[PBM_BACKGROUND].count
	  && STRINGP (fmt[PBM_BACKGROUND].value))
	{
	  bg = x_alloc_image_color (f, img, fmt[PBM_BACKGROUND].value, bg);
	  img->background = bg;
	  img->background_valid = 1;
	}

      for (y = 0; y < height; ++y)
	for (x = 0; x < width; ++x)
	  {
	    if (raw_p)
	      {
		if ((x & 7) == 0)
		  {
		    if (p >= end)
		      {
			x_destroy_x_image (ximg);
			x_clear_image (f, img);
			image_error ("Invalid image size in image `%s'",
				     img->spec, Qnil);
			goto error;
		      }
		    c = *p++;
		  }
		g = c & 0x80;
		c <<= 1;
	      }
	    else
	      g = pbm_scan_number (&p, end);

	    XPutPixel (ximg, x, y, g ? fg : bg);
	  }
    }
  else
    {
      int expected_size = height * width;
      if (max_color_idx > 255)
	expected_size *= 2;
      if (type == PBM_COLOR)
	expected_size *= 3;

      if (raw_p && p + expected_size > end)
	{
	  x_destroy_x_image (ximg);
	  x_clear_image (f, img);
	  image_error ("Invalid image size in image `%s'",
		       img->spec, Qnil);
	  goto error;
	}

      for (y = 0; y < height; ++y)
	for (x = 0; x < width; ++x)
	  {
	    int r, g, b;

	    if (type == PBM_GRAY && raw_p)
	      {
		r = g = b = *p++;
		if (max_color_idx > 255)
		  r = g = b = r * 256 + *p++;
	      }
	    else if (type == PBM_GRAY)
	      r = g = b = pbm_scan_number (&p, end);
	    else if (raw_p)
	      {
		r = *p++;
		if (max_color_idx > 255)
		  r = r * 256 + *p++;
		g = *p++;
		if (max_color_idx > 255)
		  g = g * 256 + *p++;
		b = *p++;
		if (max_color_idx > 255)
		  b = b * 256 + *p++;
	      }
	    else
	      {
		r = pbm_scan_number (&p, end);
		g = pbm_scan_number (&p, end);
		b = pbm_scan_number (&p, end);
	      }

	    if (r < 0 || g < 0 || b < 0)
	      {
		x_destroy_x_image (ximg);
		image_error ("Invalid pixel value in image `%s'",
			     img->spec, Qnil);
		goto error;
	      }

	    /* RGB values are now in the range 0..max_color_idx.
	       Scale this to the range 0..0xffff supported by X.  */
	    r = (double) r * 65535 / max_color_idx;
	    g = (double) g * 65535 / max_color_idx;
	    b = (double) b * 65535 / max_color_idx;
	    XPutPixel (ximg, x, y, lookup_rgb_color (f, r, g, b));
	  }
    }

#ifdef COLOR_TABLE_SUPPORT
  /* Store in IMG->colors the colors allocated for the image, and
     free the color table.  */
  img->colors = colors_in_color_table (&img->ncolors);
  free_color_table ();
#endif /* COLOR_TABLE_SUPPORT */

  img->width = width;
  img->height = height;

  /* Maybe fill in the background field while we have ximg handy.  */

  if (NILP (image_spec_value (img->spec, QCbackground, NULL)))
    /* Casting avoids a GCC warning.  */
    IMAGE_BACKGROUND (img, f, (XImagePtr_or_DC)ximg);

  /* Put ximg into the image.  */
  image_put_x_image (f, img, ximg, 0);

  /* X and W32 versions did it here, MAC version above.  ++kfs
     img->width = width;
     img->height = height; */

  xfree (contents);
  return 1;
}


/***********************************************************************
				 PNG
 ***********************************************************************/

#if defined (HAVE_PNG) || defined (MAC_OS) || defined (HAVE_NS)

/* Function prototypes.  */

static bool png_image_p (Lisp_Object object);
static bool png_load (struct frame *f, struct image *img);

/* The symbol `png' identifying images of this type.  */

static Lisp_Object Qpng;

/* Indices of image specification fields in png_format, below.  */

enum png_keyword_index
{
  PNG_TYPE,
  PNG_DATA,
  PNG_FILE,
  PNG_ASCENT,
  PNG_MARGIN,
  PNG_RELIEF,
  PNG_ALGORITHM,
  PNG_HEURISTIC_MASK,
  PNG_MASK,
  PNG_BACKGROUND,
  PNG_LAST
};

/* Vector of image_keyword structures describing the format
   of valid user-defined image specifications.  */

static const struct image_keyword png_format[PNG_LAST] =
{
  {":type",		IMAGE_SYMBOL_VALUE,			1},
  {":data",		IMAGE_STRING_VALUE,			0},
  {":file",		IMAGE_STRING_VALUE,			0},
  {":ascent",		IMAGE_ASCENT_VALUE,			0},
  {":margin",		IMAGE_NON_NEGATIVE_INTEGER_VALUE_OR_PAIR, 0},
  {":relief",		IMAGE_INTEGER_VALUE,			0},
  {":conversion",	IMAGE_DONT_CHECK_VALUE_TYPE,		0},
  {":heuristic-mask",	IMAGE_DONT_CHECK_VALUE_TYPE,		0},
  {":mask",		IMAGE_DONT_CHECK_VALUE_TYPE,		0},
  {":background",	IMAGE_STRING_OR_NIL_VALUE,		0}
};

# if defined HAVE_NTGUI && defined WINDOWSNT
static bool init_png_functions (void);
# else
#  define init_png_functions NULL
# endif /* HAVE_NTGUI && WINDOWSNT */

/* Structure describing the image type `png'.  */

static struct image_type png_type =
{
  &Qpng,
  png_image_p,
  png_load,
  x_clear_image,
  init_png_functions,
  NULL
};

/* Return true if OBJECT is a valid PNG image specification.  */

static bool
png_image_p (Lisp_Object object)
{
  struct image_keyword fmt[PNG_LAST];
  memcpy (fmt, png_format, sizeof fmt);

  if (!parse_image_spec (object, fmt, PNG_LAST, Qpng))
    return 0;

  /* Must specify either the :data or :file keyword.  */
  return fmt[PNG_FILE].count + fmt[PNG_DATA].count == 1;
}

#endif /* HAVE_PNG || HAVE_NS */


#ifdef HAVE_PNG

#ifdef WINDOWSNT
/* PNG library details.  */

DEF_IMGLIB_FN (png_voidp, png_get_io_ptr, (png_structp));
DEF_IMGLIB_FN (int, png_sig_cmp, (png_bytep, png_size_t, png_size_t));
DEF_IMGLIB_FN (png_structp, png_create_read_struct, (png_const_charp, png_voidp,
						     png_error_ptr, png_error_ptr));
DEF_IMGLIB_FN (png_infop, png_create_info_struct, (png_structp));
DEF_IMGLIB_FN (void, png_destroy_read_struct, (png_structpp, png_infopp, png_infopp));
DEF_IMGLIB_FN (void, png_set_read_fn, (png_structp, png_voidp, png_rw_ptr));
DEF_IMGLIB_FN (void, png_set_sig_bytes, (png_structp, int));
DEF_IMGLIB_FN (void, png_read_info, (png_structp, png_infop));
DEF_IMGLIB_FN (png_uint_32, png_get_IHDR, (png_structp, png_infop,
			      png_uint_32 *, png_uint_32 *,
			      int *, int *, int *, int *, int *));
DEF_IMGLIB_FN (png_uint_32, png_get_valid, (png_structp, png_infop, png_uint_32));
DEF_IMGLIB_FN (void, png_set_strip_16, (png_structp));
DEF_IMGLIB_FN (void, png_set_expand, (png_structp));
DEF_IMGLIB_FN (void, png_set_gray_to_rgb, (png_structp));
DEF_IMGLIB_FN (void, png_set_background, (png_structp, png_color_16p,
				    int, int, double));
DEF_IMGLIB_FN (png_uint_32, png_get_bKGD, (png_structp, png_infop, png_color_16p *));
DEF_IMGLIB_FN (void, png_read_update_info, (png_structp, png_infop));
DEF_IMGLIB_FN (png_byte, png_get_channels, (png_structp, png_infop));
DEF_IMGLIB_FN (png_size_t, png_get_rowbytes, (png_structp, png_infop));
DEF_IMGLIB_FN (void, png_read_image, (png_structp, png_bytepp));
DEF_IMGLIB_FN (void, png_read_end, (png_structp, png_infop));
DEF_IMGLIB_FN (void, png_error, (png_structp, png_const_charp));

#if (PNG_LIBPNG_VER >= 10500)
DEF_IMGLIB_FN (void, png_longjmp, (png_structp, int)) PNG_NORETURN;
DEF_IMGLIB_FN (jmp_buf *, png_set_longjmp_fn, (png_structp, png_longjmp_ptr, size_t));
#endif /* libpng version >= 1.5 */

static bool
init_png_functions (void)
{
  HMODULE library;

  if (!(library = w32_delayed_load (Qpng)))
    return 0;

  LOAD_IMGLIB_FN (library, png_get_io_ptr);
  LOAD_IMGLIB_FN (library, png_sig_cmp);
  LOAD_IMGLIB_FN (library, png_create_read_struct);
  LOAD_IMGLIB_FN (library, png_create_info_struct);
  LOAD_IMGLIB_FN (library, png_destroy_read_struct);
  LOAD_IMGLIB_FN (library, png_set_read_fn);
  LOAD_IMGLIB_FN (library, png_set_sig_bytes);
  LOAD_IMGLIB_FN (library, png_read_info);
  LOAD_IMGLIB_FN (library, png_get_IHDR);
  LOAD_IMGLIB_FN (library, png_get_valid);
  LOAD_IMGLIB_FN (library, png_set_strip_16);
  LOAD_IMGLIB_FN (library, png_set_expand);
  LOAD_IMGLIB_FN (library, png_set_gray_to_rgb);
  LOAD_IMGLIB_FN (library, png_set_background);
  LOAD_IMGLIB_FN (library, png_get_bKGD);
  LOAD_IMGLIB_FN (library, png_read_update_info);
  LOAD_IMGLIB_FN (library, png_get_channels);
  LOAD_IMGLIB_FN (library, png_get_rowbytes);
  LOAD_IMGLIB_FN (library, png_read_image);
  LOAD_IMGLIB_FN (library, png_read_end);
  LOAD_IMGLIB_FN (library, png_error);

#if (PNG_LIBPNG_VER >= 10500)
  LOAD_IMGLIB_FN (library, png_longjmp);
  LOAD_IMGLIB_FN (library, png_set_longjmp_fn);
#endif /* libpng version >= 1.5 */

  return 1;
}
#else

#define fn_png_get_io_ptr		png_get_io_ptr
#define fn_png_sig_cmp			png_sig_cmp
#define fn_png_create_read_struct	png_create_read_struct
#define fn_png_create_info_struct	png_create_info_struct
#define fn_png_destroy_read_struct	png_destroy_read_struct
#define fn_png_set_read_fn		png_set_read_fn
#define fn_png_set_sig_bytes		png_set_sig_bytes
#define fn_png_read_info		png_read_info
#define fn_png_get_IHDR			png_get_IHDR
#define fn_png_get_valid		png_get_valid
#define fn_png_set_strip_16		png_set_strip_16
#define fn_png_set_expand		png_set_expand
#define fn_png_set_gray_to_rgb		png_set_gray_to_rgb
#define fn_png_set_background		png_set_background
#define fn_png_get_bKGD			png_get_bKGD
#define fn_png_read_update_info		png_read_update_info
#define fn_png_get_channels		png_get_channels
#define fn_png_get_rowbytes		png_get_rowbytes
#define fn_png_read_image		png_read_image
#define fn_png_read_end			png_read_end
#define fn_png_error			png_error

#if (PNG_LIBPNG_VER >= 10500)
#define fn_png_longjmp			png_longjmp
#define fn_png_set_longjmp_fn		png_set_longjmp_fn
#endif /* libpng version >= 1.5 */

#endif /* WINDOWSNT */

/* Fast implementations of setjmp and longjmp.  Although setjmp and longjmp
   will do, POSIX _setjmp and _longjmp (if available) are often faster.
   Do not use sys_setjmp, as PNG supports only jmp_buf.
   It's OK if the longjmp substitute restores the signal mask.  */
#ifdef HAVE__SETJMP
# define FAST_SETJMP(j) _setjmp (j)
# define FAST_LONGJMP _longjmp
#else
# define FAST_SETJMP(j) setjmp (j)
# define FAST_LONGJMP longjmp
#endif

#if PNG_LIBPNG_VER < 10500
#define PNG_LONGJMP(ptr) FAST_LONGJMP ((ptr)->jmpbuf, 1)
#define PNG_JMPBUF(ptr) ((ptr)->jmpbuf)
#else
/* In libpng version 1.5, the jmpbuf member is hidden. (Bug#7908)  */
#define PNG_LONGJMP(ptr) fn_png_longjmp (ptr, 1)
#define PNG_JMPBUF(ptr) \
  (*fn_png_set_longjmp_fn (ptr, FAST_LONGJMP, sizeof (jmp_buf)))
#endif

/* Error and warning handlers installed when the PNG library
   is initialized.  */

static _Noreturn void
my_png_error (png_struct *png_ptr, const char *msg)
{
  eassert (png_ptr != NULL);
  /* Avoid compiler warning about deprecated direct access to
     png_ptr's fields in libpng versions 1.4.x.  */
  image_error ("PNG error: %s", build_string (msg), Qnil);
  PNG_LONGJMP (png_ptr);
}


static void
my_png_warning (png_struct *png_ptr, const char *msg)
{
  eassert (png_ptr != NULL);
  image_error ("PNG warning: %s", build_string (msg), Qnil);
}

/* Memory source for PNG decoding.  */

struct png_memory_storage
{
  unsigned char *bytes;		/* The data       */
  ptrdiff_t len;		/* How big is it? */
  ptrdiff_t index;		/* Where are we?  */
};


/* Function set as reader function when reading PNG image from memory.
   PNG_PTR is a pointer to the PNG control structure.  Copy LENGTH
   bytes from the input to DATA.  */

static void
png_read_from_memory (png_structp png_ptr, png_bytep data, png_size_t length)
{
  struct png_memory_storage *tbr = fn_png_get_io_ptr (png_ptr);

  if (length > tbr->len - tbr->index)
    fn_png_error (png_ptr, "Read error");

  memcpy (data, tbr->bytes + tbr->index, length);
  tbr->index = tbr->index + length;
}


/* Function set as reader function when reading PNG image from a file.
   PNG_PTR is a pointer to the PNG control structure.  Copy LENGTH
   bytes from the input to DATA.  */

static void
png_read_from_file (png_structp png_ptr, png_bytep data, png_size_t length)
{
  FILE *fp = fn_png_get_io_ptr (png_ptr);

  if (fread (data, 1, length, fp) < length)
    fn_png_error (png_ptr, "Read error");
}


/* Load PNG image IMG for use on frame F.  Value is true if
   successful.  */

struct png_load_context
{
  /* These are members so that longjmp doesn't munge local variables.  */
  png_struct *png_ptr;
  png_info *info_ptr;
  png_info *end_info;
  FILE *fp;
  png_byte *pixels;
  png_byte **rows;
};

static bool
png_load_body(struct frame *f, struct image *img, struct png_load_context *c)
{
  Lisp_Object file, specified_file;
  Lisp_Object specified_data;
  int x, y;
  ptrdiff_t i;
  XImagePtr ximg, mask_img = NULL;
  png_struct *png_ptr;
  /* volatile has to go AFTER the 'png_info *' to work: */
  png_info *volatile info_ptr = NULL, *end_info = NULL;
  FILE *fp = NULL;
  png_byte sig[8];
  png_byte *pixels = NULL;
  png_byte **rows = NULL;
  png_uint_32 width, height;
  int bit_depth, color_type, interlace_type;
  png_byte channels;
  png_uint_32 row_bytes;
  bool transparent_p;
  struct png_memory_storage tbr;  /* Data to be read */

  /* Find out what file to load.  */
  specified_file = image_spec_value (img->spec, QCfile, NULL);
  specified_data = image_spec_value (img->spec, QCdata, NULL);

  if (NILP (specified_data))
    {
      file = x_find_image_file (specified_file);
      if (!STRINGP (file))
	{
	  image_error ("Cannot find image file `%s'", specified_file, Qnil);
	  return 0;
	}

      /* Open the image file.  */
      fp = emacs_fopen (SSDATA (file), "rb");
      if (!fp)
	{
	  image_error ("Cannot open image file `%s'", file, Qnil);
	  return 0;
	}

      /* Check PNG signature.  */
      if (fread (sig, 1, sizeof sig, fp) != sizeof sig
	  || fn_png_sig_cmp (sig, 0, sizeof sig))
	{
	  fclose (fp);
	  image_error ("Not a PNG file: `%s'", file, Qnil);
	  return 0;
	}
    }
  else
    {
      if (!STRINGP (specified_data))
	{
	  image_error ("Invalid image data `%s'", specified_data, Qnil);
	  return 0;
	}

      /* Read from memory.  */
      tbr.bytes = SDATA (specified_data);
      tbr.len = SBYTES (specified_data);
      tbr.index = 0;

      /* Check PNG signature.  */
      if (tbr.len < sizeof sig
	  || fn_png_sig_cmp (tbr.bytes, 0, sizeof sig))
	{
	  image_error ("Not a PNG image: `%s'", img->spec, Qnil);
	  return 0;
	}

      /* Need to skip past the signature.  */
      tbr.bytes += sizeof (sig);
    }

  /* Initialize read and info structs for PNG lib.  */
  png_ptr = fn_png_create_read_struct (PNG_LIBPNG_VER_STRING,
				       NULL, my_png_error,
				       my_png_warning);
  if (png_ptr)
    {
      info_ptr = fn_png_create_info_struct (png_ptr);
      end_info = fn_png_create_info_struct (png_ptr);
    }

  c->png_ptr = png_ptr;
  c->info_ptr = info_ptr;
  c->end_info = end_info;
  c->fp = fp;
  c->pixels = pixels;
  c->rows = rows;

  if (! (info_ptr && end_info))
    {
      fn_png_destroy_read_struct (&c->png_ptr, &c->info_ptr, &c->end_info);
      png_ptr = 0;
    }
  if (! png_ptr)
    {
      if (fp) fclose (fp);
      return 0;
    }

  /* Set error jump-back.  We come back here when the PNG library
     detects an error.  */
  if (FAST_SETJMP (PNG_JMPBUF (png_ptr)))
    {
    error:
      if (c->png_ptr)
	fn_png_destroy_read_struct (&c->png_ptr, &c->info_ptr, &c->end_info);
      xfree (c->pixels);
      xfree (c->rows);
      if (c->fp)
	fclose (c->fp);
      return 0;
    }

  /* Silence a bogus diagnostic; see GCC bug 54561.  */
  IF_LINT (fp = c->fp);

  /* Read image info.  */
  if (!NILP (specified_data))
    fn_png_set_read_fn (png_ptr, &tbr, png_read_from_memory);
  else
    fn_png_set_read_fn (png_ptr, fp, png_read_from_file);

  fn_png_set_sig_bytes (png_ptr, sizeof sig);
  fn_png_read_info (png_ptr, info_ptr);
  fn_png_get_IHDR (png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
		   &interlace_type, NULL, NULL);

  if (! (width <= INT_MAX && height <= INT_MAX
	 && check_image_size (f, width, height)))
    {
      image_error ("Invalid image size (see `max-image-size')", Qnil, Qnil);
      goto error;
    }

  /* Create the X image and pixmap now, so that the work below can be
     omitted if the image is too large for X.  */
  if (!image_create_x_image_and_pixmap (f, img, width, height, 0, &ximg, 0))
    goto error;

  /* If image contains simply transparency data, we prefer to
     construct a clipping mask.  */
  if (fn_png_get_valid (png_ptr, info_ptr, PNG_INFO_tRNS))
    transparent_p = 1;
  else
    transparent_p = 0;

  /* This function is easier to write if we only have to handle
     one data format: RGB or RGBA with 8 bits per channel.  Let's
     transform other formats into that format.  */

  /* Strip more than 8 bits per channel.  */
  if (bit_depth == 16)
    fn_png_set_strip_16 (png_ptr);

  /* Expand data to 24 bit RGB, or 8 bit grayscale, with alpha channel
     if available.  */
  fn_png_set_expand (png_ptr);

  /* Convert grayscale images to RGB.  */
  if (color_type == PNG_COLOR_TYPE_GRAY
      || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    fn_png_set_gray_to_rgb (png_ptr);

  /* Handle alpha channel by combining the image with a background
     color.  Do this only if a real alpha channel is supplied.  For
     simple transparency, we prefer a clipping mask.  */
  if (!transparent_p)
    {
      /* png_color_16 *image_bg; */
      Lisp_Object specified_bg
	= image_spec_value (img->spec, QCbackground, NULL);
      int shift = (bit_depth == 16) ? 0 : 8;

      if (STRINGP (specified_bg))
	/* The user specified `:background', use that.  */
	{
	  XColor color;
	  if (x_defined_color (f, SSDATA (specified_bg), &color, 0))
	    {
	      png_color_16 user_bg;

	      memset (&user_bg, 0, sizeof user_bg);
	      user_bg.red = color.red >> shift;
	      user_bg.green = color.green >> shift;
	      user_bg.blue = color.blue >> shift;

	      fn_png_set_background (png_ptr, &user_bg,
				     PNG_BACKGROUND_GAMMA_SCREEN, 0, 1.0);
	    }
	}
      else
	{
	  /* We use the current frame background, ignoring any default
	     background color set by the image.  */
#if defined (HAVE_X_WINDOWS) || defined (HAVE_NTGUI)
	  XColor color;
	  png_color_16 frame_background;

	  color.pixel = FRAME_BACKGROUND_PIXEL (f);
	  x_query_color (f, &color);

	  memset (&frame_background, 0, sizeof frame_background);
	  frame_background.red = color.red >> shift;
	  frame_background.green = color.green >> shift;
	  frame_background.blue = color.blue >> shift;
#endif /* HAVE_X_WINDOWS */

#ifdef MAC_OS
	  unsigned long color;
	  png_color_16 frame_background;
	  color = FRAME_BACKGROUND_PIXEL(f);
# if 0 /* MAC TODO: Colormap support.  */
	  x_query_color(f, &color);
# endif /* 0 */
	  bzero(&frame_background, sizeof(frame_background));
	  frame_background.red = RED_FROM_ULONG(color);
	  frame_background.green = GREEN_FROM_ULONG(color);
	  frame_background.blue = BLUE_FROM_ULONG(color);
#endif /* MAC_OS */

	  fn_png_set_background (png_ptr, &frame_background,
				 PNG_BACKGROUND_GAMMA_SCREEN, 0, 1.0);
	}
    }

  /* Update info structure.  */
  fn_png_read_update_info (png_ptr, info_ptr);

  /* Get number of channels.  Valid values are 1 for grayscale images
     and images with a palette, 2 for grayscale images with transparency
     information (alpha channel), 3 for RGB images, and 4 for RGB
     images with alpha channel, i.e. RGBA.  If conversions above were
     sufficient we should only have 3 or 4 channels here.  */
  channels = fn_png_get_channels (png_ptr, info_ptr);
  eassert (channels == 3 || channels == 4);

  /* Number of bytes needed for one row of the image.  */
  row_bytes = fn_png_get_rowbytes (png_ptr, info_ptr);

  /* Allocate memory for the image.  */
  if (min (PTRDIFF_MAX, SIZE_MAX) / sizeof *rows < height
      || min (PTRDIFF_MAX, SIZE_MAX) / sizeof *pixels / height < row_bytes)
    memory_full (SIZE_MAX);
  c->pixels = pixels = xmalloc (sizeof *pixels * row_bytes * height);
  c->rows = rows = xmalloc (height * sizeof *rows);
  for (i = 0; i < height; ++i)
    rows[i] = pixels + i * row_bytes;

  /* Read the entire image.  */
  fn_png_read_image (png_ptr, rows);
  fn_png_read_end (png_ptr, info_ptr);
  if (fp)
    {
      fclose (fp);
      c->fp = NULL;
    }

  /* Create an image and pixmap serving as mask if the PNG image
     contains an alpha channel.  */
  if (channels == 4
      && !transparent_p
      && !image_create_x_image_and_pixmap (f, img, width, height, 1,
					   &mask_img, 1))
    {
      x_destroy_x_image (ximg);
      x_clear_image_1 (f, img, CLEAR_IMAGE_PIXMAP);
      goto error;
    }

  /* Fill the X image and mask from PNG data.  */
  init_color_table ();

  for (y = 0; y < height; ++y)
    {
      png_byte *p = rows[y];

      for (x = 0; x < width; ++x)
	{
	  int r, g, b;

	  r = *p++ << 8;
	  g = *p++ << 8;
	  b = *p++ << 8;
	  XPutPixel (ximg, x, y, lookup_rgb_color (f, r, g, b));
	  /* An alpha channel, aka mask channel, associates variable
	     transparency with an image.  Where other image formats
	     support binary transparency---fully transparent or fully
	     opaque---PNG allows up to 254 levels of partial transparency.
	     The PNG library implements partial transparency by combining
	     the image with a specified background color.

	     I'm not sure how to handle this here nicely: because the
	     background on which the image is displayed may change, for
	     real alpha channel support, it would be necessary to create
	     a new image for each possible background.

	     What I'm doing now is that a mask is created if we have
	     boolean transparency information.  Otherwise I'm using
	     the frame's background color to combine the image with.  */

	  if (channels == 4)
	    {
	      if (mask_img)
		XPutPixel (mask_img, x, y, *p > 0 ? PIX_MASK_DRAW : PIX_MASK_RETAIN);
	      ++p;
	    }
	}
    }

  if (NILP(image_spec_value(img->spec, QCbackground, NULL)))
    /* Set IMG's background color from the PNG image, unless the user
       overrode it.  */
    {
      png_color_16 *bg;
      if (fn_png_get_bKGD(png_ptr, info_ptr, &bg))
	{
	  img->background = lookup_rgb_color(f, bg->red, bg->green, bg->blue);
	  img->background_valid = 1;
	}
    }

#ifdef COLOR_TABLE_SUPPORT
  /* Remember colors allocated for this image: */
  img->colors = colors_in_color_table(&img->ncolors);
  free_color_table();
#endif /* COLOR_TABLE_SUPPORT */

  /* Clean up: */
  fn_png_destroy_read_struct(&c->png_ptr, &c->info_ptr, &c->end_info);
  xfree(rows);
  xfree(pixels);

  img->width = width;
  img->height = height;

  /* Maybe fill in the background field while we have ximg handy.
     Casting avoids a GCC warning.  */
  IMAGE_BACKGROUND(img, f, (XImagePtr_or_DC)ximg);

  /* Put ximg into the image: */
  image_put_x_image(f, img, ximg, 0);

  /* Same for the mask: */
  if (mask_img)
    {
      /* Fill in the background_transparent field while we have the
	 mask handy.  Casting avoids a GCC warning.  */
      image_background_transparent(img, f, (XImagePtr_or_DC)mask_img);

      image_put_x_image(f, img, mask_img, 1);
    }

  return 1;
}

static bool
png_load(struct frame *f, struct image *img)
{
  struct png_load_context c;
  return png_load_body(f, img, &c);
}

#else /* HAVE_PNG */
# ifdef MAC_OS
static int png_load(struct frame *f, struct image *img)
{
#  ifdef MAC_OSX
  if (MyCGImageCreateWithPNGDataProvider) {
    return image_load_quartz2d(f, img, 1);
  } else
#  endif /* MAC_OSX */
    return image_load_quicktime(f, img, kQTFileTypePNG);
}
# else
#  ifdef HAVE_NS
static bool
png_load (struct frame *f, struct image *img)
{
  return ns_load_image (f, img,
                        image_spec_value (img->spec, QCfile, NULL),
                        image_spec_value (img->spec, QCdata, NULL));
}
#  endif  /* HAVE_NS */
# endif  /* MAC_OS */
#endif /* !HAVE_PNG */



/***********************************************************************
				 JPEG
 ***********************************************************************/

#if defined (HAVE_JPEG) || defined (MAC_OS) || defined (HAVE_NS)

static bool jpeg_image_p (Lisp_Object object);
static bool jpeg_load (struct frame *f, struct image *img);

/* The symbol `jpeg' identifying images of this type.  */

static Lisp_Object Qjpeg;

/* Indices of image specification fields in gs_format, below.  */

enum jpeg_keyword_index
{
  JPEG_TYPE,
  JPEG_DATA,
  JPEG_FILE,
  JPEG_ASCENT,
  JPEG_MARGIN,
  JPEG_RELIEF,
  JPEG_ALGORITHM,
  JPEG_HEURISTIC_MASK,
  JPEG_MASK,
  JPEG_BACKGROUND,
  JPEG_LAST
};

/* Vector of image_keyword structures describing the format
   of valid user-defined image specifications.  */

static const struct image_keyword jpeg_format[JPEG_LAST] =
{
  {":type",		IMAGE_SYMBOL_VALUE,			1},
  {":data",		IMAGE_STRING_VALUE,			0},
  {":file",		IMAGE_STRING_VALUE,			0},
  {":ascent",		IMAGE_ASCENT_VALUE,			0},
  {":margin",		IMAGE_NON_NEGATIVE_INTEGER_VALUE_OR_PAIR, 0},
  {":relief",		IMAGE_INTEGER_VALUE,			0},
  {":conversions",	IMAGE_DONT_CHECK_VALUE_TYPE,		0},
  {":heuristic-mask",	IMAGE_DONT_CHECK_VALUE_TYPE,		0},
  {":mask",		IMAGE_DONT_CHECK_VALUE_TYPE,		0},
  {":background",	IMAGE_STRING_OR_NIL_VALUE,		0}
};

# if defined HAVE_NTGUI && defined WINDOWSNT
static bool init_jpeg_functions(void);
# else
#  define init_jpeg_functions NULL
# endif /* HAVE_NTGUI && WINDOWSNT */

/* Structure describing the image type `jpeg': */
static struct image_type jpeg_type =
{
  &Qjpeg,
  jpeg_image_p,
  jpeg_load,
  x_clear_image,
  init_jpeg_functions,
  NULL
};

/* Return true if OBJECT is a valid JPEG image specification: */
static bool
jpeg_image_p (Lisp_Object object)
{
  struct image_keyword fmt[JPEG_LAST];

  memcpy (fmt, jpeg_format, sizeof fmt);

  if (!parse_image_spec (object, fmt, JPEG_LAST, Qjpeg))
    return 0;

  /* Must specify either the :data or :file keyword.  */
  return fmt[JPEG_FILE].count + fmt[JPEG_DATA].count == 1;
}

#endif /* HAVE_JPEG || MAC_OS || HAVE_NS */

#ifdef HAVE_JPEG

/* Work around a warning about HAVE_STDLIB_H being redefined in
   jconfig.h.  */
#ifdef HAVE_STDLIB_H
# ifndef HAVE_STDLIB_H_1
#  define HAVE_STDLIB_H_1 1
# endif /* !HAVE_STDLIB_H_1 */
# undef HAVE_STDLIB_H
#endif /* HAVE_STLIB_H */

#if defined (HAVE_NTGUI) && !defined (__WIN32__)
/* In older releases of the jpeg library, jpeglib.h will define boolean
   differently depending on __WIN32__, so make sure it is defined.  */
#define __WIN32__ 1
#endif

/* rpcndr.h (via windows.h) and jpeglib.h both define boolean types.
   Some versions of jpeglib try to detect whether rpcndr.h is loaded,
   using the Windows boolean type instead of the jpeglib boolean type
   if so.  Cygwin jpeglib, however, doesn't try to detect whether its
   headers are included along with windows.h, so under Cygwin, jpeglib
   attempts to define a conflicting boolean type.  Worse, forcing
   Cygwin jpeglib headers to use the Windows boolean type doesn't work
   because it created an ABI incompatibility between the
   already-compiled jpeg library and the header interface definition.

   The best we can do is to define jpeglib's boolean type to a
   different name.  This name, jpeg_boolean, remains in effect through
   the rest of image.c.
*/
#if defined CYGWIN && defined HAVE_NTGUI
#define boolean jpeg_boolean
#endif
#include <jpeglib.h>
#include <jerror.h>

#if defined(HAVE_STLIB_H_1) && HAVE_STDLIB_H_1 && !defined(HAVE_STDLIB_H)
# define HAVE_STDLIB_H 1
#endif /* HAVE_STDLIB_H_1 && !HAVE_STDLIB_H */

#ifdef WINDOWSNT

/* JPEG library details.  */
DEF_IMGLIB_FN (void, jpeg_CreateDecompress, (j_decompress_ptr, int, size_t));
DEF_IMGLIB_FN (boolean, jpeg_start_decompress, (j_decompress_ptr));
DEF_IMGLIB_FN (boolean, jpeg_finish_decompress, (j_decompress_ptr));
DEF_IMGLIB_FN (void, jpeg_destroy_decompress, (j_decompress_ptr));
DEF_IMGLIB_FN (int, jpeg_read_header, (j_decompress_ptr, boolean));
DEF_IMGLIB_FN (JDIMENSION, jpeg_read_scanlines, (j_decompress_ptr, JSAMPARRAY, JDIMENSION));
DEF_IMGLIB_FN (struct jpeg_error_mgr *, jpeg_std_error, (struct jpeg_error_mgr *));
DEF_IMGLIB_FN (boolean, jpeg_resync_to_restart, (j_decompress_ptr, int));

static bool
init_jpeg_functions (void)
{
  HMODULE library;

  if (!(library = w32_delayed_load (Qjpeg)))
    return 0;

  LOAD_IMGLIB_FN (library, jpeg_finish_decompress);
  LOAD_IMGLIB_FN (library, jpeg_read_scanlines);
  LOAD_IMGLIB_FN (library, jpeg_start_decompress);
  LOAD_IMGLIB_FN (library, jpeg_read_header);
  LOAD_IMGLIB_FN (library, jpeg_CreateDecompress);
  LOAD_IMGLIB_FN (library, jpeg_destroy_decompress);
  LOAD_IMGLIB_FN (library, jpeg_std_error);
  LOAD_IMGLIB_FN (library, jpeg_resync_to_restart);
  return 1;
}

/* Wrapper since we can't directly assign the function pointer
   to another function pointer that was declared more completely easily.  */
static boolean
jpeg_resync_to_restart_wrapper (j_decompress_ptr cinfo, int desired)
{
  return fn_jpeg_resync_to_restart (cinfo, desired);
}

#else

#define fn_jpeg_CreateDecompress(a,b,c)	jpeg_create_decompress (a)
#define fn_jpeg_start_decompress	jpeg_start_decompress
#define fn_jpeg_finish_decompress	jpeg_finish_decompress
#define fn_jpeg_destroy_decompress	jpeg_destroy_decompress
#define fn_jpeg_read_header		jpeg_read_header
#define fn_jpeg_read_scanlines		jpeg_read_scanlines
#define fn_jpeg_std_error		jpeg_std_error
#define jpeg_resync_to_restart_wrapper	jpeg_resync_to_restart

#endif /* WINDOWSNT */

struct my_jpeg_error_mgr
{
  struct jpeg_error_mgr pub;
  sys_jmp_buf setjmp_buffer;

  /* The remaining members are so that longjmp doesn't munge local
     variables.  */
  struct jpeg_decompress_struct cinfo;
  enum
    {
      MY_JPEG_ERROR_EXIT,
      MY_JPEG_INVALID_IMAGE_SIZE,
      MY_JPEG_CANNOT_CREATE_X
    } failure_code;
#ifdef lint
  FILE *fp;
#endif /* lint */
};


static _Noreturn void
my_error_exit (j_common_ptr cinfo)
{
  struct my_jpeg_error_mgr *mgr = (struct my_jpeg_error_mgr *) cinfo->err;
  mgr->failure_code = MY_JPEG_ERROR_EXIT;
  sys_longjmp (mgr->setjmp_buffer, 1);
  emacs_abort();
}


/* Init source method for JPEG data source manager.  Called by
   jpeg_read_header() before any data is actually read.  See
   libjpeg.doc from the JPEG lib distribution.  */
static void our_common_init_source(j_decompress_ptr cinfo)
{
  return;
}


/* Method to terminate data source.  Called by
   jpeg_finish_decompress() after all data has been processed.  */
static void our_common_term_source(j_decompress_ptr cinfo)
{
  return;
}


/* Fill input buffer method for JPEG data source manager.  Called
   whenever more data is needed.  We read the whole image in one step,
   so this only adds a fake end of input marker at the end.  */

static JOCTET our_memory_buffer[2];

static boolean
our_memory_fill_input_buffer (j_decompress_ptr cinfo)
{
  /* Insert a fake EOI marker.  */
  struct jpeg_source_mgr *src = cinfo->src;

  our_memory_buffer[0] = (JOCTET) 0xFF;
  our_memory_buffer[1] = (JOCTET) JPEG_EOI;

  src->next_input_byte = our_memory_buffer;
  src->bytes_in_buffer = 2;
  return 1;
}


/* Method to skip over NUM_BYTES bytes in the image data.  CINFO->src
   is the JPEG data source manager.  */

static void
our_memory_skip_input_data (j_decompress_ptr cinfo, long int num_bytes)
{
  struct jpeg_source_mgr *src = cinfo->src;

  if (src)
    {
      if (num_bytes > src->bytes_in_buffer)
	ERREXIT (cinfo, JERR_INPUT_EOF);

      src->bytes_in_buffer -= num_bytes;
      src->next_input_byte += num_bytes;
    }
}


/* Set up the JPEG lib for reading an image from DATA which contains
   LEN bytes.  CINFO is the decompression info structure created for
   reading the image.  */

static void
jpeg_memory_src (j_decompress_ptr cinfo, JOCTET *data, ptrdiff_t len)
{
  struct jpeg_source_mgr *src = cinfo->src;

  if (! src)
    {
      /* First time for this JPEG object?  */
      src = cinfo->mem->alloc_small ((j_common_ptr) cinfo,
				     JPOOL_PERMANENT, sizeof *src);
      cinfo->src = src;
      src->next_input_byte = data;
    }

  src->init_source = our_common_init_source;
  src->fill_input_buffer = our_memory_fill_input_buffer;
  src->skip_input_data = our_memory_skip_input_data;
  src->resync_to_restart = jpeg_resync_to_restart_wrapper; /* Use default method.  */
  src->term_source = our_common_term_source;
  src->bytes_in_buffer = len;
  src->next_input_byte = data;
}


struct jpeg_stdio_mgr
{
  struct jpeg_source_mgr mgr;
  boolean finished;
  FILE *file;
  JOCTET *buffer;
};


/* Size of buffer to read JPEG from file.
   Not too big, as we want to use alloc_small.  */
#define JPEG_STDIO_BUFFER_SIZE 8192


/* Fill input buffer method for JPEG data source manager.  Called
   whenever more data is needed.  The data is read from a FILE *.  */

static boolean
our_stdio_fill_input_buffer (j_decompress_ptr cinfo)
{
  struct jpeg_stdio_mgr *src;

  src = (struct jpeg_stdio_mgr *) cinfo->src;
  if (!src->finished)
    {
      ptrdiff_t bytes;

      bytes = fread (src->buffer, 1, JPEG_STDIO_BUFFER_SIZE, src->file);
      if (bytes > 0)
        src->mgr.bytes_in_buffer = bytes;
      else
        {
          WARNMS (cinfo, JWRN_JPEG_EOF);
          src->finished = 1;
          src->buffer[0] = (JOCTET) 0xFF;
          src->buffer[1] = (JOCTET) JPEG_EOI;
          src->mgr.bytes_in_buffer = 2;
        }
      src->mgr.next_input_byte = src->buffer;
    }

  return 1;
}


/* Method to skip over NUM_BYTES bytes in the image data.  CINFO->src
   is the JPEG data source manager.  */

static void
our_stdio_skip_input_data (j_decompress_ptr cinfo, long int num_bytes)
{
  struct jpeg_stdio_mgr *src;
  src = (struct jpeg_stdio_mgr *) cinfo->src;

  while (num_bytes > 0 && !src->finished)
    {
      if (num_bytes <= src->mgr.bytes_in_buffer)
        {
          src->mgr.bytes_in_buffer -= num_bytes;
          src->mgr.next_input_byte += num_bytes;
          break;
        }
      else
        {
          num_bytes -= src->mgr.bytes_in_buffer;
          src->mgr.bytes_in_buffer = 0;
          src->mgr.next_input_byte = NULL;

          our_stdio_fill_input_buffer (cinfo);
        }
    }
}


/* Set up the JPEG lib for reading an image from a FILE *.
   CINFO is the decompression info structure created for
   reading the image.  */

static void
jpeg_file_src (j_decompress_ptr cinfo, FILE *fp)
{
  struct jpeg_stdio_mgr *src = (struct jpeg_stdio_mgr *) cinfo->src;

  if (! src)
    {
      /* First time for this JPEG object?  */
      src = cinfo->mem->alloc_small ((j_common_ptr) cinfo,
				     JPOOL_PERMANENT, sizeof *src);
      cinfo->src = (struct jpeg_source_mgr *) src;
      src->buffer = cinfo->mem->alloc_small ((j_common_ptr) cinfo,
					     JPOOL_PERMANENT,
					     JPEG_STDIO_BUFFER_SIZE);
    }

  src->file = fp;
  src->finished = 0;
  src->mgr.init_source = our_common_init_source;
  src->mgr.fill_input_buffer = our_stdio_fill_input_buffer;
  src->mgr.skip_input_data = our_stdio_skip_input_data;
  src->mgr.resync_to_restart = jpeg_resync_to_restart_wrapper; /* Use default method.  */
  src->mgr.term_source = our_common_term_source;
  src->mgr.bytes_in_buffer = 0;
  src->mgr.next_input_byte = NULL;
}


/* Load image IMG for use on frame F.  Patterned after example.c
   from the JPEG lib.  */

static bool
jpeg_load_body(struct frame *f, struct image *img,
               struct my_jpeg_error_mgr *mgr)
{
  Lisp_Object file, specified_file;
  Lisp_Object specified_data;
  /* the 'volatile' has to go AFTER the 'FILE *' part to work: */
  FILE *volatile fp = NULL;
  JSAMPARRAY buffer;
  int row_stride, x, y;
  XImagePtr ximg = NULL;
  unsigned long *colors;
  int width, height;

  /* Open the JPEG file: */
  specified_file = image_spec_value(img->spec, QCfile, NULL);
  specified_data = image_spec_value(img->spec, QCdata, NULL);

  if (NILP(specified_data))
    {
      file = x_find_image_file(specified_file);
      if (!STRINGP(file))
	{
	  image_error("Cannot find image file `%s'", specified_file, Qnil);
	  return 0;
	}

      fp = emacs_fopen(SSDATA(file), "rb");
      if (fp == NULL)
	{
	  image_error("Cannot open `%s'", file, Qnil);
	  return 0;
	}
    }
  else if (!STRINGP(specified_data))
    {
      image_error("Invalid image data `%s'", specified_data, Qnil);
      return 0;
    }

  IF_LINT(mgr->fp = fp);

  /* Customize libjpeg's error handling to call my_error_exit when an
     error is detected.  This function will perform a longjmp.  */
  mgr->cinfo.err = fn_jpeg_std_error(&mgr->pub);
  mgr->pub.error_exit = my_error_exit;
  if (sys_setjmp(mgr->setjmp_buffer))
    {
      switch (mgr->failure_code)
	{
	case MY_JPEG_ERROR_EXIT:
	  {
	    char buf[JMSG_LENGTH_MAX];
	    mgr->cinfo.err->format_message((j_common_ptr)&mgr->cinfo, buf);
	    image_error("Error reading JPEG image `%s': %s", img->spec,
                        build_string(buf));
	    break;
	  }
	case MY_JPEG_INVALID_IMAGE_SIZE:
	  image_error("Invalid image size (see `max-image-size')", Qnil, Qnil);
	  break;
	case MY_JPEG_CANNOT_CREATE_X:
	  break;
        default:
          break;
	}

      /* Close the input file and destroy the JPEG object.  */
      if (fp)
	fclose (fp);
      fn_jpeg_destroy_decompress (&mgr->cinfo);

      /* If we already have an XImage, free that.  */
      x_destroy_x_image (ximg);

      /* Free pixmap and colors.  */
      x_clear_image (f, img);
      return 0;
    }

  /* Silence a bogus diagnostic; see GCC bug 54561.  */
  IF_LINT (fp = mgr->fp);

  /* Create the JPEG decompression object.  Let it read from fp.
	 Read the JPEG image header.  */
  fn_jpeg_CreateDecompress (&mgr->cinfo, JPEG_LIB_VERSION, sizeof *&mgr->cinfo);

  if (NILP (specified_data))
    jpeg_file_src (&mgr->cinfo, fp);
  else
    jpeg_memory_src (&mgr->cinfo, SDATA (specified_data),
		     SBYTES (specified_data));

  fn_jpeg_read_header (&mgr->cinfo, 1);

  /* Customize decompression so that color quantization will be used.
	 Start decompression.  */
  mgr->cinfo.quantize_colors = 1;
  fn_jpeg_start_decompress (&mgr->cinfo);
  width = img->width = mgr->cinfo.output_width;
  height = img->height = mgr->cinfo.output_height;

  if (!check_image_size (f, width, height))
    {
      mgr->failure_code = MY_JPEG_INVALID_IMAGE_SIZE;
      sys_longjmp (mgr->setjmp_buffer, 1);
    }

  /* Create X image and pixmap.  */
  if (!image_create_x_image_and_pixmap (f, img, width, height, 0, &ximg, 0))
    {
      mgr->failure_code = MY_JPEG_CANNOT_CREATE_X;
      sys_longjmp (mgr->setjmp_buffer, 1);
    }

  /* Allocate colors.  When color quantization is used,
     mgr->cinfo.actual_number_of_colors has been set with the number of
     colors generated, and mgr->cinfo.colormap is a two-dimensional array
     of color indices in the range 0..mgr->cinfo.actual_number_of_colors.
     No more than 255 colors will be generated.  */
  {
    int i, ir, ig, ib;

    if (mgr->cinfo.out_color_components > 2)
      ir = 0, ig = 1, ib = 2;
    else if (mgr->cinfo.out_color_components > 1)
      ir = 0, ig = 1, ib = 0;
    else
      ir = 0, ig = 0, ib = 0;

    /* Use the color table mechanism because it handles colors that
       cannot be allocated nicely.  Such colors will be replaced with
       a default color, and we don't have to care about which colors
       can be freed safely, and which can't.  */
    init_color_table ();
    colors = alloca (mgr->cinfo.actual_number_of_colors * sizeof *colors);

    for (i = 0; i < mgr->cinfo.actual_number_of_colors; ++i)
      {
	/* Multiply RGB values with 255 because X expects RGB values
	   in the range 0..0xffff.  */
	int r = mgr->cinfo.colormap[ir][i] << 8;
	int g = mgr->cinfo.colormap[ig][i] << 8;
	int b = mgr->cinfo.colormap[ib][i] << 8;
	colors[i] = lookup_rgb_color (f, r, g, b);
      }

#ifdef COLOR_TABLE_SUPPORT
    /* Remember those colors actually allocated.  */
    img->colors = colors_in_color_table (&img->ncolors);
    free_color_table ();
#endif /* COLOR_TABLE_SUPPORT */
  }

  /* Read pixels.  */
  row_stride = width * mgr->cinfo.output_components;
  buffer = mgr->cinfo.mem->alloc_sarray ((j_common_ptr) &mgr->cinfo,
					 JPOOL_IMAGE, row_stride, 1);
  for (y = 0; y < height; ++y)
    {
      fn_jpeg_read_scanlines (&mgr->cinfo, buffer, 1);
      for (x = 0; x < mgr->cinfo.output_width; ++x)
	XPutPixel (ximg, x, y, colors[buffer[0][x]]);
    }

  /* Clean up.  */
  fn_jpeg_finish_decompress (&mgr->cinfo);
  fn_jpeg_destroy_decompress (&mgr->cinfo);
  if (fp)
    fclose (fp);

  /* Maybe fill in the background field while we have ximg handy. */
  if (NILP (image_spec_value (img->spec, QCbackground, NULL)))
    /* Casting avoids a GCC warning.  */
    IMAGE_BACKGROUND (img, f, (XImagePtr_or_DC)ximg);

  /* Put ximg into the image.  */
  image_put_x_image (f, img, ximg, 0);
  return 1;
}

static bool
jpeg_load (struct frame *f, struct image *img)
{
  struct my_jpeg_error_mgr mgr;
  return jpeg_load_body (f, img, &mgr);
}

#else /* HAVE_JPEG */
# ifdef MAC_OS
static int jpeg_load(struct frame *f, struct image *img)
{
#  ifdef MAC_OSX
  return image_load_quartz2d(f, img, 0);
#  else
  return image_load_quicktime(f, img, kQTFileTypeJPEG);
#  endif /* MAC_OSX */
}
# else
#  ifdef HAVE_NS
static bool
jpeg_load (struct frame *f, struct image *img)
{
  return ns_load_image (f, img,
			image_spec_value (img->spec, QCfile, NULL),
			image_spec_value (img->spec, QCdata, NULL));
}
#  endif  /* HAVE_NS */
# endif  /* MAC_OS */
#endif /* !HAVE_JPEG */



/***********************************************************************
				 TIFF
 ***********************************************************************/

#if defined (HAVE_TIFF) || defined (MAC_OS) || defined (HAVE_NS)

static bool tiff_image_p (Lisp_Object object);
static bool tiff_load (struct frame *f, struct image *img);

/* The symbol `tiff' identifying images of this type.  */

static Lisp_Object Qtiff;

/* Indices of image specification fields in tiff_format, below.  */

enum tiff_keyword_index
{
  TIFF_TYPE,
  TIFF_DATA,
  TIFF_FILE,
  TIFF_ASCENT,
  TIFF_MARGIN,
  TIFF_RELIEF,
  TIFF_ALGORITHM,
  TIFF_HEURISTIC_MASK,
  TIFF_MASK,
  TIFF_BACKGROUND,
  TIFF_INDEX,
  TIFF_LAST
};

/* Vector of image_keyword structures describing the format
   of valid user-defined image specifications.  */

static const struct image_keyword tiff_format[TIFF_LAST] =
{
  {":type",		IMAGE_SYMBOL_VALUE,			1},
  {":data",		IMAGE_STRING_VALUE,			0},
  {":file",		IMAGE_STRING_VALUE,			0},
  {":ascent",		IMAGE_ASCENT_VALUE,			0},
  {":margin",		IMAGE_NON_NEGATIVE_INTEGER_VALUE_OR_PAIR, 0},
  {":relief",		IMAGE_INTEGER_VALUE,			0},
  {":conversions",	IMAGE_DONT_CHECK_VALUE_TYPE,		0},
  {":heuristic-mask",	IMAGE_DONT_CHECK_VALUE_TYPE,		0},
  {":mask",		IMAGE_DONT_CHECK_VALUE_TYPE,		0},
  {":background",	IMAGE_STRING_OR_NIL_VALUE,		0},
  {":index",		IMAGE_NON_NEGATIVE_INTEGER_VALUE,	0}
};

# if defined HAVE_NTGUI && defined WINDOWSNT
static bool init_tiff_functions (void);
# else
#  define init_tiff_functions NULL
# endif /* HAVE_NTGUI && WINDOWSNT */

/* Structure describing the image type `tiff': */
static struct image_type tiff_type =
{
  &Qtiff,
  tiff_image_p,
  tiff_load,
  x_clear_image,
  init_tiff_functions,
  NULL
};

/* Return true if OBJECT is a valid TIFF image specification: */
static bool
tiff_image_p (Lisp_Object object)
{
  struct image_keyword fmt[TIFF_LAST];
  memcpy (fmt, tiff_format, sizeof fmt);

  if (!parse_image_spec (object, fmt, TIFF_LAST, Qtiff))
    return 0;

  /* Must specify either the :data or :file keyword.  */
  return fmt[TIFF_FILE].count + fmt[TIFF_DATA].count == 1;
}

#endif /* HAVE_TIFF || MAC_OS || HAVE_NS */

#ifdef HAVE_TIFF

#include <tiffio.h>

#ifdef WINDOWSNT

/* TIFF library details.  */
DEF_IMGLIB_FN (TIFFErrorHandler, TIFFSetErrorHandler, (TIFFErrorHandler));
DEF_IMGLIB_FN (TIFFErrorHandler, TIFFSetWarningHandler, (TIFFErrorHandler));
DEF_IMGLIB_FN (TIFF *, TIFFOpen, (const char *, const char *));
DEF_IMGLIB_FN (TIFF *, TIFFClientOpen, (const char *, const char *, thandle_t,
				TIFFReadWriteProc, TIFFReadWriteProc,
				TIFFSeekProc, TIFFCloseProc, TIFFSizeProc,
				TIFFMapFileProc, TIFFUnmapFileProc));
DEF_IMGLIB_FN (int, TIFFGetField, (TIFF *, ttag_t, ...));
DEF_IMGLIB_FN (int, TIFFReadRGBAImage, (TIFF *, uint32, uint32, uint32 *, int));
DEF_IMGLIB_FN (void, TIFFClose, (TIFF *));
DEF_IMGLIB_FN (int, TIFFSetDirectory, (TIFF *, tdir_t));

static bool
init_tiff_functions (void)
{
  HMODULE library;

  if (!(library = w32_delayed_load (Qtiff)))
    return 0;

  LOAD_IMGLIB_FN (library, TIFFSetErrorHandler);
  LOAD_IMGLIB_FN (library, TIFFSetWarningHandler);
  LOAD_IMGLIB_FN (library, TIFFOpen);
  LOAD_IMGLIB_FN (library, TIFFClientOpen);
  LOAD_IMGLIB_FN (library, TIFFGetField);
  LOAD_IMGLIB_FN (library, TIFFReadRGBAImage);
  LOAD_IMGLIB_FN (library, TIFFClose);
  LOAD_IMGLIB_FN (library, TIFFSetDirectory);
  return 1;
}

#else

#define fn_TIFFSetErrorHandler		TIFFSetErrorHandler
#define fn_TIFFSetWarningHandler	TIFFSetWarningHandler
#define fn_TIFFOpen			TIFFOpen
#define fn_TIFFClientOpen		TIFFClientOpen
#define fn_TIFFGetField			TIFFGetField
#define fn_TIFFReadRGBAImage		TIFFReadRGBAImage
#define fn_TIFFClose			TIFFClose
#define fn_TIFFSetDirectory		TIFFSetDirectory
#endif /* WINDOWSNT */


/* Reading from a memory buffer for TIFF images Based on the PNG
   memory source, but we have to provide a lot of extra functions.
   Blah.

   We really only need to implement read and seek, but I am not
   convinced that the TIFF library is smart enough not to destroy
   itself if we only hand it the function pointers we need to
   override.  */

typedef struct
{
  unsigned char *bytes;
  ptrdiff_t len;
  ptrdiff_t index;
}
tiff_memory_source;

static tsize_t
tiff_read_from_memory (thandle_t data, tdata_t buf, tsize_t size)
{
  tiff_memory_source *src = (tiff_memory_source *) data;

  size = min (size, src->len - src->index);
  memcpy (buf, src->bytes + src->index, size);
  src->index += size;
  return size;
}

static tsize_t
tiff_write_from_memory (thandle_t data, tdata_t buf, tsize_t size)
{
  return -1;
}

static toff_t
tiff_seek_in_memory(thandle_t data, toff_t off, int whence)
{
  tiff_memory_source *src = (tiff_memory_source *)data;
  ptrdiff_t idx;

  switch (whence)
    {
    case SEEK_SET:		/* Go from beginning of source.  */
      idx = off;
      break;
    case SEEK_END:		/* Go from end of source.  */
      idx = (src->len + off);
      break;
    case SEEK_CUR:		/* Go from current position.  */
      idx = (src->index + off);
      break;
    default:			/* Invalid `whence'.   */
      return -1;
    }

  if ((idx > src->len) || (idx < 0))
    return -1;

  src->index = idx;
  return src->index;
}

static int
tiff_close_memory (thandle_t data)
{
  /* NOOP */
  return 0;
}

static int
tiff_mmap_memory(thandle_t data, tdata_t *pbase, toff_t *psize)
{
  /* It is already _IN_ memory. */
  return 0;
}

static void
tiff_unmap_memory(thandle_t data, tdata_t base, toff_t size)
{
  return; /* We do NOT need to do this. */
}

static toff_t
tiff_size_of_memory (thandle_t data)
{
  return ((tiff_memory_source *) data)->len;
}

/* GCC 3.x on x86 Windows targets has a bug that triggers an internal
   compiler error compiling tiff_handler, see Bugzilla bug #17406
   (http://gcc.gnu.org/bugzilla/show_bug.cgi?id=17406).  Declaring
   this function as external works around that problem.  */
#if defined (__MINGW32__) && __GNUC__ == 3
# define MINGW_STATIC
#else
# define MINGW_STATIC static
#endif

MINGW_STATIC void
tiff_handler (const char *, const char *, const char *, va_list)
  ATTRIBUTE_FORMAT_PRINTF (3, 0);
MINGW_STATIC void
tiff_handler (const char *log_format, const char *title,
	      const char *format, va_list ap)
{
  /* doprnt is not suitable here, as TIFF handlers are called from
     libtiff and are passed arbitrary printf directives.  Instead, use
     vsnprintf, taking care to be portable to nonstandard environments
     where vsnprintf returns -1 on buffer overflow.  Since it's just a
     log entry, it's OK to truncate it.  */
  char buf[4000];
  int len = vsnprintf (buf, sizeof buf, format, ap);
  add_to_log (log_format, build_string (title),
	      make_string (buf, max (0, min (len, sizeof buf - 1))));
}
#undef MINGW_STATIC

static void tiff_error_handler (const char *, const char *, va_list)
  ATTRIBUTE_FORMAT_PRINTF (2, 0);
static void
tiff_error_handler (const char *title, const char *format, va_list ap)
{
  tiff_handler ("TIFF error: %s %s", title, format, ap);
}


static void tiff_warning_handler (const char *, const char *, va_list)
  ATTRIBUTE_FORMAT_PRINTF (2, 0);
static void
tiff_warning_handler (const char *title, const char *format, va_list ap)
{
  tiff_handler ("TIFF warning: %s %s", title, format, ap);
}


/* Load TIFF image IMG for use on frame F.  Value is true if
   successful.  */

static bool
tiff_load (struct frame *f, struct image *img)
{
  Lisp_Object file, specified_file;
  Lisp_Object specified_data;
  TIFF *tiff;
  int width, height, x, y, count;
  uint32 *buf;
  int rc;
  XImagePtr ximg;
  tiff_memory_source memsrc;
  Lisp_Object image;

  specified_file = image_spec_value (img->spec, QCfile, NULL);
  specified_data = image_spec_value (img->spec, QCdata, NULL);

  fn_TIFFSetErrorHandler ((TIFFErrorHandler) tiff_error_handler);
  fn_TIFFSetWarningHandler ((TIFFErrorHandler) tiff_warning_handler);

  if (NILP (specified_data))
    {
      /* Read from a file */
      file = x_find_image_file (specified_file);
      if (!STRINGP (file))
	{
	  image_error ("Cannot find image file `%s'", specified_file, Qnil);
	  return 0;
	}
#ifdef WINDOWSNT
      file = ansi_encode_filename (file);
#endif

      /* Try to open the image file.  */
      tiff = fn_TIFFOpen (SSDATA (file), "r");
      if (tiff == NULL)
	{
	  image_error ("Cannot open `%s'", file, Qnil);
	  return 0;
	}
    }
  else
    {
      if (!STRINGP (specified_data))
	{
	  image_error ("Invalid image data `%s'", specified_data, Qnil);
	  return 0;
	}

      /* Memory source! */
      memsrc.bytes = SDATA (specified_data);
      memsrc.len = SBYTES (specified_data);
      memsrc.index = 0;

      tiff = fn_TIFFClientOpen ("memory_source", "r", (thandle_t)&memsrc,
				tiff_read_from_memory,
				tiff_write_from_memory,
				tiff_seek_in_memory,
				tiff_close_memory,
				tiff_size_of_memory,
				tiff_mmap_memory,
				tiff_unmap_memory);

      if (!tiff)
	{
	  image_error ("Cannot open memory source for `%s'", img->spec, Qnil);
	  return 0;
	}
    }

  image = image_spec_value (img->spec, QCindex, NULL);
  if (INTEGERP (image))
    {
      EMACS_INT ino = XFASTINT (image);
      if (! (TYPE_MINIMUM (tdir_t) <= ino && ino <= TYPE_MAXIMUM (tdir_t)
	     && fn_TIFFSetDirectory (tiff, ino)))
	{
	  image_error ("Invalid image number `%s' in image `%s'",
		       image, img->spec);
	  fn_TIFFClose (tiff);
	  return 0;
	}
    }

  /* Get width and height of the image, and allocate a raster buffer
     of width x height 32-bit values.  */
  fn_TIFFGetField (tiff, TIFFTAG_IMAGEWIDTH, &width);
  fn_TIFFGetField (tiff, TIFFTAG_IMAGELENGTH, &height);

  if (!check_image_size (f, width, height))
    {
      image_error ("Invalid image size (see `max-image-size')", Qnil, Qnil);
      fn_TIFFClose (tiff);
      return 0;
    }

  /* Create the X image and pixmap.  */
  if (! (height <= min (PTRDIFF_MAX, SIZE_MAX) / sizeof *buf / width
	 && image_create_x_image_and_pixmap (f, img, width, height, 0,
					     &ximg, 0)))
    {
      fn_TIFFClose (tiff);
      return 0;
    }

  buf = xmalloc (sizeof *buf * width * height);

  rc = fn_TIFFReadRGBAImage (tiff, width, height, buf, 0);

  /* Count the number of images in the file.  */
  for (count = 1; fn_TIFFSetDirectory (tiff, count); count++)
    continue;

  if (count > 1)
    img->lisp_data = Fcons (Qcount,
			    Fcons (make_number (count),
				   img->lisp_data));

  fn_TIFFClose (tiff);
  if (!rc)
    {
      image_error ("Error reading TIFF image `%s'", img->spec, Qnil);
      xfree (buf);
      return 0;
    }

  /* Initialize the color table.  */
  init_color_table ();

  /* Process the pixel raster.  Origin is in the lower-left corner.  */
  for (y = 0; y < height; ++y)
    {
      uint32 *row = buf + y * width;

      for (x = 0; x < width; ++x)
	{
	  uint32 abgr = row[x];
	  int r = TIFFGetR (abgr) << 8;
	  int g = TIFFGetG (abgr) << 8;
	  int b = TIFFGetB (abgr) << 8;
	  XPutPixel (ximg, x, height - 1 - y, lookup_rgb_color (f, r, g, b));
	}
    }

#ifdef COLOR_TABLE_SUPPORT
  /* Remember the colors allocated for the image.  Free the color table.  */
  img->colors = colors_in_color_table (&img->ncolors);
  free_color_table ();
#endif /* COLOR_TABLE_SUPPORT */

  img->width = width;
  img->height = height;

  /* Maybe fill in the background field while we have ximg handy. */
  if (NILP (image_spec_value (img->spec, QCbackground, NULL)))
    /* Casting avoids a GCC warning on W32.  */
    IMAGE_BACKGROUND (img, f, (XImagePtr_or_DC)ximg);

  /* Put ximg into the image.  */
  image_put_x_image (f, img, ximg, 0);
  xfree (buf);

  return 1;
}

#else /* HAVE_TIFF */
# ifdef MAC_OS
static int tiff_load(struct frame *f, struct image *img)
{
  return image_load_quicktime(f, img, kQTFileTypeTIFF);
}
# else
#  ifdef HAVE_NS
static bool
tiff_load (struct frame *f, struct image *img)
{
  return ns_load_image (f, img,
                        image_spec_value (img->spec, QCfile, NULL),
                        image_spec_value (img->spec, QCdata, NULL));
}
#  endif /* HAVE_NS */
# endif /* MAC_OS */
#endif /* !HAVE_TIFF */



/***********************************************************************
				 GIF
 ***********************************************************************/

#if defined (HAVE_GIF) || defined(MAC_OS) || defined (HAVE_NS)

static bool gif_image_p (Lisp_Object object);
static bool gif_load (struct frame *f, struct image *img);
static void gif_clear_image (struct frame *f, struct image *img);

/* The symbol `gif' identifying images of this type: */
static Lisp_Object Qgif;

/* Indices of image specification fields in gif_format, below: */
enum gif_keyword_index {
  GIF_TYPE,
  GIF_DATA,
  GIF_FILE,
  GIF_ASCENT,
  GIF_MARGIN,
  GIF_RELIEF,
  GIF_ALGORITHM,
  GIF_HEURISTIC_MASK,
  GIF_MASK,
  GIF_IMAGE,
  GIF_BACKGROUND,
  GIF_LAST
};

/* Vector of image_keyword structures describing the format
   of valid user-defined image specifications.  */
static const struct image_keyword gif_format[GIF_LAST] = {
  {":type",		IMAGE_SYMBOL_VALUE,			1},
  {":data",		IMAGE_STRING_VALUE,			0},
  {":file",		IMAGE_STRING_VALUE,			0},
  {":ascent",		IMAGE_ASCENT_VALUE,			0},
  {":margin",		IMAGE_NON_NEGATIVE_INTEGER_VALUE_OR_PAIR, 0},
  {":relief",		IMAGE_INTEGER_VALUE,			0},
  {":conversion",	IMAGE_DONT_CHECK_VALUE_TYPE,		0},
  {":heuristic-mask",	IMAGE_DONT_CHECK_VALUE_TYPE,		0},
  {":mask",		IMAGE_DONT_CHECK_VALUE_TYPE,		0},
  {":index",		IMAGE_NON_NEGATIVE_INTEGER_VALUE,	0},
  {":background",	IMAGE_STRING_OR_NIL_VALUE,		0}
};

# if defined HAVE_NTGUI && defined WINDOWSNT
static bool init_gif_functions (void);
# else
#  define init_gif_functions NULL
# endif /* HAVE_NTGUI && WINDOWSNT */

/* Structure describing the image type `gif': */
static struct image_type gif_type = {
  &Qgif,
  gif_image_p,
  gif_load,
  gif_clear_image,
  init_gif_functions,
  NULL
};

/* Free X resources of GIF image IMG which is used on frame F: */
static void gif_clear_image(struct frame *f, struct image *img)
{
  img->lisp_data = Qnil;
  x_clear_image (f, img);
}

/* Return true if OBJECT is a valid GIF image specification: */
static bool gif_image_p(Lisp_Object object)
{
  struct image_keyword fmt[GIF_LAST];
  memcpy (fmt, gif_format, sizeof fmt);

  if (!parse_image_spec (object, fmt, GIF_LAST, Qgif))
    return 0;

  /* Must specify either the :data or :file keyword.  */
  return fmt[GIF_FILE].count + fmt[GIF_DATA].count == 1;
}

#endif /* HAVE_GIF || MAC_OS || HAVE_NS */

#ifdef HAVE_GIF

#if defined (HAVE_NTGUI) || defined (MAC_OS)
/* winuser.h might define DrawText to DrawTextA or DrawTextW.
 * Undefine before redefining to avoid a preprocessor warning: */
# ifdef DrawText
#  undef DrawText
# endif /* DrawText */
/* avoid conflict with QuickdrawText.h with this: */
# define DrawText gif_DrawText
# include <gif_lib.h>
# undef DrawText
/* Giflib pre-5.0 did NOT define these macros (used only if HAVE_NTGUI): */
# ifndef GIFLIB_MINOR
#  define GIFLIB_MINOR 0
# endif /* !GIFLIB_MINOR */
# ifndef GIFLIB_RELEASE
#  define GIFLIB_RELEASE 0
# endif /* !GIFLIB_RELEASE */
#else /* HAVE_NTGUI || MAC_OS */
# include <gif_lib.h>
#endif /* HAVE_NTGUI || MAC_OS */

/* Giflib before 5.0 did NOT define these macros: */
#ifndef GIFLIB_MAJOR
# define GIFLIB_MAJOR 4
#endif /* !GIFLIB_MAJOR */

#ifdef WINDOWSNT

/* GIF library details.  */
DEF_IMGLIB_FN (int, DGifCloseFile, (GifFileType *));
DEF_IMGLIB_FN (int, DGifSlurp, (GifFileType *));
#if GIFLIB_MAJOR < 5
DEF_IMGLIB_FN (GifFileType *, DGifOpen, (void *, InputFunc));
DEF_IMGLIB_FN (GifFileType *, DGifOpenFileName, (const char *));
#else
DEF_IMGLIB_FN (GifFileType *, DGifOpen, (void *, InputFunc, int *));
DEF_IMGLIB_FN (GifFileType *, DGifOpenFileName, (const char *, int *));
DEF_IMGLIB_FN (char *, GifErrorString, (int));
#endif

static bool
init_gif_functions (void)
{
  HMODULE library;

  if (!(library = w32_delayed_load (Qgif)))
    return 0;

  LOAD_IMGLIB_FN (library, DGifCloseFile);
  LOAD_IMGLIB_FN (library, DGifSlurp);
  LOAD_IMGLIB_FN (library, DGifOpen);
  LOAD_IMGLIB_FN (library, DGifOpenFileName);
#if GIFLIB_MAJOR >= 5
  LOAD_IMGLIB_FN (library, GifErrorString);
#endif
  return 1;
}

#else

#define fn_DGifCloseFile	DGifCloseFile
#define fn_DGifSlurp		DGifSlurp
#define fn_DGifOpen		DGifOpen
#define fn_DGifOpenFileName	DGifOpenFileName
#if 5 <= GIFLIB_MAJOR
# define fn_GifErrorString	GifErrorString
#endif

#endif /* WINDOWSNT */

/* Reading a GIF image from memory
   Based on the PNG memory stuff to a certain extent. */
typedef struct
{
  unsigned char *bytes;
  ptrdiff_t len;
  ptrdiff_t index;
}
gif_memory_source;

/* Make the current memory source available to gif_read_from_memory.
   It is done this way because not all versions of libungif support
   a UserData field in the GifFileType structure.  */
static gif_memory_source *current_gif_memory_src;

static int
gif_read_from_memory (GifFileType *file, GifByteType *buf, int len)
{
  gif_memory_source *src = current_gif_memory_src;

  if (len > src->len - src->index)
    return -1;

  memcpy (buf, src->bytes + src->index, len);
  src->index += len;
  return len;
}


/* Load GIF image IMG for use on frame F.  Value is true if
   successful.  */
static const int interlace_start[] = {0, 4, 2, 1};
static const int interlace_increment[] = {8, 8, 4, 2};

#define GIF_LOCAL_DESCRIPTOR_EXTENSION 249

static bool gif_load(struct frame *f, struct image *img)
{
  Lisp_Object file;
  int rc, width, height, x, y, i, j;
  XImagePtr ximg;
  ColorMapObject *gif_color_map;
  unsigned long pixel_colors[256];
  GifFileType *gif;
  gif_memory_source memsrc;
  Lisp_Object specified_bg = image_spec_value (img->spec, QCbackground, NULL);
  Lisp_Object specified_file = image_spec_value (img->spec, QCfile, NULL);
  Lisp_Object specified_data = image_spec_value (img->spec, QCdata, NULL);
  unsigned long bgcolor = 0;
  EMACS_INT idx;
#if GIFLIB_MAJOR >= 5
  int gif_err;
#endif

  if (NILP (specified_data))
    {
      file = x_find_image_file (specified_file);
      if (!STRINGP (file))
	{
	  image_error ("Cannot find image file `%s'", specified_file, Qnil);
	  return 0;
	}
#ifdef WINDOWSNT
      file = ansi_encode_filename (file);
#endif /* WINDOWSNT */

      /* Open the GIF file.  */
#if GIFLIB_MAJOR < 5
      gif = fn_DGifOpenFileName (SSDATA (file));
      if (gif == NULL)
	{
	  image_error ("Cannot open `%s'", file, Qnil);
	  return 0;
	}
#else
      gif = fn_DGifOpenFileName (SSDATA (file), &gif_err);
      if (gif == NULL)
	{
	  image_error ("Cannot open `%s': %s",
		       file, build_string (fn_GifErrorString (gif_err)));
	  return 0;
	}
#endif
    }
  else
    {
      if (!STRINGP (specified_data))
	{
	  image_error ("Invalid image data `%s'", specified_data, Qnil);
	  return 0;
	}

      /* Read from memory! */
      current_gif_memory_src = &memsrc;
      memsrc.bytes = SDATA (specified_data);
      memsrc.len = SBYTES (specified_data);
      memsrc.index = 0;

#if GIFLIB_MAJOR < 5
      gif = fn_DGifOpen (&memsrc, gif_read_from_memory);
      if (!gif)
	{
	  image_error ("Cannot open memory source `%s'", img->spec, Qnil);
	  return 0;
	}
#else
      gif = fn_DGifOpen (&memsrc, gif_read_from_memory, &gif_err);
      if (!gif)
	{
	  image_error ("Cannot open memory source `%s': %s",
		       img->spec, build_string (fn_GifErrorString (gif_err)));
	  return 0;
	}
#endif
    }

  /* Before reading entire contents, check the declared image size. */
  if (!check_image_size (f, gif->SWidth, gif->SHeight))
    {
      image_error ("Invalid image size (see `max-image-size')", Qnil, Qnil);
      fn_DGifCloseFile (gif);
      return 0;
    }

  /* Read entire contents.  */
  rc = fn_DGifSlurp (gif);
  if (rc == GIF_ERROR || gif->ImageCount <= 0)
    {
      image_error ("Error reading `%s'", img->spec, Qnil);
      fn_DGifCloseFile (gif);
      return 0;
    }

  /* Which sub-image are we to display?  */
  {
    Lisp_Object image_number = image_spec_value (img->spec, QCindex, NULL);
    idx = INTEGERP (image_number) ? XFASTINT (image_number) : 0;
    if (idx < 0 || idx >= gif->ImageCount)
      {
	image_error ("Invalid image number `%s' in image `%s'",
		     image_number, img->spec);
	fn_DGifCloseFile (gif);
	return 0;
      }
  }

  width = img->width = gif->SWidth;
  height = img->height = gif->SHeight;

  img->corners[TOP_CORNER] = gif->SavedImages[0].ImageDesc.Top;
  img->corners[LEFT_CORNER] = gif->SavedImages[0].ImageDesc.Left;
  img->corners[BOT_CORNER]
    = img->corners[TOP_CORNER] + gif->SavedImages[0].ImageDesc.Height;
  img->corners[RIGHT_CORNER]
    = img->corners[LEFT_CORNER] + gif->SavedImages[0].ImageDesc.Width;

  if (!check_image_size (f, width, height))
    {
      image_error ("Invalid image size (see `max-image-size')", Qnil, Qnil);
      fn_DGifCloseFile (gif);
      return 0;
    }

  /* Check that the selected subimages fit.  It's not clear whether
     the GIF spec requires this, but Emacs can crash if they don't fit.  */
  for (j = 0; j <= idx; ++j)
    {
      struct SavedImage *subimage = gif->SavedImages + j;
      int subimg_width = subimage->ImageDesc.Width;
      int subimg_height = subimage->ImageDesc.Height;
      int subimg_top = subimage->ImageDesc.Top;
      int subimg_left = subimage->ImageDesc.Left;
      if (! (0 <= subimg_width && 0 <= subimg_height
	     && 0 <= subimg_top && subimg_top <= height - subimg_height
	     && 0 <= subimg_left && subimg_left <= width - subimg_width))
	{
	  image_error ("Subimage does not fit in image", Qnil, Qnil);
	  fn_DGifCloseFile (gif);
	  return 0;
	}
    }

  /* Create the X image and pixmap.  */
  if (!image_create_x_image_and_pixmap (f, img, width, height, 0, &ximg, 0))
    {
      fn_DGifCloseFile (gif);
      return 0;
    }

  /* Clear the part of the screen image not covered by the image.
     Full animated GIF support requires more here (see the gif89 spec,
     disposal methods).  Let's simply assume that the part not covered
     by a sub-image is in the frame's background color.  */
  for (y = 0; y < img->corners[TOP_CORNER]; ++y)
    for (x = 0; x < width; ++x)
      XPutPixel (ximg, x, y, FRAME_BACKGROUND_PIXEL (f));

  for (y = img->corners[BOT_CORNER]; y < height; ++y)
    for (x = 0; x < width; ++x)
      XPutPixel (ximg, x, y, FRAME_BACKGROUND_PIXEL (f));

  for (y = img->corners[TOP_CORNER]; y < img->corners[BOT_CORNER]; ++y)
    {
      for (x = 0; x < img->corners[LEFT_CORNER]; ++x)
	XPutPixel (ximg, x, y, FRAME_BACKGROUND_PIXEL (f));
      for (x = img->corners[RIGHT_CORNER]; x < width; ++x)
	XPutPixel (ximg, x, y, FRAME_BACKGROUND_PIXEL (f));
    }

  /* Read the GIF image into the X image.   */

  /* FIXME: With the current implementation, loading an animated gif
     is quadratic in the number of animation frames, since each frame
     is a separate struct image.  We must provide a way for a single
     gif_load call to construct and save all animation frames.  */
  init_color_table ();
  if (STRINGP (specified_bg))
    bgcolor = x_alloc_image_color (f, img, specified_bg,
				   FRAME_BACKGROUND_PIXEL (f));
  for (j = 0; j <= idx; ++j)
    {
      /* We use a local variable `raster' here because RasterBits is a
	 char *, which invites problems with bytes >= 0x80.  */
      struct SavedImage *subimage = gif->SavedImages + j;
      unsigned char *raster = (unsigned char *) subimage->RasterBits;
      int transparency_color_index = -1;
      int disposal = 0;
      int subimg_width = subimage->ImageDesc.Width;
      int subimg_height = subimage->ImageDesc.Height;
      int subimg_top = subimage->ImageDesc.Top;
      int subimg_left = subimage->ImageDesc.Left;

      /* Find the Graphic Control Extension block for this sub-image.
	 Extract the disposal method and transparency color.  */
      for (i = 0; i < subimage->ExtensionBlockCount; i++)
	{
	  ExtensionBlock *extblock = subimage->ExtensionBlocks + i;

	  if ((extblock->Function == GIF_LOCAL_DESCRIPTOR_EXTENSION)
	      && extblock->ByteCount == 4
	      && extblock->Bytes[0] & 1)
	    {
	      /* From gif89a spec: 1 = "keep in place", 2 = "restore
		 to background".  Treat any other value like 2.  */
	      disposal = (extblock->Bytes[0] >> 2) & 7;
	      transparency_color_index = (unsigned char) extblock->Bytes[3];
	      break;
	    }
	}

      /* We can't "keep in place" the first subimage.  */
      if (j == 0)
	disposal = 2;

      /* For disposal == 0, the spec says "No disposal specified. The
	 decoder is not required to take any action."  In practice, it
	 seems we need to treat this like "keep in place", see e.g.
	 http://upload.wikimedia.org/wikipedia/commons/3/37/Clock.gif */
      if (disposal == 0)
	disposal = 1;

      /* Allocate subimage colors.  */
      memset (pixel_colors, 0, sizeof pixel_colors);
      gif_color_map = subimage->ImageDesc.ColorMap;
      if (!gif_color_map)
	gif_color_map = gif->SColorMap;

      if (gif_color_map)
	for (i = 0; i < gif_color_map->ColorCount; ++i)
	  {
	    if (transparency_color_index == i)
	      pixel_colors[i] = STRINGP (specified_bg)
		? bgcolor : FRAME_BACKGROUND_PIXEL (f);
	    else
	      {
		int r = gif_color_map->Colors[i].Red << 8;
		int g = gif_color_map->Colors[i].Green << 8;
		int b = gif_color_map->Colors[i].Blue << 8;
		pixel_colors[i] = lookup_rgb_color (f, r, g, b);
	      }
	  }

      /* Apply the pixel values.  */
      if (GIFLIB_MAJOR < 5 && gif->SavedImages[j].ImageDesc.Interlace)
	{
	  int row, pass;

	  for (y = 0, row = interlace_start[0], pass = 0;
	       y < subimg_height;
	       y++, row += interlace_increment[pass])
	    {
	      while (subimg_height <= row)
		row = interlace_start[++pass];

	      for (x = 0; x < subimg_width; x++)
		{
		  int c = raster[y * subimg_width + x];
		  if (transparency_color_index != c || disposal != 1)
		    XPutPixel (ximg, x + subimg_left, row + subimg_top,
			       pixel_colors[c]);
		}
	    }
	}
      else
	{
	  for (y = 0; y < subimg_height; ++y)
	    for (x = 0; x < subimg_width; ++x)
	      {
		int c = raster[y * subimg_width + x];
		if (transparency_color_index != c || disposal != 1)
		  XPutPixel (ximg, x + subimg_left, y + subimg_top,
			     pixel_colors[c]);
	      }
	}
    }

#ifdef COLOR_TABLE_SUPPORT
  img->colors = colors_in_color_table (&img->ncolors);
  free_color_table ();
#endif /* COLOR_TABLE_SUPPORT */

  /* Save GIF image extension data for `image-metadata'.
     Format is (count IMAGES extension-data (FUNCTION "BYTES" ...)).  */
  img->lisp_data = Qnil;
  if (gif->SavedImages[idx].ExtensionBlockCount > 0)
    {
      int delay = 0;
      ExtensionBlock *ext = gif->SavedImages[idx].ExtensionBlocks;
      for (i = 0; i < gif->SavedImages[idx].ExtensionBlockCount; i++, ext++)
	/* Append (... FUNCTION "BYTES") */
	{
	  img->lisp_data
	    = Fcons (make_number (ext->Function),
		     Fcons (make_unibyte_string (ext->Bytes, ext->ByteCount),
			    img->lisp_data));
	  if (ext->Function == GIF_LOCAL_DESCRIPTOR_EXTENSION
	      && ext->ByteCount == 4)
	    {
	      delay = ext->Bytes[2] << CHAR_BIT;
	      delay |= ext->Bytes[1];
	    }
	}
      img->lisp_data = list2 (Qextension_data, img->lisp_data);
      if (delay)
	img->lisp_data
	  = Fcons (Qdelay,
		   Fcons (make_float (delay / 100.0),
			  img->lisp_data));
    }

  if (gif->ImageCount > 1)
    img->lisp_data = Fcons (Qcount,
			    Fcons (make_number (gif->ImageCount),
				   img->lisp_data));

  fn_DGifCloseFile (gif);

  /* Maybe fill in the background field while we have ximg handy. */
  if (NILP (image_spec_value (img->spec, QCbackground, NULL)))
    /* Casting avoids a GCC warning.  */
    IMAGE_BACKGROUND (img, f, (XImagePtr_or_DC)ximg);

  /* Put ximg into the image.  */
  image_put_x_image (f, img, ximg, 0);

  return 1;
}

#else  /* !HAVE_GIF */
# ifdef MAC_OS
static int gif_load(struct frame *f, struct image *img)
{
  Lisp_Object specified_file, file;
  Lisp_Object specified_data;
  OSErr err;
  Boolean graphic_p, movie_p, prefer_graphic_p;
  Handle dh = NULL;
  Movie movie = NULL;
  Lisp_Object image;
  Track track = NULL;
  Media media = NULL;
  long nsamples;
  Rect rect;
  Lisp_Object specified_bg;
  XColor color;
  RGBColor bg_color;
  int width, height;
  XImagePtr ximg;
  TimeScale time_scale;
  TimeValue time, duration;
  int ino;
  CGrafPtr old_port;
  GDHandle old_gdh;

  specified_file = image_spec_value(img->spec, QCfile, NULL);
  specified_data = image_spec_value(img->spec, QCdata, NULL);

  /* Animated gifs use QuickTime Movie Toolbox, so initialize it here: */
  EnterMovies();

  if (NILP (specified_data)) {
    /* Read from a file: */
    FSSpec fss;
    short refnum;

    err = find_image_fsspec (specified_file, &file, &fss);
    if (err != noErr) {
      if (err == fnfErr)
        image_error("Cannot find image file `%s'", specified_file, Qnil);
      else
        goto open_error;
    }

    err = CanQuickTimeOpenFile (&fss, kQTFileTypeGIF, 0,
                                &graphic_p, &movie_p, &prefer_graphic_p, 0);
    if (err != noErr)
      goto open_error;

    if (!graphic_p && !movie_p)
      goto open_error;
    if (prefer_graphic_p)
      return image_load_qt_1 (f, img, kQTFileTypeGIF, &fss, NULL);
    err = OpenMovieFile (&fss, &refnum, fsRdPerm);
    if (err != noErr)
      goto open_error;
    err = NewMovieFromFile (&movie, refnum, NULL, NULL, 0, NULL);
    CloseMovieFile (refnum);
    if (err != noErr) {
      image_error("Error reading `%s'", file, Qnil);
      return 0;
    }
  } else {
    /* Memory source! */
    Handle dref = NULL;
    long file_type_atom[3];

    err = PtrToHand(SDATA(specified_data), &dh, SBYTES(specified_data));
    if (err != noErr) {
      image_error ("Cannot allocate data handle for `%s'",
                   img->spec, Qnil);
      goto error;
    }

    file_type_atom[0] = EndianU32_NtoB (sizeof (long) * 3);
    file_type_atom[1] = EndianU32_NtoB (kDataRefExtensionMacOSFileType);
    file_type_atom[2] = EndianU32_NtoB (kQTFileTypeGIF);
    err = PtrToHand (&dh, &dref, sizeof (Handle));
    if (err == noErr)
    /* no file name */
      err = PtrAndHand ("\p", dref, 1);
    if (err == noErr)
      err = PtrAndHand (file_type_atom, dref, sizeof (long) * 3);
    if (err != noErr) {
      image_error("Cannot allocate handle data ref for `%s'", img->spec, Qnil);
      goto error;
    }
    err = CanQuickTimeOpenDataRef (dref, HandleDataHandlerSubType, &graphic_p,
                                   &movie_p, &prefer_graphic_p, 0);
    if (err != noErr)
      goto open_error;

    if (!graphic_p && !movie_p)
      goto open_error;
    if (prefer_graphic_p) {
      int success_p;

      DisposeHandle (dref);
      success_p = image_load_qt_1 (f, img, kQTFileTypeGIF, NULL, dh);
      DisposeHandle (dh);
      return success_p;
    }
    err = NewMovieFromDataRef (&movie, 0, NULL, dref,
                               HandleDataHandlerSubType);
    DisposeHandle (dref);
    if (err != noErr)
      goto open_error;
  }

  image = image_spec_value (img->spec, QCindex, NULL);
  ino = INTEGERP (image) ? XFASTINT (image) : 0;
  track = GetMovieIndTrack (movie, 1);
  media = GetTrackMedia (track);
  nsamples = GetMediaSampleCount (media);
  if (ino >= nsamples) {
    image_error ("Invalid image number `%s' in image `%s'",
                 image, img->spec);
    goto error;
  }
  time_scale = GetMediaTimeScale (media);

  specified_bg = image_spec_value (img->spec, QCbackground, NULL);
  if (!STRINGP (specified_bg) ||
      !mac_defined_color (f, SDATA (specified_bg), &color, 0))
  {
    color.pixel = FRAME_BACKGROUND_PIXEL (f);
    color.red = RED16_FROM_ULONG (color.pixel);
    color.green = GREEN16_FROM_ULONG (color.pixel);
    color.blue = BLUE16_FROM_ULONG (color.pixel);
  }
  GetMovieBox (movie, &rect);
  width = img->width = rect.right - rect.left;
  height = img->height = rect.bottom - rect.top;
  if (!x_create_x_image_and_pixmap (f, width, height, 0, &ximg, &img->pixmap))
    goto error;

  GetGWorld (&old_port, &old_gdh);
  SetGWorld (ximg, NULL);
  bg_color.red = color.red;
  bg_color.green = color.green;
  bg_color.blue = color.blue;
  RGBBackColor (&bg_color);
  SetGWorld (old_port, old_gdh);
  SetMovieActive (movie, 1);
  SetMovieGWorld (movie, ximg, NULL);
  SampleNumToMediaTime (media, ino + 1, &time, &duration);
  SetMovieTimeValue (movie, time);
  MoviesTask (movie, 0L);
  DisposeTrackMedia (media);
  DisposeMovieTrack (track);
  DisposeMovie (movie);
  if (dh) {
    DisposeHandle(dh);
  }

  /* Save GIF image extension data for `image-extension-data'.
   * Format is (count IMAGES 0xf9 GRAPHIC_CONTROL_EXTENSION_BLOCK).  */
  {
    Lisp_Object gce = make_uninit_string (4);
    int centisec = ((float)duration / time_scale) * 100.0f + 0.5f;

    /* Fill the delay time field: */
    SSET(gce, 1, centisec & 0xff);
    SSET(gce, 2, (centisec >> 8) & 0xff);
    /* We do NOT know about other fields: */
    SSET(gce, 0, 0);
    SSET(gce, 3, 0);

    img->data.lisp_val = list4 (Qcount, make_number (nsamples),
				make_number (0xf9), gce);
  }

  /* Maybe fill in the background field while we have ximg handy: */
  if (NILP (image_spec_value(img->spec, QCbackground, NULL)))
    IMAGE_BACKGROUND(img, f, ximg);

  /* Put the image into the pixmap: */
  x_put_x_image(f, ximg, img->pixmap, width, height);
  x_destroy_x_image(ximg);
  return 1;

open_error:
  image_error("Cannot open `%s'", file, Qnil);
error:
  if (media)
    DisposeTrackMedia(media);
  if (track)
    DisposeMovieTrack(track);
  if (movie)
    DisposeMovie(movie);
  if (dh)
    DisposeHandle(dh);
  return 0;
}
# else
#  ifdef HAVE_NS
static bool
gif_load (struct frame *f, struct image *img)
{
  return ns_load_image (f, img,
                        image_spec_value (img->spec, QCfile, NULL),
			image_spec_value (img->spec, QCdata, NULL));
}
#  endif /* HAVE_NS */
# endif /* MAC_OS */
#endif /* HAVE_GIF */


#ifdef HAVE_IMAGEMAGICK
/***********************************************************************
				 ImageMagick
***********************************************************************/

/* Scale an image size by returning SIZE / DIVISOR * MULTIPLIER,
   safely rounded and clipped to int range.  */
static int
scale_image_size (int size, size_t divisor, size_t multiplier)
{
  if (divisor != 0)
    {
      double s = size;
      double scaled = s * multiplier / divisor + 0.5;
      if (scaled < INT_MAX)
	return scaled;
    }
  return INT_MAX;
}

/* Compute the desired size of an image with native size WIDTH x HEIGHT.
   Use SPEC to deduce the size.  Store the desired size into
   *D_WIDTH x *D_HEIGHT.  Store -1 x -1 if the native size is OK.  */
static void
compute_image_size (size_t width, size_t height,
		    Lisp_Object spec,
		    int *d_width, int *d_height)
{
  Lisp_Object value;
  int desired_width, desired_height;

  /* If width and/or height is set in the display spec assume we want
     to scale to those values.  If either h or w is unspecified, the
     unspecified should be calculated from the specified to preserve
     aspect ratio.  */
  value = image_spec_value (spec, QCwidth, NULL);
  desired_width = NATNUMP (value) ? min (XFASTINT (value), INT_MAX) : -1;
  value = image_spec_value (spec, QCheight, NULL);
  desired_height = NATNUMP (value) ? min (XFASTINT (value), INT_MAX) : -1;

  if (desired_width == -1)
    {
      value = image_spec_value (spec, QCmax_width, NULL);
      if (NATNUMP (value))
	{
	  int max_width = min (XFASTINT (value), INT_MAX);
	  if (max_width < width)
	    {
	      /* The image is wider than :max-width. */
	      desired_width = max_width;
	      if (desired_height == -1)
		{
		  desired_height = scale_image_size (desired_width,
						     width, height);
		  value = image_spec_value (spec, QCmax_height, NULL);
		  if (NATNUMP (value))
		    {
		      int max_height = min (XFASTINT (value), INT_MAX);
		      if (max_height < desired_height)
			{
			  desired_height = max_height;
			  desired_width = scale_image_size (desired_height,
							    height, width);
			}
		    }
		}
	    }
	}
    }

  if (desired_height == -1)
    {
      value = image_spec_value (spec, QCmax_height, NULL);
      if (NATNUMP (value))
	{
	  int max_height = min (XFASTINT (value), INT_MAX);
	  if (max_height < height)
	    desired_height = max_height;
	}
    }

  if (desired_width != -1 && desired_height == -1)
    /* w known, calculate h.  */
    desired_height = scale_image_size (desired_width, width, height);

  if (desired_width == -1 && desired_height != -1)
    /* h known, calculate w.  */
    desired_width = scale_image_size (desired_height, height, width);

  *d_width = desired_width;
  *d_height = desired_height;
}

static Lisp_Object Qimagemagick;

static bool imagemagick_image_p (Lisp_Object);
static bool imagemagick_load (struct frame *, struct image *);
static void imagemagick_clear_image (struct frame *, struct image *);

/* Indices of image specification fields in imagemagick_format: */
enum imagemagick_keyword_index
  {
    IMAGEMAGICK_TYPE,
    IMAGEMAGICK_DATA,
    IMAGEMAGICK_FILE,
    IMAGEMAGICK_ASCENT,
    IMAGEMAGICK_MARGIN,
    IMAGEMAGICK_RELIEF,
    IMAGEMAGICK_ALGORITHM,
    IMAGEMAGICK_HEURISTIC_MASK,
    IMAGEMAGICK_MASK,
    IMAGEMAGICK_BACKGROUND,
    IMAGEMAGICK_HEIGHT,
    IMAGEMAGICK_WIDTH,
    IMAGEMAGICK_MAX_HEIGHT,
    IMAGEMAGICK_MAX_WIDTH,
    IMAGEMAGICK_FORMAT,
    IMAGEMAGICK_ROTATION,
    IMAGEMAGICK_CROP,
    IMAGEMAGICK_LAST
  };

/* Vector of image_keyword structures describing the format
   of valid user-defined image specifications.  */
static struct image_keyword imagemagick_format[IMAGEMAGICK_LAST] =
  {
    {":type",		IMAGE_SYMBOL_VALUE,			1},
    {":data",		IMAGE_STRING_VALUE,			0},
    {":file",		IMAGE_STRING_VALUE,			0},
    {":ascent",		IMAGE_ASCENT_VALUE,			0},
    {":margin",		IMAGE_NON_NEGATIVE_INTEGER_VALUE_OR_PAIR, 0},
    {":relief",		IMAGE_INTEGER_VALUE,			0},
    {":conversion",	IMAGE_DONT_CHECK_VALUE_TYPE,		0},
    {":heuristic-mask",	IMAGE_DONT_CHECK_VALUE_TYPE,		0},
    {":mask",		IMAGE_DONT_CHECK_VALUE_TYPE,		0},
    {":background",	IMAGE_STRING_OR_NIL_VALUE,		0},
    {":height",		IMAGE_INTEGER_VALUE,			0},
    {":width",		IMAGE_INTEGER_VALUE,			0},
    {":max-height",	IMAGE_INTEGER_VALUE,			0},
    {":max-width",	IMAGE_INTEGER_VALUE,			0},
    {":format",		IMAGE_SYMBOL_VALUE,			0},
    {":rotation",	IMAGE_NUMBER_VALUE,     		0},
    {":crop",		IMAGE_DONT_CHECK_VALUE_TYPE,		0}
  };

#if defined HAVE_NTGUI && defined WINDOWSNT
static bool init_imagemagick_functions (void);
#else
# define init_imagemagick_functions NULL
#endif /* HAVE_NTGUI && WINDOWSNT */

/* Structure describing the image type for any image handled via
   ImageMagick.  */
static struct image_type imagemagick_type =
  {
    &Qimagemagick,
    imagemagick_image_p,
    imagemagick_load,
    imagemagick_clear_image,
    init_imagemagick_functions,
    NULL
  };

/* Free X resources of imagemagick image IMG which is used on frame F: */
static void
imagemagick_clear_image (struct frame *f,
                         struct image *img)
{
  x_clear_image (f, img);
}

/* Return true if OBJECT is a valid IMAGEMAGICK image specification.  Do
   this by calling parse_image_spec and supplying the keywords that
   identify the IMAGEMAGICK format.   */
static bool imagemagick_image_p(Lisp_Object object)
{
  struct image_keyword fmt[IMAGEMAGICK_LAST];
  memcpy (fmt, imagemagick_format, sizeof fmt);

  if (!parse_image_spec (object, fmt, IMAGEMAGICK_LAST, Qimagemagick))
    return 0;

  /* Must specify either the :data or :file keyword.  */
  return fmt[IMAGEMAGICK_FILE].count + fmt[IMAGEMAGICK_DATA].count == 1;
}

/* The GIF library also defines DrawRectangle, but Emacs never uses it.
   Therefore rename the function so it avoids collision in ImageMagick: */
#define DrawRectangle DrawRectangleGif
#include <wand/MagickWand.h>

/* ImageMagick 6.5.3 through 6.6.5 hid PixelGetMagickColor for some reason.
   Emacs seems to work fine with the hidden version, so unhide it.  */
#include <magick/version.h>
#if 0x653 <= MagickLibVersion && MagickLibVersion <= 0x665
extern WandExport void PixelGetMagickColor (const PixelWand *,
					    MagickPixelPacket *);
#endif

/* Log ImageMagick error message.
   Useful when a ImageMagick function returns the status `MagickFalse'.  */
static void
imagemagick_error (MagickWand *wand)
{
  char *description;
  ExceptionType severity;

  description = MagickGetException (wand, &severity);
  image_error ("ImageMagick error: %s",
	       build_string (description),
	       Qnil);
  MagickRelinquishMemory (description);
}

/* Possibly give ImageMagick some extra help to determine the image
   type by supplying a "dummy" filename based on the Content-Type.  */
static char *
imagemagick_filename_hint (Lisp_Object spec, char hint_buffer[MaxTextExtent])
{
  Lisp_Object symbol = intern ("image-format-suffixes");
  Lisp_Object val = find_symbol_value (symbol);
  Lisp_Object format;

  if (! CONSP (val))
    return NULL;

  format = image_spec_value (spec, intern (":format"), NULL);
  val = Fcar_safe (Fcdr_safe (Fassq (format, val)));
  if (! STRINGP (val))
    return NULL;

  /* It is OK to truncate the hint if it has MaxTextExtent or more bytes,
     as ImageMagick would ignore the extra bytes anyway.  */
  snprintf (hint_buffer, MaxTextExtent, "/tmp/foo.%s", SSDATA (val));
  return hint_buffer;
}

/* Animated images (e.g., GIF89a) are composed from one "master image"
   (which is the first one, and then there's a number of images that
   follow.  If following images have non-transparent colors, these are
   composed "on top" of the master image.  So, in general, one has to
   compute ann the preceding images to be able to display a particular
   sub-image.

   Computing all the preceding images is too slow, so we maintain a
   cache of previously computed images.  We have to maintain a cache
   separate from the image cache, because the images may be scaled
   before display. */
struct animation_cache
{
  MagickWand *wand;
  int index;
  struct timespec update_time;
  struct animation_cache *next;
  char signature[FLEXIBLE_ARRAY_MEMBER];
};

static struct animation_cache *animation_cache = NULL;

static struct animation_cache *
imagemagick_create_cache (char *signature)
{
  struct animation_cache *cache
    = xmalloc (offsetof (struct animation_cache, signature)
	       + strlen (signature) + 1);
  cache->wand = 0;
  cache->index = 0;
  cache->next = 0;
  strcpy (cache->signature, signature);
  return cache;
}

/* Discard cached images that have NOT been used for a minute: */
static void
imagemagick_prune_animation_cache (void)
{
  struct animation_cache **pcache = &animation_cache;
  struct timespec old = timespec_sub (current_timespec (),
				      make_timespec (60, 0));

  while (*pcache)
    {
      struct animation_cache *cache = *pcache;
      if (timespec_cmp (old, cache->update_time) <= 0)
	pcache = &cache->next;
      else
	{
	  if (cache->wand)
	    DestroyMagickWand (cache->wand);
	  *pcache = cache->next;
	  xfree (cache);
	}
    }
}

static struct animation_cache *
imagemagick_get_animation_cache (MagickWand *wand)
{
  char *signature = MagickGetImageSignature (wand);
  struct animation_cache *cache;
  struct animation_cache **pcache = &animation_cache;

  imagemagick_prune_animation_cache ();

  while (1)
    {
      cache = *pcache;
      if (! cache)
	{
          *pcache = cache = imagemagick_create_cache (signature);
          break;
        }
      if (strcmp (signature, cache->signature) == 0)
	break;
      pcache = &cache->next;
    }

  DestroyString (signature);
  cache->update_time = current_timespec ();
  return cache;
}

static MagickWand *
imagemagick_compute_animated_image (MagickWand *super_wand, int ino)
{
  int i;
  MagickWand *composite_wand;
  size_t dest_width, dest_height;
  struct animation_cache *cache = imagemagick_get_animation_cache (super_wand);

  MagickSetIteratorIndex (super_wand, 0);

  if (ino == 0 || cache->wand == NULL || cache->index > ino)
    {
      composite_wand = MagickGetImage (super_wand);
      if (cache->wand)
	DestroyMagickWand (cache->wand);
    }
  else
    composite_wand = cache->wand;

  dest_width = MagickGetImageWidth (composite_wand);
  dest_height = MagickGetImageHeight (composite_wand);

  for (i = max (1, cache->index + 1); i <= ino; i++)
    {
      MagickWand *sub_wand;
      PixelIterator *source_iterator, *dest_iterator;
      PixelWand **source, **dest;
      size_t source_width, source_height;
      ssize_t source_left, source_top;
      MagickPixelPacket pixel;
      DisposeType dispose;
      ptrdiff_t lines = 0;

      MagickSetIteratorIndex (super_wand, i);
      sub_wand = MagickGetImage (super_wand);

      MagickGetImagePage (sub_wand, &source_width, &source_height,
			  &source_left, &source_top);

      /* This flag says how to handle transparent pixels.  */
      dispose = MagickGetImageDispose (sub_wand);

      source_iterator = NewPixelIterator (sub_wand);
      if (! source_iterator)
	{
	  DestroyMagickWand (composite_wand);
	  DestroyMagickWand (sub_wand);
	  cache->wand = NULL;
	  image_error ("Imagemagick pixel iterator creation failed",
		       Qnil, Qnil);
	  return NULL;
	}

      dest_iterator = NewPixelIterator (composite_wand);
      if (! dest_iterator)
	{
	  DestroyMagickWand (composite_wand);
	  DestroyMagickWand (sub_wand);
	  DestroyPixelIterator (source_iterator);
	  cache->wand = NULL;
	  image_error ("Imagemagick pixel iterator creation failed",
		       Qnil, Qnil);
	  return NULL;
	}

      /* The sub-image may not start at origin, so move the destination
	 iterator to where the sub-image should start. */
      if (source_top > 0)
	{
	  PixelSetIteratorRow (dest_iterator, source_top);
	  lines = source_top;
	}

      while ((source = PixelGetNextIteratorRow (source_iterator, &source_width))
	     != NULL)
	{
	  ptrdiff_t x;

	  /* Sanity check.  This shouldn't happen, but apparently
	     does in some pictures.  */
	  if (++lines >= dest_height)
	    break;

	  dest = PixelGetNextIteratorRow (dest_iterator, &dest_width);
	  for (x = 0; x < source_width; x++)
	    {
	      /* Sanity check.  This shouldn't happen, but apparently
		 also does in some pictures.  */
	      if (x + source_left > dest_width - 1)
		break;
	      /* Normally we only copy over non-transparent pixels,
		 but if the disposal method is "Background", then we
		 copy over all pixels.  */
	      if (dispose == BackgroundDispose ||
		  PixelGetAlpha (source[x]))
		{
		  PixelGetMagickColor (source[x], &pixel);
		  PixelSetMagickColor (dest[x + source_left], &pixel);
		}
	    }
	  PixelSyncIterator (dest_iterator);
	}

      DestroyPixelIterator (source_iterator);
      DestroyPixelIterator (dest_iterator);
      DestroyMagickWand (sub_wand);
    }

  /* Cache a copy for the next iteration.  The current wand will be
     destroyed by the caller. */
  cache->wand = CloneMagickWand (composite_wand);
  cache->index = ino;

  return composite_wand;
}


/* Helper function for imagemagick_load, which does the actual loading
   given contents and size, apart from frame and image structures,
   passed from imagemagick_load.  Uses librimagemagick to do most of
   the image processing.

   F is a pointer to the Emacs frame; IMG to the image structure to
   prepare; CONTENTS is the string containing the IMAGEMAGICK data to
   be parsed; SIZE is the number of bytes of data; and FILENAME is
   either the file name or the image data.

   Return true if successful.  */
static bool
imagemagick_load_image (struct frame *f, struct image *img,
			unsigned char *contents, unsigned int size,
			char *filename)
{
  size_t width, height;
  MagickBooleanType status;
  XImagePtr ximg;
  int x, y;
  MagickWand *image_wand;
  PixelIterator *iterator;
  PixelWand **pixels, *bg_wand = NULL;
  MagickPixelPacket  pixel;
  Lisp_Object image;
  Lisp_Object value;
  Lisp_Object crop;
  EMACS_INT ino;
  int desired_width, desired_height;
  double rotation;
#if defined(HAVE_MAGICKEXPORTIMAGEPIXELS) && !defined(HAVE_NS)
  int pixelwidth = 0; /* (same condition in which it is used) */
#endif /* HAVE_MAGICKEXPORTIMAGEPIXELS && !HAVE_NS */
  char hint_buffer[MaxTextExtent];
  char *filename_hint = NULL;

  /* Handle image index for image types who can contain more than one image.
     Interface :index is same as for GIF.  First we "ping" the image to see how
     many sub-images it contains.  Pinging is faster than loading the image to
     find out things about it.  */

  /* Initialize the imagemagick environment.  */
  MagickWandGenesis ();
  image = image_spec_value (img->spec, QCindex, NULL);
  ino = INTEGERP (image) ? XFASTINT (image) : 0;
  image_wand = NewMagickWand ();

  if (filename)
    status = MagickReadImage (image_wand, filename);
  else
    {
      filename_hint = imagemagick_filename_hint (img->spec, hint_buffer);
      MagickSetFilename (image_wand, filename_hint);
      status = MagickReadImageBlob (image_wand, contents, size);
    }

  if (status == MagickFalse)
    {
      imagemagick_error (image_wand);
      DestroyMagickWand (image_wand);
      return 0;
    }

  if (ino < 0 || ino >= MagickGetNumberImages (image_wand))
    {
      image_error ("Invalid image number `%s' in image `%s'",
		   image, img->spec);
      DestroyMagickWand (image_wand);
      return 0;
    }

  if (MagickGetNumberImages (image_wand) > 1)
    img->lisp_data =
      Fcons (Qcount,
             Fcons (make_number (MagickGetNumberImages (image_wand)),
                    img->lisp_data));

  /* If we have an animated image, get the new wand based on the
     "super-wand". */
  if (MagickGetNumberImages (image_wand) > 1)
    {
      MagickWand *super_wand = image_wand;
      image_wand = imagemagick_compute_animated_image (super_wand, ino);
      if (! image_wand)
	image_wand = super_wand;
      else
	DestroyMagickWand (super_wand);
    }

  /* Retrieve the frame's background color, for use later.  */
  {
    XColor bgcolor;
    Lisp_Object specified_bg;

    specified_bg = image_spec_value (img->spec, QCbackground, NULL);
    if (!STRINGP (specified_bg)
	|| !x_defined_color (f, SSDATA (specified_bg), &bgcolor, 0))
      {
#ifndef HAVE_NS
	bgcolor.pixel = FRAME_BACKGROUND_PIXEL (f);
	x_query_color (f, &bgcolor);
#else
	ns_query_color (FRAME_BACKGROUND_COLOR (f), &bgcolor, 1);
#endif /* !HAVE_NS */
      }

    bg_wand = NewPixelWand ();
    PixelSetRed   (bg_wand, (double) bgcolor.red   / 65535);
    PixelSetGreen (bg_wand, (double) bgcolor.green / 65535);
    PixelSetBlue  (bg_wand, (double) bgcolor.blue  / 65535);
  }

  compute_image_size (MagickGetImageWidth (image_wand),
		      MagickGetImageHeight (image_wand),
		      img->spec, &desired_width, &desired_height);

  if (desired_width != -1 && desired_height != -1)
    {
      status = MagickScaleImage (image_wand, desired_width, desired_height);
      if (status == MagickFalse)
	{
	  image_error ("Imagemagick scale failed", Qnil, Qnil);
	  imagemagick_error (image_wand);
	  goto imagemagick_error;
	}
    }

  /* crop behaves similar to image slicing in Emacs but is more memory
     efficient.  */
  crop = image_spec_value (img->spec, QCcrop, NULL);

  if (CONSP (crop) && TYPE_RANGED_INTEGERP (size_t, XCAR (crop)))
    {
      /* After some testing, it seems MagickCropImage is the fastest crop
         function in ImageMagick.  This crop function seems to do less copying
         than the alternatives, but it still reads the entire image into memory
         before cropping, which is apparently difficult to avoid when using
         imagemagick.  */
      size_t crop_width = XINT (XCAR (crop));
      crop = XCDR (crop);
      if (CONSP (crop) && TYPE_RANGED_INTEGERP (size_t, XCAR (crop)))
	{
	  size_t crop_height = XINT (XCAR (crop));
	  crop = XCDR (crop);
	  if (CONSP (crop) && TYPE_RANGED_INTEGERP (ssize_t, XCAR (crop)))
	    {
	      ssize_t crop_x = XINT (XCAR (crop));
	      crop = XCDR (crop);
	      if (CONSP (crop) && TYPE_RANGED_INTEGERP (ssize_t, XCAR (crop)))
		{
		  ssize_t crop_y = XINT (XCAR (crop));
		  MagickCropImage (image_wand, crop_width, crop_height,
				   crop_x, crop_y);
		}
	    }
	}
    }

  /* Furthermore :rotation. we need background color and angle for
     rotation.  */
  /*
    TODO background handling for rotation specified_bg =
    image_spec_value (img->spec, QCbackground, NULL); if (!STRINGP
    (specified_bg).  */
  value = image_spec_value (img->spec, QCrotation, NULL);
  if (FLOATP (value))
    {
      rotation = extract_float (value);
      status = MagickRotateImage (image_wand, bg_wand, rotation);
      if (status == MagickFalse)
        {
          image_error ("Imagemagick image rotate failed", Qnil, Qnil);
	  imagemagick_error (image_wand);
          goto imagemagick_error;
        }
    }

  /* Set the canvas background color to the frame or specified
     background, and flatten the image.  Note: as of ImageMagick
     6.6.0, SVG image transparency is not handled properly
     (e.g. etc/images/splash.svg shows a white background always).  */
  {
    MagickWand *new_wand;
    MagickSetImageBackgroundColor (image_wand, bg_wand);
#ifdef HAVE_MAGICKMERGEIMAGELAYERS
    new_wand = MagickMergeImageLayers (image_wand, MergeLayer);
#else
    new_wand = MagickFlattenImages (image_wand);
#endif /* HAVE_MAGICKMERGEIMAGELAYERS */
    DestroyMagickWand (image_wand);
    image_wand = new_wand;
  }

  /* Finally we are done manipulating the image.  Figure out the
     resulting width/height and transfer ownership to Emacs.  */
  height = MagickGetImageHeight (image_wand);
  width = MagickGetImageWidth (image_wand);

  if (! (width <= INT_MAX && height <= INT_MAX
	 && check_image_size (f, width, height)))
    {
      image_error ("Invalid image size (see `max-image-size')", Qnil, Qnil);
      goto imagemagick_error;
    }

  /* We can now get a valid pixel buffer from the imagemagick file, if all
     went ok.  */

  init_color_table ();

#if defined(HAVE_MAGICKEXPORTIMAGEPIXELS) && !defined(HAVE_NS)
  if (imagemagick_render_type != 0)
    {
      /* Magicexportimage is normally faster than pixelpushing.  This
         method is also well tested.  Some aspects of this method are
         ad-hoc and needs to be more researched. */
      int imagedepth = 24; /*MagickGetImageDepth(image_wand);*/
      const char *exportdepth = imagedepth <= 8 ? "I" : "BGRP"; /*"RGBP";*/
      /* Try to create a x pixmap to hold the imagemagick pixmap.  */
      if (!image_create_x_image_and_pixmap (f, img, width, height, imagedepth,
					    &ximg, 0))
	{
# ifdef COLOR_TABLE_SUPPORT
	  free_color_table ();
# endif /* COLOR_TABLE_SUPPORT */
	  image_error ("Imagemagick X bitmap allocation failure", Qnil, Qnil);
	  goto imagemagick_error;
	}

      /* Oddly, the below code does NOT seem to work: */
# ifdef TRY_TO_BUILD_BROKEN_CODE
      switch (ximg->bitmap_unit)
        {
          case 8:
            pixelwidth = CharPixel;
            break;
          case 16:
            pixelwidth = ShortPixel;
            break;
          case 32:
            pixelwidth = LongPixel;
            break;
          default:
            break;
        }
# endif /* TRY_TO_BUILD_BROKEN_CODE */
      /*
        Here im just guessing the format of the bitmap.
        happens to work fine for:
        - bw djvu images
        on rgb display.
        seems about 3 times as fast as pixel pushing(not carefully measured)
      */
      pixelwidth = CharPixel; /* ??? TODO: figure out */
      MagickExportImagePixels (image_wand, 0, 0, width, height,
			       exportdepth, pixelwidth, ximg->data);
    }
  else
#endif /* HAVE_MAGICKEXPORTIMAGEPIXELS && !HAVE_NS */
    {
      size_t image_height;

      /* Try to create a x pixmap to hold the imagemagick pixmap.  */
      if (!image_create_x_image_and_pixmap (f, img, width, height, 0,
					    &ximg, 0))
        {
#ifdef COLOR_TABLE_SUPPORT
	  free_color_table ();
#endif /* COLOR_TABLE_SUPPORT */
          image_error ("Imagemagick X bitmap allocation failure", Qnil, Qnil);
          goto imagemagick_error;
        }

      /* Copy imagemagick image to x with primitive yet robust pixel
         pusher loop.  This has been tested a lot with many different
         images.  */

      /* Copy pixels from the imagemagick image structure to the x image map. */
      iterator = NewPixelIterator (image_wand);
      if (! iterator)
        {
#ifdef COLOR_TABLE_SUPPORT
	  free_color_table ();
#endif /* COLOR_TABLE_SUPPORT */
	  x_destroy_x_image (ximg);
          image_error ("Imagemagick pixel iterator creation failed",
                       Qnil, Qnil);
          goto imagemagick_error;
        }

      image_height = MagickGetImageHeight (image_wand);
      for (y = 0; y < image_height; y++)
        {
          pixels = PixelGetNextIteratorRow (iterator, &width);
          if (! pixels)
            break;
          for (x = 0; x < (long) width; x++)
            {
              PixelGetMagickColor (pixels[x], &pixel);
              XPutPixel (ximg, x, y,
                         lookup_rgb_color (f,
                                           pixel.red,
                                           pixel.green,
                                           pixel.blue));
            }
        }
      DestroyPixelIterator (iterator);
    }

#ifdef COLOR_TABLE_SUPPORT
  /* Remember colors allocated for this image.  */
  img->colors = colors_in_color_table (&img->ncolors);
  free_color_table ();
#endif /* COLOR_TABLE_SUPPORT */

  img->width  = width;
  img->height = height;

  /* Put ximg into the image.  */
  image_put_x_image (f, img, ximg, 0);

  /* Final cleanup. image_wand should be the only resource left. */
  DestroyMagickWand (image_wand);
  if (bg_wand) DestroyPixelWand (bg_wand);

  /* `MagickWandTerminus' terminates the imagemagick environment.  */
  MagickWandTerminus ();

  return 1;

 imagemagick_error:
  DestroyMagickWand (image_wand);
  if (bg_wand) DestroyPixelWand (bg_wand);

  MagickWandTerminus ();
  /* TODO: more cleanup.  */
  image_error ("Error parsing IMAGEMAGICK image `%s'", img->spec, Qnil);
  return 0;
}


/* Load IMAGEMAGICK image IMG for use on frame F.  Value is true if
   successful. this function will go into the imagemagick_type structure, and
   the prototype thus needs to be compatible with that structure.  */
static bool imagemagick_load (struct frame *f, struct image *img)
{
  bool success_p = 0;
  Lisp_Object file_name;

  /* If IMG->spec specifies a file name, create a non-file spec from it.  */
  file_name = image_spec_value (img->spec, QCfile, NULL);
  if (STRINGP (file_name))
    {
      Lisp_Object file;

      file = x_find_image_file (file_name);
      if (!STRINGP (file))
	{
	  image_error ("Cannot find image file `%s'", file_name, Qnil);
	  return 0;
	}
#ifdef WINDOWSNT
      file = ansi_encode_filename (file);
#endif /* WINDOWSNT */
      success_p = imagemagick_load_image (f, img, 0, 0, SSDATA (file));
    }
  /* Else its not a file, its a lisp object.  Load the image from a
     lisp object rather than a file.  */
  else
    {
      Lisp_Object data;

      data = image_spec_value (img->spec, QCdata, NULL);
      if (!STRINGP (data))
	{
	  image_error ("Invalid image data `%s'", data, Qnil);
	  return 0;
	}
      success_p = imagemagick_load_image (f, img, SDATA (data),
                                          SBYTES (data), NULL);
    }

  return success_p;
}

DEFUN ("imagemagick-types", Fimagemagick_types, Simagemagick_types, 0, 0, 0,
       doc: /* Return a list of image types supported by ImageMagick.
Each entry in this list is a symbol named after an ImageMagick format
tag.  See the ImageMagick manual for a list of ImageMagick formats and
their descriptions (http://www.imagemagick.org/script/formats.php).
You can also try the shell command: `identify -list format'.

Note that ImageMagick recognizes many file-types that Emacs does not
recognize as images, such as C.  See `imagemagick-types-enable'
and `imagemagick-types-inhibit'.  */)
  (void)
{
  Lisp_Object typelist = Qnil;
  size_t numf = 0;
  ExceptionInfo ex;
  char **imtypes;
  size_t i;
  Lisp_Object Qimagemagicktype;

  GetExceptionInfo(&ex);
  imtypes = GetMagickList ("*", &numf, &ex);
  DestroyExceptionInfo(&ex);

  for (i = 0; i < numf; i++)
    {
      Qimagemagicktype = intern (imtypes[i]);
      typelist = Fcons (Qimagemagicktype, typelist);
      imtypes[i] = MagickRelinquishMemory (imtypes[i]);
    }

  MagickRelinquishMemory (imtypes);
  return Fnreverse (typelist);
}

#endif	/* defined (HAVE_IMAGEMAGICK) */



/***********************************************************************
				 SVG
 ***********************************************************************/

#if defined (HAVE_RSVG)

/* Function prototypes: */
static bool svg_image_p (Lisp_Object object);
static bool svg_load (struct frame *f, struct image *img);

static bool svg_load_image (struct frame *, struct image *,
			    unsigned char *, ptrdiff_t);

/* The symbol `svg' identifying images of this type: */
static Lisp_Object Qsvg;

/* Indices of image specification fields in svg_format, below: */
enum svg_keyword_index
{
  SVG_TYPE,
  SVG_DATA,
  SVG_FILE,
  SVG_ASCENT,
  SVG_MARGIN,
  SVG_RELIEF,
  SVG_ALGORITHM,
  SVG_HEURISTIC_MASK,
  SVG_MASK,
  SVG_BACKGROUND,
  SVG_LAST
};

/* Vector of image_keyword structures describing the format
   of valid user-defined image specifications.  */
static const struct image_keyword svg_format[SVG_LAST] =
{
  {":type",		IMAGE_SYMBOL_VALUE,			1},
  {":data",		IMAGE_STRING_VALUE,			0},
  {":file",		IMAGE_STRING_VALUE,			0},
  {":ascent",		IMAGE_ASCENT_VALUE,			0},
  {":margin",		IMAGE_NON_NEGATIVE_INTEGER_VALUE_OR_PAIR, 0},
  {":relief",		IMAGE_INTEGER_VALUE,			0},
  {":conversion",	IMAGE_DONT_CHECK_VALUE_TYPE,		0},
  {":heuristic-mask",	IMAGE_DONT_CHECK_VALUE_TYPE,		0},
  {":mask",		IMAGE_DONT_CHECK_VALUE_TYPE,		0},
  {":background",	IMAGE_STRING_OR_NIL_VALUE,		0}
};

#if defined HAVE_NTGUI && defined WINDOWSNT
static bool init_svg_functions (void);
#else
# define init_svg_functions NULL
#endif /* HAVE_NTGUI && WINDOWSNT */

/* Structure describing the image type `svg'.  Its the same type of
   structure defined for all image formats, handled by emacs image
   functions.  See struct image_type in dispextern.h.  */
static struct image_type svg_type =
{
  &Qsvg,
  svg_image_p,
  svg_load,
  x_clear_image,
  init_svg_functions,
  NULL
};


/* Return true if OBJECT is a valid SVG image specification.  Do
   this by calling parse_image_spec and supplying the keywords that
   identify the SVG format.   */
static bool svg_image_p (Lisp_Object object)
{
  struct image_keyword fmt[SVG_LAST];
  memcpy (fmt, svg_format, sizeof fmt);

  if (!parse_image_spec (object, fmt, SVG_LAST, Qsvg))
    return 0;

  /* Must specify either the :data or :file keyword.  */
  return fmt[SVG_FILE].count + fmt[SVG_DATA].count == 1;
}

#include <librsvg/rsvg.h>
/* the above is the last header to be included in this file, so we should
 * no longer need this: */
#if defined(XLIB_ILLEGAL_ACCESS) && defined(lint)
# undef XLIB_ILLEGAL_ACCESS
#endif /* XLIB_ILLEGAL_ACCESS && lint */

#ifdef WINDOWSNT

/* SVG library functions.  */
DEF_IMGLIB_FN (RsvgHandle *, rsvg_handle_new, (void));
DEF_IMGLIB_FN (void, rsvg_handle_get_dimensions, (RsvgHandle *, RsvgDimensionData *));
DEF_IMGLIB_FN (gboolean, rsvg_handle_write, (RsvgHandle *, const guchar *, gsize, GError **));
DEF_IMGLIB_FN (gboolean, rsvg_handle_close, (RsvgHandle *, GError **));
DEF_IMGLIB_FN (GdkPixbuf *, rsvg_handle_get_pixbuf, (RsvgHandle *));
DEF_IMGLIB_FN (void *, rsvg_handle_set_size_callback, (RsvgHandle *, RsvgSizeFunc, gpointer, GDestroyNotify));

DEF_IMGLIB_FN (int, gdk_pixbuf_get_width, (const GdkPixbuf *));
DEF_IMGLIB_FN (int, gdk_pixbuf_get_height, (const GdkPixbuf *));
DEF_IMGLIB_FN (guchar *, gdk_pixbuf_get_pixels, (const GdkPixbuf *));
DEF_IMGLIB_FN (int, gdk_pixbuf_get_rowstride, (const GdkPixbuf *));
DEF_IMGLIB_FN (GdkColorspace, gdk_pixbuf_get_colorspace, (const GdkPixbuf *));
DEF_IMGLIB_FN (int, gdk_pixbuf_get_n_channels, (const GdkPixbuf *));
DEF_IMGLIB_FN (gboolean, gdk_pixbuf_get_has_alpha, (const GdkPixbuf *));
DEF_IMGLIB_FN (int, gdk_pixbuf_get_bits_per_sample, (const GdkPixbuf *));

#if ! GLIB_CHECK_VERSION (2, 36, 0)
DEF_IMGLIB_FN (void, g_type_init, (void));
#endif
DEF_IMGLIB_FN (void, g_object_unref, (gpointer));
DEF_IMGLIB_FN (void, g_error_free, (GError *));

Lisp_Object Qgdk_pixbuf, Qglib, Qgobject;

static bool
init_svg_functions (void)
{
  HMODULE library, gdklib, glib, gobject;

  if (!(glib = w32_delayed_load (Qglib))
      || !(gobject = w32_delayed_load (Qgobject))
      || !(gdklib = w32_delayed_load (Qgdk_pixbuf))
      || !(library = w32_delayed_load (Qsvg)))
    return 0;

  LOAD_IMGLIB_FN (library, rsvg_handle_new);
  LOAD_IMGLIB_FN (library, rsvg_handle_get_dimensions);
  LOAD_IMGLIB_FN (library, rsvg_handle_write);
  LOAD_IMGLIB_FN (library, rsvg_handle_close);
  LOAD_IMGLIB_FN (library, rsvg_handle_get_pixbuf);

  LOAD_IMGLIB_FN (gdklib, gdk_pixbuf_get_width);
  LOAD_IMGLIB_FN (gdklib, gdk_pixbuf_get_height);
  LOAD_IMGLIB_FN (gdklib, gdk_pixbuf_get_pixels);
  LOAD_IMGLIB_FN (gdklib, gdk_pixbuf_get_rowstride);
  LOAD_IMGLIB_FN (gdklib, gdk_pixbuf_get_colorspace);
  LOAD_IMGLIB_FN (gdklib, gdk_pixbuf_get_n_channels);
  LOAD_IMGLIB_FN (gdklib, gdk_pixbuf_get_has_alpha);
  LOAD_IMGLIB_FN (gdklib, gdk_pixbuf_get_bits_per_sample);

#if ! GLIB_CHECK_VERSION (2, 36, 0)
  LOAD_IMGLIB_FN (gobject, g_type_init);
#endif
  LOAD_IMGLIB_FN (gobject, g_object_unref);
  LOAD_IMGLIB_FN (glib, g_error_free);

  return 1;
}

#else
/* The following aliases for library functions allow dynamic loading
   to be used on some platforms.  */
#define fn_rsvg_handle_new		rsvg_handle_new
#define fn_rsvg_handle_get_dimensions   rsvg_handle_get_dimensions
#define fn_rsvg_handle_write		rsvg_handle_write
#define fn_rsvg_handle_close		rsvg_handle_close
#define fn_rsvg_handle_get_pixbuf	rsvg_handle_get_pixbuf

#define fn_gdk_pixbuf_get_width		  gdk_pixbuf_get_width
#define fn_gdk_pixbuf_get_height	  gdk_pixbuf_get_height
#define fn_gdk_pixbuf_get_pixels	  gdk_pixbuf_get_pixels
#define fn_gdk_pixbuf_get_rowstride	  gdk_pixbuf_get_rowstride
#define fn_gdk_pixbuf_get_colorspace	  gdk_pixbuf_get_colorspace
#define fn_gdk_pixbuf_get_n_channels	  gdk_pixbuf_get_n_channels
#define fn_gdk_pixbuf_get_has_alpha	  gdk_pixbuf_get_has_alpha
#define fn_gdk_pixbuf_get_bits_per_sample gdk_pixbuf_get_bits_per_sample

#if ! GLIB_CHECK_VERSION (2, 36, 0)
# define fn_g_type_init                    g_type_init
#endif
#define fn_g_object_unref                 g_object_unref
#define fn_g_error_free                   g_error_free
#endif /* !WINDOWSNT  */

/* Load SVG image IMG for use on frame F.  Value is true if
   successful.  */

static bool
svg_load (struct frame *f, struct image *img)
{
  bool success_p = 0;
  Lisp_Object file_name;

  /* If IMG->spec specifies a file name, create a non-file spec from it.  */
  file_name = image_spec_value (img->spec, QCfile, NULL);
  if (STRINGP (file_name))
    {
      Lisp_Object file;
      unsigned char *contents;
      ptrdiff_t size;

      file = x_find_image_file (file_name);
      if (!STRINGP (file))
	{
	  image_error ("Cannot find image file `%s'", file_name, Qnil);
	  return 0;
	}

      /* Read the entire file into memory.  */
      contents = slurp_file (SSDATA (file), &size);
      if (contents == NULL)
	{
	  image_error ("Error loading SVG image `%s'", img->spec, Qnil);
	  return 0;
	}
      /* If the file was slurped into memory properly, parse it.  */
      success_p = svg_load_image (f, img, contents, size);
      xfree (contents);
    }
  /* Else its not a file, its a lisp object.  Load the image from a
     lisp object rather than a file.  */
  else
    {
      Lisp_Object data;

      data = image_spec_value(img->spec, QCdata, NULL);
      if (!STRINGP(data))
	{
	  image_error("Invalid image data `%s'", data, Qnil);
	  return 0;
	}
      success_p = svg_load_image(f, img, SDATA(data), SBYTES(data));
    }

  return success_p;
}

/* svg_load_image is a helper function for svg_load, which does the
   actual loading given contents and size, apart from frame and image
   structures, passed from svg_load.

   Uses librsvg to do most of the image processing.

   Returns true when successful.  */
static bool
svg_load_image(struct frame *f,         /* Pointer to emacs frame structure.  */
               struct image *img,       /* Pointer to emacs image structure.  */
               unsigned char *contents, /* String containing the SVG XML data to be parsed.  */
               ptrdiff_t size)          /* Size of data in bytes.  */
{
  RsvgHandle *rsvg_handle;
  RsvgDimensionData dimension_data;
  GError *err = NULL;
  GdkPixbuf *pixbuf;
  int width;
  int height;
  const guint8 *pixels;
  int rowstride;
  XImagePtr ximg;
  Lisp_Object specified_bg;
  XColor background;
  int x;
  int y;

#if ! GLIB_CHECK_VERSION (2, 36, 0)
  /* g_type_init is a glib function that must be called prior to
     using gnome type library functions (obsolete since 2.36.0).  */
  fn_g_type_init ();
#endif

  /* Make a handle to a new rsvg object.  */
  rsvg_handle = fn_rsvg_handle_new ();

  /* Parse the contents argument and fill in the rsvg_handle.  */
  fn_rsvg_handle_write (rsvg_handle, contents, size, &err);
  if (err) goto rsvg_error;

  /* The parsing is complete, rsvg_handle is ready to used, close it
     for further writes.  */
  fn_rsvg_handle_close (rsvg_handle, &err);
  if (err) goto rsvg_error;

  fn_rsvg_handle_get_dimensions (rsvg_handle, &dimension_data);
  if (! check_image_size (f, dimension_data.width, dimension_data.height))
    {
      image_error ("Invalid image size (see `max-image-size')", Qnil, Qnil);
      goto rsvg_error;
    }

  /* We can now get a valid pixel buffer from the svg file, if all
     went ok.  */
  pixbuf = fn_rsvg_handle_get_pixbuf (rsvg_handle);
  if (!pixbuf) goto rsvg_error;
  fn_g_object_unref (rsvg_handle);

  /* Extract some meta data from the svg handle.  */
  width     = fn_gdk_pixbuf_get_width (pixbuf);
  height    = fn_gdk_pixbuf_get_height (pixbuf);
  pixels    = fn_gdk_pixbuf_get_pixels (pixbuf);
  rowstride = fn_gdk_pixbuf_get_rowstride (pixbuf);

  /* Validate the svg meta data.  */
  eassert (fn_gdk_pixbuf_get_colorspace (pixbuf) == GDK_COLORSPACE_RGB);
  eassert (fn_gdk_pixbuf_get_n_channels (pixbuf) == 4);
  eassert (fn_gdk_pixbuf_get_has_alpha (pixbuf));
  eassert (fn_gdk_pixbuf_get_bits_per_sample (pixbuf) == 8);

  /* Try to create a x pixmap to hold the svg pixmap.  */
  if (!image_create_x_image_and_pixmap (f, img, width, height, 0, &ximg, 0))
    {
      fn_g_object_unref (pixbuf);
      return 0;
    }

  init_color_table ();

  /* Handle alpha channel by combining the image with a background
     color.  */
  specified_bg = image_spec_value (img->spec, QCbackground, NULL);
  if (!STRINGP (specified_bg)
      || !x_defined_color (f, SSDATA (specified_bg), &background, 0))
    {
#ifndef HAVE_NS
      background.pixel = FRAME_BACKGROUND_PIXEL (f);
      x_query_color (f, &background);
#else
      ns_query_color (FRAME_BACKGROUND_COLOR (f), &background, 1);
#endif /* HAVE_NS */
    }

  /* SVG pixmaps specify transparency in the last byte, so right
     shift 8 bits to get rid of it, since emacs doesn't support
     transparency.  */
  background.red   >>= 8;
  background.green >>= 8;
  background.blue  >>= 8;

  /* This loop handles opacity values, since Emacs assumes
     non-transparent images.  Each pixel must be "flattened" by
     calculating the resulting color, given the transparency of the
     pixel, and the image background color.  */
  for (y = 0; y < height; ++y)
    {
      for (x = 0; x < width; ++x)
	{
	  int red;
	  int green;
	  int blue;
	  int opacity;

	  red     = *pixels++;
	  green   = *pixels++;
	  blue    = *pixels++;
	  opacity = *pixels++;

	  red   = ((red * opacity)
		   + (background.red * ((1 << 8) - opacity)));
	  green = ((green * opacity)
		   + (background.green * ((1 << 8) - opacity)));
	  blue  = ((blue * opacity)
		   + (background.blue * ((1 << 8) - opacity)));

	  XPutPixel (ximg, x, y, lookup_rgb_color (f, red, green, blue));
	}

      pixels += rowstride - 4 * width;
    }

#ifdef COLOR_TABLE_SUPPORT
  /* Remember colors allocated for this image.  */
  img->colors = colors_in_color_table (&img->ncolors);
  free_color_table ();
#endif /* COLOR_TABLE_SUPPORT */

  fn_g_object_unref (pixbuf);

  img->width  = width;
  img->height = height;

  /* Maybe fill in the background field while we have ximg handy.
     Casting avoids a GCC warning.  */
  IMAGE_BACKGROUND (img, f, (XImagePtr_or_DC)ximg);

  /* Put ximg into the image.  */
  image_put_x_image (f, img, ximg, 0);

  return 1;

 rsvg_error:
  fn_g_object_unref (rsvg_handle);
  /* FIXME: Use error->message so the user knows what is the actual
     problem with the image.  */
  image_error ("Error parsing SVG image `%s'", img->spec, Qnil);
  fn_g_error_free (err);
  return 0;
}
#endif	/* defined (HAVE_RSVG) */


/***********************************************************************
				Ghostscript
 ***********************************************************************/

#ifdef HAVE_X_WINDOWS
# define HAVE_GHOSTSCRIPT 1
#endif /* HAVE_X_WINDOWS */

#ifdef HAVE_GHOSTSCRIPT

static bool gs_image_p (Lisp_Object object);
static bool gs_load (struct frame *f, struct image *img);
static void gs_clear_image (struct frame *f, struct image *img);

/* Keyword symbols: */
static Lisp_Object QCloader, QCbounding_box, QCpt_width, QCpt_height;

/* Indices of image specification fields in gs_format, below: */
enum gs_keyword_index
{
  GS_TYPE,
  GS_PT_WIDTH,
  GS_PT_HEIGHT,
  GS_FILE,
  GS_LOADER,
  GS_BOUNDING_BOX,
  GS_ASCENT,
  GS_MARGIN,
  GS_RELIEF,
  GS_ALGORITHM,
  GS_HEURISTIC_MASK,
  GS_MASK,
  GS_BACKGROUND,
  GS_LAST
};

/* Vector of image_keyword structures describing the format
   of valid user-defined image specifications.  */
static const struct image_keyword gs_format[GS_LAST] =
{
  {":type",		IMAGE_SYMBOL_VALUE,			1},
  {":pt-width",		IMAGE_POSITIVE_INTEGER_VALUE,		1},
  {":pt-height",	IMAGE_POSITIVE_INTEGER_VALUE,		1},
  {":file",		IMAGE_STRING_VALUE,			1},
  {":loader",		IMAGE_FUNCTION_VALUE,			0},
  {":bounding-box",	IMAGE_DONT_CHECK_VALUE_TYPE,		1},
  {":ascent",		IMAGE_ASCENT_VALUE,			0},
  {":margin",		IMAGE_NON_NEGATIVE_INTEGER_VALUE_OR_PAIR, 0},
  {":relief",		IMAGE_INTEGER_VALUE,			0},
  {":conversion",	IMAGE_DONT_CHECK_VALUE_TYPE,		0},
  {":heuristic-mask",	IMAGE_DONT_CHECK_VALUE_TYPE,		0},
  {":mask",		IMAGE_DONT_CHECK_VALUE_TYPE,		0},
  {":background",	IMAGE_STRING_OR_NIL_VALUE,		0}
};

/* Structure describing the image type `ghostscript': */
static struct image_type gs_type =
{
  &Qpostscript,
  gs_image_p,
  gs_load,
  gs_clear_image,
  NULL,
  NULL
};


/* Free X resources of Ghostscript image IMG which is used on frame F: */
static void gs_clear_image (struct frame *f, struct image *img)
{
  x_clear_image (f, img);
}


/* Return true if OBJECT is a valid Ghostscript image
   specification.  */
static bool gs_image_p (Lisp_Object object)
{
  struct image_keyword fmt[GS_LAST];
  Lisp_Object tem;
  int i;

  memcpy (fmt, gs_format, sizeof fmt);

  if (!parse_image_spec (object, fmt, GS_LAST, Qpostscript))
    return 0;

  /* Bounding box must be a list or vector containing 4 integers.  */
  tem = fmt[GS_BOUNDING_BOX].value;
  if (CONSP (tem))
    {
      for (i = 0; i < 4; ++i, tem = XCDR (tem))
	if (!CONSP (tem) || !INTEGERP (XCAR (tem)))
	  return 0;
      if (!NILP (tem))
	return 0;
    }
  else if (VECTORP (tem))
    {
      if (ASIZE (tem) != 4)
	return 0;
      for (i = 0; i < 4; ++i)
	if (!INTEGERP (AREF (tem, i)))
	  return 0;
    }
  else
    return 0;

  return 1;
}


/* Load Ghostscript image IMG for use on frame F.  Value is true
   if successful.  */
static bool gs_load (struct frame *f, struct image *img)
{
  uprintmax_t printnum1, printnum2;
  char buffer[sizeof " " + INT_STRLEN_BOUND (printmax_t)];
  Lisp_Object window_and_pixmap_id = Qnil, loader, pt_height, pt_width;
  Lisp_Object frame;
  double in_width, in_height;
  Lisp_Object pixel_colors = Qnil;

  /* Compute pixel size of pixmap needed from the given size in the
     image specification.  Sizes in the specification are in pt.  1 pt
     = 1/72 in, xdpi and ydpi are stored in the frame's X display
     info.  */
  pt_width = image_spec_value (img->spec, QCpt_width, NULL);
  in_width = INTEGERP (pt_width) ? XFASTINT (pt_width) / 72.0 : 0;
  in_width *= FRAME_RES_X (f);
  pt_height = image_spec_value (img->spec, QCpt_height, NULL);
  in_height = INTEGERP (pt_height) ? XFASTINT (pt_height) / 72.0 : 0;
  in_height *= FRAME_RES_Y (f);

  if (! (in_width <= INT_MAX && in_height <= INT_MAX
	 && check_image_size (f, in_width, in_height)))
    {
      image_error ("Invalid image size (see `max-image-size')", Qnil, Qnil);
      return 0;
    }
  img->width = in_width;
  img->height = in_height;

  /* Create the pixmap.  */
  eassert (img->pixmap == NO_PIXMAP);

  if (x_check_image_size (0, img->width, img->height))
    {
      /* Only W32 version did BLOCK_INPUT here.  ++kfs */
      block_input ();
      img->pixmap = XCreatePixmap (FRAME_X_DISPLAY (f), FRAME_X_WINDOW (f),
				   img->width, img->height,
				   DefaultDepthOfScreen (FRAME_X_SCREEN (f)));
      unblock_input ();
    }

  if (!img->pixmap)
    {
      image_error ("Unable to create pixmap for `%s'", img->spec, Qnil);
      return 0;
    }

  /* Call the loader to fill the pixmap.  It returns a process object
     if successful.  We do not record_unwind_protect here because
     other places in redisplay like calling window scroll functions
     don't either.  Let the Lisp loader use `unwind-protect' instead.  */
  printnum1 = FRAME_X_WINDOW (f);
  printnum2 = img->pixmap;
  window_and_pixmap_id
    = make_formatted_string (buffer, "%"pMu" %"pMu, printnum1, printnum2);

  printnum1 = FRAME_FOREGROUND_PIXEL (f);
  printnum2 = FRAME_BACKGROUND_PIXEL (f);
  pixel_colors
    = make_formatted_string (buffer, "%"pMu" %"pMu, printnum1, printnum2);

  XSETFRAME (frame, f);
  loader = image_spec_value (img->spec, QCloader, NULL);
  if (NILP (loader))
    loader = intern ("gs-load-image");

  img->lisp_data = call6 (loader, frame, img->spec,
			  make_number (img->width),
			  make_number (img->height),
			  window_and_pixmap_id,
			  pixel_colors);
  return PROCESSP (img->lisp_data);
}


/* Kill the Ghostscript process that was started to fill PIXMAP on
   frame F.  Called from XTread_socket when receiving an event
   telling Emacs that Ghostscript has finished drawing.  */
void x_kill_gs_process (Pixmap pixmap, struct frame *f)
{
  struct image_cache *c = FRAME_IMAGE_CACHE (f);
  int class;
  ptrdiff_t i;
  struct image *img;

  /* Find the image containing PIXMAP.  */
  for (i = 0; i < c->used; ++i)
    if (c->images[i]->pixmap == pixmap)
      break;

  /* Should someone in between have cleared the image cache, for
     instance, give up.  */
  if (i == c->used)
    return;

  /* Kill the GS process.  We should have found PIXMAP in the image
     cache and its image should contain a process object.  */
  img = c->images[i];
  eassert (PROCESSP (img->lisp_data));
  Fkill_process (img->lisp_data, Qnil);
  img->lisp_data = Qnil;

# if defined(HAVE_X_WINDOWS)

  /* On displays with a mutable colormap, figure out the colors
     allocated for the image by looking at the pixels of an XImage for
     img->pixmap.  */
  class = FRAME_X_VISUAL (f)->class;
  if (class != StaticColor && class != StaticGray && class != TrueColor)
    {
      XImagePtr ximg;

      block_input ();

      /* Try to get an XImage for img->pixmep.  */
      ximg = XGetImage (FRAME_X_DISPLAY (f), img->pixmap,
			0, 0, img->width, img->height, ~0, ZPixmap);
      if (ximg)
	{
	  int x, y;

	  /* Initialize the color table: */
	  init_color_table();

	  /* For each pixel of the image, look its color up in the
	     color table.  After having done so, the color table will
	     contain an entry for each color used by the image.  */
	  for (y = 0; y < img->height; ++y)
	    for (x = 0; x < img->width; ++x)
	      {
		unsigned long pixel = XGetPixel(ximg, x, y);
		lookup_pixel_color(f, pixel);
	      }

	  /* Record colors in the image.  Free color table and XImage: */
#  ifdef COLOR_TABLE_SUPPORT
	  img->colors = colors_in_color_table(&img->ncolors);
	  free_color_table();
#  endif /* COLOR_TABLE_SUPPORT */
	  XDestroyImage(ximg);

#  if 0 /* This does NOT seem to be the case.  If we free the colors
	 * here, we get a BadAccess later in x_clear_image when
	 * freeing the colors.  */
	  /* We have allocated colors once, but Ghostscript has also
	   * allocated colors on behalf of us.  So, to get the
	   * reference counts right, free them once.  */
	  if (img->ncolors) {
	    x_free_colors(f, img->colors, img->ncolors);
          }
#  endif /* 0 */
	}
      else
	image_error("Cannot get X image of `%s'; colors will not be freed",
		    img->spec, Qnil);

      unblock_input();
    }
# endif /* HAVE_X_WINDOWS */

  /* Now that we have the pixmap, compute mask and transform the
     image if requested.  */
  block_input();
  postprocess_image(f, img);
  unblock_input();
}
#endif /* HAVE_GHOSTSCRIPT */

/***********************************************************************
				Tests
 ***********************************************************************/

#ifdef GLYPH_DEBUG
DEFUN("imagep", Fimagep, Simagep, 1, 1, 0,
      doc: /* Value is non-nil if SPEC is a valid image specification.  */)
  (Lisp_Object spec)
{
  return (valid_image_p(spec) ? Qt : Qnil);
}


DEFUN("lookup-image", Flookup_image, Slookup_image, 1, 1, 0, "")
  (Lisp_Object spec)
{
  ptrdiff_t id = -1;

  if (valid_image_p(spec))
    id = lookup_image(SELECTED_FRAME(), spec);

  debug_print(spec);
  return make_number(id);
}
#endif /* GLYPH_DEBUG */


/***********************************************************************
			    Initialization
 ***********************************************************************/

DEFUN ("init-image-library", Finit_image_library, Sinit_image_library, 1, 1, 0,
       doc: /* Initialize image library implementing image type TYPE.
Return non-nil if TYPE is a supported image type.

If image libraries are loaded dynamically (currently only the case on
MS-Windows), load the library for TYPE if it is not yet loaded, using
the library file(s) specified by `dynamic-library-alist'.  */)
  (Lisp_Object type)
{
  return lookup_image_type (type) ? Qt : Qnil;
}

/* Look up image type TYPE, and return a pointer to its image_type
   structure.  Return 0 if TYPE is not a known image type.  */
static struct image_type *
lookup_image_type (Lisp_Object type)
{
  /* Types pbm and xbm are built-in and always available.  */
  if (EQ (type, Qpbm))
    return define_image_type (&pbm_type);

  if (EQ (type, Qxbm))
    return define_image_type (&xbm_type);

#if defined(HAVE_XPM) || defined(MAC_OS) || defined(HAVE_NS)
  if (EQ(type, Qxpm))
    return define_image_type(&xpm_type);
#endif /* HAVE_XPM || MAC_OS || HAVE_NS */

#if defined(HAVE_JPEG) || defined(MAC_OS) || defined(HAVE_NS)
  if (EQ(type, Qjpeg))
    return define_image_type(&jpeg_type);
#endif /* HAVE_JPEG || MAC_OS || HAVE_NS */

#if defined(HAVE_TIFF) || defined(MAC_OS) || defined(HAVE_NS)
  if (EQ(type, Qtiff))
    return define_image_type(&tiff_type);
#endif /* HAVE_TIFF || MAC_OS || HAVE_NS */

#if defined(HAVE_GIF) || defined(MAC_OS) || defined(HAVE_NS)
  if (EQ(type, Qgif))
    return define_image_type(&gif_type);
#endif /* HAVE_GIF || MAC_OS || HAVE_NS */

#if defined(HAVE_PNG) || defined(MAC_OS) || defined(HAVE_NS)
  if (EQ(type, Qpng))
    return define_image_type(&png_type);
#endif /* HAVE_PNG || MAC_OS || HAVE_NS */

#if defined(HAVE_RSVG)
  if (EQ(type, Qsvg))
    return define_image_type(&svg_type);
#endif /* HAVE_RSVG */

#if defined(HAVE_IMAGEMAGICK)
  if (EQ(type, Qimagemagick))
    return define_image_type(&imagemagick_type);
#endif /* HAVE_IMAGEMAGICK */

#ifdef HAVE_GHOSTSCRIPT
  if (EQ(type, Qpostscript))
    return define_image_type(&gs_type);
#endif /* HAVE_GHOSTSCRIPT */

  return NULL;
}

/* Reset image_types before dumping.
 * Called from Fdump_emacs: */
void reset_image_types(void)
{
  while (image_types)
    {
      struct image_type *next = image_types->next;
      xfree(image_types);
      image_types = next;
    }
}

void syms_of_image(void)
{
  /* Initialize this only once; it will be reset before dumping.  */
  image_types = NULL;

  /* Must be defined now because we're going to update it below, while
     defining the supported image types.  */
  DEFVAR_LISP ("image-types", Vimage_types,
    doc: /* List of potentially supported image types.
Each element of the list is a symbol for an image type, like 'jpeg or 'png.
To check whether it is really supported, use `image-type-available-p'.  */);
  Vimage_types = Qnil;

  DEFVAR_LISP ("max-image-size", Vmax_image_size,
    doc: /* Maximum size of images.
Emacs will not load an image into memory if its pixel width or
pixel height exceeds this limit.

If the value is an integer, it directly specifies the maximum
image height and width, measured in pixels.  If it is a floating
point number, it specifies the maximum image height and width
as a ratio to the frame height and width.  If the value is
non-numeric, there is no explicit limit on the size of images.  */);
  Vmax_image_size = make_float (MAX_IMAGE_SIZE);

  DEFSYM(Qcount, "count");
  DEFSYM(Qextension_data, "extension-data");
  DEFSYM(Qdelay, "delay");

  DEFSYM(QCascent, ":ascent");
  DEFSYM(QCmargin, ":margin");
  DEFSYM(QCrelief, ":relief");
  DEFSYM(QCconversion, ":conversion");
  DEFSYM(QCcolor_symbols, ":color-symbols");
  DEFSYM(QCheuristic_mask, ":heuristic-mask");
  DEFSYM(QCindex, ":index");
  DEFSYM(QCgeometry, ":geometry");
  DEFSYM(QCcrop, ":crop");
  DEFSYM(QCrotation, ":rotation");
  DEFSYM(QCmatrix, ":matrix");
  DEFSYM(QCcolor_adjustment, ":color-adjustment");
  DEFSYM(QCmask, ":mask");

  DEFSYM(Qlaplace, "laplace");
  DEFSYM(Qemboss, "emboss");
  DEFSYM(Qedge_detection, "edge-detection");
  DEFSYM(Qheuristic, "heuristic");

  DEFSYM(Qpostscript, "postscript");
  DEFSYM(QCmax_width, ":max-width");
  DEFSYM(QCmax_height, ":max-height");
#ifdef HAVE_GHOSTSCRIPT
  ADD_IMAGE_TYPE (Qpostscript);
  DEFSYM(QCloader, ":loader");
  DEFSYM(QCbounding_box, ":bounding-box");
  DEFSYM(QCpt_width, ":pt-width");
  DEFSYM(QCpt_height, ":pt-height");
#endif /* HAVE_GHOSTSCRIPT */

#ifdef HAVE_NTGUI
  DEFSYM(Qlibpng_version, "libpng-version");
  Fset(Qlibpng_version,
# if HAVE_PNG
       make_number(PNG_LIBPNG_VER)
# else
       make_number(-1)
# endif /* HAVE_PNG */
       );
  DEFSYM(Qlibgif_version, "libgif-version");
  Fset(Qlibgif_version,
# ifdef HAVE_GIF
       make_number((GIFLIB_MAJOR * 10000)
                   + (GIFLIB_MINOR * 100)
                   + GIFLIB_RELEASE)
# else
	make_number(-1)
# endif /* HAVE_GIF */
       );
  DEFSYM(Qlibjpeg_version, "libjpeg-version");
  Fset(Qlibjpeg_version,
# if HAVE_JPEG
       make_number(JPEG_LIB_VERSION)
# else
       make_number(-1)
# endif /* HAVE_JPEG */
       );
#endif /* HAVE_NTGUI */

  DEFSYM(Qpbm, "pbm");
  ADD_IMAGE_TYPE(Qpbm);

  DEFSYM(Qxbm, "xbm");
  ADD_IMAGE_TYPE(Qxbm);

#if defined(HAVE_XPM) || defined(MAC_OS) || defined(HAVE_NS)
  DEFSYM(Qxpm, "xpm");
  ADD_IMAGE_TYPE(Qxpm);
#endif /* HAVE_XPM || MAC_OS || HAVE_NS */

#if defined(HAVE_JPEG) || defined(MAC_OS) || defined(HAVE_NS)
  DEFSYM(Qjpeg, "jpeg");
  ADD_IMAGE_TYPE(Qjpeg);
#endif /* HAVE_JPEG || MAC_OS || HAVE_NS */

#if defined(HAVE_TIFF) || defined(MAC_OS) || defined(HAVE_NS)
  DEFSYM(Qtiff, "tiff");
  ADD_IMAGE_TYPE(Qtiff);
#endif /* HAVE_TIFF || MAC_OS || HAVE_NS */

#if defined(HAVE_GIF) || defined(MAC_OS) || defined(HAVE_NS)
  DEFSYM(Qgif, "gif");
  ADD_IMAGE_TYPE(Qgif);
#endif /* HAVE_GIF || MAC_OS || HAVE_NS */

#if defined(HAVE_PNG) || defined(MAC_OS) || defined(HAVE_NS)
  DEFSYM(Qpng, "png");
  ADD_IMAGE_TYPE(Qpng);
#endif /* HAVE_PNG || MAC_OS || HAVE_NS */

#if defined(HAVE_IMAGEMAGICK)
  DEFSYM(Qimagemagick, "imagemagick");
  ADD_IMAGE_TYPE(Qimagemagick);
#endif /* HAVE_IMAGEMAGICK */

#if defined(HAVE_RSVG)
  DEFSYM(Qsvg, "svg");
  ADD_IMAGE_TYPE(Qsvg);
# ifdef HAVE_NTGUI
  /* Other libraries used directly by svg code.  */
  DEFSYM(Qgdk_pixbuf, "gdk-pixbuf");
  DEFSYM(Qglib, "glib");
  DEFSYM(Qgobject, "gobject");
# endif /* HAVE_NTGUI  */
#endif /* HAVE_RSVG  */

  defsubr(&Sinit_image_library);
#ifdef HAVE_IMAGEMAGICK
  defsubr(&Simagemagick_types);
#endif /* HAVE_IMAGEMAGICK */
  defsubr(&Sclear_image_cache);
  defsubr(&Simage_flush);
  defsubr(&Simage_size);
  defsubr(&Simage_mask_p);
  defsubr(&Simage_metadata);

#ifdef GLYPH_DEBUG
  defsubr(&Simagep);
  defsubr(&Slookup_image);
#endif /* GLYPH_DEBUG */

  DEFVAR_BOOL("cross-disabled-images", cross_disabled_images,
    doc: /* Non-nil means always draw a cross over disabled images.
Disabled images are those having a `:conversion disabled' property.
A cross is always drawn on black & white displays.  */);
  cross_disabled_images = 0;

  DEFVAR_LISP("x-bitmap-file-path", Vx_bitmap_file_path,
    doc: /* List of directories to search for window system bitmap files.  */);
  Vx_bitmap_file_path = decode_env_path(0, PATH_BITMAPS, 0);

  DEFVAR_LISP("image-cache-eviction-delay", Vimage_cache_eviction_delay,
    doc: /* Maximum time after which images are removed from the cache.
When an image has not been displayed this many seconds, Emacs
automatically removes it from the image cache.  If the cache contains
a large number of images, the actual eviction time may be shorter.
The value can also be nil, meaning the cache is never cleared.

The function `clear-image-cache' disregards this variable.  */);
  Vimage_cache_eviction_delay = make_number(300);
#ifdef HAVE_IMAGEMAGICK
  DEFVAR_INT("imagemagick-render-type", imagemagick_render_type,
    doc: /* Integer indicating which ImageMagick rendering method to use.
The options are:
  0 -- the default method (pixel pushing)
  1 -- a newer method ("MagickExportImagePixels") that may perform
       better (speed etc) in some cases, but has not been as thoroughly
       tested with Emacs as the default method.  This method requires
       ImageMagick version 6.4.6 (approximately) or later.
*/);
  /* MagickExportImagePixels is in 6.4.6-9, but not 6.4.4-10.  */
  imagemagick_render_type = 0;
#endif /* HAVE_IMAGEMAGICK */

  return;
}

#if defined(MAC_OS) || defined(HAVE_CARBON)
void init_image(void)
{
# if defined(MAC_OS) && defined(MAC_OSX) && \
     (defined(TARGET_API_MAC_CARBON) && TARGET_API_MAC_CARBON)
  init_image_func_pointer();
# else
#  if defined(lint) && lint
  int ret = fprintf(stderr, "init_image()\n");
  if (ret != 0) {
    (void)ret;
  }
#  endif /* lint */
# endif /* MAC_OS && MAC_OSX && TARGET_API_MAC_CARBON */
  return;
}
#endif /* MAC_OS || HAVE_CARBON */

#ifdef ZPixmap
# undef ZPixmap
#endif /* ZPixmap */

#ifdef XLIB_ILLEGAL_ACCESS
# undef XLIB_ILLEGAL_ACCESS
#endif /* XLIB_ILLEGAL_ACCESS */

#ifdef FRAME_X_VISUAL
# undef FRAME_X_VISUAL
#endif /* FRAME_X_VISUAL */

#ifdef PUT_PIXEL
# undef PUT_PIXEL
#endif /* PUT_PIXEL */

/* arch-tag: 123c2a5e-14a8-4c53-ab95-af47d7db49b9
   (do not change this comment) */
