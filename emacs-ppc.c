/* emacs-ppc.c -*- C -*- */

#include <stdio.h>
#include <stdlib.h>

int main(void) {
	fputs("To install emacs on PPC you must run the following command\nsudo /usr/libexec/dumpemacs\n", stderr);
	exit(1);
}

/* EOF */
