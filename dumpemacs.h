/* dumpemacs.h -*- C -*- */

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

int is_emacs_valid(int debugflag);
int runit(const char * const argv[], int dropprivs);

/* EOF */
