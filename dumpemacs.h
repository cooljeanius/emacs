/* dumpemacs.h -*- C -*- */

#ifndef kEmacsVersion
# define kEmacsVersion		"21.2"
#endif /* !kEmacsVersion */
#define kEmacsVersionMinor	"1"
#define kEmacsBinDir "/usr/bin"
#define kEmacsLibExecDir "/usr/libexec"
#define kEmacsShareDir "/usr/share/emacs"

#ifdef __ppc__
# define kEmacsArch "ppc"
#endif /* __ppc__ */
#ifdef __ppc64__
# define kEmacsArch  "ppc64"
#endif /* __ppc64__ */
#ifdef __i386__
# define kEmacsArch "i386"
#endif /* __i386__ */
#ifdef __x86_64__
# define kEmacsArch "x86_64"
#endif /* __x86_64__ */

#ifndef kEmacsArch
# error "Unsupported architecture"
#endif /* !kEmacsArch */

#ifndef kEmacsWrapperPath
# define kEmacsWrapperPath	kEmacsBinDir "/emacs"
#endif /* !kEmacsWrapperPath */
#define kEmacsDumpedPath	kEmacsBinDir "/emacs"
#define kEmacsUndumpedPath	kEmacsBinDir "/emacs-undumped"
#define kEmacsArchPath		kEmacsBinDir "/emacs-" kEmacsArch
#define kDumpEmacsPath		kEmacsLibExecDir "/dumpemacs"

/* this is taken from "src/config.h": */
/* The _Noreturn keyword of C11: */
#if !(defined(_Noreturn) \
    || (defined(__STDC_VERSION__) && (201112 <= __STDC_VERSION__)))
# if ((3 <= __GNUC__) || ((__GNUC__ == 2) && (8 <= __GNUC_MINOR__)) \
     || (0x5110 <= __SUNPRO_C))
#  define _Noreturn __attribute__ ((__noreturn__))
# elif defined(_MSC_VER) && (1200 <= _MSC_VER)
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn
# endif /* compatible compiler check */
#endif /* have _Noreturn */

int is_emacs_valid(int debugflag);
int runit(const char * const argv[], int dropprivs);

/* EOF */
