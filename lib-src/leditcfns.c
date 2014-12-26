/* leditcfns.c */

#ifdef HAVE_CONFIG_H
# include "../src/config.h"
#else
# define _LEDITCFNS_C_NON_AUTOTOOLS_BUILD 1
#endif /* HAVE_CONFIG_H */

#include <sgtty.h>
#include <signal.h>
#include <stdio.h> /* for printf() */
#include <string.h> /* for strcpy() and strlen() */
#include <unistd.h> /* for getpid() */
#define STRLEN 100
static char str[STRLEN + 1] = "%?emacs"; /* extra char for the null */

/* prototypes: */
extern int switch_to_proc(void);
extern int set_proc_str(char *);

/* actual functions: */
/* first, switch_to_proc(): */
int
switch_to_proc(void)
{
    char *ptr = str;
    while (*ptr) {
        ioctl(0, TIOCSTI, ptr++);
    }
    ioctl(0, TIOCSTI, "\n");
    kill(getpid(), SIGTSTP);

    return 0;
}

/* next, set_proc_str(): */
int
set_proc_str(char *ptr)
{
    if (strlen(ptr) <= STRLEN) {
        strcpy(str, ptr);
    } else {
        printf("string too long for set-proc-str: %s\n", ptr);
    }

    return 0;
}

/* EOF */
