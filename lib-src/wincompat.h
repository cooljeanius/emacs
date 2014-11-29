/* wincompat.h */
/* misc. compatibility code in case a Windows source file somehow gets
 * pulled in on a non-Windows platform */

#ifndef _WINCOMPAT_H
#define _WINCOMPAT_H 1

#ifdef __cplusplus
# include "../types.h"
#else
# if !defined(BOOL) && !(defined(__WINESRC__) && defined(WINDOWS_COMPATIBILITY_CHECK))
#  if !defined(OBJ_C) && !defined(__OBJC__) && !defined(__APPLE__)
typedef int BOOL;
#  else
/* Apple also defines BOOL: */
typedef int emacs_local_BOOL;
#   define BOOL emacs_local_BOOL
#  endif /* !OBJ_C && !__OBJC__ */
# endif /* !BOOL && !(__WINESRC__ && WINDOWS_COMPATIBILITY_CHECK) */
# if !defined(WINAPI) && !(defined(__WINESRC__) && defined(WINDOWS_COMPATIBILITY_CHECK))
#  define WINAPI /* (nothing) */
# endif /* !WINAPI && !(__WINESRC__ && WINDOWS_COMPATIBILITY_CHECK) */
#endif /* __cplusplus */

#if defined(HAVE_FCNTL_H)
# include <fcntl.h>
#endif /* HAVE_FCNTL_H */

#if defined(HAVE_STDARG_H) || (defined(__STDC__) && (defined(__GNUC__) || defined(__clang__))) || (defined(__APPLE__) && defined(__clang__))
# include <stdarg.h>
#endif /* HAVE_STDARG_H || (__STDC__ && (__GNUC__ || __clang__)) || (__APPLE__ && __clang__) */

#if defined(HAVE_STDBOOL_H) || (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L))
# include <stdbool.h>
#endif /* HAVE_STDBOOL_H || C99 */

#if defined(HAVE_STDDEF_H) || defined(STDC_HEADERS) || defined(__STDC__)
# include <stddef.h>
#endif /* HAVE_STDDEF_H || STDC_HEADERS || __STDC__ */

#if defined(HAVE_BASETSD_H) || (defined(__WINESRC__) && defined(WINDOWS_COMPATIBILITY_CHECK))
# include <basetsd.h>
#else
/* from Wine's <basetsd.h>: */
# if defined(__midl) || defined(__WIDL__)
#  if !defined(INT_PTR)
typedef /* [public] */ signed __int3264 INT_PTR;
#  endif /* !INT_PTR */
#  if !defined(UINT_PTR)
typedef /* [public] */ unsigned __int3264 UINT_PTR;
#  endif /* !UINT_PTR */
#  if !defined(LONG_PTR)
typedef /* [public] */ signed __int3264 LONG_PTR;
#  endif /* !LONG_PTR */
#  if !defined(ULONG_PTR)
typedef /* [public] */ unsigned __int3264 ULONG_PTR;
#  endif /* !ULONG_PTR */
#  ifndef DWORD_PTR
typedef ULONG_PTR DWORD_PTR;
#  endif /* !DWORD_PTR */
# elif defined(_WIN64)
#  if !defined(INT_PTR)
typedef signed __int64 INT_PTR;
#  endif /* !INT_PTR */
#  if !defined(UINT_PTR)
typedef unsigned __int64 UINT_PTR;
#  endif /* !UINT_PTR */
#  if !defined(LONG_PTR)
typedef signed __int64 LONG_PTR;
#  endif /* !LONG_PTR */
#  if !defined(ULONG_PTR)
typedef unsigned __int64 ULONG_PTR;
#  endif /* !ULONG_PTR */
#  ifndef DWORD_PTR
typedef ULONG_PTR DWORD_PTR;
#  endif /* !DWORD_PTR */
# else
#  if !defined(INT_PTR)
typedef long INT_PTR;
#  endif /* !INT_PTR */
#  if !defined(UINT_PTR)
typedef unsigned long UINT_PTR;
#  endif /* !UINT_PTR */
#  if !defined(LONG_PTR)
typedef long LONG_PTR;
#  endif /* !LONG_PTR */
#  if !defined(ULONG_PTR)
typedef unsigned long ULONG_PTR;
#  endif /* !ULONG_PTR */
#  ifndef DWORD_PTR
typedef ULONG_PTR DWORD_PTR;
#  endif /* !DWORD_PTR */
# endif /* (__midl || __WIDL__) || _WIN64 */
#endif /* HAVE_BASETSD_H || (__WINESRC__ && WINDOWS_COMPATIBILITY_CHECK) */

#if (defined(__clang__) || defined(__llvm__) || defined(__GNUC__)) && defined(WINAPI) && (!defined(_WINDEF_) && defined(__WINE_WINBASE_H))
# undef WINAPI
#endif /* (__clang__ || __llvm__ || __GNUC__) && WINAPI && (!_WINDEF_ && __WINE_WINBASE_H) */

#if defined(HAVE_WINDEF_H) || (defined(__WINESRC__) && defined(WINDOWS_COMPATIBILITY_CHECK))
# include <windef.h>
#else
/* from Wine's <windef.h>: */
# ifndef CALLBACK
#  ifdef __stdcall
#   define CALLBACK __stdcall
#  else
#   define CALLBACK /* (nothing) */
#  endif /* __stdcall */
# endif /* !CALLBACK */
# ifndef LPVOID
typedef void *LPVOID;
# endif /* !LPVOID */
# ifndef WORD
typedef unsigned short WORD;
# endif /* !WORD */
# ifndef DWORD
#  ifdef _MSC_VER
typedef unsigned long DWORD;
#  else
typedef unsigned int DWORD;
#  endif /* _MSC_VER */
# endif /* !DWORD */
# ifndef MAKELONG
#  define MAKELONG(low,high) ((LONG)(((WORD)((DWORD_PTR)(low) & 0xFFFF)) | ((DWORD)((WORD)((DWORD_PTR)(high) & 0xFFFF))) << 16))
# endif /* !MAKELONG */
# ifndef INT
typedef int INT;
# endif /* !INT */
# ifndef UINT
typedef unsigned int UINT;
# endif /* !UINT */
# ifndef COLORREF
typedef DWORD COLORREF;
# endif /* !COLORREF */
# ifndef WPARAM
typedef UINT_PTR WPARAM;
# endif /* !WPARAM */
# ifndef LPARAM
typedef LONG_PTR LPARAM;
# endif /* !LPARAM */
# ifndef LRESULT
typedef LONG_PTR LRESULT;
# endif /* !LRESULT */
# ifdef __WINESRC__
#  ifndef WINE_NO_UNICODE_MACROS
#   define WINE_NO_UNICODE_MACROS 1
#  endif /* !WINE_NO_UNICODE_MACROS */
#  ifndef WINE_STRICT_PROTOTYPES
#   define WINE_STRICT_PROTOTYPES 1
#  endif /* WINE_STRICT_PROTOTYPES */
# endif /* _WINESRC__ */
# ifndef WINELIB_NAME_AW
#  ifdef WINE_NO_UNICODE_MACROS
#   define WINELIB_NAME_AW(func) \
       func##_must_be_suffixed_with_W_or_A_in_this_context \
       func##_must_be_suffixed_with_W_or_A_in_this_context
#  else  /* WINE_NO_UNICODE_MACROS */
#   ifdef UNICODE
#    define WINELIB_NAME_AW(func) func##W
#   else
#    define WINELIB_NAME_AW(func) func##A
#   endif /* UNICODE */
#  endif  /* WINE_NO_UNICODE_MACROS */
# endif /* !WINELIB_NAME_AW */
# ifndef DECL_WINELIB_TYPE_AW
#  ifdef WINE_NO_UNICODE_MACROS
#   define DECL_WINELIB_TYPE_AW(type)  /* nothing */
#  else
#   define DECL_WINELIB_TYPE_AW(type) typedef WINELIB_NAME_AW(type) type;
#  endif /* WINE_NO_UNICODE_MACROS */
# endif /* !DECL_WINELIB_TYPE_AW */
# ifndef FARPROC
#  if defined __cplusplus
extern "C" {
#  endif /* __cplusplus */
#  if defined(WINE_STRICT_PROTOTYPES) || defined(__STRICT_ANSI__) || \
      defined(__PROTOTYPES) || defined(PROTOTYPES) || \
      (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199409L))
typedef INT_PTR (CALLBACK *FARPROC)(void);
#  else
typedef INT_PTR (CALLBACK *FARPROC)();
#  endif /* WINE_STRICT_PROTOTYPES || (ansi) || (protos) || (c95+) */
#  if defined __cplusplus
}
#  endif /* __cplusplus */
# endif /* !FARPROC */
/* 'DECLARE_HANDLE()' declarations are confusing; just use regular
 * typedefs here instead: */
# ifndef HBITMAP
typedef void *HBITMAP;
# endif /* !HBITMAP */
# ifndef HDC
typedef void *HDC;
# endif /* !HDC */
# ifndef HFONT
typedef void *HFONT;
# endif /* !HFONT */
# ifndef HICON
typedef void *HICON;
# endif /* !HICON */
# ifndef HINSTANCE
typedef void *HINSTANCE;
# endif /* !HINSTANCE */
# ifndef HMENU
typedef void *HMENU;
# endif /* !HMENU */
# ifndef HWND
typedef void *HWND;
# endif /* !HWND */
#endif /* WINDEF_H || (__WINESRC__ && WINDOWS_COMPATIBILITY_CHECK) */

#ifndef _NTSYSTEM_
# define _NTSYSTEM_
#endif /* !_NTSYSTEM_ */

#if defined(HAVE_WINNT_H) || (defined(__WINESRC__) && defined(WINDOWS_COMPATIBILITY_CHECK))
# include <winnt.h>
#else
/* from Wine's <winnt.h>: */
# ifndef FILE_ATTRIBUTE_READONLY
#  define FILE_ATTRIBUTE_READONLY 0x00000001
# endif /* !FILE_ATTRIBUTE_READONLY */
# ifndef FILE_ATTRIBUTE_DIRECTORY
#  define FILE_ATTRIBUTE_DIRECTORY 0x00000010
# endif /* !FILE_ATTRIBUTE_DIRECTORY */
# ifndef LONG
#  ifdef _MSC_VER
typedef long LONG;
#  else
typedef int LONG;
#  endif /* _MSC_VER */
# endif /* !LONG */
/* these next two are actually from Wine's <windef.h> again, but we need
 * to put them after we have LONG: */
# if !defined(tagRECT) && !defined(RECT) && !defined(LPRECT) && !defined(_WINDEF_)
typedef struct tagRECT
{
    LONG left;
    LONG top;
    LONG right;
    LONG bottom;
} RECT, *LPRECT;
# endif /* !tagRECT && !RECT */
# if !defined(tagSIZE) && !defined(LPSIZE) && !defined(_WINDEF_)
typedef struct tagSIZE
{
    LONG cx;
    LONG cy;
} *LPSIZE;
# endif /* !tagSIZE && !LPSIZE */
# ifndef HANDLE
typedef void *HANDLE;
# endif /* !HANDLE */
# ifndef CHAR
typedef char CHAR;
# endif /* !CHAR */
# ifndef WCHAR
#  if defined(WINE_UNICODE_NATIVE) || defined(HAVE_WCHAR_T) || defined(_WCHAR_T)
typedef wchar_t WCHAR;
#  else
typedef unsigned short WCHAR;
#  endif /* WINE_UNICODE_NATIVE || HAVE_WCHAR_T || _WCHAR_T */
# endif /* !WCHAR */
# ifndef LPSTR
typedef CHAR *LPSTR;
# endif /* !LPSTR */
# ifndef LPWSTR
typedef WCHAR *LPWSTR;
# endif /* !LPWSTR */
# ifndef LPCSTR
typedef const CHAR *LPCSTR;
# endif /* !LPCSTR */
# ifndef LPCWSTR
typedef const WCHAR *LPCWSTR;
# endif /* !LPCWSTR */
# if !defined(_LIST_ENTRY) && !defined(LIST_ENTRY)
typedef struct _LIST_ENTRY {
    struct _LIST_ENTRY *Flink;
    struct _LIST_ENTRY *Blink;
} LIST_ENTRY;
# endif /* !_LIST_ENTRY && !LIST_ENTRY */
# if !defined(_RTL_CRITICAL_SECTION_DEBUG) && !defined(PRTL_CRITICAL_SECTION_DEBUG)
typedef struct _RTL_CRITICAL_SECTION_DEBUG
{
    WORD Type;
    WORD CreatorBackTraceIndex;
    struct _RTL_CRITICAL_SECTION *CriticalSection;
    LIST_ENTRY ProcessLocksList;
    DWORD EntryCount;
    DWORD ContentionCount;
#  ifdef __WINESRC__  /* in Wine we store the name here */
    DWORD_PTR Spare[8 / sizeof(DWORD_PTR)];
#  else
    DWORD Spare[2];
#  endif /* __WINESRC__ */
} *PRTL_CRITICAL_SECTION_DEBUG;
# endif /* !_RTL_CRITICAL_SECTION_DEBUG */
# if !defined(_RTL_CRITICAL_SECTION) && !defined(RTL_CRITICAL_SECTION)
typedef struct _RTL_CRITICAL_SECTION {
    PRTL_CRITICAL_SECTION_DEBUG DebugInfo;
    LONG LockCount;
    LONG RecursionCount;
    HANDLE OwningThread;
    HANDLE LockSemaphore;
    ULONG_PTR SpinCount;
}  RTL_CRITICAL_SECTION;
# endif /* !_RTL_CRITICAL_SECTION && !RTL_CRITICAL_SECTION */
# ifndef WIN32_NO_STATUS
#  ifndef STATUS_TIMEOUT
#   define STATUS_TIMEOUT ((DWORD)0x00000102)
#  endif /* !STATUS_TIMEOUT */
# endif /* !WIN32_NO_STATUS */
# ifndef SYNCHRONIZE
#  define SYNCHRONIZE 0x00100000
# endif /* !SYNCHRONIZE */
#endif /* HAVE_WINNT_H || (__WINESRC__ && WINDOWS_COMPATIBILITY_CHECK) */

#if defined(HAVE_WINBASE_H) || (defined(__WINESRC__) && defined(WINDOWS_COMPATIBILITY_CHECK))
# include <winbase.h>
#else
/* from Wine's <winbase.h>: */
# if !defined(FILETIME) && !defined(_FILETIME) && !defined(_FILETIME_)
#  define _FILETIME_ 1
/* 64-bit number of 100-nanosecond intervals since January 1, 1601: */
typedef struct _FILETIME
{
#  ifdef WORDS_BIGENDIAN
    DWORD dwHighDateTime;
    DWORD dwLowDateTime;
#  else
    DWORD dwLowDateTime;
    DWORD dwHighDateTime;
#  endif /* WORDS_BIGENDIAN */
} FILETIME;
# endif /* !FILETIME && !_FILETIME && !_FILETIME_ */
# ifndef FILE_TYPE_CHAR
#  define FILE_TYPE_CHAR 2
# endif /* !FILE_TYPE_CHAR */
# ifndef WINBASEAPI
#  if !defined(_KERNEL32_) && defined(DECLSPEC_IMPORT)
#   define WINBASEAPI DECLSPEC_IMPORT
#  else
#   define WINBASEAPI /* (nothing) */
#  endif /* !_KERNEL32_ && DECLSPEC_IMPORT */
# endif /* !WINBASEAPI */
# ifndef CRITICAL_SECTION
typedef RTL_CRITICAL_SECTION CRITICAL_SECTION;
# endif /* !CRITICAL_SECTION */
# ifndef GetCurrentDirectory
#  if defined __cplusplus
extern "C" {
#  endif /* __cplusplus */
WINBASEAPI UINT WINAPI GetCurrentDirectoryA(UINT,LPSTR);
WINBASEAPI UINT WINAPI GetCurrentDirectoryW(UINT,LPWSTR);
#  define GetCurrentDirectory WINELIB_NAME_AW(GetCurrentDirectory)
#  if defined __cplusplus
}
#  endif /* __cplusplus */
# endif /* !GetCurrentDirectory */
# ifndef GetStdHandle
#  if defined __cplusplus
extern "C" {
#  endif /* __cplusplus */
WINBASEAPI HANDLE WINAPI GetStdHandle(DWORD);
#  if defined __cplusplus
}
#  endif /* __cplusplus */
# endif /* !GetStdHandle */
# ifndef INVALID_HANDLE_VALUE
#  define INVALID_HANDLE_VALUE ((HANDLE)~(ULONG_PTR)0)
# endif /* !INVALID_HANDLE_VALUE */
# ifndef INVALID_FILE_SIZE
#  define INVALID_FILE_SIZE (~0u)
# endif /* !INVALID_FILE_SIZE */
# ifndef INVALID_SET_FILE_POINTER
#  define INVALID_SET_FILE_POINTER (~0u)
# endif /* !INVALID_SET_FILE_POINTER */
# ifndef INVALID_FILE_ATTRIBUTES
#  define INVALID_FILE_ATTRIBUTES (~0u)
# endif /* !INVALID_FILE_ATTRIBUTES */
# ifndef OpenProcess
#  if defined __cplusplus
extern "C" {
#  endif /* __cplusplus */
WINBASEAPI HANDLE WINAPI OpenProcess(DWORD,BOOL,DWORD);
#  if defined __cplusplus
}
#  endif /* __cplusplus */
# endif /* !OpenProcess */
# ifndef STD_INPUT_HANDLE
#  define STD_INPUT_HANDLE ((DWORD)-10)
# endif /* !STD_INPUT_HANDLE */
# ifndef STD_OUTPUT_HANDLE
#  define STD_OUTPUT_HANDLE ((DWORD)-11)
# endif /* !STD_OUTPUT_HANDLE */
# ifndef STD_ERROR_HANDLE
#  define STD_ERROR_HANDLE ((DWORD)-12)
# endif /* !STD_ERROR_HANDLE */
# if !defined(SYSTEMTIME) && !defined(_SYSTEMTIME) && !defined(_SYSTEMTIME_)
#  define _SYSTEMTIME_ 1
typedef struct _SYSTEMTIME
{
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
} SYSTEMTIME;
# endif /* !SYSTEMTIME && !_SYSTEMTIME && !_SYSTEMTIME_ */
# ifndef WAIT_FAILED
#  define WAIT_FAILED 0xffffffff
# endif /* !WAIT_FAILED */
# ifndef WAIT_OBJECT_0
#  define WAIT_OBJECT_0 0
# endif /* !WAIT_OBJECT_0 */
# ifndef WAIT_TIMEOUT
#  ifdef STATUS_TIMEOUT
#   define WAIT_TIMEOUT STATUS_TIMEOUT
#  else
#   define WAIT_TIMEOUT 0x00000102
#  endif /* STATUS_TIMEOUT */
# endif /* !WAIT_TIMEOUT */
# ifndef WIN32_FIND_DATA
/* Find* structures: */
#  if !defined(_WIN32_FIND_DATAA) && !defined(WIN32_FIND_DATAA) && \
      !defined(LPWIN32_FIND_DATAA)
typedef struct _WIN32_FIND_DATAA
{
    DWORD     dwFileAttributes;
    FILETIME  ftCreationTime;
    FILETIME  ftLastAccessTime;
    FILETIME  ftLastWriteTime;
    DWORD     nFileSizeHigh;
    DWORD     nFileSizeLow;
    DWORD     dwReserved0;
    DWORD     dwReserved1;
    CHAR      cFileName[260];
    CHAR      cAlternateFileName[14];
} WIN32_FIND_DATAA, *LPWIN32_FIND_DATAA;
#  endif /* !_WIN32_FIND_DATAA && !WIN32_FIND_DATAA && !LPWIN32_FIND_DATAA */
#  if !defined(_WIN32_FIND_DATAW) && !defined(WIN32_FIND_DATAW) && \
      !defined(LPWIN32_FIND_DATAW)
typedef struct _WIN32_FIND_DATAW
{
    DWORD     dwFileAttributes;
    FILETIME  ftCreationTime;
    FILETIME  ftLastAccessTime;
    FILETIME  ftLastWriteTime;
    DWORD     nFileSizeHigh;
    DWORD     nFileSizeLow;
    DWORD     dwReserved0;
    DWORD     dwReserved1;
    WCHAR     cFileName[260];
    WCHAR     cAlternateFileName[14];
} WIN32_FIND_DATAW, *LPWIN32_FIND_DATAW;
#  endif /* !_WIN32_FIND_DATAW && !WIN32_FIND_DATAW && !LPWIN32_FIND_DATAW */
DECL_WINELIB_TYPE_AW(WIN32_FIND_DATA)
# endif /* !WIN32_FIND_DATA */
/* needs to go after the find things: */
# ifndef FindFirstFile
#  if defined __cplusplus
extern "C" {
#  endif /* __cplusplus */
    WINBASEAPI HANDLE WINAPI FindFirstFileA(LPCSTR,LPWIN32_FIND_DATAA);
    WINBASEAPI HANDLE WINAPI FindFirstFileW(LPCWSTR,LPWIN32_FIND_DATAW);
#define FindFirstFile WINELIB_NAME_AW(FindFirstFile)
#  if defined __cplusplus
}
#  endif /* __cplusplus */
# endif /* !FindFirstFile */
#endif /* HAVE_WINBASE_H || (__WINESRC__ && WINDOWS_COMPATIBILITY_CHECK) */

/* not actually used in our code, but has to come before <winuser.h>
 * is included, to silence some warnings from '-Wundef': */
#if defined(HAVE_WINRESRC_H) || \
    (defined(__WINESRC__) && defined(WINDOWS_COMPATIBILITY_CHECK) && defined(lint))
# include <winresrc.h>
#else
# ifndef _WIN32_WINNT
#  define _WIN32_WINNT 0x0500
# endif /* !_WIN32_WINNT */
#endif /* HAVE_WINRESRC_H || (__WINESRC__ && WINDOWS_COMPATIBILITY_CHECK && lint) */

#if defined(HAVE_WINUSER_H) || (defined(__WINESRC__) && defined(WINDOWS_COMPATIBILITY_CHECK))
# include <winuser.h>
#else
/* from Wine's <winuser.h>: */
# ifndef MAKELPARAM
#  define MAKELPARAM(low,high) ((LPARAM)(DWORD)MAKELONG(low,high))
# endif /* !MAKE_LPARAM */
# if !defined(_USER32_) && defined(DECLSPEC_IMPORT)
#  define WINUSERAPI DECLSPEC_IMPORT
# else
#  define WINUSERAPI /* (nothing) */
# endif /* !_USER32_ && DECLSPEC_IMPORT */
# ifndef MENUITEMINFO
#  if !defined(tagMENUITEMINFOA) && !defined(MENUITEMINFOA)
typedef struct tagMENUITEMINFOA {
    UINT    cbSize;
    UINT    fMask;
    UINT    fType;
    UINT    fState;
    UINT    wID;
    HMENU   hSubMenu;
    HBITMAP hbmpChecked;
    HBITMAP hbmpUnchecked;
    ULONG_PTR dwItemData;
    LPSTR   dwTypeData;
    UINT    cch;
    HBITMAP hbmpItem;
} MENUITEMINFOA;
# endif /* !tagMENUITEMINFOA && !MENUITEMINFOA */
# if !defined(tagMENUITEMINFOW) && !defined(MENUITEMINFOW)
typedef struct tagMENUITEMINFOW {
    UINT    cbSize;
    UINT    fMask;
    UINT    fType;
    UINT    fState;
    UINT    wID;
    HMENU   hSubMenu;
    HBITMAP hbmpChecked;
    HBITMAP hbmpUnchecked;
    ULONG_PTR dwItemData;
    LPWSTR  dwTypeData;
    UINT    cch;
    HBITMAP hbmpItem;
} MENUITEMINFOW;
#  endif /* !tagMENUITEMINFOW && !MENUITEMINFOW */
DECL_WINELIB_TYPE_AW(MENUITEMINFO)
# endif /* !MENUITEMINFO */
# ifndef LPCMENUITEMINFO
typedef const MENUITEMINFOA *LPCMENUITEMINFOA;
typedef const MENUITEMINFOW *LPCMENUITEMINFOW;
DECL_WINELIB_TYPE_AW(LPCMENUITEMINFO)
# endif /* !LPCMENUITEMINFO */
# ifndef DLGPROC
#  if defined(STRICT)
#   if defined __cplusplus
extern "C" {
#   endif /* __cplusplus */
typedef INT_PTR (CALLBACK *DLGPROC)(HWND,UINT,WPARAM,LPARAM);
#   if defined __cplusplus
}
#   endif /* __cplusplus */
#  else
typedef FARPROC DLGPROC;
#  endif /* STRICT */
# endif /* DLGPROC */
# ifndef WNDPROC
#  if defined __cplusplus
extern "C" {
#  endif /* __cplusplus */
typedef LRESULT (CALLBACK *WNDPROC)(HWND,UINT,WPARAM,LPARAM);
#  if defined __cplusplus
}
#  endif /* __cplusplus */
# endif /* !WNDPROC */
# ifndef GetWindowRect
#  if defined __cplusplus
extern "C" {
#  endif /* __cplusplus */
WINUSERAPI BOOL WINAPI GetWindowRect(HWND,LPRECT);
#  if defined __cplusplus
}
#  endif /* __cplusplus */
# endif /* !GetWindowRect */
# ifndef InvalidateRect
#  if defined __cplusplus
extern "C" {
#  endif /* __cplusplus */
WINUSERAPI BOOL WINAPI InvalidateRect(HWND,const RECT*,BOOL);
#  if defined __cplusplus
}
#  endif /* __cplusplus */
# endif /* !InvalidateRect */
# ifndef SendMessage
#  if defined __cplusplus
extern "C" {
#  endif /* __cplusplus */
WINUSERAPI LRESULT WINAPI SendMessageA(HWND,UINT,WPARAM,LPARAM);
WINUSERAPI LRESULT WINAPI SendMessageW(HWND,UINT,WPARAM,LPARAM);
#  define SendMessage WINELIB_NAME_AW(SendMessage)
#  if defined __cplusplus
}
#  endif /* __cplusplus */
# endif /* !SendMessage */
# ifndef SetWindowText
#  if defined __cplusplus
extern "C" {
#  endif /* __cplusplus */
WINUSERAPI BOOL WINAPI SetWindowTextA(HWND,LPCSTR);
WINUSERAPI BOOL WINAPI SetWindowTextW(HWND,LPCWSTR);
#  define SetWindowText WINELIB_NAME_AW(SetWindowText)
#  if defined __cplusplus
}
#  endif /* __cplusplus */
# endif /* !SetWindowText */
# ifndef ShowWindow
#  if defined __cplusplus
extern "C" {
#  endif /* __cplusplus */
WINUSERAPI BOOL WINAPI ShowWindow(HWND,INT);
#  if defined __cplusplus
}
#  endif /* __cplusplus */
# endif /* !ShowWindow */
#endif /* HAVE_WINUSER_H || (__WINESRC__ && WINDOWS_COMPATIBILITY_CHECK) */

#if defined(HAVE_WINUSER_RH) || (defined(__WINESRC__) && defined(WINDOWS_COMPATIBILITY_CHECK))
# include <winuser.rh>
#else
# ifndef WM_USER
#  define WM_USER 0x0400
# endif /* !WM_USER */
#endif /* HAVE_WINUSER_RH || (__WINESRC__ && WINDOWS_COMPATIBILITY_CHECK) */

#if defined(HAVE_COMMCTRL_H) || (defined(__WINESRC__) && defined(WINDOWS_COMPATIBILITY_CHECK))
# include <commctrl.h>
#else
/* from Wine's <commctrl.h>: */
# ifndef TB_ENABLEBUTTON
#  define TB_ENABLEBUTTON (WM_USER+1)
# endif /* TB_ENABLEBUTTON */
# ifndef TB_CHECKBUTTON
#  define TB_CHECKBUTTON (WM_USER+2)
# endif /* !TB_CHECKBUTTON */
# ifndef TB_SETCMDID
#  define TB_SETCMDID (WM_USER+42)
# endif /* !TB_SETCMDID */
#endif /* HAVE_COMMCTRL_H || (__WINESRC__ && WINDOWS_COMPATIBILITY_CHECK) */

#if defined(HAVE_WINCON_H) || (defined(__WINESRC__) && defined(WINDOWS_COMPATIBILITY_CHECK))
# include <wincon.h>
#else
/* from Wine's <wincon.h>: */
# ifndef ENABLE_PROCESSED_INPUT
#  define ENABLE_PROCESSED_INPUT 0x0001
# endif /* !ENABLE_PROCESSED_INPUT */
# ifndef ENABLE_LINE_INPUT
#  define ENABLE_LINE_INPUT 0x0002
# endif /* !ENABLE_LINE_INPUT */
#endif /* HAVE_WINCON_H || (__WINESRC__ && WINDOWS_COMPATIBILITY_CHECK) */

/* in case <fcntl.h> forgot to define these: */
#ifndef __WINE_FCNTL_H
# ifndef _O_RDWR
#  define _O_RDWR 2
# endif /* !_O_RDWR */
# ifndef _O_CREAT
#  define _O_CREAT 0x0100
# endif /* !_O_CREAT */
# ifndef _O_EXCL
#  define _O_EXCL 0x0400
# endif /* !_O_EXCL */
#endif /* !__WINE_FCNTL_H */

/* throw in a prototype: */
extern int wincompat_test_c_dummy_func(void);

#endif /* !_WINCOMPAT_H */

/* EOF */
