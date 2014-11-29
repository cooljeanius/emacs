/* wincompat_test.c */

#include "wincompat.h"

#if defined(HAVE_STDIO_H) || defined(STDC_HEADERS) || defined(__STDC__)
# include <stdio.h>
#endif /* HAVE_STDIO_H || STDC_HEADERS || __STDC__ */

typedef int wincompat_test_c_dummy_t;

int wincompat_test_c_dummy_func(void)
{
    /* use various typedef-ed types and defined macros: */
    WORD myword = 0U;
    DWORD mydword = 0U + 1U;
    INT myint = 0 + 1 + _WIN32_WINNT;
    UINT myuint = 0U + 1U + (2U * 4U) + WM_USER;
    LONG mylong = 0L;

    int printed = printf("\n");

    if (sizeof(myword) > sizeof(mydword)) {
        if (mydword > myword) {
            return 1;
        } else {
            return 3;
        }
    } else if (sizeof(myint) > sizeof(myuint)) {
        return 2;
    } else if (!(sizeof(mylong) > sizeof(myint))) {
        return 4;
    } else {
        return 0;
    }

    /* should never get here: */
    return printed; /*NOTREACHED*/
}

#ifdef TEST

int main(void)
{
    wincompat_test_c_dummy_t ret_value = wincompat_test_c_dummy_func();

    printf("Hello world.\n");

    if (ret_value == 0) {
        return ret_value;
    } else {
        return 0;
    }
}

#endif /* TEST */

/* EOF */
