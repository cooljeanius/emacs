/* emacs-ppc.c -*- C -*- */

#include <stdio.h>
#include <stdlib.h>

#if defined(__ppc__) || defined(__ppc64__) || defined(__POWERPC__) || \
    defined(_ARCH_PPC) || defined(_ARCH_PPC64) || defined(_ARCH_PPCGR) || \
    defined(__ALTIVEC__) || defined(MAC_OS) || defined(MAC_OS8)
# include "src/m/macppc.h"
# include "src/m/powermac.h"
# include "src/m/powermacintosh.h"
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "skipping including some powermac-specific includes..."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* __ppc[64]__ || __POWERPC__ || _ARCH_PPC[64|GR] || __ALTIVEC__ || MAC_OS[8] */

/* only this so far: */
int main(void) {
	fputs("To install emacs on PPC you must run the following command\nsudo /usr/libexec/dumpemacs\n",
          stderr);
	exit(1);
}

/* EOF */
