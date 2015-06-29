/* src/macselect.c: Selection processing for Emacs on Mac OS.
   Copyright (C) 2005, 2006, 2007 Free Software Foundation, Inc.

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

#include <config.h>

#include "lisp.h"

/* prefer Carbon over NS in this file: */
#if defined(HAVE_CARBON) && defined(HAVE_NS)
# undef HAVE_NS
#endif /* HAVE_CARBON && HAVE_NS */
#include "macterm.h"
#include "blockinput.h"
#include "keymap.h"

#if !TARGET_API_MAC_CARBON
# include <Endian.h>
typedef int ScrapRef;
typedef ResType ScrapFlavorType;
#else /* TARGET_API is actually MAC_CARBON: */
# if defined(HAVE_CARBON) && defined(HAVE_HITOOLBOX_APPEARANCE_H)
#  include <HIToolbox/Appearance.h>
# else
#  if !defined(__APPEARANCE__) || defined(__LP64__)
OSStatus SetThemeCursor(ThemeCursor inCursor);
#  endif /* !__APPEARANCE__ || __LP64__ */
# endif /* HAVE_CARBON && HAVE_HITOOLBOX_APPEARANCE_H */
# if defined(HAVE_CARBON) && defined(HAVE_HITOOLBOX_CARBONEVENTS_H)
#  include <HIToolbox/CarbonEvents.h>
# else
#  if !defined(__CARBONEVENTS__) || defined(__LP64__)
CFStringRef CreateTypeStringWithOSType(OSType inType);
#  endif /* !__CARBONEVENTS__ || __LP64__ */
# endif /* HAVE_CARBON && HAVE_HITOOLBOX_CARBONEVENTS_H */
# if defined(HAVE_CARBON) && defined(HAVE_HITOOLBOX_DRAG_H)
#  include <HIToolbox/Drag.h>
# else
#  if !defined(__DRAG__) || defined(__LP64__)
/* inline the prototypes we need: */
OSErr CountDragItems(DragRef theDrag, UInt16 *numItems);
OSErr GetDragItemReferenceNumber(DragRef theDrag, UInt16 theIndex,
                                 DragItemRef *theItemRef);
OSErr GetDragModifiers(DragRef theDrag, SInt16 *modifiers,
                       SInt16 *mouseDownModifiers,
                       SInt16 *mouseUpModifiers);
OSErr GetDragMouse(DragRef theDrag, Point *mouse,
                   Point *globalPinnedMouse);
OSErr GetFlavorFlags(DragRef theDrag, DragItemRef theItemRef,
                     FlavorType theType, FlavorFlags *theFlags);
OSErr HideDragHilite(DragRef theDrag);
OSErr InstallReceiveHandler(DragReceiveHandlerUPP receiveHandler,
                            WindowRef theWindow, void *handlerRefCon);
OSErr InstallTrackingHandler(DragTrackingHandlerUPP trackingHandler,
                             WindowRef theWindow, void *handlerRefCon);
OSErr RemoveReceiveHandler(DragReceiveHandlerUPP receiveHandler,
                           WindowRef theWindow);
OSErr RemoveTrackingHandler(DragTrackingHandlerUPP trackingHandler,
                            WindowRef theWindow);
OSErr ShowDragHilite(DragRef theDrag, RgnHandle hiliteFrame,
                     Boolean inside);
#  endif /* !__DRAG__ || __LP64__ */
# endif /* HAVE_CARBON && HAVE_HITOOLBOX_DRAG_H */
# if defined(HAVE_CARBON) && defined(HAVE_HITOOLBOX_MACWINDOWS_H)
#  include <HIToolbox/MacWindows.h>
# else
#  if !defined(__MACWINDOWS__) || defined(__LP64__)
/* inline the prototypes we need: */
WindowRef GetFrontWindowOfClass(WindowClass inWindowClass,
                                Boolean mustBeVisible);
Rect *GetWindowPortBounds(WindowRef window, Rect *bounds);
#  endif /* !__MACWINDOWS__ || __LP64__ */
# endif /* HAVE_CARBON && HAVE_HITOOLBOX_MACWINDOWS_H */
# if defined(HAVE_CARBON) && defined(HAVE_HITOOLBOX_SCRAP_H)
#  include <HIToolbox/Scrap.h>
# else
#  if !defined(__SCRAP__) || defined(__LP64__)
/* inline the prototypes we need: */
OSStatus ClearScrap(ScrapRef *inOutScrap);
OSStatus GetScrapByName(CFStringRef name, OptionBits options,
                        ScrapRef *scrap);
OSStatus GetScrapFlavorCount(ScrapRef scrap, UInt32 *infoCount);
OSStatus GetScrapFlavorData(ScrapRef scrap, ScrapFlavorType flavorType,
                            Size *byteCount, void *destination);
OSStatus GetScrapFlavorFlags(ScrapRef scrap, ScrapFlavorType flavorType,
                             ScrapFlavorFlags *flavorFlags);
OSStatus GetScrapFlavorInfoList(ScrapRef scrap, UInt32 *infoCount,
                                ScrapFlavorInfo info[]);
OSStatus GetScrapFlavorSize(ScrapRef scrap, ScrapFlavorType flavorType,
                            Size *byteCount);
OSStatus PutScrapFlavor(ScrapRef scrap, ScrapFlavorType flavorType,
                        ScrapFlavorFlags flavorFlags, Size flavorSize,
                        const void *flavorData);
#  endif /* !__SCRAP__ || __LP64__ */
# endif /* HAVE_CARBON && HAVE_HITOOLBOX_SCRAP_H */
# if defined(HAVE_CARBON) && defined(HAVE_QD_QUICKDRAWAPI_H)
#  include <QD/QuickdrawAPI.h>
# else
#  if !defined(__QUICKDRAWAPI__) || defined(__LP64__)
/* inline the prototypes we need: */
void DisposeRgn(RgnHandle rgn);
void GlobalToLocal(Point *pt);
RgnHandle NewRgn(void);
void RectRgn(RgnHandle rgn, const Rect *r);
#  endif /* !__QUICKDRAWAPI__ || __LP64__ */
# endif /* HAVE_CARBON && HAVE_QD_QUICKDRAWAPI_H */
#endif /* !TARGET_API_MAC_CARBON */

static OSStatus get_scrap_from_symbol P_((Lisp_Object, int, ScrapRef *));
static ScrapFlavorType get_flavor_type_from_symbol P_((Lisp_Object));
static int valid_scrap_target_type_p P_((Lisp_Object));
static OSStatus clear_scrap P_((ScrapRef *));
static OSStatus put_scrap_string P_((ScrapRef, Lisp_Object, Lisp_Object));
static OSStatus put_scrap_private_timestamp P_((ScrapRef, unsigned long));
static ScrapFlavorType scrap_has_target_type P_((ScrapRef, Lisp_Object));
static Lisp_Object get_scrap_string P_((ScrapRef, Lisp_Object));
static OSStatus get_scrap_private_timestamp P_((ScrapRef, unsigned long *));
static Lisp_Object get_scrap_target_type_list P_((ScrapRef));
static void x_own_selection P_((Lisp_Object, Lisp_Object));
static Lisp_Object x_get_local_selection P_((Lisp_Object, Lisp_Object, int));
static Lisp_Object x_get_foreign_selection P_((Lisp_Object, Lisp_Object,
                                               Lisp_Object));
#ifndef EMACS_GLOBALS_H
EXFUN (Fx_selection_owner_p, 1);
#endif /* !EMACS_GLOBALS_H */
#ifdef MAC_OSX
static OSStatus mac_handle_service_event P_((EventHandlerCallRef,
					     EventRef, void *));
void init_service_handler P_((void));
#endif /* MAC_OSX */

Lisp_Object QPRIMARY, QSECONDARY, QTIMESTAMP, QTARGETS;

#ifndef EMACS_GLOBALS_H
static Lisp_Object Vx_lost_selection_functions;
/* Coding system for communicating with other programs via scrap.  */
static Lisp_Object Vselection_coding_system;

/* Coding system for the next communicating with other programs.  */
static Lisp_Object Vnext_selection_coding_system;
#endif /* !EMACS_GLOBALS_H */

static Lisp_Object Qforeign_selection;

/* The timestamp of the last input event Emacs received from the
   window server.  */
/* Defined in keyboard.c.  */
extern unsigned long last_event_timestamp;

/* This is an association list whose elements are of the form
     ( SELECTION-NAME SELECTION-VALUE SELECTION-TIMESTAMP FRAME)
   SELECTION-NAME is a lisp symbol.
   SELECTION-VALUE is the value that emacs owns for that selection.
     It may be any kind of Lisp object.
   SELECTION-TIMESTAMP is the time at which emacs began owning this selection,
     as a cons of two 16-bit numbers (making a 32 bit time.)
   FRAME is the frame for which we made the selection.
   If there is an entry in this alist, and the data for the flavor
     type SCRAP_FLAVOR_TYPE_EMACS_TIMESTAMP in the corresponding scrap
     (if exists) coincides with SELECTION-TIMESTAMP, then it can be
     assumed that Emacs owns that selection.
   The only (eq) parts of this list that are visible from Lisp are the
    selection-values.  */
static Lisp_Object Vselection_alist;

#if defined(__GNUC__) && defined(__GNUC_MINOR__)
# if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6))
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wmultichar"
# endif /* gcc 4.6+ */
#endif /* any gcc */

#define SCRAP_FLAVOR_TYPE_EMACS_TIMESTAMP 'Etsp'

#if defined(__GNUC__) && defined(__GNUC_MINOR__)
# if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6))
#  pragma GCC diagnostic pop
# endif /* gcc 4.6+ */
#endif /* any gcc */

#ifndef EMACS_GLOBALS_H
/* This is an alist whose CARs are selection-types and whose CDRs are
   the names of Lisp functions to call to convert the given Emacs
   selection value to a string representing the given selection type.
   This is for Lisp-level extension of the emacs selection
   handling.  */
static Lisp_Object Vselection_converter_alist;
#endif /* !EMACS_GLOBALS_H */

/* A selection name (represented as a Lisp symbol) can be associated
   with a named scrap via `mac-scrap-name' property.  Likewise for a
   selection type with a scrap flavor type via `mac-ostype'.  */
static Lisp_Object Qmac_scrap_name, Qmac_ostype;

#ifdef MAC_OSX
# ifndef EMACS_GLOBALS_H
/* Selection name for communication via Services menu: */
static Lisp_Object Vmac_service_selection;
# endif /* !EMACS_GLOBALS_H */
#endif /* MAC_OSX */

/* Get a reference to the scrap corresponding to the symbol SYM.  The
   reference is set to *SCRAP, and it becomes NULL if there is no
   corresponding scrap.  Clear the scrap if CLEAR_P is non-zero.  */
static OSStatus
get_scrap_from_symbol(Lisp_Object sym, int clear_p, ScrapRef *scrap)
{
  OSStatus err = noErr;
  Lisp_Object str = Fget(sym, Qmac_scrap_name);

  if (!STRINGP (str))
    *scrap = NULL;
  else
    {
#if TARGET_API_MAC_CARBON
# ifdef MAC_OSX
      CFStringRef scrap_name = cfstring_create_with_string(str);
      OptionBits options = (clear_p ? kScrapClearNamedScrap
			    : kScrapGetNamedScrap);

      err = GetScrapByName(scrap_name, options, scrap);
      CFRelease(scrap_name);
# else	/* !MAC_OSX: */
      if (clear_p)
	err = ClearCurrentScrap();
      if (err == noErr)
	err = GetCurrentScrap(scrap);
# endif	/* !MAC_OSX */
#else	/* !TARGET_API_MAC_CARBON */
      if (clear_p)
	err = ZeroScrap();
      if (err == noErr)
	*scrap = 1;
#endif	/* !TARGET_API_MAC_CARBON */
    }

  return err;
}

/* Get a scrap flavor type from the symbol SYM.  Return 0 if no
   corresponding flavor type.  */
static ScrapFlavorType get_flavor_type_from_symbol(Lisp_Object sym)
{
  Lisp_Object str = Fget(sym, Qmac_ostype);

  if (STRINGP(str) && SBYTES(str) == 4)
    return EndianU32_BtoN(*((UInt32 *)SDATA(str)));

  return 0;
}

/* Check if the symbol SYM has a corresponding scrap flavor type: */
static int valid_scrap_target_type_p(Lisp_Object sym)
{
  return get_flavor_type_from_symbol(sym) != 0;
}

/* Clear the scrap whose reference is *SCRAP. */
static INLINE OSStatus clear_scrap(ScrapRef *scrap)
{
#if TARGET_API_MAC_CARBON
# ifdef MAC_OSX
  return ClearScrap(scrap);
# else
  return ClearCurrentScrap();
# endif /* MAC_OSX */
#else  /* !TARGET_API_MAC_CARBON */
  return ZeroScrap();
#endif	/* !TARGET_API_MAC_CARBON */
}

/* Put Lisp String STR to the scrap SCRAP.  The target type is
   specified by TYPE. */
static OSStatus
put_scrap_string(ScrapRef scrap, Lisp_Object type, Lisp_Object str)
{
  ScrapFlavorType flavor_type = get_flavor_type_from_symbol(type);

  if (flavor_type == 0)
    return noTypeErr;

#if TARGET_API_MAC_CARBON
  return PutScrapFlavor(scrap, flavor_type, kScrapFlavorMaskNone,
                        SBYTES(str), SDATA(str));
#else  /* !TARGET_API_MAC_CARBON */
  return PutScrap(SBYTES(str), flavor_type, SDATA(str));
#endif	/* !TARGET_API_MAC_CARBON */
}

#if defined(__GNUC__) && defined(__GNUC_MINOR__)
# if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6))
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wmultichar"
# endif /* gcc 4.6+ */
#endif /* any gcc */

/* Put TIMESTAMP to the scrap SCRAP.  The timestamp is used for
   checking if the scrap is owned by the process.  */
static INLINE OSStatus
put_scrap_private_timestamp(ScrapRef scrap, unsigned long timestamp)
{
#if TARGET_API_MAC_CARBON
  return PutScrapFlavor(scrap, SCRAP_FLAVOR_TYPE_EMACS_TIMESTAMP,
                        kScrapFlavorMaskSenderOnly,
                        sizeof(timestamp), &timestamp);
#else  /* !TARGET_API_MAC_CARBON */
  return PutScrap(sizeof(timestamp), SCRAP_FLAVOR_TYPE_EMACS_TIMESTAMP,
		  &timestamp);
#endif	/* !TARGET_API_MAC_CARBON */
}

#if defined(__GNUC__) && defined(__GNUC_MINOR__)
# if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6))
#  pragma GCC diagnostic pop
# endif /* gcc 4.6+ */
#endif /* any gcc */

/* Check if data for the target type TYPE is available in SCRAP: */
static ScrapFlavorType
scrap_has_target_type(ScrapRef scrap, Lisp_Object type)
{
  OSStatus err;
  ScrapFlavorType flavor_type = get_flavor_type_from_symbol(type);

  if (flavor_type)
    {
#if TARGET_API_MAC_CARBON
      ScrapFlavorFlags flags;

      err = GetScrapFlavorFlags(scrap, flavor_type, &flags);
      if (err != noErr)
	flavor_type = 0;
#else  /* !TARGET_API_MAC_CARBON */
      SInt32 size, offset;

      size = GetScrap(NULL, flavor_type, &offset);
      if (size < 0)
	flavor_type = 0;
#endif	/* !TARGET_API_MAC_CARBON */
    }

  return flavor_type;
}

/* Get data for the target type TYPE from SCRAP and create a Lisp
   string.  Return nil if failed to get data.  */
static Lisp_Object get_scrap_string(ScrapRef scrap, Lisp_Object type)
{
  OSStatus err;
  Lisp_Object result = Qnil;
  ScrapFlavorType flavor_type = get_flavor_type_from_symbol(type);
#if TARGET_API_MAC_CARBON
  Size size;

  if (flavor_type)
    {
      err = GetScrapFlavorSize(scrap, flavor_type, &size);
      if (err == noErr)
	{
	  do {
            result = make_uninit_string(size);
            err = GetScrapFlavorData(scrap, flavor_type, &size,
                                     SDATA(result));
            if (err != noErr)
              result = Qnil;
            else if (size < SBYTES(result))
              result = make_unibyte_string((const char *)SDATA(result),
                                           size);
          } while (STRINGP(result) && (size > SBYTES(result)));
	}
    }
#else
  Handle handle;
  SInt32 size, offset;

  if (flavor_type)
    size = GetScrap(NULL, flavor_type, &offset);
  if (size >= 0)
    {
      handle = NewHandle(size);
      HLock(handle);
      size = GetScrap(handle, flavor_type, &offset);
      if (size >= 0)
	result = make_unibyte_string(*handle, size);
      DisposeHandle(handle);
    }
#endif /* TARGET_API_MAC_CARBON */

  return result;
}


#if defined(__GNUC__) && defined(__GNUC_MINOR__)
# if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6))
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wmultichar"
# endif /* gcc 4.6+ */
#endif /* any gcc */

/* Get timestamp from the scrap SCRAP and set to *TIMESTAMP: */
static OSStatus
get_scrap_private_timestamp(ScrapRef scrap, unsigned long *timestamp)
{
  OSStatus err = noErr;
#if TARGET_API_MAC_CARBON
  ScrapFlavorFlags flags;

  err = GetScrapFlavorFlags(scrap, SCRAP_FLAVOR_TYPE_EMACS_TIMESTAMP,
                            &flags);
  if (err == noErr)
    {
      if (!(flags & kScrapFlavorMaskSenderOnly))
	err = noTypeErr;
      else
	{
	  Size size = sizeof(*timestamp);

	  err =
            GetScrapFlavorData(scrap, SCRAP_FLAVOR_TYPE_EMACS_TIMESTAMP,
                               &size, timestamp);
	  if ((err == noErr) && (size != sizeof(*timestamp)))
	    err = noTypeErr;
	}
    }
#else  /* !TARGET_API_MAC_CARBON */
  Handle handle;
  SInt32 size, offset;

  size = GetScrap(NULL, SCRAP_FLAVOR_TYPE_EMACS_TIMESTAMP, &offset);
  if (size == sizeof(*timestamp))
    {
      handle = NewHandle(size);
      HLock(handle);
      size = GetScrap(handle, SCRAP_FLAVOR_TYPE_EMACS_TIMESTAMP, &offset);
      if (size == sizeof(*timestamp))
	*timestamp = *((unsigned long *)*handle);
      DisposeHandle(handle);
    }
  if (size != sizeof(*timestamp))
    err = noTypeErr;
#endif	/* !TARGET_API_MAC_CARBON */

  return err;
}

#if defined(__GNUC__) && defined(__GNUC_MINOR__)
# if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6))
#  pragma GCC diagnostic pop
# endif /* gcc 4.6+ */
#endif /* any gcc */

/* Get the list of target types in SCRAP.  The return value is a list
   of target type symbols possibly followed by scrap flavor type
   strings.  */
static Lisp_Object get_scrap_target_type_list(ScrapRef scrap)
{
  Lisp_Object result = Qnil, rest, target_type;
#if TARGET_API_MAC_CARBON
  OSStatus err;
  UInt32 count, i, type;
  ScrapFlavorInfo *flavor_info = (ScrapFlavorInfo *)NULL;
  Lisp_Object strings = Qnil;

  err = GetScrapFlavorCount(scrap, &count);
  if (err == noErr)
    flavor_info = ((ScrapFlavorInfo *)
                   xmalloc(sizeof(ScrapFlavorInfo) * count));
  err = GetScrapFlavorInfoList(scrap, &count, flavor_info);
  if (err != noErr)
    {
      xfree (flavor_info);
      flavor_info = NULL;
    }
  if (flavor_info == NULL)
    count = 0;
#endif /* TARGET_API_MAC_CARBON */
  for (rest = Vselection_converter_alist; CONSP (rest); rest = XCDR (rest))
    {
      ScrapFlavorType flavor_type = 0;

      if (CONSP (XCAR (rest))
	  && (target_type = XCAR (XCAR (rest)),
	      SYMBOLP (target_type))
	  && (flavor_type = scrap_has_target_type (scrap, target_type)))
	{
	  result = Fcons (target_type, result);
#if TARGET_API_MAC_CARBON
	  for (i = 0; i < count; i++)
	    if (flavor_info[i].flavorType == flavor_type)
	      {
		flavor_info[i].flavorType = 0;
		break;
	      }
#endif /* TARGET_API_MAC_CARBON */
	}
    }
#if TARGET_API_MAC_CARBON
  if (flavor_info)
    {
      for (i = 0; i < count; i++)
	if (flavor_info[i].flavorType)
	  {
	    type = EndianU32_NtoB (flavor_info[i].flavorType);
	    strings = Fcons (make_unibyte_string ((char *) &type, 4), strings);
	  }
      result = nconc2 (result, strings);
      xfree (flavor_info);
    }
#endif /* TARGET_API_MAC_CARBON */

  return result;
}

/* Do protocol to assert ourself as a selection owner.
   Update the Vselection_alist so that we can reply to later requests for
   our selection.  */
static void
x_own_selection(Lisp_Object selection_name, Lisp_Object selection_value)
{
  OSStatus err;
  ScrapRef scrap;
  struct gcpro gcpro1, gcpro2;
  Lisp_Object rest, handler_fn, value, type;
  int count;

  value = LISP_INITIALLY_ZERO;

  CHECK_SYMBOL(selection_name);

  GCPRO2(selection_name, selection_value);

  BLOCK_INPUT;

  err = get_scrap_from_symbol(selection_name, 1, &scrap);
  if (err == noErr && scrap)
    {
      /* Don't allow a quit within the converter.
	 When the user types C-g, he would be surprised
	 if by luck it came during a converter.  */
      count = SPECPDL_INDEX();
      specbind(Qinhibit_quit, Qt);

      for (rest = Vselection_converter_alist; CONSP(rest); rest = XCDR(rest))
	{
	  if (!(CONSP(XCAR(rest))
		&& (type = XCAR(XCAR(rest)),
		    SYMBOLP(type))
		&& valid_scrap_target_type_p(type)
		&& (handler_fn = XCDR(XCAR(rest)),
		    SYMBOLP(handler_fn))))
	    continue;

	  if (!NILP(handler_fn))
	    value = call3(handler_fn, selection_name,
			  type, selection_value);

	  if (STRINGP(value))
	    err = put_scrap_string(scrap, type, value);
	  else if (CONSP(value)
		   && EQ(XCAR(value), type)
		   && STRINGP(XCDR(value)))
	    err = put_scrap_string(scrap, type, XCDR(value));
	}

      unbind_to(count, Qnil);

      if (err == noErr)
	err = put_scrap_private_timestamp(scrap, last_event_timestamp);
    }

  UNBLOCK_INPUT;

  UNGCPRO;

  if (scrap && err != noErr)
    error ("Cannot set selection");

  /* Now update the local cache */
  {
    Lisp_Object selection_time;
    Lisp_Object selection_data;
    Lisp_Object prev_value;

    selection_time = long_to_cons (last_event_timestamp);
    selection_data = Fcons (selection_name,
			    Fcons (selection_value,
				   Fcons (selection_time,
					  Fcons (selected_frame, Qnil))));
    prev_value = assq_no_quit (selection_name, Vselection_alist);

    Vselection_alist = Fcons (selection_data, Vselection_alist);

    /* If we already owned the selection, remove the old selection data.
       Perhaps we should destructively modify it instead.
       Do NOT use Fdelq as that may QUIT.  */
    if (!NILP (prev_value))
      {
	Lisp_Object rest; /* we know it is not the CAR, so it is easy. */
	for (rest = Vselection_alist; !NILP (rest); rest = Fcdr (rest))
	  if (EQ (prev_value, Fcar (XCDR (rest))))
	    {
	      XSETCDR (rest, Fcdr (XCDR (rest)));
	      break;
	    }
      }
  }
}

/* Given a selection-name and desired type, look up our local copy of
   the selection value and convert it to the type.
   The value is nil or a string.
   This function is used both for remote requests (LOCAL_REQUEST is zero)
   and for local x-get-selection-internal (LOCAL_REQUEST is nonzero).

   This calls random Lisp code, and may signal or gc.  */
static Lisp_Object
x_get_local_selection(Lisp_Object selection_symbol,
                      Lisp_Object target_type, int local_request)
{
  Lisp_Object local_value;
  Lisp_Object handler_fn, value, type, check;
  int count;

  type = LISP_INITIALLY_ZERO;

  if (NILP(Fx_selection_owner_p(selection_symbol,
                                LISP_INITIALLY_ZERO)))
    return Qnil;

  local_value = assq_no_quit(selection_symbol, Vselection_alist);

  /* TIMESTAMP is a special case because that is/was easiest: */
  if (EQ(target_type, QTIMESTAMP))
    {
      handler_fn = Qnil;
      value = XCAR(XCDR(XCDR(local_value)));
    }
#if 0
  else if (EQ(target_type, QDELETE))
    {
      handler_fn = Qnil;
      Fx_disown_selection_internal(selection_symbol,
                                   XCAR(XCDR(XCDR(local_value))));
      value = QNULL;
    }
#endif /* 0 */
  else
    {
      /* Don't allow a quit within the converter.
	 When the user types C-g, he would be surprised
	 if by luck it came during a converter.  */
      count = SPECPDL_INDEX();
      specbind(Qinhibit_quit, Qt);

      CHECK_SYMBOL(target_type);
      handler_fn = Fcdr(Fassq(target_type, Vselection_converter_alist));
      /* gcpro is not needed here since nothing but HANDLER_FN
	 is live, and that ought to be a symbol.  */

      if (!NILP(handler_fn))
	value = call3(handler_fn,
		      selection_symbol, (local_request ? Qnil : target_type),
		      XCAR(XCDR(local_value)));
      else
	value = Qnil;
      unbind_to(count, Qnil);
    }

  /* Make sure this value is of a type that we could transmit
     to another X client.  */

  check = value;
  if (CONSP(value) && SYMBOLP(XCAR(value)))
    {
      type = XCAR(value),
      check = XCDR(value);
    }

  if (NILP(type)) {
    ; /* ??? */
  }

  if (STRINGP(check) || VECTORP(check) || SYMBOLP(check)
      || INTEGERP(check) || NILP(value))
    return value;
  /* Check for a value that cons_to_long could handle: */
  else if (CONSP(check)
	   && INTEGERP(XCAR(check))
	   && (INTEGERP(XCDR(check))
	       || (CONSP(XCDR(check))
                   && INTEGERP(XCAR(XCDR(check)))
                   && NILP(XCDR(XCDR(check))))))
    return value;

  signal_error("Invalid data returned by selection-conversion function",
               list2(handler_fn, value));
}


/* Clear all selections that were made from frame F.
   We do this when about to delete a frame.  */
void x_clear_frame_selections(FRAME_PTR f)
{
  Lisp_Object frame;
  Lisp_Object rest;

  XSETFRAME(frame, f);

  /* Otherwise, we are really honest and truly being told to drop it.
     Do NOT use Fdelq as that may QUIT;.  */

  /* Delete elements from the beginning of Vselection_alist.  */
  while (!NILP(Vselection_alist)
	 && EQ(frame, Fcar(Fcdr(Fcdr(Fcdr(Fcar(Vselection_alist)))))))
    {
      /* Let random Lisp code notice that the selection has been stolen: */
      Lisp_Object hooks, selection_symbol;

      hooks = Vx_lost_selection_functions;
      selection_symbol = Fcar(Fcar(Vselection_alist));

      if (!EQ(hooks, Qunbound)
	  && !NILP(Fx_selection_owner_p(selection_symbol,
                                        LISP_INITIALLY_ZERO)))
	{
	  for (; CONSP(hooks); hooks = Fcdr(hooks))
	    call1(Fcar(hooks), selection_symbol);
#if 0 /* This can crash when deleting a frame
       * from x_connection_closed.  Anyway, it seems unnecessary;
       * something else should cause a redisplay.  */
	  redisplay_preserve_echo_area(21);
#endif /* 0 */
	}

      Vselection_alist = Fcdr(Vselection_alist);
    }

  /* Delete elements after the beginning of Vselection_alist: */
  for (rest = Vselection_alist; !NILP(rest); rest = Fcdr(rest))
    if (EQ(frame, Fcar(Fcdr(Fcdr(Fcdr(Fcar(XCDR(rest))))))))
      {
	/* Let random Lisp code notice that the selection has been stolen: */
	Lisp_Object hooks, selection_symbol;

	hooks = Vx_lost_selection_functions;
	selection_symbol = Fcar(Fcar(XCDR(rest)));

	if (!EQ(hooks, Qunbound)
	  && !NILP(Fx_selection_owner_p(selection_symbol,
                                        LISP_INITIALLY_ZERO)))
	  {
	    for (; CONSP(hooks); hooks = Fcdr(hooks))
	      call1(Fcar(hooks), selection_symbol);
#if 0 /* See above */
	    redisplay_preserve_echo_area(22);
#endif /* 0 */
	  }
	XSETCDR(rest, Fcdr(XCDR(rest)));
	break;
      }
}

/* Do protocol to read selection-data from the server.
   Converts this to Lisp data and returns it.  */
static Lisp_Object
x_get_foreign_selection(Lisp_Object selection_symbol,
                        Lisp_Object target_type, Lisp_Object time_stamp)
{
  OSStatus err;
  ScrapRef scrap;
  Lisp_Object result = Qnil;

  BLOCK_INPUT;

  err = get_scrap_from_symbol (selection_symbol, 0, &scrap);
  if (err == noErr && scrap)
    {
      if (EQ (target_type, QTARGETS))
	{
	  result = get_scrap_target_type_list (scrap);
	  result = Fvconcat (1, &result);
	}
      else
	{
	  result = get_scrap_string (scrap, target_type);
	  if (STRINGP (result))
	    Fput_text_property (make_number (0), make_number (SBYTES (result)),
				Qforeign_selection, target_type, result);
	}
    }

  UNBLOCK_INPUT;

  return result;
}


DEFUN ("x-own-selection-internal", Fx_own_selection_internal,
       Sx_own_selection_internal, 2, 3, 0,
       doc: /* Assert a selection of the given TYPE with the given VALUE.
TYPE is a symbol, typically `PRIMARY', `SECONDARY', or `CLIPBOARD'.
VALUE is typically a string, or a cons of two markers, but may be
anything that the functions on `selection-converter-alist' know about.  */)
     (Lisp_Object selection_name, Lisp_Object selection_value,
      Lisp_Object frame)
{
#ifdef __APPLE_CC__
# pragma unused (frame)
#endif /* __APPLE_CC__ */
  check_mac ();
  CHECK_SYMBOL (selection_name);
  if (NILP (selection_value)) error ("SELECTION-VALUE may not be nil");
  x_own_selection (selection_name, selection_value);
  return selection_value;
}


/* Request the selection value from the owner.  If we are the owner,
   simply return our selection value.  If we are not the owner, this
   will block until all of the data has arrived.  */
DEFUN ("x-get-selection-internal", Fx_get_selection_internal,
       Sx_get_selection_internal, 2, 4, 0,
       doc: /* Return text selected from some Mac application.
SELECTION is a symbol, typically `PRIMARY', `SECONDARY', or `CLIPBOARD'.
TYPE is the type of data desired, typically `STRING'.
TIME_STAMP is ignored on Mac.  */)
     (Lisp_Object selection_symbol, Lisp_Object target_type,
      Lisp_Object time_stamp, Lisp_Object terminal)
{
#ifdef __APPLE_CC__
# pragma unused (terminal)
#endif /* __APPLE_CC__ */
  Lisp_Object val = Qnil;
  struct gcpro gcpro1, gcpro2;
  GCPRO2(target_type, val); /* we store newly consed data into these */
  check_mac();
  CHECK_SYMBOL(selection_symbol);
  CHECK_SYMBOL(target_type);

  if (NILP(val)) {
    ; /* ??? */
  }

  val = x_get_local_selection(selection_symbol, target_type, 1);

  if (NILP(val))
    {
      val = x_get_foreign_selection(selection_symbol, target_type, time_stamp);
      goto DONE;
    }

  if (CONSP(val)
      && SYMBOLP(XCAR(val)))
    {
      val = XCDR(val);
      if (CONSP(val) && NILP(XCDR(val)))
	val = XCAR(val);
    }
 DONE:
  UNGCPRO;
  return val;
}

/* from xdisp.c: */
extern void prepare_menu_bars(void);

DEFUN ("x-disown-selection-internal", Fx_disown_selection_internal,
       Sx_disown_selection_internal, 1, 3, 0,
       doc: /* If we own the selection SELECTION, disown it.
Disowning it means there is no such selection.  */)
     (Lisp_Object selection, Lisp_Object time, Lisp_Object terminal)
{
  OSStatus err;
  ScrapRef scrap;
  Lisp_Object local_selection_data;

  check_mac();
  CHECK_SYMBOL(selection);

  if (NILP(Fx_selection_owner_p(selection, terminal)))
    return Qnil; /* Do NOT disown the selection when we are not the owner. */

  local_selection_data = assq_no_quit(selection, Vselection_alist);

  /* Do NOT use Fdelq as that may QUIT;.  */

  if (EQ(local_selection_data, Fcar(Vselection_alist)))
    Vselection_alist = Fcdr(Vselection_alist);
  else
    {
      Lisp_Object rest;
      for (rest = Vselection_alist; !NILP (rest); rest = Fcdr (rest))
	if (EQ (local_selection_data, Fcar (XCDR (rest))))
	  {
	    XSETCDR (rest, Fcdr (XCDR (rest)));
	    break;
	  }
    }

  /* Let random lisp code notice that the selection has been stolen: */
  {
    Lisp_Object rest;
    rest = Vx_lost_selection_functions;
    if (!EQ(rest, Qunbound))
      {
	for (; CONSP(rest); rest = Fcdr(rest))
	  call1(Fcar(rest), selection);
	prepare_menu_bars();
	redisplay_preserve_echo_area(20);
      }
  }

  BLOCK_INPUT;

  err = get_scrap_from_symbol(selection, 0, &scrap);
  if (err == noErr && scrap)
    clear_scrap(&scrap);

  UNBLOCK_INPUT;

  return Qt;
}


DEFUN ("x-selection-owner-p", Fx_selection_owner_p, Sx_selection_owner_p,
       0, 2, 0,
       doc: /* Whether the current Emacs process owns the given SELECTION.
The arg should be the name of the selection in question, typically one of
the symbols `PRIMARY', `SECONDARY', or `CLIPBOARD'.
For convenience, the symbol nil is the same as `PRIMARY',
and t is the same as `SECONDARY'.  */)
     (Lisp_Object selection, Lisp_Object terminal)
{
#ifdef __APPLE_CC__
# pragma unused (terminal)
#endif /* __APPLE_CC__ */
  OSStatus err;
  ScrapRef scrap;
  Lisp_Object result = Qnil, local_selection_data;

  check_mac();
  CHECK_SYMBOL(selection);
  if (EQ(selection, Qnil)) selection = QPRIMARY;
  if (EQ(selection, Qt)) selection = QSECONDARY;

  local_selection_data = assq_no_quit(selection, Vselection_alist);

  if (NILP(local_selection_data))
    return Qnil;

  BLOCK_INPUT;

  err = get_scrap_from_symbol(selection, 0, &scrap);
  if ((err == noErr) && scrap)
    {
      unsigned long timestamp;

      err = get_scrap_private_timestamp (scrap, &timestamp);
      if (err == noErr
	  && (timestamp
	      == cons_to_long (XCAR (XCDR (XCDR (local_selection_data))))))
	result = Qt;
    }
  else
    result = Qt;

  UNBLOCK_INPUT;

  return result;
}

DEFUN ("x-selection-exists-p", Fx_selection_exists_p, Sx_selection_exists_p,
       0, 2, 0,
       doc: /* Whether there is an owner for the given SELECTION.
The arg should be the name of the selection in question, typically one of
the symbols `PRIMARY', `SECONDARY', or `CLIPBOARD'.
For convenience, the symbol nil is the same as `PRIMARY',
and t is the same as `SECONDARY'.  */)
     (Lisp_Object selection, Lisp_Object terminal)
{
  OSStatus err;
  ScrapRef scrap;
  Lisp_Object result = Qnil, rest;

  /* It should be safe to call this before we have an Mac frame: */
  if (! FRAME_MAC_P(SELECTED_FRAME()))
    return Qnil;

  CHECK_SYMBOL(selection);
  if (!NILP(Fx_selection_owner_p(selection, terminal)))
    return Qt;
  if (EQ(selection, Qnil)) selection = QPRIMARY;
  if (EQ(selection, Qt)) selection = QSECONDARY;

  BLOCK_INPUT;

  err = get_scrap_from_symbol(selection, 0, &scrap);
  if ((err == noErr) && scrap)
    for (rest = Vselection_converter_alist; CONSP(rest); rest = XCDR(rest))
      {
	if (CONSP(XCAR(rest)) && SYMBOLP(XCAR(XCAR(rest)))
	    && scrap_has_target_type(scrap, XCAR(XCAR(rest))))
	  {
	    result = Qt;
	    break;
	  }
      }

  UNBLOCK_INPUT;

  return result;
}


/***********************************************************************
			 Apple event support
***********************************************************************/
int mac_ready_for_apple_events = 0;
#ifndef EMACS_GLOBALS_H
static Lisp_Object Vmac_apple_event_map;
#endif /* !EMACS_GLOBALS_H */
#ifndef _EMACS_MACTERM_H
static
#endif /* !_EMACS_MACTERM_H */
Lisp_Object Qmac_apple_event_class, Qmac_apple_event_id;
static Lisp_Object Qemacs_suspension_id;
extern Lisp_Object Qundefined;
extern void mac_store_apple_event P_((Lisp_Object, Lisp_Object,
				      const AEDesc *));

struct apple_event_binding
{
  UInt32 code;			/* Apple event class or ID.  */
  Lisp_Object key, binding;
};

struct suspended_ae_info
{
  UInt32 expiration_tick, suspension_id;
  AppleEvent apple_event, reply;
  struct suspended_ae_info *next;
};

/* List of apple events deferred at the startup time.  */
static struct suspended_ae_info *deferred_apple_events = NULL;

/* List of suspended apple events, in order of expiration_tick.  */
static struct suspended_ae_info *suspended_apple_events = NULL;

static void
find_event_binding_fun(Lisp_Object key, Lisp_Object binding,
                       Lisp_Object args, void *data)
{
  struct apple_event_binding *event_binding =
    (struct apple_event_binding *)data;
  Lisp_Object code_string;

  if (!SYMBOLP (key))
    return;
  code_string = Fget (key, args);
  if (STRINGP (code_string) && SBYTES (code_string) == 4
      && (EndianU32_BtoN (*((UInt32 *) SDATA (code_string)))
	  == event_binding->code))
    {
      event_binding->key = key;
      event_binding->binding = binding;
    }
}

static void
find_event_binding(Lisp_Object keymap,
                   struct apple_event_binding *event_binding, int class_p)
{
  if (event_binding->code == 0)
    event_binding->binding =
      access_keymap (keymap, event_binding->key, 0, 1, 0);
  else
    {
      event_binding->binding = Qnil;
      map_keymap (keymap, find_event_binding_fun,
		  class_p ? Qmac_apple_event_class : Qmac_apple_event_id,
		  event_binding, 0);
    }
}

Lisp_Object
mac_find_apple_event_spec(AEEventClass class, AEEventID id,
                          Lisp_Object *class_key, Lisp_Object *id_key,
                          Lisp_Object *binding)
{
  struct apple_event_binding event_binding;
  Lisp_Object keymap;

  *binding = Qnil;

  keymap = get_keymap(Vmac_apple_event_map, 0, 0);
  if (NILP(keymap))
    return LISP_INITIALLY_ZERO;

  event_binding.code = class;
  event_binding.key = *class_key;
  event_binding.binding = Qnil;
  find_event_binding(keymap, &event_binding, 1);
  *class_key = event_binding.key;
  keymap = get_keymap(event_binding.binding, 0, 0);
  if (NILP(keymap))
    return LISP_INITIALLY_ZERO;

  event_binding.code = id;
  event_binding.key = *id_key;
  event_binding.binding = Qnil;
  find_event_binding(keymap, &event_binding, 0);
  *id_key = event_binding.key;
  *binding = event_binding.binding;

  return LISP_INITIALLY_ZERO;
}

static OSErr
defer_apple_events(const AppleEvent *apple_event, const AppleEvent *reply)
{
  OSErr err;
  struct suspended_ae_info *newinf;

  newinf = ((struct suspended_ae_info *)
            xmalloc(sizeof(struct suspended_ae_info)));
  bzero(newinf, sizeof(struct suspended_ae_info));
  newinf->apple_event.descriptorType = typeNull;
  newinf->reply.descriptorType = typeNull;

  err = AESuspendTheCurrentEvent(apple_event);

  /* Mac OS 10.3 Xcode manual says AESuspendTheCurrentEvent makes
     copies of the Apple event and the reply, but Mac OS 10.4 Xcode
     manual says it doesn't.  Anyway we create copies of them and save
     them in `deferred_apple_events'.  */
  if (err == noErr)
    err = AEDuplicateDesc(apple_event, &newinf->apple_event);
  if (err == noErr)
    err = AEDuplicateDesc(reply, &newinf->reply);
  if (err == noErr)
    {
      newinf->next = deferred_apple_events;
      deferred_apple_events = newinf;
    }
  else
    {
      AEDisposeDesc(&newinf->apple_event);
      AEDisposeDesc(&newinf->reply);
      xfree(newinf);
    }

  return err;
}

static OSErr
mac_handle_apple_event_1(Lisp_Object class, Lisp_Object id,
                         const AppleEvent *apple_event, AppleEvent *reply)
{
  OSErr err;
  static UInt32 suspension_id = 0U;
  struct suspended_ae_info *new_info;

  new_info = ((struct suspended_ae_info *)
              xmalloc(sizeof(struct suspended_ae_info)));
  bzero(new_info, sizeof(struct suspended_ae_info));
  new_info->apple_event.descriptorType = typeNull;
  new_info->reply.descriptorType = typeNull;

  err = AESuspendTheCurrentEvent(apple_event);
  if (err == noErr)
    err = AEDuplicateDesc(apple_event, &new_info->apple_event);
  if (err == noErr)
    err = AEDuplicateDesc(reply, &new_info->reply);
  if (err == noErr)
    err = AEPutAttributePtr(&new_info->apple_event,
                            KEY_EMACS_SUSPENSION_ID_ATTR, typeUInt32,
                            &suspension_id, sizeof(UInt32));
  if (err == noErr)
    {
      OSErr err1;
      SInt32 reply_requested;

      err1 = AEGetAttributePtr(&new_info->apple_event,
                               keyReplyRequestedAttr, typeSInt32, NULL,
                               &reply_requested, sizeof(SInt32), NULL);
      if (err1 != noErr)
	{
	  /* Emulate keyReplyRequestedAttr in older versions: */
	  reply_requested = reply->descriptorType != typeNull;
	  err = AEPutAttributePtr(&new_info->apple_event,
                                  keyReplyRequestedAttr, typeSInt32,
                                  &reply_requested, sizeof(SInt32));
	}
    }
  if (err == noErr)
    {
      SInt32 timeout = 0;
      struct suspended_ae_info **p;

      new_info->suspension_id = suspension_id;
      suspension_id++;
      err = AEGetAttributePtr(apple_event, keyTimeoutAttr, typeSInt32,
			      NULL, &timeout, sizeof(SInt32), NULL);
      new_info->expiration_tick = (TickCount() + timeout);

      for (p = &suspended_apple_events; *p; p = &(*p)->next)
	if ((*p)->expiration_tick >= new_info->expiration_tick)
	  break;
      new_info->next = *p;
      *p = new_info;

      mac_store_apple_event(class, id, &new_info->apple_event);
    }
  else
    {
      AEDisposeDesc(&new_info->reply);
      AEDisposeDesc(&new_info->apple_event);
      xfree(new_info);
    }

  return err;
}

#ifndef _EMACS_MACTERM_H
static
#endif /* !_EMACS_MACTERM_H */
pascal OSErr mac_handle_apple_event(const AppleEvent *apple_event,
                                    AppleEvent *reply, SInt32 refcon)
{
  OSErr err;
  UInt32 suspension_id;
  AEEventClass event_class;
  AEEventID event_id;
  Lisp_Object class_key, id_key, binding;

  if (!mac_ready_for_apple_events)
    {
      err = defer_apple_events (apple_event, reply);
      if (err != noErr)
	return errAEEventNotHandled;
      return noErr;
    }

  err = AEGetAttributePtr (apple_event, KEY_EMACS_SUSPENSION_ID_ATTR,
			   typeUInt32, NULL,
			   &suspension_id, sizeof (UInt32), NULL);
  if (err == noErr)
    /* Previously suspended event.  Pass it to the next handler.  */
    return errAEEventNotHandled;

  err = AEGetAttributePtr (apple_event, keyEventClassAttr, typeType, NULL,
			   &event_class, sizeof (AEEventClass), NULL);
  if (err == noErr)
    err = AEGetAttributePtr (apple_event, keyEventIDAttr, typeType, NULL,
			     &event_id, sizeof (AEEventID), NULL);
  if (err == noErr)
    {
      mac_find_apple_event_spec (event_class, event_id,
				 &class_key, &id_key, &binding);
      if (!NILP (binding) && !EQ (binding, Qundefined))
	{
	  if (INTEGERP (binding))
	    return XINT (binding);
	  err = mac_handle_apple_event_1 (class_key, id_key,
					  apple_event, reply);
	}
      else
	err = errAEEventNotHandled;
    }
  if (err == noErr)
    return noErr;
  else
    return errAEEventNotHandled;
}

static int
cleanup_suspended_apple_events(struct suspended_ae_info **head, int all_p)
{
  UInt32 current_tick = TickCount (), nresumed = 0;
  struct suspended_ae_info *p, *next;

  for (p = *head; p; p = next)
    {
      if (!all_p && p->expiration_tick > current_tick)
	break;
      AESetTheCurrentEvent (&p->apple_event);
      AEResumeTheCurrentEvent (&p->apple_event, &p->reply,
			       (AEEventHandlerUPP) kAENoDispatch, 0);
      AEDisposeDesc (&p->reply);
      AEDisposeDesc (&p->apple_event);
      nresumed++;
      next = p->next;
      xfree (p);
    }
  *head = p;

  return nresumed;
}

#ifndef _EMACS_MACTERM_H
static
#endif /* !_EMACS_MACTERM_H */
void cleanup_all_suspended_apple_events(void)
{
  cleanup_suspended_apple_events (&deferred_apple_events, 1);
  cleanup_suspended_apple_events (&suspended_apple_events, 1);
}

void init_apple_event_handler(void)
{
  OSErr err;
  long result;

  /* Make sure we have Apple events before starting: */
  err = Gestalt(gestaltAppleEventsAttr, (SInt32 *)&result);
  if (err != noErr)
    abort ();

  if (!(result & (1 << gestaltAppleEventsPresent)))
    abort ();

  err = AEInstallEventHandler (typeWildCard, typeWildCard,
#if TARGET_API_MAC_CARBON
			       NewAEEventHandlerUPP (mac_handle_apple_event),
#else
			       NewAEEventHandlerProc (mac_handle_apple_event),
#endif /* TARGET_API_MAC_CARBON */
			       0L, false);
  if (err != noErr)
    abort ();

  atexit (cleanup_all_suspended_apple_events);
}

static UInt32 get_suspension_id(Lisp_Object apple_event)
{
  Lisp_Object tem;

  CHECK_CONS (apple_event);
  CHECK_STRING_CAR (apple_event);
  if ((SBYTES(XCAR(apple_event)) != 4)
      || (strcmp((const char *)SDATA(XCAR(apple_event)), "aevt") != 0))
    error ("Not an apple event");

  tem = assq_no_quit (Qemacs_suspension_id, XCDR (apple_event));
  if (NILP (tem))
    error ("Suspension ID not available");

  tem = XCDR(tem);
  if (!(CONSP(tem)
	&& (STRINGP(XCAR(tem)) && (SBYTES(XCAR(tem)) == 4))
	&& (strcmp((const char *)SDATA(XCAR(tem)), "magn") == 0)
	&& (STRINGP(XCDR(tem)) && (SBYTES(XCDR(tem)) == 4))))
    error ("Bad suspension ID format");

  return *((UInt32 *) SDATA (XCDR (tem)));
}


DEFUN ("mac-process-deferred-apple-events", Fmac_process_deferred_apple_events, Smac_process_deferred_apple_events, 0, 0, 0,
       doc: /* Process Apple events that are deferred at the startup time.  */)
  (void)
{
  if (mac_ready_for_apple_events)
    return Qnil;

  BLOCK_INPUT;
  mac_ready_for_apple_events = 1;
  if (deferred_apple_events)
    {
      struct suspended_ae_info *prev, *tail, *next;

      /* `nreverse' deferred_apple_events.  */
      prev = NULL;
      for (tail = deferred_apple_events; tail; tail = next)
	{
	  next = tail->next;
	  tail->next = prev;
	  prev = tail;
	}

      /* Now `prev' points to the first cell.  */
      for (tail = prev; tail; tail = next)
	{
	  next = tail->next;
	  AEResumeTheCurrentEvent (&tail->apple_event, &tail->reply,
				   ((AEEventHandlerUPP)
				    kAEUseStandardDispatch), 0);
	  AEDisposeDesc (&tail->reply);
	  AEDisposeDesc (&tail->apple_event);
	  xfree (tail);
	}

      deferred_apple_events = NULL;
    }
  UNBLOCK_INPUT;

  return Qt;
}

DEFUN ("mac-cleanup-expired-apple-events", Fmac_cleanup_expired_apple_events, Smac_cleanup_expired_apple_events, 0, 0, 0,
       doc: /* Clean up expired Apple events.
Return the number of expired events.   */)
     (void)
{
  int nexpired;

  BLOCK_INPUT;
  nexpired = cleanup_suspended_apple_events (&suspended_apple_events, 0);
  UNBLOCK_INPUT;

  return make_number (nexpired);
}

DEFUN ("mac-ae-set-reply-parameter", Fmac_ae_set_reply_parameter, Smac_ae_set_reply_parameter, 3, 3, 0,
       doc: /* Set parameter KEYWORD to DESCRIPTOR on reply of APPLE-EVENT.
KEYWORD is a 4-byte string.  DESCRIPTOR is a Lisp representation of an
Apple event descriptor.  It has the form of (TYPE . DATA), where TYPE
is a 4-byte string.  Valid format of DATA is as follows:

  * If TYPE is "null", then DATA is nil.
  * If TYPE is "list", then DATA is a list (DESCRIPTOR1 ... DESCRIPTORn).
  * If TYPE is "reco", then DATA is a list ((KEYWORD1 . DESCRIPTOR1)
    ... (KEYWORDn . DESCRIPTORn)).
  * If TYPE is "aevt", then DATA is ignored and the descriptor is
    treated as null.
  * Otherwise, DATA is a string.

If a (sub-)descriptor is in an invalid format, it is silently treated
as null.

Return t if the parameter is successfully set.  Otherwise return nil.  */)
     (Lisp_Object apple_event, Lisp_Object keyword, Lisp_Object descriptor)
{
  Lisp_Object result = Qnil;
  UInt32 suspension_id;
  struct suspended_ae_info *p;

  suspension_id = get_suspension_id (apple_event);

  CHECK_STRING (keyword);
  if (SBYTES (keyword) != 4)
    error ("Apple event keyword must be a 4-byte string: %s",
	   SDATA (keyword));

  BLOCK_INPUT;
  for (p = suspended_apple_events; p; p = p->next)
    if (p->suspension_id == suspension_id)
      break;
  if (p && p->reply.descriptorType != typeNull)
    {
      OSErr err;

      err = mac_ae_put_lisp (&p->reply,
			     EndianU32_BtoN (*((UInt32 *) SDATA (keyword))),
			     descriptor);
      if (err == noErr)
	result = Qt;
    }
  UNBLOCK_INPUT;

  return result;
}

DEFUN ("mac-resume-apple-event", Fmac_resume_apple_event, Smac_resume_apple_event, 1, 2, 0,
       doc: /* Resume handling of APPLE-EVENT.
Every Apple event handled by the Lisp interpreter is suspended first.
This function resumes such a suspended event either to complete Apple
event handling to give a reply, or to redispatch it to other handlers.

If optional ERROR-CODE is an integer, it specifies the error number
that is set in the reply.  If ERROR-CODE is t, the resumed event is
handled with the standard dispatching mechanism, but it is not handled
by Emacs again, thus it is redispatched to other handlers.

Return t if APPLE-EVENT is successfully resumed.  Otherwise return
nil, which means the event is already resumed or expired.  */)
     (Lisp_Object apple_event, Lisp_Object error_code)
{
  Lisp_Object result = Qnil;
  UInt32 suspension_id;
  struct suspended_ae_info **p, *ae;

  suspension_id = get_suspension_id (apple_event);

  BLOCK_INPUT;
  for (p = &suspended_apple_events; *p; p = &(*p)->next)
    if ((*p)->suspension_id == suspension_id)
      break;
  if (*p)
    {
      ae = *p;
      *p = (*p)->next;
      if (INTEGERP(error_code)
	  && ae->reply.descriptorType != typeNull)
	{
	  SInt32 errn = XINT(error_code);

	  AEPutParamPtr(&ae->reply, keyErrorNumber, typeSInt32,
                        &errn, sizeof(SInt32));
	}
      AESetTheCurrentEvent(&ae->apple_event);
      AEResumeTheCurrentEvent(&ae->apple_event, &ae->reply,
			      ((AEEventHandlerUPP)(intptr_t)
                               (EQ(error_code, Qt) ?
                                kAEUseStandardDispatch : kAENoDispatch)),
			      0);
      AEDisposeDesc(&ae->reply);
      AEDisposeDesc(&ae->apple_event);
      xfree(ae);
      result = Qt;
    }
  UNBLOCK_INPUT;

  return result;
}


/***********************************************************************
                      Drag and drop support
***********************************************************************/
#if TARGET_API_MAC_CARBON
# ifndef EMACS_GLOBALS_H
static Lisp_Object Vmac_dnd_known_types;
# endif /* !EMACS_GLOBALS_H */
static pascal OSErr mac_do_track_drag P_((DragTrackingMessage, WindowRef,
					  void *, DragRef));
static pascal OSErr mac_do_receive_drag P_((WindowRef, void *, DragRef));
static DragTrackingHandlerUPP mac_do_track_dragUPP = NULL;
static DragReceiveHandlerUPP mac_do_receive_dragUPP = NULL;

extern void mac_store_drag_event P_((WindowRef, Point, SInt16,
				     const AEDesc *));

static pascal OSErr mac_do_track_drag(DragTrackingMessage message,
                                      WindowRef window, void *refcon,
                                      DragRef drag)
{
  OSErr err = noErr;
  static int can_accept;
  UInt16 num_items, index;

  if (GetFrontWindowOfClass(kMovableModalWindowClass, false))
    return dragNotAcceptedErr;

  switch (message)
    {
    case kDragTrackingEnterHandler:
      err = CountDragItems(drag, &num_items);
      if (err != noErr)
	break;
      can_accept = 0;
      for (index = 1; index <= num_items; index++)
	{
	  ItemReference item;
	  FlavorFlags flags;
	  Lisp_Object rest;

	  err = GetDragItemReferenceNumber(drag, index, &item);
	  if (err != noErr)
	    continue;
	  for (rest = Vmac_dnd_known_types; CONSP(rest); rest = XCDR(rest))
	    {
	      Lisp_Object str;
	      FlavorType type;

	      str = XCAR(rest);
	      if (!(STRINGP(str) && (SBYTES(str) == 4)))
		continue;
	      type = EndianU32_BtoN(*((UInt32 *)SDATA(str)));

	      err = GetFlavorFlags(drag, item, type, &flags);
	      if (err == noErr)
		{
		  can_accept = 1;
		  break;
		}
	    }
	}
      break;

    case kDragTrackingEnterWindow:
      if (can_accept)
	{
	  RgnHandle hilite_rgn = NewRgn();

	  if (hilite_rgn)
	    {
	      Rect r;

	      GetWindowPortBounds(window, &r);
	      OffsetRect(&r, -r.left, -r.top);
	      RectRgn(hilite_rgn, &r);
	      ShowDragHilite(drag, hilite_rgn, true);
	      DisposeRgn(hilite_rgn);
	    }
	  SetThemeCursor(kThemeCopyArrowCursor);
	}
      break;

    case kDragTrackingInWindow:
      break;

    case kDragTrackingLeaveWindow:
      if (can_accept)
	{
	  HideDragHilite(drag);
	  SetThemeCursor(kThemeArrowCursor);
	}
      break;

    case kDragTrackingLeaveHandler:
      break;

    default:
      break;
    }

  if (err != noErr)
    return dragNotAcceptedErr;
  return noErr;
}

static pascal OSErr
mac_do_receive_drag(WindowRef window, void *refcon, DragRef drag)
{
  OSErr err;
  int num_types, i;
  Lisp_Object rest, str;
  FlavorType *types;
  AppleEvent apple_event;
  Point mouse_pos;
  SInt16 modifiers;

  if (GetFrontWindowOfClass(kMovableModalWindowClass, false))
    return dragNotAcceptedErr;

  num_types = 0;
  for (rest = Vmac_dnd_known_types; CONSP(rest); rest = XCDR(rest))
    {
      str = XCAR(rest);
      if (STRINGP(str) && (SBYTES(str) == 4))
	num_types++;
    }

  types = (FlavorType *)xmalloc(sizeof(FlavorType) * num_types);
  i = 0;
  for (rest = Vmac_dnd_known_types; CONSP(rest); rest = XCDR(rest))
    {
      str = XCAR(rest);
      if (STRINGP(str) && (SBYTES(str) == 4))
	types[i++] = EndianU32_BtoN(*((UInt32 *)SDATA(str)));
    }

  err = create_apple_event_from_drag_ref(drag, num_types, types,
					 &apple_event);
  xfree (types);

  if (err == noErr)
    err = GetDragMouse(drag, &mouse_pos, NULL);
  if (err == noErr)
    {
      GlobalToLocal(&mouse_pos);
      err = GetDragModifiers(drag, NULL, NULL, &modifiers);
    }
  if (err == noErr)
    {
      UInt32 key_modifiers = modifiers;

      err = AEPutParamPtr(&apple_event, kEventParamKeyModifiers,
			  typeUInt32, &key_modifiers, sizeof(UInt32));
    }

  if (err == noErr)
    {
      mac_store_drag_event(window, mouse_pos, 0, &apple_event);
      AEDisposeDesc(&apple_event);
      mac_wakeup_from_rne();
      return noErr;
    }
  else
    return dragNotAcceptedErr;
}
#endif	/* TARGET_API_MAC_CARBON */

OSErr install_drag_handler(WindowRef window)
{
  OSErr err = noErr;

#if TARGET_API_MAC_CARBON
  if (mac_do_track_dragUPP == NULL)
    mac_do_track_dragUPP = NewDragTrackingHandlerUPP(mac_do_track_drag);
  if (mac_do_receive_dragUPP == NULL)
    mac_do_receive_dragUPP = NewDragReceiveHandlerUPP(mac_do_receive_drag);

  err = InstallTrackingHandler(mac_do_track_dragUPP, window, NULL);
  if (err == noErr)
    err = InstallReceiveHandler(mac_do_receive_dragUPP, window, NULL);
#endif /* TARGET_API_MAC_CARBON */

  return err;
}

void remove_drag_handler(WindowRef window)
{
#if TARGET_API_MAC_CARBON
  if (mac_do_track_dragUPP)
    RemoveTrackingHandler(mac_do_track_dragUPP, window);
  if (mac_do_receive_dragUPP)
    RemoveReceiveHandler(mac_do_receive_dragUPP, window);
#endif /* TARGET_API_MAC_CARBON */
}


/***********************************************************************
			Services menu support
***********************************************************************/
#ifdef MAC_OSX
void init_service_handler(void)
{
  static const EventTypeSpec specs[] =
    {{kEventClassService, kEventServiceGetTypes},
     {kEventClassService, kEventServiceCopy},
     {kEventClassService, kEventServicePaste},
     {kEventClassService, kEventServicePerform}};
  InstallApplicationEventHandler(NewEventHandlerUPP(mac_handle_service_event),
				 GetEventTypeCount(specs), specs, NULL, NULL);
}

extern OSStatus mac_store_service_event P_((EventRef));

static OSStatus
copy_scrap_flavor_data(ScrapRef from_scrap, ScrapRef to_scrap,
                       ScrapFlavorType flavor_type)
{
  OSStatus err;
  Size size, size_allocated;
  char *buf = NULL;

  err = GetScrapFlavorSize(from_scrap, flavor_type, &size);
  if (err == noErr)
    buf = (char *)xmalloc(size);
  while (buf)
    {
      size_allocated = size;
      err = GetScrapFlavorData(from_scrap, flavor_type, &size, buf);
      if (err != noErr)
	{
	  xfree(buf);
	  buf = NULL;
	}
      else if (size_allocated < size)
	buf = xrealloc(buf, size);
      else
	break;
    }
  if (err == noErr)
    {
      if (buf == NULL)
	err = memFullErr;
      else
	{
	  err = PutScrapFlavor (to_scrap, flavor_type, kScrapFlavorMaskNone,
				size, buf);
	  xfree (buf);
	}
    }

  return err;
}

static OSStatus
mac_handle_service_event(EventHandlerCallRef call_ref, EventRef event,
                         void *data)
{
  OSStatus err = noErr;
  ScrapRef cur_scrap, specific_scrap;
  UInt32 event_kind = GetEventKind (event);
  CFMutableArrayRef copy_types, paste_types;
  CFStringRef type;
  Lisp_Object rest;
  ScrapFlavorType flavor_type;

  /* Check if Vmac_service_selection is a valid selection that has a
     corresponding scrap.  */
  if (!SYMBOLP (Vmac_service_selection))
    err = eventNotHandledErr;
  else
    err = get_scrap_from_symbol (Vmac_service_selection, 0, &cur_scrap);
  if (!(err == noErr && cur_scrap))
    return eventNotHandledErr;

  switch (event_kind)
    {
    case kEventServiceGetTypes:
      /* Set paste types. */
      err = GetEventParameter (event, kEventParamServicePasteTypes,
			       typeCFMutableArrayRef, NULL,
			       sizeof (CFMutableArrayRef), NULL,
			       &paste_types);
      if (err != noErr)
	break;

      for (rest = Vselection_converter_alist; CONSP (rest);
	   rest = XCDR (rest))
	if (CONSP (XCAR (rest)) && SYMBOLP (XCAR (XCAR (rest)))
	    && (flavor_type =
		get_flavor_type_from_symbol (XCAR (XCAR (rest)))))
	  {
	    type = CreateTypeStringWithOSType (flavor_type);
	    if (type)
	      {
		CFArrayAppendValue (paste_types, type);
		CFRelease (type);
	      }
	  }

      /* Set copy types.  */
      err = GetEventParameter (event, kEventParamServiceCopyTypes,
			       typeCFMutableArrayRef, NULL,
			       sizeof (CFMutableArrayRef), NULL,
			       &copy_types);
      if (err != noErr)
	break;

      if (NILP(Fx_selection_owner_p(Vmac_service_selection,
                                    LISP_INITIALLY_ZERO)))
	break;
      else
	goto copy_all_flavors;

    case kEventServiceCopy:
      err = GetEventParameter(event, kEventParamScrapRef,
			      typeScrapRef, NULL,
			      sizeof(ScrapRef), NULL, &specific_scrap);
      if ((err != noErr)
	  || NILP(Fx_selection_owner_p(Vmac_service_selection,
                                       LISP_INITIALLY_ZERO)))
	{
	  err = eventNotHandledErr;
	  break;
	}

    copy_all_flavors:
      {
	UInt32 count, i;
	ScrapFlavorInfo *flavor_info = (ScrapFlavorInfo *)NULL;
	ScrapFlavorFlags flags;

	err = GetScrapFlavorCount(cur_scrap, &count);
	if (err == noErr)
	  flavor_info = ((ScrapFlavorInfo *)
                         xmalloc(sizeof(ScrapFlavorInfo) * count));
	err = GetScrapFlavorInfoList(cur_scrap, &count, flavor_info);
	if (err != noErr)
	  {
	    xfree (flavor_info);
	    flavor_info = NULL;
	  }
	if (flavor_info == NULL)
	  break;

	for (i = 0; i < count; i++)
	  {
	    flavor_type = flavor_info[i].flavorType;
	    err = GetScrapFlavorFlags (cur_scrap, flavor_type, &flags);
	    if (err == noErr && !(flags & kScrapFlavorMaskSenderOnly))
	      {
		if (event_kind == kEventServiceCopy)
		  err = copy_scrap_flavor_data (cur_scrap, specific_scrap,
						flavor_type);
		else	     /* event_kind == kEventServiceGetTypes */
		  {
		    type = CreateTypeStringWithOSType(flavor_type);
		    if (type)
		      {
			CFArrayAppendValue(copy_types, type);
			CFRelease(type);
		      }
		  }
	      }
	  }
	xfree (flavor_info);
      }
      break;

    case kEventServicePaste:
    case kEventServicePerform:
      {
	int data_exists_p = 0;

        err = GetEventParameter (event, kEventParamScrapRef, typeScrapRef,
				 NULL, sizeof (ScrapRef), NULL,
				 &specific_scrap);
	if (err == noErr)
	  err = clear_scrap (&cur_scrap);
	if (err == noErr)
	  for (rest = Vselection_converter_alist; CONSP (rest);
	       rest = XCDR (rest))
	    {
	      if (! (CONSP (XCAR (rest)) && SYMBOLP (XCAR (XCAR (rest)))))
		continue;
	      flavor_type = get_flavor_type_from_symbol (XCAR (XCAR (rest)));
	      if (flavor_type == 0)
		continue;
	      err = copy_scrap_flavor_data (specific_scrap, cur_scrap,
					    flavor_type);
	      if (err == noErr)
		data_exists_p = 1;
	    }
	if (!data_exists_p)
	  err = eventNotHandledErr;
	else
	  err = mac_store_service_event (event);
      }
      break;

    default:
      break;
    }

  if (err != noErr)
    err = eventNotHandledErr;
  return err;
}
#endif /* MAC_OSX */

/* getting near the end now... */
void syms_of_macselect(void)
{
  defsubr (&Sx_get_selection_internal);
  defsubr (&Sx_own_selection_internal);
  defsubr (&Sx_disown_selection_internal);
  defsubr (&Sx_selection_owner_p);
  defsubr (&Sx_selection_exists_p);
  defsubr (&Smac_process_deferred_apple_events);
  defsubr (&Smac_cleanup_expired_apple_events);
  defsubr (&Smac_resume_apple_event);
  defsubr (&Smac_ae_set_reply_parameter);

  Vselection_alist = Qnil;
  staticpro (&Vselection_alist);

  DEFVAR_LISP ("selection-converter-alist", Vselection_converter_alist,
	       doc: /* An alist associating selection-types with functions.
These functions are called to convert the selection, with three args:
the name of the selection (typically `PRIMARY', `SECONDARY', or `CLIPBOARD');
a desired type to which the selection should be converted;
and the local selection value (whatever was given to `x-own-selection').

The function should return the value to send to the Scrap Manager
\(must be a string).  A return value of nil
means that the conversion could not be done.  */);
  Vselection_converter_alist = Qnil;

  DEFVAR_LISP ("x-lost-selection-functions", Vx_lost_selection_functions,
	       doc: /* A list of functions to be called when Emacs loses a selection.
\(This happens when a Lisp program explicitly clears the selection.)
The functions are called with one argument, the selection type
\(a symbol, typically `PRIMARY', `SECONDARY', or `CLIPBOARD').  */);
  Vx_lost_selection_functions = Qnil;

  DEFVAR_LISP ("selection-coding-system", Vselection_coding_system,
	       doc: /* Coding system for communicating with other programs.
When sending or receiving text via cut_buffer, selection, and clipboard,
the text is encoded or decoded by this coding system.
The default value is determined by the system script code.  */);
  Vselection_coding_system = Qnil;

  DEFVAR_LISP ("next-selection-coding-system", Vnext_selection_coding_system,
	       doc: /* Coding system for the next communication with other programs.
Usually, `selection-coding-system' is used for communicating with
other programs.  But, if this variable is set, it is used for the
next communication only.  After the communication, this variable is
set to nil.  */);
  Vnext_selection_coding_system = Qnil;

  DEFVAR_LISP ("mac-apple-event-map", Vmac_apple_event_map,
	       doc: /* Keymap for Apple events handled by Emacs.  */);
  Vmac_apple_event_map = Qnil;

#if TARGET_API_MAC_CARBON
  DEFVAR_LISP ("mac-dnd-known-types", Vmac_dnd_known_types,
	       doc: /* The types accepted by default for dropped data.
The types are chosen in the order they appear in the list.  */);
  Vmac_dnd_known_types = list4 (build_string ("hfs "), build_string ("utxt"),
				build_string ("TEXT"), build_string ("TIFF"));
# ifdef MAC_OSX
  Vmac_dnd_known_types = Fcons (build_string ("furl"), Vmac_dnd_known_types);
# endif /* MAC_OSX */
#endif /* TARGET_API_MAC_CARBON */

#ifdef MAC_OSX
  DEFVAR_LISP ("mac-service-selection", Vmac_service_selection,
	       doc: /* Selection name for communication via Services menu.  */);
  Vmac_service_selection = intern ("PRIMARY");
#endif /* MAC_OSX */

  QPRIMARY   = intern("PRIMARY");	staticpro(&QPRIMARY);
  QSECONDARY = intern("SECONDARY");	staticpro(&QSECONDARY);
  QTIMESTAMP = intern("TIMESTAMP");	staticpro(&QTIMESTAMP);
  QTARGETS   = intern("TARGETS");	staticpro(&QTARGETS);

  Qforeign_selection = intern("foreign-selection");
  staticpro(&Qforeign_selection);

  Qmac_scrap_name = intern("mac-scrap-name");
  staticpro(&Qmac_scrap_name);

  Qmac_ostype = intern("mac-ostype");
  staticpro(&Qmac_ostype);

  Qmac_apple_event_class = intern("mac-apple-event-class");
  staticpro(&Qmac_apple_event_class);

  Qmac_apple_event_id = intern("mac-apple-event-id");
  staticpro(&Qmac_apple_event_id);

  Qemacs_suspension_id = intern("emacs-suspension-id");
  staticpro(&Qemacs_suspension_id);
}

/* arch-tag: f3c91ad8-99e0-4bd6-9eef-251b2f848732
   (do not change this comment) */
